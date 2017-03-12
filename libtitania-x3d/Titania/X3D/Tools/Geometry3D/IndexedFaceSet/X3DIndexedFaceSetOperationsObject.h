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

#ifndef __TITANIA_X3D_TOOLS_GEOMETRY3D_X3DINDEXED_FACE_SET_OPERATIONS_OBJECT_H__
#define __TITANIA_X3D_TOOLS_GEOMETRY3D_X3DINDEXED_FACE_SET_OPERATIONS_OBJECT_H__

#include "X3DIndexedFaceSetSelectionObject.h"

namespace titania {
namespace X3D {

class X3DIndexedFaceSetOperationsObject :
	virtual public X3DIndexedFaceSetSelectionObject

{
public:

	///  @name Hidden fields

	SFTime &
	cutGeometry ()
	{ return *fields .cutGeometry; }

	const SFTime &
	cutGeometry () const
	{ return *fields .cutGeometry; }

	SFTime &
	copyGeometry ()
	{ return *fields .copyGeometry; }

	const SFTime &
	copyGeometry () const
	{ return *fields .copyGeometry; }

	SFString &
	pasteGeometry ()
	{ return *fields .pasteGeometry; }

	const SFString &
	pasteGeometry () const
	{ return *fields .pasteGeometry; }

	SFTime &
	mergePoints ()
	{ return *fields .mergePoints; }

	const SFTime &
	mergePoints () const
	{ return *fields .mergePoints; }

	SFTime &
	splitPoints ()
	{ return *fields .splitPoints; }

	const SFTime &
	splitPoints () const
	{ return *fields .splitPoints; }

	SFTime &
	formNewFace ()
	{ return *fields .formNewFace; }

	const SFTime &
	formNewFace () const
	{ return *fields .formNewFace; }

	SFTime &
	extrudeSelectedEdges ()
	{ return *fields .extrudeSelectedEdges; }

	const SFTime &
	extrudeSelectedEdges () const
	{ return *fields .extrudeSelectedEdges; }

	SFTime &
	extrudeSelectedFaces ()
	{ return *fields .extrudeSelectedFaces; }

	const SFTime &
	extrudeSelectedFaces () const
	{ return *fields .extrudeSelectedFaces; }

	SFTime &
	chipOfSelectedFaces ()
	{ return *fields .chipOfSelectedFaces; }

	const SFTime &
	chipOfSelectedFaces () const
	{ return *fields .chipOfSelectedFaces; }

	SFTime &
	flipVertexOrdering ()
	{ return *fields .flipVertexOrdering; }

	const SFTime &
	flipVertexOrdering () const
	{ return *fields .flipVertexOrdering; }

	SFTime &
	deleteSelectedFaces ()
	{ return *fields .deleteSelectedFaces; }

	const SFTime &
	deleteSelectedFaces () const
	{ return *fields .deleteSelectedFaces; }

	SFString &
	clipboard_changed ()
	{ return *fields .clipboard_changed; }

	const SFString &
	clipboard_changed () const
	{ return *fields .clipboard_changed; }

	///  @name Destruction

	virtual
	void
	dispose ()
	{ }

	virtual
	~X3DIndexedFaceSetOperationsObject () override;


protected:

	///  @name Construction

	X3DIndexedFaceSetOperationsObject ();

	virtual
	void
	initialize () override;


private:

	///  @name Event handlers

	void
	set_cutGeometry ();

	void
	set_copyGeometry ();

	void
	set_pasteGeometry ();

	void
	set_mergePoints ();

	void
	set_splitPoints ();

	void
	set_formNewFace ();

	void
	set_extrudeSelectedEdges ();

	void
	set_extrudeSelectedFaces ();

	void
	set_chipOfSelectedFaces ();

	void
	set_flipVertexOrdering ();

	void
	set_deleteSelectedFaces ();

	void
	deleteSelectedFaces (const UndoStepPtr &);

	///  @name Operations

	std::vector <int32_t>
	splitPoints (const std::vector <int32_t> &);

	std::vector <int32_t>
	formNewFace (const std::vector <std::vector <int32_t>> &);

	std::vector <int32_t>
	extrudeSelectedEdges (const std::vector <std::pair <size_t, size_t>> &, const std::set <size_t> &);

	std::vector <int32_t>
	chipOf (const std::vector <size_t> &);

	void
	flipVertexOrdering (const std::set <size_t> &);

	void
	erasePoints (const std::vector <int32_t> &);

	///  @name Members

	struct Fields
	{
		Fields ();

		SFTime* const cutGeometry;
		SFTime* const copyGeometry;
		SFString* const pasteGeometry;
		SFTime* const mergePoints;
		SFTime* const splitPoints;
		SFTime* const formNewFace;
		SFTime* const extrudeSelectedEdges;
		SFTime* const extrudeSelectedFaces;
		SFTime* const chipOfSelectedFaces;
		SFTime* const flipVertexOrdering;
		SFTime* const deleteSelectedFaces;
		SFString* const clipboard_changed;
	};

	Fields fields;

};

} // X3D
} // titania

#endif
