/**
 * @file
 * @author  Markus Sengthaler <m.sengthaler@gmail.com>
 * @version 1.0
 * @section LICENSE
 * This file is part of DynaVibe
 *
 * Copyright (C) 2013	Markus Sengthaler

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

#include "dmdataviewer.h"
#include "dmdatafilter.h"
#include "dmview.h"
#include "dmcomponent.h"
#include "dmsystem.h"
#include "dmlogger.h"
#include "dmderivedsystem.h"
#include <vector>
#include <dmnode.h>

using namespace DM;

bool ApplyFilter(Component* c, DataFilter* filter)
{
	Attribute *a = NULL;
	Node* n = NULL;

	if(filter->type == DataFilter::AttributeString || filter->type == DataFilter::AttributeDouble)
	{
		a = c->getAttribute(filter->attributeName);
		if(!a)
			return false;
	}
	else if (filter->type == DataFilter::Node)
	{
		n = dynamic_cast<Node*>(c);
		if(!n)
			return false;
	}
	
	double dvalue;

	if(filter->type == DataFilter::AttributeString)
	{
		if(a->getType() == Attribute::STRING && filter->op == DataFilter::EQUAL)
			return a->getString() == filter->svalue;
		else
			return false;
	}
	else if(filter->type == DataFilter::AttributeDouble)
		dvalue = a->getDouble();
	else if(filter->type == DataFilter::Node)
		dvalue = n->get(filter->coord);

	if(filter->op == DataFilter::GREATER)
		return dvalue > filter->dvalue;
	else if(filter->op == DataFilter::GREATEREQUAL)
		return dvalue >= filter->dvalue;
	else if(filter->op == DataFilter::LESS)
		return dvalue < filter->dvalue;
	else if(filter->op == DataFilter::LESSEQUAL)
		return dvalue <= filter->dvalue;
	else if(filter->op == DataFilter::EQUAL)
		return dvalue == filter->dvalue;

	return true;
}

void ApplyFilters(std::vector<Component*>& componentList, std::vector<DataFilter*> filters)
{
	if(filters.size() == 0)
		return;

	std::vector<Component*> newComponentList;
	foreach(Component* c, componentList)
	{
		bool excluded = false;
		foreach(DataFilter* filter, filters)
			if(!ApplyFilter(c, filter))
				excluded = true;

		if(!excluded)
			newComponentList.push_back(c);
	}

	componentList = newComponentList;
}

DataViewer::DataViewer(const View& view, System* owningSystem):
	currentViewDefinition(view),
	owningSystem(owningSystem),
	attributesCached(false)
{
	currentViewDefinition.clearFilters();
	update(view);
}

DataViewer::DataViewer(const DataViewer& ref, System* owningSystem):
	components(ref.components),
	currentViewDefinition(ref.currentViewDefinition),
	filteredComponents(ref.filteredComponents),
	owningSystem(owningSystem),
	attributesCached(false)
{

}

const std::vector<Component*>& DataViewer::getComponents()
{
	DerivedSystem* sys = dynamic_cast<DerivedSystem*>(owningSystem);
	if( sys != NULL && currentViewDefinition.writes())
		this->migrateAllComponents(sys);

	return filteredComponents;
}

void DataViewer::addComponent(Component* component)
{
	if(!owningSystem->hasChild(component))
	{
		switch(component->getType())
		{
		case NODE:
			owningSystem->addNode((Node*)component);
			break;
		case EDGE:
			owningSystem->addEdge((Edge*)component);
			break;
		case FACE:
			owningSystem->addFace((Face*)component);
			break;
		case RASTERDATA:
			owningSystem->addRasterData((RasterData*)component);
			break;
		case SUBSYSTEM:
			owningSystem->addSubSystem((System*)component);
			break;
		case COMPONENT:
			owningSystem->addComponent(component);
			break;
		default:
			return;
		}
	}
	components.push_back(component);

	bool isValid = true;
	foreach(DataFilter* filter, currentViewDefinition.getFilters())
		if(!ApplyFilter(component, filter))
			isValid = false;

	if(isValid)
		filteredComponents.push_back(component);
}

bool DataViewer::removeComponent(Component* component)
{
	std::vector<Component*>::iterator it = find(components.begin(), components.end(), component);
	if(it != components.end())
	{
		components.erase(it);
		filteredComponents.erase(find(filteredComponents.begin(), filteredComponents.end(), component));
		return true;
	}
	return false;
}

void DataViewer::update(const View& view)
{
	if(view.getName() != currentViewDefinition.getName())
	{
		Logger(Warning) << "DataViewer update failed";
		return;
	}
	// check if we need to completly renew filteredComponents
	bool renewFilteredComponents = false;
	foreach(DataFilter* newFilter, currentViewDefinition.getFilters())
	{
		bool isMissing = true;
		foreach(DataFilter* oldFilter, view.getFilters())
			if(*oldFilter == *newFilter)
			{
				isMissing = false;
				break;
			}
		if(isMissing)
		{
			renewFilteredComponents = true;
			break;
		}
	}
	
	this->attributesCached = true;
	if(renewFilteredComponents)
	{
		// recreate filteredComponents
		filteredComponents = components;
		ApplyFilters(filteredComponents, view.getFilters());
	}
	else
	{
		// adapt filteredComponents
		std::vector<DataFilter*> newFilters;
		foreach(DataFilter* newFilter, view.getFilters())
		{
			bool isNew = true;
			foreach(DataFilter* oldFilter, currentViewDefinition.getFilters())
				if(*oldFilter == *newFilter)
				{
					isNew = false;
					break;
				}

			if(isNew)
				newFilters.push_back(newFilter);
		}

		ApplyFilters(filteredComponents, newFilters);
	}
	// update view definition
	currentViewDefinition = view;
	// update attribute cache
	if(renewFilteredComponents)
	{
	}
}

void DataViewer::migrateComponent(const Component* src, Component* dest)
{
	if(src != dest && dest != NULL)
	{
		std::vector<Component*>::iterator it = find(components.begin(), components.end(), src);
		if(it != components.end())
			*it = dest;
		it = find(filteredComponents.begin(), filteredComponents.end(), src);
		if(it != filteredComponents.end())
			*it = dest;
	}
}

void DataViewer::migrateAllComponents(DerivedSystem* targetSystem)
{
	// copy stuff from successor
	foreach(Component* c, components)
		if(c->getCurrentSystem() != owningSystem)
			this->migrateComponent(c, targetSystem->SuccessorCopyTypesafe(c));
}

const View* DataViewer::getCurrentViewDefinition()
{
	return &currentViewDefinition;
}
