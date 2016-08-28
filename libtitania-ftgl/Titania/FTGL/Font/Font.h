/*
 * FTGL - OpenGL font library
 *
 * Copyright (c) 2001-2004 Henry Maddocks <ftgl@opengl.geek.nz>
 * Copyright (c) 2008 Sam Hocevar <sam@zoy.org>
 * Copyright (c) 2008 Sean Morrison <learner@brlcad.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __TITANIA_FTGL_FONT_FONT_H__
#define __TITANIA_FTGL_FONT_FONT_H__

#include "../BBox.h"
#include "../Glyph/Glyph.h"
#include "../Types.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include <cstddef>
#include <cstdint>

namespace titania {
namespace FTGL {

class FontImpl;

/**
 * Font is the public interface for the FTGL library.
 *
 * Specific font classes are derived from this class. It uses the helper
 * classes Face and Size to access the Freetype library. This class
 * is abstract and deriving classes must implement the protected
 * <code>MakeGlyph</code> function to create glyphs of the
 * appropriate type.
 *
 * It is good practice after using these functions to test the error
 * code returned. <code>FT_Error Error()</code>. Check the freetype file
 * fterrdef.h for error definitions.
 *
 * @see     Face
 * @see     Size
 */
class Font
{
protected:

	/**
	 * Open and read a font file. Sets Error flag.
	 *
	 * @param fontFilePath  font file path.
	 */
	Font (char const* fontFilePath);

	/**
	 * Open and read a font from a buffer in memory. Sets Error flag.
	 * The buffer is owned by the client and is NOT copied by FTGL. The
	 * pointer must be valid while using FTGL.
	 *
	 * @param pBufferBytes  the in-memory buffer
	 * @param bufferSizeInBytes  the length of the buffer in bytes
	 */
	Font (const uint8_t* pBufferBytes, size_t bufferSizeInBytes);


private:

	/* Allow our internal subclasses to access the private constructor */
	friend class PolygonFont;

	/**
	 * Internal FTGL Font constructor. For private use only.
	 *
	 * @param pImpl  Internal implementation object. Will be destroyed
	 *               upon Font deletion.
	 */
	Font (FontImpl* pImpl);


public:

	virtual
	~Font ();

	/**
	 * Attach auxilliary file to font e.g font metrics.
	 *
	 * Note: not all font formats implement this function.
	 *
	 * @param fontFilePath  auxilliary font file path.
	 * @return          <code>true</code> if file has been attached
	 *                  successfully.
	 */
	virtual
	bool
	attach (const char* fontFilePath);

	/**
	 * Attach auxilliary data to font e.g font metrics, from memory.
	 *
	 * Note: not all font formats implement this function.
	 *
	 * @param pBufferBytes  the in-memory buffer.
	 * @param bufferSizeInBytes  the length of the buffer in bytes.
	 * @return          <code>true</code> if file has been attached
	 *                  successfully.
	 */
	virtual
	bool
	attach (const uint8_t* pBufferBytes,
	        size_t bufferSizeInBytes);

	/**
	 * Set the glyph loading flags. By default, fonts use the most
	 * sensible flags when loading a font's glyph using FT_Load_Glyph().
	 * This function allows to override the default flags.
	 *
	 * @param flags  The glyph loading flags.
	 */
	virtual
	void
	setGlyphLoadFlags (FT_Int flags);

	/**
	 * Set the character map for the face.
	 *
	 * @param encoding      Freetype enumerate for char map code.
	 * @return              <code>true</code> if charmap was valid and
	 *                      set correctly.
	 */
	virtual
	bool
	setCharMap (FT_Encoding encoding);

	/**
	 * Get the number of character maps in this face.
	 *
	 * @return character map count.
	 */
	virtual
	uint32_t
	getCharMapCount () const;

	/**
	 * Get a list of character maps in this face.
	 *
	 * @return pointer to the first encoding.
	 */
	virtual
	FT_Encoding*
	getCharMapList ();

	/**
	 * Set the char size for the current face.
	 *
	 * @param size      the face size in points (1/72 inch)
	 * @param res       the resolution of the target device.
	 * @return          <code>true</code> if size was set correctly
	 */
	virtual
	bool
	setFaceSize (const uint32_t size,
	             const uint32_t res = 72);

	/**
	 * Get the current face size in points (1/72 inch).
	 *
	 * @return face size
	 */
	virtual
	uint32_t
	getFaceSize () const;

	/**
	 * Set the extrusion distance for the font. Only implemented by
	 * FTExtrudeFont
	 *
	 * @param depth  The extrusion distance.
	 */
	virtual
	void
	setDepth (double depth);

