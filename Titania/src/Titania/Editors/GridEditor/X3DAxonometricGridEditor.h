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

#ifndef __TITANIA_EDITORS_GRID_EDITOR_X3DAXONOMETRIC_GRID_EDITOR_H__
#define __TITANIA_EDITORS_GRID_EDITOR_X3DAXONOMETRIC_GRID_EDITOR_H__

#include "../../ComposedWidgets.h"
#include "../../UserInterfaces/X3DGridEditorInterface.h"

namespace titania {
namespace X3D {

class AxonometricGridTool;

}
}

namespace titania {
namespace puck {

class X3DAxonometricGridEditor :
	virtual public X3DGridEditorInterface
{
public:

	///  @name Destruction

	virtual
	~X3DAxonometricGridEditor () override;


protected:

	///  @name Construction

	X3DAxonometricGridEditor ();

	virtual
	void
	configure () override;

	virtual
	void
	initialize () override;

	virtual
	void
	store () override;


private:

	///  @name Event handlers

	void
	set_angle ();

	void
	set_axonometric_grid_visible ();

	virtual
	void
	on_axonometric_grid_toggled () final override;

	virtual
	void
	on_axonometric_grid_plane_changed () final override;

	void
	set_rotation ();

	void
	connectRotation (const X3D::SFRotation &);

	virtual
	void
	on_axonometric_grid_uniform_scale_clicked () final override;

	virtual
	void
	on_axonometric_major_line_grid_value_changed () final override;

	void
	on_axonometric_major_line_grid_upper_changed ();

	virtual
	void
	on_axonometric_add_major_line_grid () final override;

	virtual
	void
	on_axonometric_remove_major_line_grid () final override;

	void
	set_majorLineEvery ();

	///  @name Members

	X3DFieldToggleButton <X3D::SFBool> enabled;
	X3DFieldAdjustment3 <X3D::SFVec3f> translation;
	X3DFieldAdjustment3 <X3D::SFVec3f> scale;
	X3DFieldAdjustment <X3D::MFInt32>  dimension0;
	X3DFieldAdjustment <X3D::MFInt32>  dimension1;
	X3DFieldAdjustment2 <X3D::SFVec2d> angle;
	X3DFieldAdjustment <X3D::SFDouble> gamma;
	X3DFieldAdjustment <X3D::MFInt32>  majorLineEvery0;
	X3DFieldAdjustment <X3D::MFInt32>  majorLineEvery1;
	X3DFieldAdjustment <X3D::MFInt32>  majorLineEvery2;
	X3DFieldAdjustment <X3D::MFInt32>  majorLineEvery3;
	X3DFieldAdjustment <X3D::MFInt32>  majorLineOffset0;
	X3DFieldAdjustment <X3D::MFInt32>  majorLineOffset1;
	X3DFieldAdjustment <X3D::MFInt32>  majorLineOffset2;
	X3DFieldAdjustment <X3D::MFInt32>  majorLineOffset3;
	SFColorRGBAButton                  color;
	SFColorRGBAButton                  lineColor;
	SFColorRGBAButton                  majorLineColor;
	X3DFieldAdjustment <X3D::SFDouble> snapDistance;
	X3DFieldToggleButton <X3D::SFBool> snapToCenter;
	X3D::SFNode                        fields;
	X3D::UndoStepPtr                   undoStep;
	bool                               changing;

};

} // puck
} // titania

#endif
