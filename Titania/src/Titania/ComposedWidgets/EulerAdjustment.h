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

#ifndef __TITANIA_COMPOSED_WIDGETS_EULER_ADJUSTMENT_H__
#define __TITANIA_COMPOSED_WIDGETS_EULER_ADJUSTMENT_H__

#include "../ComposedWidgets/X3DComposedWidget.h"

namespace titania {
namespace puck {

class EulerAdjustment :
	public X3DComposedWidget
{
public:

	///  @name Construction

	EulerAdjustment (X3DBaseInterface* const editor,
	                      const Glib::RefPtr <Gtk::Adjustment> & adjustmentX,
	                      const Glib::RefPtr <Gtk::Adjustment> & adjustmentY,
	                      const Glib::RefPtr <Gtk::Adjustment> & adjustmentZ,
	                      Gtk::Widget & widget,
	                      const std::string & name);

	///  @name Member access

	void
	setNodes (const X3D::MFNode & value);

	const X3D::MFNode &
	getNodes ()
	{ return nodes; }

	///  @name Destruction

	virtual
	~EulerAdjustment () final override;


private:

	///  @name Event handlers

	void
	set_scene ();

	void
	on_value_changed (const int id);

	void
	set_field ();

	void
	set_buffer ();

	void
	set_bounds ();

	void
	connect (const X3D::SFRotation & field);

	X3D::Rotation4d
	fromEuler (const X3D::Vector3d & euler);
	
	X3D::Vector3d
	toEuler (const X3D::Rotation4d & rotation);

	///  @name Members

	const std::vector <Glib::RefPtr <Gtk::Adjustment>>  adjustments;
	Gtk::Widget &                                       widget;
	X3D::X3DScenePtr                                    scene;
	X3D::MFNode                                         nodes;
	const std::string                                   name;
	X3D::UndoStepPtr                                    undoStep;
	int                                                 input;
	bool                                                changing;
	X3D::SFTime                                         buffer;

};

} // puck
} // titania

#endif
