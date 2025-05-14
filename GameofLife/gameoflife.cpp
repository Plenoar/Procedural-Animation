#include "imgui/imgui_impl_sdl3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdlrenderer3.h"

#include <SDL3/SDL.h>
#include <stdio.h>
#include <vector> 

typedef enum {
    DEAD = 0,
    ALIVE = 1
} CellState;

#define FIXED_TIME_STEP (1.0f / 10.0f) 
#define WORLD_HEIGHT 1000
#define WORLD_WIDTH 1000
#define CELL_SIZE 3
#define GRID_ROWS (WORLD_HEIGHT / CELL_SIZE)
#define GRID_COLS (WORLD_WIDTH / CELL_SIZE)


typedef struct {
    double time_elapsed; // total simulation time
    double time_step; 
} Time;

typedef struct {
    CellState cells[GRID_ROWS][GRID_COLS];
} Grid;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

// Forward declarations
void HandleRender(SDL_Renderer* renderer, const Grid& grid); 
void initGrid(Grid* grid);
int countAliveNeighbors(const Grid& grid, int r, int c);
void game_of_life_step(Grid* current_grid); 

int main(void) 
{
    Grid grid;
    initGrid(&grid);

    grid.cells[1][2] = ALIVE;
    grid.cells[2][3] = ALIVE;
    grid.cells[3][1] = ALIVE;
    grid.cells[3][2] = ALIVE;
    grid.cells[3][3] = ALIVE;

    Time time = {
        0.0, // Initialize time_elapsed
        FIXED_TIME_STEP
    };

    SDL_InitSubSystem(SDL_INIT_VIDEO);

    SDL_CreateWindowAndRenderer("can you see me ?", WORLD_WIDTH, WORLD_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer);
    SDL_Event event;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);


    Uint64 lastCounter = SDL_GetPerformanceCounter();
    Uint64 frequency = SDL_GetPerformanceFrequency();
    double accumulator = 0.0;

    int running = 1;
    while (running) 
    {
        // Event handling
        while (SDL_PollEvent(&event)) 
        { // Process all pending events
            ImGui_ImplSDL3_ProcessEvent(&event); // Pass event to ImGui
            if (event.type == SDL_EVENT_QUIT) 
            {
                running = 0;
            }

            if (event.type == SDL_EVENT_KEY_DOWN) 
            {
                if (event.key.key == SDLK_ESCAPE) 
                {
                    running = 0;
                }
            }

            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) 
            {
                if (event.button.button == SDL_BUTTON_LEFT && !io.WantCaptureMouse) 
                { 
                    float mouse_x, mouse_y;
                    SDL_GetMouseState(&mouse_x, &mouse_y);
                    int grid_c = mouse_x / CELL_SIZE;
                    int grid_r = mouse_y / CELL_SIZE;
                    if (grid_r >= 0 && grid_r < GRID_ROWS && grid_c >= 0 && grid_c < GRID_COLS) {
                        grid.cells[grid_r][grid_c] = (grid.cells[grid_r][grid_c] == ALIVE) ? DEAD : ALIVE;
                    }
                }
            }
        }

        Uint64 currentCounter = SDL_GetPerformanceCounter();
        double delta_time = (double)(currentCounter - lastCounter) / frequency;
        if (delta_time > 0.25) { // Frame skipper guard
            delta_time = 0.25;
        }
        lastCounter = currentCounter;
        accumulator += delta_time;

        // Fixed-step simulation update
        while (accumulator >= time.time_step) 
        {
            time.time_elapsed += time.time_step;
            game_of_life_step(&grid);
            accumulator -= time.time_step;
        }

        // ImGui new frame
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        // Example ImGui Window
        ImGui::Begin("Game of Life Controls");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::Text("Current Simulation Time: %.2f", time.time_elapsed);
        if (ImGui::Button("Randomize Grid")) {
            initGrid(&grid); 
             for (int r = 0; r < GRID_ROWS; ++r)
             {
                for (int c = 0; c < GRID_COLS; ++c)
                {
                    grid.cells[r][c] = (rand() % 100 < 20) ? ALIVE : DEAD;
                }
            }
        }
         if (ImGui::Button("Clear Grid"))
         {
            initGrid(&grid);
        }
        ImGui::End();

        // Your game world rendering
        HandleRender(renderer, grid);

        // Render ImGui draw data ON TOP of the game world
        ImGui::Render();
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);

        SDL_RenderPresent(renderer);
    }

    // Cleanup
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void HandleRender(SDL_Renderer* renderer, const Grid& grid) 
{
    // Clear screen to black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw grid lines (optional, but helpful)
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255); // Dark grey for grid lines
    for (int c = 0; c <= GRID_COLS; ++c) 
    {
        SDL_RenderLine(renderer, (float)(c * CELL_SIZE), 0.0f, (float)(c * CELL_SIZE), (float)WORLD_HEIGHT);
    }
    for (int r = 0; r <= GRID_ROWS; ++r) 
    {
        SDL_RenderLine(renderer, 0.0f, (float)(r * CELL_SIZE), (float)WORLD_WIDTH, (float)(r * CELL_SIZE));
    }

    SDL_FRect cell_rect;
    cell_rect.w = (float)CELL_SIZE -1; 
    cell_rect.h = (float)CELL_SIZE -1;

    for (int r = 0; r < GRID_ROWS; ++r) 
    {
        for (int c = 0; c < GRID_COLS; ++c) 
        {
            if (grid.cells[r][c] == ALIVE) 
            {
                SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
                cell_rect.x = (float)(c * CELL_SIZE) + 0.5f;
                cell_rect.y = (float)(r * CELL_SIZE) + 0.5f;
                SDL_RenderFillRect(renderer, &cell_rect);
            }
            // No need to explicitly draw DEAD cells if the background is black
            else 
            {
                SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
                cell_rect.x = (float)(c * CELL_SIZE);
                cell_rect.y = (float)(r * CELL_SIZE);
                SDL_RenderRect(renderer, &cell_rect);
            }
            
        }
    }
}

