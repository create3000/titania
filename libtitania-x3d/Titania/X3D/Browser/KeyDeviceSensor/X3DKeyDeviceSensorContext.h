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

#ifndef __TITANIA_X3D_BROWSER_KEY_DEVICE_SENSOR_X3DKEY_DEVICE_SENSOR_CONTEXT_H__
#define __TITANIA_X3D_BROWSER_KEY_DEVICE_SENSOR_X3DKEY_DEVICE_SENSOR_CONTEXT_H__

#include "../../Basic/X3DBaseNode.h"
#include "../../Fields.h"
#include "Keys.h"

namespace titania {
namespace X3D {

class X3DKeyDeviceSensorNode;

class X3DKeyDeviceSensorContext :
	virtual public X3DBaseNode
{
public:

	///  @name Member access

	void
	setControlKey (const bool value);

	const SFBool &
	getControlKey () const
	{ return controlKey; }

	void
	setShiftKey (const bool value);

	const SFBool &
	getShiftKey () const
	{ return shiftKey; }

	void
	setAltKey (const bool value);

	const SFBool &
	getAltKey () const
	{ return altKey; }

	///  @name Event handlers

	bool
	on_external_focus_out_event (GdkEventFocus* event);

	bool
	on_external_key_press_event (GdkEventKey* event);

	bool
	on_external_key_release_event (GdkEventKey* event);

	///  @name Destruction

	virtual
	void
	dispose () override
	{ }

	virtual
	~X3DKeyDeviceSensorContext () override;


protected:

	///  @name Friends

	friend class KeyDevice;
	friend class X3DKeyDeviceSensorNode;

	///  @name Construction

	X3DKeyDeviceSensorContext ();

	virtual
	void
	initialize () override;

	///  @name Member access

	void
	addKeyDeviceSensor (X3DKeyDeviceSensorNode* const keyDeviceSensorNode);

	void
	removeKeyDeviceSensor (X3DKeyDeviceSensorNode* const keyDeviceSensorNode);

	const X3DWeakPtrArray <X3DKeyDeviceSensorNode> &
	getKeyDeviceSensors () const
	{ return keyDeviceSensorNodes; }


private:

	///  @name Event handlers

	void
	on_external_key_event ();

	///  @name Members

	X3DPtr <KeyDevice>                       keyDevice;
	X3DWeakPtrArray <X3DKeyDeviceSensorNode> keyDeviceSensorNodes;
	SFBool                                   controlKey;
	SFBool                                   shiftKey;
	SFBool                                   altKey;
	bool                                     internalControlKey;
	bool                                     internalShiftKey;
	bool                                     internalAltKey;
	Keys                                     externalKeys;

};

} // X3D
} // titania

#endif
