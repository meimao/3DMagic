/* 
Copyright (c) 2011 Andrew Keating

This file is part of 3DMagic.

3DMagic is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

3DMagic is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with 3DMagic.  If not, see <http://www.gnu.org/licenses/>.

*/
/** Implementation file for Batch class
 * 
 * @file Batch.cpp
 * @author Andrew Keating
 */

#include <Graphics/Batch.h>
#include <Graphics/Mesh.h>

#include <Graphics\BatchBuilder.h>

namespace Magic3D
{

/// destructor
Batch::~Batch()
{
    delete[] this->data;
	delete this->mesh;
}

void Batch::applyTransform(const Matrix4& matrix)
{
	BatchBuilder bb;
	bb.modify(this);
	float temp[3];
	bb.setCurrentVertex(0);
	for(int j=0; j < this->getVertexCount(); j++)
	{
		bb.getVertex3f( &temp[0], &temp[1], &temp[2] );
        Point3 point(temp);
		point = point.transform(matrix);
		bb.vertex3f(point.x(), point.y(), point.z());
	}
	bb.end();
}

/// names of shader variables for each of the auto-bound attribute types
const char* Batch::attributeTypeNames[ MAX_ATTRIBUTE_TYPES ] = {
    "vertex",
    "normal",
    "color",
    "color2",
    "texcoord0",
    "texcoord1",
    "texcoord2",
    "texcoord3",
    "texcoord4",
    "texcoord5",
    "texcoord6",
    "texcoord7",
    "tangent",
    "binormal"
};
    
const int Batch::attributeTypeCompCount[ MAX_ATTRIBUTE_TYPES ] = {
    4, // vertex
    3, // normal
    4, // color
    4, // color2
    2, // texcoord0
    2, // texcoord1
    2, // texcoord2
    2, // texcoord3
    2, // texcoord4
    2, // texcoord5
    2, // texcoord6
    2, // texcoord7
    3, // tangent
    3  // binormal
};


};