void initGrid(Grid* grid) {
    for (int r = 0; r < GRID_ROWS; ++r) 
    {
        for (int c = 0; c < GRID_COLS; ++c) 
        {
            grid->cells[r][c] = DEAD;
        }
    }
}

int countAliveNeighbors(const Grid& grid, int r, int c) 
{
    int alive_neighbors = 0;
    for (int dr = -1; dr <= 1; ++dr) 
    {
        for (int dc = -1; dc <= 1; ++dc) 
        {
            if (dr == 0 && dc == 0) 
            { 
                continue;
            }
            int nr = r + dr;
            int nc = c + dc;

            if (nr >= 0 && nr < GRID_ROWS && nc >= 0 && nc < GRID_COLS) 
            {
                if (grid.cells[nr][nc] == ALIVE) 
                {
                    alive_neighbors++;
                }
            }
        }
    }
    return alive_neighbors;
}

void game_of_life_step(Grid* current_grid) 
{
    Grid next_grid;

    for (int r = 0; r < GRID_ROWS; ++r) 
    {
        for (int c = 0; c < GRID_COLS; ++c) 
        {
            int alive_neighbors = countAliveNeighbors(*current_grid, r, c);
            CellState current_cell_state = current_grid->cells[r][c];
            CellState next_cell_state = current_cell_state;

            if (current_cell_state == ALIVE) 
            {
                if (alive_neighbors < 2 || alive_neighbors > 3) 
                {
                    next_cell_state = DEAD;
                }
            } 

            else 
            { 
                if (alive_neighbors == 3) 
                {
                    next_cell_state = ALIVE;
                }
            }
            next_grid.cells[r][c] = next_cell_state;
        }
    }
    *current_grid = next_grid;
}