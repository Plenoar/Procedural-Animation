#include<stdio.h>
#include<SDL3/SDL.h>
#include<math.h>

#define WHITE 0xFFFFFFFF // Define white color as RGBA 0xFFFFFFFF

typedef struct 
{
    float x,y;
    float radius;

}CIRCLE;

//function declarations 
void DrawCircle(SDL_Renderer *renderer, CIRCLE circle, SDL_Color color);


static SDL_Renderer *renderer = NULL;
static SDL_Window *window = NULL;
int main(void)
{   

    SDL_CreateWindowAndRenderer("one",1800,1000,0,&window,&renderer);

    SDL_Surface *surface =  SDL_GetWindowSurface(window);

    SDL_Color white = {255, 255, 255, 255};
    
    SDL_Event event;     
    
        float θ = 1;
        float θ1 = 1;
        float θ2 = 1;
    int running = 1;

    while(running)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_EVENT_QUIT)
            {
                running = 0;
            }
        }

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);

                SDL_ClearSurface(surface,0,0,0,0);
        
                CIRCLE circle = {900,500,400};
                DrawCircle(renderer, circle, white);
    
                float a  = circle.x + (circle.radius * cos(θ));
                float b = circle.y + (circle.radius * sin(θ));
                CIRCLE circle1 = {a,b,200};
                DrawCircle(renderer,circle1,white);
    
                // parent circle = circle , child circle = circle 1 
                // connect a line from parent to child 
                SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, white.a);
                SDL_RenderLine(renderer,circle.x,circle.y,circle1.x,circle1.y);

                float c  = circle1.x + (circle1.radius * cos(θ1));
                float d = circle1.y + (circle1.radius * sin(θ1));
                CIRCLE circle2 = {c,d,100};
                DrawCircle(renderer, circle2, white);    

                SDL_RenderLine(renderer,circle1.x,circle1.y,circle2.x,circle2.y);
    
                
                float e  = circle2.x + (circle2.radius * cos(θ2));
                float f = circle2.y + (circle2.radius * sin(θ2));
                CIRCLE circle3 = {e,f,50};
                DrawCircle(renderer, circle3, white);    
                
                SDL_RenderLine(renderer,circle2.x,circle2.y,circle3.x,circle3.y);
    
                SDL_RenderPresent(renderer);
            
                θ += 0.01;
                θ1 += 0.02;
                θ2 += 0.04;
    

                SDL_Delay(16);
        
    }

}

void DrawCircle(SDL_Renderer *renderer, CIRCLE circle, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    
    int x = circle.radius;
    int y = 0;
    int err = 0;

    while (x >= y) {
        SDL_RenderPoint(renderer, circle.x + x, circle.y + y);
        SDL_RenderPoint(renderer, circle.x + y, circle.y + x);
        SDL_RenderPoint(renderer, circle.x - y, circle.y + x);
        SDL_RenderPoint(renderer, circle.x - x, circle.y + y);
        SDL_RenderPoint(renderer, circle.x - x, circle.y - y);
        SDL_RenderPoint(renderer, circle.x - y, circle.y - x);
        SDL_RenderPoint(renderer, circle.x + y, circle.y - x);
        SDL_RenderPoint(renderer, circle.x + x, circle.y - y);

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
