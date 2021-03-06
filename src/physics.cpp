#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>
#include <glm\gtc\matrix_transform.hpp>
#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <stdio.h>
#include <time.h>	
#include <iostream>


#define GRAVITY -9.81
int NParticles;
bool show_test_window = true;
using namespace std;
int primera;
int ultima;
extern int counter = 0;

const int NUM_SECONDS = 1;
clock_t this_time = clock();
clock_t last_time = this_time;
int fiveSecCompleted = 5;
double time_counter = 0;

float velocity;
int emmiter;
int solver;
float radiusCapsule = 1.f;
float radiusSphere = 1.f;
float capsuleX = -3.f;
float capsuleY = 2.f;
float capsuleZ = -2.f;
float capsule2X = -3.f;
float capsule2Y = 2.f;
float capsule2Z = 2.f;
float sphereX = 3.f;
float sphereY = 1.f;
float sphereZ = 0.f;
float *partVertsVelocity;
float *partVerts;
float *lastPartVerts;
float *finalVerts;
float *TimeLife;
float *temporalVerts;
float *xForce, *yForce, *zForce;
float *vectorNFriction;
float *vectorTFriction;
float *verletVelocity;
float *preVerletVelocity;
float *normalSphere;
float fx = ((float)rand() / RAND_MAX) * 5.f - 2.f, fy = -20, fz = ((float)rand() / RAND_MAX) * 5.f - 2.f; // forces
float *topDistance;
float *bottomDistance;
float *rightDistance;
float *leftDistance;
float *frontDistance;
float *farDistance;
float *sphereDistance;
float *capsuleDistance;
float *capsuleProj;
float *modulusAP;
float *capsuleVector;
float *particleCapsuleVector;
int coefficient;
namespace Capsule {

	extern void setupCapsule(glm::vec3 posA = glm::vec3(-3.f, 2.f, -2.f), glm::vec3 posB = glm::vec3(-5.f, 2.f, 2.f), float radius = 1.f);
	extern void cleanupCapsule();
	extern void updateCapsule(glm::vec3 posA, glm::vec3 posB, float radiusCapsule);
	extern void drawCapsule();

}
namespace Sphere {
	extern void setupSphere(glm::vec3 pos = glm::vec3(3, 1.f, 0.f), float radius = 1.f);
	extern void cleanupSphere();
	extern void updateSphere(glm::vec3 pos, float radius = 1.f);
	extern void drawSphere();
}
namespace LilSpheres {
	extern const int maxParticles;
	extern void setupParticles(int numTotalParticles, float radius);
	extern void cleanupParticles();
	extern void updateParticles(int startIdx, int count, float* array_data);
	extern void drawParticles(int startIdx, int count);
	float lifeTime;
	extern float radius;
	float velocityY = 15;
	int countAliveParticles = 0;
	int mass = 1;
}

void GUI() {
		//FrameRate
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		//this function selects euler or verlet
		ImGui::RadioButton("Euler", &solver, 0);	ImGui::SameLine();
		ImGui::Text("   ");
		ImGui::SameLine();
		ImGui::RadioButton("Verlet", &solver, 1);
		ImGui::RadioButton("Fountain ", &emmiter, 0);
		ImGui::SameLine();
		ImGui::RadioButton("cascade  ", &emmiter, 1);
		ImGui::RadioButton("Elasticity", &coefficient,0);
		ImGui::SameLine();
		ImGui::RadioButton("Friction", &coefficient, 1);

	ImGui::SliderFloat("Velocity", &velocity, 0, 2); //thsi function changes the velocity of partitcles;

	if (ImGui::CollapsingHeader("Capsule"))
	{
		ImGui::SliderFloat("Capsule Radius", &radiusCapsule, 0, 3); //thsi function changes the velocity of partitcles;
		ImGui::SliderFloat("Position X Capsule", &capsuleX, -4.5f, 4.5f);
		ImGui::SliderFloat("Position Y Capsule,", &capsuleY, 0.5f, 9.5f);
		ImGui::SliderFloat("Position Z Capsule,", &capsuleZ, -4.5f, 4.5f);
		ImGui::SliderFloat("Second position X Capsule", &capsule2X, -4.5f, 4.f);
		ImGui::SliderFloat("Second position Y Capsule", &capsule2Y, -0.5f, 9.5f);
		ImGui::SliderFloat("Second position Z Capsule", &capsule2Z, -4.5f, 4.5f);
	}
	if (ImGui::CollapsingHeader("Sphere"))
	{
		ImGui::SliderFloat("Sphere radius", &radiusSphere, 0, 3); //thsi function changes the velocity of partitcles;
		ImGui::SliderFloat("Position X Sphere", &sphereX, -4.9f, 4.9f);
		ImGui::SliderFloat("Position Y Sphere,", &sphereY, 0.9f, 9.9f);
		ImGui::SliderFloat("Position Z Sphere,", &sphereZ, -4.9f, 4.9f);
	}
	//this function changes the number of particles
	if (ImGui::CollapsingHeader("Number of Particles"))
	{
		if (ImGui::Button("100")) {
			ImGui::SameLine();
			NParticles = 100;
		}
		if (ImGui::Button("125")) {
			NParticles = 125;
		}
		if (ImGui::Button("150"))
		{
			NParticles = 150;
		}
		if (ImGui::Button("200"))
		{
			NParticles = 200;
		}
	}
	if (ImGui::CollapsingHeader("Time life of Particles"))
	{
		if (ImGui::Button("1 second")) {
			LilSpheres::lifeTime = 100;
		}
		if (ImGui::Button("1.5 seconds "))
		{
			LilSpheres::lifeTime = 150;
		}
		if (ImGui::Button("2 seconds"))
		{
			LilSpheres::lifeTime = 200;
		}
		if (ImGui::Button("2.5 seconds"))
		{
			LilSpheres::lifeTime = 250;
		}
	}
	//we tryed to modifiy the life time of the particles but this doesnt works.
	//ImGui::SliderFloat("Time life ", &LilSpheres::lifeTime, 100, 200); 
	

	// ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
	if (show_test_window) {
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		ImGui::ShowTestWindow(&show_test_window);
	}
}

