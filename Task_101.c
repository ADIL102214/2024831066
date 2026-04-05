#include <SDL2/SDL.h>
#include<stdio.h>

#define FALSE 0
#define TRUE 1
#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 600

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
int game_is_running = FALSE;
int radius = 100;

int initialize_window(){
    if(SDL_Init(SDL_INIT_EVERYTHING)!=0){
        fprintf(stderr, "ERROR INITIALIZING SDL.\n");
        return FALSE;
    }
    window = SDL_CreateWindow(
        "TASK_101",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0
    );
    if(!window){
        fprintf(stderr, "ERROR INITIALIZING SDL Window\n");
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
          break;
    }
}


void draw_circle(SDL_Renderer *renderer, int centerX, int centerY, int radius){
    int x = radius;
    int y = 0;
    int error = 1-radius;
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
    SDL_SetRenderDrawColor(renderer,50,50,0,255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    draw_circle(renderer, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, radius);
    SDL_RenderPresent(renderer);
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
        //update();
        draw();
        SDL_Delay(16);
    }
    destroy_window();
    return 0;
}