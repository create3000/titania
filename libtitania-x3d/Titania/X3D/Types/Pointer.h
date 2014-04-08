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

#ifndef __TITANIA_X3D_TYPES_POINTER_H__
#define __TITANIA_X3D_TYPES_POINTER_H__

#include "../Fields/X3DPtr.h"

namespace titania {
namespace X3D {

// Forward declarations

//
class BrowserApplication;
class Browser;
class RenderingProperties;
class BrowserProperties;
class BrowserOptions;
class X3DJavaScriptEngine;
class Notification;
class Selection;
class Console;

//
class World;
class Scene;
class X3DExecutionContext;
class Proto;
class X3DProtoObject;
class X3DPrototypeInstance;
class ExternProto;
class NamedNode;
class ImportedNode;
class Route;
class ExportedNode;

//
class LayerSet;
class X3DLayerNode;
class NavigationInfo;
class X3DBackgroundNode;
class Fog;
class X3DViewpointNode;
class X3DViewportNode;
class X3DLayoutNode;

template <class Type>
class X3DBindableNodeStack;

template <class Type>
class X3DBindableNodeList;

using NavigationInfoStack = X3DBindableNodeStack <NavigationInfo>;
using BackgroundStack     = X3DBindableNodeStack <X3DBackgroundNode>;
using FogStack            = X3DBindableNodeStack <Fog>;
using ViewpointStack      = X3DBindableNodeStack <X3DViewpointNode>;

using NavigationInfoList = X3DBindableNodeList <NavigationInfo>;
using BackgroundList     = X3DBindableNodeList <X3DBackgroundNode>;
using FogList            = X3DBindableNodeList <Fog>;
using ViewpointList      = X3DBindableNodeList <X3DViewpointNode>;

//
class X3DNode;
class Group;
class Transform;
class X3DTransformNode;
class X3DGroupingNode;
class X3DTransformMatrix4DNode;
class Inline;

//
class Appearance;
class Material;
class TwoSidedMaterial;

// typedef's

using BrowserApplicationPtr  = X3DPtr <BrowserApplication>;
using BrowserPtr             = X3DPtr <Browser>;
using RenderingPropertiesPtr = X3DPtr <RenderingProperties>;
using BrowserPropertiesPtr   = X3DPtr <BrowserProperties>;
using BrowserOptionsPtr      = X3DPtr <BrowserOptions>;
using X3DJavaScriptEnginePtr = X3DPtr <X3DJavaScriptEngine>;
using SelectionPtr           = X3DPtr <Selection>;
using NotificationPtr        = X3DPtr <Notification>;
using ConsolePtr             = X3DPtr <Console>;

using WorldPtr                = X3DPtr <World>;
using ScenePtr                = X3DPtr <Scene>;
using X3DExecutionContextPtr  = X3DPtr <X3DExecutionContext>;
using ProtoPtr                = X3DPtr <Proto>;
using X3DProtoObjectPtr       = X3DPtr <X3DProtoObject>;
using X3DPrototypeInstancePtr = X3DPtr <X3DPrototypeInstance>;
using ExternProtoPtr          = X3DPtr <ExternProto>;
using NamedNodePtr            = X3DPtr <NamedNode>;
using ImportedNodePtr         = X3DPtr <ImportedNode>;
using RoutePtr                = X3DPtr <Route>;
using ExportedNodePtr         = X3DPtr <ExportedNode>;

using LayerSetPtr            = X3DPtr <LayerSet>;
using X3DLayerNodePtr        = X3DPtr <X3DLayerNode>;
using NavigationInfoStackPtr = X3DPtr <NavigationInfoStack>;
using BackgroundStackPtr     = X3DPtr <BackgroundStack>;
using FogStackPtr            = X3DPtr <FogStack>;
using ViewpointStackPtr      = X3DPtr <ViewpointStack>;
using NavigationInfoListPtr  = X3DPtr <NavigationInfoList>;
using BackgroundListPtr      = X3DPtr <BackgroundList>;
using FogListPtr             = X3DPtr <FogList>;
using ViewpointListPtr       = X3DPtr <ViewpointList>;
using NavigationInfoPtr      = X3DPtr <NavigationInfo>;
using X3DBackgroundNodePtr   = X3DPtr <X3DBackgroundNode>;
using FogPtr                 = X3DPtr <Fog>;
using X3DViewpointNodePtr    = X3DPtr <X3DViewpointNode>;
using X3DViewportNodePtr     = X3DPtr <X3DViewportNode>;
using X3DLayoutNodePtr       = X3DPtr <X3DLayoutNode>;

using X3DNodePtr                  = X3DPtr <X3DNode>;
using GroupPtr                    = X3DPtr <Group>;
using TransformPtr                = X3DPtr <Transform>;
using X3DTransformNodePtr         = X3DPtr <X3DTransformNode>;
using X3DTransformMatrix4DNodePtr = X3DPtr <X3DTransformMatrix4DNode>;
using X3DGroupingNodePtr          = X3DPtr <X3DGroupingNode>;
using InlinePtr                   = X3DPtr <Inline>;

using AppearancePtr       = X3DPtr <Appearance>;
using MaterialPtr         = X3DPtr <Material>;
using TwoSidedMaterialPtr = X3DPtr <TwoSidedMaterial>;

} // X3D
} // titania

#endif
