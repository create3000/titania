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

	EspduTransform (X3DExecutionContext* const executionContext);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const executionContext) const final override;

	///  @name Common members

	virtual
	const std::string &
	getComponentName () const final override
	{ return componentName; }

	virtual
	const std::string &
	getTypeName () const final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const final override
	{ return containerField; }

	///  @name Fields

	SFFloat &
	set_articulationParameterValue0 ()
	{ return *fields .set_articulationParameterValue0; }

	const SFFloat &
	set_articulationParameterValue0 () const
	{ return *fields .set_articulationParameterValue0; }

	SFFloat &
	set_articulationParameterValue1 ()
	{ return *fields .set_articulationParameterValue1; }

	const SFFloat &
	set_articulationParameterValue1 () const
	{ return *fields .set_articulationParameterValue1; }

	SFFloat &
	set_articulationParameterValue2 ()
	{ return *fields .set_articulationParameterValue2; }

	const SFFloat &
	set_articulationParameterValue2 () const
	{ return *fields .set_articulationParameterValue2; }

	SFFloat &
	set_articulationParameterValue3 ()
	{ return *fields .set_articulationParameterValue3; }

	const SFFloat &
	set_articulationParameterValue3 () const
	{ return *fields .set_articulationParameterValue3; }

	SFFloat &
	set_articulationParameterValue4 ()
	{ return *fields .set_articulationParameterValue4; }

	const SFFloat &
	set_articulationParameterValue4 () const
	{ return *fields .set_articulationParameterValue4; }

	SFFloat &
	set_articulationParameterValue5 ()
	{ return *fields .set_articulationParameterValue5; }

	const SFFloat &
	set_articulationParameterValue5 () const
	{ return *fields .set_articulationParameterValue5; }

	SFFloat &
	set_articulationParameterValue6 ()
	{ return *fields .set_articulationParameterValue6; }

	const SFFloat &
	set_articulationParameterValue6 () const
	{ return *fields .set_articulationParameterValue6; }

	SFFloat &
	set_articulationParameterValue7 ()
	{ return *fields .set_articulationParameterValue7; }

	const SFFloat &
	set_articulationParameterValue7 () const
	{ return *fields .set_articulationParameterValue7; }

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
	articulationParameterCount ()
	{ return *fields .articulationParameterCount; }

	const SFInt32 &
	articulationParameterCount () const
	{ return *fields .articulationParameterCount; }

	MFInt32 &
	articulationParameterDesignatorArray ()
	{ return *fields .articulationParameterDesignatorArray; }

	const MFInt32 &
	articulationParameterDesignatorArray () const
	{ return *fields .articulationParameterDesignatorArray; }

	MFInt32 &
	articulationParameterChangeIndicatorArray ()
	{ return *fields .articulationParameterChangeIndicatorArray; }

	const MFInt32 &
	articulationParameterChangeIndicatorArray () const
	{ return *fields .articulationParameterChangeIndicatorArray; }

	MFInt32 &
	articulationParameterIdPartAttachedToArray ()
	{ return *fields .articulationParameterIdPartAttachedToArray; }

	const MFInt32 &
	articulationParameterIdPartAttachedToArray () const
	{ return *fields .articulationParameterIdPartAttachedToArray; }

	MFInt32 &
	articulationParameterTypeArray ()
	{ return *fields .articulationParameterTypeArray; }

	const MFInt32 &
	articulationParameterTypeArray () const
	{ return *fields .articulationParameterTypeArray; }

	MFFloat &
	articulationParameterArray ()
	{ return *fields .articulationParameterArray; }

	const MFFloat &
	articulationParameterArray () const
	{ return *fields .articulationParameterArray; }

	SFVec3f &
	center ()
	{ return *fields .center; }

	const SFVec3f &
	center () const
	{ return *fields .center; }

	SFInt32 &
	collisionType ()
	{ return *fields .collisionType; }

	const SFInt32 &
	collisionType () const
	{ return *fields .collisionType; }

	SFInt32 &
	deadReckoning ()
	{ return *fields .deadReckoning; }

	const SFInt32 &
	deadReckoning () const
	{ return *fields .deadReckoning; }

	SFVec3f &
	detonationLocation ()
	{ return *fields .detonationLocation; }

	const SFVec3f &
	detonationLocation () const
	{ return *fields .detonationLocation; }

	SFVec3f &
	detonationRelativeLocation ()
	{ return *fields .detonationRelativeLocation; }

	const SFVec3f &
	detonationRelativeLocation () const
	{ return *fields .detonationRelativeLocation; }

	SFInt32 &
	detonationResult ()
	{ return *fields .detonationResult; }

	const SFInt32 &
	detonationResult () const
	{ return *fields .detonationResult; }

	SFInt32 &
	entityCategory ()
	{ return *fields .entityCategory; }

	const SFInt32 &
	entityCategory () const
	{ return *fields .entityCategory; }

	SFInt32 &
	entityCountry ()
	{ return *fields .entityCountry; }

	const SFInt32 &
	entityCountry () const
	{ return *fields .entityCountry; }

	SFInt32 &
	entityDomain ()
	{ return *fields .entityDomain; }

	const SFInt32 &
	entityDomain () const
	{ return *fields .entityDomain; }

	SFInt32 &
	entityExtra ()
	{ return *fields .entityExtra; }

	const SFInt32 &
	entityExtra () const
	{ return *fields .entityExtra; }

	SFInt32 &
	entityID ()
	{ return *fields .entityID; }

	const SFInt32 &
	entityID () const
	{ return *fields .entityID; }

	SFInt32 &
	entityKind ()
	{ return *fields .entityKind; }

	const SFInt32 &
	entityKind () const
	{ return *fields .entityKind; }

	SFInt32 &
	entitySpecific ()
	{ return *fields .entitySpecific; }

	const SFInt32 &
	entitySpecific () const
	{ return *fields .entitySpecific; }

	SFInt32 &
	entitySubCategory ()
	{ return *fields .entitySubCategory; }

	const SFInt32 &
	entitySubCategory () const
	{ return *fields .entitySubCategory; }

	SFInt32 &
	eventApplicationID ()
	{ return *fields .eventApplicationID; }

	const SFInt32 &
	eventApplicationID () const
	{ return *fields .eventApplicationID; }

	SFInt32 &
	eventEntityID ()
	{ return *fields .eventEntityID; }

	const SFInt32 &
	eventEntityID () const
	{ return *fields .eventEntityID; }

	SFInt32 &
	eventNumber ()
	{ return *fields .eventNumber; }

	const SFInt32 &
	eventNumber () const
	{ return *fields .eventNumber; }

	SFInt32 &
	eventSiteID ()
	{ return *fields .eventSiteID; }

	const SFInt32 &
	eventSiteID () const
	{ return *fields .eventSiteID; }

	SFBool &
	fired1 ()
	{ return *fields .fired1; }

	const SFBool &
	fired1 () const
	{ return *fields .fired1; }

	SFBool &
	fired2 ()
	{ return *fields .fired2; }

	const SFBool &
	fired2 () const
	{ return *fields .fired2; }

	SFInt32 &
	fireMissionIndex ()
	{ return *fields .fireMissionIndex; }

	const SFInt32 &
	fireMissionIndex () const
	{ return *fields .fireMissionIndex; }

	SFFloat &
	firingRange ()
	{ return *fields .firingRange; }

	const SFFloat &
	firingRange () const
	{ return *fields .firingRange; }

	SFInt32 &
	firingRate ()
	{ return *fields .firingRate; }

	const SFInt32 &
	firingRate () const
	{ return *fields .firingRate; }

	SFInt32 &
	forceID ()
	{ return *fields .forceID; }

	const SFInt32 &
	forceID () const
	{ return *fields .forceID; }

	SFInt32 &
	fuse ()
	{ return *fields .fuse; }

	const SFInt32 &
	fuse () const
	{ return *fields .fuse; }

	SFVec3f &
	linearVelocity ()
	{ return *fields .linearVelocity; }

	const SFVec3f &
	linearVelocity () const
	{ return *fields .linearVelocity; }

	SFVec3f &
	linearAcceleration ()
	{ return *fields .linearAcceleration; }

	const SFVec3f &
	linearAcceleration () const
	{ return *fields .linearAcceleration; }

	SFString &
	marking ()
	{ return *fields .marking; }

	const SFString &
	marking () const
	{ return *fields .marking; }

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

	SFInt32 &
	munitionApplicationID ()
	{ return *fields .munitionApplicationID; }

	const SFInt32 &
	munitionApplicationID () const
	{ return *fields .munitionApplicationID; }

	SFVec3f &
	munitionEndPoint ()
	{ return *fields .munitionEndPoint; }

	const SFVec3f &
	munitionEndPoint () const
	{ return *fields .munitionEndPoint; }

	SFInt32 &
	munitionEntityID ()
	{ return *fields .munitionEntityID; }

	const SFInt32 &
	munitionEntityID () const
	{ return *fields .munitionEntityID; }

	SFInt32 &
	munitionQuantity ()
	{ return *fields .munitionQuantity; }

	const SFInt32 &
	munitionQuantity () const
	{ return *fields .munitionQuantity; }

	SFInt32 &
	munitionSiteID ()
	{ return *fields .munitionSiteID; }

	const SFInt32 &
	munitionSiteID () const
	{ return *fields .munitionSiteID; }

	SFVec3f &
	munitionStartPoint ()
	{ return *fields .munitionStartPoint; }

	const SFVec3f &
	munitionStartPoint () const
	{ return *fields .munitionStartPoint; }

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

	SFTime &
	readInterval ()
	{ return *fields .readInterval; }

	const SFTime &
	readInterval () const
	{ return *fields .readInterval; }

	SFRotation &
	rotation ()
	{ return *fields .rotation; }

	const SFRotation &
	rotation () const
	{ return *fields .rotation; }

	SFVec3f &
	scale ()
	{ return *fields .scale; }

	const SFVec3f &
	scale () const
	{ return *fields .scale; }

	SFRotation &
	scaleOrientation ()
	{ return *fields .scaleOrientation; }

	const SFRotation &
	scaleOrientation () const
	{ return *fields .scaleOrientation; }

	SFInt32 &
	siteID ()
	{ return *fields .siteID; }

	const SFInt32 &
	siteID () const
	{ return *fields .siteID; }

	SFVec3f &
	translation ()
	{ return *fields .translation; }

	const SFVec3f &
	translation () const
	{ return *fields .translation; }

	SFInt32 &
	warhead ()
	{ return *fields .warhead; }

	const SFInt32 &
	warhead () const
	{ return *fields .warhead; }

	SFTime &
	writeInterval ()
	{ return *fields .writeInterval; }

	const SFTime &
	writeInterval () const
	{ return *fields .writeInterval; }

	SFFloat &
	articulationParameterValue0_changed ()
	{ return *fields .articulationParameterValue0_changed; }

	const SFFloat &
	articulationParameterValue0_changed () const
	{ return *fields .articulationParameterValue0_changed; }

	SFFloat &
	articulationParameterValue1_changed ()
	{ return *fields .articulationParameterValue1_changed; }

	const SFFloat &
	articulationParameterValue1_changed () const
	{ return *fields .articulationParameterValue1_changed; }

	SFFloat &
	articulationParameterValue2_changed ()
	{ return *fields .articulationParameterValue2_changed; }

	const SFFloat &
	articulationParameterValue2_changed () const
	{ return *fields .articulationParameterValue2_changed; }

	SFFloat &
	articulationParameterValue3_changed ()
	{ return *fields .articulationParameterValue3_changed; }

	const SFFloat &
	articulationParameterValue3_changed () const
	{ return *fields .articulationParameterValue3_changed; }

	SFFloat &
	articulationParameterValue4_changed ()
	{ return *fields .articulationParameterValue4_changed; }

	const SFFloat &
	articulationParameterValue4_changed () const
	{ return *fields .articulationParameterValue4_changed; }

	SFFloat &
	articulationParameterValue5_changed ()
	{ return *fields .articulationParameterValue5_changed; }

	const SFFloat &
	articulationParameterValue5_changed () const
	{ return *fields .articulationParameterValue5_changed; }

	SFFloat &
	articulationParameterValue6_changed ()
	{ return *fields .articulationParameterValue6_changed; }

	const SFFloat &
	articulationParameterValue6_changed () const
	{ return *fields .articulationParameterValue6_changed; }

	SFFloat &
	articulationParameterValue7_changed ()
	{ return *fields .articulationParameterValue7_changed; }

	const SFFloat &
	articulationParameterValue7_changed () const
	{ return *fields .articulationParameterValue7_changed; }

	SFTime &
	collideTime ()
	{ return *fields .collideTime; }

	const SFTime &
	collideTime () const
	{ return *fields .collideTime; }

	SFTime &
	detonateTime ()
	{ return *fields .detonateTime; }

	const SFTime &
	detonateTime () const
	{ return *fields .detonateTime; }

	SFTime &
	firedTime ()
	{ return *fields .firedTime; }

	const SFTime &
	firedTime () const
	{ return *fields .firedTime; }

	SFBool &
	isCollided ()
	{ return *fields .isCollided; }

	const SFBool &
	isCollided () const
	{ return *fields .isCollided; }

	SFBool &
	isDetonated ()
	{ return *fields .isDetonated; }

	const SFBool &
	isDetonated () const
	{ return *fields .isDetonated; }

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

	SFBool &
	rtpHeaderExpected ()
	{ return *fields .rtpHeaderExpected; }

	const SFBool &
	rtpHeaderExpected () const
	{ return *fields .rtpHeaderExpected; }


