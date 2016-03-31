// Link statically with GLEW

// Headers

#include <windows.h>
#include <GL/glew.h>
#include <GL/glfw3.h>
GLFWwindow* window;
#include <iostream>
#include <ctime>
#include <SOIL.h>
#include <math.h>
#include <vector>
#include <time.h>


#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
/*
#include "BulletDynamics\Dynamics\btDynamicsWorld.h"
#include "BulletCollision\Gimpact\btGImpactCollisionAlgorithm.h" */

#include "Leaf.h"
#include "World.h"
#include <common/TriangleSoup.hpp>
#include <common/Utilities.hpp>
#include "Texture.hpp"
#include "Shader.hpp"
#include <vector>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
#include <common/shader2.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>
#include <common/texture.hpp>

void bmpTexture(const char * imagepath, GLuint shaderProgram, const char * name, int i);

void mat4mult(float M1[], float M2[], float Mout[])
{
	float Mtemp[16];
	// Your code goes here: compute and set each element of Mout

	// e.g. Mout[0] = M1[0]*M2[0] + M1[4]*M2[1] + ... etc
	Mtemp[0] = M1[0] * M2[0] + M1[4] * M2[1] + M1[8] * M2[2] + M1[12] * M2[3];
	Mtemp[4] = M1[0] * M2[4] + M1[4] * M2[5] + M1[8] * M2[6] + M1[12] * M2[7];
	Mtemp[8] = M1[0] * M2[8] + M1[4] * M2[9] + M1[8] * M2[10] + M1[12] * M2[11];
	Mtemp[12] = M1[0] * M2[12] + M1[4] * M2[13] + M1[8] * M2[14] + M1[12] * M2[15];

	Mtemp[1] = M1[1] * M2[0] + M1[5] * M2[1] + M1[9] * M2[2] + M1[13] * M2[3];
	Mtemp[5] = M1[1] * M2[4] + M1[5] * M2[5] + M1[9] * M2[6] + M1[13] * M2[7];
	Mtemp[9] = M1[1] * M2[8] + M1[5] * M2[9] + M1[9] * M2[10] + M1[13] * M2[11];
	Mtemp[13] = M1[1] * M2[12] + M1[5] * M2[13] + M1[9] * M2[14] + M1[13] * M2[15];

	Mtemp[2] = M1[2] * M2[0] + M1[6] * M2[1] + M1[10] * M2[2] + M1[14] * M2[3];
	Mtemp[6] = M1[2] * M2[4] + M1[6] * M2[5] + M1[10] * M2[6] + M1[14] * M2[7];
	Mtemp[10] = M1[2] * M2[8] + M1[6] * M2[9] + M1[10] * M2[10] + M1[14] * M2[11];
	Mtemp[14] = M1[2] * M2[12] + M1[6] * M2[13] + M1[10] * M2[14] + M1[14] * M2[15];

	Mtemp[3] = M1[3] * M2[0] + M1[7] * M2[1] + M1[11] * M2[2] + M1[15] * M2[3];
	Mtemp[7] = M1[3] * M2[4] + M1[7] * M2[5] + M1[11] * M2[6] + M1[15] * M2[7];
	Mtemp[11] = M1[3] * M2[8] + M1[7] * M2[9] + M1[11] * M2[10] + M1[15] * M2[11];
	Mtemp[15] = M1[3] * M2[12] + M1[7] * M2[13] + M1[11] * M2[14] + M1[15] * M2[15];

	for (int i = 0; i <16; i++)
	{
		Mout[i] = Mtemp[i];
	}
}


void mat4identity(float Mtemp[])
{
	Mtemp[0] = 1;
	Mtemp[4] = 0;
	Mtemp[8] = 0;
	Mtemp[12] = 0;
	Mtemp[1] = 0;
	Mtemp[5] = 1;
	Mtemp[9] = 0;
	Mtemp[13] = 0;
	Mtemp[2] = 0;
	Mtemp[6] = 0;
	Mtemp[10] = 1;
	Mtemp[14] = 0;
	Mtemp[3] = 0;
	Mtemp[7] = 0;
	Mtemp[11] = 0;
	Mtemp[15] = 1;

};

