#ifndef CAMERA_H
#define CAMERA_H

#include "basic.h"
#include"vector3.h"
#include"color.h"
#include"picture.h"
#include<string>
#include<sstream>

namespace tl {
	const int SHADE_QUALITY = 4;
	const int DREFL_QUALITY = 4;


	class Camera {
		Vector3 O, N, Dx, Dy;
		double lens_W, lens_H;
		int W, H;

	public:

		Camera() {
			O = Vector3(0, 0, 0);
			N = Vector3(0, 1, 0);
		}

		void Initialize() {
			N = N.GetUnitVector();
			Dx = N.GetAnVerticalVector();
			Dy = Dx * N;
			Dx = Dx * lens_W / 2;
			Dy = Dy * lens_H / 2;

		}

		Vector3 Emit(double i, double j) {
			return N + Dy * (2 * i / H - 1) + Dx * (2 * j / W - 1);
		}

		/*void Output(Picture* bmp) {
			bmp->Initialize(H, W);

			for (int i = 0; i < H; i++)
				for (int j = 0; j < W; j++)
					bmp->SetColor(i, j, data[i][j]);
		}*/

		void makeDemo(int _H,int _W) {
			O = Vector3(-2, 2, 0.5);
			N = Vector3(0.6, 1, -0.6);
			H = _H;
			W = _W;
			//lens_H = 0.6;
			lens_H = 1;
			lens_W = 1;
		}

		Vector3 GetO() { return O; }
		int GetW() { return W; }
		int GetH() { return H; }
		double GetShadeQuality() { return SHADE_QUALITY; }
		double GetDreflQuality() { return DREFL_QUALITY; }

	};

}


#endif
