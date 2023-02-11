// pipe.cpp

#include "pipe.h"
#include "setup.h"
#include <stdlib.h>


Pipe::Pipe() {

    this->x = WINDOW_WIDTH;
    this->w = 32;

    this->top = rand() % (int)(WINDOW_HEIGHT / 1.5);
    this->bottom = WINDOW_HEIGHT - (this->top + 200);

    this->speed = 3;

}

Pipe::~Pipe() {
}



void Pipe::update() {
    this->x -= this->speed;
}

void Pipe::show(SDL_Renderer* renderer) {

    SDL_Rect rect;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    rect.x = this->x;
    rect.y = 0;
    rect.w = this->w;
    rect.h = this->top;

    SDL_RenderFillRect(renderer, &rect);

    rect.x = this->x;
    rect.y = WINDOW_HEIGHT - this->bottom;
    rect.w = this->w;
    rect.h = this->bottom;

    SDL_RenderFillRect(renderer, &rect);

    // SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    // SDL_RenderDrawLine(renderer, this->x, this->top, this->x + this->w, this->top);
    // SDL_RenderDrawLine(renderer, this->x, WINDOW_HEIGHT - this->bottom, this->x + this->w, WINDOW_HEIGHT - this->bottom);

}
