/*---------------------------------------------------------*/
/* ------------      Proyecto Final             -----------*/
/*-------------        2022-2   ---------------------------*/
/*------------- Alumno: Santiago Escobar    ---------------*/
/*------------- No. Cuenta : 722030330      ---------------*/
#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture

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
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);
//glm::mat4 drawTiling(glm::vec3 offset, glm::vec3 scale, Shader shader, glm::mat4 origin, Model model2R);
glm::mat4 drawObject(glm::vec3 offset, glm::vec3 scale, Shader shader, glm::mat4 origin, Model model2R);
glm::mat4 drawObject(glm::vec3 offset, glm::vec3 rotationAxis, float rotationAngle, glm::vec3 scale, Shader shader, glm::mat4 origin, Model model2R);
// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor *monitors;

void getResolution(void);

// camera
Camera camera(glm::vec3(0.0f, 100.0f, 90.0f));
float MovementSpeed = 0.1f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);


// Auto
float	movAuto_x = 0.0f,
		movAuto_z = 0.0f,
		orienta = 0.0f;
bool	animacion = false,
		recorrido1 = true,
		recorrido2 = false,
		recorrido3 = false,
		recorrido4 = false;

// Music
bool music = false;


//Object location
// 
// Origin
glm::mat4 originWorld = glm::mat4(1.0f);

//rotation
glm::vec3 yAxis = glm::vec3(0.0f, 1.0f, 0.0f);

// Floor tiling
float floorScale = 2.0f;
float floorTilingSpacing = 100.0f * floorScale; 
float floorYOffset = -1.0f;
int floorLimitX = 4;
int floorLowerLimitZ = -2;
int floorUpperLimitZ = 2;

// Fence
float fenceOffsetX = -floorTilingSpacing / 2;
float fenceScale = 15.0f;
float fenceSize = 2.436f;
float fenceTilingSpacing = fenceSize * fenceScale;
float fenceUpperLimitZ = (floorUpperLimitZ * floorTilingSpacing) - (floorTilingSpacing / 2) - (fenceTilingSpacing / 2);
float fenceLowerLimitZ = (floorLowerLimitZ * floorTilingSpacing) - (floorTilingSpacing / 2) + (fenceTilingSpacing/2);
float fenceNumber = (floorTilingSpacing * (floorUpperLimitZ - floorLowerLimitZ + 1)) / fenceTilingSpacing ;

//Volcano
float volcanoScale = 5.0f;
glm::vec3 volcanoSize = glm::vec3(37.81f * volcanoScale, 37.81f * volcanoScale, 13.978f * volcanoScale);
glm::vec3 volcanoPosition = glm::vec3((-floorLimitX*floorTilingSpacing) + (volcanoSize.x), floorYOffset + (volcanoSize.z/2) , 0.0f);

//Arlo
float arloScale = 5.0f * 10.0f;
glm::vec3 arloSize = glm::vec3(0.315f * arloScale, 0.995f * arloScale, 1.29f * arloScale);

//Tree
float treeScale = 3.0f * 7.0f;
glm::vec3 treeSize = glm::vec3(1.36f * treeScale, 1.43f * treeScale, 3.42 * treeScale);

