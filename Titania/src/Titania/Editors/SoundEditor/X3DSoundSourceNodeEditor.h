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

#ifndef __TITANIA_EDITORS_SOUND_EDITOR_X3DSOUND_SOURCE_NODE_EDITOR_H__
#define __TITANIA_EDITORS_SOUND_EDITOR_X3DSOUND_SOURCE_NODE_EDITOR_H__

#include "../../UserInterfaces/X3DSoundEditorInterface.h"

#include "../../ComposedWidgets.h"

#include <Titania/X3D/Components/Sound/Sound.h>
#include <Titania/X3D/Components/Sound/AudioClip.h>
#include <Titania/X3D/Components/Texturing/MovieTexture.h>

namespace titania {
namespace puck {

class MFStringURLWidget;

class X3DSoundSourceNodeEditor :
	virtual public X3DSoundEditorInterface
{
public:

	///  @name Destruction

	virtual
	~X3DSoundSourceNodeEditor () override;


protected:

	///  @name Construction

	X3DSoundSourceNodeEditor ();

	virtual
	void
	initialize () override;

	virtual
	void
	set_selection (const X3D::MFNode & selection) override;


private:

	///  @name sound source

	virtual
	void
	on_sound_source_unlink_clicked () final override;

	virtual
	void
	on_sound_source_changed () final override;

	void
	set_source ();

	void
	connectSource (const X3D::SFNode &);

	void
	set_node ();

	void
	set_widgets ();

	void
	on_sound_source_loop_toggled ();

	virtual
	void
	on_sound_source_stop_clicked () final override;

	virtual
	void
	on_sound_source_play_pause_clicked () final override;

	void
	set_active ();

	void
	set_elapsedTime ();

	void
	set_duration ();

	///  @name Members

	X3DFieldToggleButton <X3D::SFBool>  enabled;
	SFStringTextView                    description;
	std::unique_ptr <MFStringURLWidget> url;
	X3DFieldAdjustment <X3D::SFFloat>   speed;
	X3DFieldAdjustment <X3D::SFFloat>   pitch;
	X3DFieldToggleButton <X3D::SFBool>  loop;

	X3D::X3DPtrArray <X3D::Sound>         sounds;
	X3D::SFTime                           soundSourceNodeBuffer;
	X3D::X3DPtr <X3D::X3DSoundSourceNode> soundSourceNode;
	X3D::X3DPtr <X3D::AudioClip>          audioClip;
	X3D::X3DPtr <X3D::MovieTexture>       movieTexture;
	X3D::UndoStepPtr                      undoStep;
	bool                                  changing;

};

} // puck
} // titania

#endif
