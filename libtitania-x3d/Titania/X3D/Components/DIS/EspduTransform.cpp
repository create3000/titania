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

#include "EspduTransform.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

EspduTransform::EspduTransform (X3DExecutionContext* const executionContext) :
	                               X3DBaseNode (executionContext -> getBrowser (), executionContext), 
	                           X3DGroupingNode (),                                                    
	                             X3DSensorNode (),                                                    
	           set_articulationParameterValue0 (),                                                    // SFFloat    [in]     set_articulationParameterValue0                                 (-∞,∞)
	           set_articulationParameterValue1 (),                                                    // SFFloat    [in]     set_articulationParameterValue1                                 (-∞,∞)
	           set_articulationParameterValue2 (),                                                    // SFFloat    [in]     set_articulationParameterValue2                                 (-∞,∞)
	           set_articulationParameterValue3 (),                                                    // SFFloat    [in]     set_articulationParameterValue3                                 (-∞,∞)
	           set_articulationParameterValue4 (),                                                    // SFFloat    [in]     set_articulationParameterValue4                                 (-∞,∞)
	           set_articulationParameterValue5 (),                                                    // SFFloat    [in]     set_articulationParameterValue5                                 (-∞,∞)
	           set_articulationParameterValue6 (),                                                    // SFFloat    [in]     set_articulationParameterValue6                                 (-∞,∞)
	           set_articulationParameterValue7 (),                                                    // SFFloat    [in]     set_articulationParameterValue7                                 (-∞,∞)
	                                   address ("localhost"),                                         // SFString   [in,out] address                                     "localhost"
	                             applicationID (1),                                                   // SFInt32    [in,out] applicationID                               1                   [0,65535]
	                articulationParameterCount (),                                                    // SFInt32    [in,out] articulationParameterCount                  0                   [0,78]
	      articulationParameterDesignatorArray (),                                                    // MFInt32    [in,out] articulationParameterDesignatorArray        [ ]                 [0,255]
	 articulationParameterChangeIndicatorArray (),                                                    // MFInt32    [in,out] articulationParameterChangeIndicatorArray   [ ]                 [0,255]
	articulationParameterIdPartAttachedToArray (),                                                    // MFInt32    [in,out] articulationParameterIdPartAttachedToArray  [ ]                 [0,65535]
	            articulationParameterTypeArray (),                                                    // MFInt32    [in,out] articulationParameterTypeArray              [ ]                 [0,2147483647]
	                articulationParameterArray (),                                                    // MFFloat    [in,out] articulationParameterArray                  [ ]                 (-∞,∞)
	                                    center (),                                                    // SFVec3f    [in,out] center                                      0 0 0               (-∞,∞)
	                             collisionType (),                                                    // SFInt32    [in,out] collisionType                               0                   [0,255]
	                             deadReckoning (),                                                    // SFInt32    [in,out] deadReckoning                               0                   [0,255]
	                        detonationLocation (),                                                    // SFVec3f    [in,out] detonationLocation                          0 0 0               (-∞,∞)
	                detonationRelativeLocation (),                                                    // SFVec3f    [in,out] detonationRelativeLocation                  0 0 0               (-∞,∞)
	                          detonationResult (),                                                    // SFInt32    [in,out] detonationResult                            0                   [0,255]
	                            entityCategory (),                                                    // SFInt32    [in,out] entityCategory                              0                   [0,255]
	                             entityCountry (),                                                    // SFInt32    [in,out] entityCountry                               0                   [0,65535]
	                              entityDomain (),                                                    // SFInt32    [in,out] entityDomain                                0                   [0,255]
	                               entityExtra (),                                                    // SFInt32    [in,out] entityExtra                                 0                   [0,255]
	                                  entityID (),                                                    // SFInt32    [in,out] entityID                                    0                   [0,65535]
	                                entityKind (),                                                    // SFInt32    [in,out] entityKind                                  0                   [0,255]
	                            entitySpecific (),                                                    // SFInt32    [in,out] entitySpecific                              0                   [0,255]
	                         entitySubCategory (),                                                    // SFInt32    [in,out] entitySubCategory                           0                   [0,255]
	                        eventApplicationID (1),                                                   // SFInt32    [in,out] eventApplicationID                          1                   [0,65535]
	                             eventEntityID (),                                                    // SFInt32    [in,out] eventEntityID                               0                   [0,65535]
	                               eventNumber (),                                                    // SFInt32    [in,out] eventNumber                                 0                   [0,65355]
	                               eventSiteID (),                                                    // SFInt32    [in,out] eventSiteID                                 0                   [0,65535]
	                                    fired1 (),                                                    // SFBool     [in,out] fired1                                      FALSE
	                                    fired2 (),                                                    // SFBool     [in,out] fired2                                      FALSE
	                          fireMissionIndex (),                                                    // SFInt32    [in,out] fireMissionIndex                            0                   [0,65535]
	                               firingRange (),                                                    // SFFloat    [in,out] firingRange                                 0.0                 (0,∞)
	                                firingRate (),                                                    // SFInt32    [in,out] firingRate                                  0                   [0,65535]
	                                   forceID (),                                                    // SFInt32    [in,out] forceID                                     0                   [0,255]
	                                      fuse (),                                                    // SFInt32    [in,out] fuse                                        0                   [0,65535]
	                            linearVelocity (),                                                    // SFVec3f    [in,out] linearVelocity                              0 0 0               (-∞,∞)
	                        linearAcceleration (),                                                    // SFVec3f    [in,out] linearAcceleration                          0 0 0               (-∞,∞)
	                                   marking (),                                                    // SFString   [in,out] marking                                     ""
	                        multicastRelayHost (),                                                    // SFString   [in,out] multicastRelayHost                          ""
	                        multicastRelayPort (),                                                    // SFInt32    [in,out] multicastRelayPort                          0                   [0,4294967295]
	                     munitionApplicationID (1),                                                   // SFInt32    [in,out] munitionApplicationID                       1                   [0,65535]
	                          munitionEndPoint (),                                                    // SFVec3f    [in,out] munitionEndPoint                            0 0 0               (-∞,∞)
	                          munitionEntityID (),                                                    // SFInt32    [in,out] munitionEntityID                            0                   [0,65535]
	                          munitionQuantity (),                                                    // SFInt32    [in,out] munitionQuantity                            0                   [0,65535]
	                            munitionSiteID (),                                                    // SFInt32    [in,out] munitionSiteID                              0                   [0,65535]
	                        munitionStartPoint (),                                                    // SFVec3f    [in,out] munitionStartPoint                          0 0 0               (-∞,∞)
	                               networkMode ("standAlone"),                                        // SFString   [in,out] networkMode                                 "standAlone"        ["standAlone"|"networkReader"|"networkWriter"]
	                                      port (),                                                    // SFInt32    [in,out] port                                        0                   [0,65535]
	                              readInterval (0.1),                                                 // SFTime     [in,out] readInterval                                0.1                 [0,∞)
	                                  rotation (),                                                    // SFRotation [in,out] rotation                                    0 0 1 0             (-∞,∞)|[-1,1]
	                                     scale (1, 1, 1),                                             // SFVec3f    [in,out] scale                                       1 1 1               (-∞,∞)
	                          scaleOrientation (),                                                    // SFRotation [in,out] scaleOrientation                            0 0 1 0             (-∞,∞)|[-1,1]
	                                    siteID (),                                                    // SFInt32    [in,out] siteID                                      0                   [0,65535]
	                               translation (),                                                    // SFVec3f    [in,out] translation                                 0 0 0               (-∞,∞)
	                                   warhead (),                                                    // SFInt32    [in,out] warhead                                     0                   [0,65535]
	                             writeInterval (1),                                                   // SFTime     [in,out] writeInterval                               1.0                 [0,∞)
	       articulationParameterValue0_changed (),                                                    // SFFloat    [out]    articulationParameterValue0_changed
	       articulationParameterValue1_changed (),                                                    // SFFloat    [out]    articulationParameterValue1_changed
	       articulationParameterValue2_changed (),                                                    // SFFloat    [out]    articulationParameterValue2_changed
	       articulationParameterValue3_changed (),                                                    // SFFloat    [out]    articulationParameterValue3_changed
	       articulationParameterValue4_changed (),                                                    // SFFloat    [out]    articulationParameterValue4_changed
	       articulationParameterValue5_changed (),                                                    // SFFloat    [out]    articulationParameterValue5_changed
	       articulationParameterValue6_changed (),                                                    // SFFloat    [out]    articulationParameterValue6_changed
	       articulationParameterValue7_changed (),                                                    // SFFloat    [out]    articulationParameterValue7_changed
	                               collideTime (),                                                    // SFTime     [out]    collideTime
	                              detonateTime (),                                                    // SFTime     [out]    detonateTime
	                                 firedTime (),                                                    // SFTime     [out]    firedTime
	                                isCollided (),                                                    // SFBool     [out]    isCollided
	                               isDetonated (),                                                    // SFBool     [out]    isDetonated
	                           isNetworkReader (),                                                    // SFBool     [out]    isNetworkReader
	                           isNetworkWriter (),                                                    // SFBool     [out]    isNetworkWriter
	                          isRtpHeaderHeard (),                                                    // SFBool     [out]    isRtpHeaderHeard
	                              isStandAlone (),                                                    // SFBool     [out]    isStandAlone
	                                 timestamp (),                                                    // SFTime     [out]    timestamp
	                         rtpHeaderExpected ()                                                     // SFBool     [ ]      rtpHeaderExpected                           FALSE
{
	setComponent ("DIS");
	setTypeName ("EspduTransform");

	addField (inputOutput,    "metadata",                                   metadata);
	addField (inputOutput,    "enabled",                                    enabled);
	addField (initializeOnly, "bboxSize",                                   bboxSize);
	addField (initializeOnly, "bboxCenter",                                 bboxCenter);
	addField (inputOnly,      "addChildren",                                addChildren);
	addField (inputOnly,      "removeChildren",                             removeChildren);
	addField (inputOutput,    "children",                                   children);
	addField (outputOnly,     "isActive",                                   isActive);
	addField (inputOnly,      "set_articulationParameterValue0",            set_articulationParameterValue0);
	addField (inputOnly,      "set_articulationParameterValue1",            set_articulationParameterValue1);
	addField (inputOnly,      "set_articulationParameterValue2",            set_articulationParameterValue2);
	addField (inputOnly,      "set_articulationParameterValue3",            set_articulationParameterValue3);
	addField (inputOnly,      "set_articulationParameterValue4",            set_articulationParameterValue4);
	addField (inputOnly,      "set_articulationParameterValue5",            set_articulationParameterValue5);
	addField (inputOnly,      "set_articulationParameterValue6",            set_articulationParameterValue6);
	addField (inputOnly,      "set_articulationParameterValue7",            set_articulationParameterValue7);
	addField (inputOutput,    "address",                                    address);
	addField (inputOutput,    "applicationID",                              applicationID);
	addField (inputOutput,    "articulationParameterCount",                 articulationParameterCount);
	addField (inputOutput,    "articulationParameterDesignatorArray",       articulationParameterDesignatorArray);
	addField (inputOutput,    "articulationParameterChangeIndicatorArray",  articulationParameterChangeIndicatorArray);
	addField (inputOutput,    "articulationParameterIdPartAttachedToArray", articulationParameterIdPartAttachedToArray);
	addField (inputOutput,    "articulationParameterTypeArray",             articulationParameterTypeArray);
	addField (inputOutput,    "articulationParameterArray",                 articulationParameterArray);
	addField (inputOutput,    "center",                                     center);
	addField (inputOutput,    "collisionType",                              collisionType);
	addField (inputOutput,    "deadReckoning",                              deadReckoning);
	addField (inputOutput,    "detonationLocation",                         detonationLocation);
	addField (inputOutput,    "detonationRelativeLocation",                 detonationRelativeLocation);
	addField (inputOutput,    "detonationResult",                           detonationResult);
	addField (inputOutput,    "entityCategory",                             entityCategory);
	addField (inputOutput,    "entityCountry",                              entityCountry);
	addField (inputOutput,    "entityDomain",                               entityDomain);
	addField (inputOutput,    "entityExtra",                                entityExtra);
	addField (inputOutput,    "entityID",                                   entityID);
	addField (inputOutput,    "entityKind",                                 entityKind);
	addField (inputOutput,    "entitySpecific",                             entitySpecific);
	addField (inputOutput,    "entitySubCategory",                          entitySubCategory);
	addField (inputOutput,    "eventApplicationID",                         eventApplicationID);
	addField (inputOutput,    "eventEntityID",                              eventEntityID);
	addField (inputOutput,    "eventNumber",                                eventNumber);
	addField (inputOutput,    "eventSiteID",                                eventSiteID);
	addField (inputOutput,    "fired1",                                     fired1);
	addField (inputOutput,    "fired2",                                     fired2);
	addField (inputOutput,    "fireMissionIndex",                           fireMissionIndex);
	addField (inputOutput,    "firingRange",                                firingRange);
	addField (inputOutput,    "firingRate",                                 firingRate);
	addField (inputOutput,    "forceID",                                    forceID);
	addField (inputOutput,    "fuse",                                       fuse);
	addField (inputOutput,    "linearVelocity",                             linearVelocity);
	addField (inputOutput,    "linearAcceleration",                         linearAcceleration);
	addField (inputOutput,    "marking",                                    marking);
	addField (inputOutput,    "multicastRelayHost",                         multicastRelayHost);
	addField (inputOutput,    "multicastRelayPort",                         multicastRelayPort);
	addField (inputOutput,    "munitionApplicationID",                      munitionApplicationID);
	addField (inputOutput,    "munitionEndPoint",                           munitionEndPoint);
	addField (inputOutput,    "munitionEntityID",                           munitionEntityID);
	addField (inputOutput,    "munitionQuantity",                           munitionQuantity);
	addField (inputOutput,    "munitionSiteID",                             munitionSiteID);
	addField (inputOutput,    "munitionStartPoint",                         munitionStartPoint);
	addField (inputOutput,    "networkMode",                                networkMode);
	addField (inputOutput,    "port",                                       port);
	addField (inputOutput,    "readInterval",                               readInterval);
	addField (inputOutput,    "rotation",                                   rotation);
	addField (inputOutput,    "scale",                                      scale);
	addField (inputOutput,    "scaleOrientation",                           scaleOrientation);
	addField (inputOutput,    "siteID",                                     siteID);
	addField (inputOutput,    "translation",                                translation);
	addField (inputOutput,    "warhead",                                    warhead);
	addField (inputOutput,    "writeInterval",                              writeInterval);
	addField (outputOnly,     "articulationParameterValue0_changed",        articulationParameterValue0_changed);
	addField (outputOnly,     "articulationParameterValue1_changed",        articulationParameterValue1_changed);
	addField (outputOnly,     "articulationParameterValue2_changed",        articulationParameterValue2_changed);
	addField (outputOnly,     "articulationParameterValue3_changed",        articulationParameterValue3_changed);
	addField (outputOnly,     "articulationParameterValue4_changed",        articulationParameterValue4_changed);
	addField (outputOnly,     "articulationParameterValue5_changed",        articulationParameterValue5_changed);
	addField (outputOnly,     "articulationParameterValue6_changed",        articulationParameterValue6_changed);
	addField (outputOnly,     "articulationParameterValue7_changed",        articulationParameterValue7_changed);
	addField (outputOnly,     "collideTime",                                collideTime);
	addField (outputOnly,     "detonateTime",                               detonateTime);
	addField (outputOnly,     "firedTime",                                  firedTime);
	addField (outputOnly,     "isCollided",                                 isCollided);
	addField (outputOnly,     "isDetonated",                                isDetonated);
	addField (outputOnly,     "isNetworkReader",                            isNetworkReader);
	addField (outputOnly,     "isNetworkWriter",                            isNetworkWriter);
	addField (outputOnly,     "isRtpHeaderHeard",                           isRtpHeaderHeard);
	addField (outputOnly,     "isStandAlone",                               isStandAlone);
	addField (outputOnly,     "timestamp",                                  timestamp);
	addField (initializeOnly, "rtpHeaderExpected",                          rtpHeaderExpected);
}

X3DBaseNode*
EspduTransform::create (X3DExecutionContext* const executionContext) const
{
	return new EspduTransform (executionContext);
}

} // X3D
} // titania
