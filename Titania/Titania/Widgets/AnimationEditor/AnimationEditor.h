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

#ifndef __TITANIA_WIDGETS_ANIMATION_EDITOR_ANIMATION_EDITOR_H__
#define __TITANIA_WIDGETS_ANIMATION_EDITOR_ANIMATION_EDITOR_H__

#include "../../ComposedWidgets.h"
#include "../../Base/X3DEditorObject.h"
#include "../../UserInterfaces/X3DAnimationEditorInterface.h"

namespace titania {
namespace puck {

class BrowserWindow;
class NodeIndex;

class AnimationEditor :
	public X3DAnimationEditorInterface,
	public X3DEditorObject
{
public:

	///  @name Construction

	AnimationEditor (X3DBrowserWindow* const);

	///  @name Destruction

	virtual
	~AnimationEditor ();


private:

	///  @name Construction

	virtual
	void
	initialize () final override;

	int32_t
	getDuration () const;

	int32_t
	getFramesPerSecond () const;

	virtual
	void
	on_map () final override;

	virtual
	void
	on_unmap () final override;

	void
	set_selection ();

	void
	set_browser (const X3D::BrowserPtr &);

	///  @name Event handlers

	virtual
	void
	on_new () final override;

	virtual
	void
	on_new_name_insert_text (const Glib::ustring &, int*) final override;

	virtual
	void
	on_new_name_delete_text (int, int) final override;

	virtual
	void
	on_new_name_changed () final override;

	virtual
	void
	on_open () final override;

	void
	set_animation (const X3D::SFNode &);

	void
	set_remove_animation (const bool);

	virtual
	void
	on_add_object () final override;

	void
	addNode (const X3D::SFNode &);

	void
	addFields (const X3D::SFNode &, Gtk::TreeIter &);

	std::string
	getNodeName (const X3D::SFNode &) const;

	void
	set_name (const size_t id, const Gtk::TreePath &);

	void
	set_live (const size_t id, const Gtk::TreePath &);

	void
	set_field (X3D::X3DFieldDefinition* const, const Gtk::TreePath &);

	void
	set_fields (const size_t id, const Gtk::TreePath &);
	
	virtual
	void
	on_enabled_toggled (const Glib::ustring &) final override;

	virtual
	bool
	on_tree_view_draw (const Cairo::RefPtr <Cairo::Context> &) final override;

	virtual
	bool
	on_draw (const Cairo::RefPtr <Cairo::Context> &) final override;

	///  @name Members

	std::unique_ptr <NodeIndex>                 nodeIndex;
	NameEntry                                   nodeName;
	X3D::X3DPtr <X3D::Group>                    animation;
	X3D::X3DPtr <X3D::TimeSensor>               timeSensor;
	X3D::X3DPtrArray <X3D::X3DInterpolatorNode> interpolators;
	std::map <uint64_t, X3D::SFNode>            nodes;
	X3D::Vector2d                               translation;
	double                                      scale;

};

} // puck
} // titania

#endif
