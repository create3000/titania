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
 ******************************************************************************/

#include "TransmitterPdu.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

TransmitterPdu::TransmitterPdu (X3DExecutionContext* const executionContext) :
	                       X3DBaseNode (executionContext -> getBrowser (), executionContext), 
	                     X3DSensorNode (),                                                    
	                  X3DBoundedObject (),                                                    
	                           address ("localhost"),                                         // SFString [in,out] address                             "localhost"
	                   antennaLocation (),                                                    // SFVec3f  [in,out] antennaLocation                     0 0 0               (-∞,∞)
	              antennaPatternLength (),                                                    // SFInt32  [in,out] antennaPatternLength                0                   [0,65535]
	                antennaPatternType (),                                                    // SFInt32  [in,out] antennaPatternType                  0                   [0,65535]
	                     applicationID (1),                                                   // SFInt32  [in,out] applicationID                       1                   [0,65535]
	                       cryptoKeyID (),                                                    // SFInt32  [in,out] cryptoKeyID                         0                   [0,65535]
	                      cryptoSystem (),                                                    // SFInt32  [in,out] cryptoSystem                        0                   [0,65535]
	                          entityID (),                                                    // SFInt32  [in,out] entityID                            0                   [0,65535]
	                         frequency (),                                                    // SFInt32  [in,out] frequency                           0                   [0,4294967296]
	                       inputSource (),                                                    // SFInt32  [in,out] inputSource                         0                   [0,255]
	      lengthOfModulationParameters (),                                                    // SFInt32  [in,out] lengthOfModulationParameters        0                   [0,255]
	              modulationTypeDetail (),                                                    // SFInt32  [in,out] modulationTypeDetail                0                   [0,65535]
	               modulationTypeMajor (),                                                    // SFInt32  [in,out] modulationTypeMajor                 0                   [0,65535]
	      modulationTypeSpreadSpectrum (),                                                    // SFInt32  [in,out] modulationTypeSpreadSpectrum        0                   [0,65535]
	              modulationTypeSystem (),                                                    // SFInt32  [in,out] modulationTypeSystem                0                   [0,65535]
	                multicastRelayHost (),                                                    // SFString [in,out] multicastRelayHost                  ""
	                multicastRelayPort (),                                                    // SFInt32  [in,out] multicastRelayPort                  0                   [0,4294967295]
	                       networkMode ("standAlone"),                                        // SFString [in,out] networkMode                         "standAlone"        ["standAlone"|"networkReader"|"networkWriter"]
	                              port (),                                                    // SFInt32  [in,out] port                                0                   [0,65535]
	                             power (),                                                    // SFFloat  [in,out] power                               0.0                 [0,∞)
	           radioEntityTypeCategory (),                                                    // SFInt32  [in,out] radioEntityTypeCategory             0                   [0,255]
	            radioEntityTypeCountry (),                                                    // SFInt32  [in,out] radioEntityTypeCountry              0                   [0,65535]
	             radioEntityTypeDomain (),                                                    // SFInt32  [in,out] radioEntityTypeDomain               0                   [0,255]
	               radioEntityTypeKind (),                                                    // SFInt32  [in,out] radioEntityTypeKind                 0                   [0,255]
	       radioEntityTypeNomenclature (),                                                    // SFInt32  [in,out] radioEntityTypeNomenclature         0                   [0,255]
	radioEntityTypeNomenclatureVersion (),                                                    // SFInt32  [in,out] radioEntityTypeNomenclatureVersion  0                   [0,65535]
	                           radioID (),                                                    // SFInt32  [in,out] radioID                             0                   [0,255]
	                      readInterval (0.1),                                                 // SFFloat  [in,out] readInterval                        0.1                 [0,∞)
	           relativeAntennaLocation (),                                                    // SFVec3f  [in,out] relativeAntennaLocation             0 0 0               (-∞,∞)
	                 rtpHeaderExpected (),                                                    // SFBool   [in,out] rtpHeaderExpected                   FALSE
	                            siteID (),                                                    // SFInt32  [in,out] siteID                              0                   [0,65535]
	        transmitFrequencyBandwidth (),                                                    // SFFloat  [in,out] transmitFrequencyBandwidth          0.0                 (-∞,∞)
	                     transmitState (),                                                    // SFInt32  [in,out] transmitState                       0                   [0,255]
	                     whichGeometry (1),                                                   // SFInt32  [in,out] whichGeometry                       1                   [-1,∞)
	                     writeInterval (1),                                                   // SFFloat  [in,out] writeInterval                       1.0                 [0,∞)
	                   isNetworkReader (),                                                    // SFBool   [out]    isNetworkReader                                         FALSE
	                   isNetworkWriter (),                                                    // SFBool   [out]    isNetworkWriter                                         FALSE
	                  isRtpHeaderHeard (),                                                    // SFBool   [out]    isRtpHeaderHeard                                        FALSE
	                      isStandAlone (),                                                    // SFBool   [out]    isStandAlone                                            FALSE
	                         timestamp ()                                                     // SFTime   [out]    timestamp
{
	setComponent ("DIS");
	setTypeName ("TransmitterPdu");

	addField (inputOutput,    "metadata",                           metadata);
	addField (initializeOnly, "bboxSize",                           bboxSize);
	addField (initializeOnly, "bboxCenter",                         bboxCenter);
	addField (inputOutput,    "enabled",                            enabled);
	addField (outputOnly,     "isActive",                           isActive);
	addField (inputOutput,    "address",                            address);
	addField (inputOutput,    "antennaLocation",                    antennaLocation);
	addField (inputOutput,    "antennaPatternLength",               antennaPatternLength);
	addField (inputOutput,    "antennaPatternType",                 antennaPatternType);
	addField (inputOutput,    "applicationID",                      applicationID);
	addField (inputOutput,    "cryptoKeyID",                        cryptoKeyID);
	addField (inputOutput,    "cryptoSystem",                       cryptoSystem);
	addField (inputOutput,    "entityID",                           entityID);
	addField (inputOutput,    "frequency",                          frequency);
	addField (inputOutput,    "inputSource",                        inputSource);
	addField (inputOutput,    "lengthOfModulationParameters",       lengthOfModulationParameters);
	addField (inputOutput,    "modulationTypeDetail",               modulationTypeDetail);
	addField (inputOutput,    "modulationTypeMajor",                modulationTypeMajor);
	addField (inputOutput,    "modulationTypeSpreadSpectrum",       modulationTypeSpreadSpectrum);
	addField (inputOutput,    "modulationTypeSystem",               modulationTypeSystem);
	addField (inputOutput,    "multicastRelayHost",                 multicastRelayHost);
	addField (inputOutput,    "multicastRelayPort",                 multicastRelayPort);
	addField (inputOutput,    "networkMode",                        networkMode);
	addField (inputOutput,    "port",                               port);
	addField (inputOutput,    "power",                              power);
	addField (inputOutput,    "radioEntityTypeCategory",            radioEntityTypeCategory);
	addField (inputOutput,    "radioEntityTypeCountry",             radioEntityTypeCountry);
	addField (inputOutput,    "radioEntityTypeDomain",              radioEntityTypeDomain);
	addField (inputOutput,    "radioEntityTypeKind",                radioEntityTypeKind);
	addField (inputOutput,    "radioEntityTypeNomenclature",        radioEntityTypeNomenclature);
	addField (inputOutput,    "radioEntityTypeNomenclatureVersion", radioEntityTypeNomenclatureVersion);
	addField (inputOutput,    "radioID",                            radioID);
	addField (inputOutput,    "readInterval",                       readInterval);
	addField (inputOutput,    "relativeAntennaLocation",            relativeAntennaLocation);
	addField (inputOutput,    "rtpHeaderExpected",                  rtpHeaderExpected);
	addField (inputOutput,    "siteID",                             siteID);
	addField (inputOutput,    "transmitFrequencyBandwidth",         transmitFrequencyBandwidth);
	addField (inputOutput,    "transmitState",                      transmitState);
	addField (inputOutput,    "whichGeometry",                      whichGeometry);
	addField (inputOutput,    "writeInterval",                      writeInterval);
	addField (outputOnly,     "isNetworkReader",                    isNetworkReader);
	addField (outputOnly,     "isNetworkWriter",                    isNetworkWriter);
	addField (outputOnly,     "isRtpHeaderHeard",                   isRtpHeaderHeard);
	addField (outputOnly,     "isStandAlone",                       isStandAlone);
	addField (outputOnly,     "timestamp",                          timestamp);
}

X3DBaseNode*
TransmitterPdu::create (X3DExecutionContext* const executionContext) const
{
	return new TransmitterPdu (executionContext);
}

void
TransmitterPdu::initialize ()
{
	X3DSensorNode::initialize ();
	X3DBoundedObject::initialize ();
}

Box3f
TransmitterPdu::getBBox ()
{
	return Box3f ();
}

void
TransmitterPdu::update ()
{ }

void
TransmitterPdu::dispose ()
{
	X3DBoundedObject::dispose ();
	X3DSensorNode::dispose ();
}

} // X3D
} // titania
