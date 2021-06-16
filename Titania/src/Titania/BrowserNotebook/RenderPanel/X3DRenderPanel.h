/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraﬂe 31a, Leipzig, Germany 2011.
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

#ifndef __TITANIA_BROWSER_NOTEBOOK_RENDER_X3DPANEL_RENDER_PANEL_H__
#define __TITANIA_BROWSER_NOTEBOOK_RENDER_X3DPANEL_RENDER_PANEL_H__

#include "../../UserInterfaces/X3DRenderPanelInterface.h"

namespace titania {
namespace puck {

class X3DRenderPanel :
	virtual public X3DRenderPanelInterface
{
public:

	///  @name Destruction

	virtual
	~X3DRenderPanel () override;


protected:

	///  @name Construction

	X3DRenderPanel ();

	///  @name Member access

	void
	setFilename (const size_t id, const basic::uri & filename);
	
	basic::uri
	getFilename (const size_t id, const basic::uri & filename) const;

	void
	setCodec (const size_t id, const std::string & codec);

	std::string
	getCodec (const size_t id, const std::string & codec) const;

	void
	setDuration (const size_t id, const int32_t duration);
	
	size_t
	getDuration (const size_t id, const int32_t duration) const;
	
	void
	setFrameRate (const size_t id, const int32_t frameRate);
	
	size_t
	getFrameRate (const size_t id, const int32_t frameRate) const;
	
	void
	setWidth (const size_t id, const int32_t width);
	
	size_t
	getWidth (const size_t id, const int32_t width) const;
	
	void
	setHeight (const size_t id, const int32_t height);
	
	size_t
	getHeight (const size_t id, const int32_t height) const;
	
	void
	setAntialiasing (const size_t id, const int32_t antialiasing);
	
	size_t
	getAntialiasing (const size_t id, const int32_t antialiasing) const;

	void
	setShading (const size_t id, const std::string & shading);

	std::string
	getShading (const size_t id, const std::string & shading) const;

	void
	setViewpoint (const size_t id, const std::string & viewpoint);
	
	std::string
	getViewpoint (const size_t id, const std::string & viewpoint) const;

};

} // puck
} // titania

#endif
