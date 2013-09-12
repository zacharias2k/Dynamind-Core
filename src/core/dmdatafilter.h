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
	
class DataFilter
{
public:
	enum CoordinateTarget
	{
		X,Y,Z,None
	};
	enum Operator
	{
		GREATER, GREATEREQUAL, LESS, LESSEQUAL,
	};

	DataFilter(CoordinateTarget coord, Operator op, double value):
		attributeName(""), coord(coord), op(op), value(value)
	{}
	DataFilter(std::string attributeName, Operator op, double value):
		attributeName(attributeName), coord(None), op(op), value(value)
	{}
	/*DataFilter(const DataFilter& ref):
		attributeName(ref.attributeName), coord(ref.coord), 
		op(ref.op), value(ref.value)
	{}*/
private:
	const std::string		attributeName;
	const CoordinateTarget	coord;
	const Operator			op;
	const double			value;
};
}

#endif // DMDATAFILTER_H
