/* ogl_font.h
 * Font rendering
 */
#pragma once

#include <exception>
#include <ft2build.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <map>
#include FT_FREETYPE_H

typedef struct _FACE_CHARACTER
{
	GLuint     gl_id;   // ID handle of the glyph texture
	glm::ivec2 size;    // Size of glyph
	glm::ivec2 bearing; // Offset from baseline to left/top of glyph
	GLuint     advance; // Offset to advance to next glyph
} FACE_CHARACTER;

typedef struct _FONT
{
	std::map<GLchar, FACE_CHARACTER> characters;
} FONT;

void tau_gra_font_init(void);
void tau_gra_font_quit(void);
FONT tau_gra_font_make(const char* filepath, int size);
void tau_gra_font_destroy(FONT* font);
void tau_gra_font_rendertext(FONT* font, const std::string& text, float x, float y, float scale);

class CTauGraFontException : public std::exception
{
public:
	CTauGraFontException(const char* message);
	inline const char* what() const throw()
	{
		return message;
	}
private:
	const char* message;
};
