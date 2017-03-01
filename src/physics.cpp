#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>
#include <glm\gtc\matrix_transform.hpp>
#include <stdio.h>
#include <time.h>	

#define GRAVITY -9.81;
bool show_test_window = true;
float *partVerts;
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
	float timeAlive = 0;
	float lifeTime = 2;
	extern float radius;
	float velocityY = 15;
}
float *partVertsVelocity;
void PhysicsInit() {
	//TODO
	//crear particulas
	srand(time(NULL));
	LilSpheres::setupParticles(LilSpheres::maxParticles,LilSpheres::radius); //POSITION

														  //TODO
														  //updateParticles is the function you can use to update the position of the particles (directly from the physics code)
														  //The access is contiguous from an start idx to idx+count particles. You may need to do multiple calls.
														  //Called here as an example to initialize to random values all particles inside the box. This code can be removed.

	partVerts = new float[LilSpheres::maxParticles * 3]; //posicions
	partVertsVelocity = new float[LilSpheres::maxParticles * 3];

	for (int i = 0; i < LilSpheres::maxParticles; ++i) {
		partVerts[i * 3 + 0] = 0; //x
		partVerts[i * 3 + 1] = 0; //y
		partVerts[i * 3 + 2] = 0; //z
		
	}
	for (int i = 0; i < LilSpheres::maxParticles; ++i) {
		partVertsVelocity[i * 3 + 0] = ((float)rand() / RAND_MAX) * 5.f - 2.f; //x
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
	srand(time(NULL));

	float *finalVerts = new float[LilSpheres::maxParticles*3];
	LilSpheres::velocityY = LilSpheres::velocityY+ dt*(-9.81) ; //Modifica la velocitat en Y

	for (int i = 0; i < LilSpheres::maxParticles; ++i) {
		//Marcamos posiciones finales para cada particula
		

		finalVerts[i * 3 + 0] = partVerts[i * 3 + 0] + dt * partVertsVelocity[i * 3 + 0];
		finalVerts[i * 3 + 1] = partVerts[i * 3 + 1] + dt * partVertsVelocity[i * 3 + 1]; //altura diferent 
		finalVerts[i * 3 + 2] = partVerts[i * 3 + 2] + dt * partVertsVelocity[i * 3 + 2];//z
		 
		partVertsVelocity[i * 3 + 1] = partVertsVelocity[i * 3 + 1] + dt*(-9.81);

		//Asignamos la posicion de cada particula a la posicion final.
		partVerts[i * 3 + 0] = finalVerts[i * 3 + 0];
		partVerts[i * 3 + 1] = finalVerts[i * 3 + 1];
		partVerts[i * 3 + 2] = finalVerts[i * 3 + 2];//z

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

}


