#include <stdio.h>
#include <SDL3/SDL.h>
#include <math.h>

#define WHITE 0xFFFFFFFF // Define white color as RGBA 0xFFFFFFFF

static SDL_Renderer *renderer = NULL;
static SDL_Window *window = NULL;

typedef struct {
    int x;
    int y;
    int length;
}LINE;

typedef struct {
    double x;
    double y;
    double radius;
} CIRCLE;

void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color) {
    if (x >= 0 && x < surface->w && y >= 0 && y < surface->h) {
        Uint32 *pixels = (Uint32 *)surface->pixels;
        pixels[(y * surface->w) + x] = color;
    }
}

void DrawCircle(SDL_Surface *surface, CIRCLE circle, Uint32 color) {
    int x = (int)circle.radius;
    int y = 0;
    int decision = 1 - x;

    while (x >= y) {
        DrawPixel(surface, (int)circle.x + x, (int)circle.y + y, color);
        DrawPixel(surface, (int)circle.x - x, (int)circle.y + y, color);
        DrawPixel(surface, (int)circle.x + x, (int)circle.y - y, color);
        DrawPixel(surface, (int)circle.x - x, (int)circle.y - y, color);
        DrawPixel(surface, (int)circle.x + y, (int)circle.y + x, color);
        DrawPixel(surface, (int)circle.x - y, (int)circle.y + x, color);
        DrawPixel(surface, (int)circle.x + y, (int)circle.y - x, color);
        DrawPixel(surface, (int)circle.x - y, (int)circle.y - x, color);

        y++;
        if (decision <= 0) {
            decision += 2 * y + 1;
        } else {
            x--;
            decision += 2 * (y - x) + 1;
        }
    }
}




int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer("Fourier Transformation", 1900, 1000, SDL_WINDOW_FULLSCREEN, &window, &renderer);
    SDL_Surface *surface = SDL_GetWindowSurface(window);

    
    int running = 1;
    
    CIRCLE circle = { 300, 500, 200 }; // Larger circle
    float time = 0.0f;


    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = 0;
            }
        }

        SDL_ClearSurface(surface, 0, 0, 0, 0); // Clear screen

        DrawCircle(surface, circle, WHITE); // Draw large circle
     

        int x = circle.x + (circle.radius * cos(time));
        int y = circle.y + (circle.radius * sin(time));
        CIRCLE smallCircle = { x, y, 5 }; // Smaller circle radius = 50
        DrawCircle(surface, smallCircle, WHITE); // Draw small circle


        SDL_UpdateWindowSurface(window);
        SDL_Delay(16); // Approx. 60 FPS

        time -= 0.05f; // Adjust speed
    }


    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
