/*---------------------------------------------------------*/
/* ------------      Proyecto Final             -----------*/
/*-------------        2022-2   ---------------------------*/
/*------------- Alumno: Santiago Escobar    ---------------*/
/*------------- No. Cuenta : 722030330      ---------------*/
#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h> //main
#include <stdlib.h>
#include <glm/glm.hpp>					//camera y model
#include <glm/gtc/matrix_transform.hpp> //camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h> //Texture

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>

//#pragma comment(lib, "winmm.lib")

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);
// glm::mat4 drawTiling(glm::vec3 offset, glm::vec3 scale, Shader shader, glm::mat4 origin, Model model2R);
glm::mat4 drawObject(glm::vec3 offset, glm::vec3 scale, Shader shader, glm::mat4 origin, Model model2R);
glm::mat4 drawObject(glm::vec3 offset, glm::vec3 rotationAxis, float rotationAngle, glm::vec3 scale, Shader shader, glm::mat4 origin, Model model2R);
glm::mat4 drawObject(glm::vec3 offset, glm::vec3 rotationAxis, glm::vec3 rotationAngle, glm::vec3 scale, Shader shader, glm::mat4 origin, Model model2R);
// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor* monitors;

void getResolution(void);

// camera
Camera camera(glm::vec3(0.0f, 80.0f, 40.0f));
float MovementSpeed = 0.1f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double deltaTime = 0.0f,
lastFrame = 0.0f;

// Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);

// Auto
float movAuto_x = 0.0f,
movAuto_z = 0.0f,
orienta = 0.0f;
bool animacion = false,
recorrido1 = true,
recorrido2 = false,
recorrido3 = false,
recorrido4 = false;

// Music
bool music = false;

// -------------------------------------------------------------------------------------------------------------------------
// Preprocessor directives to control models (load + draw)
// 1 = Draw model
// Gate and restaurant models are heavy, use with caution
// -------------------------------------------------------------------------------------------------------------------------
#define DEBUGMODE 1
#define DRAWFLOOR 1
#define DRAWFENCE 1
#define DRAWVOLCANO 0
#define DRAWLAMBO 0
#define DRAWPTERO 0
#define DRAWHOTEL 0
#define DRAWRESTAURANT 0
#define DRAWARLO 0
#define DRAWHELICOPTER 0
#define DRAWTREX 0
#define DRAWANKYLO 0
#define DRAWTRICERATOPS 0
#define DRAWVELOCIRAPTOR 0
// PENDING LOCATION
#define DRAWTREES 1
// PENDING SIZE+LOCATION
#define DRAWBUGGY 0
#define DRAWGATE 0
#define DRAWHOUSES 1


// -------------------------------------------------------------------------------------------------------------------------
// Object location
// -------------------------------------------------------------------------------------------------------------------------
// Origin
glm::mat4 originWorld = glm::mat4(1.0f);

// Rotation
glm::vec3 xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 zAxis = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 allAxis = glm::vec3(1.0f);

// Floor tiling
const float floorScale = 2.0f;
const float floorTilingSpacing = 100.0f * floorScale;
const float floorYOffset = -1.0f;
const int floorLimitX = 4;
const int floorLowerLimitZ = -2;
const int floorUpperLimitZ = 2;

// Fence
const float fenceOffsetX = -floorTilingSpacing / 2;
const float fenceScale = 15.0f;
const float fenceSize = 2.436f;
const float fenceTilingSpacing = fenceSize * fenceScale;
const float fenceUpperLimitZ = (floorUpperLimitZ * floorTilingSpacing) - (floorTilingSpacing / 2) - (fenceTilingSpacing / 2);
const float fenceLowerLimitZ = (floorLowerLimitZ * floorTilingSpacing) - (floorTilingSpacing / 2) + (fenceTilingSpacing / 2);
const float fenceNumber = (floorTilingSpacing * (floorUpperLimitZ - floorLowerLimitZ + 1)) / fenceTilingSpacing;

