#ifndef CAMERA_H
#define CAMERA_H

#include "basic.h"
#include"vector.h"
#include"color.h"
#include"picture.h"
#include<string>
#include<sstream>

namespace tl {
	const int SHADE_QUALITY = 4;
	const int DREFL_QUALITY = 4;


	class Camera {
		Vector O, N, Dx, Dy;
		double lens_W, lens_H;
		int W, H;

	public:

		Camera() {
			O = Vector(0, 0, 0);
			N = Vector(0, 1, 0);
		}

		void init() {
			N = N.normal();
			Dx = N.GetAnVerticalVector();
			Dy = Dx * N;
			Dx = Dx * lens_W / 2;
			Dy = Dy * lens_H / 2;

		}

		Vector lookAt(double i, double j) {
			return N + Dy * (2 * i / H - 1) + Dx * (2 * j / W - 1);
		}

		void makeDemo(int _H,int _W) {
			O = Vector(-2, 2, 0.5);
			N = Vector(0.6, 1, -0.6);
			H = _H;
			W = _W;
			lens_H = 0.5625;
			lens_W = 1;
		}

		Vector GetO() { return O; }
		int GetW() { return W; }
		int GetH() { return H; }
		double GetShadeQuality() { return SHADE_QUALITY; }
		double GetDreflQuality() { return DREFL_QUALITY; }

	};

}


#endif
