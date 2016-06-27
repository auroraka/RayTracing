#ifndef BASIC_H
#define BASIC_H

namespace tl {
	const static double EPS = 1e-6;
	const static double INF = 1e100;
	const static double PI = 3.1415926536897931;

	/*int dcmp(const double &a, double b = 0) {
		if (a - b>EPS) return 1;
		if (b - a>EPS) return -1;
		return 0;
	}
	double round(double x, int k) {
		for (int i = 1; i <= k; i++) x *= 10;
		x = floor(x + 0.5);
		for (int i = 1; i <= k; i++) x /= 10;
		return x;
	}*/

	inline double sqr(double a) {
		return a*a;
	}
	inline double ran() {
		return double(rand())/RAND_MAX;
	}


	enum Type { _ColorType, _RayType, _SphereType, _PlaneType, _VectorType, _CameraType, _ObjectType, _TriangleType };

}

#endif 