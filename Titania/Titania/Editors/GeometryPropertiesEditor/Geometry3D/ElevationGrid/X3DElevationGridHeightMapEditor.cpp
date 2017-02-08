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

#include "X3DElevationGridHeightMapEditor.h"

#include <Titania/X3D/Components/Geometry3D/ElevationGrid.h>
#include <Titania/OS/home.h>

namespace titania {
namespace puck {

static const std::string HEIGHT     = "/ElevationGrid/height";
static const std::string HEIGHT_MAP = "/ElevationGrid/heightMap";

X3DElevationGridHeightMapEditor::X3DElevationGridHeightMapEditor () :
	X3DGeometryPropertiesEditorInterface (),
	                               scene (),
	                                node (),
	                       metaMinHeight (0),
	                       metaMaxHeight (0),
	                            changing (false)
{
	addChildObjects (scene, node);
}

void
X3DElevationGridHeightMapEditor::initialize ()
{
	getCurrentScene () .addInterest (&X3DElevationGridHeightMapEditor::set_scene, this);
}

void
X3DElevationGridHeightMapEditor::setNode (const X3D::X3DPtr <X3D::ElevationGrid> & value)
{
	if (value == node)
		return;

	if (node)
	{
		node -> xDimension () .removeInterest (&X3DElevationGridHeightMapEditor::on_elevation_grid_height_map_image_set, this);
		node -> zDimension () .removeInterest (&X3DElevationGridHeightMapEditor::on_elevation_grid_height_map_image_set, this);
		node -> height ()     .removeInterest (&X3DElevationGridHeightMapEditor::set_height, this);
	}

	node = value;

	if (node)
	{
		node -> xDimension () .addInterest (&X3DElevationGridHeightMapEditor::on_elevation_grid_height_map_image_set, this);
		node -> zDimension () .addInterest (&X3DElevationGridHeightMapEditor::on_elevation_grid_height_map_image_set, this);
		node -> height ()     .addInterest (&X3DElevationGridHeightMapEditor::set_height, this, true);

		set_height (false);
		set_heightMap ();
	}
}

X3D::MFFloat &
X3DElevationGridHeightMapEditor::getHeight (const bool fromMetaData)
{
	try
	{
		return node -> getMetaData <X3D::MFFloat> (HEIGHT, false);
	}
	catch (const X3D::X3DError &)
	{
		if (fromMetaData)
		{
			node -> setMetaData <X3D::MFFloat> (HEIGHT, node -> height ());
	
			return node -> getMetaData <X3D::MFFloat> (HEIGHT, false);
		}

		return node -> height ();
	}
}

void
X3DElevationGridHeightMapEditor::set_scene ()
{
	// Connect units.

	if (scene)
		scene -> units_changed () .removeInterest (&X3DElevationGridHeightMapEditor::set_adjustments, this);

	scene = getCurrentScene ();

	scene -> units_changed () .addInterest (&X3DElevationGridHeightMapEditor::set_adjustments, this);
}

void
X3DElevationGridHeightMapEditor::set_height (const bool removeMetaData)
{
	if (removeMetaData)
		node -> removeMetaData (HEIGHT);

	const auto & height     = getHeight (false);
	const auto   metaMinMax = std::minmax_element (height .begin (), height .end ());

	metaMinHeight = *metaMinMax .first;
	metaMaxHeight = *metaMinMax .second;

	set_adjustments ();
}

void
X3DElevationGridHeightMapEditor::set_adjustments ()
{
	changing = true;

	const auto minMax = std::minmax_element (node -> height () .begin (), node -> height () .end ());

	getElevationGridHeightMapMinHeightAdjustment () -> set_upper (getCurrentScene () -> toUnit (node -> height () .getUnit (), *minMax .second));
	getElevationGridHeightMapMaxHeightAdjustment () -> set_lower (getCurrentScene () -> toUnit (node -> height () .getUnit (), *minMax .first));

	getElevationGridHeightMapMinHeightAdjustment () -> set_value (getCurrentScene () -> toUnit (node -> height () .getUnit (), *minMax .first));
	getElevationGridHeightMapMaxHeightAdjustment () -> set_value (getCurrentScene () -> toUnit (node -> height () .getUnit (), *minMax .second));

	changing = false;
}

void
X3DElevationGridHeightMapEditor::on_elevation_grid_height_map_min_height_changed ()
{
	on_elevation_grid_height_map_min_max_height_changed ();
}

void
X3DElevationGridHeightMapEditor::on_elevation_grid_height_map_max_height_changed ()
{
	on_elevation_grid_height_map_min_max_height_changed ();
}

void
X3DElevationGridHeightMapEditor::on_elevation_grid_height_map_min_max_height_changed ()
{
	if (changing)
		return;

	// Adjust lower and upper bounds of adjustments.

	auto minHeight = getElevationGridHeightMapMinHeightAdjustment () -> get_value ();
	auto maxHeight = getElevationGridHeightMapMaxHeightAdjustment () -> get_value ();

	getElevationGridHeightMapMinHeightAdjustment () -> set_upper (maxHeight);
	getElevationGridHeightMapMaxHeightAdjustment () -> set_lower (minHeight);

	// Determine node height from meta height.

	minHeight = getCurrentScene () -> fromUnit (node -> height () .getUnit (), minHeight);
	maxHeight = getCurrentScene () -> fromUnit (node -> height () .getUnit (), maxHeight);

	const auto & metaHeight = getHeight (true);

	node -> height () .resize (metaHeight .size ());

	for (size_t i = 0, size = metaHeight .size (); i < size; ++ i)
		node -> height () [i] = math::project <float> (metaHeight [i], metaMinHeight, metaMaxHeight, minHeight, maxHeight);

	node -> height () .removeInterest (&X3DElevationGridHeightMapEditor::set_height, this);
	node -> height () .addInterest (&X3DElevationGridHeightMapEditor::connectHeight, this);
}

void
X3DElevationGridHeightMapEditor::connectHeight (const X3D::MFFloat & field)
{
	field .removeInterest (&X3DElevationGridHeightMapEditor::connectHeight, this);
	field .addInterest (&X3DElevationGridHeightMapEditor::set_height, this, true);
}

void
X3DElevationGridHeightMapEditor::set_heightMap ()
{
	try
	{
		const auto & heightMap = node -> getMetaData <X3D::MFString> (HEIGHT_MAP, false);

		getElevationGridHeightMapImageChooserButton () .set_uri (heightMap .at (0));
		getElevationGridHeightMapImageReloadButton () .set_sensitive (true);
	}
	catch (const std::exception &)
	{
		// No data available.

		getElevationGridHeightMapImageChooserButton () .set_uri ("");
		getElevationGridHeightMapImageChooserButton () .set_current_folder (os::home ());
		getElevationGridHeightMapImageReloadButton () .set_sensitive (false);
	}
}

void
X3DElevationGridHeightMapEditor::on_elevation_grid_height_map_image_set ()
{
	const std::string path = getElevationGridHeightMapImageChooserButton () .get_file () -> get_path ();

	if (path .empty ())
		return;

	const basic::uri URL       = "file://" + path;
	const auto       minHeight = getCurrentScene () -> fromUnit (node -> height () .getUnit (), getElevationGridHeightMapMinHeightAdjustment () -> get_value ());
	const auto       maxHeight = getCurrentScene () -> fromUnit (node -> height () .getUnit (), getElevationGridHeightMapMaxHeightAdjustment () -> get_value ());

	node -> setMetaData <X3D::MFString> (HEIGHT_MAP, { URL .str () });

	node -> setHeightMap ({ URL .str () }, minHeight, minHeight != maxHeight ? maxHeight : minHeight + 10);

	getElevationGridHeightMapImageReloadButton () .set_sensitive (true);
}

void
X3DElevationGridHeightMapEditor::on_elevation_grid_height_map_image_reload_clicked ()
{
	__LOG__ << std::endl;

	on_elevation_grid_height_map_image_set ();
}

void
X3DElevationGridHeightMapEditor::on_elevation_grid_height_map_image_remove_clicked ()
{
	getElevationGridHeightMapImageChooserButton () .set_uri ("");
	getElevationGridHeightMapImageChooserButton () .set_current_folder (os::home ());
	getElevationGridHeightMapImageReloadButton () .set_sensitive (false);

	node -> removeMetaData (HEIGHT_MAP);

	node -> height () .clear ();
}

X3DElevationGridHeightMapEditor::~X3DElevationGridHeightMapEditor ()
{ }

} // puck
} // titania