// Volcano
const float volcanoScale = 15.0f;
const glm::vec3 volcanoSize = glm::vec3(37.81f * volcanoScale, 37.81f * volcanoScale, 13.978f * volcanoScale);
const glm::vec3 volcanoPosition = glm::vec3((-floorLimitX * floorTilingSpacing), floorYOffset + (volcanoSize.z / 2), 0.0f);

// ---------------
// Vehicles
// ---------------

// Helicopter
float helicopterScale = 1.2f;
float helipadScale = 0.4f;
glm::vec3 helicopterLocation = glm::vec3(25.0f, 14.0f * helicopterScale, 440.0f);
glm::vec3 helipadLocation = glm::vec3(helicopterLocation.x + (10.0f * helicopterScale), 0.0f, helicopterLocation.z);
// glm::vec3 helicopterRotationAxis = yAxis;
// float helicopterRotation = 90.0f ;
float helicopterOffsetPropellerRearX = 58.4f * helicopterScale;
float helicopterOffsetPropellerRearY = 8.46f * helicopterScale;
float helicopterOffsetPropellerRearZ = 0.0f * helicopterScale;
float helicopterOffsetPropellerFrontX = 5.84f * helicopterScale;
float helicopterOffsetPropellerFrontY = 12.46f * helicopterScale;
float helicopterOffsetPropellerFrontZ = 0.0f * helicopterScale;

//Buggy

// ---------------
// Constructions
// ---------------
// Restaurant
const float restaurantScale = 0.18f;
const glm::vec3 restaurantLocation = glm::vec3(90.0f, 0.0f, -400.0f);
const glm::vec3 restaurantRotationAxis = yAxis;
const float restaurantRotation = -135.0f;

// Hotel
const float hotelScale = 11.0f;
const glm::vec3 hotelLocation = glm::vec3(350.0f, 0.0f, 1000.0f);
const glm::vec3 hotelRotationAxis = yAxis;
const float hotelRotation = 180.0f;

// Gate
const float gatetScale = 7.0f;
const glm::vec3 gateLocation = glm::vec3(800.0f, 0.0f, 0.0f);
const glm::vec3 gateRotationAxis = yAxis;
const float gateRotation = 90.0f;

// Houses

const float houseScale = 0.5f;
const glm::vec3 houseRotationAxis = yAxis;
const float houseRotation = 90.0f;
const glm::vec3 houseLocation = glm::vec3(300.0f, floorYOffset, 400.0f);
// const int housesNumber = 5;
// glm::vec3 housesLocation[housesNumber] = {};

// ---------------
// Dinosaurs
// ---------------

// Pterosaur
glm::vec3 pterosaurLocation = glm::vec3(volcanoPosition.x + (10.0f * volcanoScale), volcanoPosition.y + (5.0f * volcanoScale), volcanoPosition.z);
const float pterosaurScale = 0.5f;
const float pterosaurWingOffsetX = 12.0f * pterosaurScale;
const float pterosaurWingOffsetY = 12.0f * pterosaurScale;
const float pterosaurWingOffsetZ = 3.6f * pterosaurScale;
const float pterosaurHeadOffsetY = 15.6f * pterosaurScale;
const float pterosaurHeadOffsetZ = -27.0f * pterosaurScale;
float pterosaurWingRotation = 30.0f;

// Arlo
const float arloScale = 5.0f * 11.1f;
const glm::vec3 arloSize = glm::vec3(0.315f * arloScale, 0.995f * arloScale, 1.29f * arloScale);
const glm::vec3 arloLocation = glm::vec3(-200.0f, 0.0f, -100.0f);
const glm::vec3 arloRotationAxis = yAxis;
const float arloRotation = -60.0f;

// Trex
const float tRexScale = 4.0f;
const glm::vec3 tRexLocation = glm::vec3(-340.0f, 0.0f, -340.0f);
const glm::vec3 tRexRotationAxis = yAxis;
const float tRexRotation = 90.0f;
const float tRexStadiumScale = 5.0f;

// Ankylo
const float ankyloScale = 8.0f;
const glm::vec3 ankyloLocation = glm::vec3(-300.0f, floorYOffset, 0.0f);
const glm::vec3 ankyloRotationAxis = yAxis;
const float ankyloRotation = 0.0f;

