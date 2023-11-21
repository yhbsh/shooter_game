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

static int keyboard[MAX_KEYBOARD_KEYS] = {0};

typedef struct {
  SDL_Rect pos;
  bool fire;
} Player;

typedef struct {
  SDL_Rect pos;
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

    SDL_QueryTexture(player_texture, NULL, NULL, &player.pos.w, &player.pos.h);
    SDL_QueryTexture(bullet_texture, NULL, NULL, &bullet.pos.w, &bullet.pos.h);
    SDL_RenderCopy(renderer, player_texture, NULL, &player.pos);
    if (bullet.alive)
      SDL_RenderCopy(renderer, bullet_texture, NULL, &bullet.pos);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      quit = event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_Q);
      if (quit)
        break;

      if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
        keyboard[event.key.keysym.scancode] = 1;

      if (event.type == SDL_KEYUP && event.key.repeat == 0)
        keyboard[event.key.keysym.scancode] = 0;

      if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_SPACE)
        player.fire = true;
    }

    if (keyboard[SDL_SCANCODE_UP] && player.pos.y >= 0) {
      player.pos.y -= VEL;
      if (player.pos.y < 0)
        player.pos.y = 0;
    }

    if (keyboard[SDL_SCANCODE_DOWN] && player.pos.y + player.pos.h <= HEIGHT) {
      player.pos.y += VEL;
      if (player.pos.y + player.pos.h >= HEIGHT)
        player.pos.y -= player.pos.y + player.pos.h - HEIGHT;
    }

    if (keyboard[SDL_SCANCODE_LEFT] && player.pos.x >= 0) {
      player.pos.x -= VEL;
      if (player.pos.x < 0)
        player.pos.x = 0;
    }

    if (keyboard[SDL_SCANCODE_RIGHT] && player.pos.x + player.pos.w <= WIDTH) {
      player.pos.x += VEL;
      if (player.pos.x + player.pos.w >= WIDTH)
        player.pos.x -= player.pos.x + player.pos.w - WIDTH;
    }

    if (player.fire) {
      bullet.pos.x = player.pos.x + player.pos.w / 2 + bullet.pos.w / 2;
      bullet.pos.y = player.pos.y + player.pos.h / 2 - bullet.pos.h / 2;
      bullet.dx = 16;
      bullet.dy = 0;
      bullet.alive = true;
    }

    bullet.pos.x += bullet.dx;
    bullet.pos.y += bullet.dy;

    if (bullet.pos.x > WIDTH)
      bullet.alive = false;

    SDL_RenderPresent(renderer);

    SDL_Delay(1000 / 60);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
