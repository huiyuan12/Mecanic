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
#define NParticles 25
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
bool textfor0 = false;
bool textfor10 = false;
bool textfor20 = false;
bool textfor50 = false;
bool textfor100 = false;

int emmiter;
int emmiter2;
int solver;
float radiusCapsule = 1.f;
float radiusSphere = 1.f;
float capsuleX = -3.f;
float capsuleY = 2.f;
float capsuleZ = -2.f;
float sphereX = 3.f;
float sphereY = 1.f;
float *partVertsVelocity;
float *partVerts;
float *lastPartVerts;
float *finalVerts;
float *TimeLife;
float *temporalVerts;
float *xForce, *yForce, *zForce;
float fx = ((float)rand() / RAND_MAX) * 5.f - 2.f, fy = -20, fz = ((float)rand() / RAND_MAX) * 5.f - 2.f; // forces
namespace Capsule {
	
	extern void setupCapsule(glm::vec3 posA = glm::vec3(-3.f, 2.f, -2.f), glm::vec3 posB = glm::vec3(-5.f, 2.f, 2.f), float radius = 1.f);
	extern void cleanupCapsule();
	extern void updateCapsule(glm::vec3 posA, glm::vec3 posB,float radiusCapsule );
	extern void drawCapsule();

}
namespace Sphere {
	extern void setupSphere(glm::vec3 pos = glm::vec3(3, 1.f, 0.f), float radius = 1.f);
	extern void cleanupSphere();
	extern void updateSphere(glm::vec3 pos, float radius = 1.f);
	extern void drawSphere();
}
void GUI() {
	{	//FrameRate
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			//this function selects euler or verlet
					ImGui::RadioButton("Euler", &solver, 0);	ImGui::SameLine();
					ImGui::Text("   ");
					ImGui::SameLine();
					ImGui::RadioButton("Verlet", &solver, 1);	
					ImGui::RadioButton("Fountain ", &emmiter, 0);
					ImGui::SameLine(); 
					ImGui::RadioButton("cascade  ", &emmiter, 1);
				}
			ImGui::SliderFloat("Velocity", &velocity, 0, 2); //thsi function changes the velocity of partitcles;

		if (ImGui::CollapsingHeader("Capsule"))
		{
			ImGui::SliderFloat("Capsule Radius", &radiusCapsule, 0, 3); //thsi function changes the velocity of partitcles;
			ImGui::SliderFloat("Position X Capsule", &capsuleX ,-4.9f, 4.9f);
			ImGui::SliderFloat("Position Y Capsule,", &capsuleY, 0.9f, 9.9f);
		}
		if (ImGui::CollapsingHeader("Sphere"))
		{
			ImGui::SliderFloat("Sphere radius", &radiusSphere, 0, 3); //thsi function changes the velocity of partitcles;
			ImGui::SliderFloat("Position X Sphere", &sphereX, -4.9f, 4.9f);
			ImGui::SliderFloat("Position Y Sphere,", &sphereY, 0.9f, 9.9f);
		}
		//this function change the number of particles
		if (ImGui::CollapsingHeader("Number of Particles"))
		{
			if (ImGui::Button("0")) {
				ImGui::SameLine();
				//number of particles changes
				textfor0 = true;
				textfor10 = false;
				textfor20 = false;
				textfor50 = false;
				textfor100 = false;

			}
			if (textfor0)
			{
				ImGui::SameLine();
				ImGui::Text("Nothing");
			}
			if (ImGui::Button("10")) {
				ImGui::SameLine();
				//change number of particles;
				textfor0 = false;
				textfor10 = true;
				textfor20 = false;
				textfor50 = false;
				textfor100 = false;
			}
			if (textfor10) {
				ImGui::SameLine();
				ImGui::Text("Oh, that's normal");
			}
			if (ImGui::Button("20"))
			{
				//change number of particles;
				textfor0 = false;
				textfor10 = false;
				textfor20 = true;
				textfor50 = false;
				textfor100 = false;
			}
			if (textfor20) {
				ImGui::SameLine();
				ImGui::Text("You like particles!");
			}
			if (ImGui::Button("50"))
			{
				ImGui::SameLine();
				//change numb of particles
				textfor0 = false;
				textfor10 = false;
				textfor20 = false;
				textfor50 = true;
				textfor100 = false;

			}
			if (textfor50)
			{
				ImGui::SameLine();
				ImGui::Text("That's a lot of particles!");
			}
			if (ImGui::Button("100"))
			{
				ImGui::SameLine();
				//change numb of partitcles
				textfor0 = false;
				textfor10 = false;
				textfor20 = false;
textfor50 = false;
textfor100 = true;
			}
			if (textfor100)
			{
				ImGui::SameLine();
				ImGui::Text("You crazy!!");
			}


		}
		
		// ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
		if (show_test_window) {
			ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
			ImGui::ShowTestWindow(&show_test_window);
		}
	}

namespace LilSpheres {
	extern const int maxParticles;
	extern void setupParticles(int numTotalParticles, float radius);
	extern void cleanupParticles();
	extern void updateParticles(int startIdx, int count, float* array_data);
	extern void drawParticles(int startIdx, int count);
	float lifeTime = 200;
	extern float radius;
	float velocityY = 15;
	int countAliveParticles = 0;
	int mass = 1;
}


