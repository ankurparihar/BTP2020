// OpenGL.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// #include "pch.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "BaseStation.h"
#include "PicoStation.h"
#include "MobileStation.h"
#include "Utility.h"

#include "src/imgui/imgui.h"
#include "src/imgui/imgui_impl_glfw.h"
#include "src/imgui/imgui_impl_opengl3.h"

int main()
{
	/* Seed random generator */
	srand((unsigned int)time(NULL));

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL tutorial", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Initialize GLEW */
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	/* Initialize ImGui layer */
	const char* glsl_version = "#version 330";
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Read 'misc/fonts/README.txt' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != NULL);

	// Our state
	bool show_demo_window = true;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	/* Print OpenGL version */
	std::cout << glGetString(GL_VERSION) << ' ' << glGetString(GL_VENDOR) << std::endl;

	/* Generate randomly located base stations */
	std::vector<BaseStation> baseStations = generateBaseStations(NUM_BASE, WINDOW_WIDTH, WINDOW_HEIGHT, BORDER);
	/* Generate randomly located pico stations */
	std::vector<PicoStation> picoStations = generatePicoStations(NUM_PICO, WINDOW_WIDTH, WINDOW_HEIGHT, BORDER);
	/* Generate randomly located mobile stations */
	std::vector<MobileStation> mobileStations = generateMobileStations(NUM_MOBILE, WINDOW_WIDTH, WINDOW_HEIGHT, BORDER);
	/* Generate connection matrix [Station x Mobile] */
	/* All are disconnected (0) initially */
	std::vector<std::vector<int>> connections = generateGrid(NUM_BASE + NUM_PICO, NUM_MOBILE);

	float positions[6] = {
		-0.5f, -0.5f,
		 0.0f,  0.5f,
		 0.5f, -0.5f
	};

	float basePos[NUM_BASE * 2 * 4], picoPos[NUM_PICO * 2 * 4], mobilePos[NUM_MOBILE * 2 * 4];

	for (int i = 0; i < NUM_BASE; ++i) {
		int x = baseStations[i].location.x;
		int y = baseStations[i].location.y;
		float r = BASE_STATION_RADIUS;
		basePos[0 + (i * 8)] = (x - r);
		basePos[1 + (i * 8)] = (y - r);
		basePos[2 + (i * 8)] = (x - r);
		basePos[3 + (i * 8)] = (y + r);
		basePos[4 + (i * 8)] = (x + r);
		basePos[5 + (i * 8)] = (y - r);
		basePos[6 + (i * 8)] = (x + r);
		basePos[7 + (i * 8)] = (y + r);
	}

	for (int i = 0; i < NUM_MOBILE; ++i) {
		int x = mobileStations[i].location.x;
		int y = mobileStations[i].location.y;
		float r = MOBILE_STATION_RADIUS;
		mobilePos[0 + (i * 8)] = (x - r);
		mobilePos[1 + (i * 8)] = (y - r);
		mobilePos[2 + (i * 8)] = (x - r);
		mobilePos[3 + (i * 8)] = (y + r);
		mobilePos[4 + (i * 8)] = (x + r);
		mobilePos[5 + (i * 8)] = (y - r);
		mobilePos[6 + (i * 8)] = (x + r);
		mobilePos[7 + (i * 8)] = (y + r);
	}

	for (int i = 0; i < NUM_PICO; ++i) {
		int x = picoStations[i].location.x;
		int y = picoStations[i].location.y;
		float r = PICO_STATION_RADIUS;
		picoPos[0 + (i * 8)] = (x - r);
		picoPos[1 + (i * 8)] = (y - r);
		picoPos[2 + (i * 8)] = (x - r);
		picoPos[3 + (i * 8)] = (y + r);
		picoPos[4 + (i * 8)] = (x + r);
		picoPos[5 + (i * 8)] = (y - r);
		picoPos[6 + (i * 8)] = (x + r);
		picoPos[7 + (i * 8)] = (y + r);
	}

	GLuint baseBuffer, mobileBuffer, picoBuffer;
	glGenBuffers(1, &baseBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, baseBuffer);
	glBufferData(GL_ARRAY_BUFFER, NUM_BASE * 2 * 4 * sizeof(float), basePos, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glGenBuffers(1, &picoBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, picoBuffer);
	glBufferData(GL_ARRAY_BUFFER, NUM_PICO * 2 * 4 * sizeof(float), picoPos, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glGenBuffers(1, &mobileBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mobileBuffer);
	glBufferData(GL_ARRAY_BUFFER, NUM_MOBILE * 2 * 4 * sizeof(float), mobilePos, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);


	ShaderProgramSource shaderSource = ParseShader("res/shaders/Basic.shader");
	unsigned int shader = CreateShader(shaderSource.VertexSource, shaderSource.FragmentSource);

	glEnableVertexAttribArray(0);

	glUseProgram(shader);

	int uniformLocation, stationColorUniformLocation, stationPerimUniformLocation, centerUniformLocation;
	uniformLocation = glGetUniformLocation(shader, "screenSize");
	glUniform2f(uniformLocation, WINDOW_WIDTH, WINDOW_HEIGHT);
	centerUniformLocation = glGetUniformLocation(shader, "center");
	stationColorUniformLocation = glGetUniformLocation(shader, "stationColor");
	stationPerimUniformLocation = glGetUniformLocation(shader, "stationRadius");

	/* Enable blending */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* Loop until user closes the window */
	while (!glfwWindowShouldClose(window)) {
		
		/* Render here */
		glClearColor(0.13f, 0.13f, 0.13f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		/* Draw Base Stations */
		glUniform4f(stationColorUniformLocation, BS_R, BS_G, BS_B, BS_A);
		glUniform1f(stationPerimUniformLocation, BASE_STATION_RADIUS);
		glBindBuffer(GL_ARRAY_BUFFER, baseBuffer);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
		for (int i = 0; i < NUM_BASE; ++i) {
			glUniform2f(centerUniformLocation, (float)(baseStations[i].location.x), (float)(baseStations[i].location.y));
			glDrawArrays(GL_TRIANGLES, i * 4, 3);
			glDrawArrays(GL_TRIANGLES, i * 4 + 1, 3);
		}
		/* Draw Pico Stations */
		glUniform4f(stationColorUniformLocation, PS_R, PS_G, PS_B, PS_A);
		glUniform1f(stationPerimUniformLocation, PICO_STATION_RADIUS);
		glBindBuffer(GL_ARRAY_BUFFER, picoBuffer);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
		for (int i = 0; i < NUM_PICO; ++i) {
			glUniform2f(centerUniformLocation, (float)(picoStations[i].location.x), (float)(picoStations[i].location.y));
			glDrawArrays(GL_TRIANGLES, i * 4, 3);
			glDrawArrays(GL_TRIANGLES, i * 4 + 1, 3);
		}
		/* Draw Mobile Stations */
		glBindBuffer(GL_ARRAY_BUFFER, mobileBuffer);
		glUniform4f(stationColorUniformLocation, MS_R, MS_G, MS_B, MS_A);
		glUniform1f(stationPerimUniformLocation, MOBILE_STATION_RADIUS);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
		for (int i = 0; i < NUM_MOBILE; ++i) {
			glUniform2f(centerUniformLocation, (float)(mobileStations[i].location.x), (float)(mobileStations[i].location.y));
			glDrawArrays(GL_TRIANGLES, i * 4, 3);
			glDrawArrays(GL_TRIANGLES, i * 4 + 1, 3);
		}

		ImGui::ShowDemoWindow(&show_demo_window);

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	/* Delete shader */
	glDeleteProgram(shader);
	/* Terminate ImGui */
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	/* Terminate GLFW */
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}