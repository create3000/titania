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

#ifndef __TITANIA_BROWSER_NOTEBOOK_RENDER_PANEL_RENDER_PANEL_H__
#define __TITANIA_BROWSER_NOTEBOOK_RENDER_PANEL_RENDER_PANEL_H__

#include "../../UserInterfaces/X3DRenderPanelInterface.h"

#include "X3DRenderPanel.h"

#include "../../Editors/BindableNodeList/ViewpointList.h"

namespace titania {
namespace puck {

class NotebookPage;
class RenderThread;
class TexturePreview;

class RenderPanel :
	virtual public X3DRenderPanelInterface,
	public X3DRenderPanel
{
public:

	///  @name Construction

	RenderPanel (X3DBrowserWindow* const browserWindow, NotebookPage* const page, const size_t id);

	///  @name Operations

	virtual
	void
	lookAtSelection () final override;

	virtual
	void
	lookAtAll () final override;

	///  @name Destruction

	virtual
	void
	dispose () final override;

	virtual
	~RenderPanel () final override;


protected:

	///  @name Construction

	virtual
	void
	initialize () final override;

	virtual
	void
	configure () final override;

	virtual
	void
	store () final override;


private:

	///  @name Member access

	std::shared_ptr <ViewpointList>
	getViewpointList () const;

	bool
	getPropertiesDialogResponse ();

	void
	setRendering (const bool value);

	bool
	getRendering () const;

	///  @name Event handlers

	virtual
	void
	on_properties_file_chooser_button_clicked () final override;

	virtual
	void
	on_properties_time_changed () final override;

	virtual
	void
	on_properties_viewpoint_chooser_button_clicked () final override;

	void
	set_viewpoint ();

	virtual
	void
	on_record_clicked () final override;

	void
	on_load_count_changed (const size_t loadCount);

	void
	set_movie_active ();

	void
	set_movie_elapsedTime ();

	void
	set_movie_duration (const X3D::time_type value);

	virtual
	void
	on_loop_toggled () final override;

	virtual
	void
	on_stop_clicked () final override;

	virtual
	void
	on_play_pause_clicked () final override;

	void
	on_frame_changed ();

	void
	set_frame (const size_t value);

	void
	set_duration (const size_t value);

	void
	on_stdout ();
	
	void
	on_stderr ();

	///  @name Members

	std::unique_ptr <TexturePreview> preview;
	X3D::X3DPtr <X3D::ImageTexture>  imageTexture;
	X3D::X3DPtr <X3D::MovieTexture>  movieTexture;
	std::unique_ptr <RenderThread>   renderThread;
	basic::uri                       filename;
	std::string                      viewpoint;
	std::shared_ptr <ViewpointList>  viewpointList;

};

} // puck
} // titania

#endif
