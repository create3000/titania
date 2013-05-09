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

#include <Titania/X3D.h>
#include <gtkmm/plug.h>

extern "C"
{
#define MOZ_X11
#include <npapi.h>
#include <npfunctions.h>
}

#include <Titania/LOG.h>
#include <Titania/OS/System.h>

using namespace titania;
using namespace titania::X3D;

struct Plugin
{
public:

	Plugin (const SFNode <Browser> & browser) :
		window (0),
		plug (NULL),
		browser (browser)
	{ }

	Window window;
	Gtk::Plug* plug;
	SFNode <Browser> browser;

};

static std::string PLUGIN_VERSION     = "0.1";
static std::string PLUGIN_NAME        = "Titania X3D plugin";
static std::string PLUGIN_DESCRIPTION = PLUGIN_NAME;
static std::string MIME_TYPES         = "model/vrml:wrl:" + PLUGIN_NAME + ";" +
                                        "model/x3d-vrml:x3dv:" + PLUGIN_NAME + ";" +
                                        "x-world/x-vrml:wrl:" + PLUGIN_NAME;

NPError
NP_Initialize (NPNetscapeFuncs* bFuncs, NPPluginFuncs* pFuncs)
{
	__LOG__ << std::endl;

	// Check the size of the provided structure based on the offset of the last member we need.
	pFuncs -> newp          = NPP_New;
	pFuncs -> destroy       = NPP_Destroy;
	pFuncs -> setwindow     = NPP_SetWindow;
	pFuncs -> newstream     = NPP_NewStream;
	pFuncs -> destroystream = NPP_DestroyStream;
	pFuncs -> asfile        = NPP_StreamAsFile;
	pFuncs -> writeready    = NPP_WriteReady;
	pFuncs -> write         = NPP_Write;
	pFuncs -> print         = NPP_Print;
	pFuncs -> event         = NPP_HandleEvent;
	pFuncs -> urlnotify     = NPP_URLNotify;
	pFuncs -> getvalue      = NPP_GetValue;
	pFuncs -> setvalue      = NPP_SetValue;

	//Gtk::Main::init_gtkmm_internals ();

	return NPERR_NO_ERROR;
}

NP_EXPORT (char*) NP_GetPluginVersion ()
{
	__LOG__ << std::endl;
	return (char*) PLUGIN_VERSION .c_str ();
}

NP_EXPORT (const char*) NP_GetMIMEDescription ()
{
	__LOG__ << std::endl;
	return MIME_TYPES .c_str ();
}

NPError
NP_GetValue (void* future, NPPVariable aVariable, void* aValue)
{
	__LOG__ << std::endl;
	return NPERR_NO_ERROR;
}

NPError OSCALL
NP_Shutdown ()
{
	__LOG__ << std::endl;
	return NPERR_NO_ERROR;
}

NPError
NPP_New (NPMIMEType pluginType, NPP instance, uint16_t mode, int16_t argc, char* argn [ ], char* argv [ ], NPSavedData* saved)
{
	__LOG__ << std::endl;

	char* src = NULL;

	for (int i = 0; i < argc; ++ i)
	{
		__LOG__ << argn [i] << ": " << argv [i] << std::endl;

		if (argn [i] == std::string ("src"))
			src = argv [i];
	}

	const gchar* cmd [ ] = { "titania", src, NULL };

	g_spawn_async_with_pipes (NULL, (gchar**) cmd, NULL,
	                          GSpawnFlags (G_SPAWN_DO_NOT_REAP_CHILD |
	                                       G_SPAWN_SEARCH_PATH |
	                                       G_SPAWN_STDOUT_TO_DEV_NULL |
	                                       G_SPAWN_STDERR_TO_DEV_NULL),
	                          NULL, NULL, NULL, NULL, NULL, NULL,
	                          NULL);

	instance -> pdata = new Plugin (createBrowser ());

	return NPERR_NO_ERROR;
}

NPError
NPP_Destroy (NPP instance, NPSavedData** save)
{
	__LOG__ << std::endl;

	if (not instance)
		return NPERR_INVALID_INSTANCE_ERROR;

	auto self = static_cast <Plugin*> (instance -> pdata);

	if (self)
		delete self;

	return NPERR_NO_ERROR;
}

NPError
NPP_SetWindow (NPP instance, NPWindow* window)
{
	__LOG__ << std::endl;
	__LOG__ << "window: " << window -> window << std::endl;
	__LOG__ << "width: "  << window -> width << std::endl;
	__LOG__ << "height: " << window -> height << std::endl;

	auto self = static_cast <Plugin*> (instance -> pdata);

	if (self -> window == (Window) window -> window)
		return NPERR_NO_ERROR;

	self -> window = (Window) window -> window;

	//self -> plug = new Gtk::Plug ((Window) window -> window);
	//self -> plug -> show ();

	//self -> plug -> add (**self -> browser);
	//self -> browser -> show ();

	__LOG__ << std::endl;
	return NPERR_NO_ERROR;
}

NPError
NPP_NewStream (NPP instance, NPMIMEType type, NPStream* stream, NPBool seekable, uint16_t* stype)
{
	__LOG__ << std::endl;
	__LOG__ << stream -> url << std::endl;
	return NPERR_GENERIC_ERROR;
}

NPError
NPP_DestroyStream (NPP instance, NPStream* stream, NPReason reason)
{
	__LOG__ << std::endl;
	return NPERR_GENERIC_ERROR;
}

int32_t
NPP_WriteReady (NPP instance, NPStream* stream)
{
	__LOG__ << std::endl;
	return 0;
}

int32_t
NPP_Write (NPP instance, NPStream* stream, int32_t offset, int32_t len, void* buffer)
{
	__LOG__ << std::endl;
	return 0;
}

void
NPP_StreamAsFile (NPP instance, NPStream* stream, const char* fname)
{ }

void
NPP_Print (NPP instance, NPPrint* platformPrint)
{
	__LOG__ << std::endl;
}

int16_t
NPP_HandleEvent (NPP instance, void* event)
{
	__LOG__ << std::endl;
	return 1;
}

void
NPP_URLNotify (NPP instance, const char* URL, NPReason reason, void* notifyData)
{
	__LOG__ << std::endl;
}

NPError
NPP_GetValue (NPP instance, NPPVariable variable, void* value)
{
	__LOG__ << std::endl;

	NPError err = NPERR_NO_ERROR;

	switch (variable)
	{
		case NPPVpluginNameString:
			*((const char**) value) = PLUGIN_NAME .c_str ();
			break;
		case NPPVpluginDescriptionString:
			*((const char**) value) = PLUGIN_DESCRIPTION .c_str ();
			break;
		case NPPVpluginNeedsXEmbed:
			*((NPBool*) value) = true;
			break;
		default:
			err = NPERR_GENERIC_ERROR;
	}

	return err;
}

NPError
NPP_SetValue (NPP instance, NPNVariable variable, void* value)
{
	__LOG__ << std::endl;
	return NPERR_GENERIC_ERROR;
}
