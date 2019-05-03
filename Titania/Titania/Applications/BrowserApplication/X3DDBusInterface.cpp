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

#include "BrowserApplication.h"

#include "../../Bits/File.h"
#include "../../Browser/BrowserSelection.h"
#include "../../Browser/BrowserWindow.h"
#include "../../Widgets/Console/Console.h"

#include <Titania/X3D/Editing/X3DEditor.h>
#include <Titania/String.h>

namespace titania {
namespace puck {

const Glib::ustring X3DDBusInterface::introspectionXML =
"<node>"
"  <interface name='de.create3000.Titania'>"
"    <method name='GetCurrentScene'>"
"      <arg type='s' name='encoding' direction='in'/>"
"      <arg type='s' name='x3dSyntax' direction='out'/>"
"    </method>"
"    <method name='GetSelection'>"
"      <arg type='s' name='encoding' direction='in'/>"
"      <arg type='s' name='x3dSyntax' direction='out'/>"
"    </method>"
"    <method name='ReplaceSelection'>"
"      <arg type='s' name='pluginName' direction='in'/>"
"      <arg type='s' name='x3dSyntax' direction='in'/>"
"      <arg type='b' name='assign' direction='in'/>"
"    </method>"
"    <method name='Open'>"
"      <arg type='s' name='x3dSyntax' direction='in'/>"
"    </method>"
"    <signal name='CurrentSceneChanged'></signal>"
"    <signal name='SelectionChanged'></signal>"
"  </interface>"
"</node>";

X3DDBusInterface::X3DDBusInterface () :
	X3DBrowserApplication (),
	    introspectionData (),
	      interfaceVTable (sigc::mem_fun (this, &BrowserApplication::on_method_call)),
	         registeredId (0)
{ }

void
X3DDBusInterface::realize ()
{
	try
	{
		if (not get_dbus_connection ())
			return;

		introspectionData = Gio::DBus::NodeInfo::create_for_xml (introspectionXML);
      registeredId      = get_dbus_connection () -> register_object ("/de/create3000/titania", introspectionData -> lookup_interface (), interfaceVTable);

		getBrowserWindow () -> getCurrentScene ()             .addInterest (&X3DDBusInterface::set_current_scene, this);
		getBrowserWindow () -> getSelection () -> getNodes () .addInterest (&X3DDBusInterface::set_selection,     this);
	}
	catch (const Glib::Error & error)
	{
		std::cerr << "Titania: Registration of DBus object failed: " << error .what () << "." << std::endl;
	}
}

void
X3DDBusInterface::set_current_scene ()
{
	get_dbus_connection () -> emit_signal ("/de/create3000/titania", "de.create3000.Titania", "CurrentSceneChanged");
}

void
X3DDBusInterface::set_selection ()
{
	get_dbus_connection () -> emit_signal ("/de/create3000/titania", "de.create3000.Titania", "SelectionChanged");
}

void
X3DDBusInterface::on_method_call (const Glib::RefPtr <Gio::DBus::Connection> & connection,
                                  const Glib::ustring & sender,
                                  const Glib::ustring & object_path,
                                  const Glib::ustring & interface_name,
                                  const Glib::ustring & method_name,
                                  const Glib::VariantContainerBase & parameters,
                                  const Glib::RefPtr <Gio::DBus::MethodInvocation> & invocation)
{
	try
	{
		using namespace std::placeholders;

		using Method = std::function <void (const Glib::VariantContainerBase &, const Glib::RefPtr <Gio::DBus::MethodInvocation> &)>;

		static const std::map <std::string, Method> functions = {
			std::pair ("GetCurrentScene",  std::bind (&X3DDBusInterface::getCurrentScene,  this, _1, _2)),
			std::pair ("GetSelection",     std::bind (&X3DDBusInterface::getSelection,     this, _1, _2)),
			std::pair ("ReplaceSelection", std::bind (&X3DDBusInterface::replaceSelection, this, _1, _2)),
			std::pair ("Open",             std::bind (&X3DDBusInterface::open,             this, _1, _2)),
		};

		functions .at (method_name) (parameters, invocation);
	}
	catch (const std::out_of_range &)
	{
		// Non-existent method on the interface.
		Gio::DBus::Error error (Gio::DBus::Error::UNKNOWN_METHOD, "Method does not exist.");

		invocation -> return_error (error);
	}
}

void
X3DDBusInterface::getCurrentScene (const Glib::VariantContainerBase & parameters,
                                   const Glib::RefPtr <Gio::DBus::MethodInvocation> & invocation) const
{
	Glib::Variant <Glib::ustring> encoding;

	parameters .get_child (encoding, 0);

	const auto & currentScene = getBrowserWindow () -> getCurrentScene ();
	const auto   x3dSyntax    = X3D::X3DEditor::exportScene (currentScene, encoding .get ());
	const auto   x3dSyntaxVar = Glib::Variant <Glib::ustring>::create (x3dSyntax);
	const auto   response     = Glib::VariantContainerBase::create_tuple (x3dSyntaxVar);

	invocation -> return_value (response);
}

void
X3DDBusInterface::getSelection (const Glib::VariantContainerBase & parameters,
                                const Glib::RefPtr <Gio::DBus::MethodInvocation> & invocation) const
{
	Glib::Variant <Glib::ustring> encoding;

	parameters .get_child (encoding, 0);

	const auto & executionContext = getBrowserWindow () -> getCurrentContext ();
	const auto & selection        = getBrowserWindow () -> getSelection () -> getNodes ();
	const auto   x3dSyntax        = X3D::X3DEditor::exportNodes (executionContext, selection, encoding .get (), false);
	const auto   x3dSyntaxVar     = Glib::Variant <Glib::ustring>::create (x3dSyntax);
	const auto   response         = Glib::VariantContainerBase::create_tuple (x3dSyntaxVar);

	invocation -> return_value (response);
}

void
X3DDBusInterface::replaceSelection (const Glib::VariantContainerBase & parameters,
                                    const Glib::RefPtr <Gio::DBus::MethodInvocation> & invocation) const
{
	Glib::Variant <Glib::ustring> pluginName;
	Glib::Variant <Glib::ustring> x3dSyntax;
	Glib::Variant <bool>          assign;

	parameters .get_child (pluginName, 0);
	parameters .get_child (x3dSyntax,  1);
	parameters .get_child (assign,     2);

	const auto & selection = getBrowserWindow () -> getSelection () -> getNodes ();

	if (not selection .empty ())
	{
		const auto executionContext = X3D::X3DExecutionContextPtr (getBrowserWindow () -> getSelectionContext (selection));

		if (executionContext)
		{
			const auto undoStep = assign .get ()
			                      ? std::make_shared <X3D::UndoStep> (_ (basic::sprintf (_ ("Assign Output From Tool »%s« To Selection"),  pluginName .get () .c_str ())))
			                      : std::make_shared <X3D::UndoStep> (_ (basic::sprintf (_ ("Replace Selection By Output From Tool »%s«"), pluginName .get () .c_str ())));

			const auto newSelection = X3D::X3DEditor::replaceNodes (executionContext, x3dSyntax .get (), selection, assign .get (), undoStep);

			getBrowserWindow () -> getSelection () -> setNodes (newSelection, undoStep);
			getBrowserWindow () -> addUndoStep (undoStep);
		}
		else
		{
			getBrowserWindow () -> getCurrentBrowser () -> getConsole () -> warn ("Can't assing output of tool »", pluginName .get (), "« to nodes of multiple contexts.\n");
		}
	}
	else
	{
		// Display message.
		getBrowserWindow () -> getCurrentBrowser () -> getConsole () -> warn ("No selection found to process output of tool »", pluginName .get (), "«.\n");
	}

	invocation -> return_value (Glib::VariantContainerBase ());
}

void
X3DDBusInterface::open (const Glib::VariantContainerBase & parameters,
                        const Glib::RefPtr <Gio::DBus::MethodInvocation> & invocation) const
{
	Glib::Variant <Glib::ustring> x3dSyntax;

	parameters .get_child (x3dSyntax, 0);

	getBrowserWindow () -> open ("data:" + File::getContentType (x3dSyntax .get ()) .first + "," + x3dSyntax .get () .raw ());

	invocation -> return_value (Glib::VariantContainerBase ());
}

X3DDBusInterface::~X3DDBusInterface ()
{ }

} // puck
} // titania
