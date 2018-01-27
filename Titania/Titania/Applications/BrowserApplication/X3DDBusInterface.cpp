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

#include "../../Browser/BrowserSelection.h"
#include "../../Browser/BrowserWindow.h"

#include <Titania/X3D/Editing/X3DEditor.h>

namespace titania {
namespace puck {

const Glib::ustring X3DDBusInterface::introspectionXML =
"<node>"
"  <interface name='de.create3000.titania'>"
"    <method name='GetSelection'>"
"      <arg type='s' name='encoding' direction='in'/>"
"      <arg type='s' name='x3dSyntax' direction='out'/>"
"    </method>"
"    <method name='ReplaceSelection'>"
"      <arg type='s' name='x3dSyntax' direction='in'/>"
"    </method>"
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

		__LOG__ << get_id () << std::endl;
		__LOG__ << get_dbus_object_path () << std::endl;
		__LOG__ << is_registered () << std::endl;
		__LOG__ << registeredId << std::endl;
	}
	catch (const Glib::Error & error)
	{
		std::cerr << "Titania: Registration of DBus object failed: " << error .what () << "." << std::endl;
	}
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
	__LOG__ << method_name << std::endl;

	if (method_name == "GetSelection")
	{
		Glib::Variant <Glib::ustring> encoding;

		parameters .get_child (encoding, 0);

		const auto x3dSyntaxVar = Glib::Variant <Glib::ustring>::create (getSelection (encoding .get ()));
		const auto response     = Glib::VariantContainerBase::create_tuple (x3dSyntaxVar);
		
		invocation -> return_value (response);
	}
}

std::string
X3DDBusInterface::getSelection (const std::string & encoding) const
{
	const auto & selection        = getBrowserWindow () -> getSelection () -> getNodes ();
	const auto   executionContext = X3D::X3DExecutionContextPtr (selection .back () -> getExecutionContext ());

	// Export nodes to stream

	std::ostringstream osstream;

	X3D::X3DEditor::exportNodes (osstream, executionContext, selection, false);

	basic::ifilestream stream (osstream .str ());

	const auto scene = getBrowserWindow () -> getCurrentBrowser () -> createX3DFromStream (executionContext -> getWorldURL (), stream);

	scene -> addMetaData ("titania-add-metadata", "true");

	if (encoding == "VRML")
		return scene -> toString ();
	else if (encoding == "JSON")
		return scene -> toJSONString ();

	return scene -> toXMLString ();
}

X3DDBusInterface::~X3DDBusInterface ()
{ }

} // puck
} // titania
