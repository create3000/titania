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

	/***
	 *  @name Construction
	 */

	AnimationEditor (X3DBrowserWindow* const);

	/***
	 *  @name Destruction
	 **/

	virtual
	~AnimationEditor ();


private:

	/***
	 *  @name Construction
	 **/

	virtual
	void
	initialize () final override;

	virtual
	void
	on_map () final override;

	virtual
	void
	on_unmap () final override;

	void
	set_selection ();

	/***
	 *  @name Member access
	 **/

	const Glib::RefPtr <Gtk::TreeStore> &
	getTreeStore () const
	{ return treeStore; }

	const Glib::RefPtr <Gtk::TreeModelFilter>
	getTreeModelFilter () const
	{ return treeModelFilter; }

	int32_t
	getDuration () const;

	int32_t
	getFramesPerSecond () const;

	bool
	isActive () const;

	virtual
	void
	on_translation_changed () final override;

	void
	setTranslation (const double);

	void
	setLowerTranslation ();

	double
	getTranslation () const
	{ return translation; }

	void
	setScale (const double);

	double
	getScale () const
	{ return scale; }

	/***
	 *  New and Open Animation
	 **/

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
	on_new_cycle_interval_changed () final override;

	virtual
	void
	on_open () final override;

	void
	set_animation (const X3D::SFNode &);

	void
	set_animation_live (const bool);
	
	void
	set_interpolators ();

	void
	removeInterpolators ();

	virtual
	void
	on_add_member () final override;

	virtual
	void
	on_remove_member () final override;

	virtual
	void
	on_tree_view_selection_changed () final override;

	void
	addNode (const X3D::SFNode &);

	void
	removeNode (const X3D::SFNode &);

	void
	addFields (const X3D::SFNode &, Gtk::TreeIter &);

	std::string
	getNodeName (const X3D::SFNode &) const;

	void
	set_name (const size_t id, const Gtk::TreePath &);

	void
	set_live (const size_t id, const Gtk::TreePath &);

	void
	set_tainted (const Gtk::TreePath &);
	
	void
	setTainted (const Gtk::TreePath &, const bool);

	bool
	hasTaintedChildren (const Gtk::TreeIter &) const;

	void
	set_user_defined_fields (const size_t id, const Gtk::TreePath &);

	virtual
	void
	on_close () final override;

	/***
	 *  Cut, Copy and Paste
	 **/

	virtual
	void
	on_cut () final override;

	virtual
	void
	on_copy () final override;

	virtual
	void
	on_paste () final override;

	void
	on_clear_clipboard ();

	/***
	 *  Frame and Time
	 **/

	virtual
	void
	on_first_frame () final override;

	virtual
	void
	on_play_pause () final override;

	virtual
	void
	on_last_frame () final override;

	virtual
	void
	on_current_frame_changed () final override;

	void
	on_update_fraction ();

	virtual
	void
	on_time () final override;

	void
	set_duration ();

	void
	set_frames_per_second ();
	
	void
	set_active ();
	
	void
	set_fraction (const float);

	void
	set_value ();

	/***
	 *  Keyframes
	 **/

	virtual
	void
	on_key_type_changed () final override;
	
	void
	set_key_type ();

	/***
	 *  Zoom
	 **/

	virtual
	void
	on_zoom_out () final override;

	virtual
	void
	on_zoom_in () final override;

	void
	on_zoom (const double, const GdkScrollDirection);

	virtual
	void
	on_zoom_fit () final override;

	virtual
	void
	on_zoom_100 () final override;

	/***
	 *  Tree View
	 **/

	virtual
	void
	on_row_activated (const Gtk::TreePath &, Gtk::TreeViewColumn*) final override;

	virtual
	void
	on_tainted_toggled (const Glib::ustring &) final override;

	void
	addKeyframe (const Gtk::TreePath &);
	
	void
	addKeyframe (const Gtk::TreePath &, const Gtk::TreePath &);

	void
	addKeyframe (const X3D::SFNode &, const X3D::X3DFieldDefinition* const);

	void
	addKeyframe (const X3D::X3DPtr <X3D::X3DNode> &, const int32_t, const std::vector <double> &, const std::string &, const UndoStepPtr &);

	void
	moveKeyframes ();

	void
	moveKeyframe (const X3D::X3DPtr <X3D::X3DNode> &, const int32_t, const int32_t, const UndoStepPtr &);

	void
	moveKeyframe (const X3D::X3DPtr <X3D::X3DNode> &, const size_t, const int32_t, const int32_t, const UndoStepPtr &);

	void
	removeKeyframes ();

	void
	removeKeyframe (const X3D::X3DPtr <X3D::X3DNode> &, const int32_t, const UndoStepPtr &);

	void
	removeKeyframe (const X3D::X3DPtr <X3D::X3DNode> &, const size_t, const int32_t, const UndoStepPtr &);

	void
	scaleKeyframes (const int32_t, const int32_t, const UndoStepPtr &);

	void
	scaleKeyframes (const X3D::X3DPtr <X3D::X3DNode> & interpolator, const int32_t, const int32_t, const UndoStepPtr &);

	void
	setInterpolators (const UndoStepPtr &);

	void
	setInterpolator (const X3D::X3DPtr <X3D::X3DNode> &, const UndoStepPtr &);

	template <class Interpolator, class Field, class Type>
	void
	setSequencer (const X3D::X3DPtr <Interpolator> &, const UndoStepPtr &);

	template <class Interpolator, class Field, class Type>
	void
	setInterpolator (const X3D::X3DPtr <Interpolator> &, const UndoStepPtr &);

	void
	setInterpolator (const X3D::X3DPtr <X3D::ColorInterpolator> &, const UndoStepPtr &);

	void
	setInterpolator (const X3D::X3DPtr <X3D::OrientationInterpolator> &, const UndoStepPtr &);

	template <class Type>
	Type
	getValue (const X3D::MFDouble &, const size_t) const;

	void
	resizeInterpolator (const X3D::X3DPtr <X3D::X3DNode> &, const size_t, const UndoStepPtr &);

	X3D::X3DPtr <X3D::X3DNode>
	getInterpolator (const std::string &, const X3D::SFNode &, const X3D::X3DFieldDefinition* const, const UndoStepPtr &);

	std::string
	getInterpolatorName (const X3D::SFNode &, const X3D::X3DFieldDefinition* const);

	/***
	 *  Drawing Area
	 **/

	virtual
	bool
	on_configure_event (GdkEventConfigure*) final override;
	
	virtual
	bool
	on_focus_in_event (GdkEventFocus*) final override;

	virtual
	bool
	on_focus_out_event (GdkEventFocus*) final override;

	virtual
	bool
	on_key_press_event (GdkEventKey*) final override;

	virtual
	bool
	on_key_release_event (GdkEventKey*) final override;

	virtual
	bool
	on_button_press_event (GdkEventButton*) final override;

	virtual
	bool
	on_button_release_event (GdkEventButton*) final override;

	virtual
	bool
	on_motion_notify_event (GdkEventMotion*) final override;

	virtual
	bool
	on_scroll_event (GdkEventScroll*) final override;

	void
	on_selection_changed ();

	void
	on_clear_selection ();

	void
	on_expand_selected_range (const int32_t);

	void
	on_select_range ();

	bool
	isSelected () const;

	std::pair <int32_t, int32_t>
	getSelectedBounds () const;

	bool
	pick (const X3D::Vector2d &);

	bool
	buildFrames (const Gtk::TreePath &, const Gtk::TreeIter &);

	void
	addKeyframes (const Gtk::TreePath &, const Gtk::TreeIter &, const double y);

	void
	queue_draw ();

	virtual
	bool
	on_tree_view_draw (const Cairo::RefPtr <Cairo::Context> &) final override;

	virtual
	bool
	on_draw (const Cairo::RefPtr <Cairo::Context> &) final override;

	void
	on_draw_keyframes (const Cairo::RefPtr <Cairo::Context> &,
	                   const Gtk::TreePath &,
	                   const Gtk::TreeIter &,
	                   const int32_t,
	                   const int32_t,
	                   const double,
	                   const Gdk::RGBA &,
	                   const Gdk::RGBA &);

	void
	on_draw_moved_keyframes (const Cairo::RefPtr <Cairo::Context> &, const Gdk::RGBA &);

	std::pair <int32_t, int32_t>
	getFrameParams () const;

	/***
	 *  @name Member types
	 **/

	class Columns :
		public Gtk::TreeModel::ColumnRecord
	{
	public:

		Columns ()
		{
			add (id);
			add (type);
			add (name);
			add (accessType);
			add (tainted);
			add (visible);
		}

		Gtk::TreeModelColumn <size_t>                      id;
		Gtk::TreeModelColumn <Glib::RefPtr <Gdk::Pixbuf>>  type;
		Gtk::TreeModelColumn <std::string>                 name;
		Gtk::TreeModelColumn <Glib::RefPtr <Gdk::Pixbuf>>  accessType;
		Gtk::TreeModelColumn <bool>                        tainted;
		Gtk::TreeModelColumn <bool>                        visible;

	};

	using InterpolatorIndex = std::map <const X3D::X3DFieldDefinition*, X3D::X3DPtr <X3D::X3DNode>>;
	using FrameKey          = std::tuple <int32_t, const X3D::X3DFieldDefinition*, Gtk::TreePath>;
	using CopiedFrame       = std::tuple <int32_t, const X3D::X3DFieldDefinition*, Gtk::TreePath, std::vector <double>, std::string>;
	using FrameArray        = std::vector <std::pair <FrameKey, X3D::Box2d>>;

	using setMetaDataInteger = void (X3D::X3DNode::*) (const std::string &, const X3D::MFInt32 &);
	using setMetaDataDouble  = void (X3D::X3DNode::*) (const std::string &, const X3D::MFDouble &);
	using setMetaDataString  = void (X3D::X3DNode::*) (const std::string &, const X3D::MFString &);

	/***
	 *  @name Static members
	 **/

	// Maximum duration is 1,000,000 frames, thus epsilon is 0.000001 as we can only save 6 digits.
	static constexpr double epsilon = 0.000001;

	static const std::map <X3D::X3DConstants::NodeType, size_t> interpolatorComponents;

	/***
	 *  @name Members
	 **/

	Columns                             columns;
	Glib::RefPtr <Gtk::TreeStore>       treeStore;
	Glib::RefPtr <Gtk::TreeModelFilter> treeModelFilter;
	std::unique_ptr <NodeIndex>         nodeIndex;
	NameEntry                           nodeName;
	X3D::X3DPtr <X3D::Group>            animation;
	X3D::X3DPtr <X3D::TimeSensor>       timeSensor;
	InterpolatorIndex                   interpolatorIndex;
	std::map <size_t, X3D::SFNode>      nodes;
	X3D::Vector2d                       fromPoint;
	double                              translation;
	double                              scale;
	guint                               button;
	size_t                              frameChange;
	FrameArray                          frames;
	std::set <FrameKey>                 activeFrames;
	std::set <FrameKey>                 selectedFrames;
	std::pair <int32_t, int32_t>        selectedBounds;
	std::pair <int32_t, int32_t>        selectedRange;
	bool                                activeSelection;
	std::vector <FrameKey>              movedFrames;
	std::vector <CopiedFrame>           copiedFrames;
	std::string                         activeType;
	bool                                changing;
	X3D::Keys                           keys;

};

