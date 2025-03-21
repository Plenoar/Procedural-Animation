// #include "typedef.h"

// void DrawCircle(SDL_Renderer *renderer, CIRCLE circle, SDL_Color color) {
//     SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    
//     int x = circle.radius;
//     int y = 0;
//     int err = 0;

//     while (x >= y) {
//         SDL_RenderPoint(renderer, circle.x + x, circle.y + y);
//         SDL_RenderPoint(renderer, circle.x + y, circle.y + x);
//         SDL_RenderPoint(renderer, circle.x - y, circle.y + x);
//         SDL_RenderPoint(renderer, circle.x - x, circle.y + y);
//         SDL_RenderPoint(renderer, circle.x - x, circle.y - y);
//         SDL_RenderPoint(renderer, circle.x - y, circle.y - x);
//         SDL_RenderPoint(renderer, circle.x + y, circle.y - x);
//         SDL_RenderPoint(renderer, circle.x + x, circle.y - y);

//         y += 1;
//         if (err <= 0) {
//             err += 2 * y + 1;
//         }
//         if (err > 0) {
//             x -= 1;
//             err -= 2 * x + 1;
//         }
//     }
// }

// void RenderWorld(SDL_Renderer *renderer, SDL_Color white,double *θ,double *θ1,double *θ2)
// {

//     SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//     SDL_RenderClear(renderer);
//     SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, white.a);

//     // Draw the circles and connecting lines
//     CIRCLE circle = {900, 500, 400};
//     DrawCircle(renderer, circle, white);

//     double a = circle.x + (circle.radius * cos(*θ));
//     double b = circle.y + (circle.radius * sin(*θ));
//     CIRCLE circle1 = {a, b, 200};
//     DrawCircle(renderer, circle1, white);
//     SDL_RenderLine(renderer, (int)circle.x, (int)circle.y, (int)circle1.x, (int)circle1.y);

//     double c = circle1.x + (circle1.radius * cos(*θ1));
//     double d = circle1.y + (circle1.radius * sin(*θ1));
//     CIRCLE circle2 = {c, d, 100};
//     DrawCircle(renderer, circle2, white);
//     SDL_RenderLine(renderer, (int)circle1.x, (int)circle1.y, (int)circle2.x, (int)circle2.y);

//     double e = circle2.x + (circle2.radius * cos(*θ2));
//     double f = circle2.y + (circle2.radius * sin(*θ2));
//     CIRCLE circle3 = {e, f, 50};
//     DrawCircle(renderer, circle3, white);
//     SDL_RenderLine(renderer, (int)circle2.x, (int)circle2.y, (int)circle3.x, (int)circle3.y);

//     SDL_RenderPresent(renderer);

//     *θ += 0.01;
//     *θ1 += 0.02;
//     *θ2 += 0.04;
// }

// int WorldSetup()
// {
//     // Initialize SDL
//    SDL_Init(SDL_INIT_VIDEO); 


//     // Create window and renderer
//     if (!SDL_CreateWindowAndRenderer("Fourier Transformation Drawing", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer))
//     {
//         printf("Error creating window and renderer: %s\n", SDL_GetError());
//         SDL_Quit();
//         return 1;
//     }

//     return 0;
// }

// void DestroyWorld()
// {
//     if (renderer)
//     {
//         SDL_DestroyRenderer(renderer);
//         renderer = NULL;
//     }
//     if (window)
//     {
//         SDL_DestroyWindow(window);
//         window = NULL;
//     }
//     SDL_Quit();
// }
// void ProcessInput(USER *user, int *running)
// {
//     SDL_Event event;
//     while (SDL_PollEvent(&event))
//     {
//         switch (event.type)
//         {
//         case SDL_EVENT_QUIT:
//             *running = 0;
//             break;

//         }
//     }
// }
#include "world.h"
#include <SDL3/SDL.h>
#include <math.h>
#include "typedef.h"
#include "constants.h"


void worldToScreen(const CAMERA *camera, double wx, double wy, double *sx, double *sy) {
    double scale_x = (double)WINDOW_WIDTH / camera->width;
    double scale_y = (double)WINDOW_HEIGHT / camera->height;
    double scale = scale_x; // Use isotropic scaling to preserve circle shapes
    *sx = (wx - camera->x) * scale;
    *sy = (wy - camera->y) * scale;
}

