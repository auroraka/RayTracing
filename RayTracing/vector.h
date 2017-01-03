#ifndef VECTOR3_H
#define VECTOR3_H

#include<sstream>
#include "basic.h"


namespace tl {

	class Vector {
	public:
		double x, y, z;

		Vector(double X = 0, double Y = 0, double Z = 0) : x(X), y(Y), z(Z) {}
		~Vector() {}

		friend Vector operator + (const Vector&, const Vector&);
		friend Vector operator - (const Vector&, const Vector&);
		friend Vector operator * (const Vector&, const double&);
		friend Vector operator / (const Vector&, const double&);
		friend Vector operator * (const Vector&, const Vector&); //cross product
		friend Vector& operator += (Vector&, const Vector&);
		friend Vector& operator -= (Vector&, const Vector&);
		friend Vector& operator *= (Vector&, const double&);
		friend Vector& operator *= (Vector&, const Vector&);
		friend Vector operator - (const Vector&);
		
		double dot(const Vector& term) {
			return x * term.x + y * term.y + z * term.z;
		}

		double len2() {
			return x * x + y * y + z * z;
		}

		double len() {
			return sqrt(x * x + y * y + z * z);
		}

		double dis2(Vector& term) {
			return (term - *this).len2();
		}

		double dis(Vector& term) {
			return (term - *this).len();
		}

		
		Vector normal() {
			return *this / len();
		}

	
		Vector GetAnVerticalVector() {
			Vector ret = *this * Vector(0, 0, 1);
			if (ret.isZero()) ret = Vector(1, 0, 0);
			else ret = ret.normal();
			return ret;
		}

		bool isZero() {
			return fabs(x) < EPS && fabs(y) < EPS && fabs(z) < EPS;
		}

		Vector getReflectDir(Vector N) {
			return *this - N * (2 * dot(N));
		}

		Vector getRefractDir(Vector N, double n) {
			Vector V = normal();
			double cosI = -N.dot(V), cosT2 = 1 - (n * n) * (1 - cosI * cosI);
			if (cosT2 > EPS) return V * n + N * (n * cosI - sqrt(cosT2));
			return V.getReflectDir(N);
		}

		Vector diffuse() {
			Vector Vert = GetAnVerticalVector();
			double theta = acos(sqrt(ran()));
			double phi = ran() * 2 * PI;
			return rotate(Vert, theta).rotate(*this, phi);
		}

		Vector rotate(Vector axis, double theta) {
			Vector ret;
			double cost = cos(theta);
			double sint = sin(theta);
			ret.x += x * (axis.x * axis.x + (1 - axis.x * axis.x) * cost);
			ret.x += y * (axis.x * axis.y * (1 - cost) - axis.z * sint);
			ret.x += z * (axis.x * axis.z * (1 - cost) + axis.y * sint);
			ret.y += x * (axis.y * axis.x * (1 - cost) + axis.z * sint);
			ret.y += y * (axis.y * axis.y + (1 - axis.y * axis.y) * cost);
			ret.y += z * (axis.y * axis.z * (1 - cost) - axis.x * sint);
			ret.z += x * (axis.z * axis.x * (1 - cost) - axis.y * sint);
			ret.z += y * (axis.z * axis.y * (1 - cost) + axis.x * sint);
			ret.z += z * (axis.z * axis.z + (1 - axis.z * axis.z) * cost);
			return ret;
		}

	};

	Vector operator + (const Vector& A, const Vector& B) {
		return Vector(A.x + B.x, A.y + B.y, A.z + B.z);
	}

	Vector operator - (const Vector& A, const Vector& B) {
		return Vector(A.x - B.x, A.y - B.y, A.z - B.z);
	}

	Vector operator * (const Vector& A, const double& k) {
		return Vector(A.x * k, A.y * k, A.z * k);
	}

	Vector operator / (const Vector& A, const double& k) {
		return Vector(A.x / k, A.y / k, A.z / k);
	}

	Vector operator * (const Vector& A, const Vector& B) {
		return Vector(A.y * B.z - A.z * B.y, A.z * B.x - A.x * B.z, A.x * B.y - A.y * B.x);
	}

	Vector& operator += (Vector& A, const Vector& B) {
		A = A + B;
		return A;
	}

	Vector& operator -= (Vector& A, const Vector& B) {
		A = A - B;
		return A;
	}

	Vector& operator *= (Vector& A, const double& k) {
		A = A * k;
		return A;
	}

	Vector& operator += (Vector& A, const double& k) {
		A = A / k;
		return A;
	}

	Vector& operator *= (Vector& A, const Vector& B) {
		A = A * B;
		return A;
	}
	Vector operator - (const Vector& A) {
		return Vector(-A.x, -A.y, -A.z);
	}

}

#endif
