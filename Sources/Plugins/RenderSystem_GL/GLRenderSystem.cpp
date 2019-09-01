/*=============================================================================
GLRenderSystem.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "GLRenderSystem.h"
#include "GLHelper.h"
#include "GLHardwareBuffer.h"
#include "GLTexture.h"
#include "WGLRenderContext.h"

namespace SE_GL
{

bool GL_ISENABLED(GLenum cap)
{
	GLboolean param;
	glGetBooleanv(cap, &param);
	return param != 0;
}

#define GL_ENABLE(cap, value) \
	if (value) \
		glEnable(cap); \
	else \
		glDisable(cap);

GLRenderSystem::GLRenderSystem() :
	RenderSystem(),
	_projectionTransform(Matrix4::Identity),
	_worldTransform(Matrix4::Identity)
{
	_isReady = false;
	_clearColor = Color32::Black;
	_depthValue = 1.0f;
	_stencilValue = 0xffffffff;

	_renderContext = NULL;
}

GLRenderSystem::~GLRenderSystem()
{
	Destroy();
}

void GLRenderSystem::Destroy()
{
	if (!_isReady)
		return;
}

void GLRenderSystem::SetRenderTarget(int index, RenderTarget* value)
{
	//TODO
}

Viewport GLRenderSystem::GetViewport()
{
	return _viewport;
}

void GLRenderSystem::SetViewport(const Viewport& viewport)
{
	_viewport = viewport;
	glViewport(_viewport.GetLeft(), _viewport.GetTop(), _viewport.GetWidth(), _viewport.GetHeight());
}

const Matrix4& GLRenderSystem::GetProjectionTransform()
{
	return _projectionTransform;
}

void GLRenderSystem::SetProjectionTransform(const Matrix4& value)
{
	_projectionTransform = value;

	Matrix4 matProj = Matrix4::Transpose(_projectionTransform);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(matProj.Data);
	glLoadIdentity();
	glOrtho(0, 800, 600, 0, -1.0, 1.0);
}

void GLRenderSystem::UpdateProjectionTransform()
{
	if (_camera != NULL)
	{
		_projectionTransform = _camera->GetProjection();
		Matrix4 matProj = Matrix4::Transpose(_projectionTransform);

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(matProj.Data);
		glLoadIdentity();
		glOrtho(0, 800, 600, 0, -1.0, 1.0);
	}
}

const Matrix4& GLRenderSystem::GetWorldTransform()
{
	return _worldTransform;
}

void GLRenderSystem::SetWorldTransform(const Matrix4& value)
{
	_worldTransform = value;

	Matrix4 matWorldView;
	if (_camera == NULL) 
	{
		matWorldView = Matrix4::Transpose(value);
	}
	else
	{
		Matrix4 matView = _camera->GetView();
		matWorldView = Matrix4::Transpose(matView * value);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(matWorldView.Data);
}

void GLRenderSystem::SetAmbientColor(const Color32& value)
{
	GLfloat params[] = { value.r, value.g, value.b, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, params);
}

void GLRenderSystem::SetScissorState(ScissorState state)
{
	GL_ENABLE(GL_SCISSOR_TEST, state.Enable);

	if (state.Enable)
	{
		RectangleInt clipRect = RectangleInt(
			GetViewport().GetLeft() + state.Rectangle.GetLeft(),
			GetViewport().GetTop() + GetViewport().GetHeight() - state.Rectangle.GetBottom(),
			state.Rectangle.GetWidth(), state.Rectangle.GetHeight());

		glScissor(clipRect.X, clipRect.Y, clipRect.Width, clipRect.Height);
	}
}

void GLRenderSystem::SetFogState(FogState state)
{
	GL_ENABLE(GL_FOG, state.Enable);

	GLint param;
	switch (state.VertexMode)
	{
	case FogMode_Linear: param = GL_LINEAR; break;
	case FogMode_Exponential: param = GL_EXP; break;
	case FogMode_Exponential2: param = GL_EXP2; break;
	case FogMode_None:
	default: glDisable(GL_FOG); return;
	}
	glFogi(GL_FOG_MODE, param);

	glFogf(GL_FOG_DENSITY, state.Density);
	glFogfv(GL_FOG_COLOR, state.Color);
	glFogf(GL_FOG_START, state.Start);
	glFogf(GL_FOG_END, state.End);
}

void GLRenderSystem::SetPointState(PointState state)
{
	GL_ENABLE(GL_POINT_SPRITE_ARB, state.Enable);

	if (state.Enable)
	{
		GLfloat _glPointSizeMin;
		glGetFloatv(GL_POINT_SIZE_MIN_ARB, &_glPointSizeMin);

		GLfloat _glPointSizeMax;
		glGetFloatv(GL_POINT_SIZE_MAX_ARB, &_glPointSizeMax);

		glPointSize(Math::Clamp(state.Size, _glPointSizeMin, _glPointSizeMax));
		glPointParameterfARB(GL_POINT_SIZE_MIN_ARB, Math::Max(state.MinSize, _glPointSizeMin));
		glPointParameterfARB(GL_POINT_SIZE_MAX_ARB, Math::Min(state.MaxSize, _glPointSizeMax));
		glTexEnvf(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);

		if (state.ScaleEnable)
		{
			GLfloat params[] = { state.ConstantScale, state.LinearScale, state.QuadraticScale };
			glPointParameterfvARB(GL_POINT_DISTANCE_ATTENUATION_ARB, params);
		}
		else
		{
			GLfloat params[] = { 1.0f, 0.0f, 0.0f };
			glPointParameterfvARB(GL_POINT_DISTANCE_ATTENUATION_ARB, params);
		}
	}
}

void GLRenderSystem::SetFillMode(FillMode mode)
{
	GLenum glmode;
	switch (mode)
	{
	case FillMode_Point: glmode = GL_POINT; break;
	case FillMode_WireFrame: glmode = GL_LINE; break;
	case FillMode_Solid: glmode = GL_FILL; break;
	default: glmode = GL_FILL;
	}

	glPolygonMode(GL_FRONT_AND_BACK, glmode);
}

void GLRenderSystem::SetShadeMode(ShadeMode mode)
{
	GLenum glmode;
	switch (mode)
	{
	case ShadeMode_Flat: glmode = GL_FLAT; break;
	case ShadeMode_Smooth: glmode = GL_SMOOTH; break;
	default: glmode = GL_FLAT;
	}

	glShadeModel(glmode);
}

void GLRenderSystem::SetCullMode(CullMode mode)
{
	GLenum glmode;
	switch (mode)
	{
	case CullMode_Front: glmode = GL_FRONT; break;
	case CullMode_Back: glmode = GL_BACK; break;
	case CullMode_None:
	default: glDisable(GL_CULL_FACE); return;
	}

	glEnable(glmode);
	glFrontFace(GL_CCW);
}

void GLRenderSystem::SetNormalizeNormals(bool value)
{
	GL_ENABLE(GL_NORMALIZE, value);
}

void GLRenderSystem::SetDithering(bool value)
{
	GL_ENABLE(GL_DITHER, value);
}

void GLRenderSystem::SetLighting(bool value)
{
	GL_ENABLE(GL_LIGHTING, value);
}

void GLRenderSystem::SetMaterialState(MaterialState state)
{
	if (!state.VertexColor)
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, state.AmbientColor);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, state.DiffuseColor);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, state.SpecularColor);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, state.EmissiveColor);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, state.Shininess);
	}

	GL_ENABLE(GL_COLOR_MATERIAL, state.VertexColor);
}

void GLRenderSystem::SetLightState(LightState state)
{
	int count = 0;

	if (state.Lighting)
	{
		count = Math::Min(state.Lights.Count(), 8);
		for (int i = 0; i < count; i++)
		{
			Light* light = state.Lights[i];
			GLenum index = GL_LIGHT0 + i;

			if (!light->IsEnabled())
			{
				glDisable(index);
				continue;
			}

			glLightfv(index, GL_AMBIENT, light->GetAmbientColor());
			glLightfv(index, GL_DIFFUSE, light->GetDiffuseColor());
			glLightfv(index, GL_SPECULAR, light->GetSpecularColor());

			glLightf(index, GL_CONSTANT_ATTENUATION, light->GetConstantAttenuation());
			glLightf(index, GL_LINEAR_ATTENUATION, light->GetLinearAttenuation());
			glLightf(index, GL_QUADRATIC_ATTENUATION, light->GetQuadraticAttenuation());

			if (light->GetLightType() == LightType_Directional)
			{
				DirectionalLight* dirLight = (DirectionalLight*)light;

				GLfloat direction[] = { dirLight->GetDirection().X,
					dirLight->GetDirection().Y, dirLight->GetDirection().Z, 0.0f };
				glLightfv(index, GL_POSITION, direction);
			}
			else if (light->GetLightType() == LightType_Point)
			{
				PointLight* pointLight = (PointLight*)light;

				GLfloat position[] = { pointLight->GetPosition().X,
					pointLight->GetPosition().Y, pointLight->GetPosition().Z, 1.0f };
				glLightfv(index, GL_POSITION, position);
			}

			if (light->GetLightType() == LightType_Spot)
			{
				SpotLight* spotLight = (SpotLight*)light;

				GLfloat position[] = { spotLight->GetPosition().X,
					spotLight->GetPosition().Y, spotLight->GetPosition().Z, 1.0f };
				glLightfv(index, GL_POSITION, position);

				GLfloat direction[] = { spotLight->GetDirection().X,
					spotLight->GetDirection().Y, spotLight->GetDirection().Z, 0.0f };
				glLightfv(index, GL_SPOT_DIRECTION, direction);

				glLightf(index, GL_SPOT_EXPONENT, spotLight->GetFalloffExponent());
				glLightf(index, GL_SPOT_CUTOFF, spotLight->GetCutoffAngle());
			}
			else
			{
				GLfloat direction[] = { 0.0f, 0.0f, -1.0f, 0.0f };
				glLightfv(index, GL_SPOT_DIRECTION, direction);
				glLightf(index, GL_SPOT_EXPONENT, 0.0f);
				glLightf(index, GL_SPOT_CUTOFF, 180.0f);
			}

			glEnable(index);
		}
	}

	for (int i = count; i < 8; i++)
	{
		glDisable(GL_LIGHT0 + i);
	}

	SetLighting(state.Lighting);
}

void GLRenderSystem::SetSamplerState(int stage, SamplerState state)
{
	GLenum textureType = GL_TEXTURE_2D;
	glActiveTextureARB(GL_TEXTURE0 + stage);

    glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GLHelper::GetAddressMode(state.GetAddressModeU()));
    glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GLHelper::GetAddressMode(state.GetAddressModeV()));

	glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GLHelper::GetFilterType(state.GetMagFilter()));
	glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER,
		GLHelper::GetMinFilterType(state.GetMinFilter(), state.GetMipFilter()));
}

void GLRenderSystem::SetTextureState(int stage, TextureState state)
{
	GLenum textureType = GL_TEXTURE_2D;
	glActiveTextureARB(GL_TEXTURE0 + stage);

	GLTexture* texture = (GLTexture*) state.GetTexture();
	if (texture == NULL)
	{
		glDisable(textureType);
	}
	else
	{
		glEnable(textureType);
		glBindTexture(textureType, texture->GetTextureID());

		/*glMatrixMode(GL_TEXTURE);
		Matrix4 matTexture = Matrix4::IDENTITY;
		matTexture.Translate(Vector3(state.GetOffset().X, state.GetOffset().Y, 0.0));
		matTexture.Scale(Vector3(state.GetTile().X, state.GetTile().Y, 0.0));
		glLoadMatrixf(Matrix4::Transpose(matTexture));
		glMatrixMode(GL_MODELVIEW);*/

		//
	}
}

