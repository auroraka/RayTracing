#ifndef MATERIAL_H
#define MATERIAL_H

#include "picture.h"
#include "basic.h"


namespace tl {
	class Material {
	public:
		Color color, absor;
		double refl, refr;
		double diff, spec;
		double rindex;
		double drefl;
		Picture* texture;

		Material::Material() {
			color = absor = Color();
			refl = refr = 0;
			diff = spec = 0;
			rindex = 0;
			drefl = 0;
			texture = NULL;
		}

	};

}


#endif