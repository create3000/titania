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

#ifndef __TITANIA_EDITORS_GEOMETRY_PROPERTIES_EDITOR_GEOMETRY_PROPERTIES_EDITOR_H__
#define __TITANIA_EDITORS_GEOMETRY_PROPERTIES_EDITOR_GEOMETRY_PROPERTIES_EDITOR_H__

#include "../../UserInterfaces/X3DGeometryPropertiesEditorInterface.h"

#include "Geometry2D/X3DArc2DEditor.h"
#include "Geometry2D/X3DArcClose2DEditor.h"
#include "Geometry2D/X3DCircle2DEditor.h"
#include "Geometry2D/X3DDisk2DEditor.h"
#include "Geometry2D/X3DRectangle2DEditor.h"
#include "Geometry3D/X3DBoxEditor.h"
#include "Geometry3D/X3DConeEditor.h"
#include "Geometry3D/X3DCylinderEditor.h"
#include "Geometry3D/X3DElevationGridEditor.h"
#include "Geometry3D/X3DExtrusionEditor.h"
#include "Geometry3D/X3DSphereEditor.h"
#include "Geospatial/X3DGeoElevationGridEditor.h"
#include "NURBS/X3DNurbsCurveEditor.h"
#include "NURBS/X3DNurbsPatchSurfaceEditor.h"
#include "NURBS/X3DNurbsTrimmedSurfaceEditor.h"
#include "Rendering/X3DIndexedLineSetEditor.h"
#include "X3DColorNodeEditor.h"
#include "X3DPrimitiveCountEditor.h"
#include "X3DGeometryTool.h"

#include "../../ComposedWidgets.h"

namespace titania {
namespace puck {

class GeometryPropertiesEditor :
	virtual public X3DGeometryPropertiesEditorInterface,
	public X3DArc2DEditor,
	public X3DArcClose2DEditor,
	public X3DCircle2DEditor,
	public X3DDisk2DEditor,
	public X3DRectangle2DEditor,
	public X3DBoxEditor,
	public X3DConeEditor,
	public X3DCylinderEditor,
	public X3DElevationGridEditor,
	public X3DExtrusionEditor,
	public X3DSphereEditor,
	public X3DGeoElevationGridEditor,
	public X3DNurbsCurveEditor,
	public X3DNurbsPatchSurfaceEditor,
	public X3DNurbsTrimmedSurfaceEditor,
	public X3DIndexedLineSetEditor,
	public X3DColorNodeEditor,
	public X3DPrimitiveCountEditor,
	public X3DGeometryTool
{
public:

	///  @name Construction

	GeometryPropertiesEditor (X3DBrowserWindow* const browserWindow);

	///  @name Destruction

	virtual
	~GeometryPropertiesEditor () final override;


private:

	///  @name Construction

	virtual
	void
	configure () final override;

	virtual
	void
	initialize () final override;

	virtual
	void
	set_selection (const X3D::MFNode & selection) final override;

	virtual
	void
	on_geometry_changed () final override;

	virtual
	void
	on_geometry_unlink_clicked () final override;

	virtual
	void
	set_geometry () final override;

	void
	connectGeometry (const X3D::SFNode & field);

	void
	set_buffer ();

	void
	set_normal ();

	virtual
	void
	on_add_normals_clicked () final override;

	virtual
	void
	on_remove_normals_clicked () final override;

	virtual
	void
	on_color_per_vertex_toggled () final override;

	virtual
	void
	on_normal_per_vertex_toggled () final override;

	virtual
	void
	store () final override;

	///  @name Members

	X3DFieldToggleButton <X3D::SFBool>  solid;
	X3DFieldToggleButton <X3D::SFBool>  ccw;
	X3DFieldToggleButton <X3D::SFBool>  convex;
	X3DFieldAdjustment <X3D::SFFloat>   creaseAngle;
	X3DFieldAdjustment <X3D::SFDouble>  creaseAngleDouble;
	X3DFieldToggleButton <X3D::SFBool>  colorPerVertex;
	X3DFieldToggleButton <X3D::SFBool>  normalPerVertex;

	X3D::MFNode                          geometryNodes;
	X3D::SFNode                          geometryNode;
	X3D::X3DPtrArray <X3D::X3DShapeNode> shapeNodes;
	X3D::SFTime                          nodesBuffer;

	bool changing;

};

} // puck
} // titania

#endif
