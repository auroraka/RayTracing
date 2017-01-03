#ifndef SCENE_H
#define SCENE_H

#include"color.h"
#include"vector.h"
#include"object.h"
#include"light.h"
#include"camera.h"
#include<string>
#include<fstream>
#include<sstream>
#include <vector>
using namespace std;

namespace tl {
	class Scene {
		Light* light;
		Color background_color;
		Camera* camera;
	
	public:
		vector<Object*> objs;
		Picture* picture;

		Light* GetLight() { return light; }
		Color GetBackgroundColor() { return background_color; }
		Camera* GetCamera() { return camera; }


		Scene() {
			objs.clear();
			light = NULL;
			background_color = Color();
			camera = new Camera;
		}

		~Scene() {
			for (int i = 0; i < objs.size(); i++) {
				if (objs[i]->GetMaterial()->texture != NULL) {
					delete objs[i]->GetMaterial()->texture;
					delete objs[i];
				}
				delete objs[i];
			}
			objs.clear();
			
			delete light;
			delete camera;
		}

		void createScene() {
			int H = 1080;
			int W = 1920;
			//int H = 180;
			//int W = 300;
			picture = new Picture(H, W);
			
			
			background_color = Color(0.1, 0.1, 0.1);

			//cam
			camera->makeDemo(H,W);

			//light
			light = new Light;
			light->makeDemo();

			//obj
			Object* obj;
			for (int i = 0; i <=7; i++) {
				if (i == 5 || i==0 || i==1) continue;
				obj = new Sphere;
				obj->makeDemo(i);
				objs.push_back(obj);
			}

			//plane
			obj = new Plane;
			obj->makeDemo(5);
			objs.push_back(obj);


			camera->init();
		}

		Object* intersectWithObject(Vector ray_O, Vector ray_V) {
			Object* ret = NULL;

			for (int i = 0; i < objs.size(); i++) {
				if (objs[i]->intersect(ray_O, ray_V) && (ret == NULL || objs[i]->irst.dist < ret->irst.dist)) ret = objs[i];
			}
			return ret;
		}

		Light* intersectWithLight(Vector ray_O, Vector ray_V) {
			if (light->intersect(ray_O, ray_V)) {
				return light;
			}
			else {
				return NULL;
			}
		}

	};

}
#endif
