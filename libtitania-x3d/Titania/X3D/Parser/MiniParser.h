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

#ifndef __TITANIA_X3D_PARSER_MINI_PARSER_H__
#define __TITANIA_X3D_PARSER_MINI_PARSER_H__

#include "../Parser/Grammar.h"
#include "../Types/Numbers.h"
#include "../Types/String.h"

#include <istream>

namespace titania {
namespace X3D {

class MiniParser
{
public:

	// Decode

	static
	bool
	Decode (std::istream & istream, bool & value);

	static
	bool
	Decode (std::istream & istream, Color3f & value);

	static
	bool
	Decode (std::istream & istream, Color4f & value);

	static
	bool
	Decode (std::istream & istream, double & value);

	static
	bool
	Decode (std::istream & istream, float & value);

	static
	bool
	Decode (std::istream & istream, int32_t & value);

	template <class Type>
	static
	bool
	Decode (std::istream & istream, matrix3 <Type> & value)
	{
		std::string whiteSpaces;

		Type e11, e12, e13;
		Type e21, e22, e23;
		Type e31, e32, e33;
		
		Grammar::WhiteSpacesNoComma (istream, whiteSpaces);
	
		if (Grammar::Number <Type> (istream, e11))
		{
			Grammar::WhiteSpacesNoComma (istream, whiteSpaces);
	
			if (Grammar::Number <Type> (istream, e12))
			{
				Grammar::WhiteSpacesNoComma (istream, whiteSpaces);
	
				if (Grammar::Number <Type> (istream, e13))
				{
					Grammar::WhiteSpacesNoComma (istream, whiteSpaces);
	
					if (Grammar::Number <Type> (istream, e21))
					{
						Grammar::WhiteSpacesNoComma (istream, whiteSpaces);
	
						if (Grammar::Number <Type> (istream, e22))
						{
							Grammar::WhiteSpacesNoComma (istream, whiteSpaces);
	
							if (Grammar::Number <Type> (istream, e23))
							{
								Grammar::WhiteSpacesNoComma (istream, whiteSpaces);
	
								if (Grammar::Number <Type> (istream, e31))
								{
									Grammar::WhiteSpacesNoComma (istream, whiteSpaces);
	
									if (Grammar::Number <Type> (istream, e32))
									{
										Grammar::WhiteSpacesNoComma (istream, whiteSpaces);
	
										if (Grammar::Number <Type> (istream, e33))
										{
											value = matrix3 <Type> (e11, e12, e13,
											                        e21, e22, e23,
											                        e31, e32, e33);

											return true;
										}
									}
								}
							}
						}
					}
				}
			}
		}

		istream .setstate (std::ios_base::failbit);
		return false;
	}