void FrictionVerlet(int vNormalX, int vNormalY, int vNormalZ, float friction1, float friction2, int it) {
	float dotProductResult = ((vNormalX)* partVertsVelocity[it * 3 + 0]) + ((vNormalY)* partVertsVelocity[it * 3 + 1]) + ((vNormalZ)* partVertsVelocity[it * 3 + 2]);

	vectorNFriction[it * 3 + 0] = dotProductResult * vNormalX;
	vectorNFriction[it * 3 + 1] = dotProductResult * vNormalY;
	vectorNFriction[it * 3 + 2] = dotProductResult * vNormalZ;
	

	vectorTFriction[it * 3 + 0] = verletVelocity[it * 3 + 0] - vectorNFriction[it * 3 + 0];
	vectorTFriction[it * 3 + 1] = verletVelocity[it * 3 + 1] - vectorNFriction[it * 3 + 1];
	vectorTFriction[it * 3 + 2] = verletVelocity[it * 3 + 2] - vectorNFriction[it * 3 + 2];

	//lastPartVerts[it * 3 + 0] = lastPartVerts[it * 3 + 0] + friction1 * vectorNFriction[it * 3 + 0] + friction2 * vectorTFriction[it * 3 + 0];
	//lastPartVerts[it * 3 + 1] = lastPartVerts[it * 3 + 1] + friction1 * vectorNFriction[it * 3 + 1] + friction2 * vectorTFriction[it * 3 + 1];
	//lastPartVerts[it * 3 + 2] = lastPartVerts[it * 3 + 2] + friction1 * vectorNFriction[it * 3 + 1] + friction2 * vectorTFriction[it * 3 + 2];

	partVerts[it * 3 + 0] = partVerts[it * 3 + 0] + friction1 * vectorNFriction[it * 3 + 0] + friction2 * vectorTFriction[it * 3 + 0];
	partVerts[it * 3 + 1] = partVerts[it * 3 + 1] + friction1 * vectorNFriction[it * 3 + 1] + friction2 * vectorTFriction[it * 3 + 1];
	partVerts[it * 3 + 2] = partVerts[it * 3 + 2] + friction1 * vectorNFriction[it * 3 + 1] + friction2 * vectorTFriction[it * 3 + 2];



}
void FrictionEuler(int vNormalX, int vNormalY, int vNormalZ, float friction, int it) {

	float dotProductResult = ((vNormalX)* partVertsVelocity[it * 3 + 0]) + ((vNormalY)* partVertsVelocity[it * 3 + 1]) + ((vNormalZ)* partVertsVelocity[it * 3 + 2]);

	vectorNFriction[it * 3 + 0] = dotProductResult * vNormalX;
	vectorNFriction[it * 3 + 1] = dotProductResult * vNormalY;
	vectorNFriction[it * 3 + 2] = dotProductResult * vNormalZ;	

	vectorTFriction[it * 3 + 0] = partVertsVelocity[it * 3 + 0] - vectorNFriction[it * 3 + 0];
	vectorTFriction[it * 3 + 1] = partVertsVelocity[it * 3 + 1] - vectorNFriction[it * 3 + 1];
	vectorTFriction[it * 3 + 2] = partVertsVelocity[it * 3 + 2] - vectorNFriction[it * 3 + 2];

	partVertsVelocity[it * 3 + 0] = partVertsVelocity[it * 3 + 0] - (friction * vectorTFriction[it * 3 + 0]);
	partVertsVelocity[it * 3 + 1] = partVertsVelocity[it * 3 + 1] - (friction * vectorTFriction[it * 3 + 1]);
	partVertsVelocity[it * 3 + 2] = partVertsVelocity[it * 3 + 2] - (friction * vectorTFriction[it * 3 + 2]);


}
void ElasticityEuler(int vNormalX, int vNormalY, int vNormalZ, float elasticity, int it) {
	partVertsVelocity[it * 3 + 0] = partVertsVelocity[it * 3 + 0] - (1 + elasticity) * (vNormalX * partVertsVelocity[it * 3 + 0]) * vNormalX;
	partVertsVelocity[it * 3 + 1] = partVertsVelocity[it * 3 + 1] - (1 + elasticity) * (vNormalY * partVertsVelocity[it * 3 + 1]) * vNormalY;
	partVertsVelocity[it * 3 + 2] = partVertsVelocity[it * 3 + 2] - (1 + elasticity) * (vNormalZ * partVertsVelocity[it * 3 + 2]) * vNormalZ;
}
void ElasticityVerlet(int vNormalX, int vNormalY, int vNormalZ, float elasticity, int d, int it) {

	float tempX, tempY, tempZ;
	float prevTempX, prevTempY, prevTempZ;

	tempX = partVerts[it * 3 + 0] - (1 + elasticity) * (vNormalX * partVerts[it * 3 + 0] + d) * vNormalX;
	tempY = partVerts[it * 3 + 1] - (1 + elasticity) * (vNormalY * partVerts[it * 3 + 1] + d)* vNormalY;
	tempZ = partVerts[it * 3 + 2] - (1 + elasticity) * (vNormalZ * partVerts[it * 3 + 2] + d) * vNormalZ;

	prevTempX = lastPartVerts[it * 3 + 0] - (1 + elasticity) * (vNormalX * lastPartVerts[it * 3 + 0] + d) * vNormalX;
	prevTempY = lastPartVerts[it * 3 + 1] - (1 + elasticity) * (vNormalY * lastPartVerts[it * 3 + 1] + d)* vNormalY;
	prevTempZ = lastPartVerts[it * 3 + 2] - (1 + elasticity) * (vNormalZ * lastPartVerts[it * 3 + 2] + d) * vNormalZ;

	lastPartVerts[it * 3 + 0] = prevTempX;
	lastPartVerts[it * 3 + 1] = prevTempY;
	lastPartVerts[it * 3 + 2] = prevTempZ;

	partVerts[it * 3 + 0] = tempX;
	partVerts[it * 3 + 1] = tempY;
	partVerts[it * 3 + 2] = tempZ;

}