template <class Interpolator, class Field, class Type>
void
AnimationEditor::setSequencer (const X3D::X3DPtr <Interpolator> & interpolator, const UndoStepPtr & undoStep)
{
	const auto   components = interpolatorComponents .at (interpolator -> getType () .back ());
	const auto & key        = interpolator -> template getMetaData <X3D::MFInt32> ("/Interpolator/key",       true);
	auto &       keyValue   = interpolator -> template getMetaData <X3D::MFDouble> ("/Interpolator/keyValue", true);
	auto &       keyType    = interpolator -> template getMetaData <X3D::MFString> ("/Interpolator/keyType",  true);

	keyValue .resize (key .size () * components);
	keyType  .resize (key .size ());

	undoStep -> addObjects (interpolator);
	undoStep -> addUndoFunction (&X3D::MFFloat::setValue, std::ref (interpolator -> key ()),      interpolator -> key ());
	undoStep -> addUndoFunction (&Field::setValue,        std::ref (interpolator -> keyValue ()), interpolator -> keyValue ());

	size_t       i        = 0;
	size_t       iN       = 0;
	const size_t size     = key .size ();
	const auto   duration = getDuration ();

	interpolator -> key ()      .clear ();
	interpolator -> keyValue () .clear ();

	while (i < size)
	{
		if (key [i] < 0 or key [i] > duration)
			continue;

		const auto fraction = key [i] / (double) duration;
		const auto value    = Type (keyValue [iN]);

		interpolator -> key ()      .emplace_back (fraction);
		interpolator -> keyValue () .emplace_back (value);

		++ i;
		iN += components;
	}

	undoStep -> addRedoFunction (&X3D::MFFloat::setValue, std::ref (interpolator -> key ()),      interpolator -> key ());
	undoStep -> addRedoFunction (&Field::setValue,        std::ref (interpolator -> keyValue ()), interpolator -> keyValue ());
}

