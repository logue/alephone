/*

	Copyright (C) 1991-2001 and beyond by Bungie Studios, Inc.
	and the "Aleph One" developers.
 
	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	This license is contained in the file "COPYING",
	which is included with this source code; it is available online at
	http://www.gnu.org/licenses/gpl.html

	Font handler
	by Loren Petrich,
	December 17, 2000
	
	This is for specifying and working with text fonts

Dec 25, 2000 (Loren Petrich):
	Added OpenGL-rendering support

Dec 31, 2000 (Loren Petrich):
	Switched to a 32-bit intermediate GWorld, so that text antialiasing
	will work properly.

Jan 12, 2001 (Loren Petrich):
	Fixed MacOS version of TextWidth() -- uses current font
*/

#include "cseries.h"

#ifdef HAVE_OPENGL
#include "OGL_Headers.h"
#include "OGL_Blitter.h"
#include "OGL_Render.h"
#endif

#include <math.h>
#include <string.h>
#include "FontHandler.h"

#include "shape_descriptors.h"
#include "screen_drawing.h"
#include "screen.h"

#ifdef HAVE_OPENGL
std::set<FontSpecifier *> *FontSpecifier::m_font_registry = NULL;
#endif

// MacOS-specific: stuff that gets reused
// static CTabHandle Grays = NULL;

// Font-specifier equality and assignment:

bool FontSpecifier::operator==(FontSpecifier &F)
{
	if (Size != F.Size)
		return false;
	if (Style != F.Style)
		return false;
	if (File != F.File)
		return false;
	return true;
}

FontSpecifier &FontSpecifier::operator=(FontSpecifier &F)
{
	Size = F.Size;
	Style = F.Style;
	File = F.File;
	return *this;
}

FontSpecifier::~FontSpecifier()
{
#ifdef HAVE_OPENGL
	OGL_Reset(false);
#endif
}

// Initializer: call before using because of difficulties in setting up a proper constructor:

void FontSpecifier::Init()
{
	Info = NULL;
	Update();
#ifdef HAVE_OPENGL
	for (int n = 0; n < 256; ++n)
	{
		OGL_Texture[n] = NULL;
	}
#endif
}

void FontSpecifier::Update()
{
	// Clear away
	if (Info)
	{
		unload_font(Info);
		Info = NULL;
	}

	TextSpec Spec;
	Spec.size = Size;
	Spec.style = Style;
	Spec.adjust_height = AdjustLineHeight;

	// Simply implements format "#<value>"; may want to generalize this
	if (File[0] == '#')
	{
		short ID;
		sscanf(File.c_str() + 1, "%hd", &ID);

		Spec.font = ID;
		if (ID == 4)
		{
			Spec.font = -1;
			Spec.normal = "Monaco";
			//Spec.size = Size * 1.34f;
			Spec.size = 12;
		}
		else if (ID == 22)
		{
			Spec.font = -1;
			Spec.normal = "Courier Prime";
			Spec.bold = "Courier Prime Bold";
			//Spec.oblique = "Courier Prime Italic";
			//Spec.bold_oblique = "Courier Prime Bold Italic";
			Spec.adjust_height -= Size * 0.084f;
			Spec.size = 11;
		}
	}
	else
	{
		Spec.font = -1; // no way to fall back :(
		Spec.normal = File;
	}

	Info = load_font(Spec);

	if (Info)
	{
		Ascent = Info->get_ascent();
		Descent = Info->get_descent();
		Leading = Info->get_leading();
		Height = Ascent + Leading;
		LineSpacing = Ascent + Descent + Leading;
		for (int k = 0; k < 256; k++)
			Widths[k] = char_width(k, Info, Style);
	}
	else
		Ascent = Descent = Leading = Height = LineSpacing = 0;
}

// Defined in screen_drawing_sdl.cpp
extern int8 char_width(uint8 c, const sdl_font_info *font, uint16 style);

