#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_scancode.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define HEIGHT 800
#define WIDTH 800
#define VEL 6
#define MAX_KEYBOARD_KEYS 355

static bool keyboard[MAX_KEYBOARD_KEYS] = {0};

typedef struct {
  SDL_Rect rect;
  bool fire;
} Player;

typedef struct {
  SDL_Rect rect;
  int dx, dy;
  bool alive;
} Bullet;

int main(void) {
  const char *player_asset_filename = "player.png";
  const char *bullet_asset_filename = "bullet.png";

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, HEIGHT, WIDTH, SDL_WINDOW_OPENGL);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_Texture *player_texture = IMG_LoadTexture(renderer, player_asset_filename);
  SDL_Texture *bullet_texture = IMG_LoadTexture(renderer, bullet_asset_filename);

  SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", player_asset_filename);

  bool quit = false;
  Player player = {0};
  Bullet bullet = {0};

  while (!quit) {
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 24, 24, 24, 255);

    SDL_QueryTexture(player_texture, NULL, NULL, &player.rect.w, &player.rect.h);
    SDL_QueryTexture(bullet_texture, NULL, NULL, &bullet.rect.w, &bullet.rect.h);
    SDL_RenderCopy(renderer, player_texture, NULL, &player.rect);
    if (bullet.alive)
      SDL_RenderCopy(renderer, bullet_texture, NULL, &bullet.rect);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      quit = event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_Q);
      if (quit)
        break;

      if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
        keyboard[event.key.keysym.scancode] = true;

      if (event.type == SDL_KEYUP && event.key.repeat == 0)
        keyboard[event.key.keysym.scancode] = false;

      if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_SPACE)
        player.fire = true;

      if (event.type == SDL_KEYUP && event.key.keysym.scancode == SDL_SCANCODE_SPACE)
        player.fire = false;
    }

    if (keyboard[SDL_SCANCODE_UP] && player.rect.y >= 0) {
      player.rect.y -= VEL;
      if (player.rect.y < 0)
        player.rect.y = 0;
    }

    if (keyboard[SDL_SCANCODE_DOWN] && player.rect.y + player.rect.h <= HEIGHT) {
      player.rect.y += VEL;
      if (player.rect.y + player.rect.h >= HEIGHT)
        player.rect.y -= player.rect.y + player.rect.h - HEIGHT;
    }

    if (keyboard[SDL_SCANCODE_LEFT] && player.rect.x >= 0) {
      player.rect.x -= VEL;
      if (player.rect.x < 0)
        player.rect.x = 0;
    }

    if (keyboard[SDL_SCANCODE_RIGHT] && player.rect.x + player.rect.w <= WIDTH) {
      player.rect.x += VEL;
      if (player.rect.x + player.rect.w >= WIDTH)
        player.rect.x -= player.rect.x + player.rect.w - WIDTH;
    }

    if (player.fire) {
      bullet.rect.x = player.rect.x + player.rect.w / 2 + bullet.rect.w / 2;
      bullet.rect.y = player.rect.y + player.rect.h / 2 - bullet.rect.h / 2;
      bullet.dx = 16;
      bullet.dy = 0;
      bullet.alive = true;
    }

    bullet.rect.x += bullet.dx;
    bullet.rect.y += bullet.dy;

    if (bullet.rect.x > WIDTH)
      bullet.alive = false;

    SDL_RenderPresent(renderer);

    SDL_Delay(1000 / 60);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
