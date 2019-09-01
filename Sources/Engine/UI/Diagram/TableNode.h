/*=============================================================================
TableNode.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_UI_DIAGRAM_TABLENODE_H_
#define _SE_UI_DIAGRAM_TABLENODE_H_

#include "UI/Common.h"
#include "UI/Diagram/Node.h"

namespace SonataEngine
{
	namespace UI
	{
		class Link;
		class TableNode;

		enum TablePropertyType
		{
			TablePropertyType_Input,
			TablePropertyType_Output
		};

		class SE_UI_EXPORT TableProperty : public Node
		{
			SE_DECLARE_CLASS(TableProperty, Node);

			SE_BEGIN_REFLECTION(TableProperty);
			SE_END_REFLECTION(TableProperty);

		public:
			/** @name Constructor / Destructor. */
			//@{
			/** Constructor. */
			TableProperty();

			/** Destructor. */
			virtual ~TableProperty();
			//@}

			/** @name Properties. */
			//@{
			TableNode* GetOwner() const { return _Owner; }
			void SetOwner(TableNode* value) { _Owner = value; }

			TablePropertyType GetPropertyType() const { return _PropertyType; }
			void SetPropertyType(TablePropertyType value) { _PropertyType = value; }

			/** Name of the property. */
			const String& GetPropertyName() const { return _PropertyName; }
			void SetPropertyName(const String& value) { _PropertyName = value; }
			//@}

			/** @name Operations. */
			//@{
			/** Perform the update of the widget. */
			virtual void DoUpdate(real64 elapsed);

			/** Perform the rendering of the widget. */
			virtual void DoRender();
			//@}

		protected:
			TableNode* _Owner;
			TablePropertyType _PropertyType;
			String _PropertyName;

			TextPtr _TextObj;
		};

		enum TablePreviewType
		{
			TablePreviewType_Text,
			TablePreviewType_Image
		};

		class TableNode;

		class SE_UI_EXPORT TablePreview
		{
		public:
			TablePreview(TableNode* owner);
			virtual ~TablePreview();

			virtual TablePreviewType GetPreviewType() const { return _PreviewType; }
			void SetPreviewType(TablePreviewType value) { _PreviewType = value; }
			void SetPosition(const PointInt& value) { _Position = value; }
			virtual int32 GetWidth() const;
			virtual int32 GetHeight() const;
			virtual void Update(real64 elapsed);
			virtual void Render();

			const String& GetText() const { return _Text; }
			void SetText(const String& value) { _Text = value; }

			Image* GetImage() const { return _Image; }
			void SetImage(Image* value);

		protected:
			TableNode* _Owner;
			TablePreviewType _PreviewType;
			String _Text;
			Image* _Image;

			PointInt _Position;
			SizeInt _Size;
			TextPtr _TextObj;
			Texture* _texture;
		};

		/**
			@brief Node representing a table.
		*/
		class SE_UI_EXPORT TableNode : public Node
		{
			SE_DECLARE_CLASS(TableNode, Node);

			SE_BEGIN_REFLECTION(TableNode);
			SE_END_REFLECTION(TableNode);

		public:
			typedef Array<TableProperty*> TablePropertyList;

			/** @name Constructor / Destructor. */
			//@{
			/** Constructor. */
			TableNode();

			/** Destructor. */
			virtual ~TableNode();
			//@}

			/** @name Properties. */
			//@{
			/** Gets or sets the title of the table. */
			bool GetShowTitle() const { return _ShowTitle; }
			void SetShowTitle(bool value) { _ShowTitle = value; }

			bool GetShowPreview() const { return _ShowPreview; }
			void SetShowPreview(bool value) { _ShowPreview = value; }

			bool GetShowProperties() const { return _ShowProperties; }
			void SetShowProperties(bool value) { _ShowProperties = value; }

			String GetTitle() const { return _Title; }
			void SetTitle(String value) { _Title = value; }

			TablePreview* GetPreview() const { return _Preview; }
			void SetPreview(TablePreview* value) { _Preview = value; }

			/** List of input properties belonging to the table. */
			TablePropertyList& InputProperties() { return _InputProperties; }

			/** List of input properties belonging to the table. */
			TablePropertyList& OutputProperties() { return _OutputProperties; }
			//@}

			/** @name Operations. */
			//@{
			/** Perform the update of the widget. */
			virtual void DoUpdate(real64 elapsed);

			/** Perform the rendering of the widget. */
			virtual void DoRender();
			//@}

			/** @name Events. */
			//@{
			//@}

		protected:
			bool _ShowTitle;
			bool _ShowPreview;
			bool _ShowProperties;
			String _Title;
			TablePreview* _Preview;
			TablePropertyList _InputProperties;
			TablePropertyList _OutputProperties;

			TextPtr _TextObj;
		};
	}
}

#endif 
