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

#ifndef __TITANIA_WIDGETS_ANIMATION_EDITOR_ANIMATION_EDITOR_H__
#define __TITANIA_WIDGETS_ANIMATION_EDITOR_ANIMATION_EDITOR_H__

#include "../../Base/X3DEditorObject.h"
#include "../../ComposedWidgets.h"
#include "../../UserInterfaces/X3DAnimationEditorInterface.h"

#include <Titania/X3D/Components/Interpolation/ColorInterpolator.h>
#include <Titania/X3D/Components/Interpolation/OrientationInterpolator.h>
#include <Titania/X3D/Components/Time/TimeSensor.h>

#include <Titania/Math/Algorithms/CatmullRomSplineInterpolator.h>

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

	AnimationEditor (X3DBrowserWindow* const browserWindow);

	virtual
	void
	setup () final override;

	/***
	 *  @name Destruction
	 **/

	virtual
	void
	dispose () final override;

	virtual
	~AnimationEditor () final override;


protected:

	/***
	 *  @name Construction
	 **/

	virtual
	void
	initialize () final override;

	virtual
	void
	configure () final override;

	virtual
	void
	set_selection (const X3D::MFNode & selection) final override;

	/***
	 *  @name Destruction
	 **/

	virtual
	void
	store () final override;