void animate(void)
{
	//Vehículo
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

	//Debug
	std::cout << fenceNumber << std::endl;


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

	vector<std::string> faces
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	// -----------
	Model piso("resources/objects/piso/tile.obj");
	Model pisoPasto("resources/objects/piso/pasto.obj");
	Model carro("resources/objects/lambo/carroceria.obj");
	Model llanta("resources/objects/lambo/Wheel.obj");
	Model volcano("resources/objects/volcano/volcano2.obj");
	Model fence("resources/objects/fence/fence.obj");
	Model cube10("resources/objects/unitcube/unitCube.obj");	
	Model stadium("resources/objects/stadium/stadium.obj");
	Model cuelloLargo("resources/objects/dinosaurs/cuellolargo/arlo.obj");
	Model tree("resources/objects/tree/tree.obj");	
	Model tRex("resources/objects/dinosaurs/t-rex/t-rex.obj");
	


	ModelAnim animacionPersonaje("resources/objects/Personaje1/PersonajeBrazo.dae");
	animacionPersonaje.initShaders(animShader.ID);
		
	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//Setup Advanced Lights
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
		//Remember to activate the shader with the animation
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

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-40.3f, 1.75f, 0.3f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.2f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		animacionPersonaje.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario + Piso
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);
		

		for (int i = 0; i < floorLimitX; i++) {
			for (int j = floorLowerLimitZ; j <= floorUpperLimitZ; j++) {
				drawObject(glm::vec3((floorTilingSpacing * static_cast<float>(i)), floorYOffset, (floorTilingSpacing* static_cast<float>(j))), glm::vec3(floorScale), staticShader, originWorld, piso);
				drawObject(glm::vec3((-floorTilingSpacing * static_cast<float>(i)), floorYOffset, (floorTilingSpacing * static_cast<float>(j))), glm::vec3(floorScale), staticShader, originWorld , pisoPasto);
			}
		}

		// -------------------------------------------------------------------------------------------------------------------------
		// Fence
		// -------------------------------------------------------------------------------------------------------------------------
		// Simulate doors 
		/*Pareto
			//Left gate
			tmp = drawObject(glm::vec3(fenceOffsetX, floorYOffset, 0.0f), yAxis, -45.0f, glm::vec3(fenceScale), staticShader, glm::mat4(1.0f), fenceDoor);
			//Right gate
			drawObject(glm::vec3(fenceOffsetX, floorYOffset, 0.0f), yAxis, -225.0f, glm::vec3(fenceScale), staticShader, glm::mat4(1.0f), fenceDoor);
		*/	

		//tmp = drawObject(glm::vec3(fenceOffsetX, floorYOffset, fenceLowerLimitZ), yAxis, -90.0f, glm::vec3(fenceScale), staticShader, originWorld, fence);
		//for (int i = 1; i <= fenceNumber; i++) {
			//tmp = drawObject(glm::vec3(0.0f, 0.0f, fenceTilingSpacing), yAxis, -90.0f, glm::vec3(fenceScale), staticShader, tmp, fence);
		//}
		// -------------------------------------------------------------------------------------------------------------------------
		// Volcan
		// -------------------------------------------------------------------------------------------------------------------------
		

		// Funcionales
		// drawObject(volcanoPosition, glm::vec3(volcanoScale) , staticShader, originWorld, volcano);
		//drawObject(glm::vec3(0.0f), glm::vec3(4.0f), staticShader, originWorld, stadium);
		//drawObject(glm::vec3(-10.0f, 0.0f, 0.0f), glm::vec3(treeScale), staticShader, originWorld, tree);		
		//drawObject(glm::vec3(0.0f), glm::vec3(arloScale), staticShader, originWorld, cuelloLargo);
		//drawObject(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(1.0f), staticShader, originWorld, tRex);

		//Pruebas

		tmp = drawObject(glm::vec3(0.0f), glm::vec3(1.0f), staticShader, originWorld, cube10);
		for(int i = 1; i< 10; i++){
			tmp = drawObject(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(1.0f), staticShader, tmp , cube10);
		}

		

		

		

	
		// -------------------------------------------------------------------------------------------------------------------------
		// Carro
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(15.0f + movAuto_x, -1.0f, movAuto_z));
		tmp = model = glm::rotate(model, glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		carro.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(8.5f, 2.5f, 12.9f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Izq delantera

		model = glm::translate(tmp, glm::vec3(-8.5f, 2.5f, 12.9f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Der delantera

		model = glm::translate(tmp, glm::vec3(-8.5f, 2.5f, -14.5f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Der trasera

		model = glm::translate(tmp, glm::vec3(8.5f, 2.5f, -14.5f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Izq trase
		
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
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
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
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
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

	//Car animation
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


glm::mat4 drawObject(glm::vec3 offset, glm::vec3 scale, Shader shader, glm::mat4 origin, Model model2R) {
	glm::mat4 model = glm::translate(origin, offset);
	glm::mat4 tempPos = model;
	model = glm::scale(model, scale);
	shader.setMat4("model", model);
	model2R.Draw(shader);
	return tempPos;
}
glm::mat4 drawObject(glm::vec3 offset, glm::vec3 rotationAxis, float rotationAngle, glm::vec3 scale, Shader shader, glm::mat4 origin, Model model2R) {
	glm::mat4 model = glm::translate(origin, offset);
	glm::mat4 tempPos = model;
	model = glm::rotate(model, glm::radians(rotationAngle), rotationAxis);
	model = glm::scale(model, scale);
	shader.setMat4("model", model);
	model2R.Draw(shader);
	return tempPos;
}
