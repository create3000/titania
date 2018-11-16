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

#include "MagicImport.h"

#include "BrowserSelection.h"
#include "UserData.h"
#include "X3DBrowserWindow.h"

#include <Titania/X3D/Bits/Cast.h>
#include <Titania/X3D/Bits/Traverse.h>
#include <Titania/X3D/Components/Core/X3DPrototypeInstance.h>
#include <Titania/X3D/Components/EnvironmentalEffects/X3DBackgroundNode.h>
#include <Titania/X3D/Components/Shape/Appearance.h>
#include <Titania/X3D/Components/Shape/Material.h>
#include <Titania/X3D/Components/Shape/TwoSidedMaterial.h>
#include <Titania/X3D/Components/Texturing/X3DTexture2DNode.h>
#include <Titania/X3D/Components/Texturing3D/X3DTexture3DNode.h>
#include <Titania/X3D/Editing/X3DEditor.h>
#include <Titania/X3D/Prototype/ExternProtoDeclaration.h>
#include <Titania/X3D/Prototype/ProtoDeclaration.h>

namespace titania {
namespace puck {

using namespace std::placeholders;

MagicImport::MagicImport (X3DBrowserWindow* const browserWindow) :
	X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	 importFunctions1 ({ std::pair ("Material", std::bind (&MagicImport::material, this, _1, _2, _3, _4)),
	                     std::pair ("Texture",  std::bind (&MagicImport::texture,  this, _1, _2, _3, _4)) }),
	 importFunctions2 ({ std::pair ("Background",     std::bind (&MagicImport::bind, this, _1, _2, _3, _4)),
	                     std::pair ("Fog",            std::bind (&MagicImport::bind, this, _1, _2, _3, _4)),
	                     std::pair ("NavigationInfo", std::bind (&MagicImport::bind, this, _1, _2, _3, _4)),
	                     std::pair ("Viewpoint",      std::bind (&MagicImport::bind, this, _1, _2, _3, _4)) }),
	    frontMaterial (true)
{
	setup ();
}

bool
MagicImport::import (const X3D::X3DExecutionContextPtr & executionContext, const X3D::MFNode & selection, const X3D::X3DScenePtr & scene, const X3D::UndoStepPtr & undoStep)
{
	try
	{
		const std::string magic = scene -> getMetaData ("titania magic");

		if (importFunctions1 .at (magic) (executionContext, const_cast <X3D::MFNode &> (selection), scene, undoStep))
		{
			X3D::X3DEditor::requestUpdateInstances (executionContext, undoStep);
			return true;
		}

		return false;
	}
	catch (const X3D::Error <X3D::INVALID_NAME> &)
	{
		return false;
	}
	catch (const std::out_of_range &)
	{
		return false;
	}
}

void
MagicImport::process (const X3D::X3DExecutionContextPtr & executionContext, const X3D::MFNode & importedNodes, const X3D::X3DScenePtr & scene, const X3D::UndoStepPtr & undoStep)
{
	try
	{
		const std::string magic = scene -> getMetaData ("titania magic");

		importFunctions2 .at (magic) (executionContext, const_cast <X3D::MFNode &> (importedNodes), scene, undoStep);

		X3D::X3DEditor::requestUpdateInstances (executionContext, undoStep);
	}
	catch (const X3D::Error <X3D::INVALID_NAME> &)
	{ }
	catch (const std::out_of_range &)
	{ }
}

void
MagicImport::bind (const X3D::X3DExecutionContextPtr & executionContext, X3D::MFNode & importedNodes, const X3D::X3DScenePtr & scene, const X3D::UndoStepPtr & undoStep)
{
	X3D::traverse (importedNodes, [&] (X3D::SFNode & node)
	{
		const X3D::X3DPtr <X3D::X3DBindableNode> bindableNode (node);
		
		if (bindableNode)
		{
			bindableNode -> set_bind () = true;
			return false;
		}
		
		return true;
	});
}

bool
MagicImport::material (const X3D::X3DExecutionContextPtr & executionContext, X3D::MFNode & selection, const X3D::X3DScenePtr & scene, const X3D::UndoStepPtr & undoStep)
{
	if (selection .empty ())
		return false;

	// Find first material node in scene

	X3D::SFNode material;

	X3D::traverse (scene -> getRootNodes (), [&] (X3D::SFNode & node)
	{
		const X3D::X3DPtr <X3D::Appearance> appearance (node);
		
		if (appearance and X3D::x3d_cast <X3D::X3DMaterialNode*> (appearance -> material ()))
		{
			importProtoDeclaration (executionContext, appearance -> material (), undoStep);
			
			material = appearance -> material ();
			
			material -> setExecutionContext (executionContext);
			return false;
		}
		
		return true;
	});

	// Assign material to all appearances in selection

	X3D::traverse (selection, [&] (X3D::SFNode & node)
	{
		using setValue = void (X3D::SFColor::*) (const X3D::Color3f &);
		
		const auto appearance = dynamic_cast <X3D::Appearance*> (node .getValue ());
		
		if (appearance)
		{
			const X3D::X3DPtr <X3D::Material> lhs (appearance -> material ());
			const X3D::X3DPtr <X3D::Material> rhs (material);
			
			if (rhs)
			{
				if (lhs and lhs -> getExecutionContext () == executionContext)
				{
					undoStep -> addUndoFunction (&X3D::SFFloat::setValue,             std::ref (lhs -> ambientIntensity ()), lhs -> ambientIntensity ());
					undoStep -> addUndoFunction ((setValue) & X3D::SFColor::setValue, std::ref (lhs -> diffuseColor ()),     lhs -> diffuseColor ());
					undoStep -> addUndoFunction ((setValue) & X3D::SFColor::setValue, std::ref (lhs -> specularColor ()),    lhs -> specularColor ());
					undoStep -> addUndoFunction ((setValue) & X3D::SFColor::setValue, std::ref (lhs -> emissiveColor ()),    lhs -> emissiveColor ());
					undoStep -> addUndoFunction (&X3D::SFFloat::setValue,             std::ref (lhs -> shininess ()),        lhs -> shininess ());
					undoStep -> addUndoFunction (&X3D::SFFloat::setValue,             std::ref (lhs -> transparency ()),     lhs -> transparency ());
					
					undoStep -> addRedoFunction (&X3D::SFFloat::setValue,             std::ref (lhs -> ambientIntensity ()), rhs -> ambientIntensity ());
					undoStep -> addRedoFunction ((setValue) & X3D::SFColor::setValue, std::ref (lhs -> diffuseColor ()),     rhs -> diffuseColor ());
					undoStep -> addRedoFunction ((setValue) & X3D::SFColor::setValue, std::ref (lhs -> specularColor ()),    rhs -> specularColor ());
					undoStep -> addRedoFunction ((setValue) & X3D::SFColor::setValue, std::ref (lhs -> emissiveColor ()),    rhs -> emissiveColor ());
					undoStep -> addRedoFunction (&X3D::SFFloat::setValue,             std::ref (lhs -> shininess ()),        rhs -> shininess ());
					undoStep -> addRedoFunction (&X3D::SFFloat::setValue,             std::ref (lhs -> transparency ()),     rhs -> transparency ());
					
					lhs -> ambientIntensity () = rhs -> ambientIntensity ();
					lhs -> diffuseColor ()     = rhs -> diffuseColor ();
					lhs -> specularColor ()    = rhs -> specularColor ();
					lhs -> emissiveColor ()    = rhs -> emissiveColor ();
					lhs -> shininess ()        = rhs -> shininess ();
					lhs -> transparency ()     = rhs -> transparency ();
					
					X3D::X3DEditor::updateNamedNode (executionContext, rhs -> getName (), appearance -> material (), undoStep);
				}
				else
				{
					const X3D::X3DPtr <X3D::TwoSidedMaterial> lhs (appearance -> material ());
					
					if (lhs and lhs -> getExecutionContext () == executionContext)
					{
						auto & ambientIntensity = frontMaterial ? lhs -> ambientIntensity () : lhs -> backAmbientIntensity ();
						auto & diffuseColor     = frontMaterial ? lhs -> diffuseColor ()     : lhs -> backDiffuseColor ();
						auto & specularColor    = frontMaterial ? lhs -> specularColor ()    : lhs -> backSpecularColor ();
						auto & emissiveColor    = frontMaterial ? lhs -> emissiveColor ()    : lhs -> backEmissiveColor ();
						auto & shininess        = frontMaterial ? lhs -> shininess ()        : lhs -> backShininess ();
						auto & transparency     = frontMaterial ? lhs -> transparency ()     : lhs -> backTransparency ();
						
						undoStep -> addUndoFunction (&X3D::SFFloat::setValue,             std::ref (ambientIntensity), ambientIntensity);
						undoStep -> addUndoFunction ((setValue) & X3D::SFColor::setValue, std::ref (diffuseColor),     diffuseColor);
						undoStep -> addUndoFunction ((setValue) & X3D::SFColor::setValue, std::ref (specularColor),    specularColor);
						undoStep -> addUndoFunction ((setValue) & X3D::SFColor::setValue, std::ref (emissiveColor),    emissiveColor);
						undoStep -> addUndoFunction (&X3D::SFFloat::setValue,             std::ref (shininess),        shininess);
						undoStep -> addUndoFunction (&X3D::SFFloat::setValue,             std::ref (transparency),     transparency);
						
						undoStep -> addRedoFunction (&X3D::SFFloat::setValue,             std::ref (ambientIntensity), rhs -> ambientIntensity ());
						undoStep -> addRedoFunction ((setValue) & X3D::SFColor::setValue, std::ref (diffuseColor),     rhs -> diffuseColor ());
						undoStep -> addRedoFunction ((setValue) & X3D::SFColor::setValue, std::ref (specularColor),    rhs -> specularColor ());
						undoStep -> addRedoFunction ((setValue) & X3D::SFColor::setValue, std::ref (emissiveColor),    rhs -> emissiveColor ());
						undoStep -> addRedoFunction (&X3D::SFFloat::setValue,             std::ref (shininess),        rhs -> shininess ());
						undoStep -> addRedoFunction (&X3D::SFFloat::setValue,             std::ref (transparency),     rhs -> transparency ());
						
						ambientIntensity = rhs -> ambientIntensity ();
						diffuseColor     = rhs -> diffuseColor ();
						specularColor    = rhs -> specularColor ();
						emissiveColor    = rhs -> emissiveColor ();
						shininess        = rhs -> shininess ();
						transparency     = rhs -> transparency ();
						
						X3D::X3DEditor::updateNamedNode (executionContext, rhs -> getName (), appearance -> material (), undoStep);
					}
					else
						X3D::X3DEditor::replaceNode (executionContext, node, appearance -> material (), material, undoStep);
				}
			}
			else
			{
				const X3D::X3DPtr <X3D::TwoSidedMaterial> rhs (material);
				
				if (rhs)
				{
					if (lhs and lhs -> getExecutionContext () == executionContext)
					{
						undoStep -> addUndoFunction (&X3D::SFFloat::setValue,             std::ref (lhs -> ambientIntensity ()), lhs -> ambientIntensity ());
						undoStep -> addUndoFunction ((setValue) & X3D::SFColor::setValue, std::ref (lhs -> diffuseColor ()),     lhs -> diffuseColor ());
						undoStep -> addUndoFunction ((setValue) & X3D::SFColor::setValue, std::ref (lhs -> specularColor ()),    lhs -> specularColor ());
						undoStep -> addUndoFunction ((setValue) & X3D::SFColor::setValue, std::ref (lhs -> emissiveColor ()),    lhs -> emissiveColor ());
						undoStep -> addUndoFunction (&X3D::SFFloat::setValue,             std::ref (lhs -> shininess ()),        lhs -> shininess ());
						undoStep -> addUndoFunction (&X3D::SFFloat::setValue,             std::ref (lhs -> transparency ()),     lhs -> transparency ());
						
						undoStep -> addRedoFunction (&X3D::SFFloat::setValue,             std::ref (lhs -> ambientIntensity ()), rhs -> ambientIntensity ());
						undoStep -> addRedoFunction ((setValue) & X3D::SFColor::setValue, std::ref (lhs -> diffuseColor ()),     rhs -> diffuseColor ());
						undoStep -> addRedoFunction ((setValue) & X3D::SFColor::setValue, std::ref (lhs -> specularColor ()),    rhs -> specularColor ());
						undoStep -> addRedoFunction ((setValue) & X3D::SFColor::setValue, std::ref (lhs -> emissiveColor ()),    rhs -> emissiveColor ());
						undoStep -> addRedoFunction (&X3D::SFFloat::setValue,             std::ref (lhs -> shininess ()),        rhs -> shininess ());
						undoStep -> addRedoFunction (&X3D::SFFloat::setValue,             std::ref (lhs -> transparency ()),     rhs -> transparency ());
						
						lhs -> ambientIntensity () = rhs -> ambientIntensity ();
						lhs -> diffuseColor ()     = rhs -> diffuseColor ();
						lhs -> specularColor ()    = rhs -> specularColor ();
						lhs -> emissiveColor ()    = rhs -> emissiveColor ();
						lhs -> shininess ()        = rhs -> shininess ();
						lhs -> transparency ()     = rhs -> transparency ();
						
						X3D::X3DEditor::updateNamedNode (executionContext, rhs -> getName (), appearance -> material (), undoStep);
					}
					else
					{
						const X3D::X3DPtr <X3D::TwoSidedMaterial> lhs (appearance -> material ());
						
						if (lhs and lhs -> getExecutionContext () == executionContext)
						{
							undoStep -> addUndoFunction (&X3D::SFFloat::setValue,             std::ref (lhs -> ambientIntensity ()),     lhs -> ambientIntensity ());
							undoStep -> addUndoFunction ((setValue) & X3D::SFColor::setValue, std::ref (lhs -> diffuseColor ()),         lhs -> diffuseColor ());
							undoStep -> addUndoFunction ((setValue) & X3D::SFColor::setValue, std::ref (lhs -> specularColor ()),        lhs -> specularColor ());
							undoStep -> addUndoFunction ((setValue) & X3D::SFColor::setValue, std::ref (lhs -> emissiveColor ()),        lhs -> emissiveColor ());
							undoStep -> addUndoFunction (&X3D::SFFloat::setValue,             std::ref (lhs -> shininess ()),            lhs -> shininess ());
							undoStep -> addUndoFunction (&X3D::SFFloat::setValue,             std::ref (lhs -> transparency ()),         lhs -> transparency ());
							
							undoStep -> addUndoFunction (&X3D::SFFloat::setValue,             std::ref (lhs -> backAmbientIntensity ()), lhs -> backAmbientIntensity ());
							undoStep -> addUndoFunction ((setValue) & X3D::SFColor::setValue, std::ref (lhs -> backDiffuseColor ()),     lhs -> backDiffuseColor ());
							undoStep -> addUndoFunction ((setValue) & X3D::SFColor::setValue, std::ref (lhs -> backSpecularColor ()),    lhs -> backSpecularColor ());
							undoStep -> addUndoFunction ((setValue) & X3D::SFColor::setValue, std::ref (lhs -> backEmissiveColor ()),    lhs -> backEmissiveColor ());
							undoStep -> addUndoFunction (&X3D::SFFloat::setValue,             std::ref (lhs -> backShininess ()),        lhs -> backShininess ());
							undoStep -> addUndoFunction (&X3D::SFFloat::setValue,             std::ref (lhs -> backTransparency ()),     lhs -> backTransparency ());
							
							undoStep -> addRedoFunction (&X3D::SFFloat::setValue,             std::ref (lhs -> ambientIntensity ()),     rhs -> ambientIntensity ());
							undoStep -> addRedoFunction ((setValue) & X3D::SFColor::setValue, std::ref (lhs -> diffuseColor ()),         rhs -> diffuseColor ());
							undoStep -> addRedoFunction ((setValue) & X3D::SFColor::setValue, std::ref (lhs -> specularColor ()),        rhs -> specularColor ());
							undoStep -> addRedoFunction ((setValue) & X3D::SFColor::setValue, std::ref (lhs -> emissiveColor ()),        rhs -> emissiveColor ());
							undoStep -> addRedoFunction (&X3D::SFFloat::setValue,             std::ref (lhs -> shininess ()),            rhs -> shininess ());
							undoStep -> addRedoFunction (&X3D::SFFloat::setValue,             std::ref (lhs -> transparency ()),         rhs -> transparency ());
							
							undoStep -> addRedoFunction (&X3D::SFFloat::setValue,             std::ref (lhs -> backAmbientIntensity ()), rhs -> backAmbientIntensity ());
							undoStep -> addRedoFunction ((setValue) & X3D::SFColor::setValue, std::ref (lhs -> backDiffuseColor ()),     rhs -> backDiffuseColor ());
							undoStep -> addRedoFunction ((setValue) & X3D::SFColor::setValue, std::ref (lhs -> backSpecularColor ()),    rhs -> backSpecularColor ());
							undoStep -> addRedoFunction ((setValue) & X3D::SFColor::setValue, std::ref (lhs -> backEmissiveColor ()),    rhs -> backEmissiveColor ());
							undoStep -> addRedoFunction (&X3D::SFFloat::setValue,             std::ref (lhs -> backShininess ()),        rhs -> backShininess ());
							undoStep -> addRedoFunction (&X3D::SFFloat::setValue,             std::ref (lhs -> backTransparency ()),     rhs -> backTransparency ());
							
							lhs -> ambientIntensity () = rhs -> ambientIntensity ();
							lhs -> diffuseColor ()     = rhs -> diffuseColor ();
							lhs -> specularColor ()    = rhs -> specularColor ();
							lhs -> emissiveColor ()    = rhs -> emissiveColor ();
							lhs -> shininess ()        = rhs -> shininess ();
							lhs -> transparency ()     = rhs -> transparency ();
							
							lhs -> backAmbientIntensity () = rhs -> backAmbientIntensity ();
							lhs -> backDiffuseColor ()     = rhs -> backDiffuseColor ();
							lhs -> backSpecularColor ()    = rhs -> backSpecularColor ();
							lhs -> backEmissiveColor ()    = rhs -> backEmissiveColor ();
							lhs -> backShininess ()        = rhs -> backShininess ();
							lhs -> backTransparency ()     = rhs -> backTransparency ();
							
							X3D::X3DEditor::updateNamedNode (executionContext, rhs -> getName (), appearance -> material (), undoStep);
						}
						else
						{
							if (appearance -> material ())
								material -> getUserData <UserData> () -> expanded = appearance -> material () -> getUserData <UserData> () -> expanded;

							X3D::X3DEditor::replaceNode (executionContext, node, appearance -> material (), material, undoStep);
						}
					}
				}
			}
		}
		
		return true;
	});

	return true;
}

bool
MagicImport::texture (const X3D::X3DExecutionContextPtr & executionContext, X3D::MFNode & selection, const X3D::X3DScenePtr & scene, const X3D::UndoStepPtr & undoStep)
{
	if (selection .empty ())
		return false;

	// Find first texture node in scene

	X3D::SFNode texture;

	X3D::traverse (scene -> getRootNodes (), [&] (X3D::SFNode & node)
	{
		const X3D::X3DPtr <X3D::Appearance> appearance (node);
		
		if (appearance and X3D::x3d_cast <X3D::X3DTextureNode*> (appearance -> texture ()))
		{
			importProtoDeclaration (executionContext, appearance -> texture (), undoStep);
			
			texture = appearance -> texture ();
			return false;
		}
		
		return true;
	});

	X3D::traverse (texture, [&] (X3D::SFNode & node)
	{
		node -> setExecutionContext (executionContext);
		return true;
	});

	const X3D::X3DPtr <X3D::X3DTexture2DNode> texture2D (texture);
	const X3D::X3DPtr <X3D::X3DTexture3DNode> texture3D (texture);

	// Assign texture to all appearances in selection

	X3D::traverse (selection, [&] (X3D::SFNode & node)
	{
		const auto appearance = dynamic_cast <X3D::Appearance*> (node .getValue ());
		
		if (appearance)
		{
			const X3D::X3DPtr <X3D::X3DTexture2DNode> oldTexture2D (appearance -> texture ());
			const X3D::X3DPtr <X3D::X3DTexture3DNode> oldTexture3D (appearance -> texture ());
			
			if (oldTexture2D and texture2D)
			{
				texture2D -> repeatS ()           = oldTexture2D -> repeatS ();
				texture2D -> repeatT ()           = oldTexture2D -> repeatT ();
				texture2D -> textureProperties () = oldTexture2D -> textureProperties ();
			}
			else if (oldTexture3D and texture3D)
			{
				texture3D -> repeatS ()           = oldTexture3D -> repeatS ();
				texture3D -> repeatT ()           = oldTexture3D -> repeatT ();
				texture3D -> repeatR ()           = oldTexture3D -> repeatR ();
				texture3D -> textureProperties () = oldTexture3D -> textureProperties ();
			}

			if (appearance -> texture ())
				texture -> getUserData <UserData> () -> expanded = appearance -> texture () -> getUserData <UserData> () -> expanded;
			
			X3D::X3DEditor::replaceNode (executionContext, node, appearance -> texture (), texture, undoStep);
		}
		
		return true;
	});

	return true;
}

void
MagicImport::importProtoDeclaration (const X3D::X3DExecutionContextPtr & executionContext, const X3D::SFNode & node, const X3D::UndoStepPtr & undoStep)
{
	const auto prototypeInstance = dynamic_cast <X3D::X3DPrototypeInstance*> (node .getValue ());

	if (prototypeInstance)
	{
		const auto name          = prototypeInstance -> getProtoDeclarationNode () -> getName ();
		const bool isExternProto = prototypeInstance -> getProtoDeclarationNode () -> isExternproto ();

		try
		{
			const auto protoDeclaration = executionContext -> getProtoDeclaration (name);

			undoStep -> addUndoFunction (&X3D::X3DExecutionContext::updateProtoDeclaration,
			                             executionContext,
			                             name,
			                             protoDeclaration);
		}
		catch (const X3D::X3DError &)
		{
			try
			{
				const auto externProtoDeclaration = executionContext -> getExternProtoDeclaration (name);

				undoStep -> addUndoFunction (&X3D::X3DExecutionContext::updateExternProtoDeclaration,
				                             executionContext,
				                             name,
				                             externProtoDeclaration);
			}
			catch (const X3D::X3DError &)
			{
				if (isExternProto)
					undoStep -> addUndoFunction (&X3D::X3DExecutionContext::removeExternProtoDeclaration, executionContext, name);

				else
					undoStep -> addUndoFunction (&X3D::X3DExecutionContext::removeProtoDeclaration, executionContext, name);
			}
		}

		const X3D::SFNode protoNode (prototypeInstance -> getProtoDeclarationNode () -> copy (executionContext, X3D::COPY_OR_CLONE));

		if (isExternProto)
		{
			undoStep -> addRedoFunction (&X3D::X3DExecutionContext::updateExternProtoDeclaration,
			                             executionContext,
			                             name,
			                             X3D::ExternProtoDeclarationPtr (protoNode));
		}
		else
		{
			undoStep -> addRedoFunction (&X3D::X3DExecutionContext::updateProtoDeclaration,
			                             executionContext,
			                             name,
			                             X3D::ProtoDeclarationPtr (protoNode));
		}
	}
}

MagicImport::~MagicImport ()
{
	dispose ();
}

} // puck
} // titania
