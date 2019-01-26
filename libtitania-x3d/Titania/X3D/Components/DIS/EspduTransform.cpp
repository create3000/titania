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

#include "EspduTransform.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const Component   EspduTransform::component      = Component ("DIS", 1);
const std::string EspduTransform::typeName       = "EspduTransform";
const std::string EspduTransform::containerField = "children";

EspduTransform::Fields::Fields () :
	set_articulationParameterValue0 (new SFFloat ()),
	set_articulationParameterValue1 (new SFFloat ()),
	set_articulationParameterValue2 (new SFFloat ()),
	set_articulationParameterValue3 (new SFFloat ()),
	set_articulationParameterValue4 (new SFFloat ()),
	set_articulationParameterValue5 (new SFFloat ()),
	set_articulationParameterValue6 (new SFFloat ()),
	set_articulationParameterValue7 (new SFFloat ()),
	address (new SFString ("localhost")),
	applicationID (new SFInt32 (1)),
	articulationParameterCount (new SFInt32 ()),
	articulationParameterDesignatorArray (new MFInt32 ()),
	articulationParameterChangeIndicatorArray (new MFInt32 ()),
	articulationParameterIdPartAttachedToArray (new MFInt32 ()),
	articulationParameterTypeArray (new MFInt32 ()),
	articulationParameterArray (new MFFloat ()),
	center (new SFVec3f ()),
	collisionType (new SFInt32 ()),
	deadReckoning (new SFInt32 ()),
	detonationLocation (new SFVec3f ()),
	detonationRelativeLocation (new SFVec3f ()),
	detonationResult (new SFInt32 ()),
	entityCategory (new SFInt32 ()),
	entityCountry (new SFInt32 ()),
	entityDomain (new SFInt32 ()),
	entityExtra (new SFInt32 ()),
	entityID (new SFInt32 ()),
	entityKind (new SFInt32 ()),
	entitySpecific (new SFInt32 ()),
	entitySubCategory (new SFInt32 ()),
	eventApplicationID (new SFInt32 (1)),
	eventEntityID (new SFInt32 ()),
	eventNumber (new SFInt32 ()),
	eventSiteID (new SFInt32 ()),
	fired1 (new SFBool ()),
	fired2 (new SFBool ()),
	fireMissionIndex (new SFInt32 ()),
	firingRange (new SFFloat ()),
	firingRate (new SFInt32 ()),
	forceID (new SFInt32 ()),
	fuse (new SFInt32 ()),
	linearVelocity (new SFVec3f ()),
	linearAcceleration (new SFVec3f ()),
	marking (new SFString ()),
	multicastRelayHost (new SFString ()),
	multicastRelayPort (new SFInt32 ()),
	munitionApplicationID (new SFInt32 (1)),
	munitionEndPoint (new SFVec3f ()),
	munitionEntityID (new SFInt32 ()),
	munitionQuantity (new SFInt32 ()),
	munitionSiteID (new SFInt32 ()),
	munitionStartPoint (new SFVec3f ()),
	networkMode (new SFString ("standAlone")),
	port (new SFInt32 ()),
	readInterval (new SFTime (0.1)),
	rotation (new SFRotation ()),
	scale (new SFVec3f (1, 1, 1)),
	scaleOrientation (new SFRotation ()),
	siteID (new SFInt32 ()),
	translation (new SFVec3f ()),
	warhead (new SFInt32 ()),
	writeInterval (new SFTime (1)),
	articulationParameterValue0_changed (new SFFloat ()),
	articulationParameterValue1_changed (new SFFloat ()),
	articulationParameterValue2_changed (new SFFloat ()),
	articulationParameterValue3_changed (new SFFloat ()),
	articulationParameterValue4_changed (new SFFloat ()),
	articulationParameterValue5_changed (new SFFloat ()),
	articulationParameterValue6_changed (new SFFloat ()),
	articulationParameterValue7_changed (new SFFloat ()),
	collideTime (new SFTime ()),
	detonateTime (new SFTime ()),
	firedTime (new SFTime ()),
	isCollided (new SFBool ()),
	isDetonated (new SFBool ()),
	isNetworkReader (new SFBool ()),
	isNetworkWriter (new SFBool ()),
	isRtpHeaderHeard (new SFBool ()),
	isStandAlone (new SFBool ()),
	timestamp (new SFTime ()),
	rtpHeaderExpected (new SFBool ())
{ }

