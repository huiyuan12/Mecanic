#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <cstdio>

//Boolean variables allow to show/hide the primitives
bool renderSphere = true;
bool renderCapsule = true;
bool renderParticles = true;

extern int primera;
extern int ultima;
namespace Sphere {
extern void setupSphere(glm::vec3 pos = glm::vec3(3, 1.f, 0.f), float radius = 1.f);
extern void cleanupSphere();
extern void updateSphere(glm::vec3 pos, float radius = 1.f);
extern void drawSphere();
}
namespace Capsule {
extern void setupCapsule(glm::vec3 posA = glm::vec3(-3.f, 2.f, -2.f), glm::vec3 posB = glm::vec3(-5.f, 2.f, 2.f), float radius = 1.f);
extern void cleanupCapsule();
extern void updateCapsule(glm::vec3 posA, glm::vec3 posB, float radius = 1.f);
extern void drawCapsule();
}
namespace LilSpheres {
extern const int maxParticles;
extern void setupParticles(int numTotalParticles, float radius = 0.05f);
extern void cleanupParticles();
extern void updateParticles(int startIdx, int count, float* array_data);
extern void drawParticles(int startIdx, int count);
}

void setupPrims() {
	Sphere::setupSphere();
	Capsule::setupCapsule();
}
void cleanupPrims() {
	Sphere::cleanupSphere();
	Capsule::cleanupCapsule();
	LilSpheres::cleanupParticles();
}

void renderPrims() {
	if(renderSphere)
		Sphere::drawSphere();
	if(renderCapsule)
		Capsule::drawCapsule();
	if (renderParticles) {
		LilSpheres::drawParticles(0, LilSpheres::maxParticles);
		
	}
	
}