// Triceratops
const float triceratopsScale = 2.8f;
const glm::vec3 triceratopsLocation = glm::vec3(-200.0f, floorYOffset, 100.0f);
const glm::vec3 triceratopsRotationAxis = yAxis;
const float triceratopsRotation = 90.0f;

// Velociraptor
const float velociraptorScale = 0.2f;
const glm::vec3 velociraptorLocation = glm::vec3(-130.0f, -floorYOffset * 10.0f, 0.0f);
const glm::vec3 velociraptorRotationAxis = allAxis;
const glm::vec3 velociraptorRotation = glm::vec3(-260.0f, -60.0f, 90.0f);

// ---------------
// Biome
// ---------------
// Tree
const float treeScale = 3.0f * 5.0f;
float palmTreeScale = 1.89f * 13.0f;
const glm::vec3 treeSize = glm::vec3(1.36f * treeScale, 1.43f * treeScale, 3.42 * treeScale);
const int treeNumber = 2;
glm::vec3 treeLocation[treeNumber] = {
	glm::vec3(100.0f, floorYOffset, 100.0f),
	glm::vec3(150.0f, floorYOffset, 50.0f),
};
glm::vec3 palmTreeLocation[treeNumber] = {
	glm::vec3(arloLocation.x - (arloSize.x * 3), floorYOffset, arloLocation.z + (arloSize.z / 2)),
	glm::vec3(-150.0f, floorYOffset, -50.0f),
};

#if DEBUGMODE == 1
// Debug+measure cube
int cubeNumber = 10;
glm::vec3 cubeLocation = glm::vec3(0.0f);
glm::mat4 cubeLocationTmpX = glm::mat4(1.0f);
glm::mat4 cubeLocationTmpY = glm::mat4(1.0f);
glm::mat4 cubeLocationTmpZ = glm::mat4(1.0f);
// glm::vec3 debugObjectLocation = tRexLocation;
glm::vec3 debugObjectLocation = glm::vec3(0.0f);
glm::vec3 debugObjectRotation = glm::vec3(0.0f);
float debugObjectSpeedMovement = 1.0f;
float debugObjectScale = 1.0f;
// float debugObjectScale = tRexScale;
#endif

void animate(void)
{
	// Vehículo
	if (animacion)
	{
		movAuto_z += 3.0f;
	}
}

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	// Debug
	// std::cout << fenceNumber << std::endl;

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CGeIHC", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	vector<std::string> faces{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg" };

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	// -----------
#if DEBUGMODE == 1
	// Testing and measures cube 10 units in 3dsmax around 1m here
	Model cube10("resources/objects/unitcube/unitCube.obj");
#endif
	//Floor
#if DRAWFLOOR == 1
	Model piso("resources/objects/piso/tile.obj");
	Model pisoPasto("resources/objects/piso/pasto.obj");
	Model pisoArena("resources/objects/piso/beach.obj");
	Model pisoAgua("resources/objects/piso/ocean.obj");
#endif
#if DRAWFENCE == 1
	Model fence("resources/objects/fence/fence.obj");
#endif
#if DRAWVOLCANO == 1
	Model volcano("resources/objects/volcano/volcano2.obj");
#endif
	// Vehicles
#if DRAWHELICOPTER == 1
	Model helicopterBody("resources/objects/helicopter/helicopter_body.obj");
	Model helicopterPropellerFront("resources/objects/helicopter/helicopter_propeller_front.obj");
	Model helicopterPropellerRear("resources/objects/helicopter/helicopter_propeller_rear.obj");
	Model helipad("resources/objects/helicopter/helipad.obj");
#endif
#if DRAWBUGGY == 1
	Model buggy("resources/objects/buggy/buggy.obj");
#endif
#if DRAWLAMBO == 1
	Model carro("resources/objects/lambo/carroceria.obj");
	Model llanta("resources/objects/lambo/Wheel.obj");
#endif
	//Constructions
#if DRAWRESTAURANT == 1
	Model restaurant("resources/objects/restaurant/restaurant.obj");
#endif
#if DRAWHOTEL == 1
	Model hotel("resources/objects/hotel/hotel.obj");