	template <class Type>
	static
	bool
	Decode (std::istream & istream, matrix4 <Type> & value)
	{
		std::string whiteSpaces;

		Type e11, e12, e13, e14;
		Type e21, e22, e23, e24;
		Type e31, e32, e33, e34;
		Type e41, e42, e43, e44;
		
		Grammar::WhiteSpacesNoComma (istream, whiteSpaces);
	
		if (Grammar::Number <Type> (istream, e11))
		{
			Grammar::WhiteSpacesNoComma (istream, whiteSpaces);
	
			if (Grammar::Number <Type> (istream, e12))
			{
				Grammar::WhiteSpacesNoComma (istream, whiteSpaces);
	
				if (Grammar::Number <Type> (istream, e13))
				{
					Grammar::WhiteSpacesNoComma (istream, whiteSpaces);
	
					if (Grammar::Number <Type> (istream, e14))
					{
						Grammar::WhiteSpacesNoComma (istream, whiteSpaces);
	
						if (Grammar::Number <Type> (istream, e21))
						{
							Grammar::WhiteSpacesNoComma (istream, whiteSpaces);
	
							if (Grammar::Number <Type> (istream, e22))
							{
								Grammar::WhiteSpacesNoComma (istream, whiteSpaces);
	
								if (Grammar::Number <Type> (istream, e23))
								{
									Grammar::WhiteSpacesNoComma (istream, whiteSpaces);
	
									if (Grammar::Number <Type> (istream, e24))
									{
										Grammar::WhiteSpacesNoComma (istream, whiteSpaces);
	
										if (Grammar::Number <Type> (istream, e31))
										{
											Grammar::WhiteSpacesNoComma (istream, whiteSpaces);
	
											if (Grammar::Number <Type> (istream, e32))
											{
												Grammar::WhiteSpacesNoComma (istream, whiteSpaces);
	
												if (Grammar::Number <Type> (istream, e33))
												{
													Grammar::WhiteSpacesNoComma (istream, whiteSpaces);
	
													if (Grammar::Number <Type> (istream, e34))
													{
														Grammar::WhiteSpacesNoComma (istream, whiteSpaces);
	
														if (Grammar::Number <Type> (istream, e41))
														{
															Grammar::WhiteSpacesNoComma (istream, whiteSpaces);
	
															if (Grammar::Number <Type> (istream, e42))
															{
																Grammar::WhiteSpacesNoComma (istream, whiteSpaces);
	
																if (Grammar::Number <Type> (istream, e43))
																{
																	Grammar::WhiteSpacesNoComma (istream, whiteSpaces);
	
																	if (Grammar::Number <Type> (istream, e44))
																	{
																		value = matrix4 <Type> (e11, e12, e13, e14,
																		                        e21, e22, e23, e24,
																		                        e31, e32, e33, e34,
																		                        e41, e42, e43, e44);

																		return true;
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}

		istream .setstate (std::ios_base::failbit);
		return false;
	}

	template <class Type>
	static
	bool
	Decode (std::istream & istream, rotation4 <Type> & value)
	{
		std::string whiteSpaces;
	
		Type x, y, z, angle;
	
		Grammar::WhiteSpacesNoComma (istream, whiteSpaces);
	
		if (Grammar::Number <Type> (istream, x))
		{
			Grammar::WhiteSpacesNoComma (istream, whiteSpaces);
	
			if (Grammar::Number <Type> (istream, y))
			{
				Grammar::WhiteSpacesNoComma (istream, whiteSpaces);
	
				if (Grammar::Number <Type> (istream, z))
				{
					Grammar::WhiteSpacesNoComma (istream, whiteSpaces);
	
					if (Grammar::Number <Type> (istream, angle))
						value = rotation4 <Type> (x, y, z, angle);

					return true;
				}
		   }
		}

		istream .setstate (std::ios_base::failbit);
		return false;
	}

	static
	bool
	Decode (std::istream & istream, X3D::String & value);

	template <class Type>
	static
	bool
	Decode (std::istream & istream, vector2 <Type> & value)
	{
		std::string whiteSpaces;

		Type x, y;

		Grammar::WhiteSpacesNoComma (istream, whiteSpaces);

		if (Grammar::Number <Type> (istream, x))
		{
			Grammar::WhiteSpacesNoComma (istream, whiteSpaces);

			if (Grammar::Number <Type> (istream, y))
			{
				value = vector2 <Type> (x, y);
				return true;
		   }
		}

		istream .setstate (std::ios_base::failbit);
		return false;
	}

	template <class Type>
	static
	bool
	Decode (std::istream & istream, vector3 <Type> & value)
	{
		std::string whiteSpaces;

		Type x, y, z;

		Grammar::WhiteSpacesNoComma (istream, whiteSpaces);

		if (Grammar::Number <Type> (istream, x))
		{
			Grammar::WhiteSpacesNoComma (istream, whiteSpaces);

			if (Grammar::Number <Type> (istream, y))
			{
				Grammar::WhiteSpacesNoComma (istream, whiteSpaces);

				if (Grammar::Number <Type> (istream, z))
				{
					value = vector3 <Type> (x, y, z);
					return true;
				}
		   }
		}

		istream .setstate (std::ios_base::failbit);
		return false;
	}

	template <class Type>
	static
	bool
	Decode (std::istream & istream, vector4 <Type> & value)
	{
		std::string whiteSpaces;

		Type x, y, z, w;

		Grammar::WhiteSpacesNoComma (istream, whiteSpaces);

		if (Grammar::Number <Type> (istream, x))
		{
			Grammar::WhiteSpacesNoComma (istream, whiteSpaces);

			if (Grammar::Number <Type> (istream, y))
			{
				Grammar::WhiteSpacesNoComma (istream, whiteSpaces);

				if (Grammar::Number <Type> (istream, z))
				{
					Grammar::WhiteSpacesNoComma (istream, whiteSpaces);
	
					if (Grammar::Number <Type> (istream, w))
					{
						value = vector4 <Type> (x, y, z, w);
						return true;
					}
				}
		   }
		}

		istream .setstate (std::ios_base::failbit);
		return false;
	}

	template <class Type>
	static
	std::enable_if_t <std::is_enum_v <Type>, bool>
	Decode (std::istream & istream, Type & value)
	{
		int32_t tmp;

		if (Decode (istream, tmp))
		{
			value = Type (tmp);
			return true;
		}

		return false;
	}

};

} // X3D
} // titania

#endif