void GLRenderSystem::DisableSamplerState(int stage)
{
	GLenum textureType = GL_TEXTURE_2D;
	glActiveTextureARB(GL_TEXTURE0 + stage);
	glDisable(textureType);
}

void GLRenderSystem::SetColorWriteEnable(ColorFlag value)
{
	GLboolean red, green, blue, alpha;
    red = (value & ColorFlag_Red) != 0;
    green = (value & ColorFlag_Green) != 0;
    blue = (value & ColorFlag_Blue) != 0;
    alpha = (value & ColorFlag_Alpha) != 0;
	glColorMask(red, green, blue, alpha);
}

void GLRenderSystem::SetDepthState(DepthState state)
{
	SetZBufferEnable(state.Enable);

	if (state.Enable)
	{
		SetZBufferWriteEnable(state.WriteEnable);
		SetZBufferFunction(state.Function);
		SetDepthBias(state.DepthBias);
	}
}

void GLRenderSystem::SetZBufferEnable(bool value)
{
	GL_ENABLE(GL_DEPTH_TEST, value);
}

void GLRenderSystem::SetZBufferWriteEnable(bool value)
{
	glDepthMask(value);
}

void GLRenderSystem::SetZBufferFunction(ComparisonFunction value)
{
	glDepthFunc(GLHelper::GetCmpFunction(value));
}

