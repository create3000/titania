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

#ifndef __TITANIA_X3D_BITS_GOLDEN_GATE_H__
#define __TITANIA_X3D_BITS_GOLDEN_GATE_H__

#include "../Execution/X3DScene.h"

#include <Titania/Basic/URI.h>
#include <Titania/Stream/InputFileStream.h>

#include <regex>
#include <string>

namespace titania {
namespace X3D {

class GoldenGate
{
public:

	///  @name Construction

	GoldenGate () = delete;

	///  @name Member access

	static
	std::vector <std::string>
	getContentTypes ();

	///  @name Operations

	static
	void
	read (basic::ifilestream & istream, const X3DScenePtr & scene, const basic::uri & worldURL);

	///  @name Destruction

	virtual
	~GoldenGate () = delete;


private:

	///  @name Member types

	using GoldenFunction = std::function <void (const X3DScenePtr &, const basic::uri &, basic::ifilestream &)>;

	class GoldenParser;

	///  @name Operations

	static
	std::map <std::string, GoldenGate::GoldenFunction>
	getContentTypeFunctions ();

	static
	std::map <std::string, GoldenGate::GoldenFunction>
	getExtensions ();

	static
	void
	vrml (const X3DScenePtr & scene, const basic::uri & uri, basic::ifilestream & istream);
	
	static
	void
	text (const X3DScenePtr & scene, const basic::uri & uri, basic::ifilestream & istream);
	
	static
	void
	image (const X3DScenePtr & scene, const basic::uri & uri, basic::ifilestream & istream);
	
	static
	void
	audio (const X3DScenePtr & scene, const basic::uri & uri, basic::ifilestream & istream);
	
	static
	void
	video (const X3DScenePtr & scene, const basic::uri & uri, basic::ifilestream & istream);

	static
	bool
	isVRML1 (std::istream & istream);

	///  @name Static members

	static const std::regex Name;
	static const std::regex Description;
	static const std::regex Width;
	static const std::regex Height;
	static const std::regex URL;

};

} // X3D
} // titania

#endif
