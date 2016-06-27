#ifndef LIGHT_H
#define LIGHT_H

#include "basic.h"
#include"vector.h"
#include"color.h"
#include"object.h"
#include<sstream>
#include<string>
#include<cmath>
#include <vector>

namespace tl {

	class Light {
	protected:
		int sample;
		Color color;

	public:
		double crash_dist;
		Vector O, Dx, Dy;

		~Light() {}

		int GetSample() { return sample; }
		Color GetColor() { return color; }
		Vector GetO() { return O; }

		Light() {
			sample = rand();
		}

		bool intersect(Vector ray_O, Vector ray_V) {
			ray_V = ray_V.normal();
			Vector N = (Dx * Dy).normal();
			double d = N.dot(ray_V);
			if (fabs(d) < EPS) return false;
			double l = (N * O.dot(N) - ray_O).dot(N) / d;
			if (l < EPS) return false;

			Vector C = (ray_O + ray_V * l) - O;
			if (fabs(Dx.dot(C)) > Dx.dot(Dx)) return false;
			if (fabs(Dy.dot(C)) > Dy.dot(Dy)) return false;

			crash_dist = l;
			return true;
		}

		double getShade(Vector C, std::vector<Object*> &objs, int shade_quality) {
			int shade = 0;

			for (int i = -2; i < 2; i++)
				for (int j = -2; j < 2; j++)
					for (int k = 0; k < shade_quality; k++) {
						Vector V = O - C + Dx * ((ran() + i) / 2) + Dy * ((ran() + j) / 2);
						double dist = V.len();

						for (int i = 0; i < objs.size(); i++) {
							if (objs[i]->intersect(C, V) && (objs[i]->irst.dist < dist)) {
								shade++;
								break;
							}
						}
					}

			return 1 - (double)shade / (16.0 * shade_quality);
		}

		void makeDemo() {
			O = Vector(3, 3, 3);
			Dx = Vector(1.5, 0, 0);
			Dy = Vector(0, 1.5, 0);
			color = Color(1, 1, 1);
		}

	};

}
#endif
