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

#ifndef __TITANIA_EDITORS_GEOMETRY_PROPERTIES_EDITOR_GEOMETRY3D_X3DHEIGHT_MAP_EDITOR_H__
#define __TITANIA_EDITORS_GEOMETRY_PROPERTIES_EDITOR_GEOMETRY3D_X3DHEIGHT_MAP_EDITOR_H__

#include "../../../Bits/String.h"
#include "../../../UserInterfaces/X3DGeometryPropertiesEditorInterface.h"

#include <Titania/X3D/Components/Core/MetadataString.h>
#include <Titania/X3D/Fields/Hash.h>

namespace titania {
namespace puck {

template <class NodeType, class FieldType>
class X3DHeightMapEditor :
	virtual public X3DGeometryPropertiesEditorInterface
{
public:

	///  @name Destruction

	virtual
	~X3DHeightMapEditor () override;


protected:

	///  @name Construction

	X3DHeightMapEditor (const Glib::RefPtr <Gtk::Adjustment> & minHeightAdjustment,
	                    const Glib::RefPtr <Gtk::Adjustment> & maxHeightAdjustment,
	                    Gtk::FileChooserButton & fileChooser,
	                    Gtk::Button & reloadButton,
	                    Gtk::Button & removeButton,
	                    Gtk::Label & loadStateLabel);

	virtual
	void
	initialize () override;

	///  @name Member access

	void
	setNode (const X3D::X3DPtr <NodeType> & value);


private:

	///  @name Member access

	std::string
	getHeightHash () const;

	FieldType
	getHeight (const bool fromMetaData);

	///  @name Event handlers

	void
	set_scene ();

	void
	set_height_verified ();

	void
	set_height (const bool removeMetaData);

	void
	set_adjustments ();

	void
	on_height_map_min_max_height_changed (const int32_t input);

	void
	connectHeight (const FieldType & field);

	void
	set_heightMap ();

	void
	on_height_map_image_set ();

	void
	on_height_map_image_reload_clicked ();

	void
	on_height_map_image_remove_clicked ();

	void
	set_loadState ();

	///  @name Static embers

	static const std::string HEIGHT;
	static const std::string HEIGHT_HASH;
	static const std::string HEIGHT_MAP;

	///  @name Members

	Glib::RefPtr <Gtk::Adjustment> minHeightAdjustment;
	Glib::RefPtr <Gtk::Adjustment> maxHeightAdjustment;
	Gtk::FileChooserButton &       fileChooser;
	Gtk::Button &                  reloadButton;
	Gtk::Button &                  removeButton;
	Gtk::Label &                   loadStateLabel;

	X3D::X3DScenePtr       scene;
	X3D::X3DPtr <NodeType> node;

	float metaMinHeight;
	float metaMaxHeight;
	bool  changing;

	X3D::UndoStepPtr undoStep;
	int32_t          minMaxInput;

};

template <class NodeType, class FieldType>
const std::string X3DHeightMapEditor <NodeType, FieldType>::HEIGHT = "/ElevationGrid/height";

template <class NodeType, class FieldType>
const std::string X3DHeightMapEditor <NodeType, FieldType>::HEIGHT_HASH = "/ElevationGrid/heightHash";

template <class NodeType, class FieldType>
const std::string X3DHeightMapEditor <NodeType, FieldType>::HEIGHT_MAP = "/ElevationGrid/heightMap";

template <class NodeType, class FieldType>
X3DHeightMapEditor <NodeType, FieldType>::X3DHeightMapEditor (const Glib::RefPtr <Gtk::Adjustment> & minHeightAdjustment,
                                                              const Glib::RefPtr <Gtk::Adjustment> & maxHeightAdjustment,
                                                              Gtk::FileChooserButton & fileChooser,
                                                              Gtk::Button & reloadButton,
                                                              Gtk::Button & removeButton,
                                                              Gtk::Label & loadStateLabel) :
	X3DGeometryPropertiesEditorInterface (),
	                 minHeightAdjustment (minHeightAdjustment),
	                 maxHeightAdjustment (maxHeightAdjustment),
	                         fileChooser (fileChooser),
	                        reloadButton (reloadButton),
	                        removeButton (removeButton),
	                      loadStateLabel (loadStateLabel),
	                               scene (),
	                                node (),
	                       metaMinHeight (0),
	                       metaMaxHeight (0),
	                            changing (false),
	                            undoStep (),
	                         minMaxInput (-1)
{
	addChildObjects (scene, node);

	// Signals

	minHeightAdjustment -> signal_value_changed () .connect (sigc::bind (sigc::mem_fun (this, &X3DHeightMapEditor::on_height_map_min_max_height_changed), 0));
	maxHeightAdjustment -> signal_value_changed () .connect (sigc::bind (sigc::mem_fun (this, &X3DHeightMapEditor::on_height_map_min_max_height_changed), 1));

	fileChooser .signal_file_set () .connect (sigc::mem_fun (this, &X3DHeightMapEditor::on_height_map_image_set));

	reloadButton .signal_clicked () .connect (sigc::mem_fun (this, &X3DHeightMapEditor::on_height_map_image_reload_clicked));
	removeButton .signal_clicked () .connect (sigc::mem_fun (this, &X3DHeightMapEditor::on_height_map_image_remove_clicked));
}

template <class NodeType, class FieldType>
void
X3DHeightMapEditor <NodeType, FieldType>::initialize ()
{
	getCurrentScene () .addInterest (&X3DHeightMapEditor::set_scene, this);
}

template <class NodeType, class FieldType>
void
X3DHeightMapEditor <NodeType, FieldType>::setNode (const X3D::X3DPtr <NodeType> & value)
{
	if (value == node)
		return;

	if (node)
	{
		node -> height ()         .removeInterest (&X3DHeightMapEditor::set_height_verified, this);
		node -> checkLoadState () .removeInterest (&X3DHeightMapEditor::set_loadState,       this);
	}

	node = value;

	if (node)
	{
		node -> height ()         .addInterest (&X3DHeightMapEditor::set_height_verified, this);
		node -> checkLoadState () .addInterest (&X3DHeightMapEditor::set_loadState,       this);

		set_height_verified ();
		set_loadState ();
	}
}

template <class NodeType, class FieldType>
std::string
X3DHeightMapEditor <NodeType, FieldType>::getHeightHash () const
{
	return basic::to_string (std::hash <FieldType>{ } (node -> height ()), std::locale::classic ());
}

template <class NodeType, class FieldType>
FieldType
X3DHeightMapEditor <NodeType, FieldType>::getHeight (const bool save)
{
	const auto height = node -> template getMetaData <FieldType> (HEIGHT);

	if (save and height .empty ())
	{
		node -> template setMetaData <FieldType> (HEIGHT, node -> height ());

		return node -> height ();
	}

	if (height .empty ())
		return node -> height ();

	return height;
}

template <class NodeType, class FieldType>
void
X3DHeightMapEditor <NodeType, FieldType>::set_scene ()
{
	// Connect units.

	if (scene)
		scene -> units_changed () .removeInterest (&X3DHeightMapEditor::set_adjustments, this);

	scene = getCurrentScene ();

	scene -> units_changed () .addInterest (&X3DHeightMapEditor::set_adjustments, this);
}

template <class NodeType, class FieldType>
void
X3DHeightMapEditor <NodeType, FieldType>::set_height_verified ()
{
	const auto metaHash   = node -> template getMetaData <X3D::MFString> (HEIGHT_HASH);
	const auto heightHash = getHeightHash ();

	if (metaHash .empty ())
		set_height (true);
	else
		set_height (heightHash not_eq metaHash [0]);
}

template <class NodeType, class FieldType>
void
X3DHeightMapEditor <NodeType, FieldType>::set_height (const bool removeMetaData)
{
	if (removeMetaData)
	{
		node -> removeMetaData (HEIGHT);
		node -> removeMetaData (HEIGHT_HASH);
	}

	const auto height = this -> getHeight (false);

	if (not height .empty ())
	{
		const auto metaMinMax = std::minmax_element (height .begin (), height .end ());

		metaMinHeight = *metaMinMax .first;
		metaMaxHeight = *metaMinMax .second;
	}
	else
	{
		metaMinHeight = 0;
		metaMaxHeight = 0;
	}

	set_adjustments ();
	set_heightMap ();
}

template <class NodeType, class FieldType>
void
X3DHeightMapEditor <NodeType, FieldType>::set_adjustments ()
{
	changing = true;

	if (not node)
		return;

	if (not node -> height () .empty ())
	{
		const auto minMax = std::minmax_element (node -> height () .begin (), node -> height () .end ());

		minHeightAdjustment -> set_upper (getCurrentScene () -> toUnit (node -> height () .getUnit (), *minMax .second));
		maxHeightAdjustment -> set_lower (getCurrentScene () -> toUnit (node -> height () .getUnit (), *minMax .first));

		minHeightAdjustment -> set_value (getCurrentScene () -> toUnit (node -> height () .getUnit (), *minMax .first));
		maxHeightAdjustment -> set_value (getCurrentScene () -> toUnit (node -> height () .getUnit (), *minMax .second));
	}
	else
	{
		minHeightAdjustment -> set_upper (getCurrentScene () -> toUnit (node -> height () .getUnit (), 0));
		maxHeightAdjustment -> set_lower (getCurrentScene () -> toUnit (node -> height () .getUnit (), 0));

		minHeightAdjustment -> set_value (getCurrentScene () -> toUnit (node -> height () .getUnit (), 0));
		maxHeightAdjustment -> set_value (getCurrentScene () -> toUnit (node -> height () .getUnit (), 0));
	}

	changing = false;
}

template <class NodeType, class FieldType>
void
X3DHeightMapEditor <NodeType, FieldType>::on_height_map_min_max_height_changed (const int32_t input)
{
	if (changing)
		return;

	// Undo.

	if (input not_eq minMaxInput)
		resetUndoGroup ("height", undoStep);

	minMaxInput = input;

	const auto heighHashValue = node -> template getMetaData <X3D::MFString> (HEIGHT_HASH);

	beginUndoGroup ("height", undoStep);

	if (addUndoFunction (node, node -> height (), undoStep))
	{
		if (heighHashValue .empty ())
			undoStep -> addUndoFunction (&NodeType::removeMetaData, node, HEIGHT_HASH);
		else
			undoStep -> addUndoFunction (&NodeType::template setMetaData <std::string>, node, HEIGHT_HASH, heighHashValue [0] .raw ());
	}

	endUndoGroup ("height", undoStep);

	// Adjust lower and upper bounds of adjustments.

	auto minHeight = minHeightAdjustment -> get_value ();
	auto maxHeight = maxHeightAdjustment -> get_value ();

	minHeightAdjustment -> set_upper (maxHeight);
	maxHeightAdjustment -> set_lower (minHeight);

	// Determine node height from meta height.

	minHeight = getCurrentScene () -> fromUnit (node -> height () .getUnit (), minHeight);
	maxHeight = getCurrentScene () -> fromUnit (node -> height () .getUnit (), maxHeight);

	const auto metaHeight = getHeight (true);

	node -> height () .resize (metaHeight .size ());

	for (size_t i = 0, size = metaHeight .size (); i < size; ++ i)
		node -> height () [i] = math::project <typename FieldType::value_type> (metaHeight [i], metaMinHeight, metaMaxHeight, minHeight, maxHeight);

	const X3D::SFString heightHash (getHeightHash ());

	node -> setMetaData (HEIGHT_HASH, heightHash);

	node -> height () .removeInterest (&X3DHeightMapEditor::set_height_verified, this);
	node -> height () .addInterest (&X3DHeightMapEditor::connectHeight, this);

	// Redo.

	beginRedoGroup ("height", undoStep);

	if (addRedoFunction (node, node -> height (), undoStep))
		undoStep -> addRedoFunction (&NodeType::template setMetaData <X3D::SFString>, node, HEIGHT_HASH, heightHash);

	endRedoGroup ("height", undoStep);
}

template <class NodeType, class FieldType>
void
X3DHeightMapEditor <NodeType, FieldType>::connectHeight (const FieldType & field)
{
	field .removeInterest (&X3DHeightMapEditor::connectHeight, this);
	field .addInterest (&X3DHeightMapEditor::set_height_verified, this);
}

template <class NodeType, class FieldType>
void
X3DHeightMapEditor <NodeType, FieldType>::set_heightMap ()
{
	changing = true;

	const auto heightMaps = node -> template getMetaData <X3D::MFString> (HEIGHT_MAP);
	bool       exists     = false;

	for (const auto & value : heightMaps)
	{
		const auto heightMap = getCurrentContext () -> getWorldURL () .transform (value .raw ());

		if (Glib::file_test (heightMap .path (), Glib::FILE_TEST_EXISTS))
		{
			exists = true;

			fileChooser .set_current_folder_uri (heightMap .parent () .str ());
			fileChooser .set_uri (heightMap .str ());
			reloadButton .set_sensitive (true);
			break;
		}
	}

	if (not exists)
	{
		// No data available.

		fileChooser .set_uri ("");
		fileChooser .set_current_folder (Glib::get_home_dir ());
		reloadButton .set_sensitive (false);
	}

	changing = false;
}

template <class NodeType, class FieldType>
void
X3DHeightMapEditor <NodeType, FieldType>::on_height_map_image_set ()
{
	if (changing)
		return;

	const auto       undoStep    = std::make_shared <X3D::UndoStep> ("Set ElevationGrid Height Map Image");
	const basic::uri path        = fileChooser .get_file () -> get_path ();
	const basic::uri URL         = path .add_file_scheme ();
	const basic::uri relativeURL = getCurrentContext () -> getWorldURL () .relative_path (URL);
	const auto       url         = X3D::MFString ({ relativeURL .str (), URL .str () });
	const auto       minHeight   = getCurrentScene () -> fromUnit (node -> height () .getUnit (), minHeightAdjustment -> get_value ());
	auto             maxHeight   = getCurrentScene () -> fromUnit (node -> height () .getUnit (), maxHeightAdjustment -> get_value ());
	const auto       heightMap   = node -> template getMetaData <X3D::MFString> (HEIGHT_MAP);

	if (path .empty ())
		return;

	if (minHeight == maxHeight)
		maxHeight = minHeight + 10;

	undoStep -> addObjects (node);

	if (heightMap .empty ())
		undoStep -> addUndoFunction (&NodeType::removeMetaData, node, HEIGHT_MAP);

	undoStep -> addUndoFunction (&NodeType::template setMetaData <X3D::MFString>, node, HEIGHT_MAP, heightMap);
	undoStep -> addUndoFunction (&FieldType::setValue,  std::ref (node -> height ()), node -> height ());

	node -> template setMetaData <X3D::MFString> (HEIGHT_MAP, url);
	node -> loadHeightMap (url, minHeight, maxHeight);

	undoStep -> addRedoFunction (&NodeType::template setMetaData <X3D::MFString>, node, HEIGHT_MAP, X3D::MFString ({ URL .str () }));
	undoStep -> addRedoFunction (&NodeType::loadHeightMap, node, url, minHeight, maxHeight);

	X3D::X3DEditor::requestUpdateInstances (node, undoStep);

	addUndoStep (undoStep);
}

template <class NodeType, class FieldType>
void
X3DHeightMapEditor <NodeType, FieldType>::on_height_map_image_reload_clicked ()
{
	on_height_map_image_set ();
}

template <class NodeType, class FieldType>
void
X3DHeightMapEditor <NodeType, FieldType>::on_height_map_image_remove_clicked ()
{
	const auto undoStep  = std::make_shared <X3D::UndoStep> ("Remove ElevationGrid Height Map Image");
	const auto heightMap = node -> template getMetaData <X3D::MFString> (HEIGHT_MAP);

	undoStep -> addObjects (node);

	if (heightMap .empty ())
		undoStep -> addUndoFunction (&NodeType::removeMetaData, node, HEIGHT_MAP);

	undoStep -> addUndoFunction (&NodeType::template setMetaData <X3D::MFString>, node, HEIGHT_MAP, heightMap);
	undoStep -> addUndoFunction (&FieldType::setValue,  std::ref (node -> height ()), node -> height ());

	node -> removeMetaData (HEIGHT_MAP);
	node -> height () .clear ();

	undoStep -> addRedoFunction (&NodeType::removeMetaData, node, HEIGHT_MAP);
	undoStep -> addRedoFunction (&FieldType::setValue,  std::ref (node -> height ()), node -> height ());

	X3D::X3DEditor::requestUpdateInstances (node, undoStep);

	addUndoStep (undoStep);

	// Widgets

	fileChooser .set_uri ("");
	fileChooser .set_current_folder (Glib::get_home_dir ());
	reloadButton .set_sensitive (false);
}

template <class NodeType, class FieldType>
void
X3DHeightMapEditor <NodeType, FieldType>::set_loadState ()
{
	loadStateLabel .set_text (loadStateInfo (node -> checkLoadState ()));

	switch (node -> checkLoadState ())
	{
		case X3D::NOT_STARTED_STATE:
		{
			fileChooser  .set_sensitive (true);
			reloadButton .set_sensitive (not fileChooser .get_file () -> get_path () .empty ());
			removeButton .set_sensitive (true);
			break;
		}
		case X3D::IN_PROGRESS_STATE:
		{
			fileChooser  .set_sensitive (false);
			reloadButton .set_sensitive (false);
			removeButton .set_sensitive (false);
			break;
		}
		case X3D::COMPLETE_STATE:
		{
			fileChooser  .set_sensitive (true);
			reloadButton .set_sensitive (true);
			removeButton .set_sensitive (true);
			break;
		}
		case X3D::FAILED_STATE:
		{
			fileChooser  .set_sensitive (true);
			reloadButton .set_sensitive (false);
			removeButton .set_sensitive (true);
			break;
		}
	}
}

template <class NodeType, class FieldType>
X3DHeightMapEditor <NodeType, FieldType>::~X3DHeightMapEditor ()
{ }

} // puck
} // titania

#endif