int FontSpecifier::TextWidth(const char *text)
{
	// Japaneese font(assume text is shiftjis)
	return Info->text_width(text, 0, false);
}
#ifdef HAVE_OPENGL
void FontSpecifier::render_text_(int n, const char *str)
{
	if (OGL_Texture[n]){
		return;
	}
	
	// Put some padding around each glyph so as to avoid clipping it
	const int Pad = 1;
	int ascent_p = Ascent + Pad, descent_p = Descent + Pad;
	int widths_p[256];
	for (int i=0; i<256; i++) {
	  widths_p[i] = Widths[i] + 2*Pad;
	}
	// Now for the totals and dimensions
	int TotalWidth = TextWidth(str) + Pad * 2;
	Widths[n] = TotalWidth;

	// For an empty font, clear out
	if (TotalWidth <= 0) return;

	int GlyphHeight = ascent_p + descent_p;
	
	int EstDim = int(sqrt(static_cast<float>(TotalWidth*GlyphHeight)) + 0.5);
	TxtrWidth[n] = MAX(128, NextPowerOfTwo(EstDim));

	// Find the character starting points and counts
	unsigned char CharStarts[256], CharCounts[256];
	int LastLine = 0;
	CharStarts[LastLine] = 0;
	CharCounts[LastLine] = 0;
	short Pos = 0;
	for (int k=0; k<256; k++)
	{
		// Over the edge? If so, then start a new line
		short NewPos = Pos + widths_p[k];
		if (NewPos > TxtrWidth[n])
		{
			LastLine++;
			CharStarts[LastLine] = k;
			Pos = widths_p[k];
			CharCounts[LastLine] = 1;
		} else {
			Pos = NewPos;
			CharCounts[LastLine]++;
		}
	}
	TxtrHeight[n] = MAX(128, NextPowerOfTwo(GlyphHeight*(LastLine+1)));

	// Render the font glyphs into the SDL surface
	SDL_Surface *FontSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, TxtrWidth[n], TxtrHeight[n], 32, 0xff0000, 0x00ff00, 0x0000ff, 0);
	if (FontSurface == NULL)
		return;

	// Set background to black
	SDL_FillRect(FontSurface, NULL, SDL_MapRGB(FontSurface->format, 0, 0, 0));
	Uint32 White = SDL_MapRGB(FontSurface->format, 0xFF, 0xFF, 0xFF);

	// Copy to surface
	/*
	for (int k = 0; k <= LastLine; k++)
	{
		char Which = CharStarts[k];
		int VPos = (k * GlyphHeight) + ascent_p;
		int HPos = Pad;
		for (int m = 0; m < CharCounts[k]; m++)
		{
		  
		  ::draw_text(FontSurface, &Which, 1, HPos, VPos, White, Info, Style);
		  HPos += widths_p[(unsigned char) (Which++)];
		}
	}
	*/
	::draw_text(FontSurface, str, strlen(str), 1, ascent_p, White, Info, Style);
	

	// Non-MacOS-specific: allocate the texture buffer
 	// Its format is LA 88, where L is the luminosity and A is the alpha channel
 	// The font value will go into A.
 	OGL_Texture[n] = new uint8[2*GetTxtrSize(n)];

	// Copy the SDL surface into the OpenGL texture
	uint8 *PixBase = (uint8 *)FontSurface->pixels;
	int Stride = FontSurface->pitch;

	for (int k = 0; k < TxtrHeight[n]; k++)
	{
		uint8 *SrcPxl = PixBase + k * Stride + 1; // Use one of the middle channels (red or green or blue)
		uint8 *DstPxl = OGL_Texture[n] + 2 * k * TxtrWidth[n];
		for (int m = 0; m < TxtrWidth[n]; m++)
		{
			*(DstPxl++) = 0xff; // Base color: white (will be modified with glColorxxx())
			*(DstPxl++) = *SrcPxl;
			SrcPxl += 4;
		}
	}

	// Clean up
	SDL_FreeSurface(FontSurface);

	// OpenGL stuff starts here
	// Load texture
	glGenTextures(1, &TxtrID[n]);
	glBindTexture(GL_TEXTURE_2D, TxtrID[n]);
	OGL_Register(this);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, TxtrWidth[n], TxtrHeight[n],
							 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, OGL_Texture[n]);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// Allocate and create display lists of rendering commands
	// DispList = glGenLists(256);
	GLfloat TWidNorm = GLfloat(1) / TxtrWidth[n];
	GLfloat THtNorm = GLfloat(1) / TxtrHeight[n];
	GLfloat Bottom = (THtNorm * GlyphHeight);
	GLfloat Right = TWidNorm * Widths[n];
	glNewList(DispList + n, GL_COMPILE);

	// Move to the current glyph's (padded) position
	glTranslatef(-Pad, 0, 0);

	// Draw the glyph rectangle
	// Due to a bug in MacOS X Classic OpenGL, glVertex2s() was changed to glVertex2f()
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0);
	glVertex2d(0, -ascent_p);
	glTexCoord2f(Right, 0);
	glVertex2d(Widths[n], -ascent_p);
	glTexCoord2f(Right, Bottom);
	glVertex2d(Widths[n], descent_p);
	glTexCoord2f(0, Bottom);
	glVertex2d(0, descent_p);
	glEnd();

	// Move to the next glyph's position
	glTranslated(Widths[n] - Pad, 0, 0);

	glEndList();
}
// Reset the OpenGL fonts; its arg indicates whether this is for starting an OpenGL session
// (this is to avoid texture and display-list memory leaks and other such things)
void FontSpecifier::OGL_Reset(bool IsStarting)
{
	// Don't delete these if there is no valid texture;
	// that indicates that there are no valid texture and display-list ID's.
	if (!IsStarting && OGL_Texture[1])
	{
		glDeleteTextures(256, TxtrID);
		glDeleteLists(DispList, 256);
		OGL_Deregister(this);
		for (int n = 0; n < 256; ++n)
		{
			// Invalidates whatever texture had been present
			if (OGL_Texture[n])
			{
				delete[] OGL_Texture[n];
				OGL_Texture[n] = NULL;
			}
		}
	}
	textMap.clear();
	if (!IsStarting)
		return;

	glGenTextures(256, TxtrID);
	DispList = glGenLists(256);
	OGL_Register(this);
	// Put some padding around each glyph so as to avoid clipping i
	for (int n = 1; n < 128; ++n)
	{
		char str[] = {n, 0};
		render_text_(n, str);
	}
}
// Renders a C-style string in OpenGL.
// assumes screen coordinates and that the left baseline point is at (0,0).
// Alters the modelview matrix so that the next characters will be drawn at the proper place.
// One can surround it with glPushMatrix() and glPopMatrix() to remember the original.
void FontSpecifier::OGL_Render(const char *Text)
{
	const char *tp = Text;

	// Bug out if no texture to render
	if (!OGL_Texture[33])
	{
		OGL_Reset(true);
		if (!OGL_Texture[33])
			return;
	}
	glPushAttrib(GL_ENABLE_BIT);

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	while (*tp)
	{
		if ((unsigned char)*tp < 128)
		{
			glBindTexture(GL_TEXTURE_2D, TxtrID[*tp]);
			glCallList(DispList + *tp);
			tp++;
		}
		else
		{
			std::string buffer("");
			unsigned char tc, tc2;
			do
			{
				buffer += *tp;
				tc = *tp++;
				tc2 = *tp;
				char tcc = *tp;
				if (isJChar(tc) && is2ndJChar(tc2))
				{
					buffer += tcc;
					tp++;
				}
			} while (*tp && tc > 127);

			if (int re = textMap[buffer])
			{
				// Already rendered
				glBindTexture(GL_TEXTURE_2D, TxtrID[re]);
				glCallList(DispList + re);
			}
			else
			{
				if (textMap.size() == 127)
				{
					// Buffer is full; clear it
					textMap.clear();
					glDeleteLists(DispList + 128, 128);
					for (int n = 0; n < 128; ++n)
					{
						delete[] OGL_Texture[128 + n];
						OGL_Texture[128 + n] = NULL;
					}
				}
				int pm = 128 + textMap.size();
				textMap[buffer] = pm;
				render_text_(pm, buffer.c_str());
				glCallList(DispList + pm);
			}
		}
	}
	glPopAttrib();
}