void GLRenderSystem::SetDepthBias(real32 value)
{
	if (value > 0.0f)
	{
		glEnable(GL_POLYGON_OFFSET_FILL);
		glEnable(GL_POLYGON_OFFSET_POINT);
		glEnable(GL_POLYGON_OFFSET_LINE);
		glPolygonOffset(0.0f, -value);
	}
	else
	{
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_POLYGON_OFFSET_POINT);
		glDisable(GL_POLYGON_OFFSET_LINE);
	}
}

void GLRenderSystem::SetStencilState(StencilState state)
{
	GL_ENABLE(GL_STENCIL_TEST, state.Enable);

	if (state.Enable)
	{
		glStencilOp(GLHelper::GetStencilOp(state.FrontFace.Fail),
			GLHelper::GetStencilOp(state.FrontFace.DepthBufferFail),
			GLHelper::GetStencilOp(state.FrontFace.Pass));

		glStencilFunc(GLHelper::GetCmpFunction(state.FrontFace.Function),
			state.Reference, state.ReadMask);

		glStencilMask(state.WriteMask);
	}
}

void GLRenderSystem::SetAlphaState(AlphaState state)
{
	GL_ENABLE(GL_ALPHA_TEST, state.TestEnable);
	if (state.TestEnable)
		glAlphaFunc(GLHelper::GetCmpFunction(state.Function), state.Reference / 255.0f);

	GL_ENABLE(GL_BLEND, state.BlendEnable);
	if (state.BlendEnable)
		SetBlendModes(state.SourceBlend, state.DestinationBlend);
}

