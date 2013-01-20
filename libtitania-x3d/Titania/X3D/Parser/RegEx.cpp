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

#include "RegEx.h"

namespace titania {
namespace X3D {

const pcrecpp::RE RegEx::Header      ("\\A#(VRML|X3D) V(.*?) (utf8)(?: (.*?))?[\\n\\r]");
const pcrecpp::RE RegEx::Whitespaces ("[\\x20\\n,\\t\\r]*");
const pcrecpp::RE RegEx::Comment     ("[\\x20\\n,\\t\\r]*#(.*?)(?=[\\n\\r]|$)");

const pcrecpp::RE RegEx::AS          ("[\\x20\\n,\\t\\r]+AS[\\x20\\n,\\t\\r]+");
const pcrecpp::RE RegEx::COMPONENT   ("[\\x20\\n,\\t\\r]*COMPONENT[\\x20\\n,\\t\\r]+");
const pcrecpp::RE RegEx::DEF         ("[\\x20\\n,\\t\\r]*DEF[\\x20\\n,\\t\\r]+");
const pcrecpp::RE RegEx::EXPORT      ("[\\x20\\n,\\t\\r]*EXPORT[\\x20\\n,\\t\\r]+");
const pcrecpp::RE RegEx::EXTERNPROTO ("[\\x20\\n,\\t\\r]*EXTERNPROTO[\\x20\\n,\\t\\r]+");
const pcrecpp::RE RegEx::_false      ("[\\x20\\n,\\t\\r]*FALSE");
const pcrecpp::RE RegEx::IMPORT      ("[\\x20\\n,\\t\\r]*IMPORT[\\x20\\n,\\t\\r]+");
const pcrecpp::RE RegEx::IS          ("[\\x20\\n,\\t\\r]*IS[\\x20\\n,\\t\\r]+");
const pcrecpp::RE RegEx::META        ("[\\x20\\n,\\t\\r]*META[\\x20\\n,\\t\\r]+");
const pcrecpp::RE RegEx::_null       ("[\\x20\\n,\\t\\r]*NULL");
const pcrecpp::RE RegEx::PROFILE     ("[\\x20\\n,\\t\\r]*PROFILE[\\x20\\n,\\t\\r]+");
const pcrecpp::RE RegEx::PROTO       ("[\\x20\\n,\\t\\r]*PROTO[\\x20\\n,\\t\\r]+");
const pcrecpp::RE RegEx::ROUTE       ("[\\x20\\n,\\t\\r]*ROUTE[\\x20\\n,\\t\\r]+");
const pcrecpp::RE RegEx::TO          ("[\\x20\\n,\\t\\r]*TO[\\x20\\n,\\t\\r]+");
const pcrecpp::RE RegEx::_true       ("[\\x20\\n,\\t\\r]*TRUE");
const pcrecpp::RE RegEx::UNIT        ("[\\x20\\n,\\t\\r]*UNIT[\\x20\\n,\\t\\r]+");
const pcrecpp::RE RegEx::USE         ("[\\x20\\n,\\t\\r]*USE[\\x20\\n,\\t\\r]+");

const pcrecpp::RE RegEx::inputOnly      ("[\\x20\\n,\\t\\r]*(?:inputOnly|eventIn)[\\x20\\n,\\t\\r]+");
const pcrecpp::RE RegEx::outputOnly     ("[\\x20\\n,\\t\\r]*(?:outputOnly|eventOut)[\\x20\\n,\\t\\r]+");
const pcrecpp::RE RegEx::inputOutput    ("[\\x20\\n,\\t\\r]*(?:inputOutput|exposedField)[\\x20\\n,\\t\\r]+");
const pcrecpp::RE RegEx::initializeOnly ("[\\x20\\n,\\t\\r]*(?:initializeOnly|field)[\\x20\\n,\\t\\r]+");

const pcrecpp::RE RegEx::OpenBrace    ("[\\x20\\n,\\t\\r]*\\{");
const pcrecpp::RE RegEx::CloseBrace   ("[\\x20\\n,\\t\\r]*\\}");
const pcrecpp::RE RegEx::OpenBracket  ("[\\x20\\n,\\t\\r]*\\[");
const pcrecpp::RE RegEx::CloseBracket ("[\\x20\\n,\\t\\r]*\\]");
const pcrecpp::RE RegEx::Period       ("[\\x20\\n,\\t\\r]*\\.");
const pcrecpp::RE RegEx::Colon        ("[\\x20\\n,\\t\\r]*\\:");
const pcrecpp::RE RegEx::Brackets     ("[\\x20\\n,\\t\\r]*\\[[\\x20\\n,\\t\\r]*\\]");

const pcrecpp::RE RegEx::QuotationMark ("\"", pcrecpp::RE_Options () .set_multiline (true));
const pcrecpp::RE RegEx::EscapedQuotationMark ("\\\\\"", pcrecpp::RE_Options () .set_multiline (true));

const pcrecpp::RE RegEx::_hex    ("[\\x20\\n,\\t\\r]*(0[xX][\\da-fA-F]+)");
const pcrecpp::RE RegEx::_float  ("[\\x20\\n,\\t\\r]*([+-]?(?:(?:(?:\\d*\\.\\d+)|(?:\\d+(?:\\.)?))(?:[eE][+-]?\\d+)?))");
const pcrecpp::RE RegEx::_int32  ("[\\x20\\n,\\t\\r]*([+-]?(?:\\d+))");
const pcrecpp::RE RegEx::_double ("[\\x20\\n,\\t\\r]*([+-]?(?:(?:(?:\\d*\\.\\d+)|(?:\\d+(?:\\.)?))(?:[eE][+-]?\\d+)?))");
const pcrecpp::RE RegEx::_string ("[\\x20\\n,\\t\\r]*\"(.*?)(?<!\\\\)\"", pcrecpp::RE_Options () .set_dotall (true));

const pcrecpp::RE RegEx::Id              ("[\\x20\\n,\\t\\r]*([^\\x30-\\x39\\x00-\\x20\\x22\\x23\\x27\\x2b\\x2c\\x2d\\x2e\\x5b\\x5c\\x5d\\x7b\\x7d\\x7f]{1}[^\\x00-\\x20\\x22\\x23\\x27\\x2c\\x2e\\x5b\\x5c\\x5d\\x7b\\x7d\\x7f]*)");
const pcrecpp::RE RegEx::ComponentNameId ("[\\x20\\n,\\t\\r]*([^\\x30-\\x39\\x00-\\x20\\x22\\x23\\x27\\x2b\\x2c\\x2d\\x2e\\x5b\\x5c\\x5d\\x7b\\x7d\\x7f]{1}[^\\x00-\\x20\\x22\\x23\\x27\\x2c\\x2e\\x5b\\x5c\\x5d\\x7b\\x7d\\x7f:]*)");
const pcrecpp::RE RegEx::FieldType ("[\\x20\\n,\\t\\r]*(MFBool|MFColorRGBA|MFColor|MFDouble|MFFloat|MFImage|MFInt32|MFMatrix3d|MFMatrix3f|MFMatrix4d|MFMatrix4f|MFNode|MFRotation|MFString|MFTime|MFVec2d|MFVec2f|MFVec3d|MFVec3f|MFVec4d|MFVec4f|SFBool|SFColorRGBA|SFColor|SFDouble|SFFloat|SFImage|SFInt32|SFMatrix3d|SFMatrix3f|SFMatrix4d|SFMatrix4f|SFNode|SFRotation|SFString|SFTime|SFVec2d|SFVec2f|SFVec3d|SFVec3f|SFVec4d|SFVec4f)");
const pcrecpp::RE RegEx::ScriptNodeInterfaceIS ("[\\x20\\n,\\t\\r]*(eventIn|inputOnly|eventOut|outputOnly|field|initializeOnly|exposedField|inputOutput)[\\x20\\n,\\t\\r]+(MFBool|MFColorRGBA|MFColor|MFDouble|MFFloat|MFImage|MFInt32|MFMatrix3d|MFMatrix3f|MFMatrix4d|MFMatrix4f|MFNode|MFRotation|MFString|MFTime|MFVec2d|MFVec2f|MFVec3d|MFVec3f|MFVec4d|MFVec4f|SFBool|SFColorRGBA|SFColor|SFDouble|SFFloat|SFImage|SFInt32|SFMatrix3d|SFMatrix3f|SFMatrix4d|SFMatrix4f|SFNode|SFRotation|SFString|SFTime|SFVec2d|SFVec2f|SFVec3d|SFVec3f|SFVec4d|SFVec4f)[\\x20\\n,\\t\\r]+([^\\x30-\\x39\\x00-\\x20\\x22\\x23\\x27\\x2b\\x2c\\x2d\\x2e\\x5b\\x5c\\x5d\\x7b\\x7d\\x7f]{1}[^\\x00-\\x20\\x22\\x23\\x27\\x2c\\x2e\\x5b\\x5c\\x5d\\x7b\\x7d\\x7f]*)[\\x20\\n,\\t\\r]+IS");

const pcrecpp::RE RegEx::_Num ("_\\d+$");

const pcrecpp::RE RegEx::ECMAScript ("\\A(?:vrmlscript|javascript|ecmascript)\\:(.*)$", pcrecpp::RE_Options () .set_dotall (true));

} // X3D
} // titania
