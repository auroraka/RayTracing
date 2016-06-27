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

namespace tl {
	class Scene {
		Object* primitive_head;
		Light* light_head;
		Color background_color;
		Camera* camera;
	
	public:
		Picture* picture;

		Object* GetPrimitiveHead() { return primitive_head; }
		Light* GetLightHead() { return light_head; }
		Color GetBackgroundColor() { return background_color; }
		Camera* GetCamera() { return camera; }


		Scene() {
			primitive_head = NULL;
			light_head = NULL;
			background_color = Color();
			camera = new Camera;
		}

		~Scene() {
			while (primitive_head != NULL) {
				Object* next_head = primitive_head->GetNext();
				if (primitive_head->GetMaterial()->texture != NULL)
					delete primitive_head->GetMaterial()->texture;
				delete primitive_head;
				primitive_head = next_head;
			}

			while (light_head != NULL) {
				Light* next_head = light_head->GetNext();
				delete light_head;
				light_head = next_head;
			}

			delete camera;
		}

		void createScene() {
			int H = 100;
			int W = 100;
			picture = new Picture(H, W);
			
			Object* new_primitive = NULL;
			Light* new_light = NULL;

			background_color = Color(0.1, 0.1, 0.1);

			//cam
			camera->makeDemo(H,W);

			//light
			new_light = new Light;
			new_light->SetNext(light_head);
			light_head = new_light;
			new_light->makeDemo();

			//obj
			for (int i = 0; i < 5; i++) {
				new_primitive = new Sphere;
				new_primitive->SetNext(primitive_head);
				primitive_head = new_primitive;
				new_primitive->makeDemo(i);
			}

			//plane
			new_primitive = new Plane;
			new_primitive->SetNext(primitive_head);
			primitive_head = new_primitive;
			new_primitive->makeDemo(5);



			camera->init();
		}

		Object* intersectWithObject(Vector ray_O, Vector ray_V) {
			Object* ret = NULL;

			for (Object* now = primitive_head; now != NULL; now = now->GetNext())
				if (now->intersect(ray_O, ray_V) && (ret == NULL || now->irst.dist < ret->irst.dist)) ret = now;

			return ret;
		}

		Light* intersectWithLight(Vector ray_O, Vector ray_V) {
			Light* ret = NULL;

			for (Light* now = light_head; now != NULL; now = now->GetNext())
				if (now->intersect(ray_O, ray_V) && (ret == NULL || now->crash_dist < ret->crash_dist)) ret = now;

			return ret;
		}

	};

}
#endif
