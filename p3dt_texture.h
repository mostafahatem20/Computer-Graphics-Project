// ==-----------------------------------------------------------------==
// COPYRIGHT
// ==-----------------------------------------------------------------==
//  Created by Omar Sherif Fathy on 11/14/19.
//  Copyright © 2019 Omar Sherif Fathy. All rights reserved.
// ==-----------------------------------------------------------------==

// ==-----------------------------------------------------------------==
// SHORT DESCRIPTION
// ==-----------------------------------------------------------------==
// This is a simple texture loader using stb header
// It expects .jpg or a png file (check stb for more extensions)
// It mainly supports RGB or RGBA textures for now
// It also expects the texture pixel type to be unsigned char (1 byte)
// so, no hdr (float) support
// ==-----------------------------------------------------------------==

#pragma once

// OPENGL
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>
#elif _WIN32
#include <glut.h>
#endif

// STB
#define STBI_HEADER_FILE_ONLY
#define STB_IMAGE_IMPLEMENTATION
#include "thirdparty/stb_image.h"

namespace p3dt {
	namespace texture {

		typedef unsigned char byte;

		///
		/// texture buffer
		///
		struct buffer_t
		{
			buffer_t()
				: width(0)
				, height(0)
				, channels(0)
				, filepath("")
				, data(nullptr)
			{}

			int width;
			int height;
			int channels;
			std::string filepath;
			byte* data;
		};

		///
		/// loads the texture pixel data from a valid filepath into a texture buffer
		///
		inline static bool
			load(const char* filepath, buffer_t* buffer)
		{
			buffer->filepath = filepath;
			buffer->data = stbi_load(filepath, &buffer->width, &buffer->height, &buffer->channels, 0);
			if (!buffer->data) {
				printf("\nWarning: invalid texture path <%s>\n", filepath);
				return false;
			}
			return true;
		}

		///
		/// deallocates the memory used by the buffer data and resets the buffer
		///
		inline static void
			unload(buffer_t* buffer)
		{
			stbi_image_free(buffer->data);
			buffer->width = 0;
			buffer->height = 0;
			buffer->channels = 0;
			buffer->filepath = "";
			buffer->data = nullptr;
		}

		///
		/// creates an opengl texture from buffer data by uploading it to vram and returns an opengl id
		/// the current support is limited to RGB and RGBA images
		///
		inline static GLuint
			create_opengl_texture(const buffer_t* buffer)
		{
			GLint internal_format = GL_RGB;
			GLenum format = GL_RGB;

			if (buffer->channels == 4) {
				internal_format = GL_RGBA;
				format = GL_RGBA;
			}
			else if (buffer->channels == 3) {
				internal_format = GL_RGB;
				format = GL_RGB;
			}
			else {
				printf("\nWarning: I only support RGB or RGBA. Texture <%s> won't be uploaded to vram.\n",
					buffer->filepath.c_str());
				return 0;
			}

			glEnable(GL_TEXTURE_2D);
			GLuint texture_id = 0;
			glGenTextures(1, &texture_id);
			glBindTexture(GL_TEXTURE_2D, texture_id);
			glTexImage2D(
				GL_TEXTURE_2D, 0, internal_format, buffer->width, buffer->height, 0, format, GL_UNSIGNED_BYTE, buffer->data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glBindTexture(GL_TEXTURE_2D, texture_id);
			glDisable(GL_TEXTURE_2D);

			return texture_id;
		}

		///
		/// destroys the opengl texture memory residing on the vram
		///
		inline static void
			destroy_opengl_texture(GLuint texture_id)
		{
			glDeleteTextures(1, &texture_id);
		}

	} // namespace texture
} // namespace p3dt
