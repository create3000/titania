/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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
 ******************************************************************************/

#ifndef __TITANIA_X3D_COMPONENTS_DIS_ESPDU_TRANSFORM_H__
#define __TITANIA_X3D_COMPONENTS_DIS_ESPDU_TRANSFORM_H__

#include "../Core/X3DSensorNode.h"
#include "../Grouping/X3DGroupingNode.h"

namespace titania {
namespace X3D {

class EspduTransform :
	public X3DGroupingNode, public X3DSensorNode
{
public:

	SFFloat    set_articulationParameterValue0;
	SFFloat    set_articulationParameterValue1;
	SFFloat    set_articulationParameterValue2;
	SFFloat    set_articulationParameterValue3;
	SFFloat    set_articulationParameterValue4;
	SFFloat    set_articulationParameterValue5;
	SFFloat    set_articulationParameterValue6;
	SFFloat    set_articulationParameterValue7;
	SFString   address;
	SFInt32    applicationID;
	SFInt32    articulationParameterCount;
	MFInt32    articulationParameterDesignatorArray;
	MFInt32    articulationParameterChangeIndicatorArray;
	MFInt32    articulationParameterIdPartAttachedToArray;
	MFInt32    articulationParameterTypeArray;
	MFFloat    articulationParameterArray;
	SFVec3f    center;
	SFInt32    collisionType;
	SFInt32    deadReckoning;
	SFVec3f    detonationLocation;
	SFVec3f    detonationRelativeLocation;
	SFInt32    detonationResult;
	SFInt32    entityCategory;
	SFInt32    entityCountry;
	SFInt32    entityDomain;
	SFInt32    entityExtra;
	SFInt32    entityID;
	SFInt32    entityKind;
	SFInt32    entitySpecific;
	SFInt32    entitySubCategory;
	SFInt32    eventApplicationID;
	SFInt32    eventEntityID;
	SFInt32    eventNumber;
	SFInt32    eventSiteID;
	SFBool     fired1;
	SFBool     fired2;
	SFInt32    fireMissionIndex;
	SFFloat    firingRange;
	SFInt32    firingRate;
	SFInt32    forceID;
	SFInt32    fuse;
	SFVec3f    linearVelocity;
	SFVec3f    linearAcceleration;
	SFString   marking;
	SFString   multicastRelayHost;
	SFInt32    multicastRelayPort;
	SFInt32    munitionApplicationID;
	SFVec3f    munitionEndPoint;
	SFInt32    munitionEntityID;
	SFInt32    munitionQuantity;
	SFInt32    munitionSiteID;
	SFVec3f    munitionStartPoint;
	SFString   networkMode;
	SFInt32    port;
	SFTime     readInterval;
	SFRotation rotation;
	SFVec3f    scale;
	SFRotation scaleOrientation;
	SFInt32    siteID;
	SFVec3f    translation;
	SFInt32    warhead;
	SFTime     writeInterval;
	SFFloat    articulationParameterValue0_changed;
	SFFloat    articulationParameterValue1_changed;
	SFFloat    articulationParameterValue2_changed;
	SFFloat    articulationParameterValue3_changed;
	SFFloat    articulationParameterValue4_changed;
	SFFloat    articulationParameterValue5_changed;
	SFFloat    articulationParameterValue6_changed;
	SFFloat    articulationParameterValue7_changed;
	SFTime     collideTime;
	SFTime     detonateTime;
	SFTime     firedTime;
	SFBool     isCollided;
	SFBool     isDetonated;
	SFBool     isNetworkReader;
	SFBool     isNetworkWriter;
	SFBool     isRtpHeaderHeard;
	SFBool     isStandAlone;
	SFTime     timestamp;
	SFBool     rtpHeaderExpected;

	EspduTransform (X3DExecutionContext* const);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const) const;

	virtual
	void
	update ();

};

} // X3D
} // titania

#endif
