// pipe.h
#ifndef PIPE_H
#define PIPE_H

// #include "SDL_ellipse.h"
#include <SDL.h>

class Pipe {

public:

    int x;
    int w;

    int top;
    int bottom;

    int speed;

    Pipe();     // Constructor
    ~Pipe();    // Destructor

     void update();
    void show(SDL_Renderer*);

};

#endif // PIPE_H