void collision()
{
	capsuleVector[0] = capsuleX - capsuleX;
	capsuleVector[1] = capsuleY - capsuleY;
	capsuleVector[2] = (-2 - 2); // 4

	float modulusCV = sqrt(pow(capsuleVector[0], 2) + pow(capsuleVector[1], 2) + pow(capsuleVector[2], 2));
	for (int i = 0; i < LilSpheres::countAliveParticles; ++i) {
		// bottom plane collision

		bottomDistance[i] = (partVerts[i * 3 + 1] * 100) / sqrt(100 * 100);
		if (bottomDistance[i] < 0.2) {
			//FrictionVerlet(0, 1, 0, 0.8, 0.8, i);
			//FrictionEuler(0, 1, 0, 0.8, i);
		    //ElasticityEuler(0, 1, 0, 0.8, i);
			//ElasticityVerlet(0, 1, 0, 0.8, -50,i);
			if (solver == 0)
			{
				if (coefficient == 0)
				{
					ElasticityEuler(0, 1, 0, 0.8, i);
				}
				else if (coefficient == 1)
				{
					FrictionEuler(0, 1, 0, 0.8, i);
				}
			}
			else if (solver == 1)
			{
				if (coefficient == 0)
				{
						//ElasticityVerlet(0, 1, 0, 0.8, -50,i);
				}
				else if (coefficient == 1)
				{
					//FrictionVerlet(0, 1, 0, 0.8, 0.8, i);
				}
			}
		}
		//right plane collison
		rightDistance[i] = ((-partVerts[i * 3 + 0] * (-100)) / sqrt(100 * 100) - 5)*(-1);
		if (rightDistance[i] <= 0) {
			//FrictionVerlet(1, 0, 0, 0.8, 0.8, i);
			//FrictionEuler(1, 0, 0, 0.8, i);
			//ElasticityEuler(1, 0, 0, 0.8, i);
			//ElasticityVerlet(1, 0, 0, 0.8, -50,i);
			if (solver == 0)
			{
				if (coefficient == 0)
				{
					ElasticityEuler(1, 0, 0, 0.8, i);
				}
				else if (coefficient == 1)
				{
					FrictionEuler(1, 0, 0, 0.8, i);
				}
			}
			else if (solver == 1)
			{
				if (coefficient == 0)
				{
					//ElasticityVerlet(1, 0, 0, 0.8, -50,i);
				}
				else if (coefficient == 1)
				{
					//FrictionVerlet(1, 0, 0, 0.8, 0.8, i);
				}
			}
		}
		//left plane collison
		leftDistance[i] = (-partVerts[i * 3 + 0] * (-100)) / sqrt(100 * 100) + 5;
		if (leftDistance[i] <= 0) {

			//FrictionVerlet(1, 0, 0, 0.8, 0.8, i);
			//FrictionEuler(1, 0, 0, 0.8, i);
			//ElasticityEuler(1, 0, 0, 0.8, i);
			//ElasticityVerlet(1, 0, 0, 0.8, -50,i);
			if (solver == 0)
			{
				if (coefficient == 0)
				{
					ElasticityEuler(1, 0, 0, 0.8, i);
				}
				else if (coefficient == 1)
				{
					FrictionEuler(1, 0, 0, 0.8, i);
				}
			}
			else if (solver == 1)
			{
				if (coefficient == 0)
				{
					//ElasticityVerlet(1, 0, 0, 0.8, -50,i);
				}
				else if (coefficient == 1)
				{
					//FrictionVerlet(1, 0, 0, 0.8, 0.8, i);
				}
			}
		}

		//top plane collison
		topDistance[i] = ((partVerts[i * 3 + 1] * 100) / sqrt(100 * 100) - 10) *(-1); // d'aquesta manera ses distancies son positives mentre estiguin dins es cub. �s perque he calculat sa normal a l'enrev�s

		if (topDistance[i] <= 0.1) {
			
			//FrictionVerlet(0, 1, 0, 0.8, 0.8, i);
			//FrictionEuler(1, 0, 0, 0.8, i);
			//ElasticityEuler(1, 0, 0, 0.8, i);
			//ElasticityVerlet(1, 0, 0, 0.8, -50,i);
			if (solver == 0)
			{
				if (coefficient == 0)
				{
					ElasticityEuler(0, 1, 0, 1.5, i);
				}
				else if (coefficient == 1)
				{
					FrictionEuler(0, 1, 0, 0.8, i);
				}
			}
			else if (solver == 1)
			{
				if (coefficient == 0)
				{
					//ElasticityVerlet(1, 0, 0, 0.8, -50,i);
				}
				else if (coefficient == 1)
				{
					//FrictionVerlet(0, 1, 0, 0.8, 0.8, i);
				}
			}
		}//

		 //front plane collision
		frontDistance[i] = -partVerts[i * 3 + 2] * (100) / sqrt(100 * 100) - 5 * (-1);
		if (frontDistance[i] <= 0) {

			//FrictionVerlet(0, 0, 1, 0.8, 0.8, i);
			//FrictionEuler(0, 0, 1, 0.8, i);
		//	ElasticityEuler(0, 0, 1, 0.8, i);
			//ElasticityVerlet(0, 0, 1, 0.8, -50,i);
			if (solver == 0)
			{
				if (coefficient == 0)
				{
					ElasticityEuler(0, 0, 1, 0.8, i);
				}
				else if (coefficient == 1)
				{
					FrictionEuler(0, 0, 1, 0.8, i);
				}
			}
			else if (solver == 1)
			{
				if (coefficient == 0)
				{
					//ElasticityVerlet(0, 0, 1, 0.8, -50,i);
				}
				else if (coefficient == 1)
				{
					//FrictionVerlet(0, 0, 1, 0.8, 0.8, i);
				}
			}
		}
		farDistance[i] = partVerts[i * 3 + 2] * (100) / sqrt(100 * 100) + 5;
		if (farDistance[i] <= 0) {

			//FrictionVerlet(0, 0, 1, 0.8, 0.8, i);
			//FrictionEuler(0, 0, 1, 0.8, i);
			//ElasticityEuler(0, 0, 1, 0.8, i);
			//ElasticityVerlet(0, 0, 1, 0.8,-50, i);
			if (solver == 0)
			{
				if (coefficient == 0)
				{
					ElasticityEuler(0, 0, 1, 0.8, i);
				}
				else if (coefficient == 1)
				{
					FrictionEuler(0, 0, 1, 0.8, i);
				}
			}
			else if (solver == 1)
			{
				if (coefficient == 0)
				{
					//ElasticityVerlet(0, 0, 1, 0.8, -50, i);
				}
				else if (coefficient == 1)
				{
					//FrictionVerlet(0, 0, 1, 0.8, 0.8, i);
				}
			}
		}
		//sphere collison

		sphereDistance[i] = sqrt((partVerts[i * 3 + 0] - sphereX)*(partVerts[i * 3 + 0] - sphereX) + (partVerts[i * 3 + 1] - sphereY)*(partVerts[i * 3 + 1] - sphereY) + (partVerts[i * 3 + 2] - sphereZ)*(partVerts[i * 3 + 2] - sphereZ)) - radiusSphere;
		if (sphereDistance[i] <= 0) {
			ElasticityEuler(sphereX, sphereY, sphereZ, 0.8, i);
		}
		//Capsule collision
		particleCapsuleVector[i * 3 + 0] = partVerts[i * 3 + 0] - capsuleX;
		particleCapsuleVector[i * 3 + 1] = partVerts[i * 3 + 1] - capsuleY;
		particleCapsuleVector[i * 3 + 2] = partVerts[i * 3 + 2] - capsuleZ;


		modulusAP[i] = sqrt(particleCapsuleVector[i * 3 + 0] * particleCapsuleVector[i * 3 + 0] + particleCapsuleVector[i * 3 + 1] * particleCapsuleVector[i * 3 + 1] + particleCapsuleVector[i * 3 + 2] * particleCapsuleVector[i * 3 + 2]);

		capsuleProj[i] = ((particleCapsuleVector[i * 3 + 2] * capsuleVector[2]) / (modulusCV*modulusCV));

		capsuleDistance[i] = sqrt(modulusAP[i] * modulusAP[i] - capsuleProj[i] * capsuleProj[i]) - radiusCapsule*1.5;

		if (capsuleDistance[i] <= 0) {
			//ElasticityEuler(capsuleX, capsuleY, capsuleZ, 0.3, i);
			//ElasticityEuler(capsule2X, capsule2Y, capsule2Z, 0.3, i);
		}
	}

}

