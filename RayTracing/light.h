#ifndef LIGHT_H
#define LIGHT_H

#include "basic.h"
#include"vector3.h"
#include"color.h"
#include"object.h"
#include<sstream>
#include<string>
#include<cmath>

namespace tl {

	class Light {
	protected:
		int sample;
		Color color;
		Light* next;

	public:
		double crash_dist;
		Vector3 O, Dx, Dy;

		~Light() {}

		int GetSample() { return sample; }
		Color GetColor() { return color; }
		Light* GetNext() { return next; }
		void SetNext(Light* light) { next = light; }
		Vector3 GetO() { return O; }

		Light() {
			sample = rand();
			next = NULL;
		}

		bool Collide(Vector3 ray_O, Vector3 ray_V) {
			ray_V = ray_V.GetUnitVector();
			Vector3 N = (Dx * Dy).GetUnitVector();
			double d = N.Dot(ray_V);
			if (fabs(d) < EPS) return false;
			double l = (N * O.Dot(N) - ray_O).Dot(N) / d;
			if (l < EPS) return false;

			Vector3 C = (ray_O + ray_V * l) - O;
			if (fabs(Dx.Dot(C)) > Dx.Dot(Dx)) return false;
			if (fabs(Dy.Dot(C)) > Dy.Dot(Dy)) return false;

			crash_dist = l;
			return true;
		}

		double CalnShade(Vector3 C, Object* primitive_head, int shade_quality) {
			int shade = 0;

			for (int i = -2; i < 2; i++)
				for (int j = -2; j < 2; j++)
					for (int k = 0; k < shade_quality; k++) {
						Vector3 V = O - C + Dx * ((ran() + i) / 2) + Dy * ((ran() + j) / 2);
						double dist = V.Module();

						for (Object* now = primitive_head; now != NULL; now = now->GetNext())
							if (now->Collide(C, V) && (now->crash.dist < dist)) {
								shade++;
								break;
							}
					}

			return 1 - (double)shade / (16.0 * shade_quality);
		}

		void makeDemo() {
			O = Vector3(3, 3, 3);
			Dx = Vector3(1.5, 0, 0);
			Dy = Vector3(0, 1.5, 0);
			color = Color(1, 1, 1);
		}

	};

}
#endif
