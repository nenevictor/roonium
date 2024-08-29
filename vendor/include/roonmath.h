#ifndef ROONMATH_H
#define ROONMATH_H

#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef DEGTORAD
#define DEGTORAD(_degrees) (_degrees * M_PI / 180.0f)
#endif

/* Stole a lot of code from "linmath.h".
 * https://github.com/datenwolf/linmath.h/blob/master/linmath.h
 */

/* Vec4 */
typedef struct roonium_vector4
{
	float x, y, z, w;
} roonium_vector4;

/* Vec3 */
typedef struct roonium_vector3
{
	float x, y, z;
} roonium_vector3;

/* Vec2 */
typedef struct roonium_vector2
{
	float x, y;
} roonium_vector2;

/* Matrix */
typedef float roonium_matrix[4][4];

/* Functions declaration. */
struct roonium_vector3 get_normal(
	const struct roonium_vector3 _a,
	const struct roonium_vector3 _b,
	const struct roonium_vector3 _c);

struct roonium_vector3 vector3_subtract(
	const roonium_vector3 _a,
	const roonium_vector3 _b);

struct roonium_vector3 vector3_normalize(
	const struct roonium_vector3 _v);

struct roonium_vector3 vector3_cross(
	const roonium_vector3 _a,
	const roonium_vector3 _b);

float vector3_length(
	const struct roonium_vector3 _v);

float vector4_multiply_inner(
	const struct roonium_vector4 _a,
	const struct roonium_vector4 _b);

void matrix_identity(
	roonium_matrix _matrix);

void matrix_multiply(
	roonium_matrix _matrix,
	roonium_matrix _a,
	roonium_matrix _b);

void matrix_perspective(
	roonium_matrix _matrix,
	const float _fov,
	const float _aspect,
	const float _n,
	const float _f);

void matrix_translate_in_place(
	roonium_matrix _matrix,
	const float _x,
	const float _y,
	const float _z);

void matrix_look_at(
	roonium_matrix _matrix,
	const struct roonium_vector3 _eye,
	const struct roonium_vector3 _center,
	const struct roonium_vector3 _up);

void matrix_rotate_y(
	roonium_matrix _destination,
	roonium_matrix _source,
	const float _angle);
#endif

#ifdef ROONMATH_IMPLEMENTATION
struct roonium_vector3 get_normal(
	const struct roonium_vector3 _a,
	const struct roonium_vector3 _b,
	const struct roonium_vector3 _c)
{
	struct roonium_vector3 result;

	const float acx = _c.x - _a.x;
	const float acy = _c.y - _a.y;
	const float acz = _c.z - _a.z;
	const float bcx = _c.x - _b.x;
	const float bcy = _c.y - _b.y;
	const float bcz = _c.z - _b.z;

	result.x = acy * bcz - acz * bcy;
	result.y = acz * bcx - acx * bcz;
	result.z = acx * bcy - acy * bcx;

	return result;
}

struct roonium_vector3 vector3_subtract(
	const roonium_vector3 _a,
	const roonium_vector3 _b)
{
	struct roonium_vector3 result;

	result.x = _a.x - _b.x;
	result.y = _a.y - _b.y;
	result.z = _a.z - _b.z;

	return result;
}

struct roonium_vector3 vector3_normalize(
	const struct roonium_vector3 _v)
{
	struct roonium_vector3 result = _v;
	const float length = sqrt(
		_v.x * _v.x + _v.y * _v.y + _v.z * _v.z);
	const float ilength = 1.0f / length;

	if (length != 0.0f)
	{
		result.z *= ilength;
		result.x *= ilength;
		result.y *= ilength;
	}

	return result;
}

struct roonium_vector3 vector3_cross(
	const roonium_vector3 _a,
	const roonium_vector3 _b)
{
	struct roonium_vector3 result;
	result.x = _a.y * _b.z - _a.z * _b.y;
	result.y = _a.z * _b.x - _a.x * _b.z;
	result.z = _a.x * _b.y - _a.y * _b.x;
	return result;
}

float vector3_length(
	const struct roonium_vector3 _v)
{
	float result = sqrt(_v.x * _v.x + _v.y * _v.y + _v.z * _v.z);

	return result;
}