// Renders text a la _draw_screen_text() (see screen_drawing.h), with
// alignment and wrapping. Modelview matrix is unaffected.
void FontSpecifier::OGL_DrawText(const char *text, const screen_rectangle &r, short flags)
{
	// Copy the text to draw
	char text_to_draw[256];
	strncpy(text_to_draw, text, 256);
	text_to_draw[255] = 0;

	/*
	// Check for wrapping, and if it occurs, be recursive
	if (flags & _wrap_text) {
		int last_non_printing_character = 0, text_width = 0;
		unsigned count = 0;
		while (count < strlen(text_to_draw) && text_width < RECTANGLE_WIDTH(&r)) {
			text_width += CharWidth(text_to_draw[count]);
			if (text_to_draw[count] == ' ')
				last_non_printing_character = count;
			count++;
		}
		
		if( count != strlen(text_to_draw)) {
			char remaining_text_to_draw[256];
			
			// If we ever have to wrap text, we can't also center vertically. Sorry.
			flags &= ~_center_vertical;
			flags |= _top_justified;
			
			// Pass the rest of it back in, recursively, on the next line
			memcpy(remaining_text_to_draw, text_to_draw + last_non_printing_character + 1, strlen(text_to_draw + last_non_printing_character + 1) + 1);
	
			screen_rectangle new_destination = r;
			new_destination.top += LineSpacing;
			OGL_DrawText(remaining_text_to_draw, new_destination, flags);
	
			// Now truncate our text to draw
			text_to_draw[last_non_printing_character] = 0;
		}
	}

	// Truncate text if necessary
	int t_width = TextWidth(text_to_draw);
	if (t_width > RECTANGLE_WIDTH(&r)) {
		int width = 0;
		int num = 0;
		char c, *p = text_to_draw;
		while ((c = *p++) != 0) {
			width += CharWidth(c);
			if (width > RECTANGLE_WIDTH(&r))
				break;
			num++;
		}
		text_to_draw[num] = 0;
		t_width = TextWidth(text_to_draw);
	}
	*/
	int t_width = TextWidth(text_to_draw) / 2;

	// Horizontal positioning
	int x, y;
	if (flags & _center_horizontal)
		x = r.left + ((RECTANGLE_WIDTH(&r) - t_width) / 2);
	else if (flags & _right_justified)
		x = r.right - t_width;
	else
		x = r.left;

	// Vertical positioning
	if (flags & _center_vertical)
	{
		if (Height > RECTANGLE_HEIGHT(&r))
			y = r.top;
		else
		{
			y = r.bottom;
			int offset = RECTANGLE_HEIGHT(&r) - Height;
			y -= (offset / 2) + (offset & 1) + 1;
		}
	}
	else if (flags & _top_justified)
	{
		if (Height > RECTANGLE_HEIGHT(&r))
			y = r.bottom;
		else
			y = r.top + Height;
	}
	else
		y = r.bottom;

	// Draw text
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslated(x, y, 0);
	OGL_Render(text_to_draw);
	glPopMatrix();
}

