#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include"color.h"
#include"vector3.h"
#include "picture.h"
#include "material.h"
#include "basic.h"
#include<iostream>
#include<sstream>
#include<string>


namespace tl {

	struct Crash {
		Vector3 N, C;
		double dist;
		bool front;
	};



	class Object {
	protected:
		int sample;
		Material* material;
		Object* next;

	public:
		Crash crash;

		Object() {
			sample = rand();
			material = new Material;
			next = NULL;
		}
		Object(const Object& obj) {
			*this = obj;
			material = new Material;
			*material = *obj.material;
		}
		~Object() {
			delete material;
		}

		int GetSample() { return sample; }
		Material* GetMaterial() { return material; }
		Object* GetNext() { return next; }
		void SetNext(Object* obj) { next = obj; }

		virtual bool Collide(Vector3 ray_O, Vector3 ray_V) = 0;
		virtual Color GetTexture() = 0;
		virtual Object* PrimitiveCopy() = 0;
		virtual void makeDemo(int i) = 0;
	};






	class Sphere : public Object {
		Vector3 O, De, Dc;
		double R;

	public:
		Sphere() : Object() {
			De = Vector3(0, 0, 1);
			Dc = Vector3(0, 1, 0);
		}
		~Sphere() {}

		void Input(std::string, std::stringstream&);
		bool Collide(Vector3 ray_O, Vector3 ray_V) {
			ray_V = ray_V.GetUnitVector();
			Vector3 P = ray_O - O;
			double b = -P.Dot(ray_V);
			double det = b * b - P.Module2() + R * R;

			if (det > EPS) {
				det = sqrt(det);
				double x1 = b - det, x2 = b + det;

				if (x2 < EPS) return false;
				if (x1 > EPS) {
					crash.dist = x1;
					crash.front = true;
				}
				else {
					crash.dist = x2;
					crash.front = false;
				}
			}
			else
				return false;

			crash.C = ray_O + ray_V * crash.dist;
			crash.N = (crash.C - O).GetUnitVector();
			if (crash.front == false) crash.N = -crash.N;
			return true;
		}

		Color GetTexture() {
			Vector3 I = (crash.C - O).GetUnitVector();
			double a = acos(-I.Dot(De));
			double b = acos(std::min(std::max(I.Dot(Dc) / sin(a), -1.0), 1.0));
			double u = a / PI, v = b / 2 / PI;
			if (I.Dot(Dc * De) < 0) v = 1 - v;
			return material->texture->GetSmoothColor(u, v);
		}

		Object* PrimitiveCopy() {
			Sphere* ret = new Sphere(*this);
			return ret;
		}

		void makeDemo(int i) {
			std::cout << "make sphere " << i << std::endl;
			if (i == 0) {
				O = Vector3(0, 8, -1.5);
				R = 0.5;
				material->refr = 1;
				material->rindex = 1.7;
				material->absor = Color(0, 0, 0);
			}
			if (i == 1) {
				O = Vector3(3, 8, -1.5);
				R = 0.5;
				material->refr = 0.9;
				material->rindex = 1;
				material->absor = Color(1, 0, 0);
			}
			if (i == 2) {
				O = Vector3(6, 9, -1.5);
				R = 0.4;
				material->refr = 0.5;
				material->rindex = 1.5;
				material->absor = Color(0, 0, 1);
				material->color = Color(0.5, 0.5, 1);
				material->spec = 0.1;
				material->refl = 0.4;
			}
			if (i == 3) {
				O = Vector3(1.5, 6.3, -1.6);
				R = 0.4;
				material->color = Color(0.5, 0.5, 1);
				material->spec = 0.2;
				material->refl = 0.8;
			}
			if (i == 4) {
				O = Vector3(0, 6, -1.5);
				R = 0.5;
				De = Vector3(0, 0, 1);
				Dc = Vector3(0, 1, 0);
				material->color = Color(1, 1, 1);
				material->diff = 0.15;
				material->spec = 0.15;
				material->refl = 0.8;
				material->drefl = 0.25;
				//material->texture = new Picture;
				//material->texture->Input("test2.jpg");
			}
		}
	};

	class Plane : public Object {
		Vector3 N, Dx, Dy;
		double R;

	public:
		Plane() : Object() {}
		~Plane() {}

		bool Collide(Vector3 ray_O, Vector3 ray_V) {
			ray_V = ray_V.GetUnitVector();
			N = N.GetUnitVector();
			double d = N.Dot(ray_V);
			if (fabs(d) < EPS) return false;
			double l = (N * R - ray_O).Dot(N) / d;
			if (l < EPS) return false;

			crash.dist = l;
			crash.front = (d < 0);
			crash.C = ray_O + ray_V * crash.dist;
			crash.N = (crash.front) ? N : -N;
			return true;
		}

		Color GetTexture() {
			double u = crash.C.Dot(Dx) / Dx.Module2();
			double v = crash.C.Dot(Dy) / Dy.Module2();
			return material->texture->GetSmoothColor(u, v);
		}

		Object* PrimitiveCopy() {
			Plane* ret = new Plane(*this);
			return ret;
		}

		void makeDemo(int i) {
			std::cout << "make plane" << std::endl;
			N = Vector3(0, 0, 1);
			R = -2;
			Dx = Vector3(8, 0, 0);
			Dy = Vector3(0, 8, 0);
			material->color = Color(1, 1, 1);
			material->diff = 0.3;
			material->refl = 0.7;
			material->drefl = 0.15;
			material->texture = new Picture;
			material->texture->Input("test1.jpg");


		}

	};


}
#endif
