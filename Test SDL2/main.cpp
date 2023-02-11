
#include <iostream>
#include <vector>

#include "setup.h"

#include "matrix.h"
#include "nn.h"
#include "bird.h"
#include "pipe.h"

bool no_loop = false;

//extern "C" {
//    void showMsg(SDL_Renderer* renderer, char* txt, int x, int y, int size);
//}

const int nb_birds = 300;

std::vector <Pipe*> pipes;
std::vector <Bird*> birds;
std::vector <Bird*> saved_birds;
std::vector <Bird*> selected_birds;

long frameCount;
long generation;

void init_playground() {

    for (int i = 0; i < nb_birds; i++) {
        Bird* bird = new Bird();
        birds.push_back(bird);
    }

    Pipe* pipe = new Pipe();
    pipes.push_back(pipe);

    frameCount = 0L;
    generation = 1L;

}

int birds_count = 0;

void update(SDL_Renderer* renderer) {

    SDL_Rect rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);

    if (++frameCount >= 150L) {
        Pipe* pipe = new Pipe();
        pipes.push_back(pipe);
        frameCount = 0L;
    }

    if (pipes.size() > 0) {
        if (pipes[0]->x + pipes[0]->w - 10 <= 0) {
            delete pipes[0];
            pipes.erase(pipes.begin());
        }

        for (int i = 0; i < pipes.size(); i++) {
            pipes[i]->update();
            pipes[i]->show(renderer);
        }
    }

    if (birds.size() > 0) {

        /*for (int i = 0; i < saved_birds.size(); i++) {
            delete saved_birds[i];
        }
        saved_birds.clear();*/

        for (int i = (int) birds.size() - 1; i >= 0; i--) {

            birds[i]->think(renderer, pipes);
            birds[i]->update();
            birds[i]->show(renderer);

            if (pipes.size() > 0) {
                if (birds[i]->hits(pipes)) {
                    saved_birds.push_back(birds[i]);
                    birds.erase(birds.begin() + i);
                }
            }
        }
    }

    if (birds.size() != birds_count) {
        birds_count = birds.size();
        std::cout << "Nb birds remaining : " << birds_count << std::endl;
    }

    if (birds.size() < 1) { // Create new generation of birds and restart

        std::cout << "\nRestarting ..\n" << std::endl;

        while (pipes.size() > 0) {
            delete pipes[0];
            pipes.erase(pipes.begin());
        }

        Pipe* pipe = new Pipe();
        pipes.push_back(pipe);

        frameCount = 0L;

        std::cout << "Creating new generation of birds" << std::endl;

        for (int i = 10; i >= 0; i--) {
            for (int j = 0; j < i*2; j++) {
                selected_birds.push_back(saved_birds[saved_birds.size() - 11 + i]);
            }
        }

        /*for (int i = saved_birds.size(); i >= 0; i--)
            for (int j = 0; j < i * (nb_birds / saved_birds.size()); j++) {
                selected_birds.push_back(saved_birds[i-1]);
            }*/

        // std::cout << "New selection size : " << selected_birds.size() << std::endl;

        while (birds.size() < nb_birds) {
            Bird* bird = new Bird();

            int selected_parent = rand() % (selected_birds.size());
            bird->brain->inherit(selected_birds[selected_parent]->brain);
            birds.push_back(bird);
        }

        for (int i = 0; i < saved_birds.size(); i++) {
            delete saved_birds[i];
        }
        saved_birds.clear();
        selected_birds.clear();

        generation++;

        std::cout << "Generation " << generation << " completed" << std::endl;
    }

    // printf("Nb pipes : %d\n", pipes.size() );
    // printf("Nb birds : %d\n", birds.size() );

}

void cleanup_playground() {

    for (int i = 0; i < birds.size(); i++) {
        delete birds[i];
    }
    birds.clear();

    for (int i = 0; i < pipes.size(); i++) {
        delete pipes[i];
    }
    pipes.clear();

}



