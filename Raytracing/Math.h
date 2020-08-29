#pragma once
// Learn from : http://www.kevinbeason.com/smallpt/
// https://zhuanlan.zhihu.com/p/72673165?utm_source=wechat_session

#define _CRT_SECURE_NO_WARNINGS
#include <math.h>   
#include <stdlib.h> 
#include <stdio.h> 
#include <time.h>
#include <iostream>

using namespace std;
#define Inf	1e20
#define PI	3.141592653589793238462643

template<class T>
T clamp(T a, T min = 0, T max = 1)
{
	return a > max ? max : a < min ? min : a;
}

class vec3
{
public:
	double x = .0, y = .0, z = .0;
public:
	vec3() = default;
	vec3(double x, double y, double z) : x(x), y(y), z(z) {}
	vec3(const vec3& v) : x(v.x), y(v.y), z(v.z) {}

	const static vec3 zero, one, X, Y, Z;

public:
	vec3 operator - () const
	{
		return vec3(-x, -y, -z);
	}

	bool operator == (const vec3& v) const
	{
		return x == v.x && y == v.y && z == v.z;
	}

	vec3 operator + (const vec3& v) const
	{
		return vec3(x + v.x, y + v.y, z + v.z);
	}

	vec3 operator - (const vec3& v) const
	{
		return vec3(x - v.x, y - v.y, z - v.z);
	}

	vec3 operator * (double f) const
	{
		return vec3(x * f, y * f, z * f);
	}

	vec3 operator / (double f) const
	{
		return vec3(x / f, y / f, z / f);
	}

	vec3 operator * (const vec3& v) const
	{
		return vec3(x * v.x, y * v.y, z * v.z);
	}

	vec3 operator / (const vec3& v) const
	{
		return vec3(x / v.x, y / v.y, z / v.z);
	}

	explicit operator uint32_t () const
	{
		return 0xFF000000 | (uint8_t(clamp(x) * 255.) << 16) | (uint8_t(clamp(y) * 255.) << 8) | uint8_t(clamp(z) * 255.);
	}
};

double dot(const vec3& v1, const vec3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

vec3 cross(const vec3& v1, const vec3& v2)
{
	return vec3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

vec3 normalize(const vec3& v)
{
	return v * (1.0 / sqrt(dot(v, v)));
}

vec3 lerp(const vec3& v1, const vec3& v2, double f)
{
	return v1 + (v2 - v1) * f;
}

vec3 reflect(const vec3& I, const vec3& N)
{
	return I - N * 2 * dot(I, N);
}

vec3 refract(const vec3& I, const vec3& N, double eta)
{
	double cosi = -dot(I, N);
	double cost2 = 1 - eta * eta * (1 - cosi * cosi);
	vec3 t = I * eta + N * (eta * cosi - sqrt(cost2));
	return t * double(cost2 > 0 ? 1 : -1);
}

vec3 sph2xyz(double theta, double phi) // theta = Æ«ÀëzµÄ½Ç¶È
{
	return vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
}

vec3 spheredir(const vec3& Z, double theta, double phi)
{
	vec3 X = Z.x < 1.0f ? vec3::X : vec3::Y;
	vec3 Y = normalize(cross(Z, X));
	X = normalize(cross(Y, Z));

	vec3 n = sph2xyz(theta, phi);
	vec3 n2 = vec3(dot(n, vec3(X.x, Y.x, Z.x)), dot(n, vec3(X.y, Y.y, Z.y)), dot(n, vec3(X.z, Y.z, Z.z)));
	return n2;
}

double randx(double a = 0., double b = 1.)
{
	double x = (double)rand() / (double)RAND_MAX;
	return a + (b - a) * x;
}