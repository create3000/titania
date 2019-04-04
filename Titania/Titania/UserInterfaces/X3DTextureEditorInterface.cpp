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

#include "X3DTextureEditorInterface.h"

namespace titania {
namespace puck {

void
X3DTextureEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	create ();
}

void
X3DTextureEditorInterface::create (std::initializer_list <std::string> filenames)
{
	// Create Builder.
	m_builder = Gtk::Builder::create ();

	for (const auto & filename : filenames)
		m_builder -> add_from_file (filename);

	create ();
}

void
X3DTextureEditorInterface::create ()
{
	// Get objects.
	m_GeneratedCubeMapTextureSizeAdjustment          = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeneratedCubeMapTextureSizeAdjustment"));
	m_MovieTextureSpeedAdjustment                    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("MovieTextureSpeedAdjustment"));
	m_MultiTextureAlphaAdjustment                    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("MultiTextureAlphaAdjustment"));
	m_MultiTextureColorAdjustment                    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("MultiTextureColorAdjustment"));
	m_MultiTextureFunctionListStore                  = Glib::RefPtr <Gtk::ListStore>::cast_dynamic (m_builder -> get_object ("MultiTextureFunctionListStore"));
	m_MultiTextureModeListStore                      = Glib::RefPtr <Gtk::ListStore>::cast_dynamic (m_builder -> get_object ("MultiTextureModeListStore"));
	m_MultiTextureSourceListStore                    = Glib::RefPtr <Gtk::ListStore>::cast_dynamic (m_builder -> get_object ("MultiTextureSourceListStore"));
	m_TextureCoordinateGeneratorParameter0Adjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureCoordinateGeneratorParameter0Adjustment"));
	m_TextureCoordinateGeneratorParameter1Adjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureCoordinateGeneratorParameter1Adjustment"));
	m_TextureCoordinateGeneratorParameter2Adjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureCoordinateGeneratorParameter2Adjustment"));
	m_TextureCoordinateGeneratorParameter3Adjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureCoordinateGeneratorParameter3Adjustment"));
	m_TextureCoordinateGeneratorParameter4Adjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureCoordinateGeneratorParameter4Adjustment"));
	m_TextureCoordinateGeneratorParameter5Adjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureCoordinateGeneratorParameter5Adjustment"));
	m_TexturePropertiesAnisotropicDegreeAdjustment   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TexturePropertiesAnisotropicDegreeAdjustment"));
	m_TexturePropertiesBorderColorAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TexturePropertiesBorderColorAdjustment"));
	m_TexturePropertiesBorderWidthAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TexturePropertiesBorderWidthAdjustment"));
	m_TexturePropertiesTexturePriorityAdjustment     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TexturePropertiesTexturePriorityAdjustment"));
	m_TextureTransform3DCenterXAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransform3DCenterXAdjustment"));
	m_TextureTransform3DCenterYAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransform3DCenterYAdjustment"));
	m_TextureTransform3DCenterZAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransform3DCenterZAdjustment"));
	m_TextureTransform3DRotationAAdjustment          = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransform3DRotationAAdjustment"));
	m_TextureTransform3DRotationEulerXAdjustment     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransform3DRotationEulerXAdjustment"));
	m_TextureTransform3DRotationEulerYAdjustment     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransform3DRotationEulerYAdjustment"));
	m_TextureTransform3DRotationEulerZAdjustment     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransform3DRotationEulerZAdjustment"));
	m_TextureTransform3DRotationXAdjustment          = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransform3DRotationXAdjustment"));
	m_TextureTransform3DRotationYAdjustment          = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransform3DRotationYAdjustment"));
	m_TextureTransform3DRotationZAdjustment          = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransform3DRotationZAdjustment"));
	m_TextureTransform3DScaleXAdjustment             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransform3DScaleXAdjustment"));
	m_TextureTransform3DScaleYAdjustment             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransform3DScaleYAdjustment"));
	m_TextureTransform3DScaleZAdjustment             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransform3DScaleZAdjustment"));
	m_TextureTransform3DTranslationXAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransform3DTranslationXAdjustment"));
	m_TextureTransform3DTranslationYAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransform3DTranslationYAdjustment"));
	m_TextureTransform3DTranslationZAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransform3DTranslationZAdjustment"));
	m_TextureTransformCenterXAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformCenterXAdjustment"));
	m_TextureTransformCenterYAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformCenterYAdjustment"));
	m_TextureTransformMatrix3D00Adjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformMatrix3D00Adjustment"));
	m_TextureTransformMatrix3D01Adjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformMatrix3D01Adjustment"));
	m_TextureTransformMatrix3D02Adjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformMatrix3D02Adjustment"));
	m_TextureTransformMatrix3D03Adjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformMatrix3D03Adjustment"));
	m_TextureTransformMatrix3D04Adjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformMatrix3D04Adjustment"));
	m_TextureTransformMatrix3D05Adjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformMatrix3D05Adjustment"));
	m_TextureTransformMatrix3D06Adjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformMatrix3D06Adjustment"));
	m_TextureTransformMatrix3D07Adjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformMatrix3D07Adjustment"));
	m_TextureTransformMatrix3D08Adjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformMatrix3D08Adjustment"));
	m_TextureTransformMatrix3D09Adjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformMatrix3D09Adjustment"));
	m_TextureTransformMatrix3D10Adjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformMatrix3D10Adjustment"));
	m_TextureTransformMatrix3D11Adjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformMatrix3D11Adjustment"));
	m_TextureTransformMatrix3D12Adjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformMatrix3D12Adjustment"));
	m_TextureTransformMatrix3D13Adjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformMatrix3D13Adjustment"));
	m_TextureTransformMatrix3D14Adjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformMatrix3D14Adjustment"));
	m_TextureTransformMatrix3D15Adjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformMatrix3D15Adjustment"));
	m_TextureTransformRotationAdjustment             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformRotationAdjustment"));
	m_TextureTransformScaleXAdjustment               = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformScaleXAdjustment"));
	m_TextureTransformScaleYAdjustment               = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformScaleYAdjustment"));
	m_TextureTransformTranslationXAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformTranslationXAdjustment"));
	m_TextureTransformTranslationYAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureTransformTranslationYAdjustment"));
	m_MultiTextureModeCellRendererCombo              = Glib::RefPtr <Gtk::CellRendererCombo>::cast_dynamic (m_builder -> get_object ("MultiTextureModeCellRendererCombo"));
	m_MultiTextureSourceCellRendererCombo            = Glib::RefPtr <Gtk::CellRendererCombo>::cast_dynamic (m_builder -> get_object ("MultiTextureSourceCellRendererCombo"));
	m_MultiTextureFunctionCellRendererCombo          = Glib::RefPtr <Gtk::CellRendererCombo>::cast_dynamic (m_builder -> get_object ("MultiTextureFunctionCellRendererCombo"));
	m_ImageTextureURLCellRendererText                = Glib::RefPtr <Gtk::CellRendererText>::cast_dynamic (m_builder -> get_object ("ImageTextureURLCellRendererText"));
	m_ImageTextureURLChooserColumn                   = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("ImageTextureURLChooserColumn"));
	m_ImageTextureURLCellrendererPixbuf              = Glib::RefPtr <Gtk::CellRendererPixbuf>::cast_dynamic (m_builder -> get_object ("ImageTextureURLCellrendererPixbuf"));
	m_MovieTextureURLCellRendererText                = Glib::RefPtr <Gtk::CellRendererText>::cast_dynamic (m_builder -> get_object ("MovieTextureURLCellRendererText"));
	m_MovieTextureURLChooserColumn                   = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("MovieTextureURLChooserColumn"));
	m_MovieTextureURLCellrendererPixbuf              = Glib::RefPtr <Gtk::CellRendererPixbuf>::cast_dynamic (m_builder -> get_object ("MovieTextureURLCellrendererPixbuf"));
	m_ImageTexture3DURLCellRendererText              = Glib::RefPtr <Gtk::CellRendererText>::cast_dynamic (m_builder -> get_object ("ImageTexture3DURLCellRendererText"));
	m_ImageTexture3DURLChooserColumn                 = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("ImageTexture3DURLChooserColumn"));
	m_ImageTexture3DURLCellrendererPixbuf            = Glib::RefPtr <Gtk::CellRendererPixbuf>::cast_dynamic (m_builder -> get_object ("ImageTexture3DURLCellrendererPixbuf"));
	m_ImageCubeMapTextureURLCellRendererText         = Glib::RefPtr <Gtk::CellRendererText>::cast_dynamic (m_builder -> get_object ("ImageCubeMapTextureURLCellRendererText"));
	m_ImageCubeMapTextureURLChooserColumn            = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("ImageCubeMapTextureURLChooserColumn"));
	m_ImageTextureURLCellrendererPixbuf1             = Glib::RefPtr <Gtk::CellRendererPixbuf>::cast_dynamic (m_builder -> get_object ("ImageTextureURLCellrendererPixbuf1"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("HeaderBar", m_HeaderBar);
	m_builder -> get_widget ("TextureChildNotebook", m_TextureChildNotebook);
	m_builder -> get_widget ("TextureExpander", m_TextureExpander);
	m_builder -> get_widget ("TextureBox", m_TextureBox);
	m_builder -> get_widget ("SelectTextureBox", m_SelectTextureBox);
	m_builder -> get_widget ("TextureComboBoxText", m_TextureComboBoxText);
	m_builder -> get_widget ("TextureUnlinkButton", m_TextureUnlinkButton);
	m_builder -> get_widget ("TextureNameGrid", m_TextureNameGrid);
	m_builder -> get_widget ("TextureNameBox", m_TextureNameBox);
	m_builder -> get_widget ("TextureNameEntry", m_TextureNameEntry);
	m_builder -> get_widget ("TextureRenameButton", m_TextureRenameButton);
	m_builder -> get_widget ("TextureFormatLabel", m_TextureFormatLabel);
	m_builder -> get_widget ("TextureLoadStateLabel", m_TextureLoadStateLabel);
	m_builder -> get_widget ("PreviewBox", m_PreviewBox);
	m_builder -> get_widget ("TextureNotebook", m_TextureNotebook);
	m_builder -> get_widget ("TextureLabel", m_TextureLabel);
	m_builder -> get_widget ("MultiTextureBox", m_MultiTextureBox);
	m_builder -> get_widget ("MultiTextureColorBox", m_MultiTextureColorBox);
	m_builder -> get_widget ("MultiTextureColorButton", m_MultiTextureColorButton);
	m_builder -> get_widget ("MultiTextureColorScale", m_MultiTextureColorScale);
	m_builder -> get_widget ("MultiTextureAlphaBox", m_MultiTextureAlphaBox);
	m_builder -> get_widget ("MultiTextureAlphaScale", m_MultiTextureAlphaScale);
	m_builder -> get_widget ("MultiTextureModeBox", m_MultiTextureModeBox);
	m_builder -> get_widget ("MultiTextureModeTreeView", m_MultiTextureModeTreeView);
	m_builder -> get_widget ("MultiTextureModeAddButton", m_MultiTextureModeAddButton);
	m_builder -> get_widget ("MultiTextureModeRemoveButton", m_MultiTextureModeRemoveButton);
	m_builder -> get_widget ("MultiTextureSourceBox", m_MultiTextureSourceBox);
	m_builder -> get_widget ("MultiTextureSourceTreeView", m_MultiTextureSourceTreeView);
	m_builder -> get_widget ("MultiTextureSourceAddButton", m_MultiTextureSourceAddButton);
	m_builder -> get_widget ("MultiTextureSourceRemoveButton", m_MultiTextureSourceRemoveButton);
	m_builder -> get_widget ("MultiTextureFunctionBox", m_MultiTextureFunctionBox);
	m_builder -> get_widget ("MultiTextureFunctionTreeView", m_MultiTextureFunctionTreeView);
	m_builder -> get_widget ("MultiTextureFunctionAddButton", m_MultiTextureFunctionAddButton);
	m_builder -> get_widget ("MultiTextureFunctionRemoveButton", m_MultiTextureFunctionRemoveButton);
	m_builder -> get_widget ("ImageTextureBox", m_ImageTextureBox);
	m_builder -> get_widget ("ImageTextureURLBox", m_ImageTextureURLBox);
	m_builder -> get_widget ("ImageTextureURLTreeView", m_ImageTextureURLTreeView);
	m_builder -> get_widget ("ImageTextureURLAddButton", m_ImageTextureURLAddButton);
	m_builder -> get_widget ("ImageTextureURLRemoveButton", m_ImageTextureURLRemoveButton);
	m_builder -> get_widget ("ImageTextureEmbedButton", m_ImageTextureEmbedButton);
	m_builder -> get_widget ("ImageTextureURLReloadButton", m_ImageTextureURLReloadButton);
	m_builder -> get_widget ("PixelTextureBox", m_PixelTextureBox);
	m_builder -> get_widget ("PixelTextureOpenButton", m_PixelTextureOpenButton);
	m_builder -> get_widget ("PixelTextureSaveAsButton", m_PixelTextureSaveAsButton);
	m_builder -> get_widget ("MovieTextureBox", m_MovieTextureBox);
	m_builder -> get_widget ("MovieTextureEnabledCheckButton", m_MovieTextureEnabledCheckButton);
	m_builder -> get_widget ("MovieTextureURLBox", m_MovieTextureURLBox);
	m_builder -> get_widget ("MovieTextureURLTreeView", m_MovieTextureURLTreeView);
	m_builder -> get_widget ("MovieTextureURLAddButton", m_MovieTextureURLAddButton);
	m_builder -> get_widget ("MovieTextureURLRemoveButton", m_MovieTextureURLRemoveButton);
	m_builder -> get_widget ("MovieTextureURLReloadButton", m_MovieTextureURLReloadButton);
	m_builder -> get_widget ("MovieTextureDescriptionEntry", m_MovieTextureDescriptionEntry);
	m_builder -> get_widget ("MovieTextureSpeedSpinButton", m_MovieTextureSpeedSpinButton);
	m_builder -> get_widget ("MovieTextureControlsBox", m_MovieTextureControlsBox);
	m_builder -> get_widget ("MovieTextureLoopToggleButton", m_MovieTextureLoopToggleButton);
	m_builder -> get_widget ("MovieTexturePlayPauseButton", m_MovieTexturePlayPauseButton);
	m_builder -> get_widget ("MovieTexturePlayPauseImage", m_MovieTexturePlayPauseImage);
	m_builder -> get_widget ("MovieTextureStopButton", m_MovieTextureStopButton);
	m_builder -> get_widget ("MovieTextureStopImage", m_MovieTextureStopImage);
	m_builder -> get_widget ("MovieTextureElapsedTimeLabel", m_MovieTextureElapsedTimeLabel);
	m_builder -> get_widget ("MovieTextureDurationLabel", m_MovieTextureDurationLabel);
	m_builder -> get_widget ("ComposedTexture3DBox", m_ComposedTexture3DBox);
	m_builder -> get_widget ("ImageTexture3DBox", m_ImageTexture3DBox);
	m_builder -> get_widget ("ImageTexture3DURLBox", m_ImageTexture3DURLBox);
	m_builder -> get_widget ("ImageTexture3DURLTreeView", m_ImageTexture3DURLTreeView);
	m_builder -> get_widget ("ImageTexture3DURLAddButton", m_ImageTexture3DURLAddButton);
	m_builder -> get_widget ("ImageTexture3DURLRemoveButton", m_ImageTexture3DURLRemoveButton);
	m_builder -> get_widget ("ImageTexture3DEmbedButton", m_ImageTexture3DEmbedButton);
	m_builder -> get_widget ("ImageTexture3DURLReloadButton", m_ImageTexture3DURLReloadButton);
	m_builder -> get_widget ("PixelTexture3DBox", m_PixelTexture3DBox);
	m_builder -> get_widget ("ComposedCubeMapTextureBox", m_ComposedCubeMapTextureBox);
	m_builder -> get_widget ("ComposedCubeMapNotebook", m_ComposedCubeMapNotebook);
	m_builder -> get_widget ("ComposedCubeMapFrontTextureFormatLabel", m_ComposedCubeMapFrontTextureFormatLabel);
	m_builder -> get_widget ("ComposedCubeMapFrontTextureLoadStateLabel", m_ComposedCubeMapFrontTextureLoadStateLabel);
	m_builder -> get_widget ("ComposedCubeMapFrontTexturePreviewBox", m_ComposedCubeMapFrontTexturePreviewBox);
	m_builder -> get_widget ("ComposedCubeMapBackTextureFormatLabel", m_ComposedCubeMapBackTextureFormatLabel);
	m_builder -> get_widget ("ComposedCubeMapBackTextureLoadStateLabel", m_ComposedCubeMapBackTextureLoadStateLabel);
	m_builder -> get_widget ("ComposedCubeMapBackTexturePreviewBox", m_ComposedCubeMapBackTexturePreviewBox);
	m_builder -> get_widget ("ComposedCubeMapLeftTextureFormatLabel", m_ComposedCubeMapLeftTextureFormatLabel);
	m_builder -> get_widget ("ComposedCubeMapLeftTextureLoadStateLabel", m_ComposedCubeMapLeftTextureLoadStateLabel);
	m_builder -> get_widget ("ComposedCubeMapLeftTexturePreviewBox", m_ComposedCubeMapLeftTexturePreviewBox);
	m_builder -> get_widget ("ComposedCubeMapRightTextureFormatLabel", m_ComposedCubeMapRightTextureFormatLabel);
	m_builder -> get_widget ("ComposedCubeMapRightTextureLoadStateLabel", m_ComposedCubeMapRightTextureLoadStateLabel);
	m_builder -> get_widget ("ComposedCubeMapRightTexturePreviewBox", m_ComposedCubeMapRightTexturePreviewBox);
	m_builder -> get_widget ("ComposedCubeMapTopTextureFormatLabel", m_ComposedCubeMapTopTextureFormatLabel);
	m_builder -> get_widget ("ComposedCubeMapTopTextureLoadStateLabel", m_ComposedCubeMapTopTextureLoadStateLabel);
	m_builder -> get_widget ("ComposedCubeMapTopTexturePreviewBox", m_ComposedCubeMapTopTexturePreviewBox);
	m_builder -> get_widget ("ComposedCubeMapBottomTextureFormatLabel", m_ComposedCubeMapBottomTextureFormatLabel);
	m_builder -> get_widget ("ComposedCubeMapBottomTextureLoadStateLabel", m_ComposedCubeMapBottomTextureLoadStateLabel);
	m_builder -> get_widget ("ComposedCubeMapBottomTexturePreviewBox", m_ComposedCubeMapBottomTexturePreviewBox);
	m_builder -> get_widget ("GeneratedCubeMapTextureBox", m_GeneratedCubeMapTextureBox);
	m_builder -> get_widget ("GeneratedCubeMapTexturSizeSpinButton", m_GeneratedCubeMapTexturSizeSpinButton);
	m_builder -> get_widget ("GeneratedCubeMapTextureUpdateComboBoxText", m_GeneratedCubeMapTextureUpdateComboBoxText);
	m_builder -> get_widget ("ImageCubeMapTextureBox", m_ImageCubeMapTextureBox);
	m_builder -> get_widget ("ImageCubeMapTextureFormatLabel", m_ImageCubeMapTextureFormatLabel);
	m_builder -> get_widget ("ImageCubeMapTextureLoadStateLabel", m_ImageCubeMapTextureLoadStateLabel);
	m_builder -> get_widget ("ImageCubeMapTexturePreviewBox", m_ImageCubeMapTexturePreviewBox);
	m_builder -> get_widget ("ImageCubeMapTextureURLBox", m_ImageCubeMapTextureURLBox);
	m_builder -> get_widget ("ImageCubeMapTextureURLTreeView", m_ImageCubeMapTextureURLTreeView);
	m_builder -> get_widget ("ImageCubeMapTextureURLAddButton", m_ImageCubeMapTextureURLAddButton);
	m_builder -> get_widget ("ImageCubeMapTextureURLRemoveButton", m_ImageCubeMapTextureURLRemoveButton);
	m_builder -> get_widget ("ImageCubeMapTextureEmbedButton", m_ImageCubeMapTextureEmbedButton);
	m_builder -> get_widget ("ImageCubeMapTextureURLReloadButton", m_ImageCubeMapTextureURLReloadButton);
	m_builder -> get_widget ("Texture2DBox", m_Texture2DBox);
	m_builder -> get_widget ("Texture2DNodeRepeatSCheckButton", m_Texture2DNodeRepeatSCheckButton);
	m_builder -> get_widget ("Texture2DNodeRepeatTCheckButton", m_Texture2DNodeRepeatTCheckButton);
	m_builder -> get_widget ("Texture3DBox", m_Texture3DBox);
	m_builder -> get_widget ("Texture3DNodeRepeatSCheckButton", m_Texture3DNodeRepeatSCheckButton);
	m_builder -> get_widget ("Texture3DNodeRepeatTCheckButton", m_Texture3DNodeRepeatTCheckButton);
	m_builder -> get_widget ("Texture3DNodeRepeatRCheckButton", m_Texture3DNodeRepeatRCheckButton);
	m_builder -> get_widget ("TexturePropertiesExpander", m_TexturePropertiesExpander);
	m_builder -> get_widget ("SelectTexturePropertiesBox", m_SelectTexturePropertiesBox);
	m_builder -> get_widget ("TexturePropertiesCheckButton", m_TexturePropertiesCheckButton);
	m_builder -> get_widget ("TexturePropertiesUnlinkButton", m_TexturePropertiesUnlinkButton);
	m_builder -> get_widget ("TexturePropertiesBox", m_TexturePropertiesBox);
	m_builder -> get_widget ("TexturePropertiesGenerateMipMapsCheckButton", m_TexturePropertiesGenerateMipMapsCheckButton);
	m_builder -> get_widget ("TexturePropertiesBorderWidthSpinButton", m_TexturePropertiesBorderWidthSpinButton);
	m_builder -> get_widget ("TexturePropertiesAnisotropicDegreeSpinButton", m_TexturePropertiesAnisotropicDegreeSpinButton);
	m_builder -> get_widget ("TexturePropertiesMinificationFilterComboBoxText", m_TexturePropertiesMinificationFilterComboBoxText);
	m_builder -> get_widget ("TexturePropertiesMagnificationFilterComboBoxText", m_TexturePropertiesMagnificationFilterComboBoxText);
	m_builder -> get_widget ("TexturePropertiesBoundaryModeSComboBoxText", m_TexturePropertiesBoundaryModeSComboBoxText);
	m_builder -> get_widget ("TexturePropertiesTextureCompressionComboBoxText", m_TexturePropertiesTextureCompressionComboBoxText);
	m_builder -> get_widget ("TexturePropertiesTexturePrioritySpinButton", m_TexturePropertiesTexturePrioritySpinButton);
	m_builder -> get_widget ("TexturePropertiesBoundaryModeTComboBoxText", m_TexturePropertiesBoundaryModeTComboBoxText);
	m_builder -> get_widget ("TexturePropertiesBoundaryModeRComboBoxText", m_TexturePropertiesBoundaryModeRComboBoxText);
	m_builder -> get_widget ("TexturePropertiesBorderColorBox", m_TexturePropertiesBorderColorBox);
	m_builder -> get_widget ("TexturePropertiesBorderColorButton", m_TexturePropertiesBorderColorButton);
	m_builder -> get_widget ("TexturePropertiesBorderColorScale", m_TexturePropertiesBorderColorScale);
	m_builder -> get_widget ("TexturePropertiesNameBox", m_TexturePropertiesNameBox);
	m_builder -> get_widget ("TexturePropertiesNameEntry", m_TexturePropertiesNameEntry);
	m_builder -> get_widget ("TexturePropertiesRenameButton", m_TexturePropertiesRenameButton);
	m_builder -> get_widget ("TextureTransformExpander", m_TextureTransformExpander);
	m_builder -> get_widget ("TextureTransformNodeBox", m_TextureTransformNodeBox);
	m_builder -> get_widget ("SelectTextureTransformBox", m_SelectTextureTransformBox);
	m_builder -> get_widget ("TextureTransformComboBoxText", m_TextureTransformComboBoxText);
	m_builder -> get_widget ("TextureTransformUnlinkButton", m_TextureTransformUnlinkButton);
	m_builder -> get_widget ("TextureTransformNameGrid", m_TextureTransformNameGrid);
	m_builder -> get_widget ("TextureTransformNameBox", m_TextureTransformNameBox);
	m_builder -> get_widget ("TextureTransformNameEntry", m_TextureTransformNameEntry);
	m_builder -> get_widget ("TextureTransformRenameButton", m_TextureTransformRenameButton);
	m_builder -> get_widget ("TextureTransformNotebook", m_TextureTransformNotebook);
	m_builder -> get_widget ("TextureTransformLabel", m_TextureTransformLabel);
	m_builder -> get_widget ("TextureTransformBox", m_TextureTransformBox);
	m_builder -> get_widget ("TextureTransformTranslationBox", m_TextureTransformTranslationBox);
	m_builder -> get_widget ("TextureTransformTranslationXSpinButton", m_TextureTransformTranslationXSpinButton);
	m_builder -> get_widget ("TextureTransformTranslationYSpinButton", m_TextureTransformTranslationYSpinButton);
	m_builder -> get_widget ("TextureTransformScaleBox", m_TextureTransformScaleBox);
	m_builder -> get_widget ("TextureTransformScaleXSpinButton", m_TextureTransformScaleXSpinButton);
	m_builder -> get_widget ("TextureTransformScaleYSpinButton", m_TextureTransformScaleYSpinButton);
	m_builder -> get_widget ("TextureTransformUniformScaleButton", m_TextureTransformUniformScaleButton);
	m_builder -> get_widget ("TextureTransformUniformScaleImage", m_TextureTransformUniformScaleImage);
	m_builder -> get_widget ("TextureTransformCenterBox", m_TextureTransformCenterBox);
	m_builder -> get_widget ("TextureTransformCenterXSpinButton", m_TextureTransformCenterXSpinButton);
	m_builder -> get_widget ("TextureTransformCenterYSpinButton", m_TextureTransformCenterYSpinButton);
	m_builder -> get_widget ("TextureTransformRotationSpinButton", m_TextureTransformRotationSpinButton);
	m_builder -> get_widget ("TextureTransform3DBox", m_TextureTransform3DBox);
	m_builder -> get_widget ("TextureTransform3DTranslationBox", m_TextureTransform3DTranslationBox);
	m_builder -> get_widget ("TextureTransform3DRotationToolBox", m_TextureTransform3DRotationToolBox);
	m_builder -> get_widget ("TextureTransform3DRotationNotebook", m_TextureTransform3DRotationNotebook);
	m_builder -> get_widget ("TextureTransform3DRotationBox", m_TextureTransform3DRotationBox);
	m_builder -> get_widget ("TextureTransform3DRotationEulerBox", m_TextureTransform3DRotationEulerBox);
	m_builder -> get_widget ("TextureTransform3DRotationTypeButton", m_TextureTransform3DRotationTypeButton);
	m_builder -> get_widget ("TextureTransform3DScaleBox", m_TextureTransform3DScaleBox);
	m_builder -> get_widget ("TextureTransform3DUniformScaleButton", m_TextureTransform3DUniformScaleButton);
	m_builder -> get_widget ("TextureTransform3DUniformScaleImage", m_TextureTransform3DUniformScaleImage);
	m_builder -> get_widget ("TextureTransform3DCenterBox", m_TextureTransform3DCenterBox);
	m_builder -> get_widget ("TextureTransformMatrix3DBox", m_TextureTransformMatrix3DBox);
	m_builder -> get_widget ("TextureTransformMatrix3DMatrixGrid", m_TextureTransformMatrix3DMatrixGrid);
	m_builder -> get_widget ("TextureCoordinateGeneratorExpander", m_TextureCoordinateGeneratorExpander);
	m_builder -> get_widget ("TextureCoordinateGeneratorMainBox", m_TextureCoordinateGeneratorMainBox);
	m_builder -> get_widget ("SelectTextureCoordinateGeneratorBox", m_SelectTextureCoordinateGeneratorBox);
	m_builder -> get_widget ("TextureCoordinateGeneratorCheckButton", m_TextureCoordinateGeneratorCheckButton);
	m_builder -> get_widget ("TextureCoordinateGeneratorUnlinkButton", m_TextureCoordinateGeneratorUnlinkButton);
	m_builder -> get_widget ("TextureCoordinateGeneratorBox", m_TextureCoordinateGeneratorBox);
	m_builder -> get_widget ("TextureCoordinateGeneratorModeComboBoxText", m_TextureCoordinateGeneratorModeComboBoxText);
	m_builder -> get_widget ("TextureCoordinateGeneratorNameBox", m_TextureCoordinateGeneratorNameBox);
	m_builder -> get_widget ("TextureCoordinateGeneratorNameEntry", m_TextureCoordinateGeneratorNameEntry);
	m_builder -> get_widget ("TextureCoordinateGeneratorRenameButton", m_TextureCoordinateGeneratorRenameButton);
	m_builder -> get_widget ("TextureCoordinateGeneratorParameter0SpinButton", m_TextureCoordinateGeneratorParameter0SpinButton);
	m_builder -> get_widget ("TextureCoordinateGeneratorParameter1SpinButton", m_TextureCoordinateGeneratorParameter1SpinButton);
	m_builder -> get_widget ("TextureCoordinateGeneratorParameter2SpinButton", m_TextureCoordinateGeneratorParameter2SpinButton);
	m_builder -> get_widget ("TextureCoordinateGeneratorParameter3SpinButton", m_TextureCoordinateGeneratorParameter3SpinButton);
	m_builder -> get_widget ("TextureCoordinateGeneratorParameter4SpinButton", m_TextureCoordinateGeneratorParameter4SpinButton);
	m_builder -> get_widget ("TextureCoordinateGeneratorParameter5SpinButton", m_TextureCoordinateGeneratorParameter5SpinButton);
	m_builder -> get_widget ("TextureCoordinateGeneratorAddParameterButton", m_TextureCoordinateGeneratorAddParameterButton);
	m_builder -> get_widget ("TextureCoordinateGeneratorRemoveParameterButton", m_TextureCoordinateGeneratorRemoveParameterButton);
	m_builder -> get_widget ("UsedTexturesBox", m_UsedTexturesBox);
	m_builder -> get_widget ("PaletteBox", m_PaletteBox);
	m_builder -> get_widget ("PalettePreviewBox", m_PalettePreviewBox);
	m_builder -> get_widget ("ChangePaletteBox", m_ChangePaletteBox);
	m_builder -> get_widget ("PaletteComboBoxText", m_PaletteComboBoxText);
	m_builder -> get_widget ("PalettePreviousButton", m_PalettePreviousButton);
	m_builder -> get_widget ("PaletteNextButton", m_PaletteNextButton);
	m_builder -> get_widget ("EditPaletteDialog", m_EditPaletteDialog);
	m_builder -> get_widget ("EditPaletteCancelButton", m_EditPaletteCancelButton);
	m_builder -> get_widget ("EditPaletteOkButton", m_EditPaletteOkButton);
	m_builder -> get_widget ("PaletteNameEntry", m_PaletteNameEntry);
	m_builder -> get_widget ("TextureTransform3DRotationPopover", m_TextureTransform3DRotationPopover);
	m_builder -> get_widget ("TextureTransform3DRotationAxisAngleButton", m_TextureTransform3DRotationAxisAngleButton);
	m_builder -> get_widget ("TextureTransform3DRotationEulerButton", m_TextureTransform3DRotationEulerButton);
	m_builder -> get_widget ("PaletteMenu", m_PaletteMenu);
	m_builder -> get_widget ("AddPaletteMenuItem", m_AddPaletteMenuItem);
	m_builder -> get_widget ("RemovePaletteMenuItem", m_RemovePaletteMenuItem);
	m_builder -> get_widget ("EditPaletteMenuItem", m_EditPaletteMenuItem);
	m_builder -> get_widget ("AddObjectToPaletteMenuItem", m_AddObjectToPaletteMenuItem);
	m_builder -> get_widget ("UpdateObjectInPaletteMenuItem", m_UpdateObjectInPaletteMenuItem);
	m_builder -> get_widget ("RemoveObjectFromPaletteMenuItem", m_RemoveObjectFromPaletteMenuItem);
	m_builder -> get_widget ("ShowDefaultPalettesMenuItem", m_ShowDefaultPalettesMenuItem);

	// Connect object Gtk::ComboBoxText with id 'TextureComboBoxText'.
	m_TextureComboBoxText -> signal_changed () .connect (sigc::mem_fun (this, &X3DTextureEditorInterface::on_texture_changed));

	// Connect object Gtk::Button with id 'TextureUnlinkButton'.
	m_TextureUnlinkButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DTextureEditorInterface::on_texture_unlink_clicked));
	m_ImageTextureEmbedButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DTextureEditorInterface::on_embed_image_clicked));
	m_PixelTextureOpenButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DTextureEditorInterface::on_pixel_texture_open_clicked));
	m_PixelTextureSaveAsButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DTextureEditorInterface::on_pixel_texture_save_as_clicked));
	m_MovieTexturePlayPauseButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DTextureEditorInterface::on_movie_texture_play_pause_clicked));
	m_MovieTextureStopButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DTextureEditorInterface::on_movie_texture_stop_clicked));

	// Connect object Gtk::CheckButton with id 'TexturePropertiesCheckButton'.
	m_TexturePropertiesCheckButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DTextureEditorInterface::on_textureProperties_toggled));

	// Connect object Gtk::Button with id 'TexturePropertiesUnlinkButton'.
	m_TexturePropertiesUnlinkButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DTextureEditorInterface::on_textureProperties_unlink_clicked));

	// Connect object Gtk::ComboBoxText with id 'TextureTransformComboBoxText'.
	m_TextureTransformComboBoxText -> signal_changed () .connect (sigc::mem_fun (this, &X3DTextureEditorInterface::on_textureTransform_changed));

	// Connect object Gtk::Button with id 'TextureTransformUnlinkButton'.
	m_TextureTransformUnlinkButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DTextureEditorInterface::on_textureTransform_unlink_clicked));

	// Connect object Gtk::ToggleButton with id 'TextureTransformUniformScaleButton'.
	m_TextureTransformUniformScaleButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DTextureEditorInterface::on_texture_transform_uniform_scale_clicked));

	// Connect object Gtk::Button with id 'TextureTransform3DRotationTypeButton'.
	m_TextureTransform3DRotationTypeButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DTextureEditorInterface::on_texture_transform_3d_rotation_type_clicked));

	// Connect object Gtk::ToggleButton with id 'TextureTransform3DUniformScaleButton'.
	m_TextureTransform3DUniformScaleButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DTextureEditorInterface::on_texture_transform_3d_uniform_scale_clicked));

	// Connect object Gtk::CheckButton with id 'TextureCoordinateGeneratorCheckButton'.
	m_TextureCoordinateGeneratorCheckButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DTextureEditorInterface::on_textureCoordinateGenerator_toggled));

	// Connect object Gtk::Button with id 'TextureCoordinateGeneratorUnlinkButton'.
	m_TextureCoordinateGeneratorUnlinkButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DTextureEditorInterface::on_textureCoordinateGenerator_unlink_clicked));
	m_TextureCoordinateGeneratorAddParameterButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DTextureEditorInterface::on_texture_coordinate_generator_add_parameter_clicked));
	m_TextureCoordinateGeneratorRemoveParameterButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DTextureEditorInterface::on_texture_coordinate_generator_remove_parameter_clicked));

	// Connect object Gtk::Box with id 'PalettePreviewBox'.
	m_PalettePreviewBox -> signal_button_press_event () .connect (sigc::mem_fun (this, &X3DTextureEditorInterface::on_palette_button_press_event));

	// Connect object Gtk::ComboBoxText with id 'PaletteComboBoxText'.
	m_PaletteComboBoxText -> signal_changed () .connect (sigc::mem_fun (this, &X3DTextureEditorInterface::on_palette_changed));

	// Connect object Gtk::Button with id 'PalettePreviousButton'.
	m_PalettePreviousButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DTextureEditorInterface::on_palette_previous_clicked));
	m_PaletteNextButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DTextureEditorInterface::on_palette_next_clicked));
	m_EditPaletteCancelButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DTextureEditorInterface::on_edit_palette_cancel_clicked));
	m_EditPaletteOkButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DTextureEditorInterface::on_edit_palette_ok_clicked));

	// Connect object Gtk::Entry with id 'PaletteNameEntry'.
	m_PaletteNameEntry -> signal_changed () .connect (sigc::mem_fun (this, &X3DTextureEditorInterface::on_palette_name_changed));
	m_PaletteNameEntry -> signal_delete_text () .connect (sigc::mem_fun (this, &X3DTextureEditorInterface::on_palette_name_delete_text), false);
	m_PaletteNameEntry -> signal_insert_text () .connect (sigc::mem_fun (this, &X3DTextureEditorInterface::on_palette_name_insert_text), false);

	// Connect object Gtk::RadioButton with id 'TextureTransform3DRotationAxisAngleButton'.
	m_TextureTransform3DRotationAxisAngleButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DTextureEditorInterface::on_texture_transform_3d_rotation_axis_angle_toggled));
	m_TextureTransform3DRotationEulerButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DTextureEditorInterface::on_texture_transform_3d_rotation_euler_toggled));

	// Connect object Gtk::ImageMenuItem with id 'AddPaletteMenuItem'.
	m_AddPaletteMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DTextureEditorInterface::on_add_palette_activate));
	m_RemovePaletteMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DTextureEditorInterface::on_remove_palette_activate));
	m_EditPaletteMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DTextureEditorInterface::on_edit_palette_activate));
	m_AddObjectToPaletteMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DTextureEditorInterface::on_add_object_to_palette_activate));
	m_UpdateObjectInPaletteMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DTextureEditorInterface::on_update_object_in_palette_activate));
	m_RemoveObjectFromPaletteMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DTextureEditorInterface::on_remove_object_from_palette_activate));

	// Connect object Gtk::CheckMenuItem with id 'ShowDefaultPalettesMenuItem'.
	m_ShowDefaultPalettesMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DTextureEditorInterface::on_show_default_palettes_toggled));
}

X3DTextureEditorInterface::~X3DTextureEditorInterface ()
{
	delete m_Window;
	delete m_EditPaletteDialog;
}

} // puck
} // titania
