#include "gui_app.h"
#include <stdio.h>

bool gui_init(GuiApp* app, const char* title_image, const char* title_panel,
              int img_w, int img_h, int panel_w, int panel_h) {
    if (!app) return false;
    app->win_image = SDL_CreateWindow(title_image,
                                      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                      img_w, img_h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!app->win_image) {
        fprintf(stderr, "Erro ao criar janela imagem: %s\n", SDL_GetError());
        return false;
    }
    app->ren_image = SDL_CreateRenderer(app->win_image, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!app->ren_image) {
        fprintf(stderr, "Erro ao criar renderer imagem: %s\n", SDL_GetError());
        return false;
    }

    app->win_panel = SDL_CreateWindow(title_panel,
                                      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                      panel_w, panel_h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!app->win_panel) {
        fprintf(stderr, "Erro ao criar janela painel: %s\n", SDL_GetError());
        return false;
    }
    app->ren_panel = SDL_CreateRenderer(app->win_panel, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!app->ren_panel) {
        fprintf(stderr, "Erro ao criar renderer painel: %s\n", SDL_GetError());
        return false;
    }

    app->img_win_w = img_w; app->img_win_h = img_h;
    app->panel_w   = panel_w; app->panel_h = panel_h;
    return true;
}

void gui_shutdown(GuiApp* app) {
    if (!app) return;
    if (app->ren_image) SDL_DestroyRenderer(app->ren_image);
    if (app->win_image) SDL_DestroyWindow(app->win_image);
    if (app->ren_panel) SDL_DestroyRenderer(app->ren_panel);
    if (app->win_panel) SDL_DestroyWindow(app->win_panel);
}

bool gui_frame(GuiApp* app) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            return false;
        }
        if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE) {
            // Se qualquer janela fechar, encerramos
            return false;
        }
        // Futuro: teclado, redimensionamento, etc.
    }

    // Limpa e apresenta janelas
    SDL_SetRenderDrawColor(app->ren_image, 20, 20, 20, 255);
    SDL_RenderClear(app->ren_image);
    SDL_RenderPresent(app->ren_image);

    SDL_SetRenderDrawColor(app->ren_panel, 30, 30, 30, 255);
    SDL_RenderClear(app->ren_panel);
    SDL_RenderPresent(app->ren_panel);

    return true;
}