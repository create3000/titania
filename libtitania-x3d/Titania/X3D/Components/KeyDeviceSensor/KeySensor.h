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

#ifndef __TITANIA_X3D_COMPONENTS_KEY_DEVICE_SENSOR_KEY_SENSOR_H__
#define __TITANIA_X3D_COMPONENTS_KEY_DEVICE_SENSOR_KEY_SENSOR_H__

#include "../KeyDeviceSensor/X3DKeyDeviceSensorNode.h"

namespace titania {
namespace X3D {

class KeySensor :
	public X3DKeyDeviceSensorNode
{
public:

	KeySensor (X3DExecutionContext* const);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const) const final override;

	///  @name Common members

	virtual
	const std::string &
	getComponentName () const final override
	{ return componentName; }

	virtual
	const std::string &
	getTypeName () const
	throw (Error <DISPOSED>) final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const final override
	{ return containerField; }

	///  @name Fields

	SFBool &
	shiftKey ()
	{ return *fields .shiftKey; }

	const SFBool &
	shiftKey () const
	{ return *fields .shiftKey; }

	SFBool &
	controlKey ()
	{ return *fields .controlKey; }

	const SFBool &
	controlKey () const
	{ return *fields .controlKey; }

	SFBool &
	altKey ()
	{ return *fields .altKey; }

	const SFBool &
	altKey () const
	{ return *fields .altKey; }

	SFInt32 &
	actionKeyPress ()
	{ return *fields .actionKeyPress; }

	const SFInt32 &
	actionKeyPress () const
	{ return *fields .actionKeyPress; }

	SFInt32 &
	actionKeyRelease ()
	{ return *fields .actionKeyRelease; }

	const SFInt32 &
	actionKeyRelease () const
	{ return *fields .actionKeyRelease; }

	SFString &
	keyPress ()
	{ return *fields .keyPress; }

	const SFString &
	keyPress () const
	{ return *fields .keyPress; }

	SFString &
	keyRelease ()
	{ return *fields .keyRelease; }

	const SFString &
	keyRelease () const
	{ return *fields .keyRelease; }

	virtual
	void
	set_actionKeyPressEvent (int) final override;

	virtual
	void
	set_actionKeyReleaseEvent (int) final override;

	virtual
	void
	set_keyPressEvent (const String &) final override;

	virtual
	void
	set_keyReleaseEvent (const String &) final override;


private:

	static constexpr int Shift_R   = 1;
	static constexpr int Shift_L   = 2;
	static constexpr int Control_R = 1;
	static constexpr int Control_L = 2;


	///  @name Static members

	static const std::string componentName;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFBool* const shiftKey;
		SFBool* const controlKey;
		SFBool* const altKey;
		SFInt32* const actionKeyPress;
		SFInt32* const actionKeyRelease;
		SFString* const keyPress;
		SFString* const keyRelease;
	};

	Fields fields;

	int shiftKeys;
	int controlKeys;

};

} // X3D
} // titania

#endif
