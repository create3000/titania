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

#ifndef __TITANIA_COMPOSED_WIDGETS_MFSTRING_COMBO_BOX_TEXT_H__
#define __TITANIA_COMPOSED_WIDGETS_MFSTRING_COMBO_BOX_TEXT_H__

#include "../ComposedWidgets/X3DComposedWidget.h"

namespace titania {
namespace puck {

class MFStringComboBoxText :
	public X3DComposedWidget
{
public:

	///  @name Construction

	MFStringComboBoxText (X3DBaseInterface* const,
	                      Gtk::ComboBoxText &,
	                      const std::string &,
	                      const size_t,
	                      const std::string &);

	///  @name Member access

	void
	setNodes (const X3D::MFNode &);

	const X3D::MFNode &
	getNodes ()
	{ return nodes; }

	///  @name Destruction

	virtual
	~MFStringComboBoxText () final override
	{ dispose (); }


private:

	///  @name Event handlers

	void
	on_changed ();

	void
	set_field ();

	void
	set_buffer ();

	void
	connect (const X3D::MFString &);

	template <class NodeType>
	std::pair <X3D::String, int>
	getString (const X3D::X3DPtrArray <NodeType> &, const std::string &, const size_t, const std::string &);

	///  @name Members

	Gtk::ComboBoxText & comboBoxText;
	X3D::MFNode         nodes;
	const std::string   name;
	const size_t        index;
	const std::string   defaultValue;
	X3D::UndoStepPtr    undoStep;
	bool                changing;
	X3D::SFTime         buffer;

};

/***
 *  Returns the string in @a nodes in field @a fieldName. The second value indicates if the field
 *  was found and what value it has, where:
 *  -2 means no field named @a fieldName found
 *  -1 means inconsistent
 *   0 means all values are empty
 *   1 means all values are equal
 */
template <class NodeType>
std::pair <X3D::String, int>
MFStringComboBoxText::getString (const X3D::X3DPtrArray <NodeType> & nodes, const std::string & fieldName, const size_t index, const std::string & defaultValue)
{
	X3D::String found;
	int         active = -2;

	for (const auto & node : basic::make_reverse_range (nodes))
	{
		try
		{
			const auto & field = node -> template getField <X3D::MFString> (fieldName);

			if (active < 0)
			{
				try
				{
					found = field .at (index);
				}
				catch (const std::exception &)
				{
					found = defaultValue;
				}

				active = not found .empty ();
			}
			else if (field .at (index) not_eq found)
			{
				if (found .empty ())
				{
					try
					{
						found = field .at (index);
					}
					catch (const std::exception &)
					{
						found = defaultValue;
					}
				}

				active = -1;
				break;
			}
		}
		catch (const X3D::X3DError &)
		{ }
	}

	return std::pair (std::move (found), active);
}

} // puck
} // titania

#endif
