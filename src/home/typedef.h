#ifndef TYPEDEF_H
#define TYPEDEF_H

typedef struct 
{   

    double x_pos; // x postition 
    double y_pos; // y position 
    unsigned char wasd; // bit manipulation for events 
}USER;

typedef struct
{
    double width ;
    double height ;
    double time_elapsed;
    double time_step; 
}WORLD;

typedef struct {
    double x, y;       // Camera position
    double width, height;  // Camera viewport size
    double lookahead;   // How much the camera looks ahead
    double deadzone_x;  // Dead zone width (prevents jittery movement)
    double deadzone_y;  // Dead zone height
    int locked;        // If the camera should stay fixed
} CAMERA;


typedef struct 
{
    double x,y;
    double radius;

}CIRCLE;



#endif 