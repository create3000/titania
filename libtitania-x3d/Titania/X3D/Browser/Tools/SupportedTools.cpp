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

#include "SupportedTools.h"

#include "../../Tools/CADGeometry/CADAssemblyTool.h"
#include "../../Tools/CADGeometry/CADFaceTool.h"
#include "../../Tools/CADGeometry/CADLayerTool.h"
#include "../../Tools/CADGeometry/CADPartTool.h"
#include "../../Tools/CADGeometry/IndexedQuadSetTool.h"
#include "../../Tools/CADGeometry/QuadSetTool.h"

#include "../../Tools/EnvironmentalSensor/ProximitySensorTool.h"
#include "../../Tools/EnvironmentalSensor/TransformSensorTool.h"
#include "../../Tools/EnvironmentalSensor/VisibilitySensorTool.h"

#include "../../Tools/Geometry2D/Arc2DTool.h"
#include "../../Tools/Geometry2D/ArcClose2DTool.h"
#include "../../Tools/Geometry2D/Circle2DTool.h"
#include "../../Tools/Geometry2D/Disk2DTool.h"
#include "../../Tools/Geometry2D/Polyline2DTool.h"
#include "../../Tools/Geometry2D/Polypoint2DTool.h"
#include "../../Tools/Geometry2D/Rectangle2DTool.h"
#include "../../Tools/Geometry2D/TriangleSet2DTool.h"

#include "../../Tools/Geometry3D/Box/BoxTool.h"
#include "../../Tools/Geometry3D/Cone/ConeTool.h"
#include "../../Tools/Geometry3D/Cylinder/CylinderTool.h"
#include "../../Tools/Geometry3D/ElevationGrid/ElevationGridTool.h"
#include "../../Tools/Geometry3D/Extrusion/ExtrusionTool.h"
#include "../../Tools/Geometry3D/IndexedFaceSet/IndexedFaceSetTool.h"
#include "../../Tools/Geometry3D/Sphere/SphereTool.h"

#include "../../Tools/Geospatial/GeoElevationGridTool.h"
#include "../../Tools/Geospatial/GeoLODTool.h"
#include "../../Tools/Geospatial/GeoLocationTool.h"
#include "../../Tools/Geospatial/GeoTransformTool.h"
#include "../../Tools/Geospatial/GeoViewpointTool.h"

#include "../../Tools/Grouping/GroupTool.h"
#include "../../Tools/Grouping/SwitchTool.h"
#include "../../Tools/Grouping/TransformTool.h"

#include "../../Tools/Layout/LayoutGroupTool.h"
#include "../../Tools/Layout/ScreenGroupTool.h"

#include "../../Tools/Lighting/DirectionalLightTool.h"
#include "../../Tools/Lighting/PointLightTool.h"
#include "../../Tools/Lighting/SpotLightTool.h"

#include "../../Tools/NURBS/NurbsCurveTool.h"
#include "../../Tools/NURBS/NurbsPatchSurfaceTool.h"
#include "../../Tools/NURBS/NurbsSweptSurfaceTool.h"
#include "../../Tools/NURBS/NurbsSwungSurfaceTool.h"
#include "../../Tools/NURBS/NurbsTrimmedSurfaceTool.h"

#include "../../Tools/Navigation/BillboardTool.h"
#include "../../Tools/Navigation/CollisionTool.h"
#include "../../Tools/Navigation/LODTool.h"
#include "../../Tools/Navigation/OrthoViewpointTool.h"
#include "../../Tools/Navigation/ViewpointTool.h"
#include "../../Tools/Networking/AnchorTool.h"
#include "../../Tools/Networking/InlineTool.h"

#include "../../Tools/ParticleSystems/ParticleSystemTool.h"

#include "../../Tools/Picking/PickableGroupTool.h"

#include "../../Tools/Rendering/IndexedLineSetTool.h"
#include "../../Tools/Rendering/IndexedTriangleFanSetTool.h"
#include "../../Tools/Rendering/IndexedTriangleSetTool.h"
#include "../../Tools/Rendering/IndexedTriangleStripSetTool.h"
#include "../../Tools/Rendering/LineSetTool.h"
#include "../../Tools/Rendering/PointSetTool.h"
#include "../../Tools/Rendering/TriangleFanSetTool.h"
#include "../../Tools/Rendering/TriangleSetTool.h"
#include "../../Tools/Rendering/TriangleStripSetTool.h"

#include "../../Tools/RigidBodyPhysics/CollidableOffsetTool.h"
#include "../../Tools/RigidBodyPhysics/CollidableShapeTool.h"