#endif
#if DRAWGATE == 1	
	Model gate("resources/objects/gate/gate.obj");
	// Model gaten("resources/objects/gate/gate_no_normales.obj");
#endif
#if DRAWHOUSES == 1	
	Model house("resources/objects/house/house.obj");
#endif

	// Dinosaurs
#if DRAWPTERO == 1
	Model pterosaurBody("resources/objects/dinosaurs/pterosaur/cuerpo-ptero.obj");
	Model pterosaurLeftWing("resources/objects/dinosaurs/pterosaur/ala-izquierda-ptero.obj");
	Model pterosaurRightWing("resources/objects/dinosaurs/pterosaur/ala-derecha-ptero.obj");
	Model pterosaurHead("resources/objects/dinosaurs/pterosaur/boca-ptero.obj");
#endif
#if DRAWARLO == 1
	Model cuelloLargo("resources/objects/dinosaurs/cuellolargo/arlo.obj");
#endif
#if DRAWTREX == 1
	Model tRex("resources/objects/dinosaurs/t-rex/t-rex.obj");
	Model stadium("resources/objects/stadium/stadium.obj");
#endif
#if DRAWANKYLO == 1
	Model ankylosaurus("resources/objects/dinosaurs/anquilo/anquilo.obj");
#endif
#if DRAWTRICERATOPS == 1
	Model triceratops("resources/objects/dinosaurs/triceratops/triceratops.obj");
#endif
#if DRAWVELOCIRAPTOR == 1
	Model velociraptor("resources/objects/dinosaurs/velociraptor/velociraptor.obj");
#endif	
	// Biome
#if DRAWTREES == 1
	Model tree("resources/objects/tree/tree.obj");
	Model palmTree("resources/objects/tree/bananatree.obj");
#endif



	// ModelAnim animacionPersonaje("resources/objects/Personaje1/PersonajeBrazo.dae");
	// animacionPersonaje.initShaders(animShader.ID);

	// draw in wireframe
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);

		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		// my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		staticShader.use();
		// Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("dirLight.diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));

		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.032f);

		staticShader.setVec3("pointLight[1].position", glm::vec3(-80.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.032f);

		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(0.6f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);

		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		// Remember to activate the shader with the animation
		animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);

		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);

		// model = glm::translate(glm::mat4(1.0f), glm::vec3(-40.3f, 1.75f, 0.3f)); // translate it down so it's at the center of the scene
		// model = glm::scale(model, glm::vec3(1.2f));	// it's a bit too big for our scene, so scale it down
		// model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		// animShader.setMat4("model", model);
		// animacionPersonaje.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario + Piso
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

#if DRAWFLOOR == 1
		for (int i = 0; i < floorLimitX; i++)
		{
			for (int j = floorLowerLimitZ; j <= floorUpperLimitZ; j++)
			{
				drawObject(glm::vec3((floorTilingSpacing * static_cast<float>(i)), floorYOffset, (floorTilingSpacing * static_cast<float>(j))), glm::vec3(floorScale), staticShader, originWorld, piso);
				drawObject(glm::vec3((-floorTilingSpacing * static_cast<float>(i + 1)), floorYOffset, (floorTilingSpacing * static_cast<float>(j))), glm::vec3(floorScale), staticShader, originWorld, pisoPasto);
			}
		}
		drawObject(glm::vec3(0.0f, floorYOffset * 2.0f, 0.0f), glm::vec3(floorScale * 10), staticShader, originWorld, pisoAgua);
#endif

		// -------------------------------------------------------------------------------------------------------------------------
		// Fence
		// -------------------------------------------------------------------------------------------------------------------------
		/* Simulate doors  Pareto
			//Left gate
			tmp = drawObject(glm::vec3(fenceOffsetX, floorYOffset, 0.0f), yAxis, -45.0f, glm::vec3(fenceScale), staticShader, glm::mat4(1.0f), fenceDoor);
			//Right gate
			drawObject(glm::vec3(fenceOffsetX, floorYOffset, 0.0f), yAxis, -225.0f, glm::vec3(fenceScale), staticShader, glm::mat4(1.0f), fenceDoor);
		*/
