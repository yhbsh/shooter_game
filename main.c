#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define HEIGHT 800
#define WIDTH 800

int main(void) {
  const char *filename = "player.png";
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, HEIGHT, WIDTH, SDL_WINDOW_OPENGL);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_Texture *texture = IMG_LoadTexture(renderer, filename);

  SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

  bool quit = false;

  while (!quit) {
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 24, 120, 210, 255);

    SDL_Rect r = {0};
    SDL_QueryTexture(texture, NULL, NULL, &r.w, &r.h);
    SDL_RenderCopy(renderer, texture, NULL, &r);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      quit = event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q);
      if (quit)
        break;
    }

    SDL_RenderPresent(renderer);

    SDL_Delay(1000 / 60);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
