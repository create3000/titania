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

#ifndef __TITANIA_X3D_PARSER_WAVEFRONT_PARSER_H__
#define __TITANIA_X3D_PARSER_WAVEFRONT_PARSER_H__

#include "../../Execution/X3DScene.h"
#include "../../Parser/X3DParser.h"

namespace titania {
namespace X3D {

class TextureCoordinate;
class Normal;
class Coordinate;
class IndexedFaceSet;
class Shape;
class Material;
class ImageTexture;

namespace Wavefront {

class Parser :
	public X3D::X3DParser
{
public:

	///  @name Construction

	Parser (const X3D::X3DScenePtr &, const basic::uri &, std::istream &);

	///  @name Operations

	virtual
	void
	parseIntoScene () final override;

	///  @name Destruction

	virtual
	~Parser ();


private:

	///  @name Operations

	void
	comments ();

	bool
	comment ();

	void
	whiteSpaces ();

	void
	whiteSpacesNoLineTerminator ();

	void
	statements ();

	bool
	statement ();

	bool
	mtllib ();

	bool
	usemtl ();

	bool
	o ();

	bool
	g ();

	bool
	s ();

	bool
	vts ();

	bool
	vt ();

	bool
	vns ();

	bool
	vn ();

	bool
	vs ();

	bool
	v ();

	bool
	fs ();

	bool
	f ();

	bool
	indices ();

	int32_t
	getIndex (const int32_t, const int32_t);

	bool
	Int32 (int32_t &);

	bool
	Vec2f (Vector2f &);

	bool
	Vec3f (Vector3f &);

	///  @name Members

	const X3D::X3DScenePtr scene;
	const basic::uri       uri;
	std::istream &         istream;

	size_t                    lineNumber;
	std::string               whiteSpaceCharacters;
	std::string               commentCharacters;
	std::vector <std::string> currentComments;

	X3DPtr <Material>          defaultMaterial;
	X3DPtr <Material>          material;
	X3DPtr <ImageTexture>      texture;
	X3DPtr <TextureCoordinate> texCoord;
	X3DPtr <Normal>            normal;
	X3DPtr <Coordinate>        coord;
	X3DPtr <IndexedFaceSet>    geometry;
	X3DPtr <Shape>             shape;
	X3DPtr <Transform>         group;
	X3DPtr <Transform>         object;

	int32_t                                                    smoothingGroup;
	std::map <std::string, std::map <int32_t, X3DPtr <Shape>>> smoothingGroups;

	std::map <std::string, X3DPtr <Material>>     materials;
	std::map <std::string, X3DPtr <ImageTexture>> textures;

};

} // Wavefront
} // X3D
} // titania

#endif
