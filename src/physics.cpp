#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>
#include <glm\gtc\matrix_transform.hpp>
#include <stdio.h>
#include <time.h>	

#define GRAVITY -9.81;
bool show_test_window = true;
float *partVerts;
extern int counter = 0;
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
	float lifeTime = 2000;
	extern float radius;
	float velocityY = 22;

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
		partVertsVelocity[i * 3 + 0] = rand()% 5 + (-5); //x
		partVertsVelocity[i * 3 + 2] = rand() % 5 + (-5); //z

	}


}
//hola
void PhysicsUpdate(float dt) {

	//TODO
	srand(time(NULL));
	float *finalVerts = new float[LilSpheres::maxParticles*3];

	for (int i = 0; i < LilSpheres::maxParticles; ++i) {
		//Marcamos posiciones finales para cada particula
		
		finalVerts[i * 3 + 0] = partVerts[i * 3 + 0] + dt * partVertsVelocity[i * 3 + 0];
		finalVerts[i * 3 + 1] = partVerts[i * 3 + 1]+dt * LilSpheres::velocityY;  
		finalVerts[i * 3 + 2] = partVerts[i * 3 + 2] + dt*partVertsVelocity[i * 3 + 2];//z

		LilSpheres::velocityY = LilSpheres::velocityY + dt*(-9.81/300); //Modifica la velocitat en Y

		//Asignamos la posicion de cada particula a la posicion final.
		partVerts[i * 3 + 0] = finalVerts[i * 3 + 0];
		partVerts[i * 3 + 1] = finalVerts[i * 3 + 1];
		partVerts[i * 3 + 2] = finalVerts[i * 3 + 2];//z

	}

	//euler velter
	//actualizar
	//calculos
	//rebotes

	LilSpheres::updateParticles(0, LilSpheres::maxParticles, partVerts);
}
void PhysicsCleanup() {
	//TODO
	//destruir

	
	counter++;
	if (counter > LilSpheres::lifeTime) {
		
		delete[] partVerts;
	}
}



