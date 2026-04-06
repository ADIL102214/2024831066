#include<SDL2/SDL.h>
#include<stdio.h>
#include<math.h>

#define FALSE 0
#define TRUE 1
#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 600
#define RADIUS 50

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
int game_is_running = FALSE;
float circle1_x = -RADIUS;
float circle1_y = WINDOW_HEIGHT/2.0f;
float circle2_x = RADIUS;
float circle2_y = RADIUS;
int collision = 0;

int initialize_window(){
    if(SDL_Init(SDL_INIT_EVERYTHING)!=0){
        fprintf(stderr, "ERROR INITIALIZING SDL.\n");
        return FALSE;
    }
    window = SDL_CreateWindow(
        "Task_103",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0
    );
    if(!window){
        fprintf(stderr, "ERROR INITIALIZING SDL WINDOW\n");
        return FALSE;
    }
    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );
    if(!renderer){
        fprintf(stderr, "Error creating SDL renderer\n");
        return FALSE;
    }
    return TRUE;
}

void process_input(){
    SDL_Event event;
    SDL_PollEvent(&event);
    switch(event.type){
    case SDL_QUIT:
      game_is_running = FALSE;
      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE) game_is_running = FALSE;
      if (event.key.keysym.sym == SDLK_RIGHT) circle2_x+=20.0f;
      if (event.key.keysym.sym == SDLK_LEFT) circle2_x-=20.0f;
      if (event.key.keysym.sym == SDLK_UP) circle2_y-=20.0f;
      if (event.key.keysym.sym == SDLK_DOWN) circle2_y+=20.0f;
      break;
    }
}

void draw_circle(SDL_Renderer *renderer, int centerX, int centerY, int radius){
    int x = RADIUS;
    int y = 0;
    int error = 0;
    while(x>=y){
        SDL_RenderDrawPoint(renderer, centerX+x, centerY-y);
        SDL_RenderDrawPoint(renderer, centerX+x, centerY+y);
        SDL_RenderDrawPoint(renderer, centerX-x, centerY-y);
        SDL_RenderDrawPoint(renderer, centerX-x, centerY+y);
        SDL_RenderDrawPoint(renderer, centerX+y, centerY-x);
        SDL_RenderDrawPoint(renderer, centerX+y, centerY+x);
        SDL_RenderDrawPoint(renderer, centerX-y, centerY-x);
        SDL_RenderDrawPoint(renderer, centerX-y, centerY+x);
        if(error<=0){
            y++;
            error+=2*y+1;
        }else if(error>0){
            x--;
            y++;
            error+=2*y-2*x+1;
        }
    } 
}

void draw(){
    SDL_SetRenderDrawColor(renderer,255,195,105,255);
    SDL_RenderClear(renderer);
    if(collision%2 == 0){
        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        draw_circle(renderer, (int)circle1_x, (int)circle1_y, RADIUS);
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        draw_circle(renderer, (int)circle2_x, (int)circle2_y, RADIUS);
    }else if(collision%2 != 0){
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        draw_circle(renderer, (int)circle1_x, (int)circle1_y, RADIUS);
        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        draw_circle(renderer, (int)circle2_x, (int)circle2_y, RADIUS);
    }
    SDL_RenderPresent(renderer);
}

void update(){
    circle1_x += 3.0f;
    if(circle1_x > WINDOW_WIDTH+RADIUS) circle1_x = -RADIUS;

    if(pow((2*RADIUS),2) >= pow((circle2_x-circle1_x),2)+pow((circle2_y-circle1_y),2)){
        collision++ ;
        circle2_x = RADIUS;
        circle2_y = RADIUS;
    }
    if (circle2_x < RADIUS) circle2_x = RADIUS;
    if (circle2_x > WINDOW_WIDTH - RADIUS) circle2_x = WINDOW_WIDTH - RADIUS;
    if (circle2_y < RADIUS) circle2_y = RADIUS;
    if (circle2_y > WINDOW_HEIGHT - RADIUS) circle2_y = WINDOW_HEIGHT - RADIUS;
}

void destroy_window(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* argv[]){
    game_is_running = initialize_window();
    while(game_is_running){
        process_input();
        draw();
        update();
        SDL_Delay(16);
    }
    destroy_window();
    return 0;
}