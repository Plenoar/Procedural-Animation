#include <stdio.h>
#include <SDL3/SDL.h>

#include "typedef.h"
#include "constants.h"
#include "camera.h"
#include "world.h"


static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

// Function to update the game state
void UpdateWorld(WORLD *world, USER *user, CAMERA *camera, double *θ, double *θ1, double *θ2, double deltaTime) {
    // User movement based on input
    double speed = 100.0; // Pixels per second
    if (user->wasd & 1) user->y_pos -= speed * deltaTime; // W
    if (user->wasd & 4) user->y_pos += speed * deltaTime; // S
    if (user->wasd & 2) user->x_pos -= speed * deltaTime; // A
    if (user->wasd & 8) user->x_pos += speed * deltaTime; // D

    // Update camera (assuming UpdateCamera is defined elsewhere)
    UpdateCamera(*world, camera, user, (float)deltaTime);

    // Update angles for animation (e.g., Fourier circles)
    double θ_speed = 1.0;  // Radians per second
    double θ1_speed = 2.0;
    double θ2_speed = 4.0;
    *θ += θ_speed * deltaTime;
    *θ1 += θ1_speed * deltaTime;
    *θ2 += θ2_speed * deltaTime;
}

int main(void)
{   
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer("Fourier Transformation Drawing", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);

    // Initial setup
    WORLD world = {WORLD_WIDTH, WORLD_HEIGHT, 0.0, FIXED_TIME_STEP}; // FIXED_TIME_STEP might be 1.0f / 160.0f
    USER user = {100.0, 100.0, 0}; // Starting position and input state
    CAMERA camera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 100.0f, 50.0f, 25.0f, 0}; // Camera settings
    double θ = 0.0, θ1 = 0.0, θ2 = 0.0; // Angles for animation
    SDL_Color white = {255, 255, 255, 255}; // Color for rendering

    SDL_Event event;
    
    int running = 1;

    Uint64 lastCounter = SDL_GetPerformanceCounter();
    Uint64 frequency = SDL_GetPerformanceFrequency();
    double accumulator = 0.0; 
 
    while(running)
    {
        // Measure time
        Uint64 currentCounter = SDL_GetPerformanceCounter(); // i still dont understand what the counter is supposed to do here 
        double delta_time = (double)(currentCounter - lastCounter) / frequency;
        lastCounter = currentCounter;

        accumulator += delta_time;

        ProcessInput(&user, &camera ,&running);

        while(accumulator >= world.time_step)
        {
            world.time_elapsed += world.time_step;
            UpdateWorld(&world, &user, &camera, &θ, &θ1, &θ2, world.time_step);
            accumulator -= world.time_step;

        }
        RenderWorld(renderer, white,θ,θ1,θ2,camera );
        
        // Calculate FPS
        double fps = 1.0 / delta_time;
        printf("Frame Time: %.6f sec | FPS: %.2f\n", delta_time, fps);
        

    }

    DestroyWorld();
    return 0;
}


