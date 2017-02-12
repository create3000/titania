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

#include "../../Browser/X3DBrowser.h"

#include "Nodes.h"

namespace titania {
namespace X3D {
namespace VRML1 {

Parser::Parser (const X3D::X3DScenePtr & scene, const basic::uri & uri, std::istream & istream) :
	X3D::X3DParser (),
	         scene (scene),
	           uri (uri),
	       istream (istream),
	         nodes ()
{
	nodes .emplace ("Background",         [ ] (X3D::X3DExecutionContext* const executionContext) { return new Background         (executionContext); });
	nodes .emplace ("CollideStyle",       [ ] (X3D::X3DExecutionContext* const executionContext) { return new CollideStyle       (executionContext); });
	nodes .emplace ("Cone",               [ ] (X3D::X3DExecutionContext* const executionContext) { return new Cone               (executionContext); });
	nodes .emplace ("Coordinate3",        [ ] (X3D::X3DExecutionContext* const executionContext) { return new Coordinate3        (executionContext); });
	nodes .emplace ("Cube",               [ ] (X3D::X3DExecutionContext* const executionContext) { return new Cube               (executionContext); });
	nodes .emplace ("Cylinder",           [ ] (X3D::X3DExecutionContext* const executionContext) { return new Cylinder           (executionContext); });
	nodes .emplace ("DirectedSound",      [ ] (X3D::X3DExecutionContext* const executionContext) { return new DirectedSound      (executionContext); });
	nodes .emplace ("DirectionalLight",   [ ] (X3D::X3DExecutionContext* const executionContext) { return new DirectionalLight   (executionContext); });
	nodes .emplace ("ElevationGrid",      [ ] (X3D::X3DExecutionContext* const executionContext) { return new ElevationGrid      (executionContext); });
	nodes .emplace ("Environment",        [ ] (X3D::X3DExecutionContext* const executionContext) { return new Environment        (executionContext); });
	nodes .emplace ("FontStyle",          [ ] (X3D::X3DExecutionContext* const executionContext) { return new FontStyle          (executionContext); });
	nodes .emplace ("GeneralCylinder",    [ ] (X3D::X3DExecutionContext* const executionContext) { return new GeneralCylinder    (executionContext); });
	nodes .emplace ("IndexedFaceSet",     [ ] (X3D::X3DExecutionContext* const executionContext) { return new IndexedFaceSet     (executionContext); });
	nodes .emplace ("IndexedLineSet",     [ ] (X3D::X3DExecutionContext* const executionContext) { return new IndexedLineSet     (executionContext); });
	nodes .emplace ("Info",               [ ] (X3D::X3DExecutionContext* const executionContext) { return new Info               (executionContext); });
	nodes .emplace ("LOD",                [ ] (X3D::X3DExecutionContext* const executionContext) { return new LOD                (executionContext); });
	nodes .emplace ("MaterialBinding",    [ ] (X3D::X3DExecutionContext* const executionContext) { return new MaterialBinding    (executionContext); });
	nodes .emplace ("Material",           [ ] (X3D::X3DExecutionContext* const executionContext) { return new Material           (executionContext); });
	nodes .emplace ("MatrixTransform",    [ ] (X3D::X3DExecutionContext* const executionContext) { return new MatrixTransform    (executionContext); });
	nodes .emplace ("NavigationInfo",     [ ] (X3D::X3DExecutionContext* const executionContext) { return new NavigationInfo     (executionContext); });
	nodes .emplace ("NormalBinding",      [ ] (X3D::X3DExecutionContext* const executionContext) { return new NormalBinding      (executionContext); });
	nodes .emplace ("Normal",             [ ] (X3D::X3DExecutionContext* const executionContext) { return new Normal             (executionContext); });
	nodes .emplace ("OrthographicCamera", [ ] (X3D::X3DExecutionContext* const executionContext) { return new OrthographicCamera (executionContext); });
	nodes .emplace ("PerspectiveCamera",  [ ] (X3D::X3DExecutionContext* const executionContext) { return new PerspectiveCamera  (executionContext); });
	nodes .emplace ("PointLight",         [ ] (X3D::X3DExecutionContext* const executionContext) { return new PointLight         (executionContext); });
	nodes .emplace ("PointSet",           [ ] (X3D::X3DExecutionContext* const executionContext) { return new PointSet           (executionContext); });
	nodes .emplace ("PointSound",         [ ] (X3D::X3DExecutionContext* const executionContext) { return new PointSound         (executionContext); });
	nodes .emplace ("Rotation",           [ ] (X3D::X3DExecutionContext* const executionContext) { return new Rotation           (executionContext); });
	nodes .emplace ("Scale",              [ ] (X3D::X3DExecutionContext* const executionContext) { return new Scale              (executionContext); });
	nodes .emplace ("Separator",          [ ] (X3D::X3DExecutionContext* const executionContext) { return new Separator          (executionContext); });
	nodes .emplace ("ShapeHints",         [ ] (X3D::X3DExecutionContext* const executionContext) { return new ShapeHints         (executionContext); });
	nodes .emplace ("Sphere",             [ ] (X3D::X3DExecutionContext* const executionContext) { return new Sphere             (executionContext); });
	nodes .emplace ("SpotLight",          [ ] (X3D::X3DExecutionContext* const executionContext) { return new SpotLight          (executionContext); });
	nodes .emplace ("Switch",             [ ] (X3D::X3DExecutionContext* const executionContext) { return new Switch             (executionContext); });
	nodes .emplace ("TextLanguage",       [ ] (X3D::X3DExecutionContext* const executionContext) { return new TextLanguage       (executionContext); });
	nodes .emplace ("Texture2Transform",  [ ] (X3D::X3DExecutionContext* const executionContext) { return new Texture2Transform  (executionContext); });
	nodes .emplace ("Texture2",           [ ] (X3D::X3DExecutionContext* const executionContext) { return new Texture2           (executionContext); });
	nodes .emplace ("TextureCoordinate2", [ ] (X3D::X3DExecutionContext* const executionContext) { return new TextureCoordinate2 (executionContext); });
	nodes .emplace ("Text",               [ ] (X3D::X3DExecutionContext* const executionContext) { return new Text               (executionContext); });
	nodes .emplace ("Transform",          [ ] (X3D::X3DExecutionContext* const executionContext) { return new Transform          (executionContext); });
	nodes .emplace ("Translation",        [ ] (X3D::X3DExecutionContext* const executionContext) { return new Translation        (executionContext); });
	nodes .emplace ("WorldInfo",          [ ] (X3D::X3DExecutionContext* const executionContext) { return new WorldInfo          (executionContext); });
	nodes .emplace ("WWWAnchor",          [ ] (X3D::X3DExecutionContext* const executionContext) { return new WWWAnchor          (executionContext); });
	nodes .emplace ("WWWInline",          [ ] (X3D::X3DExecutionContext* const executionContext) { return new WWWInline          (executionContext); });
}

void
Parser::parseIntoScene ()
{
	//__LOG__ << this << " " << std::endl;

	scene -> setWorldURL (uri);
	scene -> setEncoding (EncodingType::VRML);
	scene -> setProfile (getBrowser () -> getProfile ("Full"));

	istream .imbue (std::locale::classic ());

	try
	{
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
		__LOG__ << istream .rdbuf () << std::endl;
		throw;
	}
}

Parser::~Parser ()
{ }

} // Wavefront
} // X3D
} // titania
