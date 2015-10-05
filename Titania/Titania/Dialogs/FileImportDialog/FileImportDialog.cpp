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

#include "FileImportDialog.h"

#include "../../Browser/BrowserSelection.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"

#include <Titania/X3D/Prototype/ProtoDeclaration.h>
#include <Titania/X3D/Prototype/ExternProtoDeclaration.h>
#include <Titania/OS.h>

namespace titania {
namespace puck {

namespace ImportType {

static constexpr int EXTERN_PROTOS = 0;
static constexpr int PROTOS        = 1;
static constexpr int SCENE         = 2;
static constexpr int INLINE        = 3;

}

FileImportDialog::FileImportDialog (X3DBrowserWindow* const browserWindow) :
	            X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	X3DFileImportDialogInterface (get_ui ("Dialogs/FileImportDialog.xml"), gconf_dir ())
{
	if (getConfig () .hasItem ("importType"))
	{
		switch (getConfig () .getInteger ("importType"))
		{
			case ImportType::EXTERN_PROTOS:
				getImportExternprotosButton () .set_active (true);
				break;
		   case ImportType::PROTOS:
				getImportProtosButton () .set_active (true);
				break;
		   case ImportType::SCENE:
				getImportSceneButton () .set_active (true);
				break;
		   default:
				getImportAsInlineButton () .set_active (true);
				break;
		}
	}

	getFileFilterX3D   () -> set_name (_ ("X3D"));
	getFileFilterImage () -> set_name (_ ("Images"));
	getFileFilterAudio () -> set_name (_ ("Audio"));
	getFileFilterVideo () -> set_name (_ ("Videos"));
	getFileFilterAll   () -> set_name (_ ("All Files"));

	getWindow () .add_filter (getFileFilterX3D ());
	getWindow () .add_filter (getFileFilterImage ());
	getWindow () .add_filter (getFileFilterAudio ());
	getWindow () .add_filter (getFileFilterVideo ());
	getWindow () .set_filter (getFileFilterX3D ());

	const auto worldURL = getRootContext () -> getWorldURL ();

	if (not worldURL .empty () and worldURL .is_local ())
		getWindow () .set_uri (worldURL .filename () .str ());

	else
		getWindow () .set_filename (os::home () + _ ("scene.x3dv"));

	setup ();
}

void
FileImportDialog::setFilter (const std::string & name)
{
	if (name == _("Images"))
		getWindow () .set_filter (getFileFilterImage ());
	else if (name == _("Audio"))
		getWindow () .set_filter (getFileFilterAudio ());
	else if (name == _("Videos"))
		getWindow () .set_filter (getFileFilterVideo ());
	else
		getWindow () .set_filter (getFileFilterX3D ());
}

void
FileImportDialog::run ()
{
	const auto responseId = getImportDialog () .run ();

	getImportDialog () .hide ();

	if (responseId == Gtk::RESPONSE_OK)
	{
		if (getConfig () .hasItem ("currentFolder"))
			getWindow () .set_current_folder_uri (getConfig () .getString ("currentFolder"));
		
		setFilter (getConfig () .getString ("filter"));
		
		const auto responseId = getWindow () .run ();

		getConfig () .setItem ("currentFolder", getWindow () .get_current_folder_uri ());

		if (getWindow () .get_filter ())
			getConfig () .setItem ("filter", getWindow () .get_filter () -> get_name ());

		if (responseId == Gtk::RESPONSE_OK)
		{
		   if (getImportExternprotosButton () .get_active ())
		   {
				getConfig () .setItem ("importType", ImportType::EXTERN_PROTOS);

				try
				{
					const auto uri   = getWindow () .get_uri ();
					const auto scene = getBrowser () -> createX3DFromURL ({ Glib::uri_unescape_string (uri) });

					for (const auto & prototype : scene -> getProtoDeclarations ())
					{
						const auto & name = prototype -> getName ();

						X3D::FieldDefinitionArray _externInterfaceDeclarations;

						for (const auto & fieldDefinition : prototype -> getFieldDefinitions ())
						{
						   _externInterfaceDeclarations .emplace_back (fieldDefinition -> copy (X3D::FLAT_COPY));
							_externInterfaceDeclarations .back () -> setAccessType (fieldDefinition -> getAccessType ());
							_externInterfaceDeclarations .back () -> setName (fieldDefinition -> getName ());
						}

						basic::uri worldURL (uri + "#" + name);
			
						const auto relativePath = getExecutionContext () -> getWorldURL () .relative_path (worldURL);

						X3D::MFString url;

						url .emplace_back (relativePath .str ());
						url .emplace_back (worldURL .str ());
						
						const X3D::ExternProtoDeclarationPtr externproto = getExecutionContext () -> createExternProtoDeclaration (name, _externInterfaceDeclarations, url);

						getExecutionContext () -> updateExternProtoDeclaration (name, externproto);
					}
				}
				catch (const X3D::X3DError & error)
				{
					getBrowser () -> print (error .what ());
				}
			}

			else if (getImportProtosButton () .get_active ())
			{
				try
				{
					getConfig () .setItem ("importType", ImportType::PROTOS);

					const auto scene = getBrowser () -> createX3DFromURL ({ Glib::uri_unescape_string (getWindow () .get_uri ()) });

					for (const auto & prototype : scene -> getProtoDeclarations ())
						getExecutionContext () -> addProtoDeclaration (prototype -> getName (), prototype);
				}
				catch (const X3D::X3DError & error)
				{
					getBrowser () -> print (error .what ());
				}
			}

			else if (getImportSceneButton () .get_active ())
			{
				getConfig () .setItem ("importType", ImportType::SCENE);

				const auto undoStep = std::make_shared <UndoStep> (_ ("Import As Inline"));

				const auto nodes = getBrowserWindow () -> importURL ({ Glib::uri_unescape_string (getWindow () .get_uri ()) },
				                                                     false,
				                                                     undoStep);

				getBrowserWindow () -> getSelection () -> setChildren (nodes, undoStep);
				getBrowserWindow () -> addUndoStep (undoStep);
			}

			else if (getImportAsInlineButton () .get_active ())
			{
				getConfig () .setItem ("importType", ImportType::INLINE);

				const auto undoStep = std::make_shared <UndoStep> (_ ("Import"));

				const auto nodes = getBrowserWindow () -> importURL ({ Glib::uri_unescape_string (getWindow () .get_uri ()) },
				                                                     true,
				                                                     undoStep);

				getBrowserWindow () -> getSelection () -> setChildren (nodes, undoStep);
				getBrowserWindow () -> addUndoStep (undoStep);
			}

		}
	}

	quit ();
}

FileImportDialog::~FileImportDialog ()
{
	dispose ();
}

} // puck
} // titania
