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
	
	/* Some properties */
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);	// Non resizable window
	glfwWindowHint(GLFW_SAMPLES, 4);			// Hint GLFW to allocate Multi-sampling Anti-aliasing buffers
	glEnable(GL_MULTISAMPLE);					// Enable MSAA

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Cell association simulator", NULL, NULL);
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
	const char* glsl_version = "#version 450";
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
	std::vector<std::vector<Station*>> connections = generateGrid(NUM_BASE, NUM_PICO, NUM_MOBILE);


	float basePos[NUM_BASE * 2 * 4], picoPos[NUM_PICO * 2 * 4], mobilePos[NUM_MOBILE * 2 * 4];

	float linePos[4] = {
		0.0f, 0.0f,		// line segment start x, y
		0.0f, 0.0f		// line segment end   x, y
	};

	GLuint baseBuffer, mobileBuffer, picoBuffer, lineBuffer;
	glGenBuffers(1, &baseBuffer);
	glGenBuffers(1, &picoBuffer);
	glGenBuffers(1, &mobileBuffer);
	glGenBuffers(1, &lineBuffer);

	ShaderProgramSource shaderSource = ParseShader("res/shaders/StationPlotter.shader");
	unsigned int StationPlotterShader = CreateShader(shaderSource.VertexSource, shaderSource.FragmentSource);
	shaderSource = ParseShader("res/shaders/StationConnector.shader");
	unsigned int StationConnectorShader = CreateShader(shaderSource.VertexSource, shaderSource.FragmentSource);

	glEnableVertexAttribArray(0);

	glUseProgram(StationPlotterShader);
	/* StationPlotter attributes */
	int uniformLocation, stationColorUniformLocation, stationPerimUniformLocation, centerUniformLocation;
	uniformLocation = glGetUniformLocation(StationPlotterShader, "screenSize");
	glUniform2f(uniformLocation, WINDOW_WIDTH, WINDOW_HEIGHT);
	centerUniformLocation = glGetUniformLocation(StationPlotterShader, "center");
	stationColorUniformLocation = glGetUniformLocation(StationPlotterShader, "stationColor");
	stationPerimUniformLocation = glGetUniformLocation(StationPlotterShader, "stationRadius");

	/* StationConnector attributes */
	// uniformLocation = glGetUniformLocation(StationConnectorShader, "screenSize");
	// glUniform2f(uniformLocation, WINDOW_WIDTH, WINDOW_HEIGHT);
	unsigned int lineBeginCenterUniformLocation = glGetUniformLocation(StationConnectorShader, "center");
	unsigned int lineLengthCenterUniformLocation = glGetUniformLocation(StationConnectorShader, "len");

	/* Enable blending */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// which method to run initially
	METHOD = STARTUP_METHOD;

	// Generate x_data for output visualization
	// for (int i = 0; i < sampleSize; ++i) {
	// 	x_data[i] = ((float)i) / 10;
	// }

	// Output constant parameters
	printf(
		"============================ Parameters ============================\n"
		"Number of Base Stations   :     %d\n"
		"Number of Pico Stations   :     %d\n"
		"Number of Mobile Stations :     %d\n"
		"Base Station Capacity     :     %d\n"
		"Pico Station Capacity     :     %d\n"
		"Base Station Power        :     %d W\n"
		"Pico Station Power        :     %d W\n"
		"Bandwidth                 :     %d MHz\n"
		"Area Dimensions           :     %d m x %d m\n"
		"====================================================================\n\n",
		NUM_BASE, NUM_PICO, NUM_MOBILE,
		BASE_STATION_CAPACITY, PICO_STATION_CAPACITY,
		BASE_POWER, PICO_POWER,
		(int)BANDWIDTH,
		WINDOW_WIDTH * SCALE, WINDOW_HEIGHT * SCALE
	);

	/* Loop until user closes the window */
	while (!glfwWindowShouldClose(window)) {

		// check if there is change in method of connections
		if (refresh) {
			refresh = false;
			if (randomize) {
				printf("============================ Randomized ============================\n");
				randomize = false;
				/* Generate randomly located base stations */
				baseStations = generateBaseStations(NUM_BASE, WINDOW_WIDTH, WINDOW_HEIGHT, BORDER);
				/* Generate randomly located pico stations */
				picoStations = generatePicoStations(NUM_PICO, WINDOW_WIDTH, WINDOW_HEIGHT, BORDER);
				/* Generate randomly located mobile stations */
				mobileStations = generateMobileStations(NUM_MOBILE, WINDOW_WIDTH, WINDOW_HEIGHT, BORDER);
				/* Generate connection matrix [Station x Mobile] */
				/* All are disconnected (0) initially */
				connections = generateGrid(NUM_BASE, NUM_PICO, NUM_MOBILE);

				for (int i = 0; i < NUM_BASE; ++i) {
					int x = baseStations[i].location.x / SCALE;
					int y = baseStations[i].location.y / SCALE;
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
					int x = mobileStations[i].location.x / SCALE;
					int y = mobileStations[i].location.y / SCALE;
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
					int x = picoStations[i].location.x / SCALE;
					int y = picoStations[i].location.y / SCALE;
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
				glBindBuffer(GL_ARRAY_BUFFER, baseBuffer);
				glBufferData(GL_ARRAY_BUFFER, NUM_BASE * 2 * 4 * sizeof(float), basePos, GL_STATIC_DRAW);
				glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
				glBindBuffer(GL_ARRAY_BUFFER, picoBuffer);
				glBufferData(GL_ARRAY_BUFFER, NUM_PICO * 2 * 4 * sizeof(float), picoPos, GL_STATIC_DRAW);
				glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
				glBindBuffer(GL_ARRAY_BUFFER, mobileBuffer);
				glBufferData(GL_ARRAY_BUFFER, NUM_MOBILE * 2 * 4 * sizeof(float), mobilePos, GL_STATIC_DRAW);
				glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
				glBindBuffer(GL_ARRAY_BUFFER, lineBuffer);
				glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float), linePos, GL_STATIC_DRAW);
				glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

				// Reset histograms
				for (int i = 0; i < sampleSize; ++i) {
					// avgThr[i] = 0.0;
					biasEffect[i] = 0.0;
					// biasEffectK[0][i] = 0.0;
					// biasEffectK[1][i] = 0.0;
					// biasEffectK[2][i] = 0.0;
					// biasEffectK[3][i] = 0.0;
				}
			}

			/* Remove all connections */
			disconnect(mobileStations, baseStations, picoStations);
			/* Update stations' attributes */
			reconfigure(mobileStations, baseStations, picoStations);
			/* Connect mobiles to stations */
			connect(mobileStations, baseStations, picoStations, METHOD);

			/* Print connections : cool xD*/
			if (printConnections) {
				for (MobileStation mobile : mobileStations) {
					std::cout << mobile << " -> ";
					if (mobile.connected) std::cout << *(mobile.station) << std::endl;
					else std::cout << "NULL" << std::endl;
				}
			}
		}

		/* Render here */
		glClearColor(0.13f, 0.13f, 0.13f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		/* Draw connections first */
		glUseProgram(StationConnectorShader);
		glBindBuffer(GL_ARRAY_BUFFER, lineBuffer);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_TRUE, 2 * sizeof(float), 0);
		for (MobileStation& mobile : mobileStations) {
			if (mobile.connected) {
				linePos[0] = (float)toGLX(mobile.location.x / SCALE);
				linePos[1] = (float)toGLY(mobile.location.y / SCALE);
				linePos[2] = (float)toGLX(mobile.station->location.x / SCALE);
				linePos[3] = (float)toGLY(mobile.station->location.y / SCALE);
				glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float), linePos, GL_STATIC_DRAW);
				glUniform2f(lineBeginCenterUniformLocation, (float)(mobile.location.x / SCALE), (float)(mobile.location.y / SCALE));
				glUniform1f(lineLengthCenterUniformLocation, (float)(mobile.location.distance(mobile.station->location) / SCALE));
				glDrawArrays(GL_LINES, 0, 2);
			}
		}

		glUseProgram(StationPlotterShader);
		/* Draw Base Stations */
		glUniform4f(stationColorUniformLocation, BS_R, BS_G, BS_B, BS_A);
		glUniform1f(stationPerimUniformLocation, BASE_STATION_RADIUS);
		glBindBuffer(GL_ARRAY_BUFFER, baseBuffer);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
		for (int i = 0; i < NUM_BASE; ++i) {
			glUniform2f(centerUniformLocation, (float)(baseStations[i].location.x / SCALE), (float)(baseStations[i].location.y / SCALE));
			glDrawArrays(GL_TRIANGLES, i * 4, 3);
			glDrawArrays(GL_TRIANGLES, i * 4 + 1, 3);
		}
		/* Draw Pico Stations */
		glUniform4f(stationColorUniformLocation, PS_R, PS_G, PS_B, PS_A);
		glUniform1f(stationPerimUniformLocation, PICO_STATION_RADIUS);
		glBindBuffer(GL_ARRAY_BUFFER, picoBuffer);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
		for (int i = 0; i < NUM_PICO; ++i) {
			glUniform2f(centerUniformLocation, (float)(picoStations[i].location.x / SCALE), (float)(picoStations[i].location.y / SCALE));
			glDrawArrays(GL_TRIANGLES, i * 4, 3);
			glDrawArrays(GL_TRIANGLES, i * 4 + 1, 3);
		}
		/* Draw Mobile Stations */
		glBindBuffer(GL_ARRAY_BUFFER, mobileBuffer);
		glUniform4f(stationColorUniformLocation, MS_R, MS_G, MS_B, MS_A);
		glUniform1f(stationPerimUniformLocation, MOBILE_STATION_RADIUS);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
		for (int i = 0; i < NUM_MOBILE; ++i) {
			glUniform2f(centerUniformLocation, (float)(mobileStations[i].location.x / SCALE), (float)(mobileStations[i].location.y / SCALE));
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
	glDeleteProgram(StationPlotterShader);
	/* Terminate ImGui */
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	/* Terminate GLFW */
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}