void PhysicsInit() {
	//TODO
	//crear particulas
	srand(time(NULL));

	//TODO
	//updateParticles is the function you can use to update the position of the particles (directly from the physics code)
	//The access is contiguous from an start idx to idx+count particles. You may need to do multiple calls.
	//Called here as an example to initialize to random values all particles inside the box. This code can be removed.
	LilSpheres::setupParticles(LilSpheres::maxParticles, LilSpheres::radius);
	NParticles = 100;
	LilSpheres::lifeTime = 200;
	partVerts = new float[LilSpheres::maxParticles * 3]; //posicions
	lastPartVerts = new float[LilSpheres::maxParticles * 3]; //posicions anteriors
	temporalVerts = new float[LilSpheres::maxParticles * 3];
	partVertsVelocity = new float[LilSpheres::maxParticles * 3];
	TimeLife = new float[LilSpheres::maxParticles];
	xForce = new float[LilSpheres::maxParticles];
	yForce = new float[LilSpheres::maxParticles];
	zForce = new float[LilSpheres::maxParticles];
	vectorNFriction = new float[LilSpheres::maxParticles*3];
	vectorTFriction = new float[LilSpheres::maxParticles*3];
	verletVelocity = new float[LilSpheres::maxParticles*3];
	preVerletVelocity = new float[LilSpheres::maxParticles];
	normalSphere = new float[LilSpheres::maxParticles];
	topDistance = new float[LilSpheres::maxParticles * 3];
	bottomDistance = new float[LilSpheres::maxParticles * 3];
	rightDistance = new float[LilSpheres::maxParticles * 3];
	leftDistance = new float[LilSpheres::maxParticles * 3];
	frontDistance = new float[LilSpheres::maxParticles * 3];
	farDistance = new float[LilSpheres::maxParticles * 3];
	sphereDistance = new float[LilSpheres::maxParticles * 3];
	capsuleDistance = new float[LilSpheres::maxParticles * 3];
	capsuleProj = new float[LilSpheres::maxParticles * 3];
	modulusAP = new float[LilSpheres::maxParticles * 3];
	capsuleVector = new float[3];
	particleCapsuleVector = new float[LilSpheres::maxParticles * 3];
	velocity = 1;
	if (emmiter == 0) {
		if (solver == 0) {
		
				partVerts = new float[LilSpheres::maxParticles * 3]; //posicions
				lastPartVerts = new float[LilSpheres::maxParticles * 3]; //posicions anteriors
				temporalVerts = new float[LilSpheres::maxParticles * 3];
				partVertsVelocity = new float[LilSpheres::maxParticles * 3];
				TimeLife = new float[LilSpheres::maxParticles];
				for (int i = 0; i < LilSpheres::maxParticles; ++i) {
					partVerts[i * 3 + 0] = 0; //x
					partVerts[i * 3 + 1] = 0.5; //y
					partVerts[i * 3 + 2] = 0; //z
					TimeLife[i] = 0;
				}
				for (int i = 0; i < LilSpheres::maxParticles; ++i) {
					partVertsVelocity[i * 3 + 0] = (((float)rand() / RAND_MAX) * 5.f - 2.f)*velocity; //x
					partVertsVelocity[i * 3 + 1] = (10.f + ((float)rand() / RAND_MAX) * 5.f)*velocity; //y random entre 15 i 10
					partVertsVelocity[i * 3 + 2] = (((float)rand() / RAND_MAX) * 5.f - 2.f)*velocity; //z
				}
				if (coefficient == 0) {

				}
				else if (coefficient == 1) {

				}
			}
		
		else if (solver == 1)
		{
			partVerts = new float[LilSpheres::maxParticles * 3]; //posicions
			lastPartVerts = new float[LilSpheres::maxParticles * 3]; //posicions anteriors
			temporalVerts = new float[LilSpheres::maxParticles * 3];
			partVertsVelocity = new float[LilSpheres::maxParticles * 3];
			TimeLife = new float[LilSpheres::maxParticles];
			xForce = new float[LilSpheres::maxParticles];
			yForce = new float[LilSpheres::maxParticles];
			zForce = new float[LilSpheres::maxParticles];
			for (int i = 0; i < LilSpheres::maxParticles; ++i) {
				partVerts[i * 3 + 0] = 0; //x
				partVerts[i * 3 + 1] = 0; //y
				partVerts[i * 3 + 2] = 0; //z
				TimeLife[i] = 0;

				lastPartVerts[i * 3 + 0] = partVerts[i * 3 + 0];
				lastPartVerts[i * 3 + 1] = partVerts[i * 3 + 1];
				lastPartVerts[i * 3 + 2] = partVerts[i * 3 + 2];
				for (int i = 0; i < LilSpheres::maxParticles; ++i) {

					xForce[i] = (((float)rand() / RAND_MAX) * 5.f - 2.f)*velocity;
					yForce[i] = 7.5*velocity;// -10 + ((float)rand() / RAND_MAX) * 5.f - 2.f;
					zForce[i] = (((float)rand() / RAND_MAX) * 5.f - 2.f)*velocity;
				}
			}
			if (coefficient == 0) {

			}
			else if (coefficient == 1) {

			}
		}
	}
	else if (emmiter == 1)
	{
		if (solver == 0)
		{
			for (int i = 0; i < LilSpheres::maxParticles; ++i) {
				partVerts = new float[LilSpheres::maxParticles * 3]; //posicions
				partVertsVelocity = new float[LilSpheres::maxParticles * 3];
				TimeLife = new float[LilSpheres::maxParticles];
				for (int i = 0; i < LilSpheres::maxParticles; ++i) {
					partVerts[i * 3 + 0] = -5 + ((float)rand() / RAND_MAX) *1.f; //xESTO O PONER -5 DIRECTO 
					partVerts[i * 3 + 1] = 7.5; //y
					partVerts[i * 3 + 2] = ((float)rand() / RAND_MAX) * 2.f - 1.f; //z
					TimeLife[i] = 0;
					for (int i = 0; i < LilSpheres::maxParticles; ++i) {
						partVertsVelocity[i * 3 + 0] = 5 * velocity; //x
						partVertsVelocity[i * 3 + 1] = (((float)rand() / RAND_MAX) * -15.f)*velocity; //y random entre 15 i 10
						partVertsVelocity[i * 3 + 2] = 0; //z
					}
				}
				if (coefficient == 0) {

				}
				else if (coefficient == 1) {

				}
			}
		}
		else if (solver == 1)
		{
			partVerts = new float[LilSpheres::maxParticles * 3]; //posicions
			lastPartVerts = new float[LilSpheres::maxParticles * 3]; //posicions anteriors
			temporalVerts = new float[LilSpheres::maxParticles * 3];
			partVertsVelocity = new float[LilSpheres::maxParticles * 3];
			TimeLife = new float[LilSpheres::maxParticles];
			xForce = new float[LilSpheres::maxParticles];
			yForce = new float[LilSpheres::maxParticles];
			zForce = new float[LilSpheres::maxParticles];
			for (int i = 0; i < LilSpheres::maxParticles; ++i) {
				partVerts[i * 3 + 0] = ((float)rand() / RAND_MAX) * 5.f - 2.f; //x
				partVerts[i * 3 + 1] = 7.5; //y
				partVerts[i * 3 + 2] = -5.f + ((float)rand() / RAND_MAX) * 1.f; //z
				TimeLife[i] = 0;

				lastPartVerts[i * 3 + 0] = partVerts[i * 3 + 0];
				lastPartVerts[i * 3 + 1] = partVerts[i * 3 + 1];
				lastPartVerts[i * 3 + 2] = partVerts[i * 3 + 2];
			}
			for (int i = 0; i < LilSpheres::maxParticles; ++i) {

				xForce[i] = 0;// ((float)rand() / RAND_MAX) * 5.f - 2.f;
				yForce[i] = -7.5*velocity;// -10 + ((float)rand() / RAND_MAX) * 5.f - 2.f;
				zForce[i] = 4 * velocity;// ((float)rand() / RAND_MAX) * 5.f - 2.f;
			}
			if (coefficient == 0) {

			}
			else if (coefficient == 1) {

			}
		}
	}
}

