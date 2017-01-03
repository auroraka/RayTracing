#ifndef PICTURE_H
#define PICTURE_H

#include "color.h"
#include "vector.h"
#include <opencv2\opencv.hpp>
using namespace cv;

namespace tl {
	class Picture {
		int H, W;
		Mat pic;
		Color toColor(Vec3b a) {
			return Color((double)a[2] / 255, (double)a[1] / 255, (double)a[0] / 255);
		}
		Vec3b toVec3b(Color a) {
			return Vec3b(a.b * 255, a.g * 255, a.r * 255);
		}

	public:
		Picture(int H = 0, int W = 0) :H(H), W(W) {
			pic = Mat(H, W, CV_8UC3);
		}
		int GetH() { return H; }
		int GetW() { return W; }
		Color colorAt(int i, int j) { return toColor(pic.at<Vec3b>(H - 1 - i, j)); }
		void draw(int i, int j, Color color) {
			pic.at<Vec3b>(H - 1 - i, j) = toVec3b(color);
		}

		void init(int _H, int _W) {
			H = _H;
			W = _W;
			pic = Mat(H, W, CV_8UC3);
		}
		void read(std::string file) {
			pic = imread(file);
			H = pic.rows;
			W = pic.cols;
		}
		void write() {
			imwrite("output.jpg", pic);
		}
		Color colorSmoothAt(double u, double v) {
			double U = (u - floor(u)) * H;
			double V = (v - floor(v)) * W;
			int U1 = (int)floor(U + EPS), U2 = U1 + 1;
			int V1 = (int)floor(V + EPS), V2 = V1 + 1;
			double rat_U = U2 - U;
			double rat_V = V2 - V;
			if (U1 < 0) U1 = H - 1; if (U2 == H) U2 = 0;
			if (V1 < 0) V1 = W - 1; if (V2 == W) V2 = 0;
			Color ret;
			ret = ret + colorAt(U1, V1) * rat_U * rat_V;
			ret = ret + colorAt(U1, V2)* rat_U * (1 - rat_V);
			ret = ret + colorAt(U2, V1)* (1 - rat_U) * rat_V;
			ret = ret + colorAt(U2, V2)* (1 - rat_U) * (1 - rat_V);
			return ret;

		}

	};

}

#endif 