private:


	///  @name Static members

	static const std::string componentName;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFFloat* const set_articulationParameterValue0;
		SFFloat* const set_articulationParameterValue1;
		SFFloat* const set_articulationParameterValue2;
		SFFloat* const set_articulationParameterValue3;
		SFFloat* const set_articulationParameterValue4;
		SFFloat* const set_articulationParameterValue5;
		SFFloat* const set_articulationParameterValue6;
		SFFloat* const set_articulationParameterValue7;
		SFString* const address;
		SFInt32* const applicationID;
		SFInt32* const articulationParameterCount;
		MFInt32* const articulationParameterDesignatorArray;
		MFInt32* const articulationParameterChangeIndicatorArray;
		MFInt32* const articulationParameterIdPartAttachedToArray;
		MFInt32* const articulationParameterTypeArray;
		MFFloat* const articulationParameterArray;
		SFVec3f* const center;
		SFInt32* const collisionType;
		SFInt32* const deadReckoning;
		SFVec3f* const detonationLocation;
		SFVec3f* const detonationRelativeLocation;
		SFInt32* const detonationResult;
		SFInt32* const entityCategory;
		SFInt32* const entityCountry;
		SFInt32* const entityDomain;
		SFInt32* const entityExtra;
		SFInt32* const entityID;
		SFInt32* const entityKind;
		SFInt32* const entitySpecific;
		SFInt32* const entitySubCategory;
		SFInt32* const eventApplicationID;
		SFInt32* const eventEntityID;
		SFInt32* const eventNumber;
		SFInt32* const eventSiteID;
		SFBool* const fired1;
		SFBool* const fired2;
		SFInt32* const fireMissionIndex;
		SFFloat* const firingRange;
		SFInt32* const firingRate;
		SFInt32* const forceID;
		SFInt32* const fuse;
		SFVec3f* const linearVelocity;
		SFVec3f* const linearAcceleration;
		SFString* const marking;
		SFString* const multicastRelayHost;
		SFInt32* const multicastRelayPort;
		SFInt32* const munitionApplicationID;
		SFVec3f* const munitionEndPoint;
		SFInt32* const munitionEntityID;
		SFInt32* const munitionQuantity;
		SFInt32* const munitionSiteID;
		SFVec3f* const munitionStartPoint;
		SFString* const networkMode;
		SFInt32* const port;
		SFTime* const readInterval;
		SFRotation* const rotation;
		SFVec3f* const scale;
		SFRotation* const scaleOrientation;
		SFInt32* const siteID;
		SFVec3f* const translation;
		SFInt32* const warhead;
		SFTime* const writeInterval;
		SFFloat* const articulationParameterValue0_changed;
		SFFloat* const articulationParameterValue1_changed;
		SFFloat* const articulationParameterValue2_changed;
		SFFloat* const articulationParameterValue3_changed;
		SFFloat* const articulationParameterValue4_changed;
		SFFloat* const articulationParameterValue5_changed;
		SFFloat* const articulationParameterValue6_changed;
		SFFloat* const articulationParameterValue7_changed;
		SFTime* const collideTime;
		SFTime* const detonateTime;
		SFTime* const firedTime;
		SFBool* const isCollided;
		SFBool* const isDetonated;
		SFBool* const isNetworkReader;
		SFBool* const isNetworkWriter;
		SFBool* const isRtpHeaderHeard;
		SFBool* const isStandAlone;
		SFTime* const timestamp;
		SFBool* const rtpHeaderExpected;
	};

	Fields fields;

};

} // X3D
} // titania

#endif
