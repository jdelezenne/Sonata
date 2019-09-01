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
#include "Player.h"
#include "AIController.h"

Color32 GetRandomColor();
ShaderMaterial* CreateDefaultShader();

class GameWorld;

class Cursor
{
public:
	Cursor();
	void Draw();

public:
	Vector2 _Position;
	MeshPtr _mesh;
};

enum GameState
{
	GameState_Game,
	GameState_Editor
};

enum EditorState
{
	EditorState_View,
	EditorState_Tiles,
	EditorState_Actors
};

class GameWorld
{
public:
	GameWorld();

	Scene* GetScene() const { return _Scene; }

	bool GetPaused() const { return _Paused; }
	void SetPaused(bool value) { _Paused = value; }

	bool GetEditor() const { return _Editor; }
	void SetEditor(bool value);

	EditorState GetEditorState() const { return _EditorState; }
	void SetEditorState(EditorState value) { _EditorState = value; }

	int32 GetEditorDepth() const { return _EditorDepth; }
	void SetEditorDepth(int32 value) { _EditorDepth = value; }

	bool GetShowBoundingBoxes() const { return _ShowBoundingBoxes; }
	void SetShowBoundingBoxes(bool value) { _ShowBoundingBoxes = value; }

	Vector2 GetCursor() const { return _Cursor._Position; }

	Actor* GetSelectedActor() const { return _SelectedActor; }
	void SetSelectedActor(Actor* value) { _SelectedActor = value; }

	void OnToolBarTile();
	void OnToolBarActor();
	void SetCursorOnSelectedActor();

	Tileset* GetTileset() const { return _Tileset; }

	Map* GetMap() const { return _Map; }

	Player* GetPlayer() const { return _Player; }

	void LoadTileset(const String& fileName);
	void SaveTileset(const String& fileName);

	void NewMap(int32 width, int32 height, int32 depth);
	void LoadMap(const String& fileName);
	void SaveMap(const String& fileName);
	void NewGame();
	void EndGame();

	void KillPlayer();
	void EndGameRequest();

	void Create();
	void UpdateCamera();
	void Update(real64 elapsed);
	void Render();

protected:
	void CreateBoundsMesh();
	void RenderBounds(const AABB& aabb);
	void SetCursor(real32 x, real32 y);

protected:
	Scene* _Scene;
	bool _Paused;
	bool _EndGameRequest;

	Camera* _camera;
	bool _Editor;
	EditorState _EditorState;
	int32 _EditorDepth;
	bool _ShowBoundingBoxes;
	Cursor _Cursor;
	Actor* _SelectedActor;

	MeshPtr _BoundsMesh;

	Tileset* _Tileset;
	Map* _Map;

	Player* _Player;
	Array<AIController*> _AIControllers;
};

#endif 
