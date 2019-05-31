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

#ifndef __TITANIA_BASE_X3DBASE_INTERFACE_H__
#define __TITANIA_BASE_X3DBASE_INTERFACE_H__

#include <Titania/X3D.h>
#include <Titania/X3D/Base/X3DEventObject.h>
#include <Titania/X3D/Editing/Undo/UndoStep.h>

#include <Titania/X3D/Components/Core/WorldInfo.h>
#include <Titania/X3D/Components/Layering/LayerSet.h>
#include <Titania/X3D/Components/Layering/X3DLayerNode.h>
#include <Titania/X3D/Execution/World.h>

#include <sigc++/trackable.h>

namespace titania {
namespace puck {

class X3DBrowserWindow;

class X3DBaseInterface :
	virtual public sigc::trackable,
	public X3D::X3DEventObject
{
public:

	/***
	 *  @name Construction
	 */

	virtual
	void
	setup () override;

	/// @name Common members

	///  Returns the type name of this object.
	///  throws X3D::Error <X3D::DISPOSED>
	virtual
	const std::string &
	getTypeName () const override
	{ return typeName; }

	/// @name Member access

	X3DBrowserWindow*
	getBrowserWindow () const
	{ return browserWindow; }

	virtual
	const X3D::BrowserPtr &
	getMasterBrowser () const;

	virtual
	const X3D::BrowserPtr &
	getCurrentBrowser () const;

	const X3D::WorldPtr &
	getCurrentWorld () const;

	virtual
	const X3D::X3DScenePtr &
	getCurrentScene () const;

	virtual
	void
	setCurrentContext (const X3D::X3DExecutionContextPtr &);

	virtual
	const X3D::X3DExecutionContextPtr &
	getCurrentContext () const;

	bool
	inProtoDeclaration () const;

	bool
	inPrototypeInstance () const;

	/***
	 *  @name World Info handling
	 */

	void
	setAddStandardMetadata (const X3D::X3DScenePtr & scene, const bool value);

	bool
	getAddStandardMetadata (const X3D::X3DScenePtr & scene);

	///  throws X3D::Error <X3D::NOT_SUPPORTED>
	X3D::X3DPtr <X3D::WorldInfo>
	createWorldInfo (const X3D::X3DScenePtr & scene);

	///  throws X3D::Error <X3D::NOT_SUPPORTED>
	X3D::X3DPtr <X3D::WorldInfo>
	getWorldInfo (const X3D::X3DScenePtr & scene) const;

	/***
	 *  @name Meta data
	 */

	template <class Type>
	void
	setMetaData (const X3D::BrowserPtr & browser, const std::string & key, const Type & value);

	template <class Type>
	Type
	getMetaData (const X3D::BrowserPtr & browser, const std::string & key, const Type & defaultValue = Type ()) const;

	/***
	 *  @name Node, Proto and Context path handline
	 */

	X3D::X3DExecutionContext*
	getSelectionContext (const X3D::MFNode & nodes, const bool currentIfEmpty = false) const;

	std::string
	getNodeName (const X3D::SFNode & node) const;

	std::deque <std::string>
	getNodePath (const X3D::SFNode & node) const;

	std::deque <std::string>
	getProtoPath (X3D::X3DExecutionContext* executionContext) const;

	std::deque <std::string>
	getContextPath (X3D::X3DExecutionContext* executionContext) const;

	std::string
	getPathFromContext (const X3D::X3DExecutionContextPtr & executionContext) const;

	X3D::X3DExecutionContextPtr
	getContextFromPath (X3D::X3DExecutionContext* executionContext, const std::string & string) const;

	/***
	 *  @name Undo/redo handling
	 */

	virtual
	void
	addUndoStep (const X3D::UndoStepPtr &);

	virtual
	void
	removeUndoStep ();

	virtual
	const X3D::UndoStepPtr &
	getUndoStep () const;

	/// @name Input/Output

	///  Extracts the value for this object from @a istream. The contents of @a istream must be in VRML Classic Encoding style.
	virtual
	void
	fromStream (std::istream &) final override
	{ }

	///  Inserts this object into @a ostream in VRML Classic Encoding style.
	virtual
	void
	toStream (std::ostream &) const final override
	{ }

	///  Inserts this object into @a ostream in X3D XML Encoding style.
	virtual
	void
	toXMLStream (std::ostream &) const final override
	{ }

	///  Inserts this object into @a ostream in X3D XML Encoding style.
	virtual
	void
	toJSONStream (std::ostream &) const final override
	{ }

	///  @name Destruction

	virtual
	void
	dispose () override;

	virtual
	~X3DBaseInterface () override;


protected:

	/// @name Construction

	X3DBaseInterface ();

	X3DBaseInterface (X3DBrowserWindow* const, X3D::Browser* const);

	/// @name Member access

	void
	setTypeName (const std::string & value)
	{ typeName = value; }

	/// @name Operations

	///  throws X3D::Error <X3D::NOT_SUPPORTED>
	X3D::X3DPtr <X3D::WorldInfo>
	getWorldInfo (const X3D::X3DScenePtr & scene, const bool create);


private:

	/// @name Event handlers

	void
	set_browser (const X3D::BrowserPtr & value);

	/// @name Members

	std::string             typeName;
	X3DBrowserWindow* const browserWindow;

};

template <class Type>
void
X3DBaseInterface::setMetaData (const X3D::BrowserPtr & browser, const std::string & key, const Type & value)
{
	const auto & layerSet = browser -> getWorld () -> getLayerSet ();

	if (layerSet -> getActiveLayer () and layerSet -> getActiveLayer () not_eq layerSet -> getLayer0 ())
	{
		layerSet -> getActiveLayer () -> setMetaData (key, value);
	}
	else
	{
		const auto scene = browser -> getExecutionContext () -> isType ({ X3D::X3DConstants::X3DScene })
		                   ? X3D::X3DScenePtr (browser -> getExecutionContext ())
		                   : X3D::X3DScenePtr (browser -> getExecutionContext () -> getScene ());

		createWorldInfo (scene) -> setMetaData (key, value);
	}
}

template <class Type>
Type
X3DBaseInterface::getMetaData (const X3D::BrowserPtr & browser, const std::string & key, const Type & defaultValue) const
{
	try
	{
		const auto & layerSet = browser -> getWorld () -> getLayerSet ();

		if (layerSet -> getActiveLayer () and layerSet -> getActiveLayer () not_eq layerSet -> getLayer0 ())
			return layerSet -> getActiveLayer () -> getMetaData <Type> (key, defaultValue);

		const auto scene = browser -> getExecutionContext () -> isType ({ X3D::X3DConstants::X3DScene })
		                   ? X3D::X3DScenePtr (browser -> getExecutionContext ())
		                   : X3D::X3DScenePtr (browser -> getExecutionContext () -> getScene ());

		return getWorldInfo (scene) -> getMetaData <Type> (key, defaultValue);
	}
	catch (const X3D::X3DError &)
	{
		return defaultValue;
	}
}

} // puck
} // titania

#endif
