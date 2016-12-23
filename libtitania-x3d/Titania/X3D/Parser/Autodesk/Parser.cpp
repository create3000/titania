/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraße 31a, Leipzig, Germany 2011.
 *
 * All rights reserved. Holger Seelig <holger.seelig@yahoo.de>.
 *
 * THIS IS UNPUBLISHED SOURCE CODE OF create3000.
 *
 * The copyright notice above does not evidence any actual of intended
 * publication of such source code, and is an unpublished work by create3000.
 * This material contains CONFIDENTIAL INFORMATION that is the property of
 * create3000.
 *
 * No permission is granted to copy, distribute, or create derivative works from
 * the contents of this software, in whole or in part, without the prior written
 * permission of create3000.
 *
 * NON-MILITARY USE ONLY
 *
 * All create3000 software are effectively free software with a non-military use
 * restriction. It is free. Well commented source is provided. You may reuse the
 * source in any way you please with the exception anything that uses it must be
 * marked to indicate is contains 'non-military use only' components.
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright 1999, 2016 Holger Seelig <holger.seelig@yahoo.de>.
 *
 * This file is part of the Titania Project.
 *
 * Titania is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License version 3 only, as published by the
 * Free Software Foundation.
 *
 * Titania is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License version 3 for more
 * details (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version 3
 * along with Titania.  If not, see <http://www.gnu.org/licenses/gpl.html> for a
 * copy of the GPLv3 License.
 *
 * For Silvio, Joy and Adi.
 *
 ******************************************************************************/

#include "Parser.h"

#include "../../Components/Geometry3D/IndexedFaceSet.h"
#include "../../Components/Grouping/Group.h" //
#include "../../Components/Grouping/Transform.h"
#include "../../Components/Rendering/Coordinate.h"
#include "../../Components/Rendering/Normal.h" //
#include "../../Components/Shape/Appearance.h"
#include "../../Components/Shape/Material.h"
#include "../../Components/Shape/Shape.h"
#include "../../Components/Texturing/ImageTexture.h" //
#include "../../Components/Texturing/TextureCoordinate.h" //

#include "../Filter.h"

extern "C" {

#include "lib3ds/lib3ds.h"

}

#include <fstream>
#include <unistd.h>

namespace titania {
namespace X3D {
namespace Autodesk {

static const auto rotation = Rotation4f (-1, 0, 0, math::PI <float> / 2);

Parser::Parser (const X3DScenePtr & scene, const basic::uri & uri, std::istream & istream) :
	               scene (scene),
	                 uri (uri),
	             istream (istream),
	                file (nullptr),
	           groupNode (),
	       materialNodes ()
{ }

void
Parser::parseIntoScene ()
{
	//__LOG__ << this << " " << std::endl;

	scene -> setWorldURL (uri);

	groupNode = scene -> createNode <X3D::Transform> ();

	scene -> updateNamedNode (get_name_from_uri (uri), X3D::SFNode (groupNode));
	scene -> getRootNodes () .emplace_back (groupNode);

	statements (save ());
}

std::string
Parser::save ()
{
	// Create temp file

	std::string filename       = "/tmp/titania-XXXXXX.3ds";
	const int   fileDescriptor = mkstemps (&filename [0], 4);

	if (fileDescriptor == -1)
		throw Error <INVALID_X3D> ("Couldn't create temp file for 3ds stream.");

	std::ofstream ofstream (filename);

	ofstream << istream .rdbuf ();

	ofstream .close ();

	close (fileDescriptor);

	return filename;
}

void
Parser::statements (const std::string & filename)
{
	// Parse 3ds file

	file = lib3ds_file_open (filename .c_str ());

	if (not file)
		throw Error <INVALID_X3D> ("Couldn't read temp file for 3ds stream.");

	lib3ds_file_eval (file, 0); // set current frame to 0

	materials ();
	meshes ();

	lib3ds_file_free (file);

	// Unlink temp file

	unlink (filename .c_str ());
}

void
Parser::materials ()
{
	for (size_t i = 0, size = file -> nmaterials; i < size; ++ i)
	{
		material (file -> materials [i]);
	}
}

void
Parser::material (Lib3dsMaterial* const material)
{
	const auto materialNode = scene -> createNode <X3D::Material> ();

	materialNodes .emplace_back (materialNode);

	// Set material name

	scene -> updateNamedNode (std::string (material -> name) + "Material", X3D::SFNode (materialNode));

	// Create material

	// Ambient intensity
	{
		Color3f ambientColor (material -> ambient [0], material -> ambient [1], material -> ambient [2]);

		float h, s, v;

		ambientColor .get_hsv (h, s, v);

		materialNode -> ambientIntensity () = v;
	}

	// Diffuse Color

	materialNode -> diffuseColor () = Color3f (material -> diffuse [0], material -> diffuse [1], material -> diffuse [2]);

	// Specular Color

	materialNode -> specularColor () = Color3f (material -> specular [0], material -> specular [1], material -> specular [2]);

	// Shininess
	// TODO: float material -> shin_strength;

	materialNode -> shininess () = std::min (128.0, std::pow (2, 10 * material -> shininess)) / 128;

	// Transparency

	materialNode -> transparency () = material -> transparency;

	// TODO:
	// int   self_illum_flag; /* bool */
	// float self_illum;
}

void
Parser::meshes ()
{
	for (size_t i = 0, size = file -> nmeshes; i < size; ++ i)
	{
		mesh (file -> meshes [i]);
	}
}

void
Parser::mesh (Lib3dsMesh* const mesh)
{
	// Create geometry nodes

	const auto transformNode = scene -> createNode <X3D::Transform> ();
	const auto coordNode     = scene -> createNode <X3D::Coordinate> ();

	groupNode -> children () .emplace_back (transformNode);

	// Set name

	scene -> updateNamedNode (mesh -> name, X3D::SFNode (transformNode));

	// Set transformation matrix

	const auto & matrix = mesh -> matrix;

	transformNode -> setMatrix (Matrix4d (matrix [0] [0], matrix [0] [1], matrix [0] [2], matrix [0] [3],
	                                      matrix [1] [0], matrix [1] [1], matrix [1] [2], matrix [1] [3],
	                                      matrix [2] [0], matrix [2] [1], matrix [2] [2], matrix [2] [3],
	                                      matrix [3] [0], matrix [3] [1], matrix [3] [2], matrix [3] [3]));

	// Each face can have a different material, thus we create a index for the faces.

	std::vector <std::vector <size_t>> materialFacesIndex (file -> nmaterials);

	for (size_t i = 0, size = mesh -> nfaces; i < size; ++ i)
	{
		const auto & face = mesh -> faces [i];

		materialFacesIndex [face .material] .emplace_back (i);
	}

	// Create Shape and geometry nodes

	for (size_t i = 0, size = materialFacesIndex .size (); i < size; ++ i)
	{
		const auto facesIndices = materialFacesIndex [i];

		std::map <unsigned, std::vector <size_t>> smoothingGroupFacesIndex;

		for (const auto & faceIndex : facesIndices)
		{
			const auto & face = mesh -> faces [faceIndex];

			smoothingGroupFacesIndex [face .smoothing_group] .emplace_back (faceIndex);
		}

		for (const auto & facesIndices : smoothingGroupFacesIndex)
		{
			const auto   shapeNode      = scene -> createNode <X3D::Shape> ();
			const auto   appearanceNode = scene -> createNode <X3D::Appearance> ();
			const auto & materialNode   = materialNodes [i];
			const auto   geometryNode   = scene -> createNode <X3D::IndexedFaceSet> ();
	
			transformNode -> children () .emplace_back (shapeNode);
	
			shapeNode -> appearance ()    = appearanceNode;
			shapeNode -> geometry ()      = geometryNode;
			appearanceNode -> material () = materialNode;
			geometryNode -> coord ()      = coordNode;
	
			geometryNode -> creaseAngle () = math::PI <float>;
	
			for (const auto & faceIndex : facesIndices .second)
			{
				const auto & face = mesh -> faces [faceIndex];
		
				geometryNode -> coordIndex () .emplace_back (face .index [0]);
				geometryNode -> coordIndex () .emplace_back (face .index [1]);
				geometryNode -> coordIndex () .emplace_back (face .index [2]);
				geometryNode -> coordIndex () .emplace_back (-1);
			}
		}
	}

	// Set coord point

	for (size_t i = 0, size = mesh -> nvertices; i < size; ++ i)
	{
		const auto & vertex = mesh -> vertices [i];

		coordNode -> point () .emplace_back (Vector3f (vertex [0], vertex [1], vertex [2]) * rotation);
	}
}

Parser::~Parser ()
{ }

} // Autodesk
} // X3D
} // titania