#if DRAWFENCE == 1
		tmp = drawObject(glm::vec3(fenceOffsetX, floorYOffset, fenceLowerLimitZ), yAxis, -90.0f, glm::vec3(fenceScale), staticShader, originWorld, fence);
		for (int i = 1; i <= fenceNumber; i++)
		{
			tmp = drawObject(glm::vec3(0.0f, 0.0f, fenceTilingSpacing), yAxis, -90.0f, glm::vec3(fenceScale), staticShader, tmp, fence);
		}
#endif
		// -------------------------------------------------------------------------------------------------------------------------
		// Volcano
		// -------------------------------------------------------------------------------------------------------------------------
#if DRAWVOLCANO == 1
		drawObject(volcanoPosition, glm::vec3(volcanoScale), staticShader, originWorld, volcano);
#endif

		// -------------------------------------------------------------------------------------------------------------------------
		// Vehicles
		// -------------------------------------------------------------------------------------------------------------------------
#if DRAWHELICOPTER == 1
		tmp = drawObject(helicopterLocation, glm::vec3(helicopterScale), staticShader, originWorld, helicopterBody);
		drawObject(glm::vec3(helicopterOffsetPropellerFrontX, helicopterOffsetPropellerFrontY, helicopterOffsetPropellerFrontZ), glm::vec3(helicopterScale), staticShader, tmp, helicopterPropellerFront);
		drawObject(glm::vec3(helicopterOffsetPropellerRearX, helicopterOffsetPropellerRearY, helicopterOffsetPropellerRearZ), zAxis, 0.0f, glm::vec3(helicopterScale), staticShader, tmp, helicopterPropellerRear);

		drawObject(helipadLocation, glm::vec3(helipadScale), staticShader, originWorld, helipad);
#endif
#if DRAWBUGGY == 1
		drawObject(glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(1.0f), staticShader, originWorld, buggy);
#endif

		// -------------------------------------------------------------------------------------------------------------------------
		// Constructions
		// -------------------------------------------------------------------------------------------------------------------------
#if DRAWRESTAURANT == 1
		drawObject(restaurantLocation, restaurantRotationAxis, restaurantRotation, glm::vec3(restaurantScale), staticShader, originWorld, restaurant);
#endif
#if DRAWHOTEL == 1
		drawObject(hotelLocation, hotelRotationAxis, hotelRotation, glm::vec3(hotelScale), staticShader, originWorld, hotel);
#endif
#if DRAWGATE == 1
		drawObject(debugObjectLocation, allAxis, debugObjectRotation, glm::vec3(debugObjectScale), staticShader, originWorld, gate);
		// drawObject(glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(1.0f), staticShader, originWorld, gaten);
#endif
#if DRAWHOUSES == 1	
	drawObject(houseLocation,houseRotationAxis, houseRotation, glm::vec3(houseScale), staticShader, originWorld, house);
#endif
		// -------------------------------------------------------------------------------------------------------------------------
		// Dinosaurs
		// -------------------------------------------------------------------------------------------------------------------------
#if DRAWPTERO == 1
		tmp = drawObject(pterosaurLocation, glm::vec3(pterosaurScale), staticShader, originWorld, pterosaurBody);
		drawObject(glm::vec3(0.0f, pterosaurHeadOffsetY, pterosaurHeadOffsetZ), glm::vec3(pterosaurScale), staticShader, tmp, pterosaurHead);
		drawObject(glm::vec3(-pterosaurWingOffsetX, pterosaurWingOffsetY, pterosaurWingOffsetZ), zAxis, -pterosaurWingRotation, glm::vec3(pterosaurScale), staticShader, tmp, pterosaurLeftWing);
		drawObject(glm::vec3(pterosaurWingOffsetX, pterosaurWingOffsetY, pterosaurWingOffsetZ), zAxis, pterosaurWingRotation, glm::vec3(pterosaurScale), staticShader, tmp, pterosaurRightWing);
#endif
#if DRAWARLO == 1
		drawObject(arloLocation, arloRotationAxis, arloRotation, glm::vec3(arloScale), staticShader, originWorld, cuelloLargo);
