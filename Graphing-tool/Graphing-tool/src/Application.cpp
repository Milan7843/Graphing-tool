#include "Application.h"
#include <chrono>         // std::chrono::seconds

Application::Application(const int width, const int height, std::string function)
	: WIDTH(width), HEIGHT(height),
	function(function),

	// Making a camera
	camera(glm::vec3(-3.0f, 2.0f, -2.0f))
{
}

Application::~Application()
{
}

// Convert ImGui format vec4 to glm format vec3
glm::vec3 imGuiVec4ToGlmVec3(ImVec4 vec4)
{
	return glm::vec3(vec4.x, vec4.y, vec4.z);
}

int Application::Start()
{
	// Initialising and configuring GLFW
	initialiseGLFW();
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	// Creating a window
	GLFWwindow* window = createGLFWWindow(WIDTH, HEIGHT, "Graph");
	if (window == NULL) return -1;
	glfwMakeContextCurrent(window);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	// Initialising GLAD
	if (!initialiseGLAD()) return -1;

	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_DEPTH_TEST);

	// Setting the callback for window resizing
	Callbacks& callbacks = Callbacks::getInstance();
	callbacks.setCamera(&camera);

	glfwSetFramebufferSizeCallback(window, &Callbacks::framebuffer_size_callback);
	
	//Shader shader("./src/shaders/vertexShader.shader", "./src/shaders/fragmentShader.shader");
	Shader shader("src/shaders/vertexShader.shader", "src/shaders/fragmentShader.shader");
	Shader calculatorShader(function, "src/shaders/calculatorVertexShader.shader", "src/shaders/calculatorFragmentShader.shader", false);
	ComputeShader meshGeneratorShader("src/shaders/meshGenerator.shader");
	ComputeShader calculatorComputeShader(function, "src/shaders/calculatorComputeShader.shader", false);

	// Creating our vertex array object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	generateGridMesh(&meshGeneratorShader);

	// Telling OpenGL how to interpret the data (only position data for now)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	// Creating a buffer for the heights to go into
	unsigned int heightsSSBO = 0;
	glGenBuffers(1, &heightsSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, heightsSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size * size * sizeof(float), 0, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, heightsSSBO);

	calculatorShader.setFloat("offset", 2.0f / (float)(size - 1.0f));
	calculatorShader.setInt("size", size);

	calculate(&calculatorComputeShader, heightsSSBO);

	unsigned int axesVAO = generateAxesVAO();


	// ImGui state
	std::string functionInput{ function };

	float timeSinceGuiSwitch = 10.0f;

	// Custom settings
	float verticalScale = 1.0f;
	unsigned int guiSwitchKeyPreviousState = 0;
	bool imGuiEnabled = true;
	bool showAdditionalInfo = false;

	// View modes
	bool smoothMesh = false;
	bool wireframe = false;

	// Calculation mode
	int calculationMode = 0;

	// Function input error log
	bool functionError = false;
	bool additionalFunctionErrorInfo = false;
	std::string functionErrorMessage = "";

	// Color customisation
	ImVec4 clearColor(0.09f, 0.05f, 0.11f, 1.0f);
	ImVec4 upperColor(0.0f, 0.0f, 1.0f, 1.0f);
	ImVec4 lowerColor(1.0f, 0.0f, 0.0f, 1.0f);


	// Render loop: runs while the window does not close
	while (!glfwWindowShouldClose(window))
	{
		// Checking for event input
		glfwPollEvents();

		// Calculating the heights (does not run if no important variables changed)
		calculate(&calculatorComputeShader, heightsSSBO);

		// Only move the camera if the GUI is not enabled
		if (!imGuiEnabled)
		{
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			camera.mouseCallback(window, xpos, ypos);

			// Also input handling
			camera.processInput(window, deltaTime);
		}
		
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Always check for window close on escape
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		// Enable/disable the ImGui GUI on key switch
		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && guiSwitchKeyPreviousState == GLFW_RELEASE)
		{
			imGuiEnabled = !imGuiEnabled;

			// If the GUI is enabled, 
			if (imGuiEnabled)
			{
				// free the mouse
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
			// Otherwise,
			else
			{
				// Lock the mouse
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				// Reset offset (the mouse may have moved which should not be registered as camera movement)
				camera.resetMouseOffset();
			}
		}
		guiSwitchKeyPreviousState = glfwGetKey(window, GLFW_KEY_F);
		
		/* RENDERING */
		
		// Drawing background
		glClearColor(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Drawing axes
		drawAxes(axesVAO, &shader, &camera);
		
		// Binding the shader program
		calculatorShader.use();
		
		// Setting uniforms
		float time = glfwGetTime();

		calculatorShader.setFloat("graphWidth", graphWidth);
		calculatorShader.setFloat("verticalScale", verticalScale);
		calculatorShader.setVector3("upperColor", imGuiVec4ToGlmVec3(upperColor));
		calculatorShader.setVector3("lowerColor", imGuiVec4ToGlmVec3(lowerColor));

		// Model matrix
		glm::mat4 model = glm::mat4(1.0f);
		calculatorShader.setMat4("model", model);
		
		// View matrix
		glm::mat4 view = glm::mat4(1.0f);
		view = camera.getViewMatrix();
		calculatorShader.setMat4("view", view);
		
		// Projection matrix
		glm::mat4 projection;
		projection = camera.getProjectionMatrix(WIDTH, HEIGHT);
		calculatorShader.setMat4("projection", projection);


		// Binding vertex array
		glBindVertexArray(VAO);

		// Binding the VAO if not in wireframe mode
		if (!wireframe)
		{
			// Drawing with colour
			calculatorShader.setBool("edgeMode", false);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawElements(GL_TRIANGLES, (size - 1) * (size - 1) * 6, GL_UNSIGNED_INT /* index type */, 0);
		}
		
		// Drawing edges if not in 'smooth' mode
		if (!smoothMesh || wireframe)
		{
			calculatorShader.setBool("edgeMode", true);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glLineWidth(1.0f);
			glDrawElements(GL_TRIANGLES, (size - 1) * (size - 1) * 6, GL_UNSIGNED_INT /* index type */, 0);
		}

		calculatorShader.setInt("calculationMode", calculationMode);
		//calculatorShader.setDouble("offset", 2.0 / (double)(size - 1.0));
		calculatorShader.setFloat("offset", 2.0f / (float)(size - 1.0f));
		calculatorShader.setInt("size", size);

		// Unbinding vertex array
		glBindVertexArray(0);


		/* FINALIZING */
		
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		// Creating the GUI window
		if (imGuiEnabled)
		{		
			ImGui::Begin("Graphing settings");
		
			ImGui::Text("Press F to return to graph view, \nwhere you can look around.");

			ImGui::InputText("Function", &functionInput);
			if (ImGui::Button("Set function"))
			{
				try
				{
					Shader _calculatorShader(functionInput, "src/shaders/calculatorVertexShader.shader", "src/shaders/calculatorFragmentShader.shader", true);
					calculatorShader = _calculatorShader;
				}
				catch (std::exception e)
				{
					functionError = true;
					// Convert to string in order to preserve it
					functionErrorMessage = std::string(e.what());
				}
			}

			// Function error
			if (functionError)
			{
				// Generic error message
				ImGui::TextColored(ImVec4(0.8f, 0.15f, 0.15f, 1.0f), "Function error!");

				// Showing more info
				if (ImGui::CollapsingHeader("Additional error information"))
				{
					ImGui::Text(functionErrorMessage.c_str());
				}
				if (ImGui::Button("Close error"))
				{
					functionError = false;
				}
			}

			// Information about the possible functions
			if (ImGui::CollapsingHeader("Function help"))
			{
				ImGui::Text("Available functions:");
				ImGui::BulletText("sin(x)    -> sine of x");
				ImGui::BulletText("cos(x)    -> cosine of x");
				ImGui::BulletText("tan(x)    -> tangent of x");
				ImGui::BulletText("asin(x)   -> arcsine of x");
				ImGui::BulletText("acos(x)   -> arccosine of x");
				ImGui::BulletText("atan(x)   -> arctangent of x");
				ImGui::BulletText("exp(x)    -> e to the power x");
				ImGui::BulletText("pow(x, z) -> x to the power z");
				ImGui::BulletText("sqrt(x)   -> square root of x");
				ImGui::BulletText("abs(x)    -> absolute value of x");
				ImGui::BulletText("floor(x)  -> floor of x (round down)");
				ImGui::BulletText("ceil(x)   -> ceil of x (round up)");
				ImGui::BulletText("min(x, z) -> minimum of x and z");
				ImGui::BulletText("max(x, z) -> maximum of x and z");

				ImGui::Separator();

				ImGui::Text("Available operators:");
				ImGui::BulletText("+ for addition");
				ImGui::BulletText("- for subtraction");
				ImGui::BulletText("/ for division");
				ImGui::BulletText("* for multiplication");

				ImGui::Separator();

				ImGui::Text("Available constants:");
				ImGui::BulletText("pi");
				ImGui::BulletText("e");

				ImGui::Separator();

				ImGui::Text("Use variables x and z for inputs.");
				ImGui::Text("x is represented by the red axis, z by the blue axis.");
				ImGui::Text("All trigonometric functions \ntake radians as input/output.");

				ImGui::Separator();
			}

			// Graph settings
			ImGui::SliderFloat("Scale", &scale, 0.1f, 10.0f);
			ImGui::SliderFloat("Vertical scale", &verticalScale, 0.1f, 10.0f);
			ImGui::SliderFloat("Graph width", &graphWidth, 0.1f, 10.0f);

			if (ImGui::Button(smoothMesh ? "Disable smooth mode" : "Enable smooth mode"))
			{
				smoothMesh = !smoothMesh;
			}
			if (ImGui::Button(wireframe ? "Disable wireframe mode" : "Enable wireframe mode"))
			{
				wireframe = !wireframe;
			}


			// Additional information
			if (showAdditionalInfo)
			{
				if (ImGui::Button("Hide additional information"))
				{
					showAdditionalInfo = false;
				}

				// Graph bounds
				std::string bounds = std::to_string(scale * graphWidth);
				std::string boundsInfo = "Current bounds:\nx: [-" + bounds + ", " + bounds + "], z: [-" + bounds + ", " + bounds + "]";
				ImGui::Text(boundsInfo.c_str());

				// Camera position
				std::string cameraPosition = "Camera position: ("
					+ std::to_string(camera.getPosition().x) + ", "
					+ std::to_string(camera.getPosition().y) + ", "
					+ std::to_string(camera.getPosition().z) + ")";
				ImGui::Text(cameraPosition.c_str());

				// Camera rotation
				std::string cameraRotation = "Camera pitch: "
					+ std::to_string(camera.getPitch()) + ", yaw: " + std::to_string(camera.getYaw());
				ImGui::Text(cameraRotation.c_str());
			}
			else
			{
				// Button to show additionial information (camera position/rotation and more)
				if (ImGui::Button("Show additional information"))
				{
					showAdditionalInfo = true;
				}
			}

			ImGui::Text("Calculation mode");
			ImGui::RadioButton("Regular", &calculationMode, 0);
			ImGui::RadioButton("Derivative", &calculationMode, 1);
			//ImGui::RadioButton("Integral", &calculationMode, 2);

			// Camera settings (speed, fov etc.)
			if (ImGui::CollapsingHeader("Camera settings"))
			{
				ImGui::SliderFloat("Move speed", camera.getCameraSpeedPointer(), 0.1f, 10.0f);
				ImGui::SliderFloat("Sensitivity", camera.getSensitivityPointer(), 0.1f, 5.0f);
				ImGui::SliderFloat("Field of view", camera.getFovPointer(), 10.0f, 90.0f);
			}

			// Customisation of the program (colors etc.)
			if (ImGui::CollapsingHeader("Customisation"))
			{
				ImGui::ColorEdit3("Background colour", (float*)&clearColor);
				ImGui::ColorEdit3("Upper graph colour", (float*)&upperColor);
				ImGui::ColorEdit3("Lower graph colour", (float*)&lowerColor);
			}

			ImGui::End();
		}
		
		// Rendering
		ImGui::Render();
		//int display_w, display_h;
		//glfwGetFramebufferSize(window, &display_w, &display_h);
		//glViewport(0, 0, display_w, display_h);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		// Showing the current color buffer to the screen
		glfwSwapBuffers(window);
	}


	// ImGui cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Terminating GLFW
	glfwTerminate();
	return 0;
}