EspduTransform::EspduTransform (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DGroupingNode (),
	  X3DSensorNode (),
	         fields ()
{
	addType (X3DConstants::EspduTransform);

	addField (inputOutput,    "metadata",                                   metadata ());
	addField (inputOutput,    "enabled",                                    enabled ());
	addField (initializeOnly, "bboxSize",                                   bboxSize ());
	addField (initializeOnly, "bboxCenter",                                 bboxCenter ());
	addField (inputOnly,      "addChildren",                                addChildren ());
	addField (inputOnly,      "removeChildren",                             removeChildren ());
	addField (inputOutput,    "children",                                   children ());
	addField (outputOnly,     "isActive",                                   isActive ());
	addField (inputOnly,      "set_articulationParameterValue0",            set_articulationParameterValue0 ());
	addField (inputOnly,      "set_articulationParameterValue1",            set_articulationParameterValue1 ());
	addField (inputOnly,      "set_articulationParameterValue2",            set_articulationParameterValue2 ());
	addField (inputOnly,      "set_articulationParameterValue3",            set_articulationParameterValue3 ());
	addField (inputOnly,      "set_articulationParameterValue4",            set_articulationParameterValue4 ());
	addField (inputOnly,      "set_articulationParameterValue5",            set_articulationParameterValue5 ());
	addField (inputOnly,      "set_articulationParameterValue6",            set_articulationParameterValue6 ());
	addField (inputOnly,      "set_articulationParameterValue7",            set_articulationParameterValue7 ());
	addField (inputOutput,    "address",                                    address ());
	addField (inputOutput,    "applicationID",                              applicationID ());
	addField (inputOutput,    "articulationParameterCount",                 articulationParameterCount ());
	addField (inputOutput,    "articulationParameterDesignatorArray",       articulationParameterDesignatorArray ());
	addField (inputOutput,    "articulationParameterChangeIndicatorArray",  articulationParameterChangeIndicatorArray ());
	addField (inputOutput,    "articulationParameterIdPartAttachedToArray", articulationParameterIdPartAttachedToArray ());
	addField (inputOutput,    "articulationParameterTypeArray",             articulationParameterTypeArray ());
	addField (inputOutput,    "articulationParameterArray",                 articulationParameterArray ());
	addField (inputOutput,    "center",                                     center ());
	addField (inputOutput,    "collisionType",                              collisionType ());
	addField (inputOutput,    "deadReckoning",                              deadReckoning ());
	addField (inputOutput,    "detonationLocation",                         detonationLocation ());
	addField (inputOutput,    "detonationRelativeLocation",                 detonationRelativeLocation ());
	addField (inputOutput,    "detonationResult",                           detonationResult ());
	addField (inputOutput,    "entityCategory",                             entityCategory ());
	addField (inputOutput,    "entityCountry",                              entityCountry ());
	addField (inputOutput,    "entityDomain",                               entityDomain ());
	addField (inputOutput,    "entityExtra",                                entityExtra ());
	addField (inputOutput,    "entityID",                                   entityID ());
	addField (inputOutput,    "entityKind",                                 entityKind ());
	addField (inputOutput,    "entitySpecific",                             entitySpecific ());
	addField (inputOutput,    "entitySubCategory",                          entitySubCategory ());
	addField (inputOutput,    "eventApplicationID",                         eventApplicationID ());
	addField (inputOutput,    "eventEntityID",                              eventEntityID ());
	addField (inputOutput,    "eventNumber",                                eventNumber ());
	addField (inputOutput,    "eventSiteID",                                eventSiteID ());
	addField (inputOutput,    "fired1",                                     fired1 ());
	addField (inputOutput,    "fired2",                                     fired2 ());
	addField (inputOutput,    "fireMissionIndex",                           fireMissionIndex ());
	addField (inputOutput,    "firingRange",                                firingRange ());
	addField (inputOutput,    "firingRate",                                 firingRate ());
	addField (inputOutput,    "forceID",                                    forceID ());
	addField (inputOutput,    "fuse",                                       fuse ());
	addField (inputOutput,    "linearVelocity",                             linearVelocity ());
	addField (inputOutput,    "linearAcceleration",                         linearAcceleration ());
	addField (inputOutput,    "marking",                                    marking ());
	addField (inputOutput,    "multicastRelayHost",                         multicastRelayHost ());
	addField (inputOutput,    "multicastRelayPort",                         multicastRelayPort ());
	addField (inputOutput,    "munitionApplicationID",                      munitionApplicationID ());
	addField (inputOutput,    "munitionEndPoint",                           munitionEndPoint ());
	addField (inputOutput,    "munitionEntityID",                           munitionEntityID ());
	addField (inputOutput,    "munitionQuantity",                           munitionQuantity ());
	addField (inputOutput,    "munitionSiteID",                             munitionSiteID ());
	addField (inputOutput,    "munitionStartPoint",                         munitionStartPoint ());
	addField (inputOutput,    "networkMode",                                networkMode ());
	addField (inputOutput,    "port",                                       port ());
	addField (inputOutput,    "readInterval",                               readInterval ());
	addField (inputOutput,    "rotation",                                   rotation ());
	addField (inputOutput,    "scale",                                      scale ());
	addField (inputOutput,    "scaleOrientation",                           scaleOrientation ());
	addField (inputOutput,    "siteID",                                     siteID ());
	addField (inputOutput,    "translation",                                translation ());
	addField (inputOutput,    "warhead",                                    warhead ());
	addField (inputOutput,    "writeInterval",                              writeInterval ());
	addField (outputOnly,     "articulationParameterValue0_changed",        articulationParameterValue0_changed ());
	addField (outputOnly,     "articulationParameterValue1_changed",        articulationParameterValue1_changed ());
	addField (outputOnly,     "articulationParameterValue2_changed",        articulationParameterValue2_changed ());
	addField (outputOnly,     "articulationParameterValue3_changed",        articulationParameterValue3_changed ());
	addField (outputOnly,     "articulationParameterValue4_changed",        articulationParameterValue4_changed ());
	addField (outputOnly,     "articulationParameterValue5_changed",        articulationParameterValue5_changed ());
	addField (outputOnly,     "articulationParameterValue6_changed",        articulationParameterValue6_changed ());
	addField (outputOnly,     "articulationParameterValue7_changed",        articulationParameterValue7_changed ());
	addField (outputOnly,     "collideTime",                                collideTime ());
	addField (outputOnly,     "detonateTime",                               detonateTime ());
	addField (outputOnly,     "firedTime",                                  firedTime ());
	addField (outputOnly,     "isCollided",                                 isCollided ());
	addField (outputOnly,     "isDetonated",                                isDetonated ());
	addField (outputOnly,     "isNetworkReader",                            isNetworkReader ());
	addField (outputOnly,     "isNetworkWriter",                            isNetworkWriter ());
	addField (outputOnly,     "isRtpHeaderHeard",                           isRtpHeaderHeard ());
	addField (outputOnly,     "isStandAlone",                               isStandAlone ());
	addField (outputOnly,     "timestamp",                                  timestamp ());
	addField (initializeOnly, "rtpHeaderExpected",                          rtpHeaderExpected ());
}

X3DBaseNode*
EspduTransform::create (X3DExecutionContext* const executionContext) const
{
	return new EspduTransform (executionContext);
}

} // X3D
} // titania
