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

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_PARSER_GRAMMAR_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_PARSER_GRAMMAR_H__

#include <Titania/InputOutput.h>
#include <Titania/InputOutput/MultiLineComment.h>

namespace titania {
namespace pb {

class Grammar
{
public:

	///  @name General

	static const io::sequence            WhiteSpaces;
	static const io::sequence            WhiteSpacesNoLineTerminator;
	static const io::single_line_comment SingleLineComment;
	static const io::multi_line_comment  MultiLineComment;

	///  @name Terminal Symbols

	static const io::character Period;
	static const io::character Comma;
	static const io::character Semicolon;
	static const io::character OpenParenthesis;
	static const io::character CloseParenthesis;
	static const io::character OpenBracket;
	static const io::character CloseBracket;
	static const io::character OpenBrace;
	static const io::character CloseBrace;

	///  @name Keywords

	static const io::string _this;
	static const io::string _false;
	static const io::string _true;
	static const io::string null;
	static const io::string function;
	static const io::string var;

	static const std::set <std::string> Keyword;
	static const std::set <std::string> FutureReservedWord;

	///  @name New Opearator

	static const io::string _new;

	///  @name Unary Operator

	static const io::string    _delete;
	static const io::string    _void;
	static const io::string    _typeof;
	static const io::string    Increment;
	static const io::string    Decrement;
	static const io::character PlusSign;
	static const io::character MinusSign;
	static const io::character Tilde;
	static const io::character ExclamationMark;

	///  @name Binary Operator

	static const io::character Multiplication;
	static const io::character Division;
	static const io::character Remainder;
	static const io::character Addition;
	static const io::character Subtraction;
	static const io::string    LeftShift;
	static const io::string    RightShift;
	static const io::string    UnsignedRightShift;
	static const io::character Less;
	static const io::character Greater;
	static const io::string    LessEqual;
	static const io::string    GreaterEqual;
	static const io::string    instanceof;
	static const io::string    in;
	static const io::string    Equal;
	static const io::string    NotEqual;
	static const io::string    StrictEqual;
	static const io::string    StrictNotEqual;
	static const io::character BitwiseAND;
	static const io::character BitwiseXOR;
	static const io::character BitwiseOR;
	static const io::string    LogicalAND;
	static const io::string    LogicalOR;
	static const io::character QuestionMark;
	static const io::character Colon;

	///  @name Assignment Operators

	static const io::character Assignment;
	static const io::string    MultiplicationAssigment;
	static const io::string    DivisionAssignment;
	static const io::string    RemainderAssignment;
	static const io::string    AdditionAssignment;
	static const io::string    SubtractionAssignment;
	static const io::string    LeftShiftAssignment;
	static const io::string    RightShiftAssignment;
	static const io::string    UnsignedRightShiftAssignment;
	static const io::string    BitwiseANDAssignment;
	static const io::string    BitwiseXORAssignment;
	static const io::string    BitwiseORAssignment;

	///  @name Return statement

	static const io::string _return;

	///  @name Values

