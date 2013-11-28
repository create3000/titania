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
#include "X3DMaterialEditorInterface.h"

namespace titania {
namespace puck {

const std::string X3DMaterialEditorInterface::m_widgetName = "MaterialEditor";

void
X3DMaterialEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	// Get objects.

	// Get widgets.
	m_builder -> get_widget ("Window", m_window);
	m_window -> set_name ("Window");
	m_builder -> get_widget ("Widget", m_widget);
	m_widget -> set_name ("Widget");
	m_builder -> get_widget ("PreviewBox", m_previewBox);
	m_previewBox -> set_name ("PreviewBox");
	m_builder -> get_widget ("DiffuseScale", m_diffuseScale);
	m_diffuseScale -> set_name ("DiffuseScale");
	m_builder -> get_widget ("SpecularScale", m_specularScale);
	m_specularScale -> set_name ("SpecularScale");
	m_builder -> get_widget ("EmissiveScale", m_emissiveScale);
	m_emissiveScale -> set_name ("EmissiveScale");
	m_builder -> get_widget ("AmbientScale", m_ambientScale);
	m_ambientScale -> set_name ("AmbientScale");
	m_builder -> get_widget ("ShininessScale", m_shininessScale);
	m_shininessScale -> set_name ("ShininessScale");
	m_builder -> get_widget ("TransparencyScale", m_transparencyScale);
	m_transparencyScale -> set_name ("TransparencyScale");
	m_builder -> get_widget ("DiffuseButton", m_diffuseButton);
	m_diffuseButton -> set_name ("DiffuseButton");
	m_builder -> get_widget ("SpecularButton", m_specularButton);
	m_specularButton -> set_name ("SpecularButton");
	m_builder -> get_widget ("EmissiveButton", m_emissiveButton);
	m_emissiveButton -> set_name ("EmissiveButton");
	m_builder -> get_widget ("AmbientButton", m_ambientButton);
	m_ambientButton -> set_name ("AmbientButton");
	m_builder -> get_widget ("ShininessButton", m_shininessButton);
	m_shininessButton -> set_name ("ShininessButton");
	m_builder -> get_widget ("TransparencyButton", m_transparencyButton);
	m_transparencyButton -> set_name ("TransparencyButton");

	// Call construct handler of base class.
	construct ();
}

X3DMaterialEditorInterface::~X3DMaterialEditorInterface ()
{
	delete m_window;
}

} // puck
} // titania
