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

    // Futuro: fontes, estados de toggle, etc.
} GuiApp;

bool gui_init(GuiApp* app, const char* title_image, const char* title_panel,
              int img_w, int img_h, int panel_w, int panel_h);

void gui_shutdown(GuiApp* app);

// Um tick do loop: processa eventos e desenha
// Retorna false quando o usuário pede para sair
bool gui_frame(GuiApp* app);

#endif