	static const io::string   hex;
	static const io::string   HEX;
	static const io::string   oct;
	static const io::string   OCT;
	static const io::string   bin;
	static const io::string   BIN;
	static const io::sequence BinaryDigits;

};

// General
const io::sequence            Grammar::WhiteSpaces ("\r\n \t");
const io::sequence            Grammar::WhiteSpacesNoLineTerminator (" \t");
const io::single_line_comment Grammar::SingleLineComment ("//");
const io::multi_line_comment  Grammar::MultiLineComment ("/*", "*/");

// Terminal Symbols
const io::character Grammar::Period ('.');
const io::character Grammar::Comma (',');
const io::character Grammar::Semicolon (';');
const io::character Grammar::OpenParenthesis ('(');
const io::character Grammar::CloseParenthesis (')');
const io::character Grammar::OpenBracket ('[');
const io::character Grammar::CloseBracket (']');
const io::character Grammar::OpenBrace ('{');
const io::character Grammar::CloseBrace ('}');

// Keywords
const io::string Grammar::_this ("this");
const io::string Grammar::_false ("false");
const io::string Grammar::_true ("true");
const io::string Grammar::null ("null");
const io::string Grammar::function ("function");
const io::string Grammar::var ("var");

const std::set <std::string> Grammar::Keyword = {
	"break",
	"case",
	"catch",
	"continue",
	"debugger",
	"default",
	"delete",
	"do",
	"else"
	"finally",
	"for",
	"function",
	"if",
	"in",
	"instanceof",
	"new",
	"return",
	"switch",
	"this",
	"throw",
	"try",
	"typeof",
	"var",
	"void",
	"while",
	"with"

};

const std::set <std::string> Grammar::FutureReservedWord = {
	"class",
	"const",
	"enum",
	"export",
	"extends",
	"import",
	"super"

};

// New Opearator
const io::string Grammar::_new ("new");

// Unary Operator
const io::string    Grammar::_delete ("delete");
const io::string    Grammar::_void ("void");
const io::string    Grammar::_typeof ("typeof");
const io::string    Grammar::Increment ("++");
const io::string    Grammar::Decrement ("--");
const io::character Grammar::PlusSign ('+');
const io::character Grammar::MinusSign ('-');
const io::character Grammar::Tilde ('~');
const io::character Grammar::ExclamationMark ('!');

// Binary Operator
const io::character Grammar::Multiplication ('*');
const io::character Grammar::Division ('/');
const io::character Grammar::Remainder ('%');
const io::character Grammar::Addition ('+');
const io::character Grammar::Subtraction ('-');
const io::string    Grammar::LeftShift ("<<");
const io::string    Grammar::RightShift (">>");
const io::string    Grammar::UnsignedRightShift (">>>");
const io::character Grammar::Less ('<');
const io::character Grammar::Greater ('>');
const io::string    Grammar::LessEqual ("<=");
const io::string    Grammar::GreaterEqual (">=");
const io::string    Grammar::instanceof ("instanceof");
const io::string    Grammar::in ("in");
const io::string    Grammar::Equal ("==");
const io::string    Grammar::NotEqual ("!=");
const io::string    Grammar::StrictEqual ("===");
const io::string    Grammar::StrictNotEqual ("!==");
const io::character Grammar::BitwiseAND ('&');
const io::character Grammar::BitwiseXOR ('^');
const io::character Grammar::BitwiseOR ('|');
const io::string    Grammar::LogicalAND ("&&");
const io::string    Grammar::LogicalOR ("||");
const io::character Grammar::QuestionMark ('?');
const io::character Grammar::Colon (':');

// Assignment Operators
const io::character Grammar::Assignment ('=');
const io::string    Grammar::MultiplicationAssigment ("*=");
const io::string    Grammar::DivisionAssignment ("/=");
const io::string    Grammar::RemainderAssignment ("%=");
const io::string    Grammar::AdditionAssignment ("+=");
const io::string    Grammar::SubtractionAssignment ("-=");
const io::string    Grammar::LeftShiftAssignment ("<<=");
const io::string    Grammar::RightShiftAssignment (">>=");
const io::string    Grammar::UnsignedRightShiftAssignment (">>>=");
const io::string    Grammar::BitwiseANDAssignment ("&=");
const io::string    Grammar::BitwiseXORAssignment ("^=");
const io::string    Grammar::BitwiseORAssignment ("|=");

// Return statement
const io::string Grammar::_return ("return");

// Values
const io::string   Grammar::hex ("0x");
const io::string   Grammar::HEX ("0X");
const io::string   Grammar::oct ("0o");
const io::string   Grammar::OCT ("0O");
const io::string   Grammar::bin ("0b");
const io::string   Grammar::BIN ("0B");
const io::sequence Grammar::BinaryDigits ("01");

} // pb
} // titania

#endif
