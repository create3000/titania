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
#include "../Editors/PolygonColoringEditor/PolygonColoringEditor.h"
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
		         std::pair ("AnimationEditor",               constructDialog <AnimationEditor>),
		         std::pair ("AppearanceEditor",              constructDialog <AppearanceEditor>),
		         std::pair ("BackgroundEditor",              constructDialog <BackgroundEditor>),
		         std::pair ("BackgroundImageEditor",         constructDialog <BackgroundImageEditor>),
		         std::pair ("BackgroundList",                constructDialog <BackgroundList>),
		         std::pair ("BindableNodeEditor",            constructDialog <BindableNodeEditor>),
		         std::pair ("BrowserSizeEditor",             constructDialog <BrowserSizeEditor>),
		         std::pair ("Console",                       constructDialog <Console>),
		         std::pair ("ExternalToolsEditor",           constructDialog <ExternalToolsEditor>),
		         std::pair ("FileExportDialog",              constructDialog <FileExportDialog>),
		         std::pair ("FileExportImageDialog",         constructDialog <FileExportImageDialog>),
		         std::pair ("FileExportProtoDialog",         constructDialog <FileExportProtoDialog>),
		         std::pair ("FileImportAsExternProtoDialog", constructDialog <FileImportAsExternProtoDialog>),
		         std::pair ("FileOpenDialog",                constructDialog <FileOpenDialog>),
		         std::pair ("FileOpenFontDialog",            constructDialog <FileOpenFontDialog>),
		         std::pair ("FileSaveACopyDialog",           constructDialog <FileSaveACopyDialog>),
		         std::pair ("FileSaveDialog",                constructDialog <FileSaveDialog>),
		         std::pair ("FileSaveVideoDialog",           constructDialog <FileSaveVideoDialog>),
		         std::pair ("FileSaveWarningDialog",         constructDialog <FileSaveWarningDialog>),
		         std::pair ("FontChooserDialog",             constructDialog <FontChooserDialog>),
		         std::pair ("FilesEditor",                   constructDialog <FilesEditor>),
		         std::pair ("FogEditor",                     constructDialog <FogEditor>),
		         std::pair ("FogList",                       constructDialog <FogList>),
		         std::pair ("GeometryPropertiesEditor",      constructDialog <GeometryPropertiesEditor>),
		         std::pair ("GridEditor",                    constructDialog <GridEditor>),
		         std::pair ("HistoryEditor",                 constructDialog <HistoryEditor>),
		         std::pair ("InlineEditor",                  constructDialog <InlineEditor>),
		         std::pair ("LayerEditor",                   constructDialog <LayerEditor>),
		         std::pair ("LibraryView",                   constructDialog <LibraryView>),
		         std::pair ("LightEditor",                   constructDialog <LightEditor>),
		         std::pair ("MessageDialog",                 constructDialog <MessageDialog>),
		         std::pair ("ModelsPalette",                 constructDialog <ModelsPalette>),
		         std::pair ("MotionBlurEditor",              constructDialog <MotionBlurEditor>),
		         std::pair ("NavigationInfoEditor",          constructDialog <NavigationInfoEditor>),
		         std::pair ("NavigationInfoList",            constructDialog <NavigationInfoList>),
		         std::pair ("NodeEditor",                    constructDialog <NodeEditor>),
		         std::pair ("NodeIndex",                     constructDialog <NodeIndex>),
		         std::pair ("NodePropertiesEditor",          constructDialog <NodePropertiesEditor>),
		         std::pair ("OpenFolderDialog",              constructDialog <OpenFolderDialog>),
		         std::pair ("OpenLocationDialog",            constructDialog <OpenLocationDialog>),
		         std::pair ("OutlineEditor",                 constructDialog <OutlineEditor>),
		         std::pair ("PolygonColoringEditor",         constructDialog <PolygonColoringEditor>),
		         std::pair ("PrecisionPlacementPanel",       constructDialog <PrecisionPlacementPanel>),
		         std::pair ("ProjectsEditor",                constructDialog <ProjectsEditor>),
		         std::pair ("PrototypeEditor",               constructDialog <PrototypeEditor>),
		         std::pair ("RouteGraph",                    constructDialog <RouteGraph>),
		         std::pair ("ScenePropertiesEditor",         constructDialog <ScenePropertiesEditor>),
		         std::pair ("ScriptEditor",                  constructDialog <ScriptEditor>),
		         std::pair ("SculpToolEditor",               constructDialog <SculpToolEditor>),
		         std::pair ("SoundEditor",                   constructDialog <SoundEditor>),
		         std::pair ("TextEditor",                    constructDialog <TextEditor>),
		         std::pair ("TextureEditor",                 constructDialog <TextureEditor>),
		         std::pair ("TextureMappingEditor",          constructDialog <TextureMappingEditor>),
		         std::pair ("UndoHistoryEditor",             constructDialog <UndoHistoryEditor>),
		         std::pair ("ViewpointEditor",               constructDialog <ViewpointEditor>),
		         std::pair ("ViewpointList",                 constructDialog <ViewpointList>),
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
