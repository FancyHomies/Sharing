#pragma once
#include "Geometry.h"

namespace PathTracing
{

vec3 radiance(const Ray& r, int depth)
{
	double t = 1e20;			// distance to intersection
	int id = -1;				// id of intersected object
	Sphere* pobj;
	if (!intersect(r, t, pobj))	// if miss, return black
		return vec3(0.,0.,0.);

	const Sphere& obj = *pobj;        // the hit object

	vec3 x = r.getPosition(t);
	vec3 n = normalize(x - obj.O);
	vec3 nl = dot(n, r.D) < 0. ? n : n * -1.;
	vec3 f = obj.m.Albedo;

	double p = f.x > f.y && f.x > f.z ? f.x : f.y > f.z ? f.y : f.z; // max refl

	if (++depth > 5)
	{
		if (randx() < p)
			f = f * (1 / p);
		else
			return obj.m.Emissive; //R.R.
	}

	if (depth > 100)
		return obj.m.Emissive;

	if (obj.m.roughness > 0) // Ideal DIFFUSE reflection 
	{
		double theta = randx() * 0.5 * PI * obj.m.roughness;
		double phi = randx() * 2. * PI;

		Ray ref(x, spheredir(n, theta, phi));

		return obj.m.Emissive + f * radiance(ref, depth);
	}
	else if (obj.m.roughness == 0 && obj.m.refractive == 0 ) // Ideal SPECULAR reflection
	{
		return obj.m.Emissive + f * radiance(Ray(x, reflect(r.D, n)), depth);
	}
	else // Ideal dielectric REFRACTION
	{
		//       L \θc| N/ R
		//        	\ | /
		// ηc        \|/		glass
		// -----------|--------------
		// ηt      	  | ╲		air
		//            |θt ╲
		//            |     ╲ T
		// ηc * sinθc = ηt * sinθt 

		// nc * sin(c) = nt * sin(t)
		// nc*nc * sin2c = nt*nt * sin2t
		// nc2 * (1-cos2c) = nt2 * (1-cos2t)
		// cos2t = nc2/nt2 * (1-cos2c)

		Ray reflRay(x, reflect(r.D, n));
		bool into = dot(n, nl) > 0; // Ray from outside going in? into : 真空到玻璃
		double nc = 1; // 真空
		double nt = 1.5; // 玻璃
		double nnt = into ? nc / nt : nt / nc;
		double cosc = dot(r.D, nl); // cosc

		double cos2t = 1 - nnt * nnt * (1 - cosc * cosc);
		// Total internal reflection 
		// 玻璃内部入射角太大折射不出来，只能内部反射
		if (cos2t < 0) 
			return obj.m.Emissive + f * radiance(reflRay, depth);

		// 求折射角
		vec3 tdir = normalize(r.D * nnt - n * ((into ? 1 : -1) * (cosc * nnt + sqrt(cos2t))));

		// Fresnel
		// R(θ) ≈ R(0°) + (1-R(0°))(1-cosθ)^5    /* θ = 偏离Z的角度 */ Schlick 1994 菲涅尔反照率近似
		// R(0°) = ((n1-n2)/(n1+n2))^2 
		// 
		// LR = R(θ) * L
		// LT = (1-R(θ))(nt/nc)^2 * L

		double cost = dot(tdir, n);
		double a = nt - nc;
		double b = nt + nc;
		double R0 = a * a / (b * b); // 0.25 / 2.5 * 2.5
		// double c = 1 - (into ? -cosc : cost);
		double c = into ? 1 + cosc : 1 - cost;

		double Re = R0 + (1 - R0) * c * c * c * c * c;


		double Tr = 1 - Re; // (1-Re)*nnt2
		double P = .25 + .5 * Re;
		double RP = Re / P;
		double TP = Tr / (1 - P);

		vec3 tc;
		if (depth > 2)
		{
			if (randx() < P)
				tc = radiance(reflRay, depth) * RP;
			else
				tc = radiance(Ray(x, tdir), depth) * TP;
		}
		else
		{
			tc = radiance(reflRay, depth)* Re + radiance(Ray(x, tdir), depth) * Tr;
		}

		return obj.m.Emissive + f * tc;
	}
};

void render()
{
	double offset = 1.0f / SIZE;
	static const size_t samplecount = 100;

	for (size_t i = 0; i < SIZE; i++)
	{
		for (size_t j = 0; j < SIZE; j++)
		{
			vec3 sum(0.,0.,0.);
			for (size_t s = 0; s < samplecount; s++)
			{
				Ray r = cam.generateRay(offset * (i + randx()*0.5 - 0.25), offset * (j + randx() * 0.5 - 0.25));
				sum = sum + radiance(r, 0);
			}

			framebuffer[i][j] = (uint32_t)(sum / (double)samplecount);
		}
	}

}

}