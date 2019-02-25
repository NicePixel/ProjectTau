#include "../print.h"
#include "ogl_font.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static FT_Library fontlib;

// Define vertex array object and vertex buffer object for rendering of a simple
// quad, or a rectangle, for a character.
// They are initialized in `tau_gra_font_init`.
static GLuint VAO = 0, VBO = 0;

#undef  TED_CURSUB
#define TED_CURSUB "tau_gra_font_init"
void tau_gra_font_init(void)
{
	TED_PRINT_INFO("Loading FreeType...");
	if (FT_Init_FreeType(&fontlib))
	{
		TED_PRINT_ERROR("FreeType failed to initialize! Aborting!!!");
		throw CTauGraFontException("Failed to initialize FreeType library");
	}
	
	GLfloat vertices[6][4] = {
		{ 0.0f,        0.0f + 1.0f,      0.0, 0.0 },
		{ 0.0f,        0.0f,             0.0, 1.0 },
		{ 0.0f + 1.0f, 0.0f,             1.0, 1.0 },

		{ 0.0f,        0.0f + 1.0f,      0.0, 0.0 },
		{ 0.0f + 1.0f, 0.0f,             1.0, 1.0 },
		{ 0.0f + 1.0f, 0.0f + 1.0f,      1.0, 0.0 }
	};

	// Create buffers for drawing quads
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

#undef  TED_CURSUB
#define TED_CURSUB "tau_gra_font_quit"
void tau_gra_font_quit(void)
{
	FT_Done_FreeType(fontlib);
}

#undef  TED_CURSUB
#define TED_CURSUB "tau_gra_font_make"
FONT tau_gra_font_make(const char* filepath, int size)
{
	FONT font;
	FT_Face face;

	if (FT_New_Face(fontlib, filepath, 0, &face))
	{
		TED_PRINT_ERROR("Font face failed to load!");
		throw CTauGraFontException("Font face failed to load!");
	}
	FT_Set_Pixel_Sizes(face, 0, size);

	// Load glyphs
	// This requires to have pixel storage mode of 1 byte:
	//   glPixelStorei(GL_UNPACK_ALIGNMENT, 1)
	// which should've already been set in `ogl_init`.
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			TED_PRINT_ERROR("Failed to load a glyph!");
			continue;
		}

		// Generate texture for the current character
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Now store character for later use
		FACE_CHARACTER character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left,  face->glyph->bitmap_top),
			static_cast<GLuint>(face->glyph->advance.x)
		};

		font.characters.insert(std::pair<GLchar, FACE_CHARACTER>(c, character));
	}

	FT_Done_Face(face);
	return font;
}

void tau_gra_font_destroy(FONT* font)
{
	for (auto it = font->characters.begin(); it != font->characters.end(); it++)
	{
		glDeleteTextures(1, &it->second.gl_id);
	}
}

void tau_gra_font_rendertext(FONT* font, SHADER* sha, MESH* panel, CTauCamera* camera, const std::string& text, float x, float y, float scale)
{
	glBindVertexArray(VAO);

	tau_gra_shader_setuniformFlt2(sha, "uvscale",   glm::value_ptr(glm::vec2(1.0f, -1.0f)));
		
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		FACE_CHARACTER ch = font->characters[*c];

		GLfloat xpos = (x + ch.bearing.x) * scale;
		GLfloat ypos = (y - (ch.size.y - ch.bearing.y)) * scale;
		GLfloat w    = ch.size.x * scale*0.5;
		GLfloat h    = ch.size.y * scale*0.5;
		
		tau_gra_shader_setuniformFlt2(sha, "scale2d", glm::value_ptr(glm::vec2(w, h)));
		tau_gra_shader_setuniformFlt2(sha, "pos2d",   glm::value_ptr(glm::vec2(-1.0 + xpos + w, -1.0 + ypos + h)));

		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.gl_id);
		//tau_gra_ren_mesh(panel);
		tau_gra_ren_mesh_unitsquare();
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.advance >> 6) ; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}
	
	glBindVertexArray(0);
}

CTauGraFontException::CTauGraFontException(const char* message) :
	message(message)
{
}
