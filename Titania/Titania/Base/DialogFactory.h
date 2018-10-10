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

#ifndef __TITANIA_BASE_DIALOG_FACTORY_H__
#define __TITANIA_BASE_DIALOG_FACTORY_H__

#include "../Dialogs/FileOpenDialog/FileImportAsExternProtoDialog.h"
#include "../Dialogs/FileOpenDialog/FileOpenDialog.h"
#include "../Dialogs/FileOpenDialog/FileOpenFontDialog.h"
#include "../Dialogs/FileOpenDialog/OpenFolderDialog.h"
#include "../Dialogs/FileSaveDialog/FileExportDialog.h"
#include "../Dialogs/FileSaveDialog/FileExportImageDialog.h"
#include "../Dialogs/FileSaveDialog/FileExportProtoDialog.h"
#include "../Dialogs/FileSaveDialog/FileSaveDialog.h"
#include "../Dialogs/FileSaveDialog/FileSaveACopyDialog.h"
#include "../Dialogs/FileSaveDialog/FileSaveVideoDialog.h"
#include "../Dialogs/FileSaveWarningDialog/FileSaveWarningDialog.h"
#include "../Dialogs/FontChooserDialog/FontChooserDialog.h"
#include "../Dialogs/MessageDialog/MessageDialog.h"
#include "../Dialogs/OpenLocationDialog/OpenLocationDialog.h"

#include "../Editors/AppearanceEditor/AppearanceEditor.h"
#include "../Editors/BackgroundEditor/BackgroundEditor.h"
#include "../Editors/BackgroundImageEditor/BackgroundImageEditor.h"
#include "../Editors/BindableNodeEditor/BindableNodeEditor.h"
#include "../Editors/BindableNodeList/BackgroundList.h"
#include "../Editors/ColorEditor/ColorEditor.h"
#include "../Editors/BindableNodeList/FogList.h"
#include "../Editors/BindableNodeList/NavigationInfoList.h"
#include "../Editors/BindableNodeList/ViewpointList.h"
#include "../Editors/BrowserSizeEditor/BrowserSizeEditor.h"
#include "../Editors/ExternalToolsEditor/ExternalToolsEditor.h"
#include "../Editors/FilesEditor/FilesEditor.h"
#include "../Editors/FogEditor/FogEditor.h"
#include "../Editors/GeometryPropertiesEditor/GeometryPropertiesEditor.h"
#include "../Editors/GridEditor/GridEditor.h"
#include "../Editors/HistoryEditor/HistoryEditor.h"
#include "../Editors/InlineEditor/InlineEditor.h"
#include "../Editors/LayerEditor/LayerEditor.h"
#include "../Editors/LibraryView/LibraryView.h"
#include "../Editors/LightEditor/LightEditor.h"
#include "../Editors/ModelsPalette/ModelsPalette.h"
#include "../Editors/MotionBlurEditor/MotionBlurEditor.h"
#include "../Editors/MotionBlurEditor/MotionBlurEditor.h"
#include "../Editors/NavigationInfoEditor/NavigationInfoEditor.h"
#include "../Editors/NodeEditor/NodeEditor.h"
#include "../Editors/NodeIndex/NodeIndex.h"
#include "../Editors/NodePropertiesEditor/NodePropertiesEditor.h"
#include "../Editors/PrecisionPlacementPanel/PrecisionPlacementPanel.h"
#include "../Editors/ProjectsEditor/ProjectsEditor.h"
#include "../Editors/PrototypeEditor/PrototypeEditor.h"
#include "../Editors/ScenePropertiesEditor/ScenePropertiesEditor.h"
#include "../Editors/SculpToolEditor/SculpToolEditor.h"
#include "../Editors/TextEditor/TextEditor.h"
#include "../Editors/TextureEditor/TextureEditor.h"
#include "../Editors/TextureMappingEditor/TextureMappingEditor.h"
#include "../Editors/ViewpointEditor/ViewpointEditor.h"
#include "../Editors/SoundEditor/SoundEditor.h"
#include "../Editors/UndoHistoryEditor/UndoHistoryEditor.h"

#include "../Widgets/AnimationEditor/AnimationEditor.h"
#include "../Widgets/Console/Console.h"
#include "../Widgets/OutlineEditor/OutlineEditor.h"
#include "../Widgets/RouteGraph/RouteGraph.h"
#include "../Widgets/ScriptEditor/ScriptEditor.h"

namespace titania {
namespace puck {

class DialogFactory
{
public:

	///  @name Construction