#endif
#if DRAWTREX == 1
		drawObject(tRexLocation, tRexRotationAxis, tRexRotation, glm::vec3(tRexScale), staticShader, originWorld, tRex);
		drawObject(glm::vec3(tRexLocation.x, -15.0f, tRexLocation.z), glm::vec3(tRexStadiumScale), staticShader, originWorld, stadium);
#endif
#if DRAWANKYLO == 1
		drawObject(ankyloLocation, ankyloRotationAxis, ankyloRotation, glm::vec3(ankyloScale), staticShader, originWorld, ankylosaurus);
#endif
#if DRAWTRICERATOPS == 1
		drawObject(triceratopsLocation, triceratopsRotationAxis, triceratopsRotation, glm::vec3(triceratopsScale), staticShader, originWorld, triceratops);
#endif
#if DRAWVELOCIRAPTOR == 1
		drawObject(velociraptorLocation, velociraptorRotationAxis, velociraptorRotation, glm::vec3(velociraptorScale), staticShader, originWorld, velociraptor);
#endif

		// -------------------------------------------------------------------------------------------------------------------------
		// Biome
		// -------------------------------------------------------------------------------------------------------------------------
#if DRAWTREES == 1
		for (int i = 0; i < treeNumber; i++)
		{
			drawObject(treeLocation[i], glm::vec3(treeScale), staticShader, originWorld, tree);
			drawObject(palmTreeLocation[i], glm::vec3(palmTreeScale), staticShader, originWorld, palmTree);
		}
#endif


#if DEBUGMODE == 1
		// Pruebas
		// drawObject(debugObjectLocation, glm::vec3(1.0f), debugObjectRotation, glm::vec3(debugObjectScale), staticShader, originWorld, model);
		cubeLocationTmpX = cubeLocationTmpY = cubeLocationTmpZ = drawObject(cubeLocation, glm::vec3(1.0f), staticShader, originWorld, cube10);

		for (int i = 1; i < cubeNumber; i++)
		{
			cubeLocationTmpX = drawObject(glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(1.0f), staticShader, cubeLocationTmpX, cube10);
			cubeLocationTmpY = drawObject(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(1.0f), staticShader, cubeLocationTmpY, cube10);
			cubeLocationTmpZ = drawObject(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(1.0f), staticShader, cubeLocationTmpZ, cube10);
		}
#endif

		// -------------------------------------------------------------------------------------------------------------------------
		// Carro (lambo)
		// -------------------------------------------------------------------------------------------------------------------------