void EulerFountain(float dt)
{

	if (LilSpheres::countAliveParticles < LilSpheres::maxParticles)
		LilSpheres::countAliveParticles += NParticles;
	finalVerts = new float[LilSpheres::maxParticles * 3];
	LilSpheres::velocityY = LilSpheres::velocityY + dt*(9.81); //Modifica la velocitat en Y

	for (int i = 0; i < LilSpheres::countAliveParticles; ++i)
	{
		if (TimeLife[i] == LilSpheres::lifeTime) {
			partVerts[i * 3 + 0] = 0; //x
			partVerts[i * 3 + 1] = 0.5; //y
			partVerts[i * 3 + 2] = 0; //z
			partVertsVelocity[i * 3 + 0] = (((float)rand() / RAND_MAX) * 5.f - 2.f)*velocity; //x
			partVertsVelocity[i * 3 + 1] = (10.f + ((float)rand() / RAND_MAX) * 5.f)*velocity; //y random entre 15 i 10
			partVertsVelocity[i * 3 + 2] = (((float)rand() / RAND_MAX) * 5.f - 2.f)*velocity; //z
			TimeLife[i] = 0;
		}
		else {
			TimeLife[i] += 2;
			finalVerts[i * 3 + 1] = partVerts[i * 3 + 1] + dt * partVertsVelocity[i * 3 + 1]; //altura diferent 
			finalVerts[i * 3 + 0] = partVerts[i * 3 + 0] + dt * partVertsVelocity[i * 3 + 0];
			finalVerts[i * 3 + 2] = partVerts[i * 3 + 2] + dt * partVertsVelocity[i * 3 + 2];//z

			partVertsVelocity[i * 3 + 1] = partVertsVelocity[i * 3 + 1] + dt*(-9.81);

			//Asignamos la posicion de cada particula a la posicion final.
			partVerts[i * 3 + 1] = finalVerts[i * 3 + 1];
			if (partVerts[i * 3 + 1] > 0.1) {
				partVerts[i * 3 + 0] = finalVerts[i * 3 + 0];
				partVerts[i * 3 + 2] = finalVerts[i * 3 + 2];//z
			}



		}
	}
}
void EulerCascade(float dt) {
	if (LilSpheres::countAliveParticles < LilSpheres::maxParticles)
		LilSpheres::countAliveParticles += NParticles;
	finalVerts = new float[LilSpheres::maxParticles * 3];

	LilSpheres::velocityY = LilSpheres::velocityY + dt*(-9.81); //Modifica la velocitat en Y

	for (int i = 0; i < LilSpheres::countAliveParticles; ++i)
	{
		if (TimeLife[i] == LilSpheres::lifeTime) {
			partVerts[i * 3 + 0] = -5 + ((float)rand() / RAND_MAX) *1.f; // ESTO O PONER -5 DIRECTO 
			partVerts[i * 3 + 1] = 7.5; //y
			partVerts[i * 3 + 2] = ((float)rand() / RAND_MAX) * 2.f - 1.f; //z
			partVertsVelocity[i * 3 + 0] = 5 * velocity; //x
			partVertsVelocity[i * 3 + 1] = (((float)rand() / RAND_MAX) * -15.f)*velocity; //y random
			partVertsVelocity[i * 3 + 2] = 0; //z
			TimeLife[i] = 0;
		}
		else {
			TimeLife[i] += 2;
			finalVerts[i * 3 + 1] = partVerts[i * 3 + 1] + dt * partVertsVelocity[i * 3 + 1]; //altura diferent 

			finalVerts[i * 3 + 0] = partVerts[i * 3 + 0] + dt * partVertsVelocity[i * 3 + 0];
			finalVerts[i * 3 + 2] = partVerts[i * 3 + 2] + dt * partVertsVelocity[i * 3 + 2];//z


			partVertsVelocity[i * 3 + 1] = partVertsVelocity[i * 3 + 1] + dt*(-9.81);

			//Asignamos la posicion de cada particula a la posicion final.

			partVerts[i * 3 + 0] = finalVerts[i * 3 + 0];

			partVerts[i * 3 + 1] = finalVerts[i * 3 + 1];
			partVerts[i * 3 + 2] = finalVerts[i * 3 + 2];//z

		}
	}
}
void VerletCascade(float dt)
{
	if (LilSpheres::countAliveParticles < LilSpheres::maxParticles)
		LilSpheres::countAliveParticles += NParticles;
	finalVerts = new float[LilSpheres::maxParticles * 3];

	for (int i = 0; i < LilSpheres::countAliveParticles; ++i)
	{
		if (TimeLife[i] == LilSpheres::lifeTime) {
			partVerts[i * 3 + 0] = ((float)rand() / RAND_MAX) * 5.f - 2.f; //x
			partVerts[i * 3 + 1] = 7.5; //y
			partVerts[i * 3 + 2] = -5.f + ((float)rand() / RAND_MAX) * 1.f; //z

			xForce[i] = 0;// ((float)rand() / RAND_MAX) * 5.f - 2.f;
			yForce[i] = -7.5*velocity;// -10 + ((float)rand() / RAND_MAX) * 5.f - 2.f;
			zForce[i] = 4 * velocity;// ((float)rand() / RAND_MAX) * 5.f - 2.f;
			lastPartVerts[i * 3 + 0] = partVerts[i * 3 + 0];
			lastPartVerts[i * 3 + 1] = partVerts[i * 3 + 1];
			lastPartVerts[i * 3 + 2] = partVerts[i * 3 + 2];

			TimeLife[i] = 0;
		}
		else {
			TimeLife[i] += 2;

			temporalVerts = partVerts;


			finalVerts[i * 3 + 0] = partVerts[i * 3 + 0] + (partVerts[i * 3 + 0] - lastPartVerts[i * 3 + 0]) + (xForce[i] / LilSpheres::mass)*(dt*dt);
			finalVerts[i * 3 + 1] = partVerts[i * 3 + 1] + (partVerts[i * 3 + 1] - lastPartVerts[i * 3 + 1]) + (yForce[i] / LilSpheres::mass)*(dt*dt);
			finalVerts[i * 3 + 2] = partVerts[i * 3 + 2] + (partVerts[i * 3 + 2] - lastPartVerts[i * 3 + 2]) + (zForce[i] / LilSpheres::mass)*(dt*dt);


			lastPartVerts[i * 3 + 0] = temporalVerts[i * 3 + 0];
			lastPartVerts[i * 3 + 1] = temporalVerts[i * 3 + 1];
			lastPartVerts[i * 3 + 2] = temporalVerts[i * 3 + 2];
			//Asignamos la posicion de cada particula a la posicion final.
			partVerts[i * 3 + 0] = finalVerts[i * 3 + 0];
			partVerts[i * 3 + 1] = finalVerts[i * 3 + 1];
			partVerts[i * 3 + 2] = finalVerts[i * 3 + 2];//z

		}
	}
}
void VerletFountain(float dt) {

	if (LilSpheres::countAliveParticles < LilSpheres::maxParticles)
		LilSpheres::countAliveParticles += NParticles;
		finalVerts = new float[LilSpheres::maxParticles * 100];

	for (int i = 0; i < LilSpheres::countAliveParticles; ++i)
	{
		if (TimeLife[i] == LilSpheres::lifeTime) {
			partVerts[i * 3 + 0] = 0; //x
			partVerts[i * 3 + 1] = 0; //y
			partVerts[i * 3 + 2] = 0; //z

			xForce[i] = (((float)rand() / RAND_MAX) * 5.f - 2.f)*velocity;
			yForce[i] = 7.5*velocity;// -10 + ((float)rand() / RAND_MAX) * 5.f - 2.f;
			zForce[i] = (((float)rand() / RAND_MAX) * 5.f - 2.f)*velocity;
			lastPartVerts[i * 3 + 0] = partVerts[i * 3 + 0];
			lastPartVerts[i * 3 + 1] = partVerts[i * 3 + 1];
			lastPartVerts[i * 3 + 2] = partVerts[i * 3 + 2];

			TimeLife[i] = 0;
		}
		else {
		

			temporalVerts = partVerts;

			finalVerts[i * 3 + 0] = partVerts[i * 3 + 0] + (partVerts[i * 3 + 0] - lastPartVerts[i * 3 + 0]) + (xForce[i] / LilSpheres::mass)*(dt*dt);
			finalVerts[i * 3 + 1] = partVerts[i * 3 + 1] + (partVerts[i * 3 + 1] - lastPartVerts[i * 3 + 1]) + (yForce[i] / LilSpheres::mass)*(dt*dt);
			finalVerts[i * 3 + 2] = partVerts[i * 3 + 2] + (partVerts[i * 3 + 2] - lastPartVerts[i * 3 + 2]) + (zForce[i] / LilSpheres::mass)*(dt*dt);


			lastPartVerts[i * 3 + 0] = temporalVerts[i * 3 + 0];
			lastPartVerts[i * 3 + 1] = temporalVerts[i * 3 + 1];
			lastPartVerts[i * 3 + 2] = temporalVerts[i * 3 + 2];
			//Asignamos la posicion de cada particula a la posicion final.
			if (partVerts[i * 3 + 1] > 4) {
				yForce[i] -= 5;
			}
			partVerts[i * 3 + 0] = finalVerts[i * 3 + 0];
			partVerts[i * 3 + 1] = finalVerts[i * 3 + 1];
			partVerts[i * 3 + 2] = finalVerts[i * 3 + 2];//z

			TimeLife[i] += 2;
		}
	}
}