	DialogFactory () :
		dialogs ({
		         std::make_pair ("AnimationEditor",               constructDialog <AnimationEditor>),
		         std::make_pair ("AppearanceEditor",              constructDialog <AppearanceEditor>),
		         std::make_pair ("BackgroundEditor",              constructDialog <BackgroundEditor>),
		         std::make_pair ("BackgroundImageEditor",         constructDialog <BackgroundImageEditor>),
		         std::make_pair ("BackgroundList",                constructDialog <BackgroundList>),
		         std::make_pair ("BindableNodeEditor",            constructDialog <BindableNodeEditor>),
		         std::make_pair ("BrowserSizeEditor",             constructDialog <BrowserSizeEditor>),
		         std::make_pair ("ColorEditor",                   constructDialog <ColorEditor>),
		         std::make_pair ("Console",                       constructDialog <Console>),
		         std::make_pair ("ExternalToolsEditor",           constructDialog <ExternalToolsEditor>),
		         std::make_pair ("FileExportDialog",              constructDialog <FileExportDialog>),
		         std::make_pair ("FileExportImageDialog",         constructDialog <FileExportImageDialog>),
		         std::make_pair ("FileExportProtoDialog",         constructDialog <FileExportProtoDialog>),
		         std::make_pair ("FileImportAsExternProtoDialog", constructDialog <FileImportAsExternProtoDialog>),
		         std::make_pair ("FileOpenDialog",                constructDialog <FileOpenDialog>),
		         std::make_pair ("FileOpenFontDialog",            constructDialog <FileOpenFontDialog>),
		         std::make_pair ("FileSaveACopyDialog",           constructDialog <FileSaveACopyDialog>),
		         std::make_pair ("FileSaveDialog",                constructDialog <FileSaveDialog>),
		         std::make_pair ("FileSaveVideoDialog",           constructDialog <FileSaveVideoDialog>),
		         std::make_pair ("FileSaveWarningDialog",         constructDialog <FileSaveWarningDialog>),
		         std::make_pair ("FontChooserDialog",             constructDialog <FontChooserDialog>),
		         std::make_pair ("FilesEditor",                   constructDialog <FilesEditor>),
		         std::make_pair ("FogEditor",                     constructDialog <FogEditor>),
		         std::make_pair ("FogList",                       constructDialog <FogList>),
		         std::make_pair ("GeometryPropertiesEditor",      constructDialog <GeometryPropertiesEditor>),
		         std::make_pair ("GridEditor",                    constructDialog <GridEditor>),
		         std::make_pair ("HistoryEditor",                 constructDialog <HistoryEditor>),
		         std::make_pair ("InlineEditor",                  constructDialog <InlineEditor>),
		         std::make_pair ("LayerEditor",                   constructDialog <LayerEditor>),
		         std::make_pair ("LibraryView",                   constructDialog <LibraryView>),
		         std::make_pair ("LightEditor",                   constructDialog <LightEditor>),
		         std::make_pair ("MessageDialog",                 constructDialog <MessageDialog>),
		         std::make_pair ("ModelsPalette",                 constructDialog <ModelsPalette>),
		         std::make_pair ("MotionBlurEditor",              constructDialog <MotionBlurEditor>),
		         std::make_pair ("NavigationInfoEditor",          constructDialog <NavigationInfoEditor>),
		         std::make_pair ("NavigationInfoList",            constructDialog <NavigationInfoList>),
		         std::make_pair ("NodeEditor",                    constructDialog <NodeEditor>),
		         std::make_pair ("NodeIndex",                     constructDialog <NodeIndex>),
		         std::make_pair ("NodePropertiesEditor",          constructDialog <NodePropertiesEditor>),
		         std::make_pair ("OpenFolderDialog",              constructDialog <OpenFolderDialog>),
		         std::make_pair ("OpenLocationDialog",            constructDialog <OpenLocationDialog>),
		         std::make_pair ("OutlineEditor",                 constructDialog <OutlineEditor>),
		         std::make_pair ("PrecisionPlacementPanel",       constructDialog <PrecisionPlacementPanel>),
		         std::make_pair ("ProjectsEditor",                constructDialog <ProjectsEditor>),
		         std::make_pair ("PrototypeEditor",               constructDialog <PrototypeEditor>),
		         std::make_pair ("RouteGraph",                    constructDialog <RouteGraph>),
		         std::make_pair ("ScenePropertiesEditor",         constructDialog <ScenePropertiesEditor>),
		         std::make_pair ("ScriptEditor",                  constructDialog <ScriptEditor>),
		         std::make_pair ("SculpToolEditor",               constructDialog <SculpToolEditor>),
		         std::make_pair ("SoundEditor",                   constructDialog <SoundEditor>),
		         std::make_pair ("TextEditor",                    constructDialog <TextEditor>),
		         std::make_pair ("TextureEditor",                 constructDialog <TextureEditor>),
		         std::make_pair ("TextureMappingEditor",          constructDialog <TextureMappingEditor>),
		         std::make_pair ("UndoHistoryEditor",             constructDialog <UndoHistoryEditor>),
		         std::make_pair ("ViewpointEditor",               constructDialog <ViewpointEditor>),
		         std::make_pair ("ViewpointList",                 constructDialog <ViewpointList>),
				         })
	{ }

	std::shared_ptr <X3DUserInterface>
	createDialog (const std::string & name, X3DBrowserWindow* const browserWindow) const
	{
		return std::shared_ptr <X3DUserInterface> (dialogs .at (name) (browserWindow));
	}

private:

	///  @name Construction

	template <class Widget>
	static
	X3DUserInterface*
	constructDialog (X3DBrowserWindow* const browserWindow)
	{ return new Widget (browserWindow); }

	///  @name Members

	std::map <std::string, std::function <X3DUserInterface* (X3DBrowserWindow* const browserWindow)>>  dialogs;

};

} // puck
} // titania

#endif