// Initialises and configures GLFW
void Application::initialiseGLFW()
{
	// Initialises GLFW
	glfwInit();
	// Setting OpenGL version 4.6
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	// OpenGL profile: core
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Application::generateGrid(std::vector<float>* vertices, int size)
{
	float offset = 2.0f / (float)(size - 1);
	for (int i = 0; i < size * size; i++)
	{
		int cx = i % size;
		int cy = i / size;

		vertices->at(i * 3 + 0) = (cx * offset - 1.0f); // x
		vertices->at(i * 3 + 1) = 0.0f; // y
		vertices->at(i * 3 + 2) = (cy * offset - 1.0f); // z
	}
}
void Application::generateGridIndices(std::vector<unsigned int>* indices, int x, int y)
{
	for (int i = 0; i < (x - 1) * (y - 1); i++)
	{
		int cx = i % (y - 1);
		int cy = i / (x - 1);

		// 0 1
		// 2 3
		// Right tri: 0 1 3
		indices->at(i * 6 + 0) = cx + cy * x; // 0
		indices->at(i * 6 + 1) = cx + cy * x + 1; // 1
		indices->at(i * 6 + 2) = cx + cy * x + x + 1; // 3
		// Left tri: 0 3 2
		indices->at(i * 6 + 3) = cx + cy * x; // 0
		indices->at(i * 6 + 4) = cx + cy * x + x + 1; // 3
		indices->at(i * 6 + 5) = cx + cy * x + x; // 2
	}
}
void Application::generateGridGPU(ComputeShader* computeShader, std::vector<float>* vertices, std::vector<unsigned int>* indices, int size)
{
	// Generating the grid vertices and indices on the GPU

	// Assigning the compute shader
	computeShader->use();

	computeShader->setInt("size", size);
	computeShader->setFloat("offset", 2.0f / (float)(size - 1.0f));

	// Creating a buffer for the vertices to go into
	unsigned int verticesSSBO = 0;
	glGenBuffers(1, &verticesSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, verticesSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size * size * 3 * sizeof(float), 0, GL_DYNAMIC_COPY);

	// Creating a buffer for the indices to go into
	unsigned int indicesSSBO = 0;
	glGenBuffers(1, &indicesSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, indicesSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, (size - 1) * (size - 1) * 6 * sizeof(unsigned int), 0, GL_DYNAMIC_COPY);

	// Bind to slot 0 (vertices) and 1 (indices)
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, verticesSSBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, indicesSSBO);

	// Running the compute shader
	glDispatchCompute(size, size, 1);
	//glDispatchCompute(std::ceil(float(size / 8.0f)), std::ceil(float(size / 8.0f)), 1);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);

	// Reading the data back from the compute shader
	glGetNamedBufferSubData(verticesSSBO, 0, size * size * 3 * sizeof(float), vertices->data());
	glGetNamedBufferSubData(indicesSSBO, 0, (size - 1) * (size - 1) * 6 * sizeof(unsigned int), indices->data());
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

// Initialises GLAD
bool Application::initialiseGLAD()
{
	// Initialising GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		// Failed to initialise: return false
		std::cout << "Failed to initialise GLAD." << std::endl;
		return false;
	}
	// Initialisation success: return true
	return true;
}