void PhysicsInit() {
	//TODO
	//crear particulas
	srand(time(NULL));

	//TODO
	//updateParticles is the function you can use to update the position of the particles (directly from the physics code)
	//The access is contiguous from an start idx to idx+count particles. You may need to do multiple calls.
	//Called here as an example to initialize to random values all particles inside the box. This code can be removed.

//SOLVER == 0 -> EULER
//EULER == 0 => FOUNTAIN!!!!!!!!!!!!!!
//EULER == 1 => CASCADE!!!!!!!!!!!!!!
	partVerts = new float[LilSpheres::maxParticles * 3]; //posicions
	lastPartVerts = new float[LilSpheres::maxParticles * 3]; //posicions anteriors
	temporalVerts = new float[LilSpheres::maxParticles * 3];
	partVertsVelocity = new float[LilSpheres::maxParticles * 3];
	TimeLife = new float[LilSpheres::maxParticles];
	xForce = new float[LilSpheres::maxParticles];
	yForce = new float[LilSpheres::maxParticles];
	zForce = new float[LilSpheres::maxParticles];
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

					xForce[i] =  (((float)rand() / RAND_MAX) * 5.f - 2.f)*velocity;
					yForce[i] = 7.5*velocity;// -10 + ((float)rand() / RAND_MAX) * 5.f - 2.f;
					zForce[i] = (((float)rand() / RAND_MAX) * 5.f - 2.f)*velocity;
				}
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
						partVertsVelocity[i * 3 + 0] = 5*velocity; //x
						partVertsVelocity[i * 3 + 1] = (((float)rand() / RAND_MAX) * -15.f)*velocity; //y random entre 15 i 10
						partVertsVelocity[i * 3 + 2] = 0; //z
					}
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
				partVerts[i * 3 + 0] =  ((float)rand() / RAND_MAX) * 5.f - 2.f; //x
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
				zForce[i] = 4*velocity;// ((float)rand() / RAND_MAX) * 5.f - 2.f;
			}
		}
	}
	/////////////
	
}

void collision()
{

	float *topDistance = new float[LilSpheres::maxParticles * 3];
	float *bottomDistance = new float[LilSpheres::maxParticles * 3];
	float *rightDistance = new float[LilSpheres::maxParticles * 3];
	float *leftDistance = new float[LilSpheres::maxParticles * 3];
	float *frontDistance = new float[LilSpheres::maxParticles * 3];
	float *farDistance = new float[LilSpheres::maxParticles * 3];


	for (int i = 0; i < LilSpheres::maxParticles; ++i) {
		// bottom plane collision
		bottomDistance[i] = (partVerts[i*3+1]*100) / sqrt(100*100);
		if (bottomDistance[i] <= 0) { 
			
			

		}

		//right plane collison
		rightDistance[i] = (partVerts[i * 3 + 0] * (-100)) / sqrt(100 * 100) - 5; // es raro pero funciona
		if (rightDistance[i] <= 0) { 

			// colisions elastiques

		}
		//left plane collison
		leftDistance[i] = (-partVerts[i * 3 + 0] * (-100)) / sqrt(100 * 100) + 5;
		if (leftDistance[i] <= 0) {

			// colisions elastiques
		

		}

		//top plane collison
		topDistance[i] = ((partVerts[i * 3 + 1] * 100) / sqrt(100 * 100)-10) *(-1); // d'aquesta manera ses distancies son positives mentre estiguin dins es cub
		
		if (topDistance[i] <= 0) {

			

		}

		//front plane collision

		frontDistance[i] = partVerts[i * 3 + 2] * (100) / sqrt(100 * 100)+5;
		if (frontDistance[i] <= 0) {


		}

		frontDistance[i] = -partVerts[i * 3 + 2] * (100) / sqrt(100 * 100) + 5;
		if (frontDistance[i] <= 0) {


		}
		
	}


	/*
	for (int i = 0; i < LilSpheres::maxParticles; ++i) {

		if (partVerts[i * 3 + 0] <= -4.9 || partVerts[i * 3 + 0] >= 4.9) {
			partVertsVelocity[i * 3 + 0] *= -0.8;
		}

		if (partVerts[i * 3 + 1] <= 0) {
			partVertsVelocity[i * 3 + 1] *= -0.8;
		}
		if (partVerts[i * 3 + 1] >= 9.9) {
			partVertsVelocity[i * 3 + 1] *= -1;
		}

		if (partVerts[i * 3 + 2] <= -4.9 || partVerts[i * 3 + 2] >= 4.9) {
			partVertsVelocity[i * 3 + 2] *= -0.8;
		}

	}*/
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
			partVertsVelocity[i * 3 + 0] = 5*velocity; //x
			partVertsVelocity[i * 3 + 1] = (((float)rand() / RAND_MAX) * -15.f)*velocity; //y random
			partVertsVelocity[i * 3 + 2] =  0; //z
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
			zForce[i] = 4*velocity;// ((float)rand() / RAND_MAX) * 5.f - 2.f;
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
	finalVerts = new float[LilSpheres::maxParticles * 3];

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
			TimeLife[i] += 2;

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
		

		}
	}
}

void PhysicsUpdate(float dt) {
	//TODO

	LilSpheres::setupParticles(LilSpheres::maxParticles, LilSpheres::radius);

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
	Sphere::updateSphere(glm::vec3(sphereX,sphereY, 0.f), radiusSphere);
	Capsule::updateCapsule(glm::vec3(capsuleX,capsuleY,-2.f),glm::vec3(capsuleX, capsuleY,2.f), radiusCapsule);
	LilSpheres::updateParticles(0, LilSpheres::maxParticles, partVerts);
}

void PhysicsCleanup() {

	
		delete[]partVerts;
	
}


