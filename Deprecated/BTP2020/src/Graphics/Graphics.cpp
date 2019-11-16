#include "Graphics.h"

bool Graphics::init() {
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	else
	{
		//Create window
		window = SDL_CreateWindow("Nearest BaseStation allocation to MobileStation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			return false;
		}
		else
		{
			//Get window surface
			screenSurface = SDL_GetWindowSurface(window);

			//Fill the surface white
			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

			//Update the surface
			SDL_UpdateWindowSurface(window);

			//Create renderer for window
			gRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			}
		}
		return true;
	}
}

void Graphics::FillRect(Uint8 r, Uint8 g, Uint8 b) {
	SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, r, g, b));
}

void Graphics::wait(Uint32 ms) {
	SDL_Delay(ms);
}

void Graphics::update() {
	// SDL_UpdateWindowSurface(window);
	SDL_RenderPresent(gRenderer);
}

template <typename T>
void Graphics::drawOutlinedRect(T posX, T posY, int width, int height) {
	SDL_Rect outlineRect = { posX, posY, width, height };
	SDL_RenderDrawRect(gRenderer, &outlineRect);
}

template <typename T>
void Graphics::drawFilledRect(T posX, T posY, int width, int height) {
	SDL_Rect fillRect = { posX, posY, width, height };
	SDL_RenderFillRect(gRenderer, &fillRect);
}

template <typename T>
void Graphics::drawDashLine(T X1, T Y1, T X2, T Y2) {
	if (X1 > X2) {
		T x = X2;
		X2 = X1;
		X1 = x;
	}
	if (Y1 > Y2) {
		T y = Y2;
		Y2 = Y1;
		Y1 = y;
	}
	if (X2 - X1 > Y2 - Y1) {
		if (typeid(T) == typeid(int)) {
			for (T i = X1; i <= X2 - 10; i += 5)
			{
				SDL_RenderDrawLine(gRenderer, i, (int)(Y1 + (float)(Y2 - Y1)*(i - X1) / (X2 - X1)), i + 5, (int)(Y1 + (float)(Y2 - Y1)*(i + 5 - X1) / (X2 - X1)));
			}
		}
	}
	else {
		if (typeid(T) == typeid(int)) {
			for (T i = Y1; i <= Y2 - 10; i += 10)
			{
				SDL_RenderDrawLine(gRenderer, i, (int)(X1 + (float)(X2 - X1)*(i - Y1) / (Y2 - Y1)), i + 5, (int)(X1 + (float)(X2 - X1)*(i + 5 - Y1) / (Y2 - Y1)));
			}
		}
	}
}

template <typename T>
void Graphics::drawLine(T X1, T Y1, T X2, T Y2) {
	if (typeid(T) == typeid(int)) {
		SDL_RenderDrawLine(gRenderer, X1, Y1, X2, Y2);
	}
}

int Graphics::handleEvent() {
	//User requests quit
	if (e.type == SDL_QUIT)
	{
		return SCREEN_QUIT_EVENT;
	}
	if (e.type == SDL_KEYDOWN) {
		switch (e.key.keysym.sym) {
		case SDLK_r:
			return NEW_SHOW;
		case SDLK_q:
			return SCREEN_QUIT_EVENT;
		}
	}
	return UNHANDLED_EVENT;
}

void Graphics::exit() {
	//Deallocate surface
	SDL_FreeSurface(screenSurface);
	screenSurface = NULL;

	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;
	//Destroy window
	SDL_DestroyWindow(window);
	window = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

// No need to call this TemporaryFunction() function,
// it's just to avoid link error.
void TemporaryFunction()
{
	Graphics g;
	g.drawFilledRect<int>(0xFF, 0xFF, 0xFF, 0xFF);
	g.drawOutlinedRect<int>(0xFF, 0xFF, 0xFF, 0xFF);
	g.drawLine<int>(0, 0, 0, 0);
	g.drawDashLine<int>(0, 0, 0, 0);
}