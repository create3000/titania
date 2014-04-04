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

#include "Pointer.h"

// Include browser at first.
#include "../Browser/Browser.h"
#include "../Browser/BrowserApplication.h"

#include "../Browser/Notification.h"
#include "../Browser/Properties/BrowserOptions.h"
#include "../Browser/Properties/BrowserProperties.h"
#include "../Browser/Properties/RenderingProperties.h"
#include "../Browser/Picking/Selection.h"
#include "../Components/Core/X3DPrototypeInstance.h"
#include "../Components/EnvironmentalEffects/Fog.h"
#include "../Components/EnvironmentalEffects/X3DBackgroundNode.h"
#include "../Components/Grouping/Group.h"
#include "../Components/Grouping/Transform.h"
#include "../Components/Layering/LayerSet.h"
#include "../Components/Layering/X3DLayerNode.h"
#include "../Components/Layering/X3DViewportNode.h"
#include "../Components/Networking/Inline.h"
#include "../Components/Shape/Appearance.h"
#include "../Components/Shape/Material.h"
#include "../Components/Shape/TwoSidedMaterial.h"
#include "../Execution/BindableNodeList.h"
#include "../Execution/BindableNodeStack.h"
#include "../Execution/ExportedNode.h"
#include "../Execution/ImportedNode.h"
#include "../Execution/NamedNode.h"
#include "../Execution/Scene.h"
#include "../Execution/World.h"
#include "../JavaScript/X3DJavaScriptEngine.h"
#include "../Prototype/ExternProto.h"
#include "../Prototype/Proto.h"
#include "../Routing/Route.h"

namespace titania {
namespace X3D {

//
template class X3DPtr <BrowserApplication>;
template class X3DPtr <Browser>;
template class X3DPtr <RenderingProperties>;
template class X3DPtr <BrowserProperties>;
template class X3DPtr <BrowserOptions>;
template class X3DPtr <X3DJavaScriptEngine>;
template class X3DPtr <Selection>;
template class X3DPtr <Notification>;
template class X3DPtr <Console>;

//
template class X3DPtr <World>;
template class X3DPtr <Scene>;
template class X3DPtr <X3DExecutionContext>;
template class X3DPtr <Proto>;
template class X3DPtr <X3DProtoObject>;
template class X3DPtr <X3DPrototypeInstance>;
template class X3DPtr <ExternProto>;
template class X3DPtr <NamedNode>;
template class X3DPtr <ImportedNode>;
template class X3DPtr <Route>;
template class X3DPtr <ExportedNode>;

//
template class X3DPtr <LayerSet>;
template class X3DPtr <X3DLayerNode>;
template class X3DPtr <NavigationInfoStack>;
template class X3DPtr <BackgroundStack>;
template class X3DPtr <FogStack>;
template class X3DPtr <ViewpointStack>;
template class X3DPtr <NavigationInfoList>;
template class X3DPtr <BackgroundList>;
template class X3DPtr <FogList>;
template class X3DPtr <ViewpointList>;
template class X3DPtr <NavigationInfo>;
template class X3DPtr <X3DBackgroundNode>;
template class X3DPtr <Fog>;
template class X3DPtr <X3DViewpointNode>;
template class X3DPtr <X3DViewportNode>;
template class X3DPtr <X3DLayoutNode>;

//
template class X3DPtr <X3DNode>;
template class X3DPtr <Group>;
template class X3DPtr <Transform>;
template class X3DPtr <X3DTransformNode>;
template class X3DPtr <X3DTransformMatrix4DNode>;
template class X3DPtr <X3DGroupingNode>;
template class X3DPtr <Inline>;

//
template class X3DPtr <Appearance>;
template class X3DPtr <Material>;
template class X3DPtr <TwoSidedMaterial>;

} // X3D
} // titania
