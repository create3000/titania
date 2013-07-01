/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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
 * For Silvio, Joy and Adi.
 *
 ******************************************************************************/

#include "MFNode.h"

namespace titania {
namespace X3D {

template <>
const std::string X3DField <Array <SFNode <X3DBaseNode>>>::typeName ("MFNode");

template <>
const X3DConstants::FieldType X3DField <Array <SFNode <X3DBaseNode>>>::type = X3DConstants::MFNode;

//template class Array <X3D::SFNode <X3DBaseNode>>;
template class X3DField <Array <SFNode <X3DBaseNode>>>;
template class X3DArrayField <SFNode <X3DBaseNode>>;

MFNode*
MFNode::clone (X3DExecutionContext* const executionContext) const
{
	MFNode* field = new MFNode ();

	clone (executionContext, field);

	return field;
}

void
MFNode::clone (X3DExecutionContext* const executionContext, X3DFieldDefinition* fieldDefinition) const
{
	MFNode* field = static_cast <MFNode*> (fieldDefinition);

	for (const auto & value :* this)
	{
		if (value)
		{
			try
			{
				field -> emplace_back (value -> clone (executionContext));
			}
			catch (const Error <INVALID_NAME> &)
			{
				field -> emplace_back (value -> copy (executionContext));
			}
		}
		else
			field -> emplace_back ();
	}
}

void
MFNode::fromStream (std::istream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{ }

void
MFNode::toStream (std::ostream & ostream) const
{
	if (size () > 1)
	{
		Generator::PushContext ();

		ostream
			<< '['
			<< Generator::TidyBreak
			<< Generator::IncIndent;

		for (const auto & field : basic::adapter (cbegin (), cend () - 1))
		{
			ostream
				<< Generator::Indent
				<< field
				<< Generator::TidyBreak;
		}

		ostream
			<< Generator::Indent
			<< back ()
			<< Generator::TidyBreak
			<< Generator::DecIndent
			<< Generator::Indent
			<< ']';

		Generator::PopContext ();

		return;
	}

	if (size () == 1)
	{
		Generator::PushContext ();
		ostream << front ();
		Generator::PopContext ();

		return;
	}

	ostream << Generator::EmptyBrackets;
}

} // X3D
} // titania