void FontSpecifier::OGL_ResetFonts(bool IsStarting)
{
	if (!m_font_registry)
		return;

	std::set<FontSpecifier *>::iterator it;
	if (IsStarting)
	{
		for (it = m_font_registry->begin();
				 it != m_font_registry->end();
				 ++it)
			(*it)->OGL_Reset(IsStarting);
	}
	else
	{
		for (it = m_font_registry->begin();
				 it != m_font_registry->end();
				 it = m_font_registry->begin())
			(*it)->OGL_Reset(IsStarting);
	}
}

void FontSpecifier::OGL_Register(FontSpecifier *F)
{
	if (!m_font_registry)
		m_font_registry = new std::set<FontSpecifier *>;
	m_font_registry->insert(F);
}

void FontSpecifier::OGL_Deregister(FontSpecifier *F)
{
	if (m_font_registry)
		m_font_registry->erase(F);
	// we could delete registry here, but why bother?
}

#endif // def HAVE_OPENGL

// Draw text without worrying about OpenGL vs. SDL mode.
int FontSpecifier::DrawText(SDL_Surface *s, const char *text, int x, int y, uint32 pixel, bool utf8)
{
	if (!s)
		return 0;
	if (s == MainScreenSurface() && MainScreenIsOpenGL())
		return draw_text(s, text, x, y, pixel, this->Info, this->Style, utf8);

#ifdef HAVE_OPENGL

	uint8 r, g, b;
	SDL_GetRGB(pixel, s->format, &r, &g, &b);
	glColor4ub(r, g, b, 255);

	// draw into both buffers
	for (int i = 0; i < 2; i++)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(x, y, 0);
		this->OGL_Render(text);
		glPopMatrix();
		MainScreenSwap();
	}
	return 1;
#else
	return 0;
#endif
}
