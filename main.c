#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define HEIGHT 800
#define WIDTH 800
#define VEL 6

typedef struct {
  SDL_Rect pos;
  bool up, down, left, right;
} Player;

int main(void) {
  const char *filename = "player.png";
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, HEIGHT, WIDTH, SDL_WINDOW_OPENGL);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_Texture *texture = IMG_LoadTexture(renderer, filename);

  SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

  bool quit = false;
  Player player = {0};

  while (!quit) {
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 24, 24, 24, 255);

    SDL_QueryTexture(texture, NULL, NULL, &player.pos.w, &player.pos.h);
    SDL_RenderCopy(renderer, texture, NULL, &player.pos);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      quit = event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q);
      if (quit)
        break;

      if (event.type == SDL_KEYDOWN) {
        if (event.key.repeat == 0) {
          if (event.key.keysym.sym == SDLK_UP) {
            player.up = true;
          }

          if (event.key.keysym.sym == SDLK_DOWN) {
            player.down = true;
          }

          if (event.key.keysym.sym == SDLK_LEFT) {
            player.left = true;
          }

          if (event.key.keysym.sym == SDLK_RIGHT) {
            player.right = true;
          }
        }
      }

      if (event.type == SDL_KEYUP) {
        if (event.key.repeat == 0) {
          if (event.key.keysym.sym == SDLK_UP) {
            player.up = false;
          }

          if (event.key.keysym.sym == SDLK_DOWN) {
            player.down = false;
          }

          if (event.key.keysym.sym == SDLK_LEFT) {
            player.left = false;
          }

          if (event.key.keysym.sym == SDLK_RIGHT) {
            player.right = false;
          }
        }
      }
    }

    if (player.up && player.pos.y >= 0) {
      player.pos.y -= VEL;
      if (player.pos.y < 0)
        player.pos.y = 0;
    }

    if (player.down && player.pos.y + player.pos.h <= HEIGHT) {
      player.pos.y += VEL;
      if (player.pos.y + player.pos.h >= HEIGHT)
        player.pos.y -= player.pos.y + player.pos.h - HEIGHT;
    }

    if (player.left && player.pos.x >= 0) {
      player.pos.x -= VEL;
      if (player.pos.x < 0)
        player.pos.x = 0;
    }

    if (player.right && player.pos.x + player.pos.w <= WIDTH) {
      player.pos.x += VEL;
      if (player.pos.x + player.pos.w >= WIDTH)
        player.pos.x -= player.pos.x + player.pos.w - WIDTH;
    }

    SDL_RenderPresent(renderer);

    SDL_Delay(1000 / 60);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
