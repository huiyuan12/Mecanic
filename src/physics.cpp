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

void GUI() {
	{	//FrameRate
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		//TODO

	}


	// ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
	if(show_test_window) {
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
	bool isAlive;
}
float *partVertsVelocity;
float *partVerts;
float *finalVerts;
float *TimeLife;
void PhysicsInit() {
	//TODO
	//crear particulas
	srand(time(NULL));

														  //TODO
														  //updateParticles is the function you can use to update the position of the particles (directly from the physics code)
														  //The access is contiguous from an start idx to idx+count particles. You may need to do multiple calls.
														  //Called here as an example to initialize to random values all particles inside the box. This code can be removed.
	
	partVerts = new float[LilSpheres::maxParticles * 3]; //posicions
	partVertsVelocity = new float[LilSpheres::maxParticles * 3];
	TimeLife = new float[LilSpheres::maxParticles];
	for (int i = 0; i < LilSpheres::maxParticles; ++i) {
		partVerts[i * 3 + 0] = 0; //x
		partVerts[i * 3 + 1] = 0.5; //y
		partVerts[i * 3 + 2] = 0; //z
		TimeLife[i] = 0;
		LilSpheres::isAlive = true;
		
	}
	for (int i = 0; i < LilSpheres::maxParticles; ++i) {
		partVertsVelocity[i * 3 + 0] = ((float)rand() / RAND_MAX) * 2.f -1.f; //x
		partVertsVelocity[i * 3 + 1] = ((float)rand() / RAND_MAX) * 15.f; //y random entre 15 i 10 
		partVertsVelocity[i * 3 + 2] = ((float)rand() / RAND_MAX) * 5.f - 2.f; //z

	}
/*	for (int i = 0; i < LilSpheres::maxParticles; ++i) {
		partVertsLifetime[i * 3] = lifetime;
	}*/
	

}

//hola	

void PhysicsUpdate(float dt) {
	//TODO
	if(LilSpheres::countAliveParticles < LilSpheres::maxParticles)
	LilSpheres::countAliveParticles += NParticles;

	LilSpheres::setupParticles(LilSpheres::maxParticles, LilSpheres::radius);


	finalVerts = new float[LilSpheres::maxParticles * 3];
	LilSpheres::velocityY = LilSpheres::velocityY+ dt*(-9.81) ; //Modifica la velocitat en Y

	/*for (int i = 0; i < LilSpheres::countAliveParticles; ++i) {
	//Marcamos posiciones finales para cada particula
		TimeLife[i] += 2;
			finalVerts[i * 3 + 0] = partVerts[i * 3 + 0] + dt * partVertsVelocity[i * 3 + 0];
			finalVerts[i * 3 + 1] = partVerts[i * 3 + 1] + dt * partVertsVelocity[i * 3 + 1]; //altura diferent 
			finalVerts[i * 3 + 2] = partVerts[i * 3 + 2] + dt * partVertsVelocity[i * 3 + 2];//z

			partVertsVelocity[i * 3 + 1] = partVertsVelocity[i * 3 + 1] + dt*(-9.81);

			//Asignamos la posicion de cada particula a la posicion final.
			partVerts[i * 3 + 0] = finalVerts[i * 3 + 0];
			partVerts[i * 3 + 1] = finalVerts[i * 3 + 1];
			partVerts[i * 3 + 2] = finalVerts[i * 3 + 2];//z

	
	
	//std::cout << TimeLife[i] << "particula " << i << std::endl;

	
}*/
	for (int i = 0; i < LilSpheres::countAliveParticles; ++i)
	{
		if (TimeLife[i] == LilSpheres::lifeTime)
		{
			partVerts[i * 3 + 0] = 0; //x
			partVerts[i * 3 + 1] = 0.5; //y
			partVerts[i * 3 + 2] = 0; //z
			partVertsVelocity[i * 3 + 0] = ((float)rand() / RAND_MAX) * 2.f - 1.f; //x
			partVertsVelocity[i * 3 + 1] = ((float)rand() / RAND_MAX) * 15.f; //y random entre 15 i 10 
			partVertsVelocity[i * 3 + 2] = ((float)rand() / RAND_MAX) * 5.f - 2.f; //z
			TimeLife[i] = 0;
			
		}
		else {
			TimeLife[i] += 2;
			finalVerts[i * 3 + 0] = partVerts[i * 3 + 0] + dt * partVertsVelocity[i * 3 + 0];
			finalVerts[i * 3 + 1] = partVerts[i * 3 + 1] + dt * partVertsVelocity[i * 3 + 1]; //altura diferent 
			finalVerts[i * 3 + 2] = partVerts[i * 3 + 2] + dt * partVertsVelocity[i * 3 + 2];//z

			partVertsVelocity[i * 3 + 1] = partVertsVelocity[i * 3 + 1] + dt*(-9.81);

			//Asignamos la posicion de cada particula a la posicion final.
			partVerts[i * 3 + 0] = finalVerts[i * 3 + 0];
			partVerts[i * 3 + 1] = finalVerts[i * 3 + 1];
			partVerts[i * 3 + 2] = finalVerts[i * 3 + 2];//z

		}

	}
	for (int i = 0; i < LilSpheres::maxParticles; ++i) {

		if (partVerts[i * 3 + 0] <= -4.9 || partVerts[i * 3 + 0] >= 4.9) {
			partVertsVelocity[i * 3 + 0] *= -0.8;
		}

		if (partVerts[i * 3 + 1] <= 0)  {
			partVertsVelocity[i * 3 + 1] *= -0.8;
		}
		if (partVerts[i * 3 + 1] >= 9.9){
			partVertsVelocity[i * 3 + 1] *= -1.1;
		}

		if (partVerts[i * 3 + 2] <= -4.9 || partVerts[i * 3 + 2] >= 4.9) {
			partVertsVelocity[i * 3 + 2] *= -0.8;
		}

	}
	//euler velter
	//actualizar
	//calculos
	//rebotes

	//dos variables definirlas extern -> primera , ultima
		
	LilSpheres::updateParticles(0, LilSpheres::maxParticles, partVerts);

	/*
	if(primera > ultima) {
		LilSpheres::updateParticles(primera, ultima-primera, partVerts);
	} else {
		LilSpheres::updateParticles(ultima, LilSpheres::maxParticles-ultima, &partVerts[ultima]);
		LilSpheres::updateParticles(0, primera, partVerts);
	}
	*/
	
}

void PhysicsCleanup() {
	//cout << "counter: " << counter << endl;
	
		delete[]partVerts;
	
}


