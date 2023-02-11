// SDL_ellipse.c

#include "SDL_ellipse.h"

void SDL_RenderDrawEllipse(SDL_Renderer* renderer, int x, int y, int x_radius, int y_radius) {
    int x_plot, y_plot;
    int prev_x, prev_y;

    float f_x_radius = (float)x_radius;
    float f_y_radius = (float)y_radius;

    float step = (30.0f / f_x_radius);
    if (step > 0.1f) step = 0.1f;

    prev_x = (int)(SDL_cos(0) * f_x_radius);
    prev_y = (int)(SDL_sin(0) * f_y_radius);

    for (float theta = step; theta < M_PI / 2.0 + step; theta += step) {
        x_plot = (int)(SDL_cos(theta) * f_x_radius);
        y_plot = (int)(SDL_sin(theta) * f_y_radius);

        SDL_RenderDrawLine(renderer, x + prev_x, y + prev_y, x + x_plot, y + y_plot);
        SDL_RenderDrawLine(renderer, x + prev_x, y - prev_y, x + x_plot, y - y_plot);
        SDL_RenderDrawLine(renderer, x - prev_x, y - prev_y, x - x_plot, y - y_plot);
        SDL_RenderDrawLine(renderer, x - prev_x, y + prev_y, x - x_plot, y + y_plot);

        prev_x = x_plot;
        prev_y = y_plot;
    }
}

void SDL_RenderDrawCircle(SDL_Renderer* renderer, int x, int y, int radius) {
    SDL_RenderDrawEllipse(renderer, x, y, radius, radius);
}

void SDL_RenderFillCircle(SDL_Renderer* renderer, int x, int y, int radius, SDL_Color color) {
    // int x_plot, y_plot;
    SDL_Color oldColor;

    SDL_GetRenderDrawColor(renderer, &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    for (int y_plot = 0; y_plot <= radius; y_plot++)
        for (int x_plot = 0; x_plot <= radius; x_plot++)
            if (x_plot * x_plot + y_plot * y_plot <= radius * radius) {
                if (x_plot != 0 && y_plot != 0) {
                    SDL_Point points[] = { {x + x_plot, y - y_plot}, {x + x_plot, y + y_plot}, {x - x_plot, y + y_plot}, {x - x_plot, y - y_plot} };
                    SDL_RenderDrawPoints(renderer, points, 4);
                }
            }
    SDL_RenderDrawLine(renderer, x - radius, y, x - 1, y);
    SDL_RenderDrawLine(renderer, x + 1, y, x + radius, y);
    SDL_RenderDrawLine(renderer, x, y - radius, x, y + radius);

    SDL_SetRenderDrawColor(renderer, oldColor.r, oldColor.g, oldColor.b, oldColor.a);
}

void SDL_RenderFillEllipse(SDL_Renderer* renderer, int x, int y, int x_radius, int y_radius, SDL_Color color) {
    int hh = y_radius * y_radius;
    int ww = x_radius * x_radius;
    int hhww = hh * ww;
    int x0 = x_radius;
    int dx = 0;
    SDL_Color oldColor;

    SDL_GetRenderDrawColor(renderer, &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    // do the horizontal diameter
    for(int x1 = -x_radius; x1 <= x_radius; x1++)
        SDL_RenderDrawPoint(renderer, x + x1, y);

    // now do both halves at the same time, away from the diameter
    for(int y1 = 1; y1 <= y_radius; y1++) {
        int x1 = x0 - (dx - 1);  // try slopes of dx - 1 or more
        for(; x1 > 0; x1--)
            if (x1 * x1 * hh + y1 * y1 * ww <= hhww)
                break;
        dx = x0 - x1;  // current approximation of the slope
        x0 = x1;

        for(int x1 = -x0; x1 <= x0; x1++) {
            SDL_RenderDrawPoint(renderer, x + x1, y - y1);
            SDL_RenderDrawPoint(renderer, x + x1, y + y1);
        }
    }

    SDL_SetRenderDrawColor(renderer, oldColor.r, oldColor.g, oldColor.b, oldColor.a);
}

