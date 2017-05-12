// -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
//
//  DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
// 
//  Copyright create3000, Scheffelstraße 31a, Leipzig, Germany 2011.
// 
//  All rights reserved. Holger Seelig <holger.seelig@yahoo.de>.
// 
//  The copyright notice above does not evidence any actual of intended
//  publication of such source code, and is an unpublished work by create3000.
//  This material contains CONFIDENTIAL INFORMATION that is the property of
//  create3000.
// 
//  No permission is granted to copy, distribute, or create derivative works from
//  the contents of this software, in whole or in part, without the prior written
//  permission of create3000.
// 
//  NON-MILITARY USE ONLY
// 
//  All create3000 software are effectively free software with a non-military use
//  restriction. It is free. Well commented source is provided. You may reuse the
//  source in any way you please with the exception anything that uses it must be
//  marked to indicate is contains 'non-military use only' components.
// 
//  DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
// 
//  Copyright 2015, 2016 Holger Seelig <holger.seelig@yahoo.de>.
// 
//  This file is part of the Cobweb Project.
// 
//  Cobweb is free software: you can redistribute it and/or modify it under the
//  terms of the GNU General Public License version 3 only, as published by the
//  Free Software Foundation.
// 
//  Cobweb is distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
//  A PARTICULAR PURPOSE. See the GNU General Public License version 3 for more
//  details (a copy is included in the LICENSE file that accompanied this code).
// 
//  You should have received a copy of the GNU General Public License version 3
//  along with Cobweb.  If not, see <http://www.gnu.org/licenses/gpl.html> for a
//  copy of the GPLv3 License.
// 
//  For Silvio, Joy and Adi.


precision mediump float;

uniform mat4 x3d_TextureMatrix [x3d_MaxTextures];
uniform mat3 x3d_NormalMatrix;
uniform mat4 x3d_ProjectionMatrix;
uniform mat4 x3d_ModelViewMatrix;

uniform float x3d_LinewidthScaleFactor;
uniform bool  x3d_Lighting;  // true if a X3DMaterialNode is attached, otherwise false

attribute vec4 x3d_Color;
attribute vec4 x3d_TexCoord;
attribute vec3 x3d_Normal;
attribute vec4 x3d_Vertex;

varying vec4 C;  // color
varying vec4 t;  // texCoord
varying vec3 vN; // normalized normal vector at this point on geometry
varying vec3 v;  // point on geometry

void
main ()
{
	gl_PointSize = x3d_LinewidthScaleFactor;

	vec4 p = x3d_ModelViewMatrix * x3d_Vertex;

	if (x3d_Lighting)
		vN = normalize (x3d_NormalMatrix * x3d_Normal);

	t = x3d_TextureMatrix [0] * x3d_TexCoord;
	C = x3d_Color;
	v = p .xyz;

	gl_Position = x3d_ProjectionMatrix * p;
}