#include "../../Tools/Shape/ShapeTool.h"

#include "../../Tools/Sound/SoundTool.h"

#include "../../Tools/Text/TextTool.h"

#include "../../Tools/X_ITE/TouchGroupTool.h"

namespace titania {
namespace X3D {

SupportedTools::SupportedTools () :
	functions ()
{
	addTool ("Anchor",                  [ ] (X3DBaseNode* const node) { return new AnchorTool                  (node); });
	addTool ("Arc2D",                   [ ] (X3DBaseNode* const node) { return new Arc2DTool                   (node); });
	addTool ("ArcClose2D",              [ ] (X3DBaseNode* const node) { return new ArcClose2DTool              (node); });
	addTool ("Billboard",               [ ] (X3DBaseNode* const node) { return new BillboardTool               (node); });
	addTool ("Box",                     [ ] (X3DBaseNode* const node) { return new BoxTool                     (node); });
	addTool ("CADAssembly",             [ ] (X3DBaseNode* const node) { return new CADAssemblyTool             (node); });
	addTool ("CADFace",                 [ ] (X3DBaseNode* const node) { return new CADFaceTool                 (node); });
	addTool ("CADLayer",                [ ] (X3DBaseNode* const node) { return new CADLayerTool                (node); });
	addTool ("CADPart",                 [ ] (X3DBaseNode* const node) { return new CADPartTool                 (node); });
	addTool ("Circle2D",                [ ] (X3DBaseNode* const node) { return new Circle2DTool                (node); });
	addTool ("CollidableOffset",        [ ] (X3DBaseNode* const node) { return new CollidableOffsetTool        (node); });
	addTool ("CollidableShape",         [ ] (X3DBaseNode* const node) { return new CollidableShapeTool         (node); });
	addTool ("Collision",               [ ] (X3DBaseNode* const node) { return new CollisionTool               (node); });
	addTool ("Cone",                    [ ] (X3DBaseNode* const node) { return new ConeTool                    (node); });
	addTool ("Cylinder",                [ ] (X3DBaseNode* const node) { return new CylinderTool                (node); });
	addTool ("DirectionalLight",        [ ] (X3DBaseNode* const node) { return new DirectionalLightTool        (node); });
	addTool ("Disk2D",                  [ ] (X3DBaseNode* const node) { return new Disk2DTool                  (node); });
	addTool ("ElevationGrid",           [ ] (X3DBaseNode* const node) { return new ElevationGridTool           (node); });
	addTool ("Extrusion",               [ ] (X3DBaseNode* const node) { return new ExtrusionTool               (node); });
	addTool ("GeoElevationGrid",        [ ] (X3DBaseNode* const node) { return new GeoElevationGridTool        (node); });
	addTool ("GeoLOD",                  [ ] (X3DBaseNode* const node) { return new GeoLODTool                  (node); });
	addTool ("GeoLocation",             [ ] (X3DBaseNode* const node) { return new GeoLocationTool             (node); });
	addTool ("GeoTransform",            [ ] (X3DBaseNode* const node) { return new GeoTransformTool            (node); });
	addTool ("GeoViewpoint",            [ ] (X3DBaseNode* const node) { return new GeoViewpointTool            (node); });
	addTool ("Group",                   [ ] (X3DBaseNode* const node) { return new GroupTool                   (node); });
	addTool ("IndexedFaceSet",          [ ] (X3DBaseNode* const node) { return new IndexedFaceSetTool          (node); });
	addTool ("IndexedLineSet",          [ ] (X3DBaseNode* const node) { return new IndexedLineSetTool          (node); });
	addTool ("IndexedQuadSet",          [ ] (X3DBaseNode* const node) { return new IndexedQuadSetTool          (node); });
	addTool ("IndexedTriangleFanSet",   [ ] (X3DBaseNode* const node) { return new IndexedTriangleFanSetTool   (node); });
	addTool ("IndexedTriangleSet",      [ ] (X3DBaseNode* const node) { return new IndexedTriangleSetTool      (node); });
	addTool ("IndexedTriangleStripSet", [ ] (X3DBaseNode* const node) { return new IndexedTriangleStripSetTool (node); });
	addTool ("Inline",                  [ ] (X3DBaseNode* const node) { return new InlineTool                  (node); });
	addTool ("LOD",                     [ ] (X3DBaseNode* const node) { return new LODTool                     (node); });
	addTool ("LayoutGroup",             [ ] (X3DBaseNode* const node) { return new LayoutGroupTool             (node); });
	addTool ("LineSet",                 [ ] (X3DBaseNode* const node) { return new LineSetTool                 (node); });
	addTool ("NurbsCurve",              [ ] (X3DBaseNode* const node) { return new NurbsCurveTool              (node); });
	addTool ("NurbsPatchSurface",       [ ] (X3DBaseNode* const node) { return new NurbsPatchSurfaceTool       (node); });
	addTool ("NurbsSweptSurface",       [ ] (X3DBaseNode* const node) { return new NurbsSweptSurfaceTool       (node); });
	addTool ("NurbsSwungSurface",       [ ] (X3DBaseNode* const node) { return new NurbsSwungSurfaceTool       (node); });
	addTool ("NurbsTrimmedSurface",     [ ] (X3DBaseNode* const node) { return new NurbsTrimmedSurfaceTool     (node); });
	addTool ("OrthoViewpoint",          [ ] (X3DBaseNode* const node) { return new OrthoViewpointTool          (node); });
	addTool ("ParticleSystem",          [ ] (X3DBaseNode* const node) { return new ParticleSystemTool          (node); });
	addTool ("PickableGroup",           [ ] (X3DBaseNode* const node) { return new PickableGroupTool           (node); });
	addTool ("PointLight",              [ ] (X3DBaseNode* const node) { return new PointLightTool              (node); });
	addTool ("PointSet",                [ ] (X3DBaseNode* const node) { return new PointSetTool                (node); });
	addTool ("Polyline2D",              [ ] (X3DBaseNode* const node) { return new Polyline2DTool              (node); });
	addTool ("Polypoint2D",             [ ] (X3DBaseNode* const node) { return new Polypoint2DTool             (node); });
	addTool ("ProximitySensor",         [ ] (X3DBaseNode* const node) { return new ProximitySensorTool         (node); });
	addTool ("QuadSet",                 [ ] (X3DBaseNode* const node) { return new QuadSetTool                 (node); });
	addTool ("Rectangle2D",             [ ] (X3DBaseNode* const node) { return new Rectangle2DTool             (node); });
	addTool ("ScreenGroup",             [ ] (X3DBaseNode* const node) { return new ScreenGroupTool             (node); });
	addTool ("Shape",                   [ ] (X3DBaseNode* const node) { return new ShapeTool                   (node); });
	addTool ("Sound",                   [ ] (X3DBaseNode* const node) { return new SoundTool                   (node); });
	addTool ("Sphere",                  [ ] (X3DBaseNode* const node) { return new SphereTool                  (node); });
	addTool ("SpotLight",               [ ] (X3DBaseNode* const node) { return new SpotLightTool               (node); });
	addTool ("Switch",                  [ ] (X3DBaseNode* const node) { return new SwitchTool                  (node); });
	addTool ("Text",                    [ ] (X3DBaseNode* const node) { return new TextTool                    (node); });
	addTool ("TransformSensor",         [ ] (X3DBaseNode* const node) { return new TransformSensorTool         (node); });
	addTool ("Transform",               [ ] (X3DBaseNode* const node) { return new TransformTool               (node); });
	addTool ("TriangleFanSet",          [ ] (X3DBaseNode* const node) { return new TriangleFanSetTool          (node); });
	addTool ("TriangleSet2D",           [ ] (X3DBaseNode* const node) { return new TriangleSet2DTool           (node); });
	addTool ("TriangleSet",             [ ] (X3DBaseNode* const node) { return new TriangleSetTool             (node); });
	addTool ("TriangleStripSet",        [ ] (X3DBaseNode* const node) { return new TriangleStripSetTool        (node); });
	addTool ("Viewpoint",               [ ] (X3DBaseNode* const node) { return new ViewpointTool               (node); });
	addTool ("VisibilitySensor",        [ ] (X3DBaseNode* const node) { return new VisibilitySensorTool        (node); });

	// Titania
	addTool ("TouchGroup",              [ ] (X3DBaseNode* const node) { return new TouchGroupTool              (node); });
}

///  throws Error <INVALID_NAME>
void
SupportedTools::addTool (const std::string & typeName, const Function & function)
{
	if (not functions .emplace (typeName, function) .second)
		throw Error <INVALID_NAME> ("Tool type '" + typeName + "' already exists.");
}

///  throws Error <INVALID_NAME>
const SupportedTools::Function &
SupportedTools::getTool (const std::string & typeName) const
{
	try
	{
		return functions .at (typeName);
	}
	catch (const std::out_of_range &)
	{
		throw Error <INVALID_NAME> ("Unknown tool type '" + typeName + "'.");
	}
}

SupportedTools::~SupportedTools ()
{ }

} // X3D
} // titania
