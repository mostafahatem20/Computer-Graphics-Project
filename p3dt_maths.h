// ==-----------------------------------------------------------------==
// COPYRIGHT
// ==-----------------------------------------------------------------==
//  Created by Omar Sherif Fathy on 11/14/19.
//  Copyright © 2019 Omar Sherif Fathy. All rights reserved.
// ==-----------------------------------------------------------------==

// ==-----------------------------------------------------------------==
// SHORT DESCRIPTION
// ==-----------------------------------------------------------------==
// This is a short and unoptimized version of the maths library
// No SIMD support ..
// ==-----------------------------------------------------------------==

#pragma once

// STL
#include <cmath>

// This is originally implemented to work with OpenGL
// So, a coloumn major by default
// In case you want to use it with DirectX
// or any other Row major Api
// you'll need to #define P3DT_MATHS_ROW_MAJOR
#if defined(P3DT_MATHS_ROW_MAJOR)
#define USE_ROW_MAJOR
#else
#define USE_COL_MAJOR
#endif

// default precision is float
// Use double if you want ...
// float is good enough for me
#define P3DT_MATHS_PRECISION float

namespace p3dt {
	namespace maths {
		constexpr auto PI = 3.141592653589793238462;
		///
		/// Vector 2 (x, y)
		///
		template<typename T = P3DT_MATHS_PRECISION>
		struct vector2_t
		{
			T x;
			T y;
		};

		///
		/// Vector 3 (x, y, z)
		///
		template<typename T = P3DT_MATHS_PRECISION>
		struct vector3_t
		{
			union
			{
				struct
				{
					T x;
					T y;
					T z;
				};
				struct
				{
					vector2_t<T> xy;
					T _z;
				};
				struct
				{
					T _x;
					vector2_t<T> yz;
				};
			};
		};

		///
		/// Vector 4 (x, y, z, w)
		///
		template<typename T = P3DT_MATHS_PRECISION>
		struct vector4_t
		{
			union
			{
				struct
				{
					T x;
					T y;
					T z;
					T w;
				};
				struct
				{
					vector2_t<T> xy;
					vector2_t<T> zw;
				};
				struct
				{
					T _x;
					vector2_t<T> yz;
					T _w;
				};
				struct
				{
					vector3_t<T> xyz;
					T _ww;
				};
				struct
				{
					T _xx;
					vector3_t<T> yzw;
				};
			};
		};

		///
		/// Matrix 2x2
		///
		template<typename T = P3DT_MATHS_PRECISION>
		struct mat2_t
		{
			union
			{
				struct
				{
					vector2_t<T> data[2];
				};
				struct
				{
					vector2_t<T> r0;
					vector2_t<T> r1;
				};
				struct
				{
					T _00;
					T _01;
					T _10;
					T _11;
				};
			};
			static mat2_t identity;
		};
		template<typename T>
		mat2_t<T> mat2_t<T>::identity = {
			1.0f,
			0.0f,
			0.0f,
			1.0f,
		};

		///
		/// Matrix 3x3
		///
		template<typename T = P3DT_MATHS_PRECISION>
		struct mat3_t
		{
			union
			{
				struct
				{
					vector3_t<T> data[3];
				};
				struct
				{
					vector3_t<T> r0;
					vector3_t<T> r1;
					vector3_t<T> r2;
				};
				struct
				{
					T _00;
					T _01;
					T _02;
					T _10;
					T _11;
					T _12;
					T _20;
					T _21;
					T _22;
				};
			};
			static mat3_t identity;
		};
		template<typename T>
		mat3_t<T> mat3_t<T>::identity = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };

		///
		/// Matrix 4x4
		///
		template<typename T = P3DT_MATHS_PRECISION>
		struct mat4_t
		{
			union
			{
				struct
				{
					vector4_t<T> data[4];
				};
				struct
				{
					vector4_t<T> r0;
					vector4_t<T> r1;
					vector4_t<T> r2;
					vector4_t<T> r3;
				};
				struct
				{
					T _00;
					T _01;
					T _02;
					T _03;
					T _10;
					T _11;
					T _12;
					T _13;
					T _20;
					T _21;
					T _22;
					T _23;
					T _30;
					T _31;
					T _32;
					T _33;
				};
			};
			static mat4_t identity;
		};
		template<typename T>
		mat4_t<T> mat4_t<T>::identity = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
										  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
	} // namespace maths
} // namespace p3dt
