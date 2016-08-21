/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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
 * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
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

#ifndef __TITANIA_X3D_COMPONENTS_SHADERS_X3DPROGRAMMABLE_SHADER_OBJECT_H__
#define __TITANIA_X3D_COMPONENTS_SHADERS_X3DPROGRAMMABLE_SHADER_OBJECT_H__

#include "../../Basic/X3DBaseNode.h"
#include "../../Basic/X3DConstants.h"
#include "../../Fields.h"

#include "../../Rendering/OpenGL.h"

namespace titania {
namespace X3D {

class ShapeContainer;

class X3DProgrammableShaderObject :
	virtual public X3DBaseNode
{
public:

	///  @name User defined fields handling

	virtual
	bool
	canUserDefinedFields () const final override
	{ return true; }

	virtual
	void
	addUserDefinedField (const AccessType, const std::string &, X3DFieldDefinition* const)
	throw (Error <INVALID_NAME>,
	       Error <INVALID_FIELD>,
	       Error <DISPOSED>) override;

	virtual
	void
	removeUserDefinedField (const std::string &)
	throw (Error <DISPOSED>) override;

	///  @name Member access

	virtual
	GLuint
	getProgramId () const = 0;

	///  @name Uniform location access lighting

	GLint
	getLightingUniformLocation () const
	{ return x3d_Lighting; }

	const std::vector <GLint> &
	getLightTypeUniformLocation () const
	{ return x3d_LightType; }

	const std::vector <GLint> &
	getLightColorUniformLocation () const
	{ return x3d_LightColor; }

	const std::vector <GLint> &
	getLightIntensityUniformLocation () const
	{ return x3d_LightIntensity; }

	const std::vector <GLint> &
	getLightAmbientIntensityUniformLocation () const
	{ return x3d_LightAmbientIntensity; }

	const std::vector <GLint> &
	getLightAttenuationUniformLocation () const
	{ return x3d_LightAttenuation; }

	const std::vector <GLint> &
	getLightLocationUniformLocation () const
	{ return x3d_LightLocation; }

	const std::vector <GLint> &
	getLightDirectionUniformLocation () const
	{ return x3d_LightDirection; }

	const std::vector <GLint> &
	getLightBeamWidthUniformLocation () const
	{ return x3d_LightBeamWidth; }

	const std::vector <GLint> &
	getLightCutOffAngleUniformLocation () const
	{ return x3d_LightCutOffAngle; }

	const std::vector <GLint> &
	getLightRadiusUniformLocation () const
	{ return x3d_LightRadius; }

	///  @name Uniform location access material

	GLint
	getSeparateBackColorUniformLocation () const
	{ return x3d_SeparateBackColor; }

	GLint
	getAmbientIntensityUniformLocation () const
	{ return x3d_AmbientIntensity; }

	GLint
	getDiffuseColorUniformLocation () const
	{ return x3d_DiffuseColor; }

	GLint
	getSpecularColorUniformLocation () const
	{ return x3d_SpecularColor; }

	GLint
	getEmissiveColorUniformLocation () const
	{ return x3d_EmissiveColor; }

	GLint
	getShininessUniformLocation () const
	{ return x3d_Shininess; }

	GLint
	getTransparencyUniformLocation () const
	{ return x3d_Transparency; }

	GLint
	getBackAmbientIntensityUniformLocation () const
	{ return x3d_BackAmbientIntensity; }

	GLint
	getBackDiffuseColorUniformLocation () const
	{ return x3d_BackDiffuseColor; }

	GLint
	getBackSpecularColorUniformLocation () const
	{ return x3d_BackSpecularColor; }

	GLint
	getBackEmissiveColorUniformLocation () const
	{ return x3d_BackEmissiveColor; }

	GLint
	getBackShininessUniformLocation () const
	{ return x3d_BackShininess; }

	GLint
	getBackTransparencyUniformLocation () const
	{ return x3d_BackTransparency; }

	///  @name Uniform location access texture

	bool
	isExtensionGPUShaderFP64Available () const
	{ return extensionGPUShaderFP64; }

	GLint
	getTextureTypeUniformLocation () const
	{ return x3d_TextureType; }

	GLint
	getTextureUniformLocation () const
	{ return x3d_Texture; }

	GLint
	getTextureMatrixUniformLocation () const
	{ return x3d_TextureMatrix; }

	///  @name Special

	void
	setTransformFeedbackVaryings (const std::vector <std::string> & value)
	{ transformFeedbackVaryings = value; }

	void
	setTextureBuffer (const std::string &, GLuint);

	///  @name Pipeline

	void
	setGlobalUniforms (ShapeContainer* const);

	void
	setLocalUniforms (ShapeContainer* const);

	void
	enableColorAttrib (const GLuint);

	void
	enableTexCoordAttrib (const std::vector <GLuint> &);

	void
	enableNormalAttrib (const GLuint);

	void
	enableVertexAttrib (const GLuint);
	
	void
	disableColorAttrib ();
	
	void
	disableTexCoordAttrib ();
	
	void
	disableNormalAttrib ();
	
	void
	disableVertexAttrib ();


protected:

	///  @name Construction

	X3DProgrammableShaderObject ();

	virtual
	void
	initialize () override
	{ }

	///  @name Operations
	
	void
	setOpenGLES (const bool value);
	
	void
	applyTransformFeedbackVaryings () const;

	void
	getDefaultUniforms ();

	void
	addShaderFields ();

	///  @name Destructions

	virtual
	void
	dispose () override;


private:

	///  @name Event handlers

	void
	set_field (X3DFieldDefinition* const);

	///  @name Members

	GLint               x3d_GeometryType;
	std::vector <GLint> x3d_ClipPlane;

	GLint x3d_FogType;
	GLint x3d_FogColor;
	GLint x3d_FogVisibilityRange;

	GLint x3d_LinewidthScaleFactor;

	GLint x3d_Lighting;
	GLint x3d_ColorMaterial;

	std::vector <GLint> x3d_LightType;
	std::vector <GLint> x3d_LightColor;
	std::vector <GLint> x3d_LightAmbientIntensity;
	std::vector <GLint> x3d_LightIntensity;
	std::vector <GLint> x3d_LightAttenuation;
	std::vector <GLint> x3d_LightLocation;
	std::vector <GLint> x3d_LightDirection;
	std::vector <GLint> x3d_LightBeamWidth;
	std::vector <GLint> x3d_LightCutOffAngle;
	std::vector <GLint> x3d_LightRadius;

	GLint x3d_SeparateBackColor;

	GLint x3d_AmbientIntensity;
	GLint x3d_DiffuseColor;
	GLint x3d_SpecularColor;
	GLint x3d_EmissiveColor;
	GLint x3d_Shininess;
	GLint x3d_Transparency;

	GLint x3d_BackAmbientIntensity;
	GLint x3d_BackDiffuseColor;
	GLint x3d_BackSpecularColor;
	GLint x3d_BackEmissiveColor;
	GLint x3d_BackShininess;
	GLint x3d_BackTransparency;

	GLint x3d_TextureType;
	GLint x3d_Texture;
	GLint x3d_CubeMapTexture;

	GLint x3d_TextureMatrix;
	GLint x3d_NormalMatrix;
	GLint x3d_ProjectionMatrix;
	GLint x3d_ModelViewMatrix;

	GLint x3d_Color;
	GLint x3d_TexCoord;
	GLint x3d_Normal;
	GLint x3d_Vertex;

	bool                      extensionGPUShaderFP64;
	std::vector <std::string> transformFeedbackVaryings;
	std::vector <size_t>      textureUnits;
	size_t                    geometryType;
	size_t                    numGlobalLights;

};

} // X3D
} // titania

#endif
