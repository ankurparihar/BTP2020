#pragma once
#include "GraphicsParams.h"

class Graphics {
public:
	// The window we'll be rendering to
	SDL_Window* window;

	// The surface contained by the window
	SDL_Surface* screenSurface;

	//The window renderer
	SDL_Renderer* gRenderer;

	//Event handler
	SDL_Event e;

	Graphics() : window(NULL), screenSurface(NULL), gRenderer(NULL) {}
	~Graphics() {
		exit();
	}

	bool init();
	void FillRect(Uint8 r, Uint8 g, Uint8 b);
	void wait(Uint32 ms);
	void update();
	void exit();
	inline int pollEvent() { return SDL_PollEvent(&e); }
	int handleEvent();

	inline void setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) { SDL_SetRenderDrawColor(gRenderer, r, g, b, a); }
	inline void clearRenderScreen() { SDL_RenderClear(gRenderer); }

	template <typename T>
	void drawFilledRect(T posX, T posY, int width, int height);
	template <typename T>
	void drawOutlinedRect(T posX, T posY, int width, int height);
	template <typename T>
	void drawLine(T posX1, T posY1, T posX2, T posY2);
	template <typename T>
	void drawDashLine(T posX1, T posY1, T posX2, T posY2);
	template <typename T>
	inline void drawLine(Point<T> pos1, Point<T> pos2) { drawLine(pos1.x, pos1.y, pos2.x, pos2.y); }
	template <typename T>
	inline void drawDashLine(Point<T> pos1, Point<T> pos2) { drawDashLine(pos1.x, pos1.y, pos2.x, pos2.y); }
};