// Creates a window using GLFW
GLFWwindow* Application::createGLFWWindow(int width, int height, const char* name)
{
	// Creating a window
	GLFWwindow* window = glfwCreateWindow(width, height, name, NULL, NULL);

	// Check if window was created succesfully
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window." << std::endl;
		return NULL;
	}
	return window;
}

unsigned int Application::generateAxesVAO()
{
	// Creating our vertex array object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Putting the vertices into the array buffer
	float vertices[] = {
		// Position
	   -1000.0f, 0.0f, 0.0f,
		1000.0f, 0.0f, 0.0f
	};
	unsigned int indices[] = {
		0, 1
	};

	unsigned int VBO;
	// Making a buffer with the ID in VBO
	glGenBuffers(1, &VBO);
	// Binding our new buffer to the GL_ARRAY_BUFFER target
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Binding our custom data into the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Binding the element buffer object
	unsigned int EBO;
	// Generating a buffer for the EBO
	glGenBuffers(1, &EBO);
	// Binding the EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// Inserting data into the buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// Telling OpenGL how to interpret the data
	// Position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	return VAO;
}

void Application::drawAxes(unsigned int VAO, Shader* shader, Camera* camera)
{
	glLineWidth(0.01f);
	float s = 1.0f;

	shader->use();

	// Model matrix
	glm::mat4 model = glm::mat4(1.0f);
	shader->setMat4("model", model);

	// View matrix
	glm::mat4 view = glm::mat4(1.0f);
	view = camera->getViewMatrix();
	shader->setMat4("view", view);

	// Projection matrix
	glm::mat4 projection;
	projection = camera->getProjectionMatrix(WIDTH, HEIGHT);
	shader->setMat4("projection", projection);

	// Binding the VAO
	glBindVertexArray(VAO);

	// Drawing the x-axis (red)
	shader->setVector3("color", 194 / 255.0f, 31 / 255.0f, 19 / 255.0f);
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT /* index type */, 0);

	// Drawing the y-axis (up) (green)
	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader->setMat4("model", model);
	shader->setVector3("color", 29 / 255.0f, 194 / 255.0f, 57 / 255.0f);
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT /* index type */, 0);

	// Drawing the z-axis (blue)
	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader->setMat4("model", model);
	shader->setVector3("color", 19 / 255.0f, 37 / 255.0f, 194 / 255.0f);
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT /* index type */, 0);
	glBindVertexArray(0);
}

