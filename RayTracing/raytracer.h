#ifndef RAYTRACER_H
#define RAYTRACER_H

#include"scene.h"
#include "basic.h"
#include<string>


namespace tl {
	const double SPEC_POWER = 20;
	const int MAX_DREFL_DEEP = 2;
	const int MAX_RAYTRACING_DEP = 10;


	class Raytracer {
		Scene scene;

		Color getDiffusion(Object* pri) {
			Color color = pri->GetMaterial()->color;
			if (pri->GetMaterial()->texture != NULL) color = color * pri->GetTexture();

			Color ret = color * scene.GetBackgroundColor() * pri->GetMaterial()->diff;

			for (Light* light = scene.GetLightHead(); light != NULL; light = light->GetNext()) {
				double shade = light->getShade(pri->irst.C, scene.GetPrimitiveHead(), scene.GetCamera()->GetShadeQuality());
				if (shade < EPS) continue;

				Vector R = (light->GetO() - pri->irst.C).normal();
				double RN = R.dot(pri->irst.N);
				if (RN > EPS) {

					if (pri->GetMaterial()->diff > EPS) {
						double diff = pri->GetMaterial()->diff * RN * shade;
						ret += color * light->GetColor() * diff;
					}
					if (pri->GetMaterial()->spec > EPS) {
						double spec = pri->GetMaterial()->spec * pow(RN, SPEC_POWER) * shade;
						ret += color * light->GetColor() * spec;
					}
				}
			}

			return ret;
		}

		Color getReflection(Object* pri, Vector ray_V, int deep) {
			ray_V = ray_V.getReflectDir(pri->irst.N);

			if (pri->GetMaterial()->drefl < EPS || deep > MAX_DREFL_DEEP)
				return rayTracing(pri->irst.C, ray_V, deep + 1) * pri->GetMaterial()->color * pri->GetMaterial()->refl;

			Vector Dx = ray_V * Vector(1, 0, 0);
			if (Dx.isZero()) Dx = Vector(1, 0, 0);
			Vector Dy = ray_V * Dx;
			Dx = Dx.normal() * pri->GetMaterial()->drefl;
			Dy = Dy.normal() * pri->GetMaterial()->drefl;

			Color ret;
			for (int k = 0; k < 16 * scene.GetCamera()->GetDreflQuality(); k++) {
				double x, y;
				do {
					x = ran() * 2 - 1;
					y = ran() * 2 - 1;
				} while (x * x + y * y > 1);
				x *= pri->GetMaterial()->drefl;
				y *= pri->GetMaterial()->drefl;

				ret += rayTracing(pri->irst.C, ray_V + Dx * x + Dy * y, deep + MAX_DREFL_DEEP);
			}

			ret = ret * pri->GetMaterial()->color * pri->GetMaterial()->refl / (16 * scene.GetCamera()->GetDreflQuality());
			return ret;
		}

		Color getRefraction(Object* pri, Vector ray_V, int deep) {
			double n = pri->GetMaterial()->rindex;
			if (pri->irst.front) n = 1 / n;

			ray_V = ray_V.getRefractDir(pri->irst.N, n);

			Color rcol = rayTracing(pri->irst.C, ray_V, deep + 1);
			if (pri->irst.front) return rcol * pri->GetMaterial()->refr;
			Color absor = pri->GetMaterial()->absor * -pri->irst.dist;
			Color trans = Color(exp(absor.r), exp(absor.g), exp(absor.b));
			return rcol * trans * pri->GetMaterial()->refr;
		}

		Color rayTracing(Vector ray_O, Vector ray_V, int deep) {
			if (deep > MAX_RAYTRACING_DEP) return Color();

			Color ret;
			Object* nearest_primitive = scene.intersectWithObject(ray_O, ray_V);
			Light* nearest_light = scene.intersectWithLight(ray_O, ray_V);

			if (nearest_light != NULL && (nearest_primitive == NULL || nearest_light->crash_dist < nearest_primitive->irst.dist)) {
				ret += nearest_light->GetColor();
			}

			if (nearest_primitive != NULL) {
				Object* obj = nearest_primitive->PrimitiveCopy();
				if (obj->GetMaterial()->diff > EPS || obj->GetMaterial()->spec > EPS) ret += getDiffusion(obj);
				if (obj->GetMaterial()->refl > EPS) ret += getReflection(obj, ray_V, deep);
				if (obj->GetMaterial()->refr > EPS) ret += getRefraction(obj, ray_V, deep);
				delete obj;
			}

			ret.normalize();
			return ret;
		}


	public:
		Raytracer() {}
		~Raytracer() {}

		void run() {
			Camera* camera = scene.GetCamera();
			scene.createScene();

			Vector ray_O = camera->GetO();
			int H = camera->GetH(), W = camera->GetW();

			
			for (int i = 0; i < H; i++) {
				std::cout << i <<" / "<<H<< std::endl;
				for (int j = 0; j < W; j++) {
					Vector ray_V = camera->lookAt(i, j);
					Color color = rayTracing(ray_O, ray_V, 1);
					scene.picture->draw(i, j, color);
				}
			}

			scene.picture->write();
		}
	};

}

#endif