	/**
	 * Set the outset distance for the font. Only implemented by
	 * FTOutlineFont, PolygonFont and FTExtrudeFont
	 *
	 * @param outset  The outset distance.
	 */
	virtual
	void
	setOutset (double outset);

	/**
	 * Set the front and back outset distances for the font. Only
	 * implemented by FTExtrudeFont
	 *
	 * @param front  The front outset distance.
	 * @param back   The back outset distance.
	 */
	virtual
	void
	setOutset (double front, double back);

	/**
	 * Enable or disable the use of Display Lists inside FTGL
	 *
	 * @param  useList <code>true</code> turns ON display lists.
	 *                 <code>false</code> turns OFF display lists.
	 */
	virtual
	void
	setUseDisplayList (bool useList);

	/**
	 * Get the global ascender height for the face.
	 *
	 * @return  Ascender height
	 */
	virtual
	double
	getAscender () const;

	/**
	 * Gets the global descender height for the face.
	 *
	 * @return  Descender height
	 */
	virtual
	double
	getDescender () const;

	/**
	 * Gets the line spacing for the font.
	 *
	 * @return  Line height
	 */
	virtual
	double
	getLineHeight () const;

	/**
	 * Get the bounding box for a string.
	 *
	 * @param string  A char buffer.
	 * @param len  The length of the string. If < 0 then all characters
	 *             will be checked until a null character is encountered
	 *             (optional).
	 * @param position  The pen position of the first character (optional).
	 * @param spacing  A displacement vector to add after each character
	 *                 has been checked (optional).
	 * @return  The corresponding bounding box.
	 */
	virtual
	BBox
	getBBox (const char* string, const int32_t len = -1,
	         Vector3d position = Vector3d (),
	         Vector3d spacing = Vector3d ());

	/**
	 * Get the bounding box for a string (deprecated).
	 *
	 * @param string  A char buffer.
	 * @param llx  Lower left near x coordinate.
	 * @param lly  Lower left near y coordinate.
	 * @param llz  Lower left near z coordinate.
	 * @param urx  Upper right far x coordinate.
	 * @param ury  Upper right far y coordinate.
	 * @param urz  Upper right far z coordinate.
	 */
	void
	getBBox (const char* string, double & llx, double & lly, double & llz,
	         double & urx, double & ury, double & urz)
	{
		BBox b = getBBox (string);

		llx = b .getLower ().x (); lly = b .getLower ().y (); llz = b .getLower ().z ();
		urx = b .getUpper ().x (); ury = b .getUpper ().y (); urz = b .getUpper ().z ();
	}

	/**
	 * Get the advance for a string.
	 *
	 * @param string  'C' style string to be checked.
	 * @param len  The length of the string. If < 0 then all characters
	 *             will be checked until a null character is encountered
	 *             (optional).
	 * @param spacing  A displacement vector to add after each character
	 *                 has been checked (optional).
	 * @return  The string's advance width.
	 */
	virtual
	double
	advance (const char* string, const int32_t len = -1,
	         Vector3d spacing = Vector3d ());

	/**
	 * Render a string of characters.
	 *
	 * @param string  'C' style string to be output.
	 * @param len  The length of the string. If < 0 then all characters
	 *             will be displayed until a null character is encountered
	 *             (optional).
	 * @param position  The pen position of the first character (optional).
	 * @param spacing  A displacement vector to add after each character
	 *                 has been displayed (optional).
	 * @param renderMode  Render mode to use for display (optional).
	 * @return  The new pen position after the last character was output.
	 */
	virtual
	Vector3d
	render (const char* string, const int32_t len = -1,
	        Vector3d position = Vector3d (),
	        Vector3d spacing = Vector3d (),
	        FTGL::RenderMode renderMode = FTGL::RenderMode::RENDER_ALL);

	/**
	 * Queries the Font for errors.
	 *
	 * @return  The current error code.
	 */
	virtual
	FT_Error
	getError () const;


protected:

	/* Allow impl to access MakeGlyph */
	friend class FontImpl;

	/**
	 * Construct a glyph of the correct type.
	 *
	 * Clients must override the function and return their specialised
	 * Glyph.
	 *
	 * @param slot  A FreeType glyph slot.
	 * @return  An FT****Glyph or <code>null</code> on failure.
	 */
	virtual
	Glyph*
	makeGlyph (FT_GlyphSlot slot) = 0;


private:

	/**
	 * Internal FTGL Font implementation object. For private use only.
	 */
	FontImpl* impl;

};

} // FTGL
} // titania

#endif