void Application::calculate(ComputeShader* computeShader, unsigned int ssbo)
{
	// Calculating the heights of each point on the GPU using a compute shader

	// Setting the changed variables
	if (graphWidth == generatedGraphWidth &&
		scale == generatedScale)
	{
		// No important changed variables: do not run
		std::cout << "Not running calculation" << std::endl;
		return;
	}
	std::cout << "Running calculation" << std::endl;
	generatedGraphWidth = graphWidth;
	generatedScale = scale;

	// Assigning the compute shader
	computeShader->use();

	computeShader->setFloat("scale", scale);
	computeShader->setFloat("graphWidth", graphWidth);
	computeShader->setInt("size", size);
	computeShader->setFloat("offset", 2.0f / (float)(size - 1));

	// Bind to slot 2 (heights)
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssbo);

	// Running the compute shader
	glDispatchCompute(size, size, 1);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);

	// Unbinding the buffer
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void Application::generateGridMesh(ComputeShader* generatorComputeShader)
{
	// CPU generated vertices
	std::vector<float> vertices(size * size * 3);
	std::vector<unsigned int> indices((size - 1) * (size - 1) * 6);

	std::chrono::steady_clock::time_point gpu_begin = std::chrono::steady_clock::now();
	// With a size of 5000, the GPU generator is almost 10 times as fast!
	// size=3200 -> t=~280ms
	generateGridGPU(generatorComputeShader, &vertices, &indices, size);
	std::chrono::steady_clock::time_point gpu_end = std::chrono::steady_clock::now();

	std::cout << "GPU mesh gen. time = " << std::chrono::duration_cast<std::chrono::microseconds>(gpu_end - gpu_begin).count() << " microseconds" << std::endl;

	unsigned int VBO;
	// Making a buffer with the ID in VBO
	glGenBuffers(1, &VBO);
	// Binding our new buffer to the GL_ARRAY_BUFFER target
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Binding our custom data into the buffer
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	// Binding the element buffer object
	unsigned int EBO;
	// Generating a buffer for the EBO
	glGenBuffers(1, &EBO);
	// Binding the EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// Inserting data into the buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}