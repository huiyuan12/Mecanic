#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>
#include <glm\gtc\matrix_transform.hpp>
#include <stdio.h>
#include <time.h>	
#include <iostream>


#define GRAVITY -9.81
#define NParticles 10
bool show_test_window = true;

int primera;
int ultima;
extern int counter = 0;

const int NUM_SECONDS = 1;
clock_t this_time = clock();
clock_t last_time = this_time;
int fiveSecCompleted = 5;
double time_counter = 0;
bool euler = true;
bool verlet = false;
int velocity;
bool textfor0 = false;
bool textfor10 = false;
bool textfor20 = false;
bool textfor50 = false;
bool textfor100 = false;
bool textEuler = false;
bool textVerlet = false;
void GUI() {
	{	//FrameRate
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		//this function selects euler or verlet
		if (ImGui::Button("Euler")) {
			euler = true;
			verlet = false;
			textEuler = true;
			textVerlet = false;
		}

		ImGui::SameLine();
		if (ImGui::Button("Verlet")) {
			verlet = true;
			euler = false;
			textEuler = false;
			textVerlet = true;
		}
		if (textEuler)
		{
			ImGui::Text("Wiiiiii fountain :D");
		}
		if (textVerlet)
		{
			ImGui::Text("Wooooow cascade :D");
		}

		ImGui::SliderInt("Velocity", &velocity, 15, 30); //thsi function changes the velocity of partitcles;

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

float *partVertsVelocity;
float *partVerts;
float *lastPartVerts;
float *finalVerts;
float *TimeLife;
float *temporalVerts;
float fx = ((float)rand() / RAND_MAX) * 5.f - 2.f , fy = -20, fz = ((float)rand() / RAND_MAX) * 5.f - 2.f; // forces
namespace Capsule {
extern void setupCapsule(glm::vec3 posA = glm::vec3(-3.f, 2.f, -2.f), glm::vec3 posB = glm::vec3(-5.f, 2.f, 2.f), float radius = 1.f);
extern void cleanupCapsule();
extern void updateCapsule(glm::vec3 posA, glm::vec3 posB, float radius = 1.f);
extern void drawCapsule();
}
void PhysicsInit() {
	//TODO
	//crear particulas
	srand(time(NULL));

														  //TODO
														  //updateParticles is the function you can use to update the position of the particles (directly from the physics code)
														  //The access is contiguous from an start idx to idx+count particles. You may need to do multiple calls.
														  //Called here as an example to initialize to random values all particles inside the box. This code can be removed.
	
	partVerts = new float[LilSpheres::maxParticles * 3]; //posicions
	lastPartVerts = new float[LilSpheres::maxParticles * 3]; //posicions anteriors
	temporalVerts = new float[LilSpheres::maxParticles * 3];
	partVertsVelocity = new float[LilSpheres::maxParticles * 3];
	TimeLife = new float[LilSpheres::maxParticles];
	for (int i = 0; i < LilSpheres::maxParticles; ++i) {
		partVerts[i * 3 + 0] = ((float)rand() / RAND_MAX) * 5.f - 2.f; //x
		partVerts[i * 3 + 1] = 7.5; //y
		partVerts[i * 3 + 2] = ((float)rand() / RAND_MAX) * 3.f - 2.f; //z
		TimeLife[i] = 0;	
		
		lastPartVerts[i * 3 + 0] = partVerts[i * 3 + 0];
		lastPartVerts[i * 3 + 1] = partVerts[i * 3 + 1];
		lastPartVerts[i * 3 + 2] = partVerts[i * 3 + 2];
	}


	/*for (int i = 0; i < LilSpheres::maxParticles; ++i) {
		partVertsVelocity[i * 3 + 0] = ((float)rand() / RAND_MAX) * 2.f -1.f; //x
		partVertsVelocity[i * 3 + 1] = ((float)rand() / RAND_MAX) * 15.f; //y random entre 15 i 10 
		partVertsVelocity[i * 3 + 2] = ((float)rand() / RAND_MAX) * 5.f - 2.f; //z
	}*/
}

void collision()
{
	for (int i = 0; i < LilSpheres::maxParticles; ++i) {

		if (partVerts[i * 3 + 0] <= -4.9 || partVerts[i * 3 + 0] >= 4.9) {
			partVertsVelocity[i * 3 + 0] *= -0.8;
		}

		if (partVerts[i * 3 + 1] <= 0) {
			partVertsVelocity[i * 3 + 1] *= -0.8;
		}
		if (partVerts[i * 3 + 1] >= 9.9) {
			partVertsVelocity[i * 3 + 1] *= -1.1;
		}

		if (partVerts[i * 3 + 2] <= -4.9 || partVerts[i * 3 + 2] >= 4.9) {
			partVertsVelocity[i * 3 + 2] *= -0.8;
		}

	}
}
void Euler(float dt)
{
	if (LilSpheres::countAliveParticles < LilSpheres::maxParticles)
		LilSpheres::countAliveParticles += NParticles;
		finalVerts = new float[LilSpheres::maxParticles * 3];
		LilSpheres::velocityY = LilSpheres::velocityY + dt*(-9.81); //Modifica la velocitat en Y

	for (int i = 0; i < LilSpheres::countAliveParticles; ++i)
	{
		if (TimeLife[i] == LilSpheres::lifeTime) {
			partVerts[i * 3 + 0] = 0; //x
			partVerts[i * 3 + 1] = 0.5; //y
			partVerts[i * 3 + 2] = 0; //z
			partVertsVelocity[i * 3 + 0] = ((float)rand() / RAND_MAX) * 2.f - 1.f; //x
			partVertsVelocity[i * 3 + 1] = ((float)rand() / RAND_MAX) * 15.f; //y random
			partVertsVelocity[i * 3 + 2] = ((float)rand() / RAND_MAX) * 5.f - 2.f; //z
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
				if (partVerts[i * 3 + 1] > 0.8) {
					partVerts[i * 3 + 0] = finalVerts[i * 3 + 0];
					partVerts[i * 3 + 2] = finalVerts[i * 3 + 2];//z
				}
		}
	}
}
void Verlet(float dt)
{

	if (LilSpheres::countAliveParticles < LilSpheres::maxParticles)
		LilSpheres::countAliveParticles += NParticles;
	finalVerts = new float[LilSpheres::maxParticles * 3];

	for (int i = 0; i < LilSpheres::countAliveParticles; ++i)
	{
		if (TimeLife[i] == LilSpheres::lifeTime) {
			partVerts[i * 3 + 0] = ((float)rand() / RAND_MAX) * 5.f - 2.f; //x
			partVerts[i * 3 + 1] = 7.5; //y
			partVerts[i * 3 + 2] = ((float)rand() / RAND_MAX) * 3.f - 2.f; //z

	
			TimeLife[i] = 0;
		}
		else {
			TimeLife[i] += 2;

			temporalVerts = partVerts;

			finalVerts[i * 3 + 0] = partVerts[i * 3 + 0] + (partVerts[i * 3 + 0] - lastPartVerts[i * 3 + 0]) + (fx / LilSpheres::mass)*(dt*dt);
			finalVerts[i * 3 + 1] = partVerts[i * 3 + 1] + (partVerts[i * 3 + 1] - lastPartVerts[i * 3 + 1]) + (fy / LilSpheres::mass)*(dt*dt);
			finalVerts[i * 3 + 2] = partVerts[i * 3 + 2] + (partVerts[i * 3 + 2] - lastPartVerts[i * 3 + 2]) + (fz / LilSpheres::mass)*(dt*dt);


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
void PhysicsUpdate(float dt) {
	//TODO
	
	LilSpheres::setupParticles(LilSpheres::maxParticles, LilSpheres::radius);
	
	Verlet(dt);
	//collision();

	LilSpheres::updateParticles(0, LilSpheres::maxParticles, partVerts);
}

void PhysicsCleanup() {

	
		delete[]partVerts;
	
}


