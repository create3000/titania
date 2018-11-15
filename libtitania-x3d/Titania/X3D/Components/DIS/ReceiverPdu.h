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

#ifndef __TITANIA_X3D_COMPONENTS_DIS_RECEIVER_PDU_H__
#define __TITANIA_X3D_COMPONENTS_DIS_RECEIVER_PDU_H__

#include "../Core/X3DSensorNode.h"
#include "../Grouping/X3DBoundedObject.h"

namespace titania {
namespace X3D {

class ReceiverPdu :
	public X3DSensorNode, public X3DBoundedObject
{
public:

	ReceiverPdu (X3DExecutionContext* const executionContext);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const executionContext) const final override;

	///  @name Common members

	virtual
	ComponentType
	getComponent () const final override
	{ return component; }

	virtual
	const std::string &
	getTypeName () const final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const final override
	{ return containerField; }

	///  @name Fields

	SFString &
	address ()
	{ return *fields .address; }

	const SFString &
	address () const
	{ return *fields .address; }

	SFInt32 &
	applicationID ()
	{ return *fields .applicationID; }

	const SFInt32 &
	applicationID () const
	{ return *fields .applicationID; }

	SFInt32 &
	entityID ()
	{ return *fields .entityID; }

	const SFInt32 &
	entityID () const
	{ return *fields .entityID; }

	SFString &
	multicastRelayHost ()
	{ return *fields .multicastRelayHost; }

	const SFString &
	multicastRelayHost () const
	{ return *fields .multicastRelayHost; }

	SFInt32 &
	multicastRelayPort ()
	{ return *fields .multicastRelayPort; }

	const SFInt32 &
	multicastRelayPort () const
	{ return *fields .multicastRelayPort; }

	SFString &
	networkMode ()
	{ return *fields .networkMode; }

	const SFString &
	networkMode () const
	{ return *fields .networkMode; }

	SFInt32 &
	port ()
	{ return *fields .port; }

	const SFInt32 &
	port () const
	{ return *fields .port; }

	SFInt32 &
	radioID ()
	{ return *fields .radioID; }

	const SFInt32 &
	radioID () const
	{ return *fields .radioID; }

	SFFloat &
	readInterval ()
	{ return *fields .readInterval; }

	const SFFloat &
	readInterval () const
	{ return *fields .readInterval; }

	SFFloat &
	receivedPower ()
	{ return *fields .receivedPower; }

	const SFFloat &
	receivedPower () const
	{ return *fields .receivedPower; }

	SFInt32 &
	receiverState ()
	{ return *fields .receiverState; }

	const SFInt32 &
	receiverState () const
	{ return *fields .receiverState; }

	SFBool &
	rtpHeaderExpected ()
	{ return *fields .rtpHeaderExpected; }

	const SFBool &
	rtpHeaderExpected () const
	{ return *fields .rtpHeaderExpected; }

	SFInt32 &
	siteID ()
	{ return *fields .siteID; }

	const SFInt32 &
	siteID () const
	{ return *fields .siteID; }

	SFInt32 &
	transmitterApplicationID ()
	{ return *fields .transmitterApplicationID; }

	const SFInt32 &
	transmitterApplicationID () const
	{ return *fields .transmitterApplicationID; }

	SFInt32 &
	transmitterEntityID ()
	{ return *fields .transmitterEntityID; }

	const SFInt32 &
	transmitterEntityID () const
	{ return *fields .transmitterEntityID; }

	SFInt32 &
	transmitterRadioID ()
	{ return *fields .transmitterRadioID; }

	const SFInt32 &
	transmitterRadioID () const
	{ return *fields .transmitterRadioID; }

	SFInt32 &
	transmitterSiteID ()
	{ return *fields .transmitterSiteID; }

	const SFInt32 &
	transmitterSiteID () const
	{ return *fields .transmitterSiteID; }

	SFInt32 &
	whichGeometry ()
	{ return *fields .whichGeometry; }

	const SFInt32 &
	whichGeometry () const
	{ return *fields .whichGeometry; }

	SFFloat &
	writeInterval ()
	{ return *fields .writeInterval; }

	const SFFloat &
	writeInterval () const
	{ return *fields .writeInterval; }

	SFBool &
	isNetworkReader ()
	{ return *fields .isNetworkReader; }

	const SFBool &
	isNetworkReader () const
	{ return *fields .isNetworkReader; }

	SFBool &
	isNetworkWriter ()
	{ return *fields .isNetworkWriter; }

	const SFBool &
	isNetworkWriter () const
	{ return *fields .isNetworkWriter; }

	SFBool &
	isRtpHeaderHeard ()
	{ return *fields .isRtpHeaderHeard; }

	const SFBool &
	isRtpHeaderHeard () const
	{ return *fields .isRtpHeaderHeard; }

	SFBool &
	isStandAlone ()
	{ return *fields .isStandAlone; }

	const SFBool &
	isStandAlone () const
	{ return *fields .isStandAlone; }

	SFTime &
	timestamp ()
	{ return *fields .timestamp; }

	const SFTime &
	timestamp () const
	{ return *fields .timestamp; }

	virtual
	Box3d
	getBBox () const final override;

	virtual
	void
	dispose () final override;


private:

	virtual
	void
	initialize () final override;

	///  @name Static members

	static const ComponentType component;
	static const std::string   typeName;
	static const std::string   containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFString* const address;
		SFInt32* const applicationID;
		SFInt32* const entityID;
		SFString* const multicastRelayHost;
		SFInt32* const multicastRelayPort;
		SFString* const networkMode;
		SFInt32* const port;
		SFInt32* const radioID;
		SFFloat* const readInterval;
		SFFloat* const receivedPower;
		SFInt32* const receiverState;
		SFBool* const rtpHeaderExpected;
		SFInt32* const siteID;
		SFInt32* const transmitterApplicationID;
		SFInt32* const transmitterEntityID;
		SFInt32* const transmitterRadioID;
		SFInt32* const transmitterSiteID;
		SFInt32* const whichGeometry;
		SFFloat* const writeInterval;
		SFBool* const isNetworkReader;
		SFBool* const isNetworkWriter;
		SFBool* const isRtpHeaderHeard;
		SFBool* const isStandAlone;
		SFTime* const timestamp;
	};

	Fields fields;

};

} // X3D
} // titania

#endif
