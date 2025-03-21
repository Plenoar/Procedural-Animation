#include "typedef.h"
#include <SDL3/SDL.h>  // Include SDL3 headers for SDL_max and SDL_min

void UpdateCamera(WORLD world, CAMERA *camera, USER *user, float deltaTime) {
    if (camera->locked) return;

    float deadzone_left = camera->x + (camera->width - camera->deadzone_x) / 2;
    float deadzone_right = camera->x + (camera->width + camera->deadzone_x) / 2;
    float deadzone_top = camera->y + (camera->height - camera->deadzone_y) / 2;
    float deadzone_bottom = camera->y + (camera->height + camera->deadzone_y) / 2;

    // Horizontal movement with dead zone
    if (user->x_pos < deadzone_left) {
        camera->x -= (deadzone_left - user->x_pos) * 0.1f * deltaTime * 60.0f;
    } else if (user->x_pos > deadzone_right) {
        camera->x += (user->x_pos - deadzone_right) * 0.1f * deltaTime * 60.0f;
    }

    // Lookahead: Adjust camera based on movement direction
    float lookahead_target = (user->wasd & 2) ? -camera->lookahead : (user->wasd & 8) ? camera->lookahead : 0;
    float target_x = user->x_pos - camera->width / 2 + lookahead_target;
    camera->x += (target_x - camera->x) * 0.05f * deltaTime * 60.0f;

    // Vertical movement with dead zone
    if (user->y_pos < deadzone_top) {
        camera->y -= (deadzone_top - user->y_pos) * 0.1f * deltaTime * 60.0f;
    } else if (user->y_pos > deadzone_bottom) {
        camera->y += (user->y_pos - deadzone_bottom) * 0.1f * deltaTime * 60.0f;
    }

    // Clamp camera to world bounds
    camera->x = SDL_max(0, SDL_min(world.width - camera->width, camera->x));
    camera->y = SDL_max(0, SDL_min(world.height - camera->height, camera->y));
}