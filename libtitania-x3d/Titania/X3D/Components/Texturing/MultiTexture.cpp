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

#include "MultiTexture.h"

#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

#include <Titania/String.h>

namespace titania {
namespace X3D {

const std::string MultiTexture::componentName  = "Texturing";
const std::string MultiTexture::typeName       = "MultiTexture";
const std::string MultiTexture::containerField = "texture";

MultiTexture::Fields::Fields () :
	   color (new SFColor (1, 1, 1)),
	   alpha (new SFFloat (1)),
	    mode (new MFString ()),
	  source (new MFString ()),
	function (new MFString ()),
	 texture (new MFNode ())
{ }

MultiTexture::MultiTexture (X3DExecutionContext* const executionContext) :
	   X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DTextureNode (),
	        fields (),
	         modes (),
	    alphaModes (),
	       sources (),
	     functions ()
{
	addField (inputOutput, "metadata", metadata ());
	addField (inputOutput, "color",    color ());
	addField (inputOutput, "alpha",    alpha ());
	addField (inputOutput, "mode",     mode ());
	addField (inputOutput, "source",   source ());
	addField (inputOutput, "function", function ());
	addField (inputOutput, "texture",  texture ());
}

X3DBaseNode*
MultiTexture::create (X3DExecutionContext* const executionContext) const
{
	return new MultiTexture (executionContext);
}

void
MultiTexture::initialize ()
{
	X3DTextureNode::initialize ();

	mode ()     .addInterest (this, &MultiTexture::set_mode);
	source ()   .addInterest (this, &MultiTexture::set_source);
	function () .addInterest (this, &MultiTexture::set_function);

	set_mode ();
	set_source ();
	set_function ();
}

bool
MultiTexture::isTransparent () const
{
	return true;
}

MultiTexture::ModeType
MultiTexture::getMode (size_t index) const
{
	if (index < modes .size ())
		return modes [index];

	return ModeType::MODULATE;
}

MultiTexture::ModeType
MultiTexture::getAlphaMode (size_t index) const
{
	if (index < alphaModes .size ())
		return alphaModes [index];

	return ModeType::MODULATE;
}

MultiTexture::SourceType
MultiTexture::getSource (size_t index) const
{
	if (index < sources .size ())
		return sources [index];

	return SourceType::DEFAULT;
}

MultiTexture::FunctionType
MultiTexture::getFunction (size_t index) const
{
	if (index < functions .size ())
		return functions [index];

	return FunctionType::DEFAULT;
}

void
MultiTexture::set_mode ()
{
	static std::map <std::string, ModeType> map = {
		std::make_pair ("MODULATE",                  ModeType::MODULATE),
		std::make_pair ("REPLACE",                   ModeType::REPLACE),
		std::make_pair ("MODULATE2X",                ModeType::MODULATE2X),
		std::make_pair ("MODULATE4X",                ModeType::MODULATE4X),
		std::make_pair ("ADD",                       ModeType::ADD),
		std::make_pair ("ADDSIGNED",                 ModeType::ADDSIGNED),
		std::make_pair ("ADDSIGNED2X",               ModeType::ADDSIGNED2X),
		std::make_pair ("SUBTRACT",                  ModeType::SUBTRACT),
		std::make_pair ("ADDSMOOTH",                 ModeType::ADDSMOOTH),
		std::make_pair ("BLENDDIFFUSEALPHA",         ModeType::BLENDDIFFUSEALPHA),
		std::make_pair ("BLENDTEXTUREALPHA",         ModeType::BLENDTEXTUREALPHA),
		std::make_pair ("BLENDFACTORALPHA",          ModeType::BLENDFACTORALPHA),
		std::make_pair ("BLENDCURRENTALPHA",         ModeType::BLENDCURRENTALPHA),
		std::make_pair ("MODULATEALPHA_ADDCOLOR",    ModeType::MODULATEALPHA_ADDCOLOR),
		std::make_pair ("MODULATEINVALPHA_ADDCOLOR", ModeType::MODULATEINVALPHA_ADDCOLOR),
		std::make_pair ("MODULATEINVCOLOR_ADDALPHA", ModeType::MODULATEINVCOLOR_ADDALPHA),
		std::make_pair ("OFF",                       ModeType::OFF),
		std::make_pair ("SELECTARG1",                ModeType::SELECTARG1),
		std::make_pair ("SELECTARG2",                ModeType::SELECTARG2),
		std::make_pair ("DOTPRODUCT3",               ModeType::DOTPRODUCT3)
	};

	modes .clear ();
	alphaModes .clear ();

	for (const auto & string : mode ())
	{
		auto mode = basic::split (string, ",");

		for (auto & s : mode)
			s = basic::trim (s);

		if (mode .size () < 2)
			mode .emplace_back (mode [0]);

		// RGB
		{
			auto iter = map .find (mode [0]);

			if (iter not_eq map .end ())
				modes .emplace_back (iter -> second);

			else
				modes .emplace_back (ModeType::MODULATE);
		}

		// Alpha
		{
			auto iter = map .find (mode [1]);

			if (iter not_eq map .end ())
				alphaModes .emplace_back (iter -> second);

			else
				alphaModes .emplace_back (ModeType::MODULATE);
		}
	}
}

void
MultiTexture::set_source ()
{
	static std::map <std::string, SourceType> map = {
		std::make_pair ("DIFFUSE",  SourceType::DIFFUSE),
		std::make_pair ("SPECULAR", SourceType::SPECULAR),
		std::make_pair ("FACTOR",   SourceType::FACTOR)
	};

	sources .clear ();

	for (const auto & string : source ())
	{
		auto iter = map .find (string);

		if (iter not_eq map .end ())
			sources .emplace_back (iter -> second);

		else
			sources .emplace_back (SourceType::DEFAULT);
	}
}

void
MultiTexture::set_function ()
{
	static std::map <std::string, FunctionType> map = {
		std::make_pair ("COMPLEMENT",     FunctionType::COMPLEMENT),
		std::make_pair ("ALPHAREPLICATE", FunctionType::ALPHAREPLICATE)
	};

	functions .clear ();

	for (const auto & string : function ())
	{
		auto iter = map .find (string);

		if (iter not_eq map .end ())
			functions .emplace_back (iter -> second);

		else
			functions .emplace_back (FunctionType::DEFAULT);
	}
}

void
MultiTexture::draw ()
{
	size_t index = 0;

	for (const auto & node : texture ())
	{
		if (x3d_cast <MultiTexture*> (node))
			continue;

		auto textureNode = x3d_cast <X3DTextureNode*> (node);

		if (textureNode)
		{
			if (getMode (index) == ModeType::OFF and getAlphaMode (index) == ModeType::OFF)
			{
				getBrowser () -> getTextureStages () .emplace_back (-1);
				++ index;
				continue;
			}

			if (getBrowser () -> getTextureUnits () .empty ())
				break;

			// Get texture unit.

			int32_t unit = getBrowser () -> getTextureUnits () .top ();
			getBrowser () -> getTextureUnits () .pop ();

			getBrowser () -> getTextureStages () .emplace_back (unit);

			// Activate texture

			glActiveTexture (GL_TEXTURE0 + unit);

			textureNode -> draw ();

			// Setup color

			Color4f texEnvColor (color () .getR (), color () .getG (), color () .getB (), alpha ());
			glTexEnvfv (GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, texEnvColor .data ());

			// 

			glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);

			// Setup source 0 to be the texture.
	
			GLenum arg1_rgb_source    = GL_TEXTURE;
			GLenum arg1_alpha_source  = GL_TEXTURE;
			
			// Setup source 1 depending on the fields.

			GLenum arg2_rgb_source    = GL_PREVIOUS;
			GLenum arg2_alpha_source  = GL_PREVIOUS;

			switch (getSource (index))
			{
				case SourceType::DEFAULT:
				{
					// Use default values.
					break;
				}

				case SourceType::DIFFUSE:
				{
					arg2_rgb_source   = GL_PRIMARY_COLOR;
					arg2_alpha_source = GL_PRIMARY_COLOR;
					break;
				}
				case SourceType::SPECULAR:
				{
					// Unsuported
					break;
				}
				case SourceType::FACTOR:
				{
					arg2_rgb_source   = GL_CONSTANT;
					arg2_alpha_source = GL_CONSTANT;
					break;
				}
			}
			
			// Setup operant 0 and 1

			GLenum arg1_rgb_operand   = GL_SRC_COLOR;
			GLenum arg1_alpha_operand = GL_SRC_ALPHA;

			GLenum arg2_rgb_operand   = GL_SRC_COLOR;
			GLenum arg2_alpha_operand = GL_SRC_ALPHA;

			switch (getFunction (index))
			{
				case FunctionType::DEFAULT:
				{
					// Use default values.
					break;
				}
				case FunctionType::COMPLEMENT:
				{
					arg1_rgb_operand   = GL_ONE_MINUS_SRC_COLOR;
					arg1_alpha_operand = GL_ONE_MINUS_SRC_ALPHA;
					break;
				}
				case FunctionType::ALPHAREPLICATE:
				{
					arg1_alpha_source = GL_PREVIOUS;
					break;
				}
			}

			//

			glTexEnvi (GL_TEXTURE_ENV, GL_SOURCE0_RGB,    arg1_rgb_source);
			glTexEnvi (GL_TEXTURE_ENV, GL_SOURCE0_ALPHA,  arg1_alpha_source);
			glTexEnvi (GL_TEXTURE_ENV, GL_OPERAND0_RGB,   arg1_rgb_operand);
			glTexEnvi (GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, arg1_alpha_operand);

			glTexEnvi (GL_TEXTURE_ENV, GL_SOURCE1_RGB,    arg2_rgb_source);
			glTexEnvi (GL_TEXTURE_ENV, GL_SOURCE1_ALPHA,  arg2_alpha_source);
			glTexEnvi (GL_TEXTURE_ENV, GL_OPERAND1_RGB,   arg2_rgb_operand);
			glTexEnvi (GL_TEXTURE_ENV, GL_OPERAND1_ALPHA, arg2_alpha_operand);

			// Set mode

			switch (getMode (index))
			{
				case ModeType::MODULATE:
				{
					glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
					break;
				}
				case ModeType::REPLACE:
				{
					glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_REPLACE);
					break;
				}
				case ModeType::MODULATE2X:
				{
					glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
					glTexEnvi (GL_TEXTURE_ENV, GL_RGB_SCALE, 2);
					break;
				}
				case ModeType::MODULATE4X:
				{
					glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
					glTexEnvi (GL_TEXTURE_ENV, GL_RGB_SCALE, 4);
					break;
				}
				case ModeType::ADD:
				{
					glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_ADD);
					break;
				}
				case ModeType::ADDSIGNED:
				{
					glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_ADD_SIGNED);
					break;
				}
				case ModeType::ADDSIGNED2X:
				{
					glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_ADD_SIGNED);
					glTexEnvi (GL_TEXTURE_ENV, GL_RGB_SCALE, 2);
					break;
				}
				case ModeType::SUBTRACT:
				{
					glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_SUBTRACT);
					break;
				}
				case ModeType::ADDSMOOTH:
				{
					// Unsupported
					glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
					break;
				}
				case ModeType::BLENDDIFFUSEALPHA:
				{
					glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_RGB,  GL_INTERPOLATE);
					glTexEnvi (GL_TEXTURE_ENV, GL_SOURCE2_RGB,  GL_PRIMARY_COLOR);
					glTexEnvi (GL_TEXTURE_ENV, GL_OPERAND2_RGB, GL_SRC_ALPHA);

					break;
				}
				case ModeType::BLENDTEXTUREALPHA:
				{
					glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_RGB,  GL_INTERPOLATE);
					glTexEnvi (GL_TEXTURE_ENV, GL_SOURCE2_RGB,  GL_TEXTURE);
					glTexEnvi (GL_TEXTURE_ENV, GL_OPERAND2_RGB, GL_SRC_ALPHA);

					break;
				}
				case ModeType::BLENDFACTORALPHA:
				{
					glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_RGB,  GL_INTERPOLATE);
					glTexEnvi (GL_TEXTURE_ENV, GL_SOURCE2_RGB,  GL_CONSTANT);
					glTexEnvi (GL_TEXTURE_ENV, GL_OPERAND2_RGB, GL_SRC_ALPHA);

					break;
				}
				case ModeType::BLENDCURRENTALPHA:
				{
					glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_RGB,  GL_INTERPOLATE);
					glTexEnvi (GL_TEXTURE_ENV, GL_SOURCE2_RGB,  GL_PREVIOUS);
					glTexEnvi (GL_TEXTURE_ENV, GL_OPERAND2_RGB, GL_SRC_ALPHA);

					break;
				}
				case ModeType::MODULATEALPHA_ADDCOLOR:
				{
					// Unsupported
					glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
					break;
				}
				case ModeType::MODULATEINVALPHA_ADDCOLOR:
				{
					// Unsupported
					glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
					break;
				}
				case ModeType::MODULATEINVCOLOR_ADDALPHA:
				{
					// Unsupported
					glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
					break;
				}
				case ModeType::OFF:
				{
					glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_RGB,  GL_REPLACE);
					glTexEnvi (GL_TEXTURE_ENV, GL_SOURCE0_RGB,  GL_PREVIOUS);
					glTexEnvi (GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);

					break;
				}
				case ModeType::SELECTARG1:
				{
					glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_REPLACE);
					break;
				}
				case ModeType::SELECTARG2:
				{
					glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_RGB,  GL_REPLACE);
					glTexEnvi (GL_TEXTURE_ENV, GL_SOURCE0_RGB,  arg2_rgb_source);
					glTexEnvi (GL_TEXTURE_ENV, GL_OPERAND0_RGB, arg2_rgb_operand);

					break;
				}
				case ModeType::DOTPRODUCT3:
				{
					if (GLEW_ARB_texture_env_dot3)
					{
						if (getAlphaMode (index) == ModeType::DOTPRODUCT3)
							glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_DOT3_RGBA);

						else
							glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_DOT3_RGB);
					}
					else
					{
						// Unsupported
						glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
					}

					break;
				}
			}

			switch (getAlphaMode (index))
			{
				case ModeType::MODULATE:
				{
					glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);
					break;
				}
				case ModeType::REPLACE:
				{
					glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_REPLACE);
					break;
				}
				case ModeType::MODULATE2X:
				{
					glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);
					glTexEnvi (GL_TEXTURE_ENV, GL_ALPHA_SCALE, 2);
					break;
				}
				case ModeType::MODULATE4X:
				{
					glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);
					glTexEnvi (GL_TEXTURE_ENV, GL_ALPHA_SCALE, 4);
					break;
				}
				case ModeType::ADD:
				{
					glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_ADD);
					break;
				}
				case ModeType::ADDSIGNED:
				{
					glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_ADD_SIGNED);
					break;
				}
				case ModeType::ADDSIGNED2X:
				{
					glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_ADD_SIGNED);
					glTexEnvi (GL_TEXTURE_ENV, GL_ALPHA_SCALE, 2);
					break;
				}
				case ModeType::SUBTRACT:
				{
					glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_SUBTRACT);
					break;
				}
				case ModeType::ADDSMOOTH:
				{
					// Unsupported
					glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);
					break;
				}
				case ModeType::BLENDDIFFUSEALPHA:
				{
					glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_ALPHA,  GL_INTERPOLATE);
					glTexEnvi (GL_TEXTURE_ENV, GL_SOURCE2_ALPHA,  GL_PRIMARY_COLOR);
					glTexEnvi (GL_TEXTURE_ENV, GL_OPERAND2_ALPHA, GL_SRC_ALPHA);

					break;
				}
				case ModeType::BLENDTEXTUREALPHA:
				{
					glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_ALPHA,  GL_INTERPOLATE);
					glTexEnvi (GL_TEXTURE_ENV, GL_SOURCE2_ALPHA,  GL_TEXTURE);
					glTexEnvi (GL_TEXTURE_ENV, GL_OPERAND2_ALPHA, GL_SRC_ALPHA);

					break;
				}
				case ModeType::BLENDFACTORALPHA:
				{
					glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_ALPHA,  GL_INTERPOLATE);
					glTexEnvi (GL_TEXTURE_ENV, GL_SOURCE2_ALPHA,  GL_CONSTANT);
					glTexEnvi (GL_TEXTURE_ENV, GL_OPERAND2_ALPHA, GL_SRC_ALPHA);

					break;
				}
				case ModeType::BLENDCURRENTALPHA:
				{
					glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_ALPHA,  GL_INTERPOLATE);
					glTexEnvi (GL_TEXTURE_ENV, GL_SOURCE2_ALPHA,  GL_PREVIOUS);
					glTexEnvi (GL_TEXTURE_ENV, GL_OPERAND2_ALPHA, GL_SRC_ALPHA);

					break;
				}
				case ModeType::MODULATEALPHA_ADDCOLOR:
				{
					// Unsupported
					glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);
					break;
				}
				case ModeType::MODULATEINVALPHA_ADDCOLOR:
				{
					// Unsupported
					glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);
					break;
				}
				case ModeType::MODULATEINVCOLOR_ADDALPHA:
				{
					// Unsupported
					glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);
					break;
				}
				case ModeType::OFF:
				{
					glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_ALPHA,  GL_REPLACE);
					glTexEnvi (GL_TEXTURE_ENV, GL_SOURCE0_ALPHA,  GL_PREVIOUS);
					glTexEnvi (GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);

					break;
				}
				case ModeType::SELECTARG1:
				{
					glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_REPLACE);
					break;
				}
				case ModeType::SELECTARG2:
				{
					glTexEnvi (GL_TEXTURE_ENV, GL_COMBINE_ALPHA,  GL_REPLACE);
					glTexEnvi (GL_TEXTURE_ENV, GL_SOURCE0_ALPHA,  arg2_alpha_source);
					glTexEnvi (GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, arg2_alpha_operand);

					break;
				}
				case ModeType::DOTPRODUCT3:
				{
					break;
				}
			}

			++ index;
		}
	}
}

} // X3D

} // titania
