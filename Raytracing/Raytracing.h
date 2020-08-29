#pragma once
#include "Geometry.h"

namespace RayTracing
{
	#define STEP 11
	static size_t samplecount = 1;

	vec3 radiance(const Ray& r, uint32_t depth)
	{
		depth++;

		Sphere* geo = nullptr;
		double t = Inf;
		if (!intersect(r, t, geo)) // Ã»¶«Î÷
			return vec3::zero;

		if (depth > 100)
			return geo->m.Emissive;

		Intersect it(r, geo, t);
		it.calc();

#if STEP >= 4
		vec3 lpos = vec3(0., 0., 99.99);
		vec3 dis = lpos - it.pos;
		vec3 pdir = normalize(dis);
		double l2 = dot(dis, dis);
		double lu = 20000.0 / l2;
		double lambert = max(0.0, dot(it.nor, pdir));
#endif

#if STEP == 0
		return it.pos;
#elif STEP == 1
		return normalize(it.pos);
#elif STEP == 2
		return it.nor;
#elif STEP == 3
		return vec3::one * dot(it.nor, vec3::Y);
#elif STEP == 4
		return vec3::one * lambert;
#elif STEP == 5
		return vec3::one * lu * lambert;
#elif STEP == 6
		return geo->m.Albedo * lu * lambert;
#elif STEP == 7
		if (geo->m.roughness == 0.0f)
		{
			return geo->m.Emissive + geo->m.Albedo * radiance(Ray(it.pos, reflect(r.D, it.nor)), depth);
		}
		else 
		{
			return geo->m.Emissive + geo->m.Albedo * lu * dot(it.nor, pdir);
		}
#elif STEP == 8
		Ray rlight(it.pos + pdir, pdir);
		Sphere* geo2 = nullptr;
		double t2 = Inf;
		intersect(rlight, t2, geo2);

		if ((geo2->m.Emissive == vec3(0.f, 0.f, 0.f)))
			lambert *= 0.5;

		return geo->m.Emissive + geo->m.Albedo * lu * lambert;
#elif STEP == 9
		double theta = randx() * 0.1f * PI * 0.5f;
		double phi = randx() * 2.f * PI;

		Ray rlight(it.pos + pdir, spheredir(pdir, theta, phi));
		Sphere* geo2 = nullptr;
		double t2 = Inf;
		intersect(rlight, t2, geo2);

		if ((geo2->m.Emissive == vec3(0.f, 0.f, 0.f)))
			lambert *= 0.5f;

		return geo->m.Emissive + geo->m.Albedo * lu * lambert;
#elif STEP == 10
		if (geo->m.roughness == 0.0f)
		{
			return geo->m.Emissive + geo->m.Albedo * radiance(Ray(it.pos, reflect(r.D, it.nor)), depth);
		}
		else
		{
			double theta = randx() * 0.1f * PI * 0.5f;
			double phi = randx() * 2.f * PI;

			Ray rlight(it.pos + pdir, spheredir(pdir, theta, phi));
			Sphere* geo2 = nullptr;
			double t2 = Inf;
			intersect(rlight, t2, geo2);

			if ((geo2->m.Emissive == vec3(0.f, 0.f, 0.f)))
				lambert *= 0.5f;

			return geo->m.Emissive + geo->m.Albedo * lu * lambert;
		}
#elif STEP == 11
		samplecount = 10;

		if (geo->m.Albedo == vec3::zero)
			return geo->m.Emissive;

		double rough = geo->m.roughness;
		if (rough == 0.0f)
		{
			return geo->m.Emissive + geo->m.Albedo * radiance(Ray(it.pos, reflect(r.D, it.nor)), depth);
		}
		else
		{
			double theta = randx() * 0.5 * PI * rough;
			double phi = randx() * 2.f * PI;

			Ray ref(it.pos, spheredir(it.nor, theta, phi));

			return geo->m.Emissive + geo->m.Albedo * radiance(ref, depth);
		}
#endif
	}

	void render()
	{
		double offset = 1.0f / SIZE;

		for (size_t i = 0; i < SIZE; i++)
		{
			for (size_t j = 0; j < SIZE; j++)
			{
				vec3 sum(0., 0., 0.);
				for (size_t s = 0; s < samplecount; s++)
				{
					Ray r = cam.generateRay(offset * (i + randx() * 0.5 - 0.25), offset * (j + randx() * 0.5 - 0.25));
					sum = sum + radiance(r, 0);
				}

				framebuffer[i][j] = (uint32_t)(sum / (double)samplecount);
			}
		}
	}

};