// bird.h
#ifndef BIRD_H
#define BIRD_H

#include <vector>
#include "SDL_ellipse.h"
#include "nn.h"
#include "pipe.h"

class Bird {

public:

    int x;
    int y;

    int r;

    double gravity;
    double lift;
    double velocity;

    NeuralNetwork* brain;

    Bird();     // Constructor
    ~Bird();    // Destructor

    void show(SDL_Renderer*);
    void up();
    void think(SDL_Renderer*, std::vector <Pipe*> pipes);
    void update();
    bool hits(std::vector <Pipe*> pipes);

};

#endif // BIRD_H
