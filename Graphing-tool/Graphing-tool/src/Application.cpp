#include "Application.h"

Application::Application(const int width, const int height, std::string function, const float scrollSens)
	: WIDTH(width), HEIGHT(height),
	function(function),
	scrollSens(scrollSens),

	// Making a camera
	camera(glm::vec3(-3.0f, 2.0f, -2.0f))
{
}

Application::~Application()
{
}

int Application::Start()
{
	// Initialising and configuring GLFW
	initialiseGLFW();

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
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

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
	Shader calculatorShader(function, "src/shaders/calculatorVertexShader.shader", "src/shaders/calculatorFragmentShader.shader");
	// Creating our vertex array object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	const int size = 100;
	float vertices[size * size * 3];
	generateGrid(vertices, size, size);
	unsigned int indices[(size - 1) * (size - 1) * 6];
	generateGridIndices(indices, size, size);

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

	unsigned int axesVAO = generateAxesVAO();


	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, &Callbacks::mouseCallback);

	// ImGui state
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Render loop: runs while the window does not close
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		/* Input handling */

		camera.processInput(window, deltaTime);
		// Scale adjustment
		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
			scale += scrollSens * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
			scale -= scrollSens * deltaTime;
		if (scale < 0.1f)
			scale = 0.1f;
		if (scale > 10.0f)
			scale = 10.0f;

		/* RENDERING */

		// Drawing background
		glClearColor(0.09f, 0.05f, 0.11, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Drawing axes
		drawAxes(axesVAO, &shader, &camera);

		// Binding the shader program
		calculatorShader.use();

		// Setting uniforms
		float time = glfwGetTime();

		calculatorShader.setFloat("scale", scale);

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

		// Binding the VAO
		glBindVertexArray(VAO);
		// Drawing with colour
		calculatorShader.setBool("edgeMode", false);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, (size - 1) * (size - 1) * 6, GL_UNSIGNED_INT /* index type */, 0);

		// Drawing edges
		calculatorShader.setBool("edgeMode", true);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(1.0f);
		glDrawElements(GL_TRIANGLES, (size - 1) * (size - 1) * 6, GL_UNSIGNED_INT /* index type */, 0);
		glBindVertexArray(0);


		/* FINALIZING*/

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		// 3. Show another simple window.
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Showing the current color buffer to the screen
		glfwSwapBuffers(window);
		// Checking for event input
		glfwPollEvents();
	}

	// Terminating GLFW
	glfwTerminate();
	return 0;
}

// Initialises and configures GLFW
void Application::initialiseGLFW()
{
	// Initialises GLFW
	glfwInit();
	// Setting OpenGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// OpenGL profile: core
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Application::generateGrid(float in[], int x, int y)
{
	float xoffset = 2.0f / (float)(x - 1);
	float yoffset = 2.0f / (float)(y - 1);
	for (int i = 0; i < x * y; i++)
	{
		int cx = i % y;
		int cy = i / x;

		in[i * 3 + 0] = cx * xoffset - 1.0f; // x
		in[i * 3 + 1] = 0.0f; // y
		in[i * 3 + 2] = cy * yoffset - 1.0f; // z
	}
}
void Application::generateGridIndices(unsigned int in[], int x, int y)
{
	for (int i = 0; i < (x - 1) * (y - 1); i++)
	{
		int cx = i % (y - 1);
		int cy = i / (x - 1);

		// 0 1
		// 2 3
		// Right tri: 0 1 3
		in[i * 6 + 0] = cx + cy * x; // 0
		in[i * 6 + 1] = cx + cy * x + 1; // 1
		in[i * 6 + 2] = cx + cy * x + x + 1; // 3
		// Left tri: 0 3 2
		in[i * 6 + 3] = cx + cy * x; // 0
		in[i * 6 + 4] = cx + cy * x + x + 1; // 3
		in[i * 6 + 5] = cx + cy * x + x; // 2
	}
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
	   -1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f
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