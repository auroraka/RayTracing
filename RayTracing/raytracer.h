#ifndef RAYTRACER_H
#define RAYTRACER_H

#include"scene.h"
#include "basic.h"
#include<string>


namespace tl {
	const double SPEC_POWER = 20;
	const int MAX_DREFL_DEP = 2;
	const int MAX_RAYTRACING_DEP = 10;


	class Raytracer {
		Scene scene;

		Color CalnDiffusion(Object* pri) {
			Color color = pri->GetMaterial()->color;
			if (pri->GetMaterial()->texture != NULL) color = color * pri->GetTexture();

			Color ret = color * scene.GetBackgroundColor() * pri->GetMaterial()->diff;

			for (Light* light = scene.GetLightHead(); light != NULL; light = light->GetNext()) {
				double shade = light->CalnShade(pri->crash.C, scene.GetPrimitiveHead(), scene.GetCamera()->GetShadeQuality());
				if (shade < EPS) continue;

				Vector3 R = (light->GetO() - pri->crash.C).GetUnitVector();
				double dot = R.Dot(pri->crash.N);
				if (dot > EPS) {

					if (pri->GetMaterial()->diff > EPS) {
						double diff = pri->GetMaterial()->diff * dot * shade;
						ret += color * light->GetColor() * diff;
					}
					if (pri->GetMaterial()->spec > EPS) {
						double spec = pri->GetMaterial()->spec * pow(dot, SPEC_POWER) * shade;
						ret += color * light->GetColor() * spec;
					}
				}
			}

			return ret;
		}

		Color CalnReflection(Object* pri, Vector3 ray_V, int dep) {
			ray_V = ray_V.Reflect(pri->crash.N);

			if (pri->GetMaterial()->drefl < EPS || dep > MAX_DREFL_DEP)
				return RayTracing(pri->crash.C, ray_V, dep + 1) * pri->GetMaterial()->color * pri->GetMaterial()->refl;

			Vector3 Dx = ray_V * Vector3(1, 0, 0);
			if (Dx.IsZeroVector()) Dx = Vector3(1, 0, 0);
			Vector3 Dy = ray_V * Dx;
			Dx = Dx.GetUnitVector() * pri->GetMaterial()->drefl;
			Dy = Dy.GetUnitVector() * pri->GetMaterial()->drefl;

			Color ret;
			for (int k = 0; k < 16 * scene.GetCamera()->GetDreflQuality(); k++) {
				double x, y;
				do {
					x = ran() * 2 - 1;
					y = ran() * 2 - 1;
				} while (x * x + y * y > 1);
				x *= pri->GetMaterial()->drefl;
				y *= pri->GetMaterial()->drefl;

				ret += RayTracing(pri->crash.C, ray_V + Dx * x + Dy * y, dep + MAX_DREFL_DEP);
			}

			ret = ret * pri->GetMaterial()->color * pri->GetMaterial()->refl / (16 * scene.GetCamera()->GetDreflQuality());
			return ret;
		}

		Color CalnRefraction(Object* pri, Vector3 ray_V, int dep) {
			double n = pri->GetMaterial()->rindex;
			if (pri->crash.front) n = 1 / n;

			ray_V = ray_V.Refract(pri->crash.N, n);

			Color rcol = RayTracing(pri->crash.C, ray_V, dep + 1);
			if (pri->crash.front) return rcol * pri->GetMaterial()->refr;
			Color absor = pri->GetMaterial()->absor * -pri->crash.dist;
			Color trans = Color(exp(absor.r), exp(absor.g), exp(absor.b));
			return rcol * trans * pri->GetMaterial()->refr;
		}

		Color RayTracing(Vector3 ray_O, Vector3 ray_V, int dep) {
			if (dep > MAX_RAYTRACING_DEP) return Color();

			Color ret;
			Object* nearest_primitive = scene.FindNearestPrimitive(ray_O, ray_V);
			Light* nearest_light = scene.FindNearestLight(ray_O, ray_V);

			if (nearest_light != NULL && (nearest_primitive == NULL || nearest_light->crash_dist < nearest_primitive->crash.dist)) {
				ret += nearest_light->GetColor();
			}

			if (nearest_primitive != NULL) {
				Object* obj = nearest_primitive->PrimitiveCopy();
				if (obj->GetMaterial()->diff > EPS || obj->GetMaterial()->spec > EPS) ret += CalnDiffusion(obj);
				if (obj->GetMaterial()->refl > EPS) ret += CalnReflection(obj, ray_V, dep);
				if (obj->GetMaterial()->refr > EPS) ret += CalnRefraction(obj, ray_V, dep);
				delete obj;
			}

			ret.Confine();
			return ret;
		}


	public:
		Raytracer() {}
		~Raytracer() {}

		void run() {
			Camera* camera = scene.GetCamera();
			scene.CreateScene();

			Vector3 ray_O = camera->GetO();
			int H = camera->GetH(), W = camera->GetW();

			
			for (int i = 0; i < H; i++) {
				std::cout << i <<" / "<<H<< std::endl;
				for (int j = 0; j < W; j++) {
					Vector3 ray_V = camera->Emit(i, j);
					Color color = RayTracing(ray_O, ray_V, 1);
					scene.picture->SetColor(i, j, color);
				}
			}

			scene.picture->Output();
		}
	};

}

#endif
