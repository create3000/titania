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

#include "TransmitterPdu.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const std::string TransmitterPdu::componentName  = "DIS";
const std::string TransmitterPdu::typeName       = "TransmitterPdu";
const std::string TransmitterPdu::containerField = "children";

TransmitterPdu::Fields::Fields () :
	address (new SFString ("localhost")),
	antennaLocation (new SFVec3f ()),
	antennaPatternLength (new SFInt32 ()),
	antennaPatternType (new SFInt32 ()),
	applicationID (new SFInt32 (1)),
	cryptoKeyID (new SFInt32 ()),
	cryptoSystem (new SFInt32 ()),
	entityID (new SFInt32 ()),
	frequency (new SFInt32 ()),
	inputSource (new SFInt32 ()),
	lengthOfModulationParameters (new SFInt32 ()),
	modulationTypeDetail (new SFInt32 ()),
	modulationTypeMajor (new SFInt32 ()),
	modulationTypeSpreadSpectrum (new SFInt32 ()),
	modulationTypeSystem (new SFInt32 ()),
	multicastRelayHost (new SFString ()),
	multicastRelayPort (new SFInt32 ()),
	networkMode (new SFString ("standAlone")),
	port (new SFInt32 ()),
	power (new SFFloat ()),
	radioEntityTypeCategory (new SFInt32 ()),
	radioEntityTypeCountry (new SFInt32 ()),
	radioEntityTypeDomain (new SFInt32 ()),
	radioEntityTypeKind (new SFInt32 ()),
	radioEntityTypeNomenclature (new SFInt32 ()),
	radioEntityTypeNomenclatureVersion (new SFInt32 ()),
	radioID (new SFInt32 ()),
	readInterval (new SFFloat (0.1)),
	relativeAntennaLocation (new SFVec3f ()),
	rtpHeaderExpected (new SFBool ()),
	siteID (new SFInt32 ()),
	transmitFrequencyBandwidth (new SFFloat ()),
	transmitState (new SFInt32 ()),
	whichGeometry (new SFInt32 (1)),
	writeInterval (new SFFloat (1)),
	isNetworkReader (new SFBool ()),
	isNetworkWriter (new SFBool ()),
	isRtpHeaderHeard (new SFBool ()),
	isStandAlone (new SFBool ()),
	timestamp (new SFTime ())
{ }

TransmitterPdu::TransmitterPdu (X3DExecutionContext* const executionContext) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext),
	   X3DSensorNode (),
	X3DBoundedObject (),
	          fields ()
{
	addType (X3DConstants::TransmitterPdu);

	addField (inputOutput,    "metadata",                           metadata ());
	addField (initializeOnly, "bboxSize",                           bboxSize ());
	addField (initializeOnly, "bboxCenter",                         bboxCenter ());
	addField (inputOutput,    "enabled",                            enabled ());
	addField (outputOnly,     "isActive",                           isActive ());
	addField (inputOutput,    "address",                            address ());
	addField (inputOutput,    "antennaLocation",                    antennaLocation ());
	addField (inputOutput,    "antennaPatternLength",               antennaPatternLength ());
	addField (inputOutput,    "antennaPatternType",                 antennaPatternType ());
	addField (inputOutput,    "applicationID",                      applicationID ());
	addField (inputOutput,    "cryptoKeyID",                        cryptoKeyID ());
	addField (inputOutput,    "cryptoSystem",                       cryptoSystem ());
	addField (inputOutput,    "entityID",                           entityID ());
	addField (inputOutput,    "frequency",                          frequency ());
	addField (inputOutput,    "inputSource",                        inputSource ());
	addField (inputOutput,    "lengthOfModulationParameters",       lengthOfModulationParameters ());
	addField (inputOutput,    "modulationTypeDetail",               modulationTypeDetail ());
	addField (inputOutput,    "modulationTypeMajor",                modulationTypeMajor ());
	addField (inputOutput,    "modulationTypeSpreadSpectrum",       modulationTypeSpreadSpectrum ());
	addField (inputOutput,    "modulationTypeSystem",               modulationTypeSystem ());
	addField (inputOutput,    "multicastRelayHost",                 multicastRelayHost ());
	addField (inputOutput,    "multicastRelayPort",                 multicastRelayPort ());
	addField (inputOutput,    "networkMode",                        networkMode ());
	addField (inputOutput,    "port",                               port ());
	addField (inputOutput,    "power",                              power ());
	addField (inputOutput,    "radioEntityTypeCategory",            radioEntityTypeCategory ());
	addField (inputOutput,    "radioEntityTypeCountry",             radioEntityTypeCountry ());
	addField (inputOutput,    "radioEntityTypeDomain",              radioEntityTypeDomain ());
	addField (inputOutput,    "radioEntityTypeKind",                radioEntityTypeKind ());
	addField (inputOutput,    "radioEntityTypeNomenclature",        radioEntityTypeNomenclature ());
	addField (inputOutput,    "radioEntityTypeNomenclatureVersion", radioEntityTypeNomenclatureVersion ());
	addField (inputOutput,    "radioID",                            radioID ());
	addField (inputOutput,    "readInterval",                       readInterval ());
	addField (inputOutput,    "relativeAntennaLocation",            relativeAntennaLocation ());
	addField (inputOutput,    "rtpHeaderExpected",                  rtpHeaderExpected ());
	addField (inputOutput,    "siteID",                             siteID ());
	addField (inputOutput,    "transmitFrequencyBandwidth",         transmitFrequencyBandwidth ());
	addField (inputOutput,    "transmitState",                      transmitState ());
	addField (inputOutput,    "whichGeometry",                      whichGeometry ());
	addField (inputOutput,    "writeInterval",                      writeInterval ());
	addField (outputOnly,     "isNetworkReader",                    isNetworkReader ());
	addField (outputOnly,     "isNetworkWriter",                    isNetworkWriter ());
	addField (outputOnly,     "isRtpHeaderHeard",                   isRtpHeaderHeard ());
	addField (outputOnly,     "isStandAlone",                       isStandAlone ());
	addField (outputOnly,     "timestamp",                          timestamp ());
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

Box3d
TransmitterPdu::getBBox () const
{
	return Box3d ();
}

void
TransmitterPdu::dispose ()
{
	X3DBoundedObject::dispose ();
	X3DSensorNode::dispose ();
}

} // X3D
} // titania
