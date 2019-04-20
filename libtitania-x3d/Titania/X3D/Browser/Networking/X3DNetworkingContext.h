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

#ifndef __TITANIA_X3D_BROWSER_NETWORKING_X3DNETWORKING_CONTEXT_H__
#define __TITANIA_X3D_BROWSER_NETWORKING_X3DNETWORKING_CONTEXT_H__

#include "../../Basic/X3DBaseNode.h"
#include "../../Types/Pointer.h"

#include <deque>
#include <mutex>
#include <atomic>

namespace titania {
namespace X3D {

class X3DNetworkingContext :
	virtual public X3DBaseNode
{
public:

	///  @name Member access

	const std::string &
	getProviderUrl () const
	{ return providerUrl; }

	const std::string &
	getUserAgent () const
	{ return userAgent; }

	void
	setLoadUrlObjects (const bool value)
	{ loadUrlObjects = value; }

	const SFBool &
	getLoadUrlObjects () const
	{ return loadUrlObjects; }

	const SFInt32 &
	getLoadingTotal () const
	{ return loadingTotal; }

	const SFInt32 &
	getLoadCount () const
	{ return loadCount; }

	void
	setNotifyOnLoad (const bool value)
	{ notifyOnLoad = value; }

	const SFBool &
	getNotifyOnLoad () const
	{ return notifyOnLoad; }

	void
	setMonitorFiles (const bool value)
	{ monitorFiles = value; }

	const SFBool &
	getMonitorFiles () const
	{ return monitorFiles; }

	void
	addLoadCount (const void* const object);

	void
	removeLoadCount (const void* const object);

	///  @name Destruction

	virtual
	void
	dispose () override;
	
	virtual
	~X3DNetworkingContext () override;


protected:

	///  @name Fiends

	friend class SceneFuture;
	friend class TextureFuture;
	friend class Texture3DFuture;

	friend class ExternProtoDeclaration;
	friend class GeoLOD;
	friend class Inline;
	friend class Notification;
	friend class RenderingProperties;
	friend class X3DEnvironmentalEffectsContext;
	friend class X3DShadersContext;
	friend class X3DUrlObject;
	friend class X3DViewpointNode;

	friend class X3DGeometricPropertyNodeTool;
	friend class X3DToolObject;

	///  @name Construction

	X3DNetworkingContext ();

	virtual
	void
	initialize () override;

	///  @name Member access

	const X3DScenePtr &
	getPrivateScene () const
	{ return privateScene; }

	const std::shared_ptr <std::mutex> &
	getDownloadMutex ();

	void
	resetLoadCount ();


private:

	///  @name Event handlers

	void
	set_loadCount ();

	///  @name Static Members

	static const std::string providerUrl;

	///  @name Members

	std::string userAgent;

	X3DScenePtr                               privateScene;
	size_t                                    downloadMutexIndex;
	std::deque <std::shared_ptr <std::mutex>> downloadMutexes;
	std::mutex                                downloadMutex;
	std::set <const void*>                    loadingObjects;
	SFInt32                                   loadingTotal;
	SFInt32                                   loadCount;
	SFBool                                    loadUrlObjects;
	SFBool                                    notifyOnLoad;
	SFBool                                    monitorFiles;

};

} // X3D
} // titania

#endif
