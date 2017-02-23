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

#ifndef __TITANIA_TEXT_EDITOR_X3DFONT_STYLE_EDITOR_H__
#define __TITANIA_TEXT_EDITOR_X3DFONT_STYLE_EDITOR_H__

#include "../../ComposedWidgets.h"
#include "../../UserInterfaces/X3DTextEditorInterface.h"

#include <Titania/X3D/Components/Text/Text.h>
#include <Titania/X3D/Components/Text/FontStyle.h>
#include <Titania/X3D/Components/Layout/ScreenFontStyle.h>

namespace titania {
namespace puck {

class MFStringFamilyWidget;

class X3DFontStyleNodeEditor :
	virtual public X3DTextEditorInterface
{
public:

	///  @name Destruction

	virtual
	~X3DFontStyleNodeEditor () override;


protected:

	///  @name Construction

	X3DFontStyleNodeEditor ();

	virtual
	void
	initialize () override;

	virtual
	void
	set_selection (const X3D::MFNode & selection) override;


private:

	///  @name fontStyle
	
	virtual
	void
	on_fontStyle_unlink_clicked () final override;

	virtual
	void
	on_fontStyle_changed () final override;

	void
	set_fontStyle ();

	void
	connectFontStyle (const X3D::SFNode &);

	void
	set_node ();

	void
	set_widgets ();

	///  @name style

	virtual
	void
	on_style_toggled () final override;

	void
	set_style ();

	void
	connectStyle (const X3D::SFString & field);

	///  @name size
	
	void
	on_size_sensitive_changed ();
	
	void
	on_point_size_sensitive_changed ();

	///  @name Members

	std::unique_ptr <MFStringFamilyWidget> family;
	X3DFieldAdjustment <X3D::SFFloat>      size;
	X3DFieldAdjustment <X3D::SFFloat>      pointSize;
	X3DFieldAdjustment <X3D::SFFloat>      spacing;
	X3DFieldToggleButton <X3D::SFBool>     horizontal;
	X3DFieldToggleButton <X3D::SFBool>     leftToRight;
	X3DFieldToggleButton <X3D::SFBool>     topToBottom;
	MFStringComboBoxText                   majorAlignment;          
	MFStringComboBoxText                   minorAlignment;          
	X3DFieldAdjustment <X3D::SFInt32>      bezierDimension;

	X3D::X3DPtrArray <X3D::Text>        texts;
	X3D::SFTime                         fontStyleNodeBuffer;
	X3D::X3DPtr <X3D::X3DFontStyleNode> fontStyleNode;
	X3D::X3DPtr <X3D::FontStyle>        fontStyle;
	X3D::X3DPtr <X3D::ScreenFontStyle>  screenFontStyle;
	X3D::UndoStepPtr                    undoStep;
	X3D::UndoStepPtr                    styleUndoStep;
	bool                                changing;

};

} // puck
} // titania

#endif
