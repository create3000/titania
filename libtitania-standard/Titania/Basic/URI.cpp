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

#include "URI.h"

namespace titania {
namespace basic {

template <>
const std::string basic_uri <std::string>::DataSchemeId = "data";

template <>
const std::string basic_uri <std::string>::FileSchemeId = "file";

template <>
const char basic_uri <std::string>::Signs::Zero = '0';
template <>
const char basic_uri <std::string>::Signs::Colon = ':';
template <>
const char basic_uri <std::string>::Signs::Slash = '/';
template <>
const char basic_uri <std::string>::Signs::QuestionMark = '?';
template <>
const char basic_uri <std::string>::Signs::NumberSign = '#';
template <>
const char basic_uri <std::string>::Signs::Dot = '.';

template <>
const std::wstring basic_uri <std::wstring>::DataSchemeId = L"data";

template <>
const std::wstring basic_uri <std::wstring>::FileSchemeId = L"file";

template <>
const wchar_t basic_uri <std::wstring>::Signs::Zero = L'0';
template <>
const wchar_t basic_uri <std::wstring>::Signs::Colon = L':';
template <>
const wchar_t basic_uri <std::wstring>::Signs::Slash = L'/';
template <>
const wchar_t basic_uri <std::wstring>::Signs::QuestionMark = L'?';
template <>
const wchar_t basic_uri <std::wstring>::Signs::NumberSign = L'#';
template <>
const wchar_t basic_uri <std::wstring>::Signs::Dot = L'.';

template <>
std::map <std::wstring, size_t> basic_uri <std::wstring>::well_known_ports = {
	std::make_pair (L"ftp",    21),
	std::make_pair (L"http",   80),
	std::make_pair (L"https", 443),
	std::make_pair (L"ftps",  990)

};

//
template class basic_uri <std::string>;
template class basic_uri <std::wstring>;

//
template std::ostream & operator << (std::ostream &, const uri &);
template std::wostream & operator << (std::wostream &, const wuri &);

} // X3D
} // titania
