
#define _USE_MATH_DEFINES
#include <cmath>
//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

// contains new std::shuffle definition
#include <algorithm>
#include <random>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

// Shader include
#include "Headers/Shader.h"

// Model geometric includes
#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"
#include "Headers/FirstPersonCamera.h"
#include "Headers/ThirdPersonCamera.h"

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Headers/Texture.h"

// Include loader Model class
#include "Headers/Model.h"

// Include Terrain
#include "Headers/Terrain.h"

#include "Headers/AnimationUtils.h"

// Include Colision headers functions
#include "Headers/Colisiones.h"

// OpenAL include
#include <AL/alut.h>

//Shadownox inclide
#include "Headers/ShadowBox.h"

// Include text reandering
#include "Headers/FontTypeRendering.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

int screenWidth;
int screenHeight;

const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;

GLFWwindow *window;

Shader shader;
//Shader con skybox
Shader shaderSkybox;
//Shader con multiples luces
Shader shaderMulLighting;
//Shader para el terreno
Shader shaderTerrain;
//Shader para las particulas de fountain
Shader shaderParticlesFountain;
//Shader para las particulas de fuego
Shader shaderParticlesFire;
//Shader para las particulas de lluvia
Shader shaderParticlesLluvia;
//Shader para visualizar el buffer de profundidad
Shader shaderViewDepth;
//Shader para dibujar el buffer de profunidad
Shader shaderDepth;
//Shader para dibujar una textura
Shader shaderTextura;

//Shadow box object
ShadowBox* shadowBox;

std::shared_ptr<Camera> camera(new ThirdPersonCamera());
float distanceFromTarget = 7.0;

Sphere skyboxSphere(20, 20);
Box boxCollider;
Sphere sphereCollider(10, 10);
Box boxViewDepth;
Box boxLightViewBox;
Box boxIntro;

// Models complex instances
Model modelRock;


// Terrenator
Model modelTerrenator;
Model modelTerrenatorFrontLeftWheel;
Model modelTerrenatorFrontRightWheel;
Model modelTerrenatorRearLeftWheel;
Model modelTerrenatorRearRightWheel;
Model modelTerrenatorVentana;

//Finish line
Model modelMeta;
Model modelDartLegoBody;
// Guardrail
Model modelGuardrail0;
Model modelGuardrail1;
// Hierba
Model modelGrass;
// Fountain
Model modelFountain;

//Model barrel
Model modelBarrel;
//Model wheels
Model modelThreeWheels;
//Model rock2
Model modelRock2;

//Celebrate
Model victoryModelAnimate;

// Terrain model instance
Terrain terrain(-1, -1, 200, 16, "../Textures/heightmap_2.png");


//Text rendering
FontTypeRendering::FontTypeRendering* textLifeRender;

GLuint textureInicio0ID, textureInicio1ID, textureInicio2ID, textureControlID, textureCreditosID,textureCespedID, textureWallID, textureWindowID, textureHighwayID, textureLandingPadID;
GLuint textureTerrainBackgroundID, textureTerrainRID, textureAsfaltoID, textureTerrainBID, textureTerrainBlendMapID;
GLuint textureParticleFountainID, textureParticleFireID, texId;
GLuint skyboxTextureID;
GLuint texturaActivaID;

bool iniciaPartida = false, presionarOpcion = false;
bool ganaste = false;
int seleccionTextura = 0;

GLenum types[6] = {
GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

std::string fileNames[6] = { "../Textures/mp_bloodvalley/blood-valley_ft.tga",
		"../Textures/mp_bloodvalley/blood-valley_bk.tga",
		"../Textures/mp_bloodvalley/blood-valley_up.tga",
		"../Textures/mp_bloodvalley/blood-valley_dn.tga",
		"../Textures/mp_bloodvalley/blood-valley_rt.tga",
		"../Textures/mp_bloodvalley/blood-valley_lf.tga" };

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

// Model matrix definitions
glm::mat4 matrixModelRock = glm::mat4(1.0);
glm::mat4 modelMatrixDart = glm::mat4(1.0f);
glm::mat4 modelMatrixFountain = glm::mat4(1.0f);

glm::mat4 modelMatrixTerrenator = glm::mat4(1.0f);
glm::mat4 modelMatrixBarrel = glm::mat4(1.0f);
glm::mat4 modelMatrixThreeWheels = glm::mat4(1.0f);
glm::mat4 modelMatrixRock2 = glm::mat4(1.0f);
glm::mat4 modelMatrixVictory = glm::mat4(1.0f);
glm::mat4 modelMatrixVictory2 = glm::mat4(1.0f);
glm::mat4 modelMatrixVictory3 = glm::mat4(1.0f);
glm::mat4 modelMatrixMeta = glm::mat4(1.0f);



int animationIndex = 1;
int modelSelected = 1;
bool enableCountSelected = true;

// Variables to animations keyframes
bool saveFrame = false, availableSave = true;
std::ofstream myfile;
std::string fileName = "";
bool record = false;


//vidas
int vidas = 3;

// giro Terrenator
float rotWheelsX = 0.0;
float rotWheelsY = 0.0;

// Terrenator caida
double inicioCaida = 0;


// Guardrails positions 0
std::vector<glm::vec3> guardrailPosition = 
{	glm::vec3(30.45, 3.0, 31.0), 
	glm::vec3(43.22, 2.16, 31.0), 
	glm::vec3(-48.32, 0.0, 56.24)
};
std::vector<float> guardrailOrientation = { 
	0.0, 0.0, 
	0.0 
};

// Guardrails positions 1
// Distancia entre guardrails 12.62 | 6.31
std::vector<glm::vec3> guardrailPosition1 =
{ 
	//Interior
	glm::vec3(-26.24, 0.0, 31.0),
	glm::vec3(-13.62, 0.0, 31.0),
	glm::vec3(-1.0  , 0.0, 31.0), 
	glm::vec3(11.62 , 0.0, 31.0),
	glm::vec3(24.24 , 0.0, 31.0), //*
	glm::vec3(36.86 , 0.0, 31.0),
	glm::vec3(49.48 , 0.0, 31.0), //*

	glm::vec3(55.79 , 0.0, 37.31),
	glm::vec3(55.79 , 0.0, 49.93),
	
	glm::vec3(49.48 , 0.0, 56.24),
	glm::vec3(36.86 , 0.0, 56.24),
	glm::vec3(24.24 , 0.0, 56.24),
	glm::vec3(11.62 , 0.0, 56.24),
	glm::vec3(-1.0  , 0.0, 56.24),
	glm::vec3(-13.62, 0.0, 56.24),
	glm::vec3(-26.24, 0.0, 56.24),
	glm::vec3(-38.86, 0.0, 56.24),

	glm::vec3(-51.48, 0.0, 49.93),
	glm::vec3(-51.48, 0.0, 37.31),
	glm::vec3(-51.48, 0.0, 24.69),
	glm::vec3(-51.48, 0.0, 12.07),
	glm::vec3(-51.48, 0.0, -0.55),
	glm::vec3(-51.48, 0.0, -13.17),
	glm::vec3(-51.48, 0.0, -25.79),
	glm::vec3(-51.48, 0.0, -38.41),
	glm::vec3(-51.48, 0.0, -51.03),

	glm::vec3(-45.17, 0.0, -57.34),
	glm::vec3(-32.55, 0.0, -57.34),
	glm::vec3(-19.93, 0.0, -57.34),
	glm::vec3(-7.31, 0.0, -57.34),
	glm::vec3( 5.31, 0.0, -57.34),
	glm::vec3( 17.93, 0.0, -57.34),
	glm::vec3( 30.55, 0.0, -57.34),
	glm::vec3( 43.17, 0.0, -57.34),

	glm::vec3( 49.48, 0.0, -51.03),
	glm::vec3( 49.48, 0.0, -38.41),
	glm::vec3( 49.48, 0.0, -32.10),

	glm::vec3( 43.17, 0.0, -25.79),
	glm::vec3( 30.55, 0.0, -25.79),
	glm::vec3( 17.93, 0.0, -25.79),
	glm::vec3(  5.31, 0.0, -25.79),
	glm::vec3( -7.31, 0.0, -25.79),
	glm::vec3(-19.93, 0.0, -25.79),
	glm::vec3(-26.24, 0.0, -25.79),

	glm::vec3(-32.55, 0.0, -19.48),
	glm::vec3(-32.55, 0.0, -6.86),
	glm::vec3(-32.55 , 0.0, 5.76),
	glm::vec3(-32.55, 0.0, 18.38),
	glm::vec3(-32.55, 0.0, 24.69),
	
	//Exterior

	glm::vec3( -6.0, 0.0, 10.0),
	glm::vec3(  6.62, 0.0, 10.0),
	glm::vec3(  19.24, 0.0, 10.0),
	glm::vec3(  31.86, 0.0, 10.0),
	glm::vec3(  44.48, 0.0, 10.0),
	glm::vec3(  57.1, 0.0, 10.0),
	glm::vec3(  69.72, 0.0, 10.0),

	glm::vec3(  76.03, 0.0, 16.31),
	glm::vec3(76.03, 0.0, 28.93),
	glm::vec3(76.03, 0.0, 41.51),
	glm::vec3(76.03, 0.0, 54.17),
	glm::vec3(76.03, 0.0, 66.79),
	glm::vec3(76.03, 0.0, 73.1),

	glm::vec3(69.72, 0.0, 79.41),
	glm::vec3(57.1, 0.0, 79.41),
	glm::vec3(44.48, 0.0, 79.41),
	glm::vec3(31.86, 0.0, 79.41),
	glm::vec3(19.24, 0.0, 79.41),
	glm::vec3(6.62, 0.0, 79.41),
	glm::vec3(-6, 0.0, 79.41),
	glm::vec3(-18.62, 0.0, 79.41),
	glm::vec3(-31.24, 0.0, 79.41),
	glm::vec3(-43.83, 0.0, 79.41),
	glm::vec3(-56.48, 0.0, 79.41),
	glm::vec3(-62.79, 0.0, 79.41),

	glm::vec3(-69.1, 0.0, 73.1),
	glm::vec3(-69.1, 0.0, 60.48),
	glm::vec3(-69.1, 0.0, 47.86),
	glm::vec3(-69.1, 0.0, 35.24),
	glm::vec3(-69.1, 0.0, 22.62),
	glm::vec3(-69.1, 0.0, 10),
	glm::vec3(-69.1, 0.0, -2.62),
	glm::vec3(-69.1, 0.0, -15.24),
	glm::vec3(-69.1, 0.0, -27.86),
	glm::vec3(-69.1, 0.0, -40.48),
	glm::vec3(-69.1, 0.0, -53.1),
	glm::vec3(-69.1, 0.0, -65.72),
	glm::vec3(-69.1, 0.0, -72.03),

	glm::vec3(-62.79, 0.0, -78.34),
	glm::vec3(-50.17, 0.0, -78.34),
	glm::vec3(-37.55, 0.0, -78.34),
	glm::vec3(-24.93, 0.0, -78.34),
	glm::vec3(-12.31, 0.0, -78.34),
	glm::vec3( -0.31, 0.0, -78.34),
	glm::vec3( 12.62, 0.0, -78.34),
	glm::vec3( 25.24, 0.0, -78.34),
	glm::vec3( 37.86, 0.0, -78.34),
	glm::vec3( 50.48, 0.0, -78.34),
	glm::vec3( 63.1, 0.0, -78.34),
	glm::vec3( 69.41, 0.0, -78.34),

	glm::vec3( 75.72, 0.0, -72.03),
	glm::vec3(75.72, 0.0, -59.41),
	glm::vec3(75.72, 0.0, -46.79),
	glm::vec3(75.72, 0.0, -34.17),
	glm::vec3(75.72, 0.0, -21.55),
	glm::vec3(75.72, 0.0, -8.93),

	glm::vec3(69.41, 0.0, -2.62),
	glm::vec3(56.79, 0.0, -2.62),
	glm::vec3(44.17, 0.0, -2.62),
	glm::vec3(31.55, 0.0, -2.62),
	glm::vec3(18.93, 0.0, -2.62),
	glm::vec3(6.31, 0.0, -2.62),
	glm::vec3(-6.31, 0.0, -2.62),

	glm::vec3(-12.62, 0.0, 3.69),
	
	glm::vec3(36.86, 0.0, 24.69),		//No retorno
	glm::vec3(36.86, 0.0, 16.07),
};
std::vector<float> guardrailOrientation1 = { 
	//Interior
	180.0, 180.0, 180.0, 180.0, 180.0, 180, 180.0,
	-90.0, -90.0,
	  0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,
	 90.0,  90.0,  90.0,  90.0,  90.0,  90.0,  90.0,  90.0,  90.0,
	180.0, 180.0, 180.0, 180.0, 180.0, 180.0, 180.0, 180.0,
	 90.0,  90.0,  90.0,
	  0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,
	 90.0,  90.0,  90.0,  90.0,  90.0,
	 
	 //Exterior
	 0.0,    0.0,   0.0,   0.0,   0.0,   0.0,   0.0,
   -90.0,  -90.0, -90.0, -90.0, -90.0, -90.0,
  -180.0, -180.0,-180.0,-180.0, -180.0,-180.0,-180.0, -180.0,-180.0,-180.0, -180.0,-180.0,
    90.0,   90.0,  90.0,   90.0,  90.0,  90.0,  90.0,   90.0,  90.0,  90.0,   90.0,  90.0,  90.0,
	 0.0,    0.0,   0.0,    0.0,   0.0,    0.0,   0.0,    0.0,  0.0,   0.0,    0.0,   0.0,
   -90.0,  -90.0, -90.0,  -90.0, -90.0,  -90.0,
   180.0,  180.0, 180.0,  180.0, 180.0,  180.0, 180.0,
    90.0,
	90.0, 90
};

// Blending model unsorted
std::map<std::string, glm::vec3> blendingUnsorted = {
		{"aircraft", glm::vec3(10.0, 0.0, -17.5)},
		{"fountain", glm::vec3(5.0, 0.0, -40.0)},
		{"fire", glm::vec3(0.0, 0.0, 7.0)},
		{"lluvia", glm::vec3(0.0, 0.0, 0.0)},
		{"ventana", glm::vec3(0.0, 0.0, 0.0)}
};

double deltaTime;
double currTime, lastTime;
double ultimoChoque;

// Jump variables
bool isJump = false;
float GRAVITY = 1.81;
double tmv = 0;
double startTimeJump = 0;

// Definition for the particle system
GLuint initVel, startTime;
GLuint VAOParticles;
GLuint nParticles = 8000;
double currTimeParticlesAnimation, lastTimeParticlesAnimation;

// Definition for the particle system fire
GLuint initVelFire, startTimeFire;
GLuint VAOParticlesFire;
GLuint nParticlesFire = 8000;
GLuint posBuf[2], velBuf[2], age[2];
GLuint particleArray[2];
GLuint feedback[2];
GLuint drawBuf = 1;
float particleSize = 0.08, particleLifetime = 3.0;
double currTimeParticlesAnimationFire, lastTimeParticlesAnimationFire;

// Colliders
std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> > collidersOBB;
std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> > collidersSBB;

// Framesbuffers
GLuint depthMap, depthMapFBO;

/**********************
 * OpenAL config
 */

// OpenAL Defines
#define NUM_BUFFERS 7
#define NUM_SOURCES 7
#define NUM_ENVIRONMENTS 1
// Listener
ALfloat listenerPos[] = { 0.0, 0.0, 4.0 };
ALfloat listenerVel[] = { 0.0, 0.0, 0.0 };
ALfloat listenerOri[] = { 0.0, 0.0, 1.0, 0.0, 1.0, 0.0 };
// Source 0
ALfloat source0Pos[] = { -2.0, 0.0, 0.0 };
ALfloat source0Vel[] = { 0.0, 0.0, 0.0 };
// Source 1
ALfloat source1Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source1Vel[] = { 0.0, 0.0, 0.0 };
// Source 2
ALfloat source2Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source2Vel[] = { 0.0, 0.0, 0.0 };

// Source 3
ALfloat source3Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source3Vel[] = { 0.0, 0.0, 0.0 };

// Source 4
ALfloat source4Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source4Vel[] = { 0.0, 0.0, 0.0 };

// Source 5
ALfloat source5Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source5Vel[] = { 0.0, 0.0, 0.0 };

// Source 6
ALfloat source6Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source6Vel[] = { 0.0, 0.0, 0.0 };

// Buffers
ALuint buffer[NUM_BUFFERS];
ALuint source[NUM_SOURCES];
ALuint environment[NUM_ENVIRONMENTS];
// Configs
ALsizei size, freq;
ALenum format;
ALvoid *data;
int ch;
ALboolean loop;
std::vector<bool> sourcesPlay = {true, true, true, true, true, true,true};

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void initParticleBuffers();
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);
void prepareScene();
void prepareDepthScene();
void renderScene(bool renderParticles = true);

