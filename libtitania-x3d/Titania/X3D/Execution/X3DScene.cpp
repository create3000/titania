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

#include "X3DScene.h"

#include "../Bits/Error.h"
#include "../Bits/Traverse.h"
#include "../Browser/X3DBrowser.h"
#include "../Components/Core/WorldInfo.h"
#include "../Execution/ExportedNode.h"
#include "../Parser/Filter.h"
#include "../Parser/Parser.h"

#include <Titania/String/to_string.h>

#include <random>

namespace titania {
namespace X3D {

static std::default_random_engine
random_engine (std::chrono::system_clock::now () .time_since_epoch () .count ());

const UnitIndex X3DScene::unitCategories = {
	std::pair ("angle",  UnitCategory::ANGLE),
	std::pair ("force",  UnitCategory::FORCE),
	std::pair ("length", UnitCategory::LENGTH),
	std::pair ("mass",   UnitCategory::MASS)

};

const UnitArray X3DScene::standardUnits = {
	Unit ("none",   "none",     1),
	Unit ("angle",  "radian",   1),
	Unit ("force",  "newton",   1),
	Unit ("length", "metre",    1),
	Unit ("mass",   "kilogram", 1)

};

X3DScene::X3DScene () :
	         X3DBaseNode (),
	 X3DExecutionContext (),
	            worldURL (),
	            encoding (EncodingType::SCRIPTED),
	specificationVersion (LATEST_VERSION),
	   characterEncoding ("utf8"),
	             comment ("Titania"),
	             profile (),
	       profileOutput (),
	          components (new ComponentInfoArray ()),
	    componentsOutput (),
	               units (standardUnits),
	         unitsOutput (),
	           metadatas (),
	      metaDataOutput (),
	       exportedNodes (),
	 exportedNodesOutput (),
	   fileChangedOutput (),
	         fileMonitor ()
{
	addType (X3DConstants::X3DScene);

	addChildObjects (getRootNodes (),
	                 profileOutput,
	                 componentsOutput,
	                 unitsOutput,
	                 metaDataOutput,
	                 exportedNodesOutput,
	                 fileChangedOutput);

	profileOutput       .setAccessType (outputOnly);
	componentsOutput    .setAccessType (outputOnly);
	unitsOutput         .setAccessType (outputOnly);
	metaDataOutput      .setAccessType (outputOnly);
	exportedNodesOutput .setAccessType (outputOnly);
	fileChangedOutput   .setAccessType (outputOnly);
}

void
X3DScene::initialize ()
{
	X3DBaseNode::initialize ();
	X3DExecutionContext::initialize ();
}

///  throws Error <X3D::INVALID_OPERATION_TIMING>, Error <X3D::DISPOSED>
void
X3DScene::setWorldURL (const basic::uri & value)
{
	worldURL = value;

	monitorFile (worldURL);
}

void
X3DScene::monitorFile (const basic::uri & URL)
{
	try
	{
		fileMonitor .reset ();

		if (not URL .is_local ())
			return;

		fileMonitor = Gio::File::create_for_path (URL .path ()) -> monitor_file ();

		fileMonitor -> signal_changed () .connect (sigc::mem_fun (this, &X3DScene::on_file_changed));
	}
	catch (const Glib::Error & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
X3DScene::on_file_changed (const Glib::RefPtr <Gio::File> & file,
                           const Glib::RefPtr <Gio::File> & other_file,
                           Gio::FileMonitorEvent event)
{
	try
	{
		if (event not_eq Gio::FILE_MONITOR_EVENT_CHANGES_DONE_HINT)
			return;

		const auto fileInfo = file -> query_info ();

		fileChangedOutput = fileInfo -> modification_time () .as_double ();
	}
	catch (const Glib::Error & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

///  throws Error <X3D::INVALID_OPERATION_TIMING>, Error <X3D::DISPOSED>
void
X3DScene::setSpecificationVersion (const std::string & value)
{
	static const std::map <std::string, SpecificationVersionType> specificationVersions = {
		std::pair ("2.0", VRML_V2_0),
		std::pair ("3.0",  X3D_V3_0),
		std::pair ("3.1",  X3D_V3_1),
		std::pair ("3.2",  X3D_V3_2),
		std::pair ("3.3",  X3D_V3_3)
	};

	try
	{
		specificationVersion = specificationVersions .at (value);
	}
	catch (const std::out_of_range &)
	{
		specificationVersion = LATEST_VERSION;
	}
}

// Profile/Component handling

void
X3DScene::setProfile (const ProfileInfoPtr & value)
{
	profile = value;

	profileOutput = getCurrentTime ();
}

void
X3DScene::setComponents (const ComponentInfoArray & value)
{
	*components = value;

	componentsOutput = getCurrentTime ();
}

void
X3DScene::inferProfileAndComponents ()
{
	const auto usedComponents = getUsedComponents ();
	const auto profilesNames  = std::vector <std::string> ({ "Interchange", "Interactive", "Immersive" });

	auto profiles = std::vector <std::tuple <ProfileInfoPtr, ComponentInfoArray, size_t>> ();

	for (const auto & profileName : profilesNames)
	{
		auto profile                     = getBrowser () -> getProfile (profileName);
		auto [components, numComponents] = getComponents (profile, usedComponents);

		profiles .emplace_back (std::move (profile), std::move (components), std::move (numComponents));
	}

	size_t lowestTotal = -1;
	size_t index       = 0;

	for (size_t i = 0, size = profiles .size (); i < size; ++ i)
	{
		const auto numExtraComponents = std::get <1> (profiles [i]) .size ();

		if (numExtraComponents < lowestTotal)
		{
			lowestTotal = numExtraComponents;
			index       = i;
		}
	}

	const auto & [profile, components, numComponents] = profiles [index];
	const auto   full                                 = getBrowser () -> getProfile ("Full");

	if (numComponents == full -> getComponents () .size ())
	{
		setProfile (full);
		setComponents ({ });
	}
	else
	{
		setProfile (profile);
		setComponents (components);
	}
}

std::map <std::string, size_t>
X3DScene::getUsedComponents () const
{
	auto usedComponents = std::map <std::string, size_t> ();

	X3D::traverse (const_cast <X3DScene*> (this), [&] (SFNode & node)
	{
		const auto & component = node -> getComponent ();

		// Protos are component 'Titania'.
		if (component .first == "Titania")
			return true;

		usedComponents .emplace (component .first, component .second);
		return true;
	},
	TRAVERSE_EXTERNPROTO_DECLARATIONS |
	TRAVERSE_PROTO_DECLARATIONS |
	TRAVERSE_PROTO_DECLARATION_BODY |
	TRAVERSE_ROOT_NODES);

	return usedComponents;
}

std::pair <ComponentInfoArray, size_t>
X3DScene::getComponents (const ProfileInfoPtr & profileInfo, const std::map <std::string, size_t> & usedComponents) const
{
	auto profileComponents = std::map <std::string, size_t> ();
	auto components        = ComponentInfoArray ();

	for (const auto & componentInfo : profileInfo -> getComponents ())
		profileComponents .emplace (componentInfo -> getName (), componentInfo -> getLevel ());

	for (const auto & [componentName, level] : usedComponents)
	{
		try
		{
			const auto profile = profileComponents .find (componentName);
	
			if (profile not_eq profileComponents .end () and profile -> second >= level)
				continue;
	
			components .emplace_back (getBrowser () -> getComponent (componentName, level));
		}
		catch (const X3DError & error)
		{
			__LOG__ << error .what () << std::endl;	
		}
	}

	auto difference = std::vector <Component> ();

	std::set_difference (profileComponents .cbegin (),
	                     profileComponents .cend (),
	                     usedComponents .cbegin (),
	                     usedComponents .cend (),
	                     std::back_inserter (difference),
	[] (const Component & lhs, const Component & rhs)
	{
		return lhs .first < rhs .first;
	});

	return std::pair (components, usedComponents .size () + difference .size ());
}

// Unit handling

///  throws Error <INVALID_NAME>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
void
X3DScene::updateUnit (const std::string & category, const std::string & name, const long double conversionFactor)
{
	try
	{
		const auto & unit = unitCategories .at (category);

		units [size_t (unit)] = Unit (category, name, conversionFactor);

		unitsOutput = getCurrentTime ();
	}
	catch (const std::out_of_range &)
	{
		throw Error <INVALID_NAME> ("Unkown standard unit category '" + category + "'.");
	}
}

const Unit &
X3DScene::getUnit (const UnitCategory unit) const
{
	try
	{
		return units .at (size_t (unit));
	}
	catch (const std::out_of_range &)
	{
		throw Error <INVALID_NAME> ("X3DScene::getUnit: invalid unit category.");
	}
}

///  throws Error <X3D::DISPOSED>
long double
X3DScene::fromUnit (const UnitCategory unit, const long double value) const
{
	switch (unit)
	{
	   case UnitCategory::NONE:
			return value;
	   
		// Base units
	   case UnitCategory::ANGLE:
	   case UnitCategory::FORCE:
	   case UnitCategory::LENGTH:
	   case UnitCategory::MASS:
			return value * getUnit (unit) .getConversionFactor ();
	
		// Derived units
		case UnitCategory::ACCELERATION:
			return value * getUnit (UnitCategory::LENGTH) .getConversionFactor ();
		case UnitCategory::ANGULAR_RATE:
			return value * getUnit (UnitCategory::ANGLE) .getConversionFactor ();
		case UnitCategory::AREA:
			return value * std::pow (getUnit (UnitCategory::LENGTH) .getConversionFactor (), 2);
		case UnitCategory::SPEED:
			return value * getUnit (UnitCategory::LENGTH) .getConversionFactor ();
		case UnitCategory::VOLUME:
			return value * std::pow (getUnit (UnitCategory::LENGTH) .getConversionFactor (), 3);
	}

	return value;
}

///  throws Error <X3D::DISPOSED>
long double
X3DScene::toUnit (const UnitCategory unit, const long double value) const
{
	switch (unit)
	{
	   case UnitCategory::NONE:
			return value;
	   
		// Base units
	   case UnitCategory::ANGLE:
	   case UnitCategory::FORCE:
	   case UnitCategory::LENGTH:
	   case UnitCategory::MASS:
			return value / getUnit (unit) .getConversionFactor ();
	
		// Derived units
		case UnitCategory::ACCELERATION:
			return value / getUnit (UnitCategory::LENGTH) .getConversionFactor ();
		case UnitCategory::ANGULAR_RATE:
			return value / getUnit (UnitCategory::ANGLE) .getConversionFactor ();
		case UnitCategory::AREA:
			return value / std::pow (getUnit (UnitCategory::LENGTH) .getConversionFactor (), 2);
		case UnitCategory::SPEED:
			return value / getUnit (UnitCategory::LENGTH) .getConversionFactor ();
		case UnitCategory::VOLUME:
			return value / std::pow (getUnit (UnitCategory::LENGTH) .getConversionFactor (), 3);
	}

	return value;
}

// MetaData handling

///  throws Error <X3D::INVALID_OPERATION_TIMING>, Error <X3D::DISPOSED>
void
X3DScene::addStandardMetaData ()
{
	// VRML files are saved as is, but we do not add extra stuff!

	setMetaData ("comment", "World of " + getBrowser () -> getName ());
	setMetaData ("generator", getBrowser () -> getName () + " V" + getBrowser () -> getVersion () + ", http://titania.create3000.de");
	setMetaData ("identifier", getWorldURL ());
	setMetaData ("modified", X3D::SFTime (SFTime::now ()) .toUTCString ());

	if (not metadatas .count ("creator"))
	{
		const std::string fullname = Glib::get_real_name ();

		if (not fullname .empty ())
			setMetaData ("creator", Glib::get_real_name ());
	}

	if (not metadatas .count ("created"))
		setMetaData ("created", X3D::SFTime (SFTime::now ()) .toUTCString ());
}

///  throws Error <X3D::INVALID_OPERATION_TIMING>, Error <X3D::DISPOSED>
void
X3DScene::addMetaData (const std::string & key, const std::string & value)
{
	if (key .empty ())
		return;

	metadatas .emplace (key, value);

	metaDataOutput = getCurrentTime ();
}

///  throws Error <X3D::INVALID_OPERATION_TIMING>, Error <X3D::DISPOSED>
void
X3DScene::removeMetaData (const std::string & key)
{
	metadatas .erase (key);

	metaDataOutput = getCurrentTime ();
}

///  throws Error <X3D::INVALID_OPERATION_TIMING>, Error <X3D::DISPOSED>
void
X3DScene::setMetaData (const std::string & key, const std::string & value)
{
	if (key .empty ())
		return;

	metadatas .erase (key);
	addMetaData (key, value);

	metaDataOutput = getCurrentTime ();
}

///  throws Error <INVALID_NAME>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
const std::string &
X3DScene::getMetaData (const std::string & key) const
{
	auto range = metadatas .equal_range (key);

	if (range .first not_eq range .second)
		return (-- range .second) -> second;

	throw Error <INVALID_NAME> ("Unkown meta key '" + key + "'.");
}

///  throws Error <X3D::INVALID_OPERATION_TIMING>, Error <X3D::DISPOSED>
const MetaDataIndex &
X3DScene::getMetaDatas () const
{
	return metadatas;
}

// Exported nodes handling

///  throws Error <NODE_IN_USE>, Error <INVALID_NAME>, Error <INVALID_NODE>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
const ExportedNodePtr &
X3DScene::addExportedNode (const std::string & exportedName, const SFNode & node)
{
	if (exportedNodes .count (exportedName))
		throw Error <NODE_IN_USE> ("Couldn't add exported node: exported name '" + exportedName + "' already in use.");

	return updateExportedNode (exportedName, node);
}

///  throws Error <INVALID_NAME>, Error <INVALID_NODE>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
const ExportedNodePtr &
X3DScene::updateExportedNode (const std::string & exportedName, const SFNode & node)
{
	if (exportedName .empty ())
		throw Error <INVALID_NAME> ("Couldn't update exported node: exported node name is empty.");

	if (not node)
		throw Error <INVALID_NODE> ("Couldn't update exported node: node is NULL.");

	// We do not throw Error <IMPORTED_NODE> as imported nodes can be exported too.
	
	// Remove exported node.

	removeExportedNode (exportedName);

	// Add exported node.

	auto & exportedNode = exportedNodes .emplace (exportedName, new ExportedNode (this, exportedName, node)) .first -> second;

	exportedNode .setTainted (true);
	exportedNode .addParent (this);

	if (getRealized ())
		exportedNode -> setup ();
	else
		addUninitializedNode (exportedNode);

	exportedNodesOutput = getCurrentTime ();

	return exportedNode;
}

///  throws Error <X3D::INVALID_OPERATION_TIMING>, Error <X3D::DISPOSED>
void
X3DScene::removeExportedNode (const std::string & exportedName)
{
	exportedNodes .erase (exportedName);

	exportedNodesOutput = getCurrentTime ();
}

///  throws Error <INVALID_NAME>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
SFNode
X3DScene::getExportedNode (const std::string & exportedName) const
{
	const auto iter = exportedNodes .find (exportedName);

	if (iter not_eq exportedNodes .end ())
		return iter -> second -> getLocalNode ();

	throw Error <INVALID_NAME> ("Exported node '" + exportedName + "' not found.");
}

/***
 *  Returns a exported name that is both unique in this scene and @a scene, if the name exits in this context otherwise return @a exportedName.
 */
std::string
X3DScene::getVeryUniqueExportedName (const X3DScene* const scene, std::string exportedName) const
{
	if (not exportedName .empty () and not exportedNodes .count (exportedName))
		return exportedName;

	exportedName = RemoveTrailingNumber (exportedName);
 	
	auto   uniqueName = exportedName;
	size_t i          = 0;

	while (exportedNodes .count (uniqueName) or scene -> exportedNodes .count (uniqueName) or uniqueName .empty ())
	{
		i = std::max <size_t> (64, i);

		const auto min = i;
		std::uniform_int_distribution <size_t> random (min, i <<= 1);

		uniqueName = exportedName;
		uniqueName += '_';
		uniqueName += basic::to_string (random (random_engine), std::locale::classic ());
	}
	
	return uniqueName;
}

// Import handling

///  importScene
///
///  throws:
///    Error <INVALID_NAME>,
///	 Error <NOT_SUPPORTED>,
///	 Error <INVALID_OPERATION_TIMING>,
///	 Error <DISPOSED>
MFNode
X3DScene::importScene (X3DExecutionContext* const executionContext)
{
	ContextLock lock (getBrowser ());

	if (getProfile () or not getComponents () .empty ())
	   setProfile (getBrowser () -> getProfile ("Full"));

	//importMetaData (other); // Makes no sense.

	const auto scene = dynamic_cast <X3DScene*> (executionContext);

	if (scene)
		updateExportedNodes (scene);

	const auto nodes = X3DExecutionContext::importScene (executionContext);

	if (scene)
		importExportedNodes (scene);

	return nodes;
}

void
X3DScene::importMetaData (const X3DScene* const scene)
{
	for (const auto & metaData : scene -> getMetaDatas ())
		addMetaData (metaData .first, metaData .second);
}

void
X3DScene::updateExportedNodes (X3DScene* const scene) const
{
	for (const auto & pair : ExportedNodeIndex (scene -> getExportedNodes ()))
	{
		try
		{
			const auto & exportedNode       = pair .second;
			const auto   uniqueExportedName = getVeryUniqueExportedName (scene, exportedNode -> getExportedName ());
	
			scene -> updateExportedNode (uniqueExportedName, exportedNode -> getLocalNode ());
	
			if (uniqueExportedName not_eq exportedNode -> getExportedName ())
				scene -> removeExportedNode (exportedNode -> getExportedName ());
		}
		catch (const Error <DISPOSED> & error)
		{ }
	}
}

void
X3DScene::importExportedNodes (const X3DScene* const scene)
{
	for (const auto & exportedNode : scene -> getExportedNodes ())
	{
		try
		{
			exportedNode .second -> copy (this);
		}
		catch (const Error <DISPOSED> & error)
		{ }
	}
}

// Input/Output

///  throws Error <INVALID_X3D>, Error <NOT_SUPPORTED>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
void
X3DScene::fromStream (const basic::uri & worldURL, std::istream & istream)
{
	setWorldURL (worldURL);

	fromStream (istream);
}

///  throws Error <INVALID_X3D>, Error <NOT_SUPPORTED>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
void
X3DScene::fromStream (std::istream & istream)
{
	Parser (istream, this) .parseIntoScene ();
}

void
X3DScene::toStream (std::ostream & ostream) const
{
	ostream .imbue (std::locale::classic ());

	const auto specificationVersion = getSpecificationVersion ();

	Generator::SpecificationVersion (ostream, specificationVersion);

	if (specificationVersion == VRML_V2_0)
		Generator::Units (ostream, false);

	ostream
		<< '#'
		<< specificationVersion
		<< Generator::Space
		<< getCharacterEncoding ();

	if (true or getComment () .empty ())
	{
		ostream
			<< Generator::Space
			<< getBrowser () -> getName ()
			<< " V"
			<< getBrowser () -> getVersion ();
	}
	else
	{
		ostream
			<< Generator::Space
			<< getComment ();
	}

	ostream
		<< Generator::ForceBreak
		<< Generator::ForceBreak;

	if (not getComments () .empty ())
	{
		for (const auto & comment : getComments ())
		{
			ostream
				<< Generator::Comment
				<< comment
				<< Generator::ForceBreak;
		}

		ostream << Generator::TidyBreak;
	}

	if (specificationVersion not_eq VRML_V2_0)
	{
		if (getProfile ())
		{
			ostream
				<< *getProfile ()
				<< Generator::Break
				<< Generator::TidyBreak;
		}

		if (not getComponents () .empty ())
		{
			for (const auto & component : getComponents ())
			{
				ostream
					<< *component
					<< Generator::Break;
			}

			ostream << Generator::TidyBreak;
		}

		// Units
		{
			bool empty = true;

			for (const auto & unit : getUnits ())
			{
				if (unit .getConversionFactor () not_eq 1)
				{
					empty = false;

					ostream
						<< unit
						<< Generator::Break;
				}
			}

			if (not empty)
				ostream << Generator::TidyBreak;
		}

		if (not getMetaDatas () .empty ())
		{
			for (const auto & metaData : getMetaDatas ())
			{
				ostream
					<< "META"
					<< Generator::Space
					<< SFString (metaData .first)
					<< Generator::Space
					<< SFString (metaData .second)
					<< Generator::Break;
			}

			ostream << Generator::TidyBreak;
		}
	}

	// Scene

	Generator::PushScene (ostream, this);
	Generator::EnterScope (ostream);
	Generator::ExportedNodes (ostream, getExportedNodes ());

	X3DExecutionContext::toStream (ostream);

	if (specificationVersion not_eq VRML_V2_0)
	{
		if (not getExportedNodes () .empty ())
		{
			ostream << Generator::TidyBreak;

			for (const auto & exportedNode : getExportedNodes ())
			{
				try
				{
					ostream
						<< exportedNode .second
						<< Generator::Break;
				}
				catch (const X3DError &)
				{ }
			}
		}
	}

	Generator::LeaveScope (ostream);
	Generator::PopScene (ostream);

	// ~Scene

	if (not getInnerComments () .empty ())
	{
		ostream << Generator::TidyBreak;

		for (const auto & comment : getInnerComments ())
		{
			ostream
				<< Generator::Indent
				<< Generator::Comment
				<< comment
				<< Generator::ForceBreak;
		}
	}

	ostream << std::flush;
}

void
X3DScene::toXMLStream (std::ostream & ostream) const
{
	ostream .imbue (std::locale::classic ());

	auto specificationVersion = getSpecificationVersion ();

	if (specificationVersion == VRML_V2_0)
		specificationVersion = LATEST_VERSION;

	const auto versionString = XMLEncode (specificationVersion);

	Generator::SpecificationVersion (ostream, specificationVersion);

	ostream
		<< "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
		<< Generator::ForceBreak;

	ostream
		<< "<!DOCTYPE X3D PUBLIC \"ISO//Web3D//DTD X3D "
		<< versionString
		<< "//EN\" \"http://www.web3d.org/specifications/x3d-"
		<< versionString
		<< ".dtd\">"
		<< Generator::ForceBreak;

	ostream
		<< "<X3D"
		<< Generator::Space
		<< "profile='"
		<< (getProfile () ? getProfile () -> getName () : "Full")
		<< "'"
		<< Generator::Space
		<< "version='"
		<< versionString
		<< "'"
		<< Generator::Space
		<< "xmlns:xsd='http://www.w3.org/2001/XMLSchema-instance'"
		<< Generator::Space
		<< "xsd:noNamespaceSchemaLocation='http://www.web3d.org/specifications/x3d-"
		<< versionString
		<< ".xsd'>"
		<< Generator::TidyBreak;

	ostream
		<< Generator::IncIndent
		<< Generator::Indent
		<< "<head>"
		<< Generator::TidyBreak
		<< Generator::IncIndent;

	// <head>

	for (const auto & component : getComponents ())
	{
		ostream
			<< XMLEncode (component)
			<< Generator::TidyBreak;
	}

	for (const auto & unit : getUnits ())
	{
		if (unit .getConversionFactor () not_eq 1)
		{
			ostream
				<< XMLEncode (unit)
				<< Generator::TidyBreak;
		}
	}

	for (const auto & metaData : getMetaDatas ())
	{
		ostream
			<< Generator::Indent
			<< "<meta"
			<< Generator::Space
			<< "name='"
			<< XMLEncode (metaData .first)
			<< "'"
			<< Generator::Space
			<< "content='"
			<< XMLEncode (metaData .second)
			<< "'"
			<< "/>"
			<< Generator::TidyBreak;
	}

	// </head>

	ostream
		<< Generator::DecIndent
		<< Generator::Indent
		<< "</head>"
		<< Generator::TidyBreak
		<< Generator::Indent
		<< "<Scene>"
		<< Generator::TidyBreak
		<< Generator::IncIndent;

	// <Scene>

	Generator::PushScene (ostream, this);
	Generator::EnterScope (ostream);
	Generator::ExportedNodes (ostream, getExportedNodes ());

	X3DExecutionContext::toXMLStream (ostream);

	for (const auto & exportedNode : getExportedNodes ())
	{
		try
		{
			ostream
				<< XMLEncode (exportedNode .second)
				<< Generator::TidyBreak;
		}
		catch (const X3DError &)
		{ }
	}

	Generator::LeaveScope (ostream);
	Generator::PopScene (ostream);

	// </Scene>

	ostream
		<< Generator::DecIndent
		<< Generator::Indent
		<< "</Scene>"
		<< Generator::TidyBreak
		<< Generator::DecIndent
		<< "</X3D>"
		<< Generator::TidyBreak
		<< std::flush;
}

void
X3DScene::toJSONStream (std::ostream & ostream) const
{
	ostream .imbue (std::locale::classic ());

	auto specificationVersion = getSpecificationVersion ();

	if (specificationVersion == VRML_V2_0)
		specificationVersion = LATEST_VERSION;

	Generator::SpecificationVersion (ostream, specificationVersion);

	Generator::PushScene (ostream, this);
	Generator::EnterScope (ostream);
	Generator::ExportedNodes (ostream, getExportedNodes ());


	// X3D

	ostream
		<< '{'
		<< Generator::TidySpace
		<< '"'
		<< "X3D"
		<< '"'
		<< ':'
		<< Generator::TidySpace
		<< '{'
		<< Generator::TidyBreak
		<< Generator::IncIndent
		<< Generator::IncIndent;


	// Encoding

	ostream
		<< Generator::Indent
		<< '"'
		<< "encoding"
		<< '"'
		<< ':'
		<< Generator::TidySpace
		<< '"'
		<< "UTF-8"
		<< '"'
		<< ','
		<< Generator::TidyBreak;


	// Profile

	ostream
		<< Generator::Indent
		<< '"'
		<< "@profile"
		<< '"'
		<< ':'
		<< Generator::TidySpace
		<< '"'
		<< (getProfile () ? getProfile () -> getName () : "Full")
		<< '"'
		<< ','
		<< Generator::TidyBreak;


	// Version

	ostream
		<< Generator::Indent
		<< '"'
		<< "@version"
		<< '"'
		<< ':'
		<< Generator::TidySpace
		<< '"'
		<< XMLEncode (specificationVersion)
		<< '"'
		<< ','
		<< Generator::TidyBreak;


	// XSD noNamespaceSchemaLocation

	ostream
		<< Generator::Indent
		<< '"'
		<< "@xsd:noNamespaceSchemaLocation"
		<< '"'
		<< ':'
		<< Generator::TidySpace
		<< '"'
		<< "http://www.web3d.org/specifications/x3d-3.3.xsd"
		<< '"'
		<< ','
		<< Generator::TidyBreak;


	// JSON schema

	ostream
		<< Generator::Indent
		<< '"'
		<< "JSON schema"
		<< '"'
		<< ':'
		<< Generator::TidySpace
		<< '"'
		<< "http://www.web3d.org/specifications/x3d-3.3-JSONSchema.json"
		<< '"'
		<< ','
		<< Generator::TidyBreak;


	// Head

	bool outputUnits = false;

	for (const auto & unit : getUnits ())
	{
		if (unit .getConversionFactor () not_eq 1)
			outputUnits = true;
	}

	if (not getMetaDatas () .empty () or not getComponents () .empty () or outputUnits)
	{
		bool headLastProperty = false;
	
		ostream
			<< Generator::Indent
			<< '"'
			<< "head"
			<< '"'
			<< ':'
			<< Generator::TidySpace
			<< '{'
			<< Generator::TidyBreak
			<< Generator::IncIndent;


		// Meta data

		if (not getMetaDatas () .empty ())
		{
			if (headLastProperty)
			{
				ostream
					<< ','
					<< Generator::TidyBreak;
			}
	
			// Meta data begin
		
			ostream
				<< Generator::Indent
				<< '"'
				<< "meta"
				<< '"'
				<< ':'
				<< Generator::TidySpace
				<< '['
				<< Generator::TidyBreak
				<< Generator::IncIndent;
		
		
			// Meta data
		
			for (const auto & metaData : getMetaDatas ())
			{
				ostream
					<< Generator::Indent
					<< '{'
					<< Generator::TidyBreak
					<< Generator::IncIndent;
		
				ostream
					<< Generator::Indent
					<< '"'
					<< "@name"
					<< '"'
					<< ':'
					<< Generator::TidySpace
					<< JSONEncode (SFString (metaData .first))
					<< ','
					<< Generator::TidyBreak;
		
				ostream
					<< Generator::Indent
					<< '"'
					<< "@content"
					<< '"'
					<< ':'
					<< Generator::TidySpace
					<< JSONEncode (SFString (metaData .second))
					<< Generator::TidyBreak;
		
				ostream
					<< Generator::DecIndent
					<< Generator::Indent
					<< '}';
		
				if (&metaData not_eq &*--getMetaDatas () .end ())
					ostream << ',';
		
				ostream << Generator::TidyBreak;
			}
		
		
			// Meta data end
		
			ostream
				<< Generator::DecIndent
				<< Generator::Indent
				<< ']';

			headLastProperty = true;
		}


		// Components

		if (not getComponents () .empty ())
		{
			if (headLastProperty)
			{
				ostream
					<< ','
					<< Generator::TidyBreak;
			}


			// Components begin
				
			ostream
				<< Generator::Indent
				<< '"'
				<< "component"
				<< '"'
				<< ':'
				<< Generator::TidySpace
				<< '['
				<< Generator::TidyBreak
				<< Generator::IncIndent;


			// Components

			for (const auto & component : getComponents ())
			{
				ostream << Generator::Indent;

				component -> toJSONStream (ostream);

				if (component not_eq getComponents () .back ())
					ostream << ',';
		
				ostream << Generator::TidyBreak;
			}


			// Components end
		
			ostream
				<< Generator::DecIndent
				<< Generator::Indent
				<< ']';

			headLastProperty = true;
		}


		// Units

		if (outputUnits)
		{
			if (headLastProperty)
			{
				ostream
					<< ','
					<< Generator::TidyBreak;
			}
	

			// Units begin
				
			ostream
				<< Generator::Indent
				<< '"'
				<< "unit"
				<< '"'
				<< ':'
				<< Generator::TidySpace
				<< '['
				<< Generator::TidyBreak
				<< Generator::IncIndent;


			// Units

			for (const auto & unit : getUnits ())
			{
				if (unit .getConversionFactor () not_eq 1)
				{
					ostream << Generator::Indent;

					unit .toJSONStream (ostream);

					if (&unit not_eq &getUnits () .back ())
						ostream << ',';
			
					ostream << Generator::TidyBreak;
				}
			}


			// Unit end
		
			ostream
				<< Generator::DecIndent
				<< Generator::Indent
				<< ']';

			headLastProperty = true;
		}	
	
	
		// Head end
	
		ostream
			<< Generator::TidyBreak
			<< Generator::DecIndent
			<< Generator::Indent
			<< '}'
			<< ','
			<< Generator::TidyBreak;
	}

	// Scene

	ostream
		<< Generator::Indent
		<< '"'
		<< "Scene"
		<< '"'
		<< ':'
		<< Generator::TidySpace
		<< '{'
		<< Generator::TidyBreak
		<< Generator::IncIndent
		<< Generator::Indent
		<< '"'
		<< "-children"
		<< '"'
		<< ':'
		<< Generator::TidySpace
		<< '['
		<< Generator::TidyBreak
		<< Generator::IncIndent;

	X3DExecutionContext::toJSONStream (ostream);

	bool lastProperty = false;

	lastProperty |= not getExternProtoDeclarations () .empty ();
	lastProperty |= not getProtoDeclarations ()       .empty ();
	lastProperty |= not getRootNodes ()               .empty ();
	lastProperty |= not getImportedNodes ()           .empty ();
	lastProperty |= not getRoutes ()                  .empty ();

	// Exported nodes

	if (not getExportedNodes () .empty ())
	{
		std::vector <std::string> exportedNodes;

		for (const auto & exportedNode : getExportedNodes ())
		{
			try
			{
				std::ostringstream osstream;
	
				osstream << SetGenerator (ostream) << JSONEncode (exportedNode .second);
	
				exportedNodes .emplace_back (osstream .str ());
			}
			catch (const X3DError &)
			{ }
		}

		if (not exportedNodes .empty ())
		{
			if (lastProperty)
			{
				ostream
					<< ','
					<< Generator::TidyBreak;
			}
	
			for (const auto & exportedNode : exportedNodes)
			{
				ostream
					<< Generator::Indent
					<< exportedNode;
		
				if (&exportedNode not_eq &exportedNodes .back ())
				{
					ostream
						<< ','
						<< Generator::TidyBreak;
				}
			}
	
			lastProperty = true;
		}
	}


	// Scene end

	ostream
		<< Generator::TidyBreak
		<< Generator::DecIndent
		<< Generator::Indent
		<< ']'
		<< Generator::TidyBreak
		<< Generator::DecIndent
		<< Generator::Indent
		<< '}';


	// X3D end

	ostream
		<< Generator::TidyBreak
		<< Generator::DecIndent
		<< Generator::Indent
		<< '}'
		<< Generator::TidyBreak
		<< Generator::DecIndent
		<< Generator::Indent
		<< '}'
		<< Generator::TidyBreak;

	Generator::LeaveScope (ostream);
	Generator::PopScene (ostream);
}

void
X3DScene::dispose ()
{
	metadatas     .clear ();
	exportedNodes .clear ();

	fileMonitor .reset ();

	X3DExecutionContext::dispose ();
	X3DBaseNode::dispose ();

	removeChildObjects (getRootNodes ());
}
 
X3DScene::~X3DScene ()
{
	__LOG__ << getWorldURL () << std::endl;

	trimFreeMemory ();
}

} // X3D
} // titania
