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

#include "Proto.h"

#include "../Components/Core/X3DPrototypeInstance.h"
#include "ExternProto.h"
#include <iomanip>

namespace titania {
namespace X3D {

Proto::Proto (X3DExecutionContext* const executionContext) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext), 
	           X3DProto (),                                                    
	X3DExecutionContext ()                                                     
{
	setTypeName ("Proto");

	addField (inputOutput, "metadata", metadata ());

	setWorldURL (executionContext -> getWorldURL ());
}

X3DBaseNode*
Proto::create (X3DExecutionContext* executionContext) const
{
	return new Proto (executionContext);
}

X3DPrototypeInstance*
Proto::createInstance (bool setup)
{
	X3DPrototypeInstance* instance = new X3DPrototypeInstance (getExecutionContext (), this);

	if (setup)
		instance -> setup ();

	return instance;
}

X3DBaseNode*
Proto::getRootNode () const
{
	return getRootNodes () .front ();
}

void
Proto::toStream (std::ostream & ostream) const
{
	Generator::PushLevel ();

	if (getComments () .size ())
	{
		for (const auto & comment : getComments ())
		{
			ostream
				<< Generator::Indent
				<< Generator::Comment
				<< comment
				<< Generator::Break;
		}
	
		ostream << Generator::TidyBreak;
	}

	ostream
		<< Generator::Indent
		<< "PROTO"
		<< Generator::Space
		<< getName ()
		<< Generator::TidySpace
		<< '[';

	FieldDefinitionArray fields = getUserDefinedFields ();

	if (fields .size ())
	{
		ostream
			<< Generator::TidyBreak
			<< Generator::IncIndent;

		size_t fieldTypeLength  = 0;
		size_t accessTypeLength = 0;

		if (Generator::Style () not_eq "clean")
		{
			for (const auto & field : fields)
			{
				fieldTypeLength = std::max (fieldTypeLength, field -> getTypeName () .length ());

				accessTypeLength = std::max (accessTypeLength, Generator::AccessTypes [field] .length ());
			}
		}

		for (const auto & field : fields)
		{
			for (const auto & comment : field -> getComments ())
			{
				ostream
					<< Generator::Indent
					<< Generator::Comment
					<< comment
					<< Generator::Break;
			}
		
			ostream
				<< Generator::Indent
				<< std::setiosflags (std::ios::left)
				<< std::setw (accessTypeLength);

			ostream << Generator::AccessTypes [field];

			ostream
				<< Generator::Space
				<< std::setiosflags (std::ios::left) << std::setw (fieldTypeLength) << field -> getTypeName ()
				<< Generator::Space
				<< field -> getName ();

			if (field -> isInitializeable ())
			{
				ostream
					<< Generator::Space
					<< *field;
			}

			ostream << Generator::Break;
		}
		
		for (const auto & comment : getInterfaceComments ())
		{
			ostream
				<< Generator::Indent
				<< Generator::Comment
				<< comment
				<< Generator::Break;
		}
		
		ostream
			<< Generator::DecIndent
			<< Generator::Indent;
	}
	else
	{
		if (getInterfaceComments () .size ())
		{
			ostream
				<< Generator::TidyBreak
				<< Generator::IncIndent;
				
			for (const auto & comment : getInterfaceComments ())
			{
				ostream
					<< Generator::Indent
					<< Generator::Comment
					<< comment
					<< Generator::Break;
			}
	
			ostream
				<< Generator::DecIndent
				<< Generator::Indent;
		}
	
		else
			ostream << Generator::TidySpace;
	}

	ostream << ']';

	Generator::PopLevel ();

	ostream << Generator::TidyBreak;

	ostream
		<< Generator::Indent
		<< '{'
		<< Generator::TidyBreak
		<< Generator::IncIndent;

	X3DExecutionContext::toStream (ostream);

	ostream
		<< Generator::DecIndent
		<< Generator::Indent
		<< '}';
}

} // X3D
} // titania
