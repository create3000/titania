/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraﬂe 31a, Leipzig, Germany 2011.
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

#ifndef __TITANIA_BASE_DIALOG_FACTORY_H__
#define __TITANIA_BASE_DIALOG_FACTORY_H__

#include "../Dialogs/FileImportDialog/FileImportDialog.h"
#include "../Dialogs/FileOpenDialog/FileOpenDialog.h"
#include "../Dialogs/FileSaveDialog/FileSaveDialog.h"
#include "../Dialogs/FileSaveWarningDialog/FileSaveWarningDialog.h"
#include "../Dialogs/OpenLocationDialog/OpenLocationDialog.h"
#include "../Dialogs/NodeIndex/NodeIndex.h"

#include "../Editors/GeometryPropertiesEditor/GeometryPropertiesEditor.h"
#include "../Editors/InlineEditor/InlineEditor.h"
#include "../Editors/LightEditor/LightEditor.h"
#include "../Editors/AppearanceEditor/AppearanceEditor.h"
#include "../Editors/MotionBlurEditor/MotionBlurEditor.h"
#include "../Editors/NodePropertiesEditor/NodePropertiesEditor.h"
#include "../Editors/LODEditor/LODEditor.h"
#include "../Editors/TextEditor/TextEditor.h"
#include "../Editors/TextureEditor/TextureEditor.h"
#include "../Editors/PrototypeInstanceDialog/PrototypeInstanceDialog.h"
#include "../Editors/ViewpointEditor/ViewpointEditor.h"

namespace titania {
namespace puck {

class DialogFactory
{
public:

	///  @name Construction

	DialogFactory () :
		dialogs ({
		            std::make_pair ("FileOpenDialog",           constructDialog <FileOpenDialog>),
		            std::make_pair ("OpenLocationDialog",       constructDialog <OpenLocationDialog>),
		            std::make_pair ("FileImportDialog",         constructDialog <FileImportDialog>),
		            std::make_pair ("FileSaveDialog",           constructDialog <FileSaveDialog>),
		            std::make_pair ("FileSaveWarningDialog",    constructDialog <FileSaveWarningDialog>),
		            std::make_pair ("NodeIndex",                constructDialog <NodeIndex>),
		            std::make_pair ("NodePropertiesEditor",     constructDialog <NodePropertiesEditor>),
		            std::make_pair ("AppearanceEditor",         constructDialog <AppearanceEditor>),
		            std::make_pair ("TextureEditor",            constructDialog <TextureEditor>),
		            std::make_pair ("TextEditor",               constructDialog <TextEditor>),
		            std::make_pair ("GeometryPropertiesEditor", constructDialog <GeometryPropertiesEditor>),
		            std::make_pair ("ViewpointEditor",          constructDialog <ViewpointEditor>),
		            std::make_pair ("LODEditor",                constructDialog <LODEditor>),
		            std::make_pair ("LightEditor",              constructDialog <LightEditor>),
		            std::make_pair ("InlineEditor",             constructDialog <InlineEditor>),
		            std::make_pair ("PrototypeInstanceDialog",  constructDialog <PrototypeInstanceDialog>)
		            //std::make_pair ("MotionBlurEditor",         constructDialog <MotionBlurEditor>)
				   })
	{ }

	std::shared_ptr <X3DUserInterface>
	createDialog (const std::string & name, BrowserWindow* const browserWindow) const
	throw (std::out_of_range)
	{
		return std::shared_ptr <X3DUserInterface> (dialogs .at (name) (browserWindow));
	}

private:

	///  @name Construction

	template <class Dialog>
	static
	X3DUserInterface*
	constructDialog (BrowserWindow* const browserWindow)
	{ return new Dialog (browserWindow); }

	///  @name Members

	std::map <std::string, std::function <X3DUserInterface* (BrowserWindow* const)>>  dialogs;

};

} // puck
} // titania

#endif
