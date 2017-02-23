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
 *	virtual
	void
	set_selection (const X3D::MFNode & selection) final override;

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

#ifndef __TITANIA_TEXT_EDITOR_TEXT_EDITOR_H__
#define __TITANIA_TEXT_EDITOR_TEXT_EDITOR_H__

#include "../../ComposedWidgets.h"
#include "../../ComposedWidgets/MFStringTextView.h"
#include "../../UserInterfaces/X3DTextEditorInterface.h"
#include "X3DFontStyleNodeEditor.h"

namespace titania {
namespace puck {

class TextEditor :
	virtual public X3DTextEditorInterface,
	public X3DFontStyleNodeEditor
{
public:

	///  @name Construction

	TextEditor (X3DBrowserWindow* const browserWindow);
	
	virtual
	~TextEditor () final override;


private:

	///  @name Construction

	virtual
	void
	configure () final override;

	virtual
	void
	initialize () final override;

	virtual
	void
	set_selection (const X3D::MFNode & selection) final override;

	///  @name text

	virtual
	void
	on_text_unlink_clicked () final override;

	virtual
	void
	on_text_toggled () final override;

	void
	set_geometry ();

	void
	connectGeometry (const X3D::SFNode &);

	void
	set_node ();

	///  @name string

	void
	set_lineBounds ();

	virtual
	void
	on_char_spacing_changed () final override;

	void
	set_char_spacing (const double);

	void
	set_length ();

	void
	connectLength (const X3D::MFFloat &);

	virtual
	void
	store () final override;

	///  @name Members

	MFStringTextView                  string;
	X3DFieldAdjustment <X3D::SFFloat> maxExtent;

	X3D::X3DPtrArray <X3D::X3DShapeNode> shapeNodes;
	X3D::SFTime                          geometryNodeBuffer;
	X3D::X3DPtr <X3D::Text>              text;
	X3D::X3DPtr <X3D::Text>              measure;
	X3D::UndoStepPtr                     undoStep;
	X3D::UndoStepPtr                     lengthUndoStep;
	bool                                 changing;


};

} // puck
} // titania

#endif
