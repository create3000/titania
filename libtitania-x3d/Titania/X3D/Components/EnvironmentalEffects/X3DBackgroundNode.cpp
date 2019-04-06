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

#include "X3DBackgroundNode.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/BindableNodeList.h"
#include "../../Execution/BindableNodeStack.h"

#include "../Layering/X3DLayerNode.h"
#include "../Navigation/X3DViewpointNode.h"
#include "../Shaders/ComposedShader.h"

#include <complex>

namespace titania {
namespace X3D {

//static constexpr float U_DIMENSION = 16;// LOW
//static constexpr float U_DIMENSION = 20; // MEDIUM
static constexpr float U_DIMENSION = 24;// HIGH
static constexpr float RADIUS      = 1;

const std::vector <Matrix4d> X3DBackgroundNode::cubeRotations = {
	Matrix4d (),                                       // front
	Matrix4d (Rotation4d (0, 1, 0,  pi <double>)),     // back
	Matrix4d (Rotation4d (0, 1, 0,  pi <double> / 2)), // left
	Matrix4d (Rotation4d (0, 1, 0, -pi <double> / 2)), // right
	Matrix4d (Rotation4d (1, 0, 0,  pi <double> / 2)), // top
	Matrix4d (Rotation4d (1, 0, 0, -pi <double> / 2)), // bottom
};

const std::vector <Vector4f> X3DBackgroundNode::cubeTexCoords = {
	// Triangle 1
	Vector4f (1, 1, 0, 1),
	Vector4f (0, 1, 0, 1),
	Vector4f (0, 0, 0, 1),
	// Triangle 2
	Vector4f (1, 1, 0, 1),
	Vector4f (0, 0, 0, 1),
	Vector4f (1, 0, 0, 1),
};

const std::vector <Vector3d> X3DBackgroundNode::cubeVertices = {
	// Triangle 1
	Vector3d ( 1,  1, -1),
	Vector3d (-1,  1, -1),
	Vector3d (-1, -1, -1),
	// Triangle 2
	Vector3d ( 1,  1, -1),
	Vector3d (-1, -1, -1),
	Vector3d ( 1, -1, -1),
};

const double X3DBackgroundNode::cubeScale = std::sqrt (RADIUS * RADIUS / 2);

X3DBackgroundNode::Fields::Fields () :
	 groundAngle (new MFFloat ()),
	 groundColor (new MFColor ()),
	    skyAngle (new MFFloat ()),
	    skyColor (new MFColor ({ SFColor () })),
	transparency (new SFFloat ())
{ }

X3DBackgroundNode::X3DBackgroundNode () :
	     X3DBindableNode (),
	              fields (),
	            textures (6),
	              hidden (false),
	          clipPlanes (),
	         modelMatrix (),
	        sphereColors (),
	      sphereVertices (),
	 sphereColorBufferId (0),
	sphereVertexBufferId (0),
	cubeTexCoordBufferId (0),
	  cubeVertexBufferId (0)
{
	addType (X3DConstants::X3DBackgroundNode);

	addChildObjects (textures);
	
	skyAngle ()    .setUnit (UnitCategory::ANGLE);
	groundAngle () .setUnit (UnitCategory::ANGLE);
}

void
X3DBackgroundNode::initialize ()
{
	X3DBindableNode::initialize ();

	addInterest (&X3DBackgroundNode::build, this);

	// Generate buffers.

	glGenBuffers (1, &sphereColorBufferId);
	glGenBuffers (1, &sphereVertexBufferId);
	glGenBuffers (1, &cubeTexCoordBufferId);
	glGenBuffers (1, &cubeVertexBufferId);

	// Transfer cube data.

	glBindBuffer (GL_ARRAY_BUFFER, cubeTexCoordBufferId);
	glBufferData (GL_ARRAY_BUFFER, sizeof (Vector4d) * cubeTexCoords .size (), cubeTexCoords .data (), GL_STATIC_COPY);

	glBindBuffer (GL_ARRAY_BUFFER, cubeVertexBufferId);
	glBufferData (GL_ARRAY_BUFFER, sizeof (Vector3d) * cubeVertices .size (), cubeVertices .data (), GL_STATIC_COPY);

	glBindBuffer (GL_ARRAY_BUFFER, 0);

	// Build sphere.

	build ();
}

void
X3DBackgroundNode::isHidden (const bool value)
{
	hidden = value;

	getBrowser () -> addEvent ();
}

bool
X3DBackgroundNode::isTransparent () const
{
	if (hidden)
		return true;

	if (transparency () == 0.0f)
		return false;

	for (const auto & texture : textures)
	{
		if (not texture or texture -> isTransparent ())
			return true;
	}

	return false;
}

void
X3DBackgroundNode::bindToLayer (X3DLayerNode* const layer)
{
	layer -> getBackgroundStack () -> pushOnTop (this);
}

void
X3DBackgroundNode::unbindFromLayer (X3DLayerNode* const layer)
{
	layer -> getBackgroundStack () -> pop (this);
}

void
X3DBackgroundNode::removeFromLayer (X3DLayerNode* const layer)
{
	layer -> getBackgroundStack () -> erase (this);
}

Color3f
X3DBackgroundNode::getColor (const float theta, const MFColor & color, const MFFloat & angle)
{
	const auto iter  = std::upper_bound (angle .cbegin (), angle .cend (), theta);
	const auto index = iter - angle .cbegin ();

	return color [index];
}

void
X3DBackgroundNode::buildHalfSphere (const double radius, const std::vector <double> & vAngle, const MFFloat & angle, const MFColor & color, const float opacity, const bool bottom)
{
	// p1 --- p4
	//  |     |
	//  |     |
	// p2 --- p3

	double phi = 0;

	std::complex <double> y;
	Vector3f              p;
	
	const auto    vAngleMax   = bottom ? pi_2 <double> : pi <double>;
	const int32_t V_DIMENSION = vAngle .size () - 1;

	for (int32_t v = 0; v < V_DIMENSION; ++ v)
	{
		auto theta1 = std::clamp <double> (vAngle [v],     0, vAngleMax);
		auto theta2 = std::clamp <double> (vAngle [v + 1], 0, vAngleMax);

		if (bottom)
		{
			theta1 = pi <double> - theta1;
			theta2 = pi <double> - theta2;
		}

		const auto z1 = std::polar (radius, theta1);
		const auto z2 = std::polar (radius, theta2);

		const auto c1 = getColor (vAngle [v],     color, angle);
		const auto c2 = getColor (vAngle [v + 1], color, angle);

		for (size_t u = 0; u < U_DIMENSION; ++ u)
		{
			// The last point is the first one.
			const size_t u1 = u < U_DIMENSION - 1 ? u + 1 : 0;

			// p1
			phi = pi2 <double> * (u / U_DIMENSION);
			y   = std::polar (-z1 .imag (), phi);

			sphereColors   .emplace_back (c1 .r (), c1 .g (), c1 .b (), opacity);
			sphereVertices .emplace_back (y .imag (), z1 .real (), y .real ());

			// p2
			y = std::polar (-z2 .imag (), phi);

			sphereColors   .emplace_back (c2 .r (), c2 .g (), c2 .b (), opacity);
			sphereVertices .emplace_back (y .imag (), z2 .real (), y .real ());

			// p3
			phi = pi2 <double> * (u1 / U_DIMENSION);
			y   = std::polar (-z2 .imag (), phi);

			sphereColors   .emplace_back (c2 .r (), c2 .g (), c2 .b (), opacity);
			sphereVertices .emplace_back (y .imag (), z2 .real (), y .real ());

			// p4
			y = std::polar (-z1 .imag (), phi);

			sphereColors   .emplace_back (c1 .r (), c1 .g (), c1 .b (), opacity);
			sphereVertices .emplace_back (y .imag (), z1 .real (), y .real ());
		}
	}
}

void
X3DBackgroundNode::build ()
{
	sphereColors   .clear ();
	sphereVertices .clear ();

	if (transparency () >= 1.0f)
		return;

	const float opacity = 1 - std::clamp <float> (transparency (), 0, 1);

	if (groundColor () .empty () and skyColor () .size () == 1)
	{
		// Build cube

		const double    s = std::sqrt (RADIUS * RADIUS / 2);
		const Color3f & c = skyColor () [0];

		// Back
		sphereVertices .emplace_back (-s,  s, -s);
		sphereVertices .emplace_back ( s,  s, -s);
		sphereVertices .emplace_back ( s, -s, -s);
		sphereVertices .emplace_back (-s, -s, -s);

		// Front
		sphereVertices .emplace_back ( s,  s,  s);
		sphereVertices .emplace_back (-s,  s,  s);
		sphereVertices .emplace_back (-s, -s,  s);
		sphereVertices .emplace_back ( s, -s,  s);

		// Left
		sphereVertices .emplace_back (-s,  s,  s);
		sphereVertices .emplace_back (-s,  s, -s);
		sphereVertices .emplace_back (-s, -s, -s);
		sphereVertices .emplace_back (-s, -s,  s);

		// Right
		sphereVertices .emplace_back (s,  s, -s);
		sphereVertices .emplace_back (s,  s,  s);
		sphereVertices .emplace_back (s, -s,  s);
		sphereVertices .emplace_back (s, -s, -s);

		// Top
		sphereVertices .emplace_back (-s,  s,  s);
		sphereVertices .emplace_back ( s,  s,  s);
		sphereVertices .emplace_back ( s,  s, -s);
		sphereVertices .emplace_back (-s,  s, -s);

		// Bottom
		sphereVertices .emplace_back ( s, -s,  s);
		sphereVertices .emplace_back (-s, -s,  s);
		sphereVertices .emplace_back (-s, -s, -s);
		sphereVertices .emplace_back ( s, -s, -s);

		for (size_t i = 0, size = sphereVertices .size (); i < size; ++ i)
			sphereColors .emplace_back (c .r (), c .g (), c .b (), opacity);
	}
	else
	{
		// Build sphere

		if (skyColor () .size () > skyAngle () .size ())
		{
			std::vector <double> vAngle (skyAngle () .begin (), skyAngle () .end ());

			if (vAngle .empty () or vAngle .front () > 0)
				vAngle .insert (vAngle .begin (), 0);

			const auto vAngleMax = groundColor () .size () > groundAngle () .size () ? pi_2 <double> : pi <double>;

			if (vAngle .back () < vAngleMax)
				vAngle .emplace_back (vAngleMax);

			buildHalfSphere (RADIUS, vAngle, skyAngle (), skyColor (), opacity, false);
		}

		if (groundColor () .size () > groundAngle () .size ())
		{
			std::vector <double> vAngle (groundAngle () .crbegin (), groundAngle () .crend ());

			if (vAngle .empty () or vAngle .front () < pi_2 <double>)
				vAngle .insert (vAngle .begin (), pi_2 <double>);

			if (vAngle .back () > 0)
				vAngle .emplace_back (0);

			buildHalfSphere (RADIUS, vAngle, groundAngle (), groundColor (), opacity, true);
		}
	}

	// Transfer sphere data.

	glBindBuffer (GL_ARRAY_BUFFER, sphereColorBufferId);
	glBufferData (GL_ARRAY_BUFFER, sizeof (Color4f) * sphereColors .size (), sphereColors .data (), GL_STATIC_COPY);

	glBindBuffer (GL_ARRAY_BUFFER, sphereVertexBufferId);
	glBufferData (GL_ARRAY_BUFFER, sizeof (Vector3d) * sphereVertices .size (), sphereVertices .data (), GL_STATIC_COPY);
}

void
X3DBackgroundNode::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	switch (type)
	{
		case TraverseType::CAMERA:
		{
			renderObject -> getLayer () -> getBackgrounds () -> pushBack (this);
			
			modelMatrix = renderObject -> getModelViewMatrix () .get ();
			break;
		}
		case TraverseType::DISPLAY:
		{
			clipPlanes = renderObject -> getClipPlanes ();
			break;
		}
		default:
			break;
	}
}

void
X3DBackgroundNode::draw (X3DRenderObject* const renderObject, const Vector4i & viewport)
{
	try
	{
		const auto browser = renderObject -> getBrowser ();

		if (browser -> getAlphaChannel () .top ())
			return;

		if (hidden)
			return;

		PolygonModeLock polygonMode (GL_FILL);

		// Get background scale.
		// Use render object projection matrix for generated cub map texture.

		const auto farValue = -ViewVolume::unProjectPoint (0, 0, 1, inverse (renderObject -> getProjectionMatrix () .get ()), viewport) .z () * 0.8;

		// Rotate and scale background.

		Vector3d   translation;
		Rotation4d rotation;

		auto modelViewMatrix = modelMatrix;

		modelViewMatrix .mult_right (renderObject -> getInverseCameraSpaceMatrix () .get ());
		modelViewMatrix .get (translation, rotation);
		modelViewMatrix .set (Vector3d (), rotation, Vector3d (farValue, farValue, farValue));

		renderObject -> getModelViewMatrix () .push (modelViewMatrix);

		// Draw background sphere and texture cube.

		drawSphere (renderObject);
		drawCube   (renderObject);

		renderObject -> getModelViewMatrix () .pop ();
	}
	catch (const std::domain_error &)
	{ }
}

void
X3DBackgroundNode::drawSphere (X3DRenderObject* const renderObject)
{
	if (transparency () >= 1.0f)
		return;

	if (sphereVertices .empty ())
		return;

	const auto   browser    = renderObject -> getBrowser ();
	const auto & shaderNode = browser -> getBackgroundSphereShader ();

	// Draw

	glDisable (GL_DEPTH_TEST);
	glDepthMask (GL_FALSE);

	glEnable (GL_CULL_FACE);
	glFrontFace (GL_CW);

	if (transparency ())
		glEnable (GL_BLEND);
	else
		glDisable (GL_BLEND);

	shaderNode -> enable ();

	// Clip planes

	shaderNode -> setClipPlanes (browser, clipPlanes);

	// Enable vertex attribute arrays.

	shaderNode -> enableColorAttrib  (sphereColorBufferId,  GL_FLOAT,  0, nullptr);
	shaderNode -> enableVertexAttrib (sphereVertexBufferId, GL_DOUBLE, 0, nullptr);

	// Uniforms

	if (shaderNode -> isExtensionGPUShaderFP64Available ())
	{
		glUniformMatrix4dv (shaderNode -> getProjectionMatrixUniformLocation (), 1, false, renderObject -> getProjectionMatrix () .get () .front () .data ());
		glUniformMatrix4dv (shaderNode -> getModelViewMatrixUniformLocation (),  1, false, renderObject -> getModelViewMatrix ()  .get () .front () .data ());
	}
	else
	{
		glUniformMatrix4fv (shaderNode -> getProjectionMatrixUniformLocation (), 1, false, Matrix4f (renderObject -> getProjectionMatrix () .get ()) .front () .data ());
		glUniformMatrix4fv (shaderNode -> getModelViewMatrixUniformLocation (),  1, false, Matrix4f (renderObject -> getModelViewMatrix ()  .get ()) .front () .data ());
	}

	// Draw.

	glDrawArrays (GL_QUADS, 0, sphereVertices .size ());

	// Disable vertex attribute arrays.

	shaderNode -> disableColorAttrib ();
	shaderNode -> disableVertexAttrib ();
	shaderNode -> disable ();

	glDisable (GL_BLEND);
	glDepthMask (GL_TRUE);
	glEnable (GL_DEPTH_TEST);
}

void
X3DBackgroundNode::drawCube (X3DRenderObject* const renderObject)
{
	const auto   browser    = renderObject -> getBrowser ();
	const auto & shaderNode = browser -> getGouraudShader ();

	// GL

	glDisable (GL_DEPTH_TEST);
	glDepthMask (GL_FALSE);

	glEnable (GL_CULL_FACE);
	glFrontFace (GL_CCW);

	shaderNode -> enable ();

	// Clip planes

	shaderNode -> setClipPlanes (browser, clipPlanes);

	// Uniforms

	glUniform1i (shaderNode -> getFogTypeUniformLocation (),                            0);
	glUniform1i (shaderNode -> getColorMaterialUniformLocation (),                      false);
	glUniform1i (shaderNode -> getLightingUniformLocation (),                           false);
	glUniform1i (shaderNode -> getNumTexturesUniformLocation (),                        1);
	glUniform1i (shaderNode -> getTextureCoordinateGeneratorModeUniformLocation () [0], 0);

	// ProjectionMatrix
	// TexureMatrix

	if (shaderNode -> isExtensionGPUShaderFP64Available ())
	{
		glUniformMatrix4dv (shaderNode -> getProjectionMatrixUniformLocation (),  1, false, renderObject -> getProjectionMatrix () .get () .front () .data ());
		glUniformMatrix4dv (shaderNode -> getTextureMatrixUniformLocation () [0], 1, false, Matrix4d () .front () .data ());
	}
	else
	{
		glUniformMatrix4fv (shaderNode -> getProjectionMatrixUniformLocation (),  1, false, Matrix4f (renderObject -> getProjectionMatrix () .get ()) .front () .data ());
		glUniformMatrix4fv (shaderNode -> getTextureMatrixUniformLocation () [0], 1, false, Matrix4f () .front () .data ());
	}

	// ModelViewMatrix

	renderObject -> getModelViewMatrix () .push ();
	renderObject -> getModelViewMatrix () .scale (Vector3d (cubeScale, cubeScale, cubeScale));

	// Attributes

	shaderNode -> enableTexCoordAttrib ({ cubeTexCoordBufferId }, GL_FLOAT, { }, { });
	shaderNode -> enableVertexAttrib (cubeVertexBufferId, GL_DOUBLE, 0, nullptr);

	for (size_t i = 0; i < 6; ++ i)
	{
		const auto & texture = textures [i];

		if (not texture)
			continue;

		if (texture -> checkLoadState () not_eq COMPLETE_STATE)
			continue;

		if (texture -> isTransparent ())
			glEnable (GL_BLEND);
		else
			glDisable (GL_BLEND);

		renderObject -> getModelViewMatrix () .push ();
		renderObject -> getModelViewMatrix () .mult_left (cubeRotations [i]);

		if (shaderNode -> isExtensionGPUShaderFP64Available ())
			glUniformMatrix4dv (shaderNode -> getModelViewMatrixUniformLocation (), 1, false, renderObject -> getModelViewMatrix () .get () .front () .data ());
		else
			glUniformMatrix4fv (shaderNode -> getModelViewMatrixUniformLocation (), 1, false, Matrix4f (renderObject -> getModelViewMatrix () .get ()) .front () .data ());

		texture -> setShaderUniforms (shaderNode);

		glDrawArrays (GL_TRIANGLES, 0, cubeVertices .size ());

		renderObject -> getModelViewMatrix () .pop ();
	}

	shaderNode -> disableTexCoordAttrib ();
	shaderNode -> disableVertexAttrib ();
	shaderNode -> disable ();

	renderObject -> getModelViewMatrix () .pop ();

	glDisable (GL_TEXTURE_2D);

	glDisable (GL_BLEND);
	glDisable (GL_TEXTURE_2D);

	glDepthMask (GL_TRUE);
	glEnable (GL_DEPTH_TEST);
}

void
X3DBackgroundNode::dispose ()
{
	try
	{
		ContextLock lock (getBrowser ());

		if (sphereColorBufferId)
			glDeleteBuffers (1, &sphereColorBufferId);

		if (sphereVertexBufferId)
			glDeleteBuffers (1, &sphereVertexBufferId);

		if (cubeTexCoordBufferId)
			glDeleteBuffers (1, &cubeTexCoordBufferId);

		if (cubeVertexBufferId)
			glDeleteBuffers (1, &cubeVertexBufferId);
	}
	catch (const Error <INVALID_OPERATION_TIMING> &)
	{ }

	X3DBindableNode::dispose ();
}

X3DBackgroundNode::~X3DBackgroundNode ()
{ }

} // X3D
} // titania
