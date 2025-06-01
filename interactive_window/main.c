#include<stdio.h>
#include <SDL3/SDL.h>
#include<stdlib.h>

/*
This is an implementation for my game project , it will sort of behave like the gateway , the user has
to press inside the black void to enter the game

what i originally has in mind , while designning this 

> also i want to animate a bird using the grids , once the bird reaches the end it will turn into ashes and
        those ashes will turn black and explode and create the portal 
> the portal will act as a gateway to the actual game 
> feature some smooth animation
        i.e. a hovering mouse grid vanish logic , the area that the mouse hovers will become transparent , sort
            of like a repulsive force
        the transparent inner grids will once again regain its blackness after some times passes

        the core of the portal will need some portal like animation 

        the edges of the grid will have relly low probability of genning a grid , thus keeping the true size
        of the window a sort of mystery 

        some of the edges will be randomlly black and transparent 

        some sound desgin for the portal , when the mouse hovers , when the user enters the portal , also 
        for the bird flying and turnning into ashes 

        when the user enters the portal we will destroy the window and create a new window and direct them
        towards the actual game

        thats it for now g

and thus i mark this side project finnished ..... for now that is 

so what have i done ? 

> mouse hover
> randomly generated edges
> i do have a fllying plane animation which is not yet implemented here


*/

typedef enum {
    TRANSPARENT = 0,
    BLACK = 1
} CellState;

typedef struct {
    double time_elapsed; // total simulation time
    double time_step; 
} Time;

#define FIXED_TIME_STEP (1.0f / 2.7f) 
#define WORLD_HEIGHT 500
#define WORLD_WIDTH 500
#define CELL_SIZE 10
#define GRID_ROWS (WORLD_HEIGHT / CELL_SIZE)
#define GRID_COLS (WORLD_WIDTH / CELL_SIZE)

typedef struct {
    CellState cells[GRID_ROWS][GRID_COLS];
} Grid;

//forward declarations
void HandleRender(SDL_Renderer *renderer, Grid grid);
void border_step(Grid *grid);

SDL_Window *window;
SDL_Renderer *renderer;

int main() 
{
        Time time = {
        0.0, // Initialize time_elapsed
        FIXED_TIME_STEP
    };

    Grid grid;
    
    for (int r = 0; r < GRID_ROWS; ++r) 
    {
        for (int c = 0; c < GRID_COLS; ++c) 
        {
            grid.cells[r][c] = BLACK;
        }
    }

    grid.cells[1][2] = TRANSPARENT;
    grid.cells[2][3] = TRANSPARENT;
    grid.cells[3][1] = TRANSPARENT;
    grid.cells[3][2] = TRANSPARENT;
    grid.cells[3][3] = TRANSPARENT;

    SDL_InitSubSystem(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer("trying out lsystem", WORLD_WIDTH, WORLD_HEIGHT, SDL_WINDOW_TRANSPARENT | SDL_WINDOW_BORDERLESS,&window, &renderer);

    float x,y;

    SDL_Event event;

    Uint64 lastCounter = SDL_GetPerformanceCounter();
    Uint64 frequency = SDL_GetPerformanceFrequency();
    double accumulator = 0.0f;

    int running = 1;
    while(running)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_EVENT_QUIT)
                {
                    running = 0;
                }
            SDL_GetMouseState(&x,&y);
                // if the mouse is hovering over the grid we will make the grid vanish 
                // convert mouse coordinates into grid coordinates

                int grid_c = x / CELL_SIZE;
                int grid_r = y / CELL_SIZE;
                if (grid_r >= 0 && grid_r < GRID_ROWS && grid_c >= 0 && grid_c < GRID_COLS) 
                {
                    grid.cells[grid_r][grid_c] = TRANSPARENT;
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
            border_step(&grid);
            accumulator -= time.time_step;
        }

        HandleRender(renderer, grid);
        SDL_RenderPresent(renderer);
        }   
    }


