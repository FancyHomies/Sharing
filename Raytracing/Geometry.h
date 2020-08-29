#pragma once

#include"Math.h"

class Ray
{
public:
	vec3 O;
	vec3 D;
	Ray() = default;
	Ray(const vec3& o, const vec3& d) : O(o), D(d) {}
	vec3 getPosition(double t) const { return O + D * t; }
};

class Material
{
public:
	vec3 Albedo;
	vec3 Emissive;
	double roughness;
	double refractive;

	Material() = default;
	Material(const vec3& a, const vec3& e, double ro, double re)
		: Albedo(a), Emissive(e), roughness(ro), refractive(re) {}
};

class Sphere
{
public:
	vec3	O;
	double	R;
	Material	m;

	Sphere() = default;
	Sphere(const vec3& o, double r, const vec3& a, const vec3& e, double ro = 0, double re = 0) : O(o), R(r), m(a, e, ro, re) {}

	vec3 getNormal(const vec3& pos) const { return normalize(pos - O); }

	double intersect(const Ray& r) const
	{
		vec3 Op = O - r.O;
		double a = dot(r.D, Op);
		double rhs = a * a + (R * R - dot(Op, Op));

		if (rhs >= 0)
		{
			double sqrtrhs = sqrt(rhs);
			return a > sqrtrhs ? a - sqrtrhs : a + sqrtrhs;
		}

		return Inf;
	}
};

class Intersect
{
public:
	Ray ray; 
	double t = 0.f;
	Sphere* sph = nullptr;
		
	vec3 pos;
	vec3 nor;

	Intersect() = default;
	Intersect(const Ray& r, Sphere* g, double t_) :ray(r), sph(g), t(t_) {}

	void calc()
	{
		pos = ray.getPosition(t);
		nor = sph->getNormal(pos);
	}
};

class Camera
{
public:
	vec3 Pos, Dir;
	double Fov;
	vec3 right, up;
	double fovscale;

	Camera() {};

	Camera(const vec3& pos, const vec3& dir, double fov)
		: Pos(pos), Dir(dir), Fov(fov) {}

	void init()
	{
		right = normalize(cross(Dir, vec3(0., 0., 1.)));
		up = normalize(cross(right, Dir));
		fovscale = tan(0.5 * Fov * PI / 180.0) * 2.0;
	}

	Ray generateRay(double x, double y)
	{
		vec3 r = right * ((x - 0.5) * fovscale);
		vec3 u = up * ((0.5 - y) * fovscale);
		return Ray(Pos, normalize(Dir + r + u));
	}
};

Camera cam(vec3(0., 290., 0.), normalize(vec3(0., -1., 0.)), 45.);

// Scene
Sphere sen[9] = {
	Sphere(vec3(0,0,1e3 + 99),		1e3,	vec3(),				vec3(10,10,10), 1,		0),		//Lite
	Sphere(vec3(-30,0,-30),			30,		vec3(1,1,1) * .999,	vec3(),			0,		0),		//Mirr
	Sphere(vec3(50,-50,-30),		30,		vec3(1,1,1) * .999,	vec3(),			0.1,	0.9),	//Glas
	Sphere(vec3(-1e5 - 100,0,0),	1e5,	vec3(.75,.25,.25),	vec3(),			1,		0),		//Left
	Sphere(vec3(1e5 + 100,0,0),		1e5,	vec3(.25,.25,.75),	vec3(),			1,		0),		//Rght
	Sphere(vec3(0, -1e5 - 200,0),	1e5,	vec3(.5,.5,.5),		vec3(),			1,		0),		//Back
	Sphere(vec3(0,1e5 + 300,0),		1e5,	vec3(),				vec3(),			1,		0),		//Frnt
	Sphere(vec3(0, 0, -1e5 - 100),	1e5,	vec3(.75,.75,.75),	vec3(),			1,		0),		//Botm
	Sphere(vec3(0, 0, 1e5 + 100),	1e5,	vec3(.75,.75,.75),	vec3(),			1,		0),		//Top
};

bool intersect(const Ray& r, double& t, Sphere*& geo)
{
	t = Inf;

	for (auto& o : sen)
	{
		double t1 = o.intersect(r);
		if (t1 >= 0 && t1 < t)
		{
			t = t1;
			geo = &o;
		}
	}

	return t < Inf;
}

#define SIZE 1024
uint32_t framebuffer[SIZE][SIZE];