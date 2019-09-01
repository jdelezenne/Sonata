/*=============================================================================
DiagramController.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _PROCEDURAL_DIAGRAMCONTROLLER_H_
#define _PROCEDURAL_DIAGRAMCONTROLLER_H_

#include "Common.h"

class DiagramController
{
public:
	DiagramController(UI::Diagram* diagram);

	bool OnDiagramMouseMove(Object* sender, const UI::MouseEventArgs& e);

	bool OnNodeMouseDown(Object* sender, const UI::MouseEventArgs& e);
	bool OnNodeKeyDown(Object* sender, const UI::KeyEventArgs& e);

	bool OnTablePostRender(Object* sender, const UI::DrawEventArgs& e);
	bool OnTableMouseMove(Object* sender, const UI::MouseEventArgs& e);
	bool OnTableMouseDown(Object* sender, const UI::MouseEventArgs& e);
	bool OnTableMouseUp(Object* sender, const UI::MouseEventArgs& e);

protected:
	UI::Diagram* _Diagram;
	UI::TableProperty* _LinkedProperty;
	bool _Linking;
	UI::Link* _Link;
	PointInt _Destination;
};

#endif 
