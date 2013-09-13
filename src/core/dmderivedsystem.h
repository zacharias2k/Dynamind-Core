/**
 * @file
 * @author  Markus Sengthaler <m.sengthaler@gmail.com>
 * @version 1.0
 * @section LICENSE
 * This file is part of DynaMinde
 *
 * Copyright (C) 2013   Markus Sengthaler

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#include "dmsystem.h"

namespace DM
{

// This system class holds a pointer to a predecessor system - for read only purpose
class DM_HELPER_DLL_EXPORT DerivedSystem: public System
{
private:
	System* predecessorSys;

	bool allComponentsLoaded;
	bool allEdgesLoaded;
	bool allFacesLoaded;
	bool allNodesLoaded;
	bool allSubSystemsLoaded;

	const Component* getComponentReadOnly(std::string uuid) const;
	const Edge* getEdgeReadOnly(Node* start, Node* end);

	Component* SuccessorCopy(const Component *src);
	Node* SuccessorCopy(const Node *src);
	Edge* SuccessorCopy(const Edge *src);
	Face* SuccessorCopy(const Face *src);
public:
	DerivedSystem(System* sys);

	Component* SuccessorCopyTypesafe(const Component *src);

	//Node* getNode(QUuid uuid);
	Component* getComponent(std::string uuid);
	Node* getNode(std::string uuid);
	Edge* getEdge(std::string uuid);
	Edge* getEdge(Node* start, Node* end);
	Face * getFace(std::string uuid);
	Component* getChild(std::string name);
	std::map<std::string, Component*> getAllComponents();
	std::map<std::string, Node*> getAllNodes();
	std::map<std::string, Edge*> getAllEdges();
	std::map<std::string, Face*> getAllFaces();
	std::map<std::string, System*> getAllSubSystems();
	std::map<std::string, RasterData*> getAllRasterData();
	//std::map<std::string, Component*> getAllComponentsInView(const DM::View & view);
};

}
