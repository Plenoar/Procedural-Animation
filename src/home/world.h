#ifndef WORLD_H
#define WORLD_H

#include "typedef.h"
#include <SDL3/SDL.h>

void DrawCircle(SDL_Renderer *renderer, CIRCLE circle, SDL_Color color);
void RenderWorld(SDL_Renderer *renderer, SDL_Color white, double θ, double θ1, double θ2, CAMERA camera);
void DestroyWorld();
void ProcessInput(USER *user,CAMERA *camera, int *running);

#endif