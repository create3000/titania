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

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_EXPRESSIONS_CONTROL_FLOW_EXCEPTION_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_EXPRESSIONS_CONTROL_FLOW_EXCEPTION_H__

#include "../Bits/Exception.h"
#include "../Primitives/var.h"

namespace titania {
namespace pb {

/**
 *  Base class to represent a control flow exception.
 */
class pbControlFlowException :
	public pbException
{
public:

	pbControlFlowException ()
	noexcept (true) :
		pbException ()
	{ }
	
	virtual
	const char*
	what () const
	noexcept (true) final override
	{ return "pbControlFlowException"; }

	virtual
	~pbControlFlowException ()
	noexcept (true)
	{ }

};

enum class ControlFlowExceptionType
{
	CONTINUE,
	LABELLED_CONTINUE,
	BREAK,
	LABELLED_BREAK,
	YIELD,
	RETURN,

};

/**
 *  Template to represent a control flow exception.
 */
template <ControlFlowExceptionType type>
class ControlFlowException :
	public pbControlFlowException
{
public:

	ControlFlowException ()
	noexcept (true) :
		pbControlFlowException ()
	{ }

	virtual
	~ControlFlowException ()
	noexcept (true)
	{ }

};

/**
 *  Class to represent a LabelledContinueException exception.
 */
class LabelledContinueException :
	public ControlFlowException <ControlFlowExceptionType::LABELLED_CONTINUE>
{
public:

	///  @name Construction

	LabelledContinueException (const size_t identifier)
	noexcept (true) :
		ControlFlowException <ControlFlowExceptionType::LABELLED_CONTINUE> (),
		                                                       identifier (identifier)
	{ }

	///  @name Member access

	size_t
	getIdentifier () const
	noexcept (true)
	{ return identifier; }


private:

	///  @name Members

	const size_t identifier;

};

/**
 *  Class to represent a LabelledBreakException exception.
 */
class LabelledBreakException :
	public ControlFlowException <ControlFlowExceptionType::LABELLED_BREAK>
{
public:

	///  @name Construction

	LabelledBreakException (const size_t identifier)
	noexcept (true) :
		ControlFlowException <ControlFlowExceptionType::LABELLED_BREAK> (),
		                                                     identifier (identifier)
	{ }

	///  @name Member access

	size_t
	getIdentifier () const
	noexcept (true)
	{ return identifier; }


private:

	///  @name Members

	const size_t identifier;

};

/**
 *  Class to represent a ReturnException exception.
 */
class ReturnException :
	public ControlFlowException <ControlFlowExceptionType::RETURN>
{
public:

	///  @name Construction

	ReturnException (var && value)
	noexcept (true) :
		ControlFlowException <ControlFlowExceptionType::RETURN> (),
		                                                  value (std::move (value))
	{ }

	///  @name Member access

	const var &
	getValue () const
	noexcept (true)
	{ return value; }


private:

	///  @name Members

	const var value;

};

using ContinueException = ControlFlowException <ControlFlowExceptionType::CONTINUE>;
using BreakException    = ControlFlowException <ControlFlowExceptionType::BREAK>;
using YieldException    = ControlFlowException <ControlFlowExceptionType::YIELD>;

} // pb
} // titania

#endif
