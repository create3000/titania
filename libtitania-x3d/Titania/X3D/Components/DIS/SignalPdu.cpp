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

#include "SignalPdu.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

SignalPdu::SignalPdu (X3DExecutionContext* const executionContext) :
	       X3DBaseNode (executionContext -> getBrowser (), executionContext), 
	     X3DSensorNode (),                                                    
	  X3DBoundedObject (),                                                    
	           address ("localhost"),                                         // SFString [in,out] address             "localhost"
	     applicationID (1),                                                   // SFInt32  [in,out] applicationID       1                   [0,65535]
	              data (),                                                    // MFInt32  [in,out] data                [ ]                 [0,255]
	        dataLength (),                                                    // SFInt32  [in,out] dataLength          0                   [0,65535]
	    encodingScheme (),                                                    // SFInt32  [in,out] encodingScheme      0                   [0,65535]
	          entityID (),                                                    // SFInt32  [in,out] entityID            0                   [0,65535]
	multicastRelayHost (),                                                    // SFString [in,out] multicastRelayHost  ""
	multicastRelayPort (),                                                    // SFInt32  [in,out] multicastRelayPort  0                   [0,4294967295]
	       networkMode ("standAlone"),                                        // SFString [in,out] networkMode         "standAlone"        ["standAlone"|"networkReader"|"networkWriter"]
	              port (),                                                    // SFInt32  [in,out] port                0                   [0,65535]
	           radioID (),                                                    // SFInt32  [in,out] radioID             0                   [0,65535]
	      readInterval (0.1),                                                 // SFFloat  [in,out] readInterval        0.1                 [0,∞)
	 rtpHeaderExpected (),                                                    // SFBool   [in,out] rtpHeaderExpected   FALSE
	        sampleRate (),                                                    // SFInt32  [in,out] sampleRate          0                   [0,65535]
	           samples (),                                                    // SFInt32  [in,out] samples             0                   [0,65535]
	            siteID (),                                                    // SFInt32  [in,out] siteID              0                   [0,65535]
	           tdlType (),                                                    // SFInt32  [in,out] tdlType             0                   [0,65535]
	     whichGeometry (1),                                                   // SFInt32  [in,out] whichGeometry       1                   [-1,∞)
	     writeInterval (1),                                                   // SFFloat  [in,out] writeInterval       1.0                 [0,∞)
	   isNetworkReader (),                                                    // SFBool   [out]    isNetworkReader
	   isNetworkWriter (),                                                    // SFBool   [out]    isNetworkWriter
	  isRtpHeaderHeard (),                                                    // SFBool   [out]    isRtpHeaderHeard
	      isStandAlone (),                                                    // SFBool   [out]    isStandAlone
	         timestamp ()                                                     // SFTime   [out]    timestamp
{
	setComponent ("DIS");
	setTypeName ("SignalPdu");

	addField (inputOutput,    "metadata",           metadata);
	addField (initializeOnly, "bboxSize",           bboxSize);
	addField (initializeOnly, "bboxCenter",         bboxCenter);
	addField (inputOutput,    "enabled",            enabled);
	addField (outputOnly,     "isActive",           isActive);
	addField (inputOutput,    "address",            address);
	addField (inputOutput,    "applicationID",      applicationID);
	addField (inputOutput,    "data",               data);
	addField (inputOutput,    "dataLength",         dataLength);
	addField (inputOutput,    "encodingScheme",     encodingScheme);
	addField (inputOutput,    "entityID",           entityID);
	addField (inputOutput,    "multicastRelayHost", multicastRelayHost);
	addField (inputOutput,    "multicastRelayPort", multicastRelayPort);
	addField (inputOutput,    "networkMode",        networkMode);
	addField (inputOutput,    "port",               port);
	addField (inputOutput,    "radioID",            radioID);
	addField (inputOutput,    "readInterval",       readInterval);
	addField (inputOutput,    "rtpHeaderExpected",  rtpHeaderExpected);
	addField (inputOutput,    "sampleRate",         sampleRate);
	addField (inputOutput,    "samples",            samples);
	addField (inputOutput,    "siteID",             siteID);
	addField (inputOutput,    "tdlType",            tdlType);
	addField (inputOutput,    "whichGeometry",      whichGeometry);
	addField (inputOutput,    "writeInterval",      writeInterval);
	addField (outputOnly,     "isNetworkReader",    isNetworkReader);
	addField (outputOnly,     "isNetworkWriter",    isNetworkWriter);
	addField (outputOnly,     "isRtpHeaderHeard",   isRtpHeaderHeard);
	addField (outputOnly,     "isStandAlone",       isStandAlone);
	addField (outputOnly,     "timestamp",          timestamp);
}

X3DBaseNode*
SignalPdu::create (X3DExecutionContext* const executionContext) const
{
	return new SignalPdu (executionContext);
}

void
SignalPdu::initialize ()
{
	X3DSensorNode::initialize ();
	X3DBoundedObject::initialize ();
}

Box3f
SignalPdu::getBBox ()
{
	return Box3f ();
}

void
SignalPdu::update ()
{ }

void
SignalPdu::dispose ()
{
	X3DBoundedObject::dispose ();
	X3DSensorNode::dispose ();
}

} // X3D
} // titania
