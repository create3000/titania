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

#include "PyBrowser.h"

#include <Titania/X3D.h>

namespace titania {
namespace python {

PyTypeObject PyBrowser::type = {
	PyObject_HEAD_INIT (NULL)
};

PyMethodDef PyBrowser::methods [ ] = {
	{ "getName", (PyCFunction) &PyBrowser::getName, METH_NOARGS, "getName" },
	{ NULL }
};

void
PyBrowser::init (PyObject* module)
{
	type .tp_new       = PyType_GenericNew;
	type .tp_name      = "Browser";
	type .tp_basicsize = sizeof (Object);
	type .tp_flags     = Py_TPFLAGS_DEFAULT;
	type .tp_init      = &PyBrowser::construct;
	type .tp_methods   = methods;
	type .tp_str       = &PyBrowser::toString;
	type .tp_dealloc   = (destructor) &PyBrowser::dealloc;

	if (PyType_Ready (&type) < 0)
		return;

	PyModule_AddObject (module, "Browser", (PyObject*) &type);
}

int
PyBrowser::construct (PyObject* self,
                    PyObject* args,
                    PyObject* kwords)
{
	auto object = (Object*) self;

	object -> field = new X3D::SFNode <X3D::Browser> (X3D::createBrowser ());

	__LOG__ << object << std::endl;

	return 0;
}

PyObject*
PyBrowser::getName (Object* self, PyObject *args)
{
__LOG__ << std::endl;
__LOG__ << self << std::endl;
__LOG__ << self -> field << std::endl;
__LOG__ << self -> field -> getNodeName () << std::endl;
	return PyString_FromString (self -> field -> getNodeName () [0] .c_str ());
}

PyObject*
PyBrowser::toString (PyObject* self)
{
__LOG__ << std::endl;
__LOG__ << self << std::endl;
__LOG__ << ((Object*) self) -> field << std::endl;
__LOG__ << ((Object*) self) -> field -> getNodeName () << std::endl;
	return PyString_FromString (((Object*) self) -> field -> toString () .c_str ());
}

void
PyBrowser::dealloc (Object* self)
{
	delete self -> field;
	self -> ob_type -> tp_free ((PyObject*) self);
}

} // python
} // titania