void PhysicsUpdate(float dt) {
	//TODO



	if (emmiter == 0) {
		if (solver == 0)
		{
			EulerFountain(dt);
		}
		else if (solver == 1)
		{
			VerletFountain(dt);
			
		}
	}
	else if (emmiter == 1)
	{
		if (solver == 0) {
			EulerCascade(dt);
		}
		else if (solver == 1)
		{
			VerletCascade(dt);
		}
	}
	collision();
	
	Sphere::updateSphere(glm::vec3(sphereX, sphereY, sphereZ), radiusSphere);
	Capsule::updateCapsule(glm::vec3(capsuleX, capsuleY, capsuleZ), glm::vec3(capsule2X, capsule2Y, capsule2Z), radiusCapsule);
	LilSpheres::updateParticles(0, LilSpheres::maxParticles, partVerts);
}


void PhysicsCleanup() {


	
	delete[]partVerts; //posicions
	delete[]lastPartVerts; //posicions anteriors
	delete[]temporalVerts;
	delete[]partVertsVelocity;
	delete[]TimeLife;
	delete[]xForce;
	delete[]yForce;
	delete[]zForce;
	delete[]vectorNFriction ;
	delete[]vectorTFriction;
	delete[]verletVelocity;
	delete[]preVerletVelocity;
	delete[]normalSphere;
	delete[]topDistance;
	delete[]bottomDistance;
	delete[]rightDistance;
	delete[]leftDistance;
	delete[]frontDistance;
	delete[]farDistance;
	delete[]sphereDistance;
	delete[]capsuleDistance;
	delete[]capsuleProj;
	delete[]modulusAP;
	delete[]capsuleVector;
	delete[]particleCapsuleVector;

}