void mat4rotx(float Mtemp[], float angle)
{
	mat4identity(Mtemp);
	Mtemp[5] = cos(angle);
	Mtemp[6] = sin(angle);
	Mtemp[9] = -sin(angle);
	Mtemp[10] = cos(angle);

};
void mat4roty(float Mtemp[], float angle)
{
	mat4identity(Mtemp);
	Mtemp[0] = cos(angle);
	Mtemp[8] = sin(angle);
	Mtemp[2] = -sin(angle);
	Mtemp[10] = cos(angle);

};
void mat4rotz(float Mtemp[], float angle)
{
	mat4identity(Mtemp);
	Mtemp[0] = cos(angle);
	Mtemp[4] = -sin(angle);
	Mtemp[1] = sin(angle);
	Mtemp[5] = cos(angle);

};

void mat4scale(float Mtemp[], float scale)
{
	mat4identity(Mtemp);
	Mtemp[0] = scale;
	Mtemp[5] = scale;
	Mtemp[10] = scale;

};

void mat4translate(float Mtemp[], float x, float y, float z)
{
	mat4identity(Mtemp);
	Mtemp[12] = x;
	Mtemp[13] = y;
	Mtemp[14] = z;
};

void mat4perspective(float Mtemp[], float vfov, float aspect, float znear, float zfar)
{
	mat4identity(Mtemp);

	double f = tan(1.57079637 - vfov / 2);

	Mtemp[0] = f / aspect;
	Mtemp[5] = f;
	Mtemp[10] = -((zfar + znear) / (zfar - znear));
	Mtemp[11] = -1;
	Mtemp[14] = -((2 * zfar*znear) / (zfar - znear));
	Mtemp[15] = 0;

};