void initParticleBuffers() {
	// Generate the buffers
	glGenBuffers(1, &initVel);   // Initial velocity buffer
	glGenBuffers(1, &startTime); // Start time buffer

	// Allocate space for all buffers
	int size = nParticles * 3 * sizeof(float);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(float), NULL, GL_STATIC_DRAW);

	// Fill the first velocity buffer with random velocities
	glm::vec3 v(0.0f);
	float velocity, theta, phi;
	GLfloat *data = new GLfloat[nParticles * 3];
	for (unsigned int i = 0; i < nParticles; i++) {

		theta = glm::mix(0.0f, glm::pi<float>() / 6.0f, ((float)rand() / RAND_MAX));
		phi = glm::mix(0.0f, glm::two_pi<float>(), ((float)rand() / RAND_MAX));

		v.x = sinf(theta) * cosf(phi);
		v.y = cosf(theta);
		v.z = sinf(theta) * sinf(phi);

		velocity = glm::mix(0.6f, 0.8f, ((float)rand() / RAND_MAX));
		v = glm::normalize(v) * velocity;

		data[3 * i] = v.x;
		data[3 * i + 1] = v.y;
		data[3 * i + 2] = v.z;
	}
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

	// Fill the start time buffer
	delete[] data;
	data = new GLfloat[nParticles];
	float time = 0.0f;
	float rate = 0.00075f;
	for (unsigned int i = 0; i < nParticles; i++) {
		data[i] = time;
		time += rate;
	}
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), data);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete[] data;

	glGenVertexArrays(1, &VAOParticles);
	glBindVertexArray(VAOParticles);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void initParticleBuffersFire() {
	// Generate the buffers
	glGenBuffers(2, posBuf);    // position buffers
	glGenBuffers(2, velBuf);    // velocity buffers
	glGenBuffers(2, age);       // age buffers

	// Allocate space for all buffers
	int size = nParticlesFire * sizeof(GLfloat);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, age[1]);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);

	// Fill the first age buffer
	std::vector<GLfloat> initialAge(nParticlesFire);
	float rate = particleLifetime / nParticlesFire;
	for(unsigned int i = 0; i < nParticlesFire; i++ ) {
		int index = i - nParticlesFire;
		float result = rate * index;
		initialAge[i] = result;
	}
	// Shuffle them for better looking results
	//Random::shuffle(initialAge);
	auto rng = std::default_random_engine {};
	std::shuffle(initialAge.begin(), initialAge.end(), rng);
	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, initialAge.data());

	glBindBuffer(GL_ARRAY_BUFFER,0);

	// Create vertex arrays for each set of buffers
	glGenVertexArrays(2, particleArray);

	// Set up particle array 0
	glBindVertexArray(particleArray[0]);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	// Set up particle array 1
	glBindVertexArray(particleArray[1]);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, age[1]);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	// Setup the feedback objects
	glGenTransformFeedbacks(2, feedback);

	// Transform feedback 0
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, age[0]);

	// Transform feedback 1
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, age[1]);

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
}

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
				glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
				nullptr);

	if (window == nullptr) {
		std::cerr
				<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
				<< std::endl;
		destroy();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Inicialización de los shaders
	shader.initialize("../Shaders/colorShader.vs", "../Shaders/colorShader.fs");
	shaderSkybox.initialize("../Shaders/skyBox.vs", "../Shaders/skyBox_fog.fs");
	shaderMulLighting.initialize("../Shaders/iluminacion_textura_animation_shadow.vs", "../Shaders/multipleLights_shadow.fs");
	shaderTerrain.initialize("../Shaders/terrain_shadow.vs", "../Shaders/terrain_shadow.fs");
	shaderParticlesFountain.initialize("../Shaders/particlesFountain.vs", "../Shaders/particlesFountain.fs");
	shaderParticlesFire.initialize("../Shaders/particlesFire.vs", "../Shaders/particlesFire.fs", {"Position", "Velocity", "Age"});
	shaderParticlesLluvia.initialize("../Shaders/particlesLluvia.vs", "../Shaders/particlesLluvia.fs", { "Position", "Velocity", "Age" });
	shaderViewDepth.initialize("../Shaders/texturizado.vs", "../Shaders/texturizado_depth_view.fs");
	shaderDepth.initialize("../Shaders/shadow_mapping_depth.vs", "../Shaders/shadow_mapping_depth.fs");
	shaderTextura.initialize( "../Shaders/texturizado.vs", "../Shaders/texturizado.fs");

	

	// Inicializacion de los objetos.
	skyboxSphere.init();
	skyboxSphere.setShader(&shaderSkybox);
	skyboxSphere.setScale(glm::vec3(100.0f, 100.0f, 100.0f));

	boxCollider.init();
	boxCollider.setShader(&shader);
	boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	sphereCollider.init();
	sphereCollider.setShader(&shader);
	sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	boxIntro.init();
	boxIntro.setShader(&shaderTextura);
	boxIntro.setScale(glm::vec3(2.0f, 2.0f, 1.0f));


	boxViewDepth.init();
	boxViewDepth.setShader(&shaderViewDepth);

	boxLightViewBox.init();
	boxLightViewBox.setShader(&shaderViewDepth);

	/*modelRock.loadModel("../models/rock/rock.obj");
	modelRock.setShader(&shaderMulLighting);*/

	/*modelAircraft.loadModel("../models/Aircraft_obj/E 45 Aircraft_obj.obj");
	modelAircraft.setShader(&shaderMulLighting);*/

	terrain.init();
	terrain.setShader(&shaderTerrain);
	terrain.setPosition(glm::vec3(100, 0, 100));



	// Terrenator
	modelTerrenator.loadModel("../models/Terrenator/Terrenator_chasis.obj");
	modelTerrenator.setShader(&shaderMulLighting);
	modelTerrenatorFrontLeftWheel.loadModel("../models/Terrenator/Terrenator_frontLeftWheel.obj");
	modelTerrenatorFrontLeftWheel.setShader(&shaderMulLighting);
	modelTerrenatorFrontRightWheel.loadModel("../models/Terrenator/Terrenator_frontRightWheel.obj");
	modelTerrenatorFrontRightWheel.setShader(&shaderMulLighting);
	modelTerrenatorRearLeftWheel.loadModel("../models/Terrenator/Terrenator_rearLeftWheel.obj");
	modelTerrenatorRearLeftWheel.setShader(&shaderMulLighting);
	modelTerrenatorRearRightWheel.loadModel("../models/Terrenator/Terrenator_rearRightWheel.obj");
	modelTerrenatorRearRightWheel.setShader(&shaderMulLighting);

	modelTerrenatorVentana.loadModel("../models/Terrenator/Terrenator_ventana.obj");
	modelTerrenatorVentana.setShader(&shaderMulLighting);

	// Finish Line
	
	modelMeta.loadModel("../models/finish_line/finish_line.fbx");
	modelMeta.setShader(&shaderMulLighting);

	//// Dart Lego
	//modelDartLegoBody.loadModel("../models/LegoDart/LeoDart_body.obj");
	//modelDartLegoBody.setShader(&shaderMulLighting);
	//modelDartLegoMask.loadModel("../models/LegoDart/LeoDart_mask.obj");
	//modelDartLegoMask.setShader(&shaderMulLighting);
	//modelDartLegoHead.loadModel("../models/LegoDart/LeoDart_head.obj");
	//modelDartLegoHead.setShader(&shaderMulLighting);
	//modelDartLegoLeftArm.loadModel("../models/LegoDart/LeoDart_left_arm.obj");
	//modelDartLegoLeftArm.setShader(&shaderMulLighting);
	//modelDartLegoRightArm.loadModel("../models/LegoDart/LeoDart_right_arm.obj");
	//modelDartLegoRightArm.setShader(&shaderMulLighting);
	//modelDartLegoLeftHand.loadModel("../models/LegoDart/LeoDart_left_hand.obj");
	//modelDartLegoLeftHand.setShader(&shaderMulLighting);
	//modelDartLegoRightHand.loadModel("../models/LegoDart/LeoDart_right_hand.obj");
	//modelDartLegoRightHand.setShader(&shaderMulLighting);
	//modelDartLegoLeftLeg.loadModel("../models/LegoDart/LeoDart_left_leg.obj");
	//modelDartLegoLeftLeg.setShader(&shaderMulLighting);
	//modelDartLegoRightLeg.loadModel("../models/LegoDart/LeoDart_right_leg.obj");
	//modelDartLegoRightLeg.setShader(&shaderMulLighting);

	//Guardrail model
	modelGuardrail0.loadModel("../models/guardrail/Guardrail.obj");
	modelGuardrail0.setShader(&shaderMulLighting);

	//Guardrail model 1
	modelGuardrail1.loadModel("../models/guardrail/GuardrailLargo.obj");
	modelGuardrail1.setShader(&shaderMulLighting);

	//Grass
	modelGrass.loadModel("../models/grass/grassModel.obj");
	modelGrass.setShader(&shaderMulLighting);

	////Fountain
	//modelFountain.loadModel("../models/fountain/fountain.obj");
	//modelFountain.setShader(&shaderMulLighting);

	////Mayow
	//mayowModelAnimate.loadModel("../models/mayow/personaje2.fbx");
	//mayowModelAnimate.setShader(&shaderMulLighting);

	//Barrel
	modelBarrel.loadModel("../models/Barrel/barrel.fbx");
	modelBarrel.setShader(&shaderMulLighting);

	//ThreeWheels
	modelThreeWheels.loadModel("../models/llantas/Car tire 2.obj");
	modelThreeWheels.setShader(&shaderMulLighting);

	//Rock2
	modelRock2.loadModel("../models/rock/rock2/CavePlatform1_Fbx.fbx");
	modelRock2.setShader(&shaderMulLighting);

	//////Victory
	victoryModelAnimate.loadModel("../models/victory/Victory Idle.dae");
	victoryModelAnimate.setShader(&shaderMulLighting);
	////victoryModelAnimate2.loadModel("../models/victory/Victory.dae");
	////victoryModelAnimate2.setShader(&shaderMulLighting);
	////victoryModelAnimate3.loadModel("../models/victory/Victory Idle2.dae");
	////victoryModelAnimate3.setShader(&shaderMulLighting);

	camera->setPosition(glm::vec3(0.0, 0.0, 10.0));
	camera->setDistanceFromTarget(distanceFromTarget);
	camera->setSensitivity(1.0);

	//Text
	textLifeRender = new FontTypeRendering::FontTypeRendering(screenWidth, screenHeight);
	textLifeRender->Initialize();

	// Definimos el tamanio de la imagen
	int imageWidth, imageHeight;
	FIBITMAP *bitmap;
	unsigned char *data;

	// Carga de texturas para el skybox
	Texture skyboxTexture = Texture("");
	glGenTextures(1, &skyboxTextureID);
	// Tipo de textura CUBE MAP
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(types); i++) {
		skyboxTexture = Texture(fileNames[i]);
		FIBITMAP *bitmap = skyboxTexture.loadImage(true);
		unsigned char *data = skyboxTexture.convertToData(bitmap, imageWidth,
				imageHeight);
		if (data) {
			glTexImage2D(types[i], 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		} else
			std::cout << "Failed to load texture" << std::endl;
		skyboxTexture.freeImage(bitmap);
	}

	// TEXTURA INICIO0
	Texture textureInicio0("../Textures/inicio0.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureInicio0.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureInicio0.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureInicio0ID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureInicio0ID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureInicio0.freeImage(bitmap);

	//TEXTURA INICIO 1
	Texture textureInicio1("../Textures/inicio1.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureInicio1.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureInicio1.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureInicio1ID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureInicio1ID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureInicio1.freeImage(bitmap);

	//TEXTURA INICIO 2
	Texture textureInicio2("../Textures/inicio2.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureInicio2.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureInicio2.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureInicio2ID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureInicio2ID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureInicio2.freeImage(bitmap);

	//TEXTURA CREDITOS
	Texture textureCreditos("../Textures/creditos.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureCreditos.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureCreditos.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureCreditosID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureCreditosID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureCreditos.freeImage(bitmap);

	//TEXTURA CONTROL
	Texture textureControl("../Textures/control.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureControl.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureControl.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureControlID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureControlID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureControl.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureCesped("../Textures/cesped.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureCesped.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureCesped.convertToData(bitmap, imageWidth,
			imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureCespedID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureCespedID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureCesped.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureWall("../Textures/whiteWall.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureWall.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureWall.convertToData(bitmap, imageWidth,
			imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureWallID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureWallID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureWall.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureWindow("../Textures/ventana.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureWindow.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureWindow.convertToData(bitmap, imageWidth,
			imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureWindowID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureWindowID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureWindow.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureHighway("../Textures/highway.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureHighway.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureHighway.convertToData(bitmap, imageWidth,
			imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureHighwayID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureHighwayID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureHighway.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureLandingPad("../Textures/landingPad.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureLandingPad.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureLandingPad.convertToData(bitmap, imageWidth,
			imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureLandingPadID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureLandingPadID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureLandingPad.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTerrainBackground("../Textures/nieve.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainBackground.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainBackground.convertToData(bitmap, imageWidth,
			imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainBackgroundID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainBackgroundID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainBackground.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureHojasSecas("../Textures/hojasCongeladas.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureHojasSecas.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureHojasSecas.convertToData(bitmap, imageWidth,
			imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainRID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureHojasSecas.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureAsfalto("../Textures/asfalto.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureAsfalto.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureAsfalto.convertToData(bitmap, imageWidth,
			imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureAsfaltoID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureAsfaltoID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureAsfalto.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texturePinturaAmarilla("../Textures/pinturaAmarilla.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = texturePinturaAmarilla.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texturePinturaAmarilla.convertToData(bitmap, imageWidth,
			imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainBID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texturePinturaAmarilla.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTerrainBlendMap("../Textures/blendMap2.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainBlendMap.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainBlendMap.convertToData(bitmap, imageWidth,
			imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainBlendMapID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainBlendMap.freeImage(bitmap);

	Texture textureParticlesFountain("../Textures/nieve3.png");
	bitmap = textureParticlesFountain.loadImage();
	data = textureParticlesFountain.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureParticleFountainID);
	glBindTexture(GL_TEXTURE_2D, textureParticleFountainID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureParticlesFountain.freeImage(bitmap);

	Texture textureParticleFire("../Textures/fire.png");
	bitmap = textureParticleFire.loadImage();
	data = textureParticleFire.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureParticleFireID);
	glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureParticleFire.freeImage(bitmap);

	std::uniform_real_distribution<float> distr01 = std::uniform_real_distribution<float>(0.0f, 1.0f);
	std::mt19937 generator;
	std::random_device rd;
	generator.seed(rd());
	int size = nParticlesFire * 2;
	std::vector<GLfloat> randData(size);
	for( int i = 0; i < randData.size(); i++ ) {
		randData[i] = distr01(generator);
	}

	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_1D, texId);
	glTexStorage1D(GL_TEXTURE_1D, 1, GL_R32F, size);
	glTexSubImage1D(GL_TEXTURE_1D, 0, 0, size, GL_RED, GL_FLOAT, randData.data());
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	/*shaderParticlesFire.setInt("Pass", 1);
	shaderParticlesFire.setInt("ParticleTex", 0);
	shaderParticlesFire.setInt("RandomTex", 1);
	shaderParticlesFire.setFloat("ParticleLifetime", particleLifetime);
	shaderParticlesFire.setFloat("ParticleSize", particleSize);
	shaderParticlesFire.setVectorFloat3("Accel", glm::value_ptr(glm::vec3(0.0f,0.1f,0.0f)));
	shaderParticlesFire.setVectorFloat3("Emitter", glm::value_ptr(glm::vec3(0.0f)));*/

	shaderParticlesLluvia.setInt("Pass", 1);
	shaderParticlesLluvia.setInt("ParticleTex", 0);
	shaderParticlesLluvia.setInt("RandomTex", 1);
	shaderParticlesLluvia.setFloat("ParticleLifetime", particleLifetime);
	shaderParticlesLluvia.setFloat("ParticleSize", particleSize);
	shaderParticlesLluvia.setVectorFloat3("Accel", glm::value_ptr(glm::vec3(0.0f, -10.3f, 0.0f)));
	shaderParticlesLluvia.setVectorFloat3("Emitter", glm::value_ptr(glm::vec3(0.0f, 10.0, 0.0f)));

	glm::mat3 basis;
	glm::vec3 u, v, n;
	v = glm::vec3(0,1,0);
	n = glm::cross(glm::vec3(1,0,0), v);
	if( glm::length(n) < 0.00001f ) {
		n = glm::cross(glm::vec3(0,1,0), v);
	}
	u = glm::cross(v,n);
	basis[0] = glm::normalize(u);
	basis[1] = glm::normalize(v);
	basis[2] = glm::normalize(n);
	shaderParticlesFire.setMatrix3("EmitterBasis", 1, false, glm::value_ptr(basis));

	shaderParticlesLluvia.setMatrix3("EmitterBasis", 1, false, glm::value_ptr(basis));

	/*******************************************
	 * Inicializacion de los buffers de la fuente
	 *******************************************/
	initParticleBuffers();

	/*******************************************
	 * Inicializacion de los buffers del fuego
	 *******************************************/
	initParticleBuffersFire();

	/*******************************************
	 * Inicializacion del framebuffer para
	 * almacenar el buffer de profunidadad
	 *******************************************/
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
				 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	/*******************************************
	 * OpenAL init
	 *******************************************/
	alutInit(0, nullptr);
	alListenerfv(AL_POSITION, listenerPos);
	alListenerfv(AL_VELOCITY, listenerVel);
	alListenerfv(AL_ORIENTATION, listenerOri);
	alGetError(); // clear any error messages
	if (alGetError() != AL_NO_ERROR) {
		printf("- Error creating buffers !!\n");
		exit(1);
	}
	else {
		printf("init() - No errors yet.");
	}
	// Config source 0
	// Generate buffers, or else no sound will happen!
	alGenBuffers(NUM_BUFFERS, buffer);
	buffer[0] = alutCreateBufferFromFile("../sounds/fountain.wav");
	buffer[1] = alutCreateBufferFromFile("../sounds/fire.wav");
	buffer[2] = alutCreateBufferFromFile("../sounds/darth_vader.wav");
	buffer[3] = alutCreateBufferFromFile("../sounds/race_track.wav");
	buffer[4] = alutCreateBufferFromFile("../sounds/racing_car.wav");
	buffer[5] = alutCreateBufferFromFile("../sounds/car_idle.wav");
	buffer[6] = alutCreateBufferFromFile("../sounds/car_crash.wav");
	int errorAlut = alutGetError();
	if (errorAlut != ALUT_ERROR_NO_ERROR){
		printf("- Error open files with alut %d !!\n", errorAlut);
		exit(2);
	}


	alGetError(); /* clear error */
	alGenSources(NUM_SOURCES, source);

	if (alGetError() != AL_NO_ERROR) {
		printf("- Error creating sources !!\n");
		exit(2);
	}
	else {
		printf("init - no errors after alGenSources\n");
	}
	alSourcef(source[0], AL_PITCH, 1.0f);
	alSourcef(source[0], AL_GAIN, 3.0f);
	alSourcefv(source[0], AL_POSITION, source0Pos);
	alSourcefv(source[0], AL_VELOCITY, source0Vel);
	alSourcei(source[0], AL_BUFFER, buffer[0]);
	alSourcei(source[0], AL_LOOPING, AL_TRUE);
	alSourcef(source[0], AL_MAX_DISTANCE, 2000);

	alSourcef(source[1], AL_PITCH, 1.0f);
	alSourcef(source[1], AL_GAIN, 3.0f);
	alSourcefv(source[1], AL_POSITION, source1Pos);
	alSourcefv(source[1], AL_VELOCITY, source1Vel);
	alSourcei(source[1], AL_BUFFER, buffer[1]);
	alSourcei(source[1], AL_LOOPING, AL_TRUE);
	alSourcef(source[1], AL_MAX_DISTANCE, 2000);

	alSourcef(source[2], AL_PITCH, 1.0f);
	alSourcef(source[2], AL_GAIN, 0.3f);
	alSourcefv(source[2], AL_POSITION, source2Pos);
	alSourcefv(source[2], AL_VELOCITY, source2Vel);
	alSourcei(source[2], AL_BUFFER, buffer[2]);
	alSourcei(source[2], AL_LOOPING, AL_TRUE);
	alSourcef(source[2], AL_MAX_DISTANCE, 500);

	//tema musical
	alSourcef(source[3], AL_PITCH, 1.0f);
	alSourcef(source[3], AL_GAIN, 1.0f);
	alSourcefv(source[3], AL_POSITION, source3Pos);
	alSourcefv(source[3], AL_VELOCITY, source3Vel);
	alSourcei(source[3], AL_BUFFER, buffer[3]);
	alSourcei(source[3], AL_LOOPING, AL_TRUE);
	alSourcef(source[3], AL_MAX_DISTANCE, 500);

	//Terrenator en marcha
	alSourcef(source[4], AL_PITCH, 1.0f);
	alSourcef(source[4], AL_GAIN, 0.05f);
	alSourcefv(source[4], AL_POSITION, source4Pos);
	alSourcefv(source[4], AL_VELOCITY, source4Vel);
	alSourcei(source[4], AL_BUFFER, buffer[4]);
	alSourcei(source[4], AL_LOOPING, AL_TRUE);
	alSourcef(source[4], AL_MAX_DISTANCE, 500);

	//Terrenator en reposo
	alSourcef(source[5], AL_PITCH, 1.0f);
	alSourcef(source[5], AL_GAIN, 3.0f);
	alSourcefv(source[5], AL_POSITION, source5Pos);
	alSourcefv(source[5], AL_VELOCITY, source5Vel);
	alSourcei(source[5], AL_BUFFER, buffer[5]);
	alSourcei(source[5], AL_LOOPING, AL_TRUE);
	alSourcef(source[5], AL_MAX_DISTANCE, 500);

	//Choque
	alSourcef(source[6], AL_PITCH, 1.0f);
	alSourcef(source[6], AL_GAIN, 0.3f);
	alSourcefv(source[6], AL_POSITION, source6Pos);
	alSourcefv(source[6], AL_VELOCITY, source6Vel);
	alSourcei(source[6], AL_BUFFER, buffer[6]);
	alSourcei(source[6], AL_LOOPING, AL_FALSE);
	alSourcef(source[6], AL_MAX_DISTANCE, 500);
}

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	// Shaders Delete
	shader.destroy();
	shaderMulLighting.destroy();
	shaderSkybox.destroy();
	shaderTerrain.destroy();
	shaderParticlesFountain.destroy();
	shaderParticlesFire.destroy();
	shaderParticlesLluvia.destroy();

	// Basic objects Delete
	skyboxSphere.destroy();
	boxCollider.destroy();
	sphereCollider.destroy();
	boxViewDepth.destroy();
	boxLightViewBox.destroy();

	// Terrains objects Delete
	terrain.destroy();

	// Custom objects Delete
	modelDartLegoBody.destroy();
	modelRock.destroy();

	modelGuardrail0.destroy();
	modelGuardrail1.destroy();

	modelGrass.destroy();
	modelFountain.destroy();

	modelTerrenator.destroy();
	modelTerrenatorFrontLeftWheel.destroy();
	modelTerrenatorFrontRightWheel.destroy();
	modelTerrenatorRearLeftWheel.destroy();
	modelTerrenatorRearRightWheel.destroy();
	modelTerrenatorVentana.destroy();

	modelMeta.destroy();
	
	modelBarrel.destroy();
	modelThreeWheels.destroy();
	modelRock2.destroy();
	victoryModelAnimate.destroy();

	// Textures Delete
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &textureCespedID);
	glDeleteTextures(1, &textureWallID);
	glDeleteTextures(1, &textureWindowID);
	glDeleteTextures(1, &textureHighwayID);
	glDeleteTextures(1, &textureLandingPadID);
	glDeleteTextures(1, &textureTerrainBackgroundID);
	glDeleteTextures(1, &textureTerrainRID);
	glDeleteTextures(1, &textureAsfaltoID);
	glDeleteTextures(1, &textureTerrainBID);
	glDeleteTextures(1, &textureTerrainBlendMapID);
	glDeleteTextures(1, &textureParticleFountainID);
	glDeleteTextures(1, &textureParticleFireID);

	// Cube Maps Delete
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDeleteTextures(1, &skyboxTextureID);

	// Remove the buffer of the fountain particles
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &initVel);
	glDeleteBuffers(1, &startTime);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAOParticles);

	// Remove the buffer of the fire particles
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(2, posBuf);
	glDeleteBuffers(2, velBuf);
	glDeleteBuffers(2, age);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	glDeleteTransformFeedbacks(2, feedback);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAOParticlesFire);
}

void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
	offsetX = xpos - lastMousePosX;
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset){
	distanceFromTarget -= yoffset;
	camera->setDistanceFromTarget(distanceFromTarget);
}

void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod) {
	if (state == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		}
	}
}

bool processInput(bool continueApplication) {
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}

	if (!iniciaPartida) {
		
		if (sourcesPlay[3]) {
			sourcesPlay[3] = false;
			alSourcePlay(source[3]);
		}
		
		bool statusEnter = glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS;
		if ( texturaActivaID == textureInicio0ID && statusEnter ) {
			iniciaPartida = true;
		} else if (texturaActivaID == textureInicio1ID && statusEnter) {
			texturaActivaID = textureControlID;
		}
		else if (texturaActivaID == textureInicio2ID && statusEnter) {
			texturaActivaID = textureCreditosID;
		}

		if (!presionarOpcion && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			presionarOpcion = true;
			
			if( seleccionTextura < 2 )
				seleccionTextura++;

			switch (seleccionTextura)
			{
			case 0: texturaActivaID = textureInicio0ID; break;
			case 1: texturaActivaID = textureInicio1ID; break;
			case 2: texturaActivaID = textureInicio2ID; break;
			default: break;
			}
		} else if (!presionarOpcion && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			presionarOpcion = true;

			if (seleccionTextura > 0)
				seleccionTextura--;

			switch (seleccionTextura)
			{
			case 0: texturaActivaID = textureInicio0ID; break;
			case 1: texturaActivaID = textureInicio1ID; break;
			case 2: texturaActivaID = textureInicio2ID; break;
			default: break;
			}
		}  else  if (presionarOpcion  && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE ) {
			presionarOpcion = false;
		}

		
	}

	const float* axes = nullptr;
	const unsigned char* botones = nullptr;
	//para control xbox one
	int numeroAxes, numeroBotones;
	if (glfwJoystickPresent(GLFW_JOYSTICK_1 == GLFW_TRUE)) {
		axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &numeroAxes);
		botones = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &numeroBotones);
	}
	else {
		float auxAxes[2] = {};
		axes = auxAxes;
		unsigned char auxbotones[2] = {};
		botones = auxbotones;
	}

	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->mouseMoveCamera(offsetX, 0.0, deltaTime);
	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		camera->mouseMoveCamera(0.0, offsetY, deltaTime);
	offsetX = 0;
	offsetY = 0;

	// Seleccionar modelo
	if (enableCountSelected && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS){
		enableCountSelected = false;
		modelSelected++;
		if(modelSelected > 2)
			modelSelected = 0;
		if(modelSelected == 1)
			fileName = "../animaciones/animation_dart_joints.txt";
		if (modelSelected == 2)
			fileName = "../animaciones/animation_dart.txt";
		std::cout << "modelSelected:" << modelSelected << std::endl;
	}
	else if(glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
		enableCountSelected = true;

	//Terreneitor
	if (modelSelected == 1 && (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || axes[0] < -0.2f)){
		if (!(modelSelected == 1 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)) {
			modelMatrixTerrenator = glm::rotate(modelMatrixTerrenator, glm::radians(1.0f), glm::vec3(0, 1, 0));
		}
		else {
			modelMatrixTerrenator = glm::rotate(modelMatrixTerrenator, glm::radians(-1.0f), glm::vec3(0, 1, 0));
		}
		rotWheelsX += 0.05;
		rotWheelsY += 0.02;
		if (rotWheelsY > 0.44f)
			rotWheelsY = 0.44f;
		
	}
	if (modelSelected == 1 && (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || axes[0] > 0.2f)) {
		if (!(modelSelected == 1 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)) {
			modelMatrixTerrenator = glm::rotate(modelMatrixTerrenator, glm::radians(-1.0f), glm::vec3(0, 1, 0));
		}
		else {
			modelMatrixTerrenator = glm::rotate(modelMatrixTerrenator, glm::radians(1.0f), glm::vec3(0, 1, 0));
		}
		rotWheelsX -= 0.05;
		rotWheelsY -= 0.02;
		if (rotWheelsY < -0.44f)
			rotWheelsY = -0.44f;
	}if (modelSelected == 1 && (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || botones[0] == GLFW_PRESS)) {
		modelMatrixTerrenator = glm::translate(modelMatrixTerrenator, glm::vec3(0, 0, 0.22));
		if (!(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) &&!(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			&& abs(axes[0]) < 0.02f) {
			if (rotWheelsY < 0.0f)
				rotWheelsY += 0.02;
			if (rotWheelsY > 0.0f)
				rotWheelsY -= 0.02;

		}
		//reproduce sonido
		if (sourcesPlay[4] && iniciaPartida) {
			sourcesPlay[4] = false;
			sourcesPlay[5] = true;
			alSourceStop(source[5]);
			alSourcePlay(source[4]);
		}
	}

	else {
		if (!sourcesPlay[4]) {
			sourcesPlay[4] = true;
			alSourceStop(source[4]);
			
		}
		if(sourcesPlay[5] && iniciaPartida){
			alSourcePlay(source[5]);
			sourcesPlay[5] = false;
		}
	}

	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		modelMatrixTerrenator = glm::translate(modelMatrixTerrenator, glm::vec3(0, 0, -0.11));
		if (!(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) && !(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			&& abs(axes[0]) < 0.02f) {
			if (rotWheelsY < 0.0f)
				rotWheelsY += 0.02;
			if (rotWheelsY > 0.0f)
				rotWheelsY -= 0.02;
		}
	}

	if (modelSelected == 1 && (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS || botones[0] == GLFW_PRESS) &&
		glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		modelMatrixTerrenator = glm::translate(modelMatrixTerrenator, glm::vec3(0, 0, 2.22));
		if (!(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) && !(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			&& abs(axes[0]) < 0.02f) {
			if (rotWheelsY < 0.0f)
				rotWheelsY += 0.6;
			if (rotWheelsY > 0.0f)
				rotWheelsY -= 0.6;

		}
	}

	bool keySpaceStatus = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
	if(!isJump && keySpaceStatus){
		isJump = true;
		startTimeJump = currTime;
		tmv = 0;
	}

	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;

	glm::mat4 view;
	glm::vec3 axis;
	glm::vec3 target;
	float angleTarget;
	

	matrixModelRock = glm::translate(matrixModelRock, glm::vec3(-3.0, 0.0, 2.0));
	
	modelMatrixTerrenator = glm::rotate(modelMatrixTerrenator, glm::radians(-90.0f), glm::vec3(0, 1, 0));
	modelMatrixTerrenator = glm::translate(modelMatrixTerrenator, glm::vec3(20.0f, 2.0f, -10.0f));

	modelMatrixMeta = glm::rotate(modelMatrixMeta, glm::radians(-90.0f), glm::vec3(0, 1, 0));
	modelMatrixMeta = glm::translate(modelMatrixMeta, glm::vec3(20.5f, 9.0f, -43.0f));

	modelMatrixDart = glm::translate(modelMatrixDart, glm::vec3(3.0, 0.0, 20.0));

	modelMatrixFountain = glm::translate(modelMatrixFountain, glm::vec3(5.0, 0.0, -40.0));
	modelMatrixFountain[3][1] = terrain.getHeightTerrain(modelMatrixFountain[3][0] , modelMatrixFountain[3][2]) + 0.2;
	modelMatrixFountain = glm::scale(modelMatrixFountain, glm::vec3(10.0f, 10.0f, 10.0f));

	lastTime = TimeManager::Instance().GetTime();

	// Time for the particles animation
	currTimeParticlesAnimation = lastTime;
	lastTimeParticlesAnimation = lastTime;

	currTimeParticlesAnimationFire = lastTime;
	lastTimeParticlesAnimationFire = lastTime;

	glm::vec3 lightPos = glm::vec3(10.0, 10.0, 0.0);

	shadowBox = new ShadowBox( -lightPos, camera.get(), 15.0f, 0.1f, 45.0f );

	texturaActivaID = textureInicio0ID;

	while (psi) {
		currTime = TimeManager::Instance().GetTime();
		if(currTime - lastTime < 0.016666667){
			glfwPollEvents();
			continue;
		}
		lastTime = currTime;
		TimeManager::Instance().CalculateFrameRate(true);
		deltaTime = TimeManager::Instance().DeltaTime;
		psi = processInput(true);

		std::map<std::string, bool> collisionDetection;

		// Variables donde se guardan las matrices de cada articulacion por 1 frame
		std::vector<float> matrixDartJoints;
		std::vector<glm::mat4> matrixDart;

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
				(float) screenWidth / (float) screenHeight, 0.1f, 100.0f);

		/*if(modelSelected == 1){
			axis = glm::axis(glm::quat_cast(modelMatrixDart));
			angleTarget = glm::angle(glm::quat_cast(modelMatrixDart));
			target = modelMatrixDart[3];
		}*/
		if (modelSelected == 1) {
			axis = glm::axis(glm::quat_cast(modelMatrixTerrenator));
			angleTarget = glm::angle(glm::quat_cast(modelMatrixTerrenator));
			target = modelMatrixTerrenator[3];
		}

		if(std::isnan(angleTarget))
			angleTarget = 0.0;
		if(axis.y < 0)
			angleTarget = -angleTarget;
		if (modelSelected == 1) {
			angleTarget -= glm::radians(0.0f);
			camera->setDistanceFromTarget(16.0f);
		}
		camera->setCameraTarget(target);
		camera->setAngleTarget(angleTarget);
		camera->updateCamera();
		view = camera->getViewMatrix();

		// Matriz de proyección del shadow mapping
		glm::mat4 lightProjection = glm::mat4(1.0f), lightView = glm::mat4(1.0f);
		glm::mat4 lightSpaceMatrix;
		shadowBox->update( screenWidth, screenHeight );
		
		//float near_plane = 0.1f, far_plane = 20.0f;
		//lightProjection = glm::ortho(-25.0f, 25.0f, -25.0f, 25.0f, near_plane, far_plane);
		//lightView = glm::lookAt(lightPos, glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0));
		
		glm::vec3 boxPosition = shadowBox->getCenter();
		lightView = glm::lookAt( boxPosition, boxPosition + glm::normalize(-lightPos), glm::vec3(0.0f, 1.0f, 0.0f) );
		lightProjection[0][0] = 2.0f / shadowBox->getWidth();
		lightProjection[1][1] = 2.0f / shadowBox->getHeight();
		lightProjection[2][2] = -2.0f / shadowBox->getLength();
		lightProjection[3][3] = 1.0f;
		
		lightSpaceMatrix = lightProjection * lightView;
		shaderDepth.setMatrix4("lightSpaceMatrix", 1, false, glm::value_ptr(lightSpaceMatrix));



		// Settea la matriz de vista y projection al shader con solo color
		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con skybox
		shaderSkybox.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderSkybox.setMatrix4("view", 1, false,
				glm::value_ptr(glm::mat4(glm::mat3(view))));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderMulLighting.setMatrix4("projection", 1, false,
					glm::value_ptr(projection));
		shaderMulLighting.setMatrix4("view", 1, false,
				glm::value_ptr(view));
		shaderMulLighting.setMatrix4("lightSpaceMatrix", 1, false,
				glm::value_ptr(lightSpaceMatrix));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderTerrain.setMatrix4("projection", 1, false,
					glm::value_ptr(projection));
		shaderTerrain.setMatrix4("view", 1, false,
				glm::value_ptr(view));
		shaderTerrain.setMatrix4("lightSpaceMatrix", 1, false,
				glm::value_ptr(lightSpaceMatrix));
		// Settea la matriz de vista y projection al shader para el fountain
		shaderParticlesFountain.setMatrix4("projection", 1, false,
					glm::value_ptr(projection));
		shaderParticlesFountain.setMatrix4("view", 1, false,
				glm::value_ptr(view));
		// Settea la matriz de vista y projection al shader para el fuego
		shaderParticlesFire.setInt("Pass", 2);
		shaderParticlesFire.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shaderParticlesFire.setMatrix4("view", 1, false, glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader para la lluvia
		shaderParticlesLluvia.setInt("Pass", 2);
		shaderParticlesLluvia.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shaderParticlesLluvia.setMatrix4("view", 1, false, glm::value_ptr(view));

		/*******************************************
		 * Propiedades de neblina
		 *******************************************/
		shaderMulLighting.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.5, 0.5, 0.4)));
		shaderTerrain.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.5, 0.5, 0.4)));
		shaderSkybox.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.5, 0.5, 0.4)));

		/*******************************************
		 * Propiedades Luz direccional
		 *******************************************/
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.002, 0.002, 0.002)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));

		/*******************************************
		 * Propiedades Luz direccional Terrain
		 *******************************************/
		shaderTerrain.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderTerrain.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.002, 0.002, 0.002)));
		shaderTerrain.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderTerrain.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderTerrain.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));

		/*******************************************
		 * Propiedades SpotLights
		 *******************************************/

		glm::mat4 spotPositionTerrenator1 = modelMatrixTerrenator;
		glm::mat4 spotPositionTerrenator2 = modelMatrixTerrenator;
		spotPositionTerrenator1 = glm::translate(spotPositionTerrenator1, glm::vec3(0.7f, 0.6f, 1.0f));
		spotPositionTerrenator2 = glm::translate(spotPositionTerrenator2, glm::vec3(-0.7f, 0.6f, 1.0f));

		//shaderMulLighting.setInt("spotLightCount", 2);
		shaderMulLighting.setVectorFloat3("spotLights[1].light.ambient", glm::value_ptr(glm::vec3(0.2f, 0.16f, 0.1f)));
		shaderMulLighting.setVectorFloat3("spotLights[1].light.diffuse", glm::value_ptr(glm::vec3(0.4f, 0.32f, 0.2f)));
		shaderMulLighting.setVectorFloat3("spotLights[1].light.specular", glm::value_ptr(glm::vec3(0.6f, 0.58f, 0.03f)));
		shaderMulLighting.setVectorFloat3("spotLights[1].position", glm::value_ptr(spotPositionTerrenator1[3]));
		shaderMulLighting.setVectorFloat3("spotLights[1].direction", glm::value_ptr(modelMatrixTerrenator[2]));
		shaderMulLighting.setFloat("spotLights[1].constant", 1.0);
		shaderMulLighting.setFloat("spotLights[1].linear", 0.02);
		shaderMulLighting.setFloat("spotLights[1].quadratic", 0.02);
		shaderMulLighting.setFloat("spotLights[1].cutOff", cos(glm::radians(15.0f)));
		shaderMulLighting.setFloat("spotLights[1].outerCutOff", cos(glm::radians(15.0f)));

		//shaderTerrain.setInt("spotLightCount", 2);
		shaderTerrain.setVectorFloat3("spotLights[1].light.ambient", glm::value_ptr(glm::vec3(0.2f, 0.16f, 0.1f)));
		shaderTerrain.setVectorFloat3("spotLights[1].light.diffuse", glm::value_ptr(glm::vec3(0.4f, 0.32f, 0.2f)));
		shaderTerrain.setVectorFloat3("spotLights[1].light.specular", glm::value_ptr(glm::vec3(0.6f, 0.58f, 0.03f)));
		shaderTerrain.setVectorFloat3("spotLights[1].position", glm::value_ptr(spotPositionTerrenator1[3]));
		shaderTerrain.setVectorFloat3("spotLights[1].direction", glm::value_ptr(modelMatrixTerrenator[2]));
		shaderTerrain.setFloat("spotLights[1].constant", 1.0);
		shaderTerrain.setFloat("spotLights[1].linear", 0.02);
		shaderTerrain.setFloat("spotLights[1].quadratic", 0.01);
		shaderTerrain.setFloat("spotLights[1].cutOff", cos(glm::radians(12.5f)));
		shaderTerrain.setFloat("spotLights[1].outerCutOff", cos(glm::radians(15.0f)));

		//shaderMulLighting.setInt("spotLightCount", 2);
		shaderMulLighting.setVectorFloat3("spotLights[2].light.ambient", glm::value_ptr(glm::vec3(0.2f, 0.16f, 0.1f)));
		shaderMulLighting.setVectorFloat3("spotLights[2].light.diffuse", glm::value_ptr(glm::vec3(0.4f, 0.32f, 0.2f)));
		shaderMulLighting.setVectorFloat3("spotLights[2].light.specular", glm::value_ptr(glm::vec3(0.6f, 0.58f, 0.03f)));
		shaderMulLighting.setVectorFloat3("spotLights[2].position", glm::value_ptr(spotPositionTerrenator2[3]));
		shaderMulLighting.setVectorFloat3("spotLights[2].direction", glm::value_ptr(modelMatrixTerrenator[2]));
		shaderMulLighting.setFloat("spotLights[2].constant", 1.0);
		shaderMulLighting.setFloat("spotLights[2].linear", 0.02);
		shaderMulLighting.setFloat("spotLights[2].quadratic", 0.02);
		shaderMulLighting.setFloat("spotLights[2].cutOff", cos(glm::radians(15.0f)));
		shaderMulLighting.setFloat("spotLights[2].outerCutOff", cos(glm::radians(15.0f)));

		//shaderTerrain.setInt("spotLightCount", 2);
		shaderTerrain.setVectorFloat3("spotLights[2].light.ambient", glm::value_ptr(glm::vec3(0.2f, 0.16f, 0.1f)));
		shaderTerrain.setVectorFloat3("spotLights[2].light.diffuse", glm::value_ptr(glm::vec3(0.4f, 0.32f, 0.2f)));
		shaderTerrain.setVectorFloat3("spotLights[2].light.specular", glm::value_ptr(glm::vec3(0.6f, 0.58f, 0.03f)));
		shaderTerrain.setVectorFloat3("spotLights[2].position", glm::value_ptr(spotPositionTerrenator2[3]));
		shaderTerrain.setVectorFloat3("spotLights[2].direction", glm::value_ptr(modelMatrixTerrenator[2]));
		shaderTerrain.setFloat("spotLights[2].constant", 1.0);
		shaderTerrain.setFloat("spotLights[2].linear", 0.02);
		shaderTerrain.setFloat("spotLights[2].quadratic", 0.01);
		shaderTerrain.setFloat("spotLights[2].cutOff", cos(glm::radians(12.5f)));
		shaderTerrain.setFloat("spotLights[2].outerCutOff", cos(glm::radians(15.0f)));

		/*******************************************
		 * Propiedades PointLights
		 *******************************************/
		if (!iniciaPartida) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glViewport(0, 0, screenWidth, screenHeight);
			shaderTextura.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0f)));
			shaderTextura.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0f)));
			glActiveTexture(GL_TEXTURE0);
			glBindTexture( GL_TEXTURE_2D, texturaActivaID );
			boxIntro.render();
			glfwSwapBuffers( window ); // intercambiar el buffer trasero
			continue;
		}

		/*******************************************
		 * 1.- We render the depth buffer
		 *******************************************/
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// render scene from light's point of view
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		//glCullFace(GL_FRONT);
		prepareDepthScene();
		renderScene(false);
		//glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		/*******************************************
		 * Debug to view the texture view map
		 *******************************************/
		// reset viewport
		/*glViewport(0, 0, screenWidth, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// render Depth map to quad for visual debugging
		shaderViewDepth.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
		shaderViewDepth.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		boxViewDepth.setScale(glm::vec3(2.0, 2.0, 1.0));
		boxViewDepth.render();*/

		/*******************************************
		 * 2.- We render the normal objects
		 *******************************************/
		glViewport(0, 0, screenWidth, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		prepareScene();
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		shaderMulLighting.setInt("shadowMap", 10);
		shaderTerrain.setInt("shadowMap", 10);
		/*******************************************
		 * Skybox
		 *******************************************/
		GLint oldCullFaceMode;
		GLint oldDepthFuncMode;
		// deshabilita el modo del recorte de caras ocultas para ver las esfera desde adentro
		glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
		glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
		shaderSkybox.setFloat("skybox", 0);
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);
		glActiveTexture(GL_TEXTURE0);
		skyboxSphere.render();
		glCullFace(oldCullFaceMode);
		glDepthFunc(oldDepthFuncMode);
		renderScene();
		/*******************************************
		 * Debug to box light box
		 *******************************************/
		/*glm::vec3 front = glm::normalize(-lightPos);
		glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), front));
		glm::vec3 up = glm::normalize(glm::cross(front, right));
		glDisable(GL_CULL_FACE);
		glm::mat4 boxViewTransform = glm::mat4(1.0f);
		boxViewTransform = glm::translate(boxViewTransform, centerBox);
		boxViewTransform[0] = glm::vec4(right, 0.0);
		boxViewTransform[1] = glm::vec4(up, 0.0);
		boxViewTransform[2] = glm::vec4(front, 0.0);
		boxViewTransform = glm::scale(boxViewTransform, glm::vec3(shadowBox->getWidth(), shadowBox->getHeight(), shadowBox->getLength()));
		boxLightViewBox.enableWireMode();
		boxLightViewBox.render(boxViewTransform);
		glEnable(GL_CULL_FACE);*/

		/*******************************************
		 * Creacion de colliders
		 * IMPORTANT do this before interpolations
		 *******************************************/

		//Collider de barril 1
		glm::mat4 modelmatrixColliderBarrel = glm::mat4(modelMatrixBarrel);
		AbstractModel::OBB BarrelCollider;
		// Set the orientation of collider before doing the scale
		BarrelCollider.u = glm::quat_cast(modelMatrixBarrel);
		modelmatrixColliderBarrel = glm::scale(modelmatrixColliderBarrel,
			glm::vec3(1.0, 1.0, 1.0));
		modelmatrixColliderBarrel = glm::translate(							
			modelmatrixColliderBarrel, glm::vec3(modelDartLegoBody.getObb().c.x - 13.67,
				modelDartLegoBody.getObb().c.y + 2.3,
				modelDartLegoBody.getObb().c.z+23.04));
		BarrelCollider.c = glm::vec3(modelmatrixColliderBarrel[3]);
		BarrelCollider.e = modelBarrel.getObb().e * glm::vec3(1.0, 1.6, 0.65);
		addOrUpdateColliders(collidersOBB, "barrel", BarrelCollider, modelMatrixBarrel);

		//Collider de barril 2
		glm::mat4 modelmatrixColliderBarrel2 = glm::mat4(modelMatrixBarrel);
		AbstractModel::OBB BarrelCollider2;
		// Set the orientation of collider before doing the scale
		BarrelCollider2.u = glm::quat_cast(modelMatrixBarrel);
		modelmatrixColliderBarrel2 = glm::scale(modelmatrixColliderBarrel2,
			glm::vec3(1.0, 1.0, 1.0));
		modelmatrixColliderBarrel2 = glm::translate(
			modelmatrixColliderBarrel2, glm::vec3(modelDartLegoBody.getObb().c.x - 19.14,
				modelDartLegoBody.getObb().c.y + 2.3,
				modelDartLegoBody.getObb().c.z -19.14));
		BarrelCollider2.c = glm::vec3(modelmatrixColliderBarrel2[3]);
		BarrelCollider2.e = modelBarrel.getObb().e * glm::vec3(1.0, 1.6, 0.65);
		addOrUpdateColliders(collidersOBB, "barrel2", BarrelCollider2, modelMatrixBarrel);

		//Collider de barril 3
		glm::mat4 modelmatrixColliderBarrel3 = glm::mat4(modelMatrixBarrel);
		AbstractModel::OBB BarrelCollider3;
		// Set the orientation of collider before doing the scale
		BarrelCollider3.u = glm::quat_cast(modelMatrixBarrel);
		modelmatrixColliderBarrel3 = glm::scale(modelmatrixColliderBarrel3,
			glm::vec3(1.0, 1.0, 1.0));
		modelmatrixColliderBarrel3 = glm::translate(
			modelmatrixColliderBarrel3, glm::vec3(modelDartLegoBody.getObb().c.x + 54.68,
				modelDartLegoBody.getObb().c.y + 2.3,
				modelDartLegoBody.getObb().c.z - 17.57));
		BarrelCollider3.c = glm::vec3(modelmatrixColliderBarrel3[3]);
		BarrelCollider3.e = modelBarrel.getObb().e * glm::vec3(1.0, 1.6, 0.65);
		addOrUpdateColliders(collidersOBB, "barrel3", BarrelCollider3, modelMatrixBarrel);

		//Collider de barril 4
		glm::mat4 modelmatrixColliderBarrel4 = glm::mat4(modelMatrixBarrel);
		AbstractModel::OBB BarrelCollider4;
		// Set the orientation of collider before doing the scale
		BarrelCollider4.u = glm::quat_cast(modelMatrixBarrel);
		modelmatrixColliderBarrel4 = glm::scale(modelmatrixColliderBarrel4,
			glm::vec3(1.0, 1.0, 1.0));
		modelmatrixColliderBarrel4 = glm::translate(
			modelmatrixColliderBarrel4, glm::vec3(modelDartLegoBody.getObb().c.x - 3.125,
				modelDartLegoBody.getObb().c.y + 2.3,
				modelDartLegoBody.getObb().c.z - 64.45));
		BarrelCollider4.c = glm::vec3(modelmatrixColliderBarrel4[3]);
		BarrelCollider4.e = modelBarrel.getObb().e * glm::vec3(1.0, 1.6, 0.65);
		addOrUpdateColliders(collidersOBB, "barrel4", BarrelCollider4, modelMatrixBarrel);

		//Collider de barril 5
		glm::mat4 modelmatrixColliderBarrel5 = glm::mat4(modelMatrixBarrel);
		AbstractModel::OBB BarrelCollider5;
		// Set the orientation of collider before doing the scale
		BarrelCollider5.u = glm::quat_cast(modelMatrixBarrel);
		modelmatrixColliderBarrel5 = glm::scale(modelmatrixColliderBarrel5,
			glm::vec3(1.0, 1.0, 1.0));
		modelmatrixColliderBarrel5 = glm::translate(
			modelmatrixColliderBarrel5, glm::vec3(modelDartLegoBody.getObb().c.x - 59.76,
				modelDartLegoBody.getObb().c.y + 2.3,
				modelDartLegoBody.getObb().c.z + 57.42));
		BarrelCollider5.c = glm::vec3(modelmatrixColliderBarrel5[3]);
		BarrelCollider5.e = modelBarrel.getObb().e * glm::vec3(1.0, 1.6, 0.65);
		addOrUpdateColliders(collidersOBB, "barrel5", BarrelCollider5, modelMatrixBarrel);

		//Collider de barril 6
		glm::mat4 modelmatrixColliderBarrel6 = glm::mat4(modelMatrixBarrel);
		AbstractModel::OBB BarrelCollider6;
		// Set the orientation of collider before doing the scale
		BarrelCollider6.u = glm::quat_cast(modelMatrixBarrel);
		modelmatrixColliderBarrel6 = glm::scale(modelmatrixColliderBarrel6,
			glm::vec3(1.0, 1.0, 1.0));
		modelmatrixColliderBarrel6 = glm::translate(
			modelmatrixColliderBarrel6, glm::vec3(modelDartLegoBody.getObb().c.x + 54.68,
				modelDartLegoBody.getObb().c.y + 2.3,
				modelDartLegoBody.getObb().c.z + 63.67));
		BarrelCollider6.c = glm::vec3(modelmatrixColliderBarrel6[3]);
		BarrelCollider6.e = modelBarrel.getObb().e * glm::vec3(1.0, 1.6, 0.65);
		addOrUpdateColliders(collidersOBB, "barrel6", BarrelCollider6, modelMatrixBarrel);

		//Collider de barril 7
		glm::mat4 modelmatrixColliderBarrel7 = glm::mat4(modelMatrixBarrel);
		AbstractModel::OBB BarrelCollider7;
		// Set the orientation of collider before doing the scale
		BarrelCollider7.u = glm::quat_cast(modelMatrixBarrel);
		modelmatrixColliderBarrel7 = glm::scale(modelmatrixColliderBarrel7,
			glm::vec3(1.0, 1.0, 1.0));
		modelmatrixColliderBarrel7 = glm::translate(
			modelmatrixColliderBarrel7, glm::vec3(modelDartLegoBody.getObb().c.x + 65.23,
				modelDartLegoBody.getObb().c.y + 2.3,
				modelDartLegoBody.getObb().c.z + 76.17));
		BarrelCollider7.c = glm::vec3(modelmatrixColliderBarrel7[3]);
		BarrelCollider7.e = modelBarrel.getObb().e * glm::vec3(1.0, 1.6, 0.65);
		addOrUpdateColliders(collidersOBB, "barrel7", BarrelCollider7, modelMatrixBarrel);

		//Collider de rock2 1
		glm::mat4 modelmatrixColliderRock2 = glm::mat4(modelMatrixRock2);
		AbstractModel::OBB Rock2Collider;
		// Set the orientation of collider before doing the scale
		Rock2Collider.u = glm::quat_cast(modelMatrixRock2);
		modelmatrixColliderRock2 = glm::scale(modelmatrixColliderRock2,
			glm::vec3(1.0, 1.0, 1.0));
		modelmatrixColliderRock2 = glm::translate(
			modelmatrixColliderRock2, glm::vec3(modelDartLegoBody.getObb().c.x + 0.78,
				modelDartLegoBody.getObb().c.y+3.0,
				modelDartLegoBody.getObb().c.z+17.57));
		Rock2Collider.c = glm::vec3(modelmatrixColliderRock2[3]);
		Rock2Collider.e = modelRock2.getObb().e * glm::vec3(0.07, 0.08, 0.08);
		addOrUpdateColliders(collidersOBB, "rock2", Rock2Collider, modelMatrixRock2);

		//Collider de rock2 2
		glm::mat4 modelmatrixColliderRock22 = glm::mat4(modelMatrixRock2);
		AbstractModel::OBB Rock2Collider2;
		// Set the orientation of collider before doing the scale
		Rock2Collider2.u = glm::quat_cast(modelMatrixRock2);
		modelmatrixColliderRock22 = glm::scale(modelmatrixColliderRock22,
			glm::vec3(1.0, 1.0, 1.0));
		modelmatrixColliderRock22 = glm::translate(
			modelmatrixColliderRock22, glm::vec3(modelDartLegoBody.getObb().c.x + 22.26,
				modelDartLegoBody.getObb().c.y + 3.0,
				modelDartLegoBody.getObb().c.z -14.45));
		Rock2Collider2.c = glm::vec3(modelmatrixColliderRock22[3]);
		Rock2Collider2.e = modelRock2.getObb().e * glm::vec3(0.07, 0.08, 0.08);
		addOrUpdateColliders(collidersOBB, "rock22", Rock2Collider2, modelMatrixRock2);

		//Collider de rock2 3
		glm::mat4 modelmatrixColliderRock23 = glm::mat4(modelMatrixRock2);
		AbstractModel::OBB Rock2Collider3;
		// Set the orientation of collider before doing the scale
		Rock2Collider3.u = glm::quat_cast(modelMatrixRock2);
		modelmatrixColliderRock23 = glm::scale(modelmatrixColliderRock23,
			glm::vec3(1.0, 1.0, 1.0));
		modelmatrixColliderRock23 = glm::translate(
			modelmatrixColliderRock23, glm::vec3(modelDartLegoBody.getObb().c.x - 64.01,
				modelDartLegoBody.getObb().c.y + 3.0,
				modelDartLegoBody.getObb().c.z + 17.18));
		Rock2Collider3.c = glm::vec3(modelmatrixColliderRock23[3]);
		Rock2Collider3.e = modelRock2.getObb().e * glm::vec3(0.07, 0.08, 0.08);
		addOrUpdateColliders(collidersOBB, "rock23", Rock2Collider3, modelMatrixRock2);

		//Collider de rock2 4
		glm::mat4 modelmatrixColliderRock24 = glm::mat4(modelMatrixRock2);
		AbstractModel::OBB Rock2Collider4;
		// Set the orientation of collider before doing the scale
		Rock2Collider4.u = glm::quat_cast(modelMatrixRock2);
		modelmatrixColliderRock24 = glm::scale(modelmatrixColliderRock24,
			glm::vec3(1.0, 1.0, 1.0));
		modelmatrixColliderRock24 = glm::translate(
			modelmatrixColliderRock24, glm::vec3(modelDartLegoBody.getObb().c.x + 70.43,
				modelDartLegoBody.getObb().c.y + 3.0,
				modelDartLegoBody.getObb().c.z + 26.95));
		Rock2Collider4.c = glm::vec3(modelmatrixColliderRock24[3]);
		Rock2Collider4.e = modelRock2.getObb().e * glm::vec3(0.07, 0.08, 0.08);
		addOrUpdateColliders(collidersOBB, "rock24", Rock2Collider4, modelMatrixRock2);

		//Collider del la rock
		AbstractModel::SBB rockCollider;
		glm::mat4 modelMatrixColliderRock= glm::mat4(matrixModelRock);
		modelMatrixColliderRock = glm::scale(modelMatrixColliderRock, glm::vec3(1.0, 1.0, 1.0));
		modelMatrixColliderRock = glm::translate(modelMatrixColliderRock, modelRock.getSbb().c);
		rockCollider.c = glm::vec3(modelMatrixColliderRock[3]);
		rockCollider.ratio = modelRock.getSbb().ratio * 1.0;
		addOrUpdateColliders(collidersSBB, "rock", rockCollider, matrixModelRock);

		//Collider de llanta 1
		glm::mat4 modelmatrixColliderWheels = glm::mat4(modelMatrixThreeWheels);
		AbstractModel::OBB WheelsCollider;
		// Set the orientation of collider before doing the scale
		WheelsCollider.u = glm::quat_cast(modelMatrixThreeWheels);
		modelmatrixColliderWheels = glm::scale(modelmatrixColliderWheels,
			glm::vec3(1.0, 1.0, 1.0));
		modelmatrixColliderWheels = glm::translate(
			modelmatrixColliderWheels, glm::vec3(modelDartLegoBody.getObb().c.x - 16.01,
				modelDartLegoBody.getObb().c.y + 2.0,
				modelDartLegoBody.getObb().c.z + 1.95));
		WheelsCollider.c = glm::vec3(modelmatrixColliderWheels[3]);
		WheelsCollider.e = modelThreeWheels.getObb().e * glm::vec3(0.09, 0.03, 0.07);
		addOrUpdateColliders(collidersOBB, "wheels", WheelsCollider, modelMatrixThreeWheels);

		//Collider de llanta 2
		glm::mat4 modelmatrixColliderWheels2 = glm::mat4(modelMatrixThreeWheels);
		AbstractModel::OBB WheelsCollider2;
		// Set the orientation of collider before doing the scale
		WheelsCollider2.u = glm::quat_cast(modelMatrixThreeWheels);
		modelmatrixColliderWheels2 = glm::scale(modelmatrixColliderWheels2,
			glm::vec3(1.0, 1.0, 1.0));
		modelmatrixColliderWheels2 = glm::translate(
			modelmatrixColliderWheels2, glm::vec3(modelDartLegoBody.getObb().c.x + 65.23,
				modelDartLegoBody.getObb().c.y + 2.0,
				modelDartLegoBody.getObb().c.z - 41.01));
		WheelsCollider2.c = glm::vec3(modelmatrixColliderWheels2[3]);
		WheelsCollider2.e = modelThreeWheels.getObb().e * glm::vec3(0.09, 0.03, 0.07);
		addOrUpdateColliders(collidersOBB, "wheels2", WheelsCollider2, modelMatrixThreeWheels);

		//Collider de llanta 3
		glm::mat4 modelmatrixColliderWheels3 = glm::mat4(modelMatrixThreeWheels);
		AbstractModel::OBB WheelsCollider3;
		// Set the orientation of collider before doing the scale
		WheelsCollider3.u = glm::quat_cast(modelMatrixThreeWheels);
		modelmatrixColliderWheels3 = glm::scale(modelmatrixColliderWheels3,
			glm::vec3(1.0, 1.0, 1.0));
		modelmatrixColliderWheels3 = glm::translate(
			modelmatrixColliderWheels3, glm::vec3(modelDartLegoBody.getObb().c.x - 30.46,
				modelDartLegoBody.getObb().c.y + 3.0,
				modelDartLegoBody.getObb().c.z - 70.11));
		WheelsCollider3.c = glm::vec3(modelmatrixColliderWheels3[3]);
		WheelsCollider3.e = modelThreeWheels.getObb().e * glm::vec3(0.09, 0.03, 0.07);
		addOrUpdateColliders(collidersOBB, "wheels3", WheelsCollider3, modelMatrixThreeWheels);

		//Collider de llanta 4
		glm::mat4 modelmatrixColliderWheels4 = glm::mat4(modelMatrixThreeWheels);
		AbstractModel::OBB WheelsCollider4;
		// Set the orientation of collider before doing the scale
		WheelsCollider4.u = glm::quat_cast(modelMatrixThreeWheels);
		modelmatrixColliderWheels4 = glm::scale(modelmatrixColliderWheels4,
			glm::vec3(1.0, 1.0, 1.0));
		modelmatrixColliderWheels4 = glm::translate(
			modelmatrixColliderWheels4, glm::vec3(modelDartLegoBody.getObb().c.x - 60.54,
				modelDartLegoBody.getObb().c.y + 2.0,
				modelDartLegoBody.getObb().c.z - 28.90));
		WheelsCollider4.c = glm::vec3(modelmatrixColliderWheels4[3]);
		WheelsCollider4.e = modelThreeWheels.getObb().e * glm::vec3(0.09, 0.03, 0.07);
		addOrUpdateColliders(collidersOBB, "wheels4", WheelsCollider4, modelMatrixThreeWheels);

		//Collider de llanta 5
		glm::mat4 modelmatrixColliderWheels5 = glm::mat4(modelMatrixThreeWheels);
		AbstractModel::OBB WheelsCollider5;
		// Set the orientation of collider before doing the scale
		WheelsCollider5.u = glm::quat_cast(modelMatrixThreeWheels);
		modelmatrixColliderWheels5 = glm::scale(modelmatrixColliderWheels5,
			glm::vec3(1.0, 1.0, 1.0));
		modelmatrixColliderWheels5 = glm::translate(
			modelmatrixColliderWheels5, glm::vec3(modelDartLegoBody.getObb().c.x + 2.43,
				modelDartLegoBody.getObb().c.y + 2.5,
				modelDartLegoBody.getObb().c.z + 67.57));
		WheelsCollider5.c = glm::vec3(modelmatrixColliderWheels5[3]);
		WheelsCollider5.e = modelThreeWheels.getObb().e * glm::vec3(0.09, 0.03, 0.07);
		addOrUpdateColliders(collidersOBB, "wheels5", WheelsCollider5, modelMatrixThreeWheels);

		// Guardrail colliders
		for (int i = 0; i < guardrailPosition1.size(); i++) {
			AbstractModel::OBB guardrailCollider;
			glm::mat4 modelMatrixColliderGuardrail = glm::mat4(1.0);
			modelMatrixColliderGuardrail = glm::translate(modelMatrixColliderGuardrail, guardrailPosition1[i]);
			modelMatrixColliderGuardrail = glm::rotate(modelMatrixColliderGuardrail, glm::radians(guardrailOrientation1[i]),
				glm::vec3(0, 1, 0));
			addOrUpdateColliders(collidersOBB, "guardrail1-" + std::to_string(i), guardrailCollider, modelMatrixColliderGuardrail);
			// Set the orientation of collider before doing the scale
			guardrailCollider.u = glm::quat_cast(modelMatrixColliderGuardrail);
			modelMatrixColliderGuardrail = glm::scale(modelMatrixColliderGuardrail, glm::vec3(0.5, 0.5, 0.5));
			modelMatrixColliderGuardrail = glm::translate(modelMatrixColliderGuardrail, modelGuardrail1.getObb().c);
			guardrailCollider.c = glm::vec3(modelMatrixColliderGuardrail[3]);
			guardrailCollider.e = modelGuardrail1.getObb().e * glm::vec3(0.5, 0.5, 0.5);
			std::get<0>(collidersOBB.find("guardrail1-" + std::to_string(i))->second) = guardrailCollider;
		}

		// Collider de Terrenator
		AbstractModel::OBB TerrenatorCollider;
		glm::mat4 modelmatrixColliderTerrenator = glm::mat4(modelMatrixTerrenator);
		modelmatrixColliderTerrenator = glm::rotate(modelmatrixColliderTerrenator,
			glm::radians(0.0f), glm::vec3(1, 0, 0));
		// Set the orientation of collider before doing the scale
		TerrenatorCollider.u = glm::quat_cast(modelmatrixColliderTerrenator);
		modelmatrixColliderTerrenator = glm::scale(modelmatrixColliderTerrenator, glm::vec3(1.021, 1.021, 1.021));
		modelmatrixColliderTerrenator = glm::translate(modelmatrixColliderTerrenator,
			glm::vec3(modelTerrenator.getObb().c.x,
					modelTerrenator.getObb().c.y - 0.8f,
					modelTerrenator.getObb().c.z));
		TerrenatorCollider.e = modelTerrenator.getObb().e * glm::vec3(0.6, 0.5, 0.5);
		TerrenatorCollider.c = glm::vec3(modelmatrixColliderTerrenator[3]);
		addOrUpdateColliders(collidersOBB, "Terrenator", TerrenatorCollider, modelMatrixTerrenator);

		// Collider de Meta
		AbstractModel::OBB MetaCollider;
		glm::mat4 modelmatrixColliderMeta = glm::mat4(modelMatrixMeta);
		modelmatrixColliderMeta = glm::rotate(modelmatrixColliderMeta,
			glm::radians(0.0f), glm::vec3(1, 0, 0));
		// Set the orientation of collider before doing the scale
		MetaCollider.u = glm::quat_cast(modelmatrixColliderMeta);
		modelmatrixColliderMeta = glm::scale(modelmatrixColliderMeta, glm::vec3(1.021, 1.021, 1.021));
		modelmatrixColliderMeta = glm::translate(modelmatrixColliderMeta,
			glm::vec3(modelMeta.getObb().c.x,
				modelMeta.getObb().c.y - 0.8f,
				modelMeta.getObb().c.z));
		MetaCollider.e = modelMeta.getObb().e * glm::vec3(2.0, 2.0, 2.0);
		MetaCollider.c = glm::vec3(modelmatrixColliderMeta[3]);
		addOrUpdateColliders(collidersOBB, "Meta", MetaCollider, modelMatrixMeta);

		/*******************************************
		 * Render de colliders
		 *******************************************/
		/*for (std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it =
				collidersOBB.begin(); it != collidersOBB.end(); it++) {
			glm::mat4 matrixCollider = glm::mat4(1.0);
			matrixCollider = glm::translate(matrixCollider, std::get<0>(it->second).c);
			matrixCollider = matrixCollider * glm::mat4(std::get<0>(it->second).u);
			matrixCollider = glm::scale(matrixCollider, std::get<0>(it->second).e * 2.0f);
			boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
			boxCollider.enableWireMode();
			boxCollider.render(matrixCollider);
		}

		for (std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
				collidersSBB.begin(); it != collidersSBB.end(); it++) {
			glm::mat4 matrixCollider = glm::mat4(1.0);
			matrixCollider = glm::translate(matrixCollider, std::get<0>(it->second).c);
			matrixCollider = glm::scale(matrixCollider, glm::vec3(std::get<0>(it->second).ratio * 2.0f));
			sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
			sphereCollider.enableWireMode();
			sphereCollider.render(matrixCollider);
		}*/

		/*******************************************
		 * Test Colisions
		 *******************************************/
		for (std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it =
				collidersOBB.begin(); it != collidersOBB.end(); it++) {
			bool isCollision = false;
			for (std::map<std::string,
					std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
					collidersOBB.begin(); jt != collidersOBB.end(); jt++) {
				if (it != jt && it->first.find("guardrail") != 0
						&& testOBBOBB(std::get<0>(it->second),
								std::get<0>(jt->second))) {
					std::cout << "Colision " << it->first << " with "
							<< jt->first << std::endl;
					isCollision = true;
					if ((it->first.compare("Terrenator") == 0) && (jt->first.compare("Meta") == 0)) {
						ganaste = true;
						vidas = 4;
					}
				}
				
				
			}
			if (it->first.compare("Terrenator") == 0 && isCollision == false) {
				sourcesPlay[6] = true;
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		for (std::map<std::string,
				std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
				collidersSBB.begin(); it != collidersSBB.end(); it++) {
			bool isCollision = false;
			for (std::map<std::string,
					std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator jt =
					collidersSBB.begin(); jt != collidersSBB.end(); jt++) {
				if (it != jt
						&& testSphereSphereIntersection(std::get<0>(it->second),
								std::get<0>(jt->second))) {
					std::cout << "Colision " << it->first << " with "
							<< jt->first << std::endl;
					isCollision = true;
				}
			}
			
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		for (std::map<std::string,
				std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
				collidersSBB.begin(); it != collidersSBB.end(); it++) {
			bool isCollision = false;
			std::map<std::string,
					std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
					collidersOBB.begin();
			for (; jt != collidersOBB.end(); jt++) {
				if (testSphereOBox(std::get<0>(it->second),
								std::get<0>(jt->second))) {
					std::cout << "Colision " << it->first << " with "
							<< jt->first << std::endl;
					isCollision = true;
					addOrUpdateCollisionDetection(collisionDetection, jt->first, isCollision);
				}
			}
			
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}
		
		std::map<std::string, bool>::iterator colIt;
		for (colIt = collisionDetection.begin(); colIt != collisionDetection.end();
				colIt++) {
			std::map<std::string,
					std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
					collidersSBB.find(colIt->first);
			std::map<std::string,
					std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
					collidersOBB.find(colIt->first);
			if (it != collidersSBB.end()) {
				if (!colIt->second)
					addOrUpdateColliders(collidersSBB, it->first);
			}
			if (jt != collidersOBB.end()) {
				if (!colIt->second) {
					addOrUpdateColliders(collidersOBB, jt->first);
				}
				else {
					if ((jt->first.compare("Terrenator") == 0) && iniciaPartida) {
						
						
						if (sourcesPlay[6] && !ganaste) {
							sourcesPlay[6] = false;
							alSourcePlay(source[6]);
							vidas = vidas - 1;
						}

						modelMatrixTerrenator = std::get<1>(jt->second);
						
						
						
						
					}
				}
			}
		}

		/*******************************************
		 * Interpolation key frames with disconect objects
		 *******************************************/
		

		animationIndex = 1;

		/*******************************************
		 * State machines
		 *******************************************/


		glEnable(GL_BLEND);
		switch (vidas) {
			case 4:
				textLifeRender->render("                              GANASTE", 0.0, 0.0, 30.0, 1.0, 0.0, 0.0);
				break;
			case 3:
				textLifeRender->render("                              Vidas: 3", 0.0, 0.0, 30.0, 1.0, 0.0, 0.0);
				break;
			case 2:
				textLifeRender->render("                              Vidas: 2", 0.0, 0.0, 30.0, 1.0, 0.0, 0.0);
				break;
			case 1:
				textLifeRender->render("                              Vidas: 1", 0.0, 0.0, 30.0, 1.0, 0.0, 0.0);
				break;
			default:
				textLifeRender->render("                              PERDISTE", 0.0, 0.0, 30.0, 1.0, 0.0, 0.0);
				break;
		}
		glDisable(GL_BLEND);
		glfwSwapBuffers(window);


		/****************************+
		 * Open AL sound data
		 */
		source0Pos[0] = modelMatrixFountain[3].x;
		source0Pos[1] = modelMatrixFountain[3].y;
		source0Pos[2] = modelMatrixFountain[3].z;
		alSourcefv(source[0], AL_POSITION, source0Pos);

		source2Pos[0] = modelMatrixDart[3].x;
		source2Pos[1] = modelMatrixDart[3].y;
		source2Pos[2] = modelMatrixDart[3].z;
		alSourcefv(source[2], AL_POSITION, source2Pos);

		source3Pos[0] = camera->getPosition().x;
		source3Pos[1] = camera->getPosition().y;
		source3Pos[2] = camera->getPosition().z;
		alSourcefv(source[3], AL_POSITION, source3Pos);

		source4Pos[0] = modelMatrixTerrenator[3].x;
		source4Pos[1] = modelMatrixTerrenator[3].y;
		source4Pos[2] = modelMatrixTerrenator[3].z;
		alSourcefv(source[4], AL_POSITION, source4Pos);

		source5Pos[0] = modelMatrixTerrenator[3].x;
		source5Pos[1] = modelMatrixTerrenator[3].y;
		source5Pos[2] = modelMatrixTerrenator[3].z;
		alSourcefv(source[5], AL_POSITION, source5Pos);

		source6Pos[0] = modelMatrixTerrenator[3].x;
		source6Pos[1] = modelMatrixTerrenator[3].y;
		source6Pos[2] = modelMatrixTerrenator[3].z;
		alSourcefv(source[5], AL_POSITION, source6Pos);

		// Listener for the Thris person camera
		/*listenerPos[0] = modelMatrixMayow[3].x;
		listenerPos[1] = modelMatrixMayow[3].y;
		listenerPos[2] = modelMatrixMayow[3].z;
		alListenerfv(AL_POSITION, listenerPos);

		glm::vec3 upModel = glm::normalize(modelMatrixMayow[1]);
		glm::vec3 frontModel = glm::normalize(modelMatrixMayow[2]);

		listenerOri[0] = frontModel.x;
		listenerOri[1] = frontModel.y;
		listenerOri[2] = frontModel.z;
		listenerOri[3] = upModel.x;
		listenerOri[4] = upModel.y;
		listenerOri[5] = upModel.z;*/

		// Listener for the First person camera
		listenerPos[0] = camera->getPosition().x;
		listenerPos[1] = camera->getPosition().y;
		listenerPos[2] = camera->getPosition().z;
		alListenerfv(AL_POSITION, listenerPos);
		listenerOri[0] = camera->getFront().x;
		listenerOri[1] = camera->getFront().y;
		listenerOri[2] = camera->getFront().z;
		listenerOri[3] = camera->getUp().x;
		listenerOri[4] = camera->getUp().y;
		listenerOri[5] = camera->getUp().z;
		alListenerfv(AL_ORIENTATION, listenerOri);

		for(unsigned int i = 0; i < 3; i++){
			if(sourcesPlay[i]){
				sourcesPlay[i] = false;
				alSourcePlay(source[i]);
				
			}
		}

	}
}

void prepareScene(){

	skyboxSphere.setShader(&shaderSkybox);

	modelRock.setShader(&shaderMulLighting);

	terrain.setShader(&shaderTerrain);

	// Terrenator
	modelTerrenator.setShader(&shaderMulLighting);
	modelTerrenatorFrontLeftWheel.setShader(&shaderMulLighting);
	modelTerrenatorFrontRightWheel.setShader(&shaderMulLighting);
	modelTerrenatorRearLeftWheel.setShader(&shaderMulLighting);
	modelTerrenatorRearRightWheel.setShader(&shaderMulLighting);
	modelTerrenatorVentana.setShader(&shaderMulLighting);

	//finish line
	modelMeta.setShader(&shaderMulLighting);

	// Dart Lego
	modelDartLegoBody.setShader(&shaderMulLighting);

	//Guardrails
	modelGuardrail0.setShader(&shaderMulLighting);
	modelGuardrail1.setShader(&shaderMulLighting);

	//Grass
	modelGrass.setShader(&shaderMulLighting);

	//Barrel
	modelBarrel.setShader(&shaderMulLighting);

	//Rock
	modelRock2.setShader(&shaderMulLighting);

	//Wheel
	modelThreeWheels.setShader(&shaderMulLighting);
}

void prepareDepthScene(){

	skyboxSphere.setShader(&shaderDepth);

	modelRock.setShader(&shaderDepth);

	terrain.setShader(&shaderDepth);

	// Terrenator
	modelTerrenator.setShader(&shaderDepth);
	modelTerrenatorFrontLeftWheel.setShader(&shaderDepth);
	modelTerrenatorFrontRightWheel.setShader(&shaderDepth);
	modelTerrenatorRearLeftWheel.setShader(&shaderDepth);
	modelTerrenatorRearRightWheel.setShader(&shaderDepth);
	modelTerrenatorVentana.setShader(&shaderDepth);

	// finish line
	modelMeta.setShader(&shaderDepth);

	// Dart Lego
	modelDartLegoBody.setShader(&shaderDepth);

	//Guardrials
	modelGuardrail0.setShader(&shaderDepth);
	modelGuardrail1.setShader(&shaderDepth);

	//Grass
	modelGrass.setShader(&shaderDepth);

	//Barrel
	modelBarrel.setShader(&shaderDepth);

	//Rock
	modelRock2.setShader(&shaderDepth);

	//Wheel
	modelThreeWheels.setShader(&shaderDepth);
}

void renderScene(bool renderParticles){
	/*******************************************
	 * Terrain Cesped
	 *******************************************/
	glm::mat4 modelCesped = glm::mat4(1.0);
	modelCesped = glm::translate(modelCesped, glm::vec3(0.0, 0.0, 0.0));
	modelCesped = glm::scale(modelCesped, glm::vec3(200.0, 0.001, 200.0));
	// Se activa la textura del background
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBackgroundID);
	shaderTerrain.setInt("backgroundTexture", 0);
	// Se activa la textura de tierra
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
	shaderTerrain.setInt("rTexture", 1);
	// Se activa la textura de hierba
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textureAsfaltoID);
	shaderTerrain.setInt("gTexture", 2);
	// Se activa la textura del camino
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
	shaderTerrain.setInt("bTexture", 3);
	// Se activa la textura del blend map
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID);
	shaderTerrain.setInt("blendMapTexture", 4);
	shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(40, 40)));
	terrain.render();
	shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
	glBindTexture(GL_TEXTURE_2D, 0);

	/*******************************************
	 * Custom objects obj
	 *******************************************/
	//Rock render
	matrixModelRock[3][1] = terrain.getHeightTerrain(matrixModelRock[3][0], matrixModelRock[3][2]);
	modelRock.render(matrixModelRock);
	// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
	glActiveTexture(GL_TEXTURE0);

	//Render the guarrails
	for (int i = 0; i < guardrailPosition.size(); i++) {
		guardrailPosition[i].y = terrain.getHeightTerrain(guardrailPosition[i].x, guardrailPosition[i].z);
		modelGuardrail0.setPosition(guardrailPosition[i]);
		modelGuardrail0.setScale(glm::vec3(0.5, 0.5, 0.5));
		modelGuardrail0.setOrientation(glm::vec3(0, guardrailOrientation[i], 0));
		modelGuardrail0.render();
	}

	//Render the guarrails
	for (int i = 0; i < guardrailPosition1.size(); i++) {
		guardrailPosition1[i].y = terrain.getHeightTerrain(guardrailPosition1[i].x, guardrailPosition1[i].z);
		modelGuardrail1.setPosition(guardrailPosition1[i]);
		modelGuardrail1.setScale(glm::vec3(0.5, 0.5, 0.5));
		modelGuardrail1.setOrientation(glm::vec3(0, guardrailOrientation1[i], 0));
		modelGuardrail1.render();
	}

	// Grass
	glDisable(GL_CULL_FACE);
	glm::vec3 grassPosition = glm::vec3(0.0, 0.0, 0.0);
	grassPosition.y = terrain.getHeightTerrain(grassPosition.x, grassPosition.z);
	modelGrass.setPosition(grassPosition);
	modelGrass.render();
	glEnable(GL_CULL_FACE);

	//// Fountain
	//glDisable(GL_CULL_FACE);
	//modelFountain.render(modelMatrixFountain);
	//glEnable(GL_CULL_FACE);


	// Terrenator
	glDisable(GL_CULL_FACE);
	glm::vec3 ejeyTerrenator = glm::normalize(terrain.getNormalTerrain(modelMatrixTerrenator[3][0],
		modelMatrixTerrenator[3][2]));
	glm::vec3 ejexTerrenator = glm::normalize(glm::vec3(modelMatrixTerrenator[0]));
	glm::vec3 ejezTerrenator = glm::normalize(glm::cross(ejexTerrenator, ejeyTerrenator)); //es perpendicular a Y
	ejexTerrenator = glm::normalize(glm::cross(ejeyTerrenator, ejezTerrenator));

	modelMatrixTerrenator[0] = glm::vec4(ejexTerrenator, 0.0f);
	modelMatrixTerrenator[1] = glm::vec4(ejeyTerrenator, 0.0f);
	modelMatrixTerrenator[2] = glm::vec4(ejezTerrenator, 0.0f);

	modelMatrixTerrenator[3][1] += -GRAVITY*(currTime - inicioCaida)* (currTime - inicioCaida);
	if (modelMatrixTerrenator[3][1] < terrain.getHeightTerrain(modelMatrixTerrenator[3][0], modelMatrixTerrenator[3][2])) {
		modelMatrixTerrenator[3][1] = terrain.getHeightTerrain(modelMatrixTerrenator[3][0], modelMatrixTerrenator[3][2]);
		inicioCaida = currTime;
	}
	
	//modelMatrixTerrenator[3][1] = terrain.getHeightTerrain(modelMatrixTerrenator[3][0], modelMatrixTerrenator[3][2]);
	//modelMatrixTerrenator = glm::scale(modelMatrixTerrenator, glm::vec3(1.3, 1.3, 1.3));
	glm::mat4 modelMatrixTerrenatorChasis = glm::mat4(modelMatrixTerrenator);
	modelMatrixTerrenatorChasis = glm::scale(modelMatrixTerrenatorChasis, glm::vec3(0.5, 0.5, 0.5));
	modelTerrenator.render(modelMatrixTerrenatorChasis);

	glm::mat4 modelMatrixFrontLeftWheel = glm::mat4(modelMatrixTerrenatorChasis);
	modelMatrixFrontLeftWheel = glm::translate(modelMatrixFrontLeftWheel, glm::vec3(2.919, -0.3686, 4.259));
	modelMatrixFrontLeftWheel = glm::rotate(modelMatrixFrontLeftWheel, rotWheelsY, glm::vec3(0, 1, 0));
	//modelMatrixFrontLeftWheel = glm::rotate(modelMatrixFrontLeftWheel, rotWheelsX, glm::vec3(1, 0, 0));
	modelMatrixFrontLeftWheel = glm::translate(modelMatrixFrontLeftWheel, glm::vec3(-2.919, 0.3686, -4.259));
	modelMatrixFrontLeftWheel = glm::translate(modelMatrixFrontLeftWheel, glm::vec3(0, 0.6, 0));
	modelTerrenatorFrontLeftWheel.render(modelMatrixFrontLeftWheel);

	glm::mat4 modelMatrixFrontRightWheel = glm::mat4(modelMatrixTerrenatorChasis);
	modelMatrixFrontRightWheel = glm::translate(modelMatrixFrontRightWheel, glm::vec3(-2.919, -0.3686, 4.259));
	modelMatrixFrontRightWheel = glm::rotate(modelMatrixFrontRightWheel, rotWheelsY, glm::vec3(0, 1, 0));
	//modelMatrixFrontRightWheel = glm::rotate(modelMatrixFrontRightWheel, rotWheelsX, glm::vec3(1, 0, 0));
	modelMatrixFrontRightWheel = glm::translate(modelMatrixFrontRightWheel, glm::vec3(2.919, 0.3686, -4.259));
	modelMatrixFrontRightWheel = glm::translate(modelMatrixFrontRightWheel, glm::vec3(0, 0.6, 0));
	modelTerrenatorFrontRightWheel.render(modelMatrixFrontRightWheel);

	modelTerrenatorRearLeftWheel.render(modelMatrixTerrenatorChasis);
	modelTerrenatorRearRightWheel.render(modelMatrixTerrenatorChasis);
	glEnable(GL_CULL_FACE);

	//Meta
	glDisable(GL_CULL_FACE);
	glm::mat4 modelMatrixMetaBody = glm::mat4(modelMatrixMeta);
	modelMatrixMetaBody = glm::scale(modelMatrixMetaBody, glm::vec3(2.0, 2.0, 2.0));
	modelMeta.render(modelMatrixMetaBody);
	glEnable(GL_CULL_FACE);

	//Barrel 1
	glDisable(GL_CULL_FACE);
	glm::mat4 modelBarril = glm::mat4(modelMatrixBarrel);
	modelBarril = translate(modelBarril, glm::vec3(-13.67, 1.3, 23.04));
	modelBarril = glm::rotate(modelBarril, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	modelBarril = glm::scale(modelBarril, glm::vec3(1.0f, 1.0f, 1.0f));
	modelBarrel.render(modelBarril);
	glEnable(GL_CULL_FACE);

	//Barrel 2
	glDisable(GL_CULL_FACE);
	glm::mat4 modelBarril2 = glm::mat4(modelMatrixBarrel);
	modelBarril2 = translate(modelBarril2, glm::vec3(-19.14, 1.3, -19.14));
	modelBarril2 = glm::rotate(modelBarril2, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	modelBarril2 = glm::scale(modelBarril2, glm::vec3(1.0f, 1.0f, 1.0f));
	modelBarrel.render(modelBarril2);
	glEnable(GL_CULL_FACE);

	//Barrel 3
	glDisable(GL_CULL_FACE);
	glm::mat4 modelBarril3 = glm::mat4(modelMatrixBarrel);
	modelBarril3 = translate(modelBarril3, glm::vec3(54.68, 1.3, -17.57));
	modelBarril3 = glm::rotate(modelBarril3, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	modelBarril3 = glm::scale(modelBarril3, glm::vec3(1.0f, 1.0f, 1.0f));
	modelBarrel.render(modelBarril3);
	glEnable(GL_CULL_FACE);

	//Barrel 4
	glDisable(GL_CULL_FACE);
	glm::mat4 modelBarril4 = glm::mat4(modelMatrixBarrel);
	modelBarril4 = translate(modelBarril4, glm::vec3(-3.125, 1.3, -64.45));
	modelBarril4 = glm::rotate(modelBarril4, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	modelBarril4 = glm::scale(modelBarril4, glm::vec3(1.0f, 1.0f, 1.0f));
	modelBarrel.render(modelBarril4);
	glEnable(GL_CULL_FACE);

	//Barrel 5
	glDisable(GL_CULL_FACE);
	glm::mat4 modelBarril5 = glm::mat4(modelMatrixBarrel);
	modelBarril5 = translate(modelBarril5, glm::vec3(-59.76, 1.3, 57.42));
	modelBarril5 = glm::rotate(modelBarril5, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	modelBarril5 = glm::scale(modelBarril5, glm::vec3(1.0f, 1.0f, 1.0f));
	modelBarrel.render(modelBarril5);
	glEnable(GL_CULL_FACE);

	//Barrel 6
	glDisable(GL_CULL_FACE);
	glm::mat4 modelBarril6 = glm::mat4(modelMatrixBarrel);
	modelBarril6 = translate(modelBarril6, glm::vec3(54.68, 1.3, 63.67));
	modelBarril6 = glm::rotate(modelBarril6, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	modelBarril6 = glm::scale(modelBarril6, glm::vec3(1.0f, 1.0f, 1.0f));
	modelBarrel.render(modelBarril6);
	glEnable(GL_CULL_FACE);

	//Barrel 7
	glDisable(GL_CULL_FACE);
	glm::mat4 modelBarril7 = glm::mat4(modelMatrixBarrel);
	modelBarril7 = translate(modelBarril7, glm::vec3(65.23, 1.3, 76.17));
	modelBarril7 = glm::rotate(modelBarril7, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	modelBarril7 = glm::scale(modelBarril7, glm::vec3(1.0f, 1.0f, 1.0f));
	modelBarrel.render(modelBarril7);
	glEnable(GL_CULL_FACE);

	//Rock2 1
	glDisable(GL_CULL_FACE);
	glm::mat4 modelRocktwo = glm::mat4(modelMatrixRock2);
	modelRocktwo = translate(modelRocktwo, glm::vec3(0.78, 3.0, 17.57));
	modelRocktwo = glm::scale(modelRocktwo, glm::vec3(0.08f, 0.08f, 0.08f));
	modelRock2.render(modelRocktwo);
	glEnable(GL_CULL_FACE);

	//Rock2 2
	glDisable(GL_CULL_FACE);
	glm::mat4 modelRocktwo2 = glm::mat4(modelMatrixRock2);
	modelRocktwo2 = translate(modelRocktwo2, glm::vec3(22.26, 3.0, -14.45));
	modelRocktwo2 = glm::scale(modelRocktwo2, glm::vec3(0.08f, 0.08f, 0.08f));
	modelRock2.render(modelRocktwo2);
	glEnable(GL_CULL_FACE);

	//Rock2 3
	glDisable(GL_CULL_FACE);
	glm::mat4 modelRocktwo3 = glm::mat4(modelMatrixRock2);
	modelRocktwo3 = translate(modelRocktwo3, glm::vec3(-64.01, 3.0, 17.18));
	modelRocktwo3 = glm::scale(modelRocktwo3, glm::vec3(0.08f, 0.08f, 0.08f));
	modelRock2.render(modelRocktwo3);
	glEnable(GL_CULL_FACE);

	//Rock2 4
	glDisable(GL_CULL_FACE);
	glm::mat4 modelRocktwo4 = glm::mat4(modelMatrixRock2);
	modelRocktwo4 = translate(modelRocktwo4, glm::vec3(70.43, 3.0, 26.95));
	modelRocktwo4 = glm::scale(modelRocktwo4, glm::vec3(0.08f, 0.08f, 0.08f));
	modelRock2.render(modelRocktwo4);
	glEnable(GL_CULL_FACE);

	//Wheels 1
	glDisable(GL_CULL_FACE);
	glm::mat4 model3Wheels = glm::mat4(modelMatrixThreeWheels);
	model3Wheels = translate(model3Wheels, glm::vec3((-16.01 - 1.4), 2.0, 1.95));
	model3Wheels = glm::rotate(model3Wheels, glm::radians(-90.0f), glm::vec3(0, 0, 1));
	model3Wheels = glm::scale(model3Wheels, glm::vec3(0.06f, 0.06f, 0.06f));
	modelThreeWheels.render(model3Wheels);
	glEnable(GL_CULL_FACE);

	//Wheels 2
	glDisable(GL_CULL_FACE);
	glm::mat4 model3Wheels2 = glm::mat4(modelMatrixThreeWheels);
	model3Wheels2 = translate(model3Wheels2, glm::vec3((65.23 - 1.4), 2.0, -41.01));
	model3Wheels2 = glm::rotate(model3Wheels2, glm::radians(-90.0f), glm::vec3(0, 0, 1));
	model3Wheels2 = glm::scale(model3Wheels2, glm::vec3(0.06f, 0.06f, 0.06f));
	modelThreeWheels.render(model3Wheels2);
	glEnable(GL_CULL_FACE);

	//Wheels 3
	glDisable(GL_CULL_FACE);
	glm::mat4 model3Wheels3 = glm::mat4(modelMatrixThreeWheels);
	model3Wheels3 = translate(model3Wheels3, glm::vec3((-30.46 - 1.4), 3.0, -70.31));
	model3Wheels3 = glm::rotate(model3Wheels3, glm::radians(-90.0f), glm::vec3(0, 0, 1));
	model3Wheels3 = glm::scale(model3Wheels3, glm::vec3(0.06f, 0.06f, 0.06f));
	modelThreeWheels.render(model3Wheels3);
	glEnable(GL_CULL_FACE);

	//Wheels 4
	glDisable(GL_CULL_FACE);
	glm::mat4 model3Wheels4 = glm::mat4(modelMatrixThreeWheels);
	model3Wheels4 = translate(model3Wheels4, glm::vec3((-60.54 - 1.4), 2.0, -28.90));
	model3Wheels4 = glm::rotate(model3Wheels4, glm::radians(-90.0f), glm::vec3(0, 0, 1));
	model3Wheels4 = glm::scale(model3Wheels4, glm::vec3(0.06f, 0.06f, 0.06f));
	modelThreeWheels.render(model3Wheels4);
	glEnable(GL_CULL_FACE);

	//Wheels 5
	glDisable(GL_CULL_FACE);
	glm::mat4 model3Wheels5 = glm::mat4(modelMatrixThreeWheels);
	model3Wheels5 = translate(model3Wheels5, glm::vec3((2.43 - 1.4), 2.5, 67.57));
	model3Wheels5 = glm::rotate(model3Wheels5, glm::radians(-90.0f), glm::vec3(0, 0, 1));
	model3Wheels5 = glm::scale(model3Wheels5, glm::vec3(0.06f, 0.06f, 0.06f));
	modelThreeWheels.render(model3Wheels5);
	glEnable(GL_CULL_FACE);

	//Victory
	glDisable(GL_CULL_FACE);
	glm::mat4 modelVictory = glm::mat4(modelMatrixVictory);
	modelVictory = translate(modelVictory, glm::vec3(13.28, 1.5, 1.95));
	modelVictory = glm::scale(modelVictory, glm::vec3(0.02f, 0.02f, 0.02f));
	victoryModelAnimate.render(modelVictory);
	glEnable(GL_CULL_FACE);

	glDisable(GL_CULL_FACE);
	glm::mat4 modelVictory2 = glm::mat4(modelMatrixVictory);
	modelVictory2 = translate(modelVictory2, glm::vec3(41.79, 1.5, 53.125));
	modelVictory2 = glm::scale(modelVictory2, glm::vec3(0.02f, 0.02f, 0.02f));
	victoryModelAnimate.render(modelVictory2);
	glEnable(GL_CULL_FACE);

	glDisable(GL_CULL_FACE);
	glm::mat4 modelVictory3 = glm::mat4(modelMatrixVictory);
	modelVictory3 = translate(modelVictory3, glm::vec3(-37.1, 1.5, 48.04));
	modelVictory3 = glm::scale(modelVictory3, glm::vec3(0.02f, 0.02f, 0.02f));
	victoryModelAnimate.render(modelVictory3);
	glEnable(GL_CULL_FACE);

	glDisable(GL_CULL_FACE);
	glm::mat4 modelVictory4 = glm::mat4(modelMatrixVictory);
	modelVictory4 = translate(modelVictory4, glm::vec3(39.45, 1.5, -38.28));
	modelVictory4 = glm::scale(modelVictory4, glm::vec3(0.02f, 0.02f, 0.02f));
	victoryModelAnimate.render(modelVictory4);
	glEnable(GL_CULL_FACE);

	glDisable(GL_CULL_FACE);
	glm::mat4 modelVictory5 = glm::mat4(modelMatrixVictory);
	modelVictory5 = translate(modelVictory5, glm::vec3(-22.65, 1.5, -84.76));
	modelVictory5 = glm::scale(modelVictory5, glm::vec3(0.02f, 0.02f, 0.02f));
	victoryModelAnimate.render(modelVictory5);
	glEnable(GL_CULL_FACE);

	//glDisable(GL_CULL_FACE);
	//glm::mat4 modelVictory2 = glm::mat4(modelMatrixVictory2);
	//modelVictory2 = translate(modelVictory2, glm::vec3(-0.023515, 1.0, 4.446066));
	//modelVictory2 = glm::scale(modelVictory2, glm::vec3(0.02f, 0.02f, 0.02f));
	//victoryModelAnimate2.render(modelVictory2);
	//glEnable(GL_CULL_FACE);

	//glDisable(GL_CULL_FACE);
	//glm::mat4 modelVictory3 = glm::mat4(modelMatrixVictory3);
	//modelVictory3 = translate(modelVictory3, glm::vec3(-0.023515, 1.0, 8.446066));
	//modelVictory3 = glm::scale(modelVictory3, glm::vec3(0.02f, 0.02f, 0.02f));
	//victoryModelAnimate3.render(modelVictory3);
	//glEnable(GL_CULL_FACE);

	//// Dart lego
	//// Se deshabilita el cull faces IMPORTANTE para la capa
	//glDisable(GL_CULL_FACE);
	//modelMatrixDart[3][1] = terrain.getHeightTerrain(modelMatrixDart[3][0], modelMatrixDart[3][2]);
	//glm::mat4 modelMatrixDartBody = glm::mat4(modelMatrixDart);
	//modelMatrixDartBody = glm::scale(modelMatrixDartBody, glm::vec3(0.5, 0.5, 0.5));
	//modelDartLegoBody.render(modelMatrixDartBody);
	//glm::mat4 modelMatrixDartHead = glm::mat4(modelMatrixDartBody);
	//modelMatrixDartHead = glm::rotate(modelMatrixDartHead, rotDartHead, glm::vec3(0, 1, 0));
	//modelDartLegoHead.render(modelMatrixDartHead);
	//modelDartLegoMask.render(modelMatrixDartHead);
	//glm::mat4 modelMatrixDartLeftArm = glm::mat4(modelMatrixDartBody);
	//modelMatrixDartLeftArm = glm::translate(modelMatrixDartLeftArm, glm::vec3(-0.023515, 2.43607, 0.446066));
	//modelMatrixDartLeftArm = glm::rotate(modelMatrixDartLeftArm, glm::radians(-5.0f), glm::vec3(1, 0, 0));
	//modelMatrixDartLeftArm = glm::rotate(modelMatrixDartLeftArm, rotDartLeftArm, glm::vec3(0, 0, 1));
	//modelMatrixDartLeftArm = glm::rotate(modelMatrixDartLeftArm, glm::radians(5.0f), glm::vec3(1, 0, 0));
	//modelMatrixDartLeftArm = glm::translate(modelMatrixDartLeftArm, glm::vec3(0.023515, -2.43607, -0.446066));
	//modelDartLegoLeftArm.render(modelMatrixDartLeftArm);
	//glm::mat4 modelMatrixDartLeftHand = glm::mat4(modelMatrixDartLeftArm);
	//modelMatrixDartLeftHand = glm::translate(modelMatrixDartLeftHand, glm::vec3(0.201343, 1.68317, 0.99774));
	//modelMatrixDartLeftHand = glm::rotate(modelMatrixDartLeftHand, glm::radians(-5.0f), glm::vec3(1, 0, 0));
	//modelMatrixDartLeftHand = glm::rotate(modelMatrixDartLeftHand, rotDartLeftHand, glm::vec3(0, 1, 0));
	//modelMatrixDartLeftHand = glm::rotate(modelMatrixDartLeftHand, glm::radians(5.0f), glm::vec3(1, 0, 0));
	//modelMatrixDartLeftHand = glm::translate(modelMatrixDartLeftHand, glm::vec3(-0.201343, -1.68317, -0.99774));
	//modelDartLegoLeftHand.render(modelMatrixDartLeftHand);
	//glm::mat4 modelMatrixDartRightArm = glm::mat4(modelMatrixDartBody);
	//modelMatrixDartRightArm = glm::translate(modelMatrixDartRightArm, glm::vec3(-0.023515, 2.43607, -0.446066));
	//modelMatrixDartRightArm = glm::rotate(modelMatrixDartRightArm, glm::radians(5.0f), glm::vec3(1, 0, 0));
	//modelMatrixDartRightArm = glm::rotate(modelMatrixDartRightArm, rotDartRightArm, glm::vec3(0, 0, 1));
	//modelMatrixDartRightArm = glm::rotate(modelMatrixDartRightArm, glm::radians(-5.0f), glm::vec3(1, 0, 0));
	//modelMatrixDartRightArm = glm::translate(modelMatrixDartRightArm, glm::vec3(0.023515, -2.43607, 0.446066));
	//modelDartLegoRightArm.render(modelMatrixDartRightArm);
	//glm::mat4 modelMatrixDartRightHand = glm::mat4(modelMatrixDartRightArm);
	//modelMatrixDartRightHand = glm::translate(modelMatrixDartRightHand, glm::vec3(0.201343, 1.68317, -0.99774));
	//modelMatrixDartRightHand = glm::rotate(modelMatrixDartRightHand, glm::radians(5.0f), glm::vec3(1, 0, 0));
	//modelMatrixDartRightHand = glm::rotate(modelMatrixDartRightHand, rotDartRightHand, glm::vec3(0, 1, 0));
	//modelMatrixDartRightHand = glm::rotate(modelMatrixDartRightHand, glm::radians(-5.0f), glm::vec3(1, 0, 0));
	//modelMatrixDartRightHand = glm::translate(modelMatrixDartRightHand, glm::vec3(-0.201343, -1.68317, 0.99774));
	//modelDartLegoRightHand.render(modelMatrixDartRightHand);
	//glm::mat4 modelMatrixDartLeftLeg = glm::mat4(modelMatrixDartBody);
	//modelMatrixDartLeftLeg = glm::translate(modelMatrixDartLeftLeg, glm::vec3(0, 1.12632, 0.423349));
	//modelMatrixDartLeftLeg = glm::rotate(modelMatrixDartLeftLeg, rotDartLeftLeg, glm::vec3(0, 0, 1));
	//modelMatrixDartLeftLeg = glm::translate(modelMatrixDartLeftLeg, glm::vec3(0, -1.12632, -0.423349));
	//modelDartLegoLeftLeg.render(modelMatrixDartLeftLeg);
	//glm::mat4 modelMatrixDartRightLeg = glm::mat4(modelMatrixDartBody);
	//modelMatrixDartRightLeg = glm::translate(modelMatrixDartRightLeg, glm::vec3(0, 1.12632, -0.423349));
	//modelMatrixDartRightLeg = glm::rotate(modelMatrixDartRightLeg, rotDartRightLeg, glm::vec3(0, 0, 1));
	//modelMatrixDartRightLeg = glm::translate(modelMatrixDartRightLeg, glm::vec3(0, -1.12632, 0.423349));
	//modelDartLegoRightLeg.render(modelMatrixDartRightLeg);
	//// Se regresa el cull faces IMPORTANTE para la capa
	//glEnable(GL_CULL_FACE);

	/*******************************************
	 * Custom Anim objects obj
	 *******************************************/

	/**********
	 * Update the position with alpha objects
	 */
	// Update the Terrenator
	//blendingUnsorted.find("Terrenator")->second = glm::vec3(modelMatrixTerrenator[3]);

	/**********
	 * Sorter with alpha objects
	 */
	std::map<float, std::pair<std::string, glm::vec3>> blendingSorted;
	std::map<std::string, glm::vec3>::iterator itblend;
	for(itblend = blendingUnsorted.begin(); itblend != blendingUnsorted.end(); itblend++){
		float distanceFromView = glm::length(camera->getPosition() - itblend->second);
		blendingSorted[distanceFromView] = std::make_pair(itblend->first, itblend->second);
	}

	/**********
	 * Render de las transparencias
	 */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	for(std::map<float, std::pair<std::string, glm::vec3> >::reverse_iterator it = blendingSorted.rbegin(); it != blendingSorted.rend(); it++){
		if(renderParticles && it->second.first.compare("fountain") == 0){
			/**********
			 * Init Render particles systems
			 */
			glm::mat4 modelMatrixParticlesFountain = glm::mat4(1.0);
			modelMatrixParticlesFountain = glm::translate(modelMatrixParticlesFountain, it->second.second);
			modelMatrixParticlesFountain[3][1] = terrain.getHeightTerrain(modelMatrixParticlesFountain[3][0], modelMatrixParticlesFountain[3][2]) + 0.36 * 10.0;
			modelMatrixParticlesFountain = glm::scale(modelMatrixParticlesFountain, glm::vec3(3.0, 3.0, 3.0));
			currTimeParticlesAnimation = TimeManager::Instance().GetTime();
			if(currTimeParticlesAnimation - lastTimeParticlesAnimation > 10.0)
				lastTimeParticlesAnimation = currTimeParticlesAnimation;
			//glDisable(GL_DEPTH_TEST);
			glDepthMask(GL_FALSE);
			// Set the point size
			glPointSize(10.0f);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureParticleFountainID);
			shaderParticlesFountain.turnOn();
			shaderParticlesFountain.setFloat("Time", float(currTimeParticlesAnimation - lastTimeParticlesAnimation));
			shaderParticlesFountain.setFloat("ParticleLifetime", 3.5f);
			shaderParticlesFountain.setInt("ParticleTex", 0);
			shaderParticlesFountain.setVectorFloat3("Gravity", glm::value_ptr(glm::vec3(0.0f, -0.3f, 0.0f)));
			shaderParticlesFountain.setMatrix4("model", 1, false, glm::value_ptr(modelMatrixParticlesFountain));
			glBindVertexArray(VAOParticles);
			glDrawArrays(GL_POINTS, 0, nParticles);
			glDepthMask(GL_TRUE);
			//glEnable(GL_DEPTH_TEST);
			shaderParticlesFountain.turnOff();
			/**********
			 * End Render particles systems
			 */
		}
		//else if(renderParticles && it->second.first.compare("fire") == 0){
		//	/**********
		//	 * Init Render particles systems
		//	 */
		//	lastTimeParticlesAnimationFire = currTimeParticlesAnimationFire;
		//	currTimeParticlesAnimationFire = TimeManager::Instance().GetTime();

		//	shaderParticlesFire.setInt("Pass", 1);
		//	shaderParticlesFire.setFloat("Time", currTimeParticlesAnimationFire);
		//	shaderParticlesFire.setFloat("DeltaT", currTimeParticlesAnimationFire - lastTimeParticlesAnimationFire);

		//	glActiveTexture(GL_TEXTURE1);
		//	glBindTexture(GL_TEXTURE_1D, texId);
		//	glEnable(GL_RASTERIZER_DISCARD);
		//	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);
		//	glBeginTransformFeedback(GL_POINTS);
		//	glBindVertexArray(particleArray[1-drawBuf]);
		//	glVertexAttribDivisor(0,0);
		//	glVertexAttribDivisor(1,0);
		//	glVertexAttribDivisor(2,0);
		//	glDrawArrays(GL_POINTS, 0, nParticlesFire);
		//	glEndTransformFeedback();
		//	glDisable(GL_RASTERIZER_DISCARD);

		//	shaderParticlesFire.setInt("Pass", 2);
		//	glm::mat4 modelFireParticles = glm::mat4(1.0);
		//	modelFireParticles = glm::translate(modelFireParticles, it->second.second);
		//	modelFireParticles[3][1] = terrain.getHeightTerrain(modelFireParticles[3][0], modelFireParticles[3][2]);
		//	shaderParticlesFire.setMatrix4("model", 1, false, glm::value_ptr(modelFireParticles));

		//	shaderParticlesFire.turnOn();
		//	glActiveTexture(GL_TEXTURE0);
		//	glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
		//	glDepthMask(GL_FALSE);
		//	glBindVertexArray(particleArray[drawBuf]);
		//	glVertexAttribDivisor(0,1);
		//	glVertexAttribDivisor(1,1);
		//	glVertexAttribDivisor(2,1);
		//	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticlesFire);
		//	glBindVertexArray(0);
		//	glDepthMask(GL_TRUE);
		//	drawBuf = 1 - drawBuf;
		//	shaderParticlesFire.turnOff();

		//	/****************************+
		//	 * Open AL sound data
		//	 */
		//	source1Pos[0] = modelFireParticles[3].x;
		//	source1Pos[1] = modelFireParticles[3].y;
		//	source1Pos[2] = modelFireParticles[3].z;
		//	alSourcefv(source[1], AL_POSITION, source1Pos);

		//	/**********
		//	 * End Render particles systems
		//	 */
		//}
		else if (renderParticles && it->second.first.compare("lluvia") == 0) {
			/**********
			 * Init Render particles systems
			 */
			 lastTimeParticlesAnimationFire = currTimeParticlesAnimationFire;
			 currTimeParticlesAnimationFire = TimeManager::Instance().GetTime();

			 shaderParticlesLluvia.setInt("Pass", 1);
			 shaderParticlesLluvia.setFloat("Time", currTimeParticlesAnimationFire);
			 shaderParticlesLluvia.setFloat("DeltaT", currTimeParticlesAnimationFire - lastTimeParticlesAnimationFire);

			 glActiveTexture(GL_TEXTURE1);
			 glBindTexture(GL_TEXTURE_1D, texId);
			 glEnable(GL_RASTERIZER_DISCARD);
			 glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);
			 glBeginTransformFeedback(GL_POINTS);
			 glBindVertexArray(particleArray[1 - drawBuf]);
			 glVertexAttribDivisor(0, 0);
			 glVertexAttribDivisor(1, 0);
			 glVertexAttribDivisor(2, 0);
			 glDrawArrays(GL_POINTS, 0, nParticlesFire);
			 glEndTransformFeedback();
			 glDisable(GL_RASTERIZER_DISCARD);

			 shaderParticlesLluvia.setInt("Pass", 2);
			 glm::mat4 modelFireParticles = glm::mat4(1.0);
			 modelFireParticles = glm::translate(modelFireParticles, it->second.second);
			 modelFireParticles[3][1] = terrain.getHeightTerrain(modelFireParticles[3][0], modelFireParticles[3][2]);
			 //--------La lluvia sigue al modelo---------//
			 glm::vec3 positionEmitter = glm::vec3(modelMatrixTerrenator[3]);
			 positionEmitter.y = 10.0;
			 shaderParticlesLluvia.setVectorFloat3("Emitter", glm::value_ptr(glm::vec3(positionEmitter)));
			 //-------------//
			 shaderParticlesLluvia.setMatrix4("model", 1, false, glm::value_ptr(modelFireParticles));

			 shaderParticlesLluvia.turnOn();
			 glActiveTexture(GL_TEXTURE0);
			 glBindTexture(GL_TEXTURE_2D, textureParticleFountainID);
			 glDepthMask(GL_FALSE);
			 glBindVertexArray(particleArray[drawBuf]);
			 glVertexAttribDivisor(0, 1);
			 glVertexAttribDivisor(1, 1);
			 glVertexAttribDivisor(2, 1);
			 glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticlesFire);
			 glBindVertexArray(0);
			 glDepthMask(GL_TRUE);
			 drawBuf = 1 - drawBuf;
			 shaderParticlesLluvia.turnOff();

			 /****************************+
			  * Open AL sound data
			  */
			 source1Pos[0] = modelFireParticles[3].x;
			 source1Pos[1] = modelFireParticles[3].y;
			 source1Pos[2] = modelFireParticles[3].z;
			 alSourcefv(source[1], AL_POSITION, source1Pos);

			 /**********
			  * End Render particles systems
			  */
		}

	}
	glEnable(GL_CULL_FACE);
}

int main(int argc, char **argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}