void HandleRender(SDL_Renderer *renderer, Grid grid)
{

    SDL_SetRenderDrawColor(renderer,0,0,0,0);
    SDL_RenderClear(renderer);
    SDL_FRect cell_rect;
    cell_rect.w = (float)CELL_SIZE; 
    cell_rect.h = (float)CELL_SIZE;

    for (int r = 0; r < GRID_ROWS; ++r) 
    {
        for (int c = 0; c < GRID_COLS; ++c) 
        {
            if (grid.cells[r][c] == BLACK) 
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                cell_rect.x = (float)(c * CELL_SIZE);
                cell_rect.y = (float)(r * CELL_SIZE);
                SDL_RenderFillRect(renderer, &cell_rect);
            }
            // No need to explicitly draw DEAD cells if the background is black
            else 
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                cell_rect.x = (float)(c * CELL_SIZE);
                cell_rect.y = (float)(r * CELL_SIZE);
                SDL_RenderRect(renderer, &cell_rect);
            }
            
        }
    }
}

void border_step(Grid *grid)
{
     // Probabilities for distance 0 to 9 from any edge
    const int prob_edge_pattern[10] = {95, 80, 70, 60, 50, 40, 30, 20, 10, 5};

    // --- Top Border (first 10 rows, all columns) ---
    // Distance from top is the row index i (0-9)
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < GRID_COLS; ++j) {
            grid->cells[i][j] = (rand() % 100 < prob_edge_pattern[i]) ? TRANSPARENT : BLACK;
        }
    }

    // --- Bottom Border (last 10 rows, all columns) ---
    // Rows from GRID_ROWS - 10 up to GRID_ROWS - 1
    // Distance from bottom edge (row GRID_ROWS - 1) for row 'actual_row' is (GRID_ROWS - 1 - actual_row)
    // The index into prob_edge_pattern is this distance.
    // If we loop i from 0 to 9 (representing the 10 rows), the actual row is GRID_ROWS - 10 + i.
    // The distance from bottom is (GRID_ROWS - 1) - (GRID_ROWS - 10 + i) = 9 - i.
    // So, the index into prob_edge_pattern for loop counter i is 9 - i.
    for (int i = 0; i < 10; ++i) { // Loop 0-9 for the 10 bottom rows
        int actual_row = GRID_ROWS - 10 + i; // The real row index
        int prob_idx = 9 - i;                // Index into prob_edge_pattern (0 for the lowest row (i=9), 9 for the top of this block (i=0))

        for (int j = 0; j < GRID_COLS; ++j) {
            grid->cells[actual_row][j] = (rand() % 100 < prob_edge_pattern[prob_idx]) ? TRANSPARENT : BLACK;
        }
    }

    // --- Left Border (first 10 columns, excluding top/bottom 10 rows) ---
    // Columns from 0 to 9
    // Rows from 10 to GRID_ROWS - 11
    // Distance from left is the column index j (0-9)
    for (int j = 0; j < 10; ++j) { // Iterate through columns 0 to 9
         // prob_idx is simply j
        // Iterate through rows that are not part of the top or bottom 10 rows
        for (int i = 10; i < GRID_ROWS - 10; ++i) {
             grid->cells[i][j] = (rand() % 100 < prob_edge_pattern[j]) ? TRANSPARENT : BLACK;
        }
    }

    // --- Right Border (last 10 columns, excluding top/bottom 10 rows) ---
    // Columns from GRID_COLS - 10 up to GRID_COLS - 1
    // Rows from 10 to GRID_ROWS - 11
    // Distance from right edge (col GRID_COLS - 1) for col 'actual_col' is (GRID_COLS - 1 - actual_col)
    // The index into prob_edge_pattern is this distance.
    // If we loop j from 0 to 9 (representing the 10 columns), the actual col is GRID_COLS - 10 + j.
    // The distance from right is (GRID_COLS - 1) - (GRID_COLS - 10 + j) = 9 - j.
    // So, the index into prob_edge_pattern for loop counter j is 9 - j.
     for (int j = 0; j < 10; ++j) { // Loop 0-9 for the 10 right columns
        int actual_col = GRID_COLS - 10 + j; // The real column index
        int prob_idx = 9 - j;                // Index into prob_edge_pattern (0 for the rightmost col (j=9), 9 for the left of this block (j=0))

        // Iterate through rows that are not part of the top or bottom 10 rows
        for (int i = 10; i < GRID_ROWS - 10; ++i) {
            grid->cells[i][actual_col] = (rand() % 100 < prob_edge_pattern[prob_idx]) ? TRANSPARENT : BLACK;
        }
    }
}

