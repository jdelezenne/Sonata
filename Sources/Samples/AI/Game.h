/*=============================================================================
Game.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _GAME_H_
#define _GAME_H_

#include "Common.h"
#include "World.h"

Color32 GetRandomColor();
ShaderMaterial* CreateDefaultShader();

class GameWorld;

class GameEntity : public RefObject
{
	SE_DECLARE_CLASS(GameEntity, Object);

	SE_BEGIN_REFLECTION(GameEntity);
		SE_Field(_name, String, Public);
		SE_Field(_Position, Vector3, Public);
	SE_END_REFLECTION(GameEntity);

public:
	GameEntity();
	GameEntity(GameWorld* world);

	GameWorld* GetGameWorld() const { return _GameWorld; }
	void SetGameWorld(GameWorld* value) { _GameWorld = value; }

	String GetName() const { return _name; }
	void SetName(String value) { _name = value; }

	Mesh* GetMesh() const { return _mesh; }
	void SetMesh(Mesh* value) { _mesh = value; }

	ModelNode* GetNode() const { return _Node; }
	void SetNode(ModelNode* value) { _Node = value; }

	void Create(Mesh* mesh);

protected:
	GameWorld* _GameWorld;
	Mesh* _mesh;
	ModelNode* _Node;

	String _name;
	Vector3 _Position;
};

enum CameraMode
{
	CameraMode_Free,
	CameraMode_FirstPerson,
	CameraMode_LookAt
};

class GameWorld
{
public:
	GameWorld();

	Scene* GetScene() const { return _Scene; }

	Map* GetMap() const { return _Map; }

	Tileset* GetTileset() const { return _Tileset; }

	PointInt GetCursor() const { return _Cursor; }

	bool GetEditor() const { return _Editor; }
	void SetEditor(bool value);

	bool GetPaused() const { return _Paused; }
	void SetPaused(bool value) { _Paused = value; }

	void LoadTileset(const String& fileName);
	void SaveTileset(const String& fileName);

	void NewMap(int32 width, int32 height);
	void LoadMap(const String& fileName);
	void SaveMap(const String& fileName);

	int GetEntityCount() const;
	void AddEntity(GameEntity* entity);
	void RemoveEntity(GameEntity* entity);
	void SetControlledEntity(GameEntity* entity);

	void Create();
	void UpdateCamera();
	void Update(real64 elapsed);
	void Render();

protected:
	void SetCursor(int32 x, int32 y);

protected:
	Camera* _camera;
	Scene* _Scene;
	bool _Paused;
	bool _Editor;

	Map* _Map;
	Tileset* _Tileset;
	PointInt _Cursor;

	Unit* _SelectedUnit;
	bool _ActionRequested;

	Array<GameEntity*> _Entities;
	GameEntity* _ControlledEntity;
};

#endif 