#if DRAWLAMBO == 1
		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(15.0f + movAuto_x, -1.0f, movAuto_z));
		tmp = model = glm::rotate(model, glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		carro.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(8.5f, 2.5f, 12.9f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader); // Izq delantera

		model = glm::translate(tmp, glm::vec3(-8.5f, 2.5f, 12.9f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader); // Der delantera

		model = glm::translate(tmp, glm::vec3(-8.5f, 2.5f, -14.5f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader); // Der trasera

		model = glm::translate(tmp, glm::vec3(8.5f, 2.5f, -14.5f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader); // Izq trase
#endif

		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		// std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		music = !music;
#if DEBUGMODE == 1
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		cubeLocation.z -= 1.0f * debugObjectSpeedMovement;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		cubeLocation.z += 1.0f * debugObjectSpeedMovement;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		cubeLocation.x -= 1.0f * debugObjectSpeedMovement;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		cubeLocation.x += 1.0f * debugObjectSpeedMovement;
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		cubeLocation.y += 1.0f * debugObjectSpeedMovement;
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		cubeLocation.y -= 1.0f * debugObjectSpeedMovement;
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		debugObjectLocation.z -= 1.0f * debugObjectSpeedMovement;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		debugObjectLocation.z += 1.0f * debugObjectSpeedMovement;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		debugObjectLocation.x -= 1.0f * debugObjectSpeedMovement;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		debugObjectLocation.x += 1.0f * debugObjectSpeedMovement;
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		debugObjectLocation.y += 1.0f * debugObjectSpeedMovement;
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		debugObjectLocation.y -= 1.0f * debugObjectSpeedMovement;
	if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS)
		debugObjectSpeedMovement > 1 ? debugObjectSpeedMovement-- : (debugObjectSpeedMovement > 0 ? debugObjectSpeedMovement -= 0.1f : false);
	if (glfwGetKey(window, GLFW_KEY_END) == GLFW_PRESS)
		debugObjectSpeedMovement >= 1 ? debugObjectSpeedMovement++ : debugObjectSpeedMovement += 0.1f;
	if (glfwGetKey(window, GLFW_KEY_DELETE) == GLFW_PRESS)
		debugObjectSpeedMovement = 1.0f;
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		debugObjectRotation.z -= 1.0f * debugObjectSpeedMovement;
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		debugObjectRotation.z += 1.0f * debugObjectSpeedMovement;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		debugObjectRotation.x -= 1.0f * debugObjectSpeedMovement;
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		debugObjectRotation.x += 1.0f * debugObjectSpeedMovement;
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		debugObjectRotation.y += 1.0f * debugObjectSpeedMovement;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		debugObjectRotation.y -= 1.0f * debugObjectSpeedMovement;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		debugObjectRotation.z = 0.0f;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		debugObjectRotation.x = 0.0f;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		debugObjectRotation.y = 0.0f;
	if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
		debugObjectScale += 0.1f * debugObjectSpeedMovement;
	if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
		debugObjectScale -= 0.1f * debugObjectSpeedMovement;
	if (action == GLFW_PRESS)
	{
		std::cout << "Location: (" +
			to_string(debugObjectLocation.x) + "," +
			to_string(debugObjectLocation.y) + "," +
			to_string(debugObjectLocation.z) + "). " + "Rotation: (" +
			to_string(debugObjectRotation.x) + "," +
			to_string(debugObjectRotation.y) + "," +
			to_string(debugObjectRotation.z) + ")." + " Scale: " +
			to_string(debugObjectScale) + "."
			<< std::endl;
	}
#endif

	// Car animation
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		animacion ^= true;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void getResolution()
{
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

glm::mat4 drawObject(glm::vec3 offset, glm::vec3 scale, Shader shader, glm::mat4 origin, Model model2R)
{
	glm::mat4 model = glm::translate(origin, offset);
	glm::mat4 tempPos = model;
	model = glm::scale(model, scale);
	shader.setMat4("model", model);
	model2R.Draw(shader);
	return tempPos;
}
glm::mat4 drawObject(glm::vec3 offset, glm::vec3 rotationAxis, float rotationAngle, glm::vec3 scale, Shader shader, glm::mat4 origin, Model model2R)
{
	glm::mat4 model = glm::translate(origin, offset);
	glm::mat4 tempPos = model;
	model = glm::rotate(model, glm::radians(rotationAngle), rotationAxis);
	model = glm::scale(model, scale);
	shader.setMat4("model", model);
	model2R.Draw(shader);
	return tempPos;
}

/**
 * @brief This function allows to rotate in multiple axis passing rotationAngle as a glm::vec3
 * Rotates only the defined rotation angle around the corresponding axis
 *
 * @param offset
 * @param rotationAxis
 * @param rotationAngle
 * @param scale
 * @param shader
 * @param origin
 * @param model2R
 * @return glm::mat4
 */
glm::mat4 drawObject(glm::vec3 offset, glm::vec3 rotationAxis, glm::vec3 rotationAngle, glm::vec3 scale, Shader shader, glm::mat4 origin, Model model2R)
{
	glm::mat4 model = glm::translate(origin, offset);
	glm::mat4 tempPos = model;
	if (rotationAxis.x == 1.0f)
		model = glm::rotate(model, glm::radians(rotationAngle.x), xAxis);
	if (rotationAxis.y == 1.0f)
		model = glm::rotate(model, glm::radians(rotationAngle.y), yAxis);
	if (rotationAxis.z == 1.0f)
		model = glm::rotate(model, glm::radians(rotationAngle.z), zAxis);
	model = glm::scale(model, scale);
	shader.setMat4("model", model);
	model2R.Draw(shader);
	return tempPos;
}