void DrawCircle(SDL_Renderer *renderer, CIRCLE circle, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    
    int x = (int)circle.radius;
    int y = 0;
    int err = 0;

    int center_x = (int)circle.x;
    int center_y = (int)circle.y;

    while (x >= y) {
        SDL_RenderPoint(renderer, center_x + x, center_y + y);
        SDL_RenderPoint(renderer, center_x + y, center_y + x);
        SDL_RenderPoint(renderer, center_x - y, center_y + x);
        SDL_RenderPoint(renderer, center_x - x, center_y + y);
        SDL_RenderPoint(renderer, center_x - x, center_y - y);
        SDL_RenderPoint(renderer, center_x - y, center_y - x);
        SDL_RenderPoint(renderer, center_x + y, center_y - x);
        SDL_RenderPoint(renderer, center_x + x, center_y - y);

        y += 1;
        if (err <= 0) {
            err += 2 * y + 1;
        }
        if (err > 0) {
            x -= 1;
            err -= 2 * x + 1;
        }
    }
}

void RenderWorld(SDL_Renderer *renderer, SDL_Color white, double θ, double θ1, double θ2, CAMERA camera) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, white.a);

    // Define the main circle in world coordinates
    CIRCLE circle_world = {900, 500, 400}; // World coordinates
    double sx, sy;
    worldToScreen(&camera, circle_world.x, circle_world.y, &sx, &sy);
    CIRCLE circle_screen = {sx, sy, circle_world.radius * ((double)WINDOW_WIDTH / camera.width)};
    DrawCircle(renderer, circle_screen, white);

    // Second circle
    double a_world = circle_world.x + (circle_world.radius * cos(θ));
    double b_world = circle_world.y + (circle_world.radius * sin(θ));
    double a_screen, b_screen;
    worldToScreen(&camera, a_world, b_world, &a_screen, &b_screen);
    CIRCLE circle1_screen = {a_screen, b_screen, 200 * ((double)WINDOW_WIDTH / camera.width)};
    DrawCircle(renderer, circle1_screen, white);
    SDL_RenderLine(renderer, (int)sx, (int)sy, (int)a_screen, (int)b_screen);

    // Add similar transformations for circle2 and circle3 as needed

    SDL_RenderPresent(renderer);
}

void DestroyWorld() {

    SDL_Quit();
}

void ProcessInput(USER *user, CAMERA *camera , int *running) {
    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        switch (event.type) 
        {
            case SDL_EVENT_QUIT:
                *running =  0;
                break;

            case SDL_EVENT_MOUSE_REMOVED:

                break;

            case SDL_EVENT_KEYBOARD_REMOVED:

                break;

            case SDL_EVENT_MOUSE_MOTION: 
            {
                float x , y;
                SDL_GetMouseState(&x,&y);
                // needs update the mouse 
                break;
            }
            case SDL_EVENT_MOUSE_BUTTON_DOWN: 
            {
                // idk do something 
                break;
            }

            case SDL_EVENT_KEY_DOWN: {
                SDL_Keycode sym = event.key.key;

                if (sym == SDLK_W) user->wasd |= 1;
                if (sym == SDLK_A)user->wasd |= 2;
                if (sym == SDLK_S)user->wasd |= 4;
                if (sym == SDLK_D)user->wasd |= 8;
                if (sym == SDLK_SPACE)user->wasd |= 16;
                if (sym == SDLK_LSHIFT)user->wasd |= 32; // assigning 32 bit
                // value for the sprint implementation , 32 in binary is  100000
                break;
            }
            case SDL_EVENT_KEY_UP:
            {
                SDL_Keycode sym = event.key.key; 
                if (sym == SDLK_W)user->wasd &= ~1;
                if (sym == SDLK_A)user->wasd &= ~2;
                if (sym == SDLK_S)user->wasd &= ~4;
                if (sym == SDLK_D)user->wasd &= ~8;
                if (sym == SDLK_SPACE)user->wasd &= ~16; // this is to clear the bit 
                if (sym == SDLK_LSHIFT)user->wasd &= ~32;  
                break;
           }

           case SDL_EVENT_MOUSE_WHEEL: {
            double zoom_factor = (event.wheel.y > 0) ? 0.9 : (event.wheel.y < 0) ? 1.1 : 1.0;
            if (zoom_factor == 1.0) break;

            double center_x = camera->x + camera->width / 2.0;
            double center_y = camera->y + camera->height / 2.0;

            double new_width = camera->width * zoom_factor;
            double new_height = camera->height * zoom_factor;

            const double min_width = 100.0;
            const double max_width = 10000.0;
            if (new_width < min_width) {
                new_width = min_width;
                new_height = min_width * ((double)WINDOW_HEIGHT / WINDOW_WIDTH);
            } else if (new_width > max_width) {
                new_width = max_width;
                new_height = max_width * ((double)WINDOW_HEIGHT / WINDOW_WIDTH);
            }

            camera->width = new_width;
            camera->height = new_height;
            camera->x = center_x - camera->width / 2.0;
            camera->y = center_y - camera->height / 2.0;
            break;

        }
    }
}
}