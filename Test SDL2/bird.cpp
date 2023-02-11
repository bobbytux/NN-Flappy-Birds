// bird.cpp

// #include <stdio.h>

#include "setup.h"
#include "bird.h"

Bird::Bird() {
    this->y = WINDOW_HEIGHT / 2;
    this->x = 64;

    this->r = 16;

    this->gravity = 0.6f;
    this->lift = -10.0f;
    this->velocity = 0.0f;

    this->brain = new NeuralNetwork(5, 4, 2);

}

Bird::~Bird() {
    delete this->brain;
}

void Bird::show(SDL_Renderer* renderer) {
    SDL_Color color = { 255, 255, 255, 100 };
    SDL_RenderFillCircle(renderer, this->x, this->y, this->r, color);
}

void Bird::up() {
    this->velocity += this->lift;
}

void Bird::think(SDL_Renderer* renderer, std::vector <Pipe*> pipes) {
    Matrix input(5, 1);
    Matrix output(2, 1);

    // Find the closest pipe in front (the first pipe may be behind the bird)
    int closest = 0;
    int smallest_dist = WINDOW_WIDTH;

    for (int i = 0; i < pipes.size(); i++) {
        int dist = pipes[i]->x + pipes[i]->w - this->x + this->r;
        if (dist < 0) // This one is behind
            continue;
        if (dist < smallest_dist) {
            smallest_dist = dist;
            closest = i;
        }
    }

    // printf("Closest : %d\n", closest);
    // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
    // SDL_RenderDrawLine(renderer, 0, pipes[closest]->top, WINDOW_WIDTH, pipes[closest]->top);
    // SDL_RenderDrawLine(renderer, 0, WINDOW_HEIGHT - pipes[closest]->bottom, WINDOW_WIDTH, WINDOW_HEIGHT - pipes[closest]->bottom);

    input.content[0][0] = (double)this->y / (double)WINDOW_HEIGHT;
    input.content[1][0] = (double)pipes[closest]->top / (double)WINDOW_HEIGHT;
    input.content[2][0] = (double)pipes[closest]->bottom / (double)WINDOW_HEIGHT;
    input.content[3][0] = (double)(pipes[closest]->x + pipes[closest]->w) / (double)WINDOW_WIDTH;
    input.content[4][0] = this->velocity / 195.0f;

    output = this->brain->predict(input);

    if (output.content[0][0] > output.content[1][0]) {
        // if(output.content[0][0] > 0.5f) {
        this->up();
    }
}

void Bird::update() {
    this->velocity += this->gravity;
    this->y += (int) this->velocity;

    if (this->y > WINDOW_HEIGHT - this->r) {
        this->y = WINDOW_HEIGHT - this->r;
        this->velocity = 0.0f;
    }

    if (this->y < this->r) {
        this->y = this->r;
        this->velocity = 0.0f;
    }

}

bool Bird::hits(std::vector <Pipe*> pipes) {

    if (this->y <= this->r)
        return true;

    if (this->y + this->r >= WINDOW_HEIGHT)
        return true;

    for (int i = 0; i < pipes.size(); i++) {
        if (this->x + this->r > pipes[i]->x) {
            if (this->x - this->r < pipes[i]->x + pipes[i]->w) {
                if (this->y - this->r < pipes[i]->top)
                    return true;
                if (this->y + this->r > WINDOW_HEIGHT - pipes[i]->bottom)
                    return true;
            }
        }
    }
    return false;
}

