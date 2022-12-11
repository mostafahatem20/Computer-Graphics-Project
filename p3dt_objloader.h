// ==-----------------------------------------------------------------==
// COPYRIGHT
// ==-----------------------------------------------------------------==
//  Created by Omar Sherif Fathy on 11/14/19.
//  Copyright © 2019 Omar Sherif Fathy. All rights reserved.
// ==-----------------------------------------------------------------==

// ==-----------------------------------------------------------------==
// SHORT DESCRIPTION
// ==-----------------------------------------------------------------==
// This is a simple .obj file loader
// It doesn't load materials from .mtl files
// It only loads vertices, normals and texture coordinates
//
// NOTE :
// --------
// It expects a triangulated mesh only !
// Tick the "triangulate faces" option in blender while exporting
// ==-----------------------------------------------------------------==

#pragma once

#include "p3dt_maths.h"

// STL
#include <array>
#include <fstream>
#include <sstream>
#include <vector>

#if defined(_WIN32)
#pragma warning(disable:4996)
#endif

namespace p3dt {

	struct vertex_t
	{
		maths::vector3_t<> position;
		maths::vector3_t<> normal;
		maths::vector2_t<> texcoord;
	};

	struct triangle_t
	{
		vertex_t a;
		vertex_t b;
		vertex_t c;
	};

	struct trimesh_t
	{
		std::vector<triangle_t> triangles;
	};

	namespace objloader {

		struct element_t
		{
			const uint32_t vertex_index;
			const uint32_t normal_index;
			const uint32_t texcoord_index;
			element_t(const uint32_t& v, const uint32_t vn, const uint32_t vt)
				: vertex_index(v)
				, normal_index(vn)
				, texcoord_index(vt)
			{}
		};

		struct texcoord_t
		{
			float m_u;
			float m_v;
			texcoord_t(float u, float v)
			{
				m_u = u;
				m_v = v;
			}
		};

		inline void
			fill(trimesh_t& mesh,
				const std::vector<maths::vector3_t<>>& vertices,
				const std::vector<maths::vector3_t<>>& normals,
				const std::vector<maths::vector2_t<>>& texcoords)
		{
			for (size_t i = 0; i < vertices.size(); i += 3) {
				vertex_t a = { { vertices[i].x, vertices[i].y, vertices[i].z },
							   { normals[i].x, normals[i].y, normals[i].z },
							   { texcoords[i].x, texcoords[i].y } };

				vertex_t b = { { vertices[i + 1].x, vertices[i + 1].y, vertices[i + 1].z },
							   { normals[i + 1].x, normals[i + 1].y, normals[i + 1].z },
							   { texcoords[i + 1].x, texcoords[i + 1].y } };

				vertex_t c = { { vertices[i + 2].x, vertices[i + 2].y, vertices[i + 2].z },
							   { normals[i + 2].x, normals[i + 2].y, normals[i + 2].z },
							   { texcoords[i + 2].x, texcoords[i + 2].y } };

				mesh.triangles.push_back({ a, b, c });
			}
		}

		inline void
			load(const std::vector<std::string>& lines, std::vector<trimesh_t>& meshes)
		{
			std::vector<element_t> _elements;
			std::vector<maths::vector3_t<>> shared_vertices;
			std::vector<maths::vector3_t<>> shared_normals;
			std::vector<maths::vector2_t<>> shared_texcoords;
			bool isTriangulated = true;
		
			for (size_t l = 0; l < lines.size(); ++l) {
				if (lines[l][0] == '#') {
					continue;
				}
				std::stringstream chunks(lines[l]);
				std::string flag;
				chunks >> flag;

				if (flag == "o") {
					if (!meshes.empty()) {
						std::vector<maths::vector3_t<>> vertices;
						std::vector<maths::vector3_t<>> normals;
						std::vector<maths::vector2_t<>> texcoords;

						for (size_t i = 0; i < _elements.size(); ++i) {
							vertices.push_back(shared_vertices[_elements[i].vertex_index]);
							normals.push_back(shared_normals[_elements[i].normal_index]);
							texcoords.push_back(shared_texcoords[_elements[i].texcoord_index]);
						}

						fill(meshes.back(), vertices, normals, texcoords);

						_elements.clear();
						vertices.clear();
						normals.clear();
						texcoords.clear();
					}
					meshes.push_back(trimesh_t{});
					continue;
				}
				else if (flag == "v") {
					maths::vector3_t<> values = {};
					chunks >> values.x >> values.y >> values.z;
					shared_vertices.push_back(values);
				}
				else if (flag == "vn") {
					maths::vector3_t<> values = {};
					chunks >> values.x >> values.y >> values.z;
					shared_normals.push_back(values);
				}
				else if (flag == "vt") {
					maths::vector2_t<> values = {};
					chunks >> values.x >> values.y;
					values.y = 1.0f - values.y;
					shared_texcoords.push_back(values);
				}
				else if (flag == "f") {
					uint32_t v0, v1, v2;
					uint32_t vn0, vn1, vn2;
					uint32_t vt0, vt1, vt2;
					std::string chunk0, chunk1, chunk2;
					chunks >> chunk0 >> chunk1 >> chunk2;
					isTriangulated &= chunk0.find("/") != std::string::npos;
					isTriangulated &= chunk1.find("/") != std::string::npos;
					isTriangulated &= chunk2.find("/") != std::string::npos;
					if (isTriangulated) {
						sscanf(chunk0.c_str(), "%i/%i/%i", &v0, &vt0, &vn0);
						sscanf(chunk1.c_str(), "%i/%i/%i", &v1, &vt1, &vn1);
						sscanf(chunk2.c_str(), "%i/%i/%i", &v2, &vt2, &vn2);
						v0--;
						vt0--;
						vn0--;
						v1--;
						vt1--;
						vn1--;
						v2--;
						vt2--;
						vn2--;
						_elements.push_back(element_t(v0, vn0, vt0));
						_elements.push_back(element_t(v1, vn1, vt1));
						_elements.push_back(element_t(v2, vn2, vt2));
					}
					else {
						printf("WARNING: MESH ISN'T TRIANGULATED !!\n");
					}
				}
			}

			std::vector<maths::vector3_t<>> vertices;
			std::vector<maths::vector3_t<>> normals;
			std::vector<maths::vector2_t<>> texcoords;

			for (size_t i = 0; i < _elements.size(); ++i) {
				vertices.push_back(shared_vertices[_elements[i].vertex_index]);
				normals.push_back(shared_normals[_elements[i].normal_index]);
				texcoords.push_back(shared_texcoords[_elements[i].texcoord_index]);
			}

			fill(meshes.back(), vertices, normals, texcoords);

			_elements.clear();
			vertices.clear();
			normals.clear();
			texcoords.clear();
		}

		static inline void
			load_from_text(const std::string& text, std::vector<trimesh_t>& meshes)
		{
			std::vector<std::string> lines;
			std::istringstream iss(text);
			for (std::string line; std::getline(iss, line);) {
				lines.push_back(line);
			}
			return load(lines, meshes);
		}

		static inline void
			load_from_file(const std::string& filepath, std::vector<trimesh_t>& meshes)
		{
			std::vector<std::string> lines;
			std::fstream fs(filepath);
			for (std::string line; std::getline(fs, line);) {
				lines.push_back(line);
			}
			return load(lines, meshes);
		}

	} // namespace objloader
} // namespace p3dt