template <class Interpolator, class Field, class Type>
void
AnimationEditor::setInterpolator (const X3D::X3DPtr <Interpolator> & interpolator, const UndoStepPtr & undoStep)
{
	const auto   components = interpolatorComponents .at (interpolator -> getType () .back ());
	const auto & key        = interpolator -> template getMetaData <X3D::MFInt32> ("/Interpolator/key",      true);
	auto &       keyValue   = interpolator -> template getMetaData <X3D::MFDouble> ("/Interpolator/keyValue", true);
	auto &       keyType    = interpolator -> template getMetaData <X3D::MFString> ("/Interpolator/keyType", true);

	keyValue .resize (key .size () * components);
	keyType  .resize (key .size ());

	undoStep -> addObjects (interpolator);
	undoStep -> addUndoFunction (&X3D::MFFloat::setValue, std::ref (interpolator -> key ()),      interpolator -> key ());
	undoStep -> addUndoFunction (&Field::setValue,        std::ref (interpolator -> keyValue ()), interpolator -> keyValue ());

	size_t       i        = 0;
	size_t       iN       = 0;
	const size_t size     = key .size ();
	const auto   duration = getDuration ();

	interpolator -> key ()      .clear ();
	interpolator -> keyValue () .clear ();

	while (i < size)
	{
		if (key [i] < 0 or key [i] > duration)
			continue;

		const auto fraction = key [i] / (double) duration;
		const auto value    = getValue <Type> (keyValue, iN);

		if (keyType [i] == "CONSTANT")
		{
			interpolator -> key ()      .emplace_back (fraction);
			interpolator -> keyValue () .emplace_back (value);

			if (key [i] < duration)
			{
				const auto nextFraction = (i == size - 1 ? 1 : key [i + 1] / (double) duration - epsilon);

				interpolator -> key ()      .emplace_back (nextFraction);
				interpolator -> keyValue () .emplace_back (value);
			}
		}
		else if (keyType [i] == "LINEAR")
		{
			interpolator -> key ()      .emplace_back (fraction);
			interpolator -> keyValue () .emplace_back (value);
		}
		else if (keyType [i] == "SPLINE" or keyType [i] == "SPLIT")
		{
			std::vector <int32_t> keys;
			std::vector <Type> keyValues;
			std::vector <Type> keyVelocitys;

			for (; i < size; ++ i, iN += components)
			{
				const auto value = getValue <Type> (keyValue, iN);

				keys .emplace_back (key [i]);
				keyValues .emplace_back (value);
				
				if (keys .size () == 1)
					continue;

				if (keyType [i] not_eq "SPLINE")
					break;
			}
	
			if (keys .size () < 2)
			{
				// This can happen if only the last frame is of type SPLINE.
				interpolator -> key ()      .emplace_back (fraction);
				interpolator -> keyValue () .emplace_back (value);
				break;
			}
	
			const bool normalizeVelocity = false;
			const bool closed            = keys .front () == 0 and keys .back () == duration and keyValues .front () == keyValues .back ();

			const math::catmull_rom_spline_interpolator <Type, double> spline (closed, keys, keyValues, keyVelocitys, normalizeVelocity);

			for (size_t k = 0, size = keys .size () - 1; k < size; ++ k)
			{
				const int32_t frames   = keys [k + 1] - keys [k];
				const double  fraction = keys [k] / (double) duration;
				const double  distance = frames / (double) duration;

				for (int32_t f = 0; f < frames; ++ f)
				{
					const auto weight = f / (double) frames;
					const auto value  = spline .interpolate (k, k + 1, weight, keyValues);

					interpolator -> key ()      .emplace_back (fraction + weight * distance);
					interpolator -> keyValue () .emplace_back (value);
				}
			}

			if (i == size)
			{
				// If this is the last part then we must insert the last keyframe.
				interpolator -> key ()      .emplace_back (keys .back () / (double) duration);
				interpolator -> keyValue () .emplace_back (keyValues .back ());
				break;
			}

			continue;
		}

		++ i;
		iN += components;
	}

	undoStep -> addRedoFunction (&X3D::MFFloat::setValue, std::ref (interpolator -> key ()),      interpolator -> key ());
	undoStep -> addRedoFunction (&Field::setValue,        std::ref (interpolator -> keyValue ()), interpolator -> keyValue ());
}

template <class Type>
inline
Type
AnimationEditor::getValue (const X3D::MFDouble & keyValue, const size_t index) const
{
	throw std::domain_error ("getValue is not defined");
}

template <>
inline
double
AnimationEditor::getValue <double> (const X3D::MFDouble & keyValue, const size_t index) const
{
	return keyValue [index];
}

template <>
inline
X3D::Vector2d
AnimationEditor::getValue <X3D::Vector2d> (const X3D::MFDouble & keyValue, const size_t index) const
{
	return X3D::Vector2d (keyValue [index], keyValue [index + 1]);
}

template <>
inline
X3D::Vector3d
AnimationEditor::getValue <X3D::Vector3d> (const X3D::MFDouble & keyValue, const size_t index) const
{
	return X3D::Vector3d (keyValue [index], keyValue [index + 1], keyValue [index + 2]);
}

} // puck
} // titania

#endif
