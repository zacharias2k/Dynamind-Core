/**
 * @file
 * @author  Markus Sengthaler <m.sengthaler@gmail.com>
 * @version 1.0
 * @section LICENSE
 * This file is part of DynaMite
 *
 * Copyright (C) 2011  Markus Sengthaler

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

#ifndef DMDATAFILTER_H
#define DMDATAFILTER_H

#include <string>
#include <dmcompilersettings.h>

namespace DM {
	
struct DataFilter
{
	enum CoordinateTarget
	{
		X,Y,Z,None
	};
	enum Operator
	{
		GREATER, GREATEREQUAL, LESS, LESSEQUAL, EQUAL, 
	};

	DataFilter(CoordinateTarget coord, Operator op, double value):
		attributeName(""), coord(coord), op(op), 
		svalue(), dvalue(value), type(Node)
	{}
	DataFilter(std::string attributeName, Operator op, double value):
		attributeName(attributeName), coord(None), op(op), 
		svalue(), dvalue(value), type(AttributeDouble)
	{}
	DataFilter(std::string attributeName, Operator op, std::string value):
		attributeName(attributeName), coord(None), op(op), 
		svalue(value), dvalue(0), type(AttributeString)
	{}

	bool operator == (const DataFilter& ref)
	{
		if(attributeName != ref.attributeName)	return false;
		if(coord != ref.coord)					return false;
		if(op != ref.op)						return false;
		if(dvalue != ref.dvalue)				return false;
		if(svalue != ref.svalue)				return false;

		return true;
	}

	enum FilterType
	{
		Node,
		AttributeDouble,
		AttributeString,
	} const type;
	const std::string		attributeName;
	const CoordinateTarget	coord;
	const Operator			op;
	const double			dvalue;
	const std::string		svalue;
};
}

#endif // DMDATAFILTER_H
