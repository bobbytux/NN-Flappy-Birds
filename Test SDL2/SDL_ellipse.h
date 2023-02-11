//#pragma once
// SDL_elipse.h
#ifndef SDL_ELLIPSE_H
#define SDL_ELLIPSE_H

#include <SDL.h>

#ifdef __cplusplus
extern "C" {
#endif
	void SDL_RenderDrawCircle(SDL_Renderer* renderer, int x, int y, int radius);
	void SDL_RenderDrawEllipse(SDL_Renderer* renderer, int x, int y, int x_radius, int y_radius);
	void SDL_RenderFillCircle(SDL_Renderer* renderer, int x, int y, int radius, SDL_Color color);
	void SDL_RenderFillEllipse(SDL_Renderer* renderer, int x, int y, int x_radius, int y_radius, SDL_Color color);
#ifdef __cplusplus
}
#endif

#endif // SDL_ELLIPSE_H