#ifndef GUI_APP_H
#define GUI_APP_H

#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct {
    SDL_Window*   win_image;
    SDL_Renderer* ren_image;
    SDL_Window*   win_panel;
    SDL_Renderer* ren_panel;
    int img_win_w, img_win_h;
    int panel_w,   panel_h;
} GuiApp;

bool gui_init(GuiApp* app, const char* title_image, const char* title_panel,
              int img_w, int img_h, int panel_w, int panel_h);

void gui_shutdown(GuiApp* app);

/* ADICIONE ESTES PROTÓTIPOS */
SDL_Texture* gui_make_texture_from_surface(SDL_Renderer* ren, SDL_Surface* surf);
void gui_draw_texture_fit(SDL_Renderer* ren, SDL_Texture* tex,
                          int win_w, int win_h, int img_w, int img_h);
void gui_draw_panel_background(SDL_Renderer* r, int w, int h);

#endif