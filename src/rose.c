#include <stdio.h>
#include <SDL3/SDL.h>
#include <math.h>

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 800
#define WHITE 0xFFFFFFFF
#define PI 3.14159


static SDL_Renderer *renderer = NULL;
static SDL_Window *window = NULL;

void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color) {
    if (x >= 0 && x < surface->w && y >= 0 && y < surface->h) {
        Uint32 *pixels = (Uint32 *)surface->pixels;
        pixels[(y * surface->w) + x] = color;
    }
}

void DrawRose(SDL_Surface *surface, int centerX, int centerY, int A, int k, Uint32 color) {
    for (float theta = 0; theta < 2 * PI * k; theta += 0.01) {
        float r = A * cos(k * theta); // Rose equation
        int x = centerX + r * cos(theta);
        int y = centerY + r * sin(theta);
        DrawPixel(surface, x, y, color);
    }
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer("Rose Pattern", SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
    SDL_Surface *surface = SDL_GetWindowSurface(window);

    SDL_LockSurface(surface);
    DrawRose(surface, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 200, 9999, WHITE); // 5-petal flower
    SDL_UnlockSurface(surface);

    SDL_UpdateWindowSurface(window);
    SDL_Delay(5000);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
