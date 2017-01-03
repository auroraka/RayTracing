#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include"color.h"
#include"vector.h"
#include "picture.h"
#include "material.h"
#include "basic.h"
#include<iostream>
#include<sstream>
#include<string>


namespace tl {

	struct IntersectResult {
		Vector N, C;
		double dist;
		bool front;
	};



	class Object {
	protected:
		int sample;
		Material* material;
		Object* next;

	public:
		IntersectResult irst;

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

		virtual bool intersect(Vector ray_O, Vector ray_V) = 0;
		virtual Color GetTexture() = 0;
		virtual Object* PrimitiveCopy() = 0;
		virtual void makeDemo(int i) = 0;
	};






	class Sphere : public Object {
		Vector O, De, Dc;
		double R;

	public:
		Sphere() : Object() {
			De = Vector(0, 0, 1);
			Dc = Vector(0, 1, 0);
		}
		~Sphere() {}

		bool intersect(Vector ray_O, Vector ray_V) {
			ray_V = ray_V.normal();
			Vector P = ray_O - O;
			double b = -P.dot(ray_V);
			double det = b * b - P.len2() + R * R;

			if (det > EPS) {
				det = sqrt(det);
				double x1 = b - det, x2 = b + det;

				if (x2 < EPS) return false;
				if (x1 > EPS) {
					irst.dist = x1;
					irst.front = true;
				}
				else {
					irst.dist = x2;
					irst.front = false;
				}
			}
			else
				return false;

			irst.C = ray_O + ray_V * irst.dist;
			irst.N = (irst.C - O).normal();
			if (irst.front == false) irst.N = -irst.N;
			return true;
		}

		Color GetTexture() {
			Vector I = (irst.C - O).normal();
			double a = acos(-I.dot(De));
			double b = acos(std::min(std::max(I.dot(Dc) / sin(a), -1.0), 1.0));
			double u = a / PI, v = b / 2 / PI;
			if (I.dot(Dc * De) < 0) v = 1 - v;
			return material->texture->colorSmoothAt(u, v);
		}

		Object* PrimitiveCopy() {
			Sphere* ret = new Sphere(*this);
			return ret;
		}

		void makeDemo(int i) {
			std::cout << "make sphere " << i << std::endl;
			if (i == 0) {
				O = Vector(-0.5,4.86, -1.8);
				R = 0.2;
				material->refr = 1;
				material->rindex = 1.7;
				material->absor = Color(1, 0, 0);
			}
			if (i == 1) {
				O = Vector(0, 5, -1.8);
				R = 0.2;
				material->refr = 1;
				material->rindex = 1.7;
				material->absor = Color(0, 1, 0);
			}
			if (i == 2) {
				O = Vector(0.5, 4.5, -1.8);
				R = 0.2;
				material->refr = 1;
				material->rindex = 1.7;
				material->absor = Color(0, 0, 1);

			}
			if (i == 3) {
				O = Vector(1, 4.3, -1.8);
				R = 0.2;
				material->refr = 1;
				material->rindex = 1.7;
				material->absor = Color(1, 1, 0);
				
			}
			if (i == 4) {
				O = Vector(1.5, 4.5, -1.8);
				R = 0.2;
				material->refr = 1;
				material->rindex = 1.7;
				material->absor = Color(0, 1, 1);
			
			}
			if (i == 5) {
				O = Vector(0.1,4.5, -1.3);
				R = 0.7;
				material->refr = 1;
				material->rindex = 1.7;
				material->absor = Color(0.1, 0, 0);
			}
			if (i == 6) {
				O = Vector(1, 5.5, -1.5);
				R = 0.4;
				material->color = Color(1, 1, 1);
				material->diff = 0.2;
				material->spec = 0.2;
				material->refl = 0.8;

			}
			if (i == 7) {
				O = Vector(-0.5, 5.5, -1.5);
				R = 0.5;
				De = Vector(0, 0, 1);
				Dc = Vector(0, 1, 0);
				material->color = Color(1, 1, 1);
				material->diff = 0.45;
				material->spec = 0.25;
				material->texture = new Picture;
				material->texture->read("test6.jpg");
				

			}
		}
	};

	class Plane : public Object {
		Vector N, Dx, Dy;
		double R;

	public:
		Plane() : Object() {}
		~Plane() {}

		bool intersect(Vector ray_O, Vector ray_V) {
			ray_V = ray_V.normal();
			N = N.normal();
			double d = N.dot(ray_V);
			if (fabs(d) < EPS) return false;
			double l = (N * R - ray_O).dot(N) / d;
			if (l < EPS) return false;

			irst.dist = l;
			irst.front = (d < 0);
			irst.C = ray_O + ray_V * irst.dist;
			irst.N = (irst.front) ? N : -N;
			return true;
		}

		Color GetTexture() {
			double u = irst.C.dot(Dx) / Dx.len2();
			double v = irst.C.dot(Dy) / Dy.len2();
			return material->texture->colorSmoothAt(u, v);
		}

		Object* PrimitiveCopy() {
			Plane* ret = new Plane(*this);
			return ret;
		}

		void makeDemo(int i) {
			std::cout << "make plane" << std::endl;
			N = Vector(0, 0, 1);
			R = -2;
			Dx = Vector(8, 0, 0);
			Dy = Vector(0, 8, 0);
			material->color = Color(1, 1, 1);
			material->diff = 0.3;
			material->refl = 0.7;
			material->drefl = 0.15;
			material->texture = new Picture;
			material->texture->read("test6.jpg");


		}

	};


}
#endif
