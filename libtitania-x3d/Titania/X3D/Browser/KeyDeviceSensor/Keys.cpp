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

#include "Keys.h"

namespace titania {
namespace X3D {

Keys::Keys () :
	  m_shift (0),
	m_control (0),
	    m_alt (false),
	m_super_l (false),
	m_super_r (false)
{ }

void
Keys::press (GdkEventKey* const event)
{
	switch (event -> keyval)
	{
		case GDK_KEY_Shift_L:
			m_shift |= Shift_L;
			break;
		case GDK_KEY_Shift_R:
			m_shift |= Shift_R;
			break;
		case GDK_KEY_Control_L:
			m_control |= Control_L;
			break;
		case GDK_KEY_Control_R:
			m_control |= Control_R;
			break;
		case GDK_KEY_Meta_L:
		case GDK_KEY_Alt_L:
			m_alt = true;
			break;
		case GDK_KEY_Super_L:
			m_super_l = true;
			break;
		case GDK_KEY_Super_R:
			m_super_r = true;
			break;
		default:
			break;
	}
}

void
Keys::release (GdkEventKey* const event)
{
	switch (event -> keyval)
	{
		case GDK_KEY_Shift_L:
			m_shift &= ~Shift_L;
			break;
		case GDK_KEY_Shift_R:
			m_shift &= ~Shift_R;
			break;
		case GDK_KEY_Control_L:
			m_control &= ~Control_L;
			break;
		case GDK_KEY_Control_R:
			m_control &= ~Control_R;
			break;
		case GDK_KEY_Meta_L:
		case GDK_KEY_Alt_L:
			m_alt = false;
			break;
		case GDK_KEY_Super_L:
			m_super_l = false;
			break;
		case GDK_KEY_Super_R:
			m_super_r = false;
			break;
		default:
			break;
	}
}

bool
Keys::shift () const
{
	return m_shift;
}

bool
Keys::control () const
{
	return m_control;
}

bool
Keys::alt () const
{
	return m_alt;
}

bool
Keys::super_l () const
{
	return m_super_l;
}

bool
Keys::super_r () const
{
	return m_super_r;
}

void
Keys::clear ()
{
	m_shift   = 0;
	m_control = 0;
	m_alt     = false;
	m_super_l = false;
	m_super_r = false;
}

} // X3D
} // titania