int main(int argc, char** argv) {

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    // int x_mouse, y_mouse;

    srand((unsigned int) time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("ERREUR : Initialisation SDL > %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    /*if (TTF_Init() != 0) {
        SDL_Log("ERREUR : Initialisation TTF > %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }*/

    // Create the window
    window = SDL_CreateWindow("NN Flappy Birds",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        0);
    if (window == NULL) {
        SDL_Log("ERREUR : Echec creation fenetre > %s\n", SDL_GetError());
        //TTF_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    // /*------------------------------------------*/

    // renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    // renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        SDL_Log("ERREUR : Echec creation renderer > %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        //TTF_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    if (SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND) != 0) {
        SDL_Log("ERREUR : Echec selection du blend mode pour le renderer > %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        //TTF_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    unsigned int current_tick, last_tick, next_tick = 0;
    unsigned int dw1, dw2 = 0;

    int counter = 0;

    last_tick = SDL_GetTicks();

    SDL_bool program_launched = SDL_TRUE;
    SDL_Event event;
    SDL_bool flagShowFPS = SDL_FALSE;

    std::string buffer;

    init_playground();

    while (program_launched) {

        while (SDL_PollEvent(&event)) {
            switch (event.type) {

                // case SDL_WINDOWEVENT:
                //     switch (event.window.event) {
                //         case SDL_WINDOWEVENT_ENTER:
                //             SDL_Log("window enter");
                //             SDL_ShowCursor(SDL_DISABLE);
                //         break;
                //         case SDL_WINDOWEVENT_LEAVE:
                //             SDL_Log("window leave");
                //             SDL_ShowCursor(SDL_ENABLE);
                //         break;
                //         default:
                //             break;
                //     }
                //     break;

            case SDL_MOUSEBUTTONDOWN:
                // if(event.button.button == SDL_BUTTON_LEFT) {
                //     SDL_GetMouseState(&x_mouse, &y_mouse);
                //     vehicule * v = new vehicule(x_mouse, y_mouse);
                //     vehicules.push_back(v);
                // }

                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_f:
                    // if(current_cell->previous != NULL) {
                    if (flagShowFPS == SDL_FALSE)
                        flagShowFPS = SDL_TRUE;
                    else
                        flagShowFPS = SDL_FALSE;
                    // }
                    break;
                default:
                    break;

                case SDLK_SPACE:
                    birds[0]->up();
                    break;
                }
                break;

            case SDL_QUIT:
                program_launched = SDL_FALSE;
                break;

            default:
                break;
            }


        }

        // SDL_GetMouseState(&x_mouse, &y_mouse);
        // SDL_Log("Mouse is at %d, %d", x_mouse, y_mouse);

        if (!no_loop) {
            update(renderer);
        }

        /*if (flagShowFPS) {

            if (++counter > 10) {
                counter = 0;
                buffer.erase();
                buffer += "FPS : ";
                buffer += std::to_string(1000 / (dw2 - dw1));
            }

            showMsg(renderer, (char*)buffer.c_str(), WINDOW_WIDTH - 150, 10, 24);
        }*/

        SDL_RenderPresent(renderer);

        next_tick = last_tick + FPS_LIMIT;
        current_tick = SDL_GetTicks();
        if (current_tick < next_tick)
            SDL_Delay(next_tick - current_tick);

        dw1 = dw2;
        last_tick = dw2 = SDL_GetTicks();
    }

    /*------------------------------------------*/

    cleanup_playground();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    //TTF_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}

//extern "C" {
//
//    void showMsg(SDL_Renderer* renderer, char* txt, int x, int y, int size) {
//
//        if (txt[0] == '\0')
//            return;
//
//        //this opens a font style and sets a size
//        TTF_Font* Arial = TTF_OpenFont("arial.ttf", size);
//        if (Arial == NULL) {
//            SDL_Log("ERREUR : Echec chargement fonte > %s\n", SDL_GetError());
//            return;
//        }
//
//        // this is the color in rgb format.
//        SDL_Color White = { 255, 255, 255 };
//
//        // as TTF_RenderText_Solid could only be used on
//        // SDL_Surface then you have to create the surface first
//        SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Arial, txt, White);
//        if (surfaceMessage == NULL) {
//            SDL_Log("ERREUR : Echec lors de la creation de la \"surface\" > %s\n", SDL_GetError());
//        }
//
//        TTF_CloseFont(Arial);
//        Arial = NULL;
//
//        // converts the surface into a texture
//        SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
//        if (Message == NULL) {
//            SDL_Log("ERREUR : Echec lors de la creation de la texture > %s\n", SDL_GetError());
//        }
//
//        SDL_FreeSurface(surfaceMessage);
//
//        SDL_Rect Message_rect = { x, y, 0, 0 }; //create a rect to store texture size
//        // Reads the size of the texture
//        if (SDL_QueryTexture(Message, NULL, NULL, &Message_rect.w, &Message_rect.h) < 0) {
//            SDL_Log("ERREUR : Echec lors de la lecture de la taille de la texture > %s\n", SDL_GetError());
//        }
//
//        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//        Message_rect.x -= 10;
//        Message_rect.w += 20;
//        SDL_RenderFillRect(renderer, &Message_rect);
//
//        Message_rect.x += 10;
//        Message_rect.w -= 20;
//        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//
//        if (SDL_RenderCopy(renderer, Message, NULL, &Message_rect) < 0) {
//            SDL_Log("ERREUR : Echec lors du rendu de la texture > %s\n", SDL_GetError());
//        }
//
//        SDL_DestroyTexture(Message);
//    }
//
//} // extern "C" {