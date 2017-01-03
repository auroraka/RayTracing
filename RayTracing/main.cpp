#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include "raytracer.h"
using namespace std;
using namespace tl;

int main() {
	srand(123);
	Raytracer* raytracer = new Raytracer;
	raytracer->run();
	return 0;
}