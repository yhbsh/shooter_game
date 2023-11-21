#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdint.h>

#define HEIGHT 800
#define WIDTH 800
#define VEL 6
#define MAX_KEYBOARD_KEYS 355

static bool keyboard[MAX_KEYBOARD_KEYS] = {0};
static bool quit = false;

typedef struct {
  SDL_Rect rect;
  bool alive;
  int dx, dy;
  SDL_Texture *texture;
} Entity;

int main(void) {
  const char *player_asset_filename = "player.png";
  const char *bullet_asset_filename = "bullet.png";

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, HEIGHT, WIDTH, SDL_WINDOW_OPENGL);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  SDL_Texture *player_texture = IMG_LoadTexture(renderer, player_asset_filename);
  SDL_Texture *bullet_texture = IMG_LoadTexture(renderer, bullet_asset_filename);
  Entity player = {0};
  Entity bullet = {0};
  player.texture = player_texture;
  bullet.texture = bullet_texture;

  while (!quit) {
    // init render context
    SDL_SetRenderDrawColor(renderer, 24, 24, 24, 255);
    SDL_RenderClear(renderer);

    // get texture size
    SDL_QueryTexture(player.texture, NULL, NULL, &player.rect.w, &player.rect.h);
    SDL_QueryTexture(bullet.texture, NULL, NULL, &bullet.rect.w, &bullet.rect.h);

    // render the player
    SDL_RenderCopy(renderer, player.texture, NULL, &player.rect);
    if (bullet.alive)
      SDL_RenderCopy(renderer, bullet.texture, NULL, &bullet.rect);

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

    if (keyboard[SDL_SCANCODE_SPACE]) {
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
