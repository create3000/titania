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

#include "jsFieldDefinitionArray.h"

#include "../../Basic/X3DArrayField.h"
#include "jsContext.h"

namespace titania {
namespace X3D {
namespace MozillaSpiderMonkey {

// Minimal X3DArrayField to wrap X3DFieldDefinitions

template <class ValueType>
class X3DConstArrayField :
	public X3DField <Array <ValueType>> 
{
public:

	typedef ValueType         value_type;
	typedef Array <ValueType> internal_type;

	typedef basic::reference_iterator <typename internal_type::iterator, ValueType>                     iterator;
	typedef basic::reference_iterator <typename internal_type::reverse_iterator, ValueType>             reverse_iterator;
	typedef basic::reference_iterator <typename internal_type::const_iterator, const ValueType>         const_iterator;
	typedef basic::reference_iterator <typename internal_type::const_reverse_iterator, const ValueType> const_reverse_iterator;

	typedef typename internal_type::difference_type difference_type;
	typedef typename internal_type::size_type       size_type;

	using X3DField <internal_type>::getValue;

	///  @name Construction

	///  Default constructor.
	X3DConstArrayField () :
		X3DField <internal_type> ()
	{ }

	template <class InputIterator>
	X3DConstArrayField (InputIterator first, InputIterator last) :
		X3DField <internal_type> ()
	{
		for (const auto & field : basic::adapter (first, last))
		{
			get () .emplace_back (field);

			field -> addParent (this);
		}
	}

	virtual
	X3DConstArrayField*
	create () const final override
	{ return new X3DConstArrayField (); }

	virtual
	X3DConstArrayField*
	clone () const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>) final override
	{ return new X3DConstArrayField (*this); }

	virtual
	X3DConstArrayField*
	clone (X3DExecutionContext* const) const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>) final override
	{ return clone (); }

	///  @name Tests

	virtual
	bool
	isArray () const final override
	{ return true; }

	const ValueType*
	at (const size_type index) const
	{ return getValue () .at (index); }

	size_type
	size () const
	{ return getValue () .size (); }

	///  @name Input/Output

	virtual
	void
	fromStream (std::istream &)
	throw (Error <INVALID_X3D>,
	       Error <NOT_SUPPORTED>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override
	{ }

	virtual
	void
	toStream (std::ostream &) const final override
	{ }

	virtual
	void
	toXMLStream (std::ostream &) const final override
	{ }

	///  @name Destruction

	virtual
	~X3DConstArrayField ()
	{ }


private:

	using X3DField <internal_type>::get;

	///  @name Element access

	virtual
	void
	set (const internal_type &) final override
	{ }

	virtual
	void
	reset () final override
	{
		for (auto & field : get ())
			field -> removeParent (this);

		get () .clear ();
	}

};

// Private type

using jsFieldDefinitionArrayPrivate = X3DConstArrayField <X3DFieldDefinition>;

// jsFieldDefinitionArray

JSClass jsFieldDefinitionArray::static_class = {
	"FieldDefinitionArray", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, get1Value, JS_StrictPropertyStub,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

JSPropertySpec jsFieldDefinitionArray::properties [ ] = {
	{ "length", LENGTH, JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, length, NULL },
	{ 0 }

};

JSFunctionSpec jsFieldDefinitionArray::functions [ ] = {
	{ 0, 0, 0, 0 }

};

void
jsFieldDefinitionArray::init (JSContext* const context, JSObject* const global)
{
	JS_InitClass (context, global, NULL, &static_class, NULL,
	              0, properties, functions, NULL, NULL);
}

JSBool
jsFieldDefinitionArray::create (JSContext* const context, const FieldDefinitionArray* const array, jsval* const vp)
{
	const auto javaScript = static_cast <jsContext*> (JS_GetContextPrivate (context));

	JSObject* const result = JS_NewObject (context, &static_class, NULL, NULL);

	if (result == NULL)
		return JS_FALSE;

	jsFieldDefinitionArrayPrivate* privateArray = nullptr;

	if (array)
		privateArray = new jsFieldDefinitionArrayPrivate (array -> begin (), array -> end ());

	else
		privateArray = new jsFieldDefinitionArrayPrivate ();

	privateArray -> addParent (javaScript);

	JS_SetPrivate (context, result, privateArray);

	*vp = OBJECT_TO_JSVAL (result);

	return JS_TRUE;
}

JSBool
jsFieldDefinitionArray::enumerate (JSContext* context, JSObject* obj, JSIterateOp enum_op, jsval* statep, jsid* idp)
{
	const auto array = static_cast <jsFieldDefinitionArrayPrivate*> (JS_GetPrivate (context, obj));

	if (not array)
	{
		*statep = JSVAL_NULL;
		return JS_TRUE;
	}

	size_t* index;

	switch (enum_op)
	{
		case JSENUMERATE_INIT:
		case JSENUMERATE_INIT_ALL:
		{
			index   = new size_t (0);
			*statep = PRIVATE_TO_JSVAL (index);

			if (idp)
				*idp = INT_TO_JSID (array -> size ());

			break;
		}
		case JSENUMERATE_NEXT:
		{
			index = (size_t*) JSVAL_TO_PRIVATE (*statep);

			if (*index < array -> size ())
			{
				if (idp)
					*idp = INT_TO_JSID (*index);

				*index = *index + 1;
				break;
			}

			//else done -- cleanup.
		}
		case JSENUMERATE_DESTROY:
		{
			index = (size_t*) JSVAL_TO_PRIVATE (*statep);
			delete index;
			*statep = JSVAL_NULL;
		}
	}

	return JS_TRUE;
}

JSBool
jsFieldDefinitionArray::get1Value (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	if (not JSID_IS_INT (id))
		return JS_TRUE;

	const int32 index = JSID_TO_INT (id);
	const auto  array = static_cast <jsFieldDefinitionArrayPrivate*> (JS_GetPrivate (context, obj));

	if (index < 0 and index >= (int32) array -> size ())
	{
		JS_ReportError (context, "index out of range");
		return JS_FALSE;
	}

	return jsX3DFieldDefinition::create (context, array -> at (index), vp);
}

JSBool
jsFieldDefinitionArray::length (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	const auto array = static_cast <jsFieldDefinitionArrayPrivate*> (JS_GetPrivate (context, obj));

	return JS_NewNumberValue (context, array -> size (), vp);
}

void
jsFieldDefinitionArray::finalize (JSContext* context, JSObject* obj)
{
	const auto javaScript = static_cast <jsContext*> (JS_GetContextPrivate (context));
	const auto array      = static_cast <jsFieldDefinitionArrayPrivate*> (JS_GetPrivate (context, obj));

	// Proto objects have no private

	if (array)
		array -> removeParent (javaScript);
}

} // MozillaSpiderMonkey
} // X3D
} // titania