void GLRenderSystem::SetBlendModes(BlendMode sourceBlend, BlendMode destinationBlend)
{
	glBlendFunc(GLHelper::GetBlendMode(sourceBlend), GLHelper::GetBlendMode(destinationBlend));
}

bool GLRenderSystem::Resize(uint32 width, uint32 height)
{
	SetViewport(Viewport(0, 0, width, height));

	// Default states
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	_isReady = true;

	return true;
}

void GLRenderSystem::Clear()
{
	glDisable(GL_SCISSOR_TEST);
	glStencilMask(GL_TRUE);
	glDepthMask(GL_TRUE);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	glClearColor(_clearColor.r, _clearColor.g, _clearColor.b, _clearColor.a);
	glClearDepth(_depthValue);
	glClearStencil(_stencilValue);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void GLRenderSystem::BeginScene()
{
	if (_camera != NULL)
	{
		// Transpose the matrices to send the components as column matrices (_00 _10 _20 _30 _01 ...)
		Matrix4 matProj = Matrix4::Transpose(_camera->GetProjection());
		Matrix4 matView = Matrix4::Transpose(_camera->GetView());

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(matProj.Data);

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(matView.Data);
	}

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void GLRenderSystem::EndScene()
{
	if (_renderContext != NULL)
		_renderContext->SwapBuffers();
}

void GLRenderSystem::GetColorBuffer(Image** image)
{
	if (image == NULL)
		return;

	//TODO
	/*
	image = new Image();
	image->Create(_Width, _Height, 0, PixelFormat::R8G8B8);
	glReadPixels(0, 0, _Width, _Height, GL_RGB, GL_UNSIGNED_BYTE, image.GetData());
	ImageHelper::Flip(image);
	*/
}

bool GLRenderSystem::CreateRenderContext(Window* window, const RenderContextDescription& desc)
{
	WGLRenderContext* renderContext = new WGLRenderContext();
	if (!renderContext->Create(window, desc))
	{
		delete renderContext;
		return false;
	}

	_renderContext = renderContext;

	SetViewport(Viewport(0, 0, renderContext->GetWidth(), renderContext->GetHeight()));

	return true;
}

bool GLRenderSystem::CreateVertexBuffer(uint32 size, HardwareBufferUsage usage, HardwareBuffer** vertexBuffer)
{
	if (vertexBuffer == NULL)
		return false;

	*vertexBuffer = new GLHardwareBuffer(size, usage);

	return true;
}

bool GLRenderSystem::CreateIndexBuffer(uint32 size, IndexBufferFormat format, HardwareBufferUsage usage, HardwareBuffer** indexBuffer)
{
	if (indexBuffer == NULL)
		return false;

	*indexBuffer = new GLHardwareBuffer(size, usage);

	return true;
}

bool GLRenderSystem::CreateVertexLayout(VertexLayout** vertexLayout)
{
	if (vertexLayout == NULL)
		return false;

	*vertexLayout = new VertexLayout();

	return true;
}

bool GLRenderSystem::UpdateVertexLayout(VertexLayout* vertexLayout)
{
	if (vertexLayout == NULL)
		return false;

	return true;
}

bool GLRenderSystem::CreateTexture(Image* image, Texture** texture)
{
	if (image == NULL)
		return false;

	if (texture == NULL)
		return false;

	*texture = new GLTexture();

	return (*texture)->Create(image);
}

bool GLRenderSystem::CreateRenderTarget(int32 width, int32 height, Texture** texture)
{
	return false;
}

void GLRenderSystem::RenderPrimitive(PrimitiveStream* primitive)
{
	VertexLayout* layout = primitive->GetVertexLayout();
	if (layout == NULL)
		return;

	if (primitive->GetVertexBuffer().GetVertexCount() == 0)
		return;

	if (primitive->IsIndexed())
	{
		if (primitive->GetPrimitiveType() == PrimitiveType_PointList)
			return;

		if (primitive->GetIndexBuffer().GetIndexCount() == 0)
			return;
	}

	const VertexBufferDescription& vbDesc = primitive->GetVertexBuffer();
	const IndexBufferDescription& ibDesc = primitive->GetIndexBuffer();

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, ((GLHardwareBuffer*)vbDesc.GetBuffer())->GetBufferID());

	uint32 size = layout->GetSize();
	VertexLayout::VertexElementList::Iterator it = layout->GetElementIterator();
	while (it.Next())
	{
		const VertexElement& element = it.Current();
		SEbyte* buffer = ((SEbyte*)(NULL) + element.GetOffset());

		switch (element.GetVertexSemantic())
		{
		case VertexSemantic_Position:
			glVertexPointer(
				VertexElement::GetTypeComponents(element.GetVertexFormat()),
				GLHelper::GetVertexFormat(element.GetVertexFormat()),
				size, buffer);
			glEnableClientState(GL_VERTEX_ARRAY);
			break;
		case VertexSemantic_Normal:
			glNormalPointer(
				GLHelper::GetVertexFormat(element.GetVertexFormat()), 
				size, buffer);
			glEnableClientState(GL_NORMAL_ARRAY);
			break;
		case VertexSemantic_Color:
			glColorPointer(4,
				GLHelper::GetVertexFormat(element.GetVertexFormat()), 
				size, buffer);
			glEnableClientState(GL_COLOR_ARRAY);
			break;
		case VertexSemantic_TextureCoordinate:
			/*for (int i=0; i<8; i++)
			{
				glClientActiveTextureARB(GL_TEXTURE0 + i);
				glTexCoordPointer(VertexElement::GetTypeComponents(element.GetVertexFormat()),
					GLHelper::GetVertexFormat(element.GetVertexFormat()), 
					size, buffer);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			}*/
			glTexCoordPointer(VertexElement::GetTypeComponents(element.GetVertexFormat()),
				GLHelper::GetVertexFormat(element.GetVertexFormat()), 
				size, buffer);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			break;
		}
	}

	GLenum mode = GLHelper::GetPrimitiveType(primitive->GetPrimitiveType());

	if (primitive->IsIndexed())
	{
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 
			((GLHardwareBuffer*)ibDesc.GetBuffer())->GetBufferID());

		GLenum indexType;
		if (ibDesc.GetIndexFormat() == IndexBufferFormat_Int16)
			indexType = GL_UNSIGNED_SHORT;
		else// if (ibDesc.GetFormat() == IndexBufferFormat_Int32)
			indexType = GL_UNSIGNED_INT;

		SEbyte* indexBuffer = ((SEbyte*)(NULL) + 0);
		glDrawElements(mode, ibDesc.GetIndexCount(), indexType, indexBuffer);
	}
	else
	{
		glDrawArrays(mode, vbDesc.GetStartVertex(), vbDesc.GetVertexCount());
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glDisable(GL_COLOR_MATERIAL);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void GLRenderSystem::DrawPoint(Pen* pen, real x, real y)
{
	DrawLine(pen, x, y, x, y);
}

void GLRenderSystem::DrawLine(Pen* pen, real x0, real y0, real x1, real y1)
{
	if (pen == NULL)
		return;

	glLineWidth(pen->Width);
	glColor4fv(pen->Color);
	glBegin(GL_LINES);
		glVertex2f(x0, y0);
		glVertex2f(x1, y1);
	glEnd();
}

void GLRenderSystem::DrawRectangle(Pen* pen, real x0, real y0, real x1, real y1)
{
	if (pen == NULL)
		return;

	glColor4fv(pen->Color);

	if (pen->Width > 0)
	{
		//glLineWidth(pen->Width);
		//glBegin(GL_LINE_LOOP);

		real32 width = (real32)pen->Width;
		pen->Width = 0;
		DrawRectangle(pen, x0, y0, x1, y0+width);
		DrawRectangle(pen, x0, y1, x1, y1-width);
		DrawRectangle(pen, x0, y0+width, x0+width, y1-width);
		DrawRectangle(pen, x1-width, y0+width, x1, y1-width);
	}
	else
	{
		glBegin(GL_TRIANGLE_FAN);
	}

	glVertex2f(x0, y0);
	glVertex2f(x1, y0);
	glVertex2f(x1, y1);
	glVertex2f(x0, y1);
	glEnd();
}

void GLRenderSystem::DrawCircle(Pen* pen, real x, real y, real radius)
{
	if (pen == NULL)
		return;

	glColor4fv(pen->Color);

	if (pen->Width > 0)
	{
		glLineWidth(pen->Width);
		glBegin(GL_LINE_LOOP);
	}
	else
	{
		glBegin(GL_TRIANGLE_FAN);
	}

	for (int i = 0; i < 16; i++)
	{
		real32 ang = i * 2 * Math::Pi / 16;
		glVertex2f(x + radius * Math::Cos(ang), y + radius * Math::Sin(ang));
	}

	glEnd();
}

void GLRenderSystem::DrawTriangle(Pen* pen, real x0, real y0, real x1, real y1, real x2, real y2)
{
	if (pen == NULL)
		return;

	glColor4fv(pen->Color);

	if (pen->Width > 0)
	{
		glLineWidth(pen->Width);
		glBegin(GL_LINE_LOOP);
	}
	else
	{
		glBegin(GL_TRIANGLE_FAN);
	}

	glVertex2f(x0, y0);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
}

void GLRenderSystem::DrawPolygon(Pen* pen, const Array<Vector2>& points)
{
	if (pen == NULL)
		return;

	int count = points.Count();
	if (count < 2)
		return;

	glColor4fv(pen->Color);

	if (pen->Width > 0)
	{
		glLineWidth(pen->Width);
		glBegin(GL_LINE_LOOP);
	}
	else
	{
		glBegin(GL_TRIANGLE_FAN);
	}

	for (int i = 0; i < count; ++i)
	{
		const Vector2& p = points[i];

		glVertex2f(p.X, p.Y);
	}

	glEnd();
}

}