int main()
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context

	window = glfwCreateWindow(1000, 800, "Tutorial 01", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwWindowHint(GLFW_REFRESH_RATE, 60);
	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	int width, height;
	glfwGetWindowSize(window, &width, &height);
	// Set viewport. This is the pixel rectangle we want to draw into.
	glViewport(0, 0, width, height);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	//glEnable(GL_CULL_FACE);


	GLfloat S[16] =
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};




	GLfloat T[16] =
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	GLfloat R[16] =
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f

	};

	GLfloat P[16] =
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	GLfloat vertices[] = {
		//shape				//color			//uv-coord
		-0.5f, 0.5f, 0.5f, 1.0f, 0.9f, 1.0f, 1.0f, 0.0f,	//löv
		-0.5f, 0.5f, -0.5f, 1.0f, 0.9f, 1.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 0.9f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 0.9f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 0.9f, 1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.9f, 1.0f, 1.0f, 0.0f,

		-3.5f, 3.5f, 3.5f, 1.0f, 1.0f, 0.9f, 1.0f, 0.0f,	//golv
		-3.5f, 3.5f, -3.5f, 1.0f, 1.0f, 0.9f, 1.0f, 1.0f,
		-3.5f, -3.5f, -3.5f, 1.0f, 1.0f, 0.9f, 0.0f, 1.0f,
		-3.5f, -3.5f, -3.5f, 1.0f, 1.0f, 0.9f, 0.0f, 1.0f,
		-3.5f, -3.5f, 3.5f, 1.0f, 1.0f, 0.9f, 0.0f, 0.0f,
		-3.5f, 3.5f, 3.5f, 1.0f, 1.0f, 0.9f, 1.0f, 0.0f,

		-3.5f, -3.5f, -3.5f, 1.0f, 1.0f, 0.9f, 1.0f, 1.0f,	//bakgrund
		3.5f, -3.5f, -3.5f, 1.0f, 1.0f, 0.9f, 1.0f, 0.0f,
		3.5f, 3.5f, -3.5f, 1.0f, 1.0f, 0.9f, 0.0f, 0.0f,
		3.5f, 3.5f, -3.5f, 1.0f, 1.0f, 0.9f, 0.0f, 0.0f,
		-3.5f, 3.5f, -3.5f, 1.0f, 1.0f, 0.9f, 0.0f, 1.0f,
		-3.5f, -3.5f, -3.5f, 1.0f, 1.0f, 0.9f, 1.0f, 1.0f
	};

	// Create Vertex Array Object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create a Vertex Buffer Object and copy the vertex data to it
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	TriangleSoup palmShape, manShape, planeShape;
	Shader palmShader, leafShader;


	palmShape.readOBJ("Meshes/Palm_Tree.obj");

	manShape.readOBJ("Meshes/sittingman.obj");

	planeShape.readOBJ("Meshes/plane.obj");



	//load shaders
	GLuint shaderLeafProgram = LoadShaders("vertexShader.glsl", "fragmentShader.glsl");

	palmShader.createShader("vertex.glsl", "fragment.glsl");

	leafShader.createShader("vertexShader.glsl", "fragmentShader.glsl");


	//useShader
	glUseProgram(shaderLeafProgram);

	Texture manTexture, palmTexture, planeTexture;

	palmTexture.createTexture("Textures/ENV_MP_Iraq_palm_tree_01_D_c.tga");


	manTexture.createTexture("Textures/SMan0003.tga");

	planeTexture.createTexture("Textures/sampleDog.tga");

	/*
	GLuint textures[2];
	glGenTextures(2, textures);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	bmpTexture("sampleDog.png", shaderLeafProgram, "texDog", 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	bmpTexture("sampleLeaf.png", shaderLeafProgram, "texLeaf", 1);*/



	// --- Put this before the rendering loop
	// Locate the sampler2D uniform in the shader program

	GLint location_T = glGetUniformLocation(palmShader.programID, "T");

	GLint location_P = glGetUniformLocation(palmShader.programID, "P");

	GLint location_tex = glGetUniformLocation(palmShader.programID, "tex");

	GLint MatrixID = glGetUniformLocation(leafShader.programID, "MVP");

	GLint uniModel = glGetUniformLocation(leafShader.programID, "model");

	GLint uniColor = glGetUniformLocation(leafShader.programID, "overrideColor");




	// Specify the layout of the vertex data
	GLint posAttrib = glGetAttribLocation(shaderLeafProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);

	GLint colAttrib = glGetAttribLocation(shaderLeafProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	GLint texAttrib = glGetAttribLocation(shaderLeafProgram, "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));






	// Compute the MVP matrix from keyboard and mouse input

	//sätter alla variabler
	double airCoeff = 1.28;
	double dens = 1.2041;
	double area = 0.0025;
	double mass = 0.005;
	btScalar transMatrix[16];

	btVector3 pos(0, 0, 0);
	btVector3 startAngVel(0.f, 0.f, 0.f);
	btVector3  flu(0.0f, 0.0f, 0.0f);
	btVector3 wind(0.0f, 0.0f, 0.0f);

	vector <Leaf> theLeaves;
	World theWorld;

	srand(time(NULL));

	btVector3 airCurrent = wind + theWorld.getDynamicsWorld()->getGravity();

	for (int i = 0; i < 50; i++)
	{
		float randNumbX = rand() % 10 - 5;
		float randNumbY = rand() % 10 - 5;
		float randNumbZ = rand() % 10 - 5;
		pos = btVector3(randNumbX, randNumbY, randNumbZ);
		startAngVel = btVector3(randNumbX * 10, randNumbY * 5, randNumbZ * 10);
		Leaf newLeaf(mass, area, dens, airCoeff, pos, flu, startAngVel);

		theLeaves.push_back(newLeaf);

		theWorld.getDynamicsWorld()->addRigidBody(newLeaf.getBody());
	}

	do {




		// Dark blue background
		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float time = (float)glfwGetTime();


		/*
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);*/

		/*
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glm::mat4 skal = glm::scale(2.0f, 2.0f, 2.0f);

		glm::mat4 rot = glm::rotate(90.0f, 0.0f, 0.0f, 1.0f);

		glm::mat4 model = skal*rot;

		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

		//draw plane
		glDrawArrays(GL_TRIANGLES, 6, 18);

		*/

		//draw palm and boy
		mat4scale(T, 0.2f);


		mat4translate(R, 0.0, 18.0, 0.0);
		mat4mult(R, T, T);

		mat4translate(R, 0, 0, -80);
		mat4mult(R, T, T);

		mat4perspective(P, 0.7, 1, 0.1, 100.0);


		glUniformMatrix4fv(location_T, 1, GL_FALSE, T); // Copy the value
		glUniformMatrix4fv(location_P, 1, GL_FALSE, P); // Copy the value

		glUseProgram(palmShader.programID);
		glBindTexture(GL_TEXTURE_2D, manTexture.textureID);
		glUniform1i(location_tex, 0);
		manShape.render();


		mat4scale(T, 0.2f);
		mat4rotx(R, 4.6);
		mat4mult(R, T, T);


		mat4translate(R, 0, 0, -80);
		mat4mult(R, T, T);



		glUniformMatrix4fv(location_T, 1, GL_FALSE, T); // Copy the value

		glBindTexture(GL_TEXTURE_2D, palmTexture.textureID);
		glUniform1i(location_tex, 0);
		palmShape.render();


		float ratio;
		int width, height;
		//float velocity = getVelocity(tid);

		theWorld.getDynamicsWorld()->stepSimulation(1 / 100.f, 100000);
		btVector3 velo;
		double airRes = 0;
		btTransform trans;
		btTransform trans_local;
		// Measure speed


		for (std::vector<Leaf>::iterator it = theLeaves.begin(); it != theLeaves.end(); ++it)
		{

			//normaliserande av en kraftevektor ger en begränsad kraftpåverkan senare, 
			//borde tänkas om lite. airCurrent2 är lite fulhax
			if (it->getBody()->getCenterOfMassPosition().getY() > -30.f) {

				btVector3 airCurrent2 = it->normVec(airCurrent);
				btVector3 normal = it->normVec(it->getRotation());
				double areaMult = it->bulletScalar(normal, airCurrent2);
				btVector3 pos = it->getPosition();

				velo = it->getBody()->getLinearVelocity();
				//glTranslatef(it->getFlutter(it->getAngVel()).getX(), it->getFlutter(it->getAngVel()).getY(),it->getFlutter(it->getAngVel()).getZ());

				it->getBody()->applyCentralForce(btVector3(it->getFlutter(it->getAngVel(), areaMult).getX(), it->getFlutter(it->getAngVel(), areaMult).getY(), it->getFlutter(it->getAngVel(), areaMult).getZ()));

				it->getBody()->setAngularVelocity(it->getAngVel());
				//bryter cirkulationen runt egen axel
				//it->getBody()->setAngularVelocity(areaMult*it->noise());

				it->getBody()->applyCentralForce(airCurrent*mass);
				//it->getBody()->applyCentralImpulse(it->noise());


				if (areaMult >= 1) {
					std::cout << areaMult << endl;
				}


				airRes = it->getAirResistance(velo, areaMult*area, dens);

				it->getBody()->applyTorque(it->getAngVel());
				it->getBody()->applyCentralForce(btVector3(0.f, airRes + it->getFlutter(it->getAngVel(), areaMult).getZ(), 0.f));
				it->getBody()->getMotionState()->getWorldTransform(trans);

				trans.getOpenGLMatrix(transMatrix);

				//glUseProgram(leafShader.programID);
				glm::mat4 skal = glm::scale(1.1f, 1.1f, 1.1f);

				glm::mat4 trans = glm::translate(0.0f, 0.0f, -80.0f);

				glm::mat4 model = skal*trans;


				mat4scale(T, 0.2f);

				mat4translate(R, 0.0, 18.0, 0.0);
				mat4mult(R, T, T);

				mat4translate(R, 0, 0, -80);
				mat4mult(R, glm::value_ptr(model), T);


				//glUniformMatrix4fv(uniModel, 1, GL_FALSE, T);



				glBindTexture(GL_TEXTURE_2D, planeTexture.textureID);
				glUniform1i(location_tex, 0);

				glUniformMatrix4fv(uniModel, 1, GL_FALSE, transMatrix);

				glMultMatrixf((GLfloat*)transMatrix);

				//glDrawArrays(GL_TRIANGLES, 0, 6);
				//glUniform3f(uniColor, 1.0f, 1.0f, 1.0f);

				planeShape.render();



				//glUniform3f(uniColor, 1.0f, 1.0f, 1.0f);
			}
			else {
				it->getBody()->translate(btVector3(0.0f, 35.f, 0.f));
			}

		}

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	glfwDestroyWindow(window);
	glfwTerminate();

	//glDeleteTextures(2, textures);

	glDeleteProgram(shaderLeafProgram);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);


	glfwTerminate();
	return 0;
}