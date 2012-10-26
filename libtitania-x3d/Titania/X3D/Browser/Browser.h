/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
/*******************************************************************************
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

#ifndef __TITANIA_X3D_BROWSER_BROWSER_H__
#define __TITANIA_X3D_BROWSER_BROWSER_H__

#include "../Browser/X3DBrowser.h"
#include "../Types/Geometry.h"

#include <list>
#include <set>

namespace titania {
namespace X3D {

class Browser :
	public X3DBrowser
{
public:

	using X3DBrowser::getComponent;

	void
	pushRenderer (X3DRenderer* renderer) { rendererStack .push_back (renderer); }

	void
	popRenderer () { rendererStack .pop_back (); }

	X3DRenderer*
	getCurrentRenderer () { return rendererStack .back (); }

	const X3DRenderer*
	getCurrentRenderer () const { return rendererStack .back (); }

	////
	void
	pick (const size_t, const size_t);

	void
	pushSensitiveNode (X3DBasicNode* node) { return sensitiveNodes .push_back (node); }

	void
	popSensitiveNode () { return sensitiveNodes .pop_back (); }

	const std::vector <X3DBasicNode*> &
	getSensitiveNodes () const { return sensitiveNodes; }

	bool
	isSensitive () const { return sensitiveNodes .size (); }

	Line3f
	getHitRay () const;

	void
	addHit (Hit* hit) { hits .push_back (hit); }

	const HitArray &
	getHits () const { return hits; }

	////

	//	void
	//	setEditMode (const bool value) { editMode = value; update (); }
	//
	//	bool
	//	getEditMode () const { return editMode; }

	virtual
	void
	replaceWorld (const SFNode <Scene> &)
	throw (Error <INVALID_SCENE>);

	virtual
	~Browser ();


protected:

	Browser ();

	friend SFNode <Browser>
	createBrowser ()
	throw (Error <BROWSER_UNAVAILABLE>);


private:

	virtual
	X3DBasicNode*
	create (X3DExecutionContext* const) const;

	virtual
	void
	initialize ();

	void
	setXY (const size_t, const size_t);

	void
	selectionBegin ();

	void
	selectionEnd ();

	void
	clearHits ();

	typedef std::vector <X3DRenderer*>     RendererStack;

	RendererStack rendererStack;

	float objectFrontAlpha;
	float objectBackAlpha;

	size_t x;
	size_t y;

	std::vector <X3DBasicNode*> sensitiveNodes;
	HitArray                    hits;
	HitComp                     hitComp;

	//	bool             editMode;

};

} // X3D
} // titania

#endif
