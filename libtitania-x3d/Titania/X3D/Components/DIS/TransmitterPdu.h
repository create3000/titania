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

#ifndef __TITANIA_X3D_COMPONENTS_DIS_TRANSMITTER_PDU_H__
#define __TITANIA_X3D_COMPONENTS_DIS_TRANSMITTER_PDU_H__

#include "../Core/X3DSensorNode.h"
#include "../Grouping/X3DBoundedObject.h"

namespace titania {
namespace X3D {

class TransmitterPdu :
	public X3DSensorNode, public X3DBoundedObject
{
public:

	TransmitterPdu (X3DExecutionContext* const executionContext);

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

	SFVec3f &
	antennaLocation ()
	{ return *fields .antennaLocation; }

	const SFVec3f &
	antennaLocation () const
	{ return *fields .antennaLocation; }

	SFInt32 &
	antennaPatternLength ()
	{ return *fields .antennaPatternLength; }

	const SFInt32 &
	antennaPatternLength () const
	{ return *fields .antennaPatternLength; }

	SFInt32 &
	antennaPatternType ()
	{ return *fields .antennaPatternType; }

	const SFInt32 &
	antennaPatternType () const
	{ return *fields .antennaPatternType; }

	SFInt32 &
	applicationID ()
	{ return *fields .applicationID; }

	const SFInt32 &
	applicationID () const
	{ return *fields .applicationID; }

	SFInt32 &
	cryptoKeyID ()
	{ return *fields .cryptoKeyID; }

	const SFInt32 &
	cryptoKeyID () const
	{ return *fields .cryptoKeyID; }

	SFInt32 &
	cryptoSystem ()
	{ return *fields .cryptoSystem; }

	const SFInt32 &
	cryptoSystem () const
	{ return *fields .cryptoSystem; }

	SFInt32 &
	entityID ()
	{ return *fields .entityID; }

	const SFInt32 &
	entityID () const
	{ return *fields .entityID; }

	SFInt32 &
	frequency ()
	{ return *fields .frequency; }

	const SFInt32 &
	frequency () const
	{ return *fields .frequency; }

	SFInt32 &
	inputSource ()
	{ return *fields .inputSource; }

	const SFInt32 &
	inputSource () const
	{ return *fields .inputSource; }

	SFInt32 &
	lengthOfModulationParameters ()
	{ return *fields .lengthOfModulationParameters; }

	const SFInt32 &
	lengthOfModulationParameters () const
	{ return *fields .lengthOfModulationParameters; }

	SFInt32 &
	modulationTypeDetail ()
	{ return *fields .modulationTypeDetail; }

	const SFInt32 &
	modulationTypeDetail () const
	{ return *fields .modulationTypeDetail; }

	SFInt32 &
	modulationTypeMajor ()
	{ return *fields .modulationTypeMajor; }

	const SFInt32 &
	modulationTypeMajor () const
	{ return *fields .modulationTypeMajor; }

	SFInt32 &
	modulationTypeSpreadSpectrum ()
	{ return *fields .modulationTypeSpreadSpectrum; }

	const SFInt32 &
	modulationTypeSpreadSpectrum () const
	{ return *fields .modulationTypeSpreadSpectrum; }

	SFInt32 &
	modulationTypeSystem ()
	{ return *fields .modulationTypeSystem; }

	const SFInt32 &
	modulationTypeSystem () const
	{ return *fields .modulationTypeSystem; }

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

	SFFloat &
	power ()
	{ return *fields .power; }

	const SFFloat &
	power () const
	{ return *fields .power; }

	SFInt32 &
	radioEntityTypeCategory ()
	{ return *fields .radioEntityTypeCategory; }

	const SFInt32 &
	radioEntityTypeCategory () const
	{ return *fields .radioEntityTypeCategory; }

	SFInt32 &
	radioEntityTypeCountry ()
	{ return *fields .radioEntityTypeCountry; }

	const SFInt32 &
	radioEntityTypeCountry () const
	{ return *fields .radioEntityTypeCountry; }

	SFInt32 &
	radioEntityTypeDomain ()
	{ return *fields .radioEntityTypeDomain; }

	const SFInt32 &
	radioEntityTypeDomain () const
	{ return *fields .radioEntityTypeDomain; }

	SFInt32 &
	radioEntityTypeKind ()
	{ return *fields .radioEntityTypeKind; }

	const SFInt32 &
	radioEntityTypeKind () const
	{ return *fields .radioEntityTypeKind; }

	SFInt32 &
	radioEntityTypeNomenclature ()
	{ return *fields .radioEntityTypeNomenclature; }

	const SFInt32 &
	radioEntityTypeNomenclature () const
	{ return *fields .radioEntityTypeNomenclature; }

	SFInt32 &
	radioEntityTypeNomenclatureVersion ()
	{ return *fields .radioEntityTypeNomenclatureVersion; }

	const SFInt32 &
	radioEntityTypeNomenclatureVersion () const
	{ return *fields .radioEntityTypeNomenclatureVersion; }

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

	SFVec3f &
	relativeAntennaLocation ()
	{ return *fields .relativeAntennaLocation; }

	const SFVec3f &
	relativeAntennaLocation () const
	{ return *fields .relativeAntennaLocation; }

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

	SFFloat &
	transmitFrequencyBandwidth ()
	{ return *fields .transmitFrequencyBandwidth; }

	const SFFloat &
	transmitFrequencyBandwidth () const
	{ return *fields .transmitFrequencyBandwidth; }

	SFInt32 &
	transmitState ()
	{ return *fields .transmitState; }

	const SFInt32 &
	transmitState () const
	{ return *fields .transmitState; }

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
		SFVec3f* const antennaLocation;
		SFInt32* const antennaPatternLength;
		SFInt32* const antennaPatternType;
		SFInt32* const applicationID;
		SFInt32* const cryptoKeyID;
		SFInt32* const cryptoSystem;
		SFInt32* const entityID;
		SFInt32* const frequency;
		SFInt32* const inputSource;
		SFInt32* const lengthOfModulationParameters;
		SFInt32* const modulationTypeDetail;
		SFInt32* const modulationTypeMajor;
		SFInt32* const modulationTypeSpreadSpectrum;
		SFInt32* const modulationTypeSystem;
		SFString* const multicastRelayHost;
		SFInt32* const multicastRelayPort;
		SFString* const networkMode;
		SFInt32* const port;
		SFFloat* const power;
		SFInt32* const radioEntityTypeCategory;
		SFInt32* const radioEntityTypeCountry;
		SFInt32* const radioEntityTypeDomain;
		SFInt32* const radioEntityTypeKind;
		SFInt32* const radioEntityTypeNomenclature;
		SFInt32* const radioEntityTypeNomenclatureVersion;
		SFInt32* const radioID;
		SFFloat* const readInterval;
		SFVec3f* const relativeAntennaLocation;
		SFBool* const rtpHeaderExpected;
		SFInt32* const siteID;
		SFFloat* const transmitFrequencyBandwidth;
		SFInt32* const transmitState;
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