private:

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
	using Interpolators     = std::set <X3D::X3DPtr <X3D::X3DNode>>;
	using FrameKey          = std::tuple <int32_t, const X3D::X3DFieldDefinition*, Gtk::TreePath>;
	using CopiedFrame       = std::tuple <int32_t, const X3D::X3DFieldDefinition*, Gtk::TreePath, std::vector <double>, std::string>;
	using FrameArray        = std::vector <std::pair <FrameKey, X3D::Box2d>>;

	/***
	 *  @name Member access
	 **/

	void
	addUndoStep (const X3D::UndoStepPtr & undoStep);

	const Glib::RefPtr <Gtk::TreeStore> &
	getTreeStore () const
	{ return treeStore; }

	const Glib::RefPtr <Gtk::TreeModelFilter>
	getTreeModelFilter () const
	{ return treeModelFilter; }

	void
	setDuration (const int32_t value);

	int32_t
	getDuration () const;

	void
	setFrameRate (const int32_t value);

	int32_t
	getFrameRate () const;

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
	addKeyframe (const X3D::X3DPtr <X3D::X3DNode> &, const int32_t, const std::vector <double> &, const std::string &, const X3D::UndoStepPtr &);

	void
	moveKeyframes ();

	void
	moveKeyframe (const X3D::X3DPtr <X3D::X3DNode> &, const int32_t, const int32_t, const X3D::UndoStepPtr &);

	void
	moveKeyframe (const X3D::X3DPtr <X3D::X3DNode> &, const size_t, const int32_t, const int32_t, const X3D::UndoStepPtr &);

	void
	removeKeyframes ();

	void
	removeKeyframe (const X3D::X3DPtr <X3D::X3DNode> &, const int32_t, const X3D::UndoStepPtr &);

	void
	removeKeyframe (const X3D::X3DPtr <X3D::X3DNode> &, const size_t, const int32_t, const X3D::UndoStepPtr &);

	void
	scaleKeyframes (const int32_t, const int32_t, const X3D::UndoStepPtr &);

	void
	scaleKeyframes (const X3D::X3DPtr <X3D::X3DNode> & interpolator, const int32_t, const int32_t, const X3D::UndoStepPtr &);

	void
	setInterpolators (const X3D::UndoStepPtr &);

	void
	setInterpolator (const X3D::X3DPtr <X3D::X3DNode> &, const X3D::UndoStepPtr &);

	template <class Interpolator, class Field, class Type>
	void
	setSequencer (const X3D::X3DPtr <Interpolator> &, const X3D::UndoStepPtr &);

	template <class Interpolator, class Field, class Type>
	void
	setInterpolator (const X3D::X3DPtr <Interpolator> &, const X3D::UndoStepPtr &);

	void
	setInterpolator (const X3D::X3DPtr <X3D::ColorInterpolator> &, const X3D::UndoStepPtr &);

	void
	setInterpolator (const X3D::X3DPtr <X3D::OrientationInterpolator> &, const X3D::UndoStepPtr &);

	template <class Interpolator, class Field, class Type>
	void
	setArrayInterpolator (const X3D::X3DPtr <Interpolator> &, const X3D::UndoStepPtr &);

	template <class Type>
	Type
	getValue (const X3D::MFDouble &, const size_t) const;

	void
	resizeInterpolator (const X3D::X3DPtr <X3D::X3DNode> &, const size_t, const X3D::UndoStepPtr &);

	X3D::X3DPtr <X3D::X3DNode>
	getInterpolator (const std::string &, const X3D::SFNode &, const X3D::X3DFieldDefinition* const, const X3D::UndoStepPtr &);

	std::string
	getInterpolatorName (const X3D::SFNode &, const X3D::X3DFieldDefinition* const);

	void
	showArraySizeErrorDialog () const;

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

	std::pair <int32_t, int32_t>
	getSelectedRange () const;

	void
	setSelectedFrames (const std::set <FrameKey> & value, const X3D::X3DPtr <X3D::Group> & affectedAnimation);

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
	 *  @name Static members
	 **/

	// Maximum duration is 1,000,000 frames, thus epsilon is 0.000001 as we can only save 6 digits.
	static constexpr double epsilon = 0.000001;

	static const std::set <X3D::X3DConstants::NodeType>         arrayInterpolators;
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
	Interpolators                       interpolators;
	std::map <size_t, X3D::SFNode>      nodes;
	X3D::Vector2d                       fromPoint;
	double                              fromTranslation;
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
AnimationEditor::setSequencer (const X3D::X3DPtr <Interpolator> & interpolator, const X3D::UndoStepPtr & undoStep)
{
	const auto components = interpolatorComponents .at (interpolator -> getType () .back ());
	const auto key        = interpolator -> template getMetaData <X3D::MFInt32>  ("/Interpolator/key");
	auto       keyValue   = interpolator -> template getMetaData <X3D::MFDouble> ("/Interpolator/keyValue");
	auto       keyType    = interpolator -> template getMetaData <X3D::MFString> ("/Interpolator/keyType");

	keyValue .resize (key .size () * components);
	keyType  .resize (key .size ());

	undoStep -> addObjects (interpolator);
	undoStep -> addUndoFunction (&X3D::MFFloat::setValue, std::ref (interpolator -> key ()),      interpolator -> key ());
	undoStep -> addUndoFunction (&Field::setValue,        std::ref (interpolator -> keyValue ()), interpolator -> keyValue ());

	size_t       i        = 0; // index in key
	size_t       iN       = 0; // index in meta data keyValue
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
AnimationEditor::setInterpolator (const X3D::X3DPtr <Interpolator> & interpolator, const X3D::UndoStepPtr & undoStep)
{
	const auto components = interpolatorComponents .at (interpolator -> getType () .back ());
	const auto key        = interpolator -> template getMetaData <X3D::MFInt32>  ("/Interpolator/key");
	auto       keyValue   = interpolator -> template getMetaData <X3D::MFDouble> ("/Interpolator/keyValue");
	auto       keyType    = interpolator -> template getMetaData <X3D::MFString> ("/Interpolator/keyType");

	keyValue .resize (key .size () * components);
	keyType  .resize (key .size ());

	undoStep -> addObjects (interpolator);
	undoStep -> addUndoFunction (&X3D::MFFloat::setValue, std::ref (interpolator -> key ()),      interpolator -> key ());
	undoStep -> addUndoFunction (&Field::setValue,        std::ref (interpolator -> keyValue ()), interpolator -> keyValue ());

	size_t       i        = 0; // index in key
	size_t       iN       = 0; // index in meta data keyValue
	const size_t size     = key .size ();
	const auto   duration = getDuration ();

	interpolator -> key ()      .clear ();
	interpolator -> keyValue () .clear ();

	while (i < size)
	{
		if (key [i] < 0 or key [i] > duration)
		{
			++ i;
			continue;
		}

		const auto value    = getValue <Type> (keyValue, iN);
		const auto fraction = key [i] / (double) duration;
		auto       iT       = i;

		if (keyType [iT] == "SPLIT" and iT + 1 < size)
			++ iT;

		if (keyType [iT] == "CONSTANT")
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
		else if (keyType [iT] == "LINEAR" or keyType [iT] == "SPLIT")
		{
			interpolator -> key ()      .emplace_back (fraction);
			interpolator -> keyValue () .emplace_back (value);
		}
		else if (keyType [iT] == "SPLINE")
		{
			std::vector <int32_t> keys;
			std::vector <Type>    keyValues;
			std::vector <Type>    keyVelocitys;

			for (; i < size; ++ i, iN += components)
			{
				const auto value = getValue <Type> (keyValue, iN);

				keys      .emplace_back (key [i]);
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

			keyVelocitys .resize (keys .size (), Type ());

			const bool normalizeVelocity = false;
			const bool closed = keys .front () == 0 and keys .back () == duration and keyValues .front () == keyValues .back ();

			const math::catmull_rom_spline_interpolator <Type, double> spline (closed, keys, keyValues, keyVelocitys, normalizeVelocity);

			for (size_t k = 0, size = keys .size () - 1; k < size; ++ k)
			{
				const int32_t frames   = keys [k + 1] - keys [k];
				const double  fraction = keys [k] / (double) duration;
				const double  distance = frames / (double) duration;
				const auto    framesN  = k + 1 == size and i == key .size () ? frames + 1 : frames;

				for (int32_t f = 0; f < framesN; ++ f)
				{
					const auto weight = f / (double) frames;
					const auto value  = spline .interpolate (k, k + 1, weight, keyValues);

					interpolator -> key ()      .emplace_back (fraction + weight * distance);
					interpolator -> keyValue () .emplace_back (value);
				}
			}

			if (i + 1 not_eq size)
			{
				i  -= 1;
				iN -= components;
			}
		}

		i  += 1;
		iN += components;
	}

	undoStep -> addRedoFunction (&X3D::MFFloat::setValue, std::ref (interpolator -> key ()),      interpolator -> key ());
	undoStep -> addRedoFunction (&Field::setValue,        std::ref (interpolator -> keyValue ()), interpolator -> keyValue ());
}

template <class Interpolator, class Field, class Type>
void
AnimationEditor::setArrayInterpolator (const X3D::X3DPtr <Interpolator> & interpolator, const X3D::UndoStepPtr & undoStep)
{
	const auto components = interpolatorComponents .at (interpolator -> getType () .back ());
	const auto key        = interpolator -> template getMetaData <X3D::MFInt32>  ("/Interpolator/key");
	auto       keyValue   = interpolator -> template getMetaData <X3D::MFDouble> ("/Interpolator/keyValue");
	auto       keyType    = interpolator -> template getMetaData <X3D::MFString> ("/Interpolator/keyType");
	const auto keySize    = interpolator -> template getMetaData <X3D::SFInt32> ("/Interpolator/keySize");

	keyValue .resize (key .size () * components * keySize);
	keyType  .resize (key .size ());

	undoStep -> addObjects (interpolator);
	undoStep -> addUndoFunction (&X3D::MFFloat::setValue, std::ref (interpolator -> key ()),      interpolator -> key ());
	undoStep -> addUndoFunction (&Field::setValue,        std::ref (interpolator -> keyValue ()), interpolator -> keyValue ());

	size_t       i        = 0; // index in key
	size_t       iN       = 0; // index in meta data keyValue
	const size_t size     = key .size ();
	const auto   duration = getDuration ();

	interpolator -> key ()      .clear ();
	interpolator -> keyValue () .clear ();

	while (i < size)
	{
		if (key [i] < 0 or key [i] > duration)
		{
			++ i;
			continue;
		}

		const auto fraction = key [i] / (double) duration;
		auto       iT       = i;

		if (keyType [iT] == "SPLIT" and iT + 1 < size)
			++ iT;

		if (keyType [iT] == "CONSTANT")
		{
			interpolator -> key () .emplace_back (fraction);

			for (size_t a = 0, size = components * keySize; a < size; a += components)
				interpolator -> keyValue () .emplace_back (getValue <Type> (keyValue, iN + a));

			if (key [i] < duration)
			{
				const auto nextFraction = (i == size - 1 ? 1 : key [i + 1] / (double) duration - epsilon);

				interpolator -> key () .emplace_back (nextFraction);

				for (size_t a = 0, size = components * keySize; a < size; a += components)
					interpolator -> keyValue () .emplace_back (getValue <Type> (keyValue, iN + a));
			}
		}
		else if (keyType [iT] == "LINEAR" or keyType [iT] == "SPLIT")
		{
			interpolator -> key () .emplace_back (fraction);

			for (size_t a = 0, size = components * keySize; a < size; a += components)
				interpolator -> keyValue () .emplace_back (getValue <Type> (keyValue, iN + a));
		}
		else if (keyType [iT] == "SPLINE")
		{
			const auto first = interpolator -> keyValue () .size ();

			// Generate key.

			std::vector <int32_t> keys;

			for (; i < size; ++ i)
			{
				keys .emplace_back (key [i]);

				if (keys .size () == 1)
					continue;

				if (keyType [i] not_eq "SPLINE")
					break;
			}

			if (keys .size () < 2)
			{
				// This can happen if only the last frame is of type SPLINE.
				interpolator -> key () .emplace_back (fraction);

				for (size_t a = 0, size = components * keySize; a < size; a += components)
					interpolator -> keyValue () .emplace_back (getValue <Type> (keyValue, iN + a));

				break;
			}

			for (size_t k = 0, size = keys .size () - 1; k < size; ++ k)
			{
				const int32_t frames   = keys [k + 1] - keys [k];
				const double  fraction = keys [k] / (double) duration;
				const double  distance = frames / (double) duration;
				const auto    framesN  = k + 1 == size and i == key .size () ? frames + 1 : frames;

				for (int32_t f = 0; f < framesN; ++ f)
				{
					const auto weight = f / (double) frames;

					interpolator -> key () .emplace_back (fraction + weight * distance);
				}
			}

			// Generate keyValue.
			for (int32_t a = 0; a < keySize; ++ a)
			{
				std::vector <Type> keyValues;
				std::vector <Type> keyVelocitys;
	
				for (size_t i = 0, aiN = iN + a * components; i < keys .size (); ++ i, aiN += components * keySize)
					keyValues .emplace_back (getValue <Type> (keyValue, aiN));

				keyVelocitys .resize (keys .size (), Type ());

				const bool normalizeVelocity = false;
				const bool closed = keys .front () == 0 and keys .back () == duration and keyValues .front () == keyValues .back ();
	
				const math::catmull_rom_spline_interpolator <Type, double> spline (closed, keys, keyValues, keyVelocitys, normalizeVelocity);
	
				size_t totalFrames = 0;

				for (size_t k = 0, size = keys .size () - 1; k < size; ++ k)
				{
					const int32_t frames  = keys [k + 1] - keys [k];
					const auto    framesN = k + 1 == size and i == key .size () ? frames + 1 : frames;
	
					for (int32_t f = 0; f < framesN; ++ f)
					{
						const auto weight = f / (double) frames;
						const auto value  = spline .interpolate (k, k + 1, weight, keyValues);
						const auto index  = first + a + (totalFrames + f) * keySize;

						interpolator -> keyValue () .set1Value (index, value);
					}

					totalFrames += frames;
				}
			}

			if (i + 1 not_eq size)
				i -= 1;

			iN += components * keySize * (keys .size () - 2);
		}

		i  += 1;
		iN += components * keySize;
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
