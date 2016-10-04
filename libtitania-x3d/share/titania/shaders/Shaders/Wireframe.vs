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

uniform int x3d_GeometryType;

uniform float x3d_LinewidthScaleFactor;
uniform bool  x3d_ColorMaterial;   // true if a X3DColorNode is attached, otherwise false
uniform bool  x3d_Lighting;        // true if a X3DMaterialNode is attached, otherwise false
uniform vec3  x3d_EmissiveColor;
uniform float x3d_Transparency;

uniform mat4 x3d_ProjectionMatrix;
uniform mat4 x3d_ModelViewMatrix;

attribute vec4 x3d_Color;
attribute vec4 x3d_Vertex;

varying vec4 C; // color
varying vec3 v; // point on geometry

void
main ()
{
	// If we are points, make the gl_PointSize one pixel larger.
	gl_PointSize = x3d_GeometryType == x3d_GeometryLines ? x3d_LinewidthScaleFactor : x3d_LinewidthScaleFactor + 1.0;

	vec4 p = x3d_ModelViewMatrix * x3d_Vertex;

	v           = vec3 (p);
	gl_Position = x3d_ProjectionMatrix * p;

	if (x3d_Lighting)
	{
		float alpha = 1.0 - x3d_Transparency;

		if (x3d_ColorMaterial)
		{
			C .rgb = x3d_Color .rgb;
			C .a   = x3d_Color .a * alpha;
		}
		else
		{
			C .rgb = x3d_EmissiveColor;
			C .a   = alpha;
		}
	}
	else
	{
		if (x3d_ColorMaterial)
			C = x3d_Color;
		else
			C = vec4 (1.0, 1.0, 1.0, 1.0);
	}
}