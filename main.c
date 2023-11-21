#include "SDL2/SDL_render.h"
#include "SDL2/SDL_timer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/_types/_null.h>

#define HEIGHT 800
#define WIDTH 800
#define VEL 6
#define MAX_KEYBOARD_KEYS 355

static bool keyboard[MAX_KEYBOARD_KEYS] = {0};
static bool quit = false;

typedef struct Entity Entity;
struct Entity {
  bool alive;
  int x, y, w, h, dx, dy;
  SDL_Texture *texture;

  Entity *next;
};

static Entity *head = NULL;

static void cap_frame_rate(long *then, float *remainder) {

  long wait = 16 + *remainder;

  *remainder -= (int)*remainder;

  long frameTime = SDL_GetTicks() - *then;

  wait -= frameTime;

  if (wait < 1)
    wait = 1;

  SDL_Delay(wait);

  *remainder += 0.667;

  *then = SDL_GetTicks();
}

int main(void) {
  const char *player_asset_filename = "player.png";
  const char *bullet_asset_filename = "bullet.png";

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, HEIGHT, WIDTH, SDL_WINDOW_OPENGL);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  SDL_Texture *player_texture = IMG_LoadTexture(renderer, player_asset_filename);
  SDL_Texture *bullet_texture = IMG_LoadTexture(renderer, bullet_asset_filename);

  Entity player = {0};
  player.texture = player_texture;

  long then = SDL_GetTicks();
  float remainder = 0;

  while (!quit) {

    // init render context
    SDL_SetRenderDrawColor(renderer, 24, 24, 24, 255);
    SDL_RenderClear(renderer);

    // render player
    SDL_QueryTexture(player.texture, NULL, NULL, &player.w, &player.h);
    SDL_Rect player_rect = {.x = player.x, .y = player.y, .w = player.w, .h = player.h};
    SDL_RenderCopy(renderer, player.texture, NULL, &player_rect);

    // render bullets
    for (Entity *tmp = head; tmp != NULL; tmp = tmp->next) {
      if (!tmp->alive)
        continue;
      SDL_QueryTexture(tmp->texture, NULL, NULL, &tmp->w, &tmp->h);
      SDL_Rect tmp_rect = {.x = tmp->x, .y = tmp->y, .w = tmp->w, .h = tmp->h};
      SDL_RenderCopy(renderer, tmp->texture, NULL, &tmp_rect);
    }

    // handle events
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
        keyboard[event.key.keysym.scancode] = true;

      if (event.type == SDL_KEYUP && event.key.keysym.scancode == SDL_SCANCODE_SPACE)
        keyboard[event.key.keysym.scancode] = false;
    }

    if (keyboard[SDL_SCANCODE_UP] && player.y >= 0) {
      player.y -= VEL;
      if (player.y < 0)
        player.y = 0;
    }

    if (keyboard[SDL_SCANCODE_DOWN] && player.y + player.h <= HEIGHT) {
      player.y += VEL;
      if (player.y + player.h >= HEIGHT)
        player.y -= player.y + player.h - HEIGHT;
    }

    if (keyboard[SDL_SCANCODE_LEFT] && player.x >= 0) {
      player.x -= VEL;
      if (player.x < 0)
        player.x = 0;
    }

    if (keyboard[SDL_SCANCODE_RIGHT] && player.x + player.w <= WIDTH) {
      player.x += VEL;
      if (player.x + player.w >= WIDTH)
        player.x -= player.x + player.w - WIDTH;
    }

    if (keyboard[SDL_SCANCODE_SPACE]) {
      Entity *bullet = malloc(sizeof(Entity));
      memset(bullet, 0, sizeof(Entity));
      bullet->x = player.x;
      bullet->y = player.y;
      bullet->dx = 16;
      bullet->dy = 0;
      bullet->alive = true;

      bullet->texture = bullet_texture;
      SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

      bullet->x += player.w / 2 + bullet->w / 2;
      bullet->y += player.h / 2 - bullet->h / 2;

      if (head == NULL) {
        head = bullet;
      } else {
        Entity *tmp;
        for (tmp = head; tmp->next != NULL; tmp = tmp->next)
          ;
        tmp->next = bullet;
      }
    }

    for (Entity *tmp = head; tmp != NULL; tmp = tmp->next) {
      tmp->x += tmp->dx;
      tmp->y += tmp->dy;

      if (tmp->x > WIDTH)
        tmp->alive = false;
    }

    SDL_RenderPresent(renderer);

    cap_frame_rate(&then, &remainder);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