float vector4_multiply_inner(
	const struct roonium_vector4 _a,
	const struct roonium_vector4 _b)
{
	float p = 0.0f;

	p += _b.x * _a.x;
	p += _b.y * _a.y;
	p += _b.z * _a.z;
	p += _b.w * _a.w;

	return p;
}

void matrix_identity(
	roonium_matrix _matrix)
{
	int i = 0, j;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			_matrix[i][j] = i == j ? 1.0f : 0.0f;
			j++;
		}
		i++;
	}
}

void matrix_multiply(
	roonium_matrix _matrix,
	roonium_matrix _a,
	roonium_matrix _b)
{
	roonium_matrix t;
	int k, r, c = 0;
	while (c < 4)
	{
		r = 0;
		while (r < 4)
		{
			k = 0;
			t[c][r] = 0.0f;
			while (k < 4)
			{
				t[c][r] += _a[k][r] * _b[c][k];
				k++;
			}
			r++;
		}
		c++;
	}

	k = 0;
	r = 0;
	while (k < 4)
	{
		r = 0;
		while (r < 4)
		{
			_matrix[k][r] = t[k][r];
			r++;
		}
		k++;
	}
}

void matrix_perspective(
	roonium_matrix _matrix,
	const float _fov,
	const float _aspect,
	const float _n,
	const float _f)
{
	float const a = 1.f / tan(_fov / 2.f);

	_matrix[0][0] = a / _aspect;
	_matrix[0][1] = 0.f;
	_matrix[0][2] = 0.f;
	_matrix[0][3] = 0.f;

	_matrix[1][0] = 0.f;
	_matrix[1][1] = a;
	_matrix[1][2] = 0.f;
	_matrix[1][3] = 0.f;

	_matrix[2][0] = 0.f;
	_matrix[2][1] = 0.f;
	_matrix[2][2] = -((_f + _n) / (_f - _n));
	_matrix[2][3] = -1.f;

	_matrix[3][0] = 0.f;
	_matrix[3][1] = 0.f;
	_matrix[3][2] = -((2.f * _f * _n) / (_f - _n));
	_matrix[3][3] = 0.f;
}

void matrix_translate_in_place(
	roonium_matrix _matrix,
	const float _x,
	const float _y,
	const float _z)
{
	struct roonium_vector4 r, t;
	int i = 0;

	t.x = _x;
	t.y = _y;
	t.z = _z;
	t.w = 0;

	while (i < 4)
	{
		r.x = _matrix[0][i];
		r.y = _matrix[1][i];
		r.z = _matrix[2][i];
		r.w = _matrix[3][i];

		_matrix[3][i] += vector4_multiply_inner(r, t);
		i++;
	}
}

void matrix_look_at(
	roonium_matrix _matrix,
	const struct roonium_vector3 _eye,
	const struct roonium_vector3 _center,
	const struct roonium_vector3 _up)
{
	struct roonium_vector3 f, s, t;

	f = vector3_subtract(_center, _eye);
	f = vector3_normalize(f);

	s = vector3_cross(f, _up);
	s = vector3_normalize(s);

	t = vector3_cross(s, f);

	_matrix[0][0] = s.x;
	_matrix[0][1] = t.x;
	_matrix[0][2] = -f.x;
	_matrix[0][3] = 0.0f;

	_matrix[1][0] = s.y;
	_matrix[1][1] = t.y;
	_matrix[1][2] = -f.y;
	_matrix[1][3] = 0.0f;

	_matrix[2][0] = s.z;
	_matrix[2][1] = t.z;
	_matrix[2][2] = -f.z;
	_matrix[2][3] = 0.0f;

	_matrix[3][0] = 0.0f;
	_matrix[3][1] = 0.0f;
	_matrix[3][2] = 0.0f;
	_matrix[3][3] = 1.0f;

	matrix_translate_in_place(
		_matrix,
		-_eye.x,
		-_eye.y,
		-_eye.z);
}

void matrix_rotate_y(
	roonium_matrix _destination,
	roonium_matrix _source,
	const float _angle)
{
	roonium_matrix rotate;

	float s = sin(_angle);
	float c = cos(_angle);

	matrix_identity(rotate);
	rotate[0][0] = c;
	rotate[0][2] = -s;
	rotate[2][0] = s;
	rotate[2][2] = c;

	matrix_multiply(
		_destination,
		_source,
		rotate);
}

#endif