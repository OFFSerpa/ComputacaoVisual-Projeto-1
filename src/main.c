// main.c
#include <stdio.h>
#include <string.h>
#include "image_loader.h"
#include "image_analysis.h"
#include "gui_app.h"
#include <SDL2/SDL.h>

int main(int argc, char **argv)
{
    int use_gui = 0;
    const char *image_path = NULL;

    // parse simples
    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "--gui") == 0)
        {
            use_gui = 1;
        }
        else
        {
            image_path = argv[i];
        }
    }

    if (!image_loader_init())
    {
        fprintf(stderr, "Falha ao iniciar SDL2 e SDL_image\n");
        return 1;
    }

    if (!use_gui && image_path == NULL)
    {
        printf("Uso: ./bin/image_loader_demo [--gui] <arquivo_imagem>\n");
        image_loader_cleanup();
        return 0;
    }

    // caminho comum: carregar uma imagem
    ImageData img = {0};
    if (image_path)
    {
        ImageLoadError err = load_image(image_path, &img);
        if (err != IMG_SUCCESS)
        {
            fprintf(stderr, "Erro ao carregar %s\n  %s\n",
                    image_path, get_image_error_string(err));
            image_loader_cleanup();
            return 1;
        }
    }
    else
    {
        fprintf(stderr, "Passe um arquivo de imagem ou use o modo de testes sem argumentos\n");
        image_loader_cleanup();
        return 1;
    }

    // converter para cinza usando seu código atual
    GrayscaleImage gray = {0};
    if (!convert_to_grayscale(&img, &gray))
    {
        fprintf(stderr, "Falha ao converter para grayscale\n");
        free_image_data(&img);
        image_loader_cleanup();
        return 1;
    }

    if (!use_gui)
    {
        ImageAnalysis st = {0};
        // calcula estatísticas a partir da imagem original
        if (!analyze_image(&img, &st))
        {
            fprintf(stderr, "Falha ao analisar a imagem\n");
        }
        else
        {
            printf("  Carregado: %dx%d pixels, %d canais\n", img.width, img.height, img.channels);
            printf("  Tipo: %s\n", get_color_type_string(st.color_type));
            printf("  Intensidade media: %.1f\n", st.avg_intensity);
            int contrast = st.max_intensity - st.min_intensity;
            printf("  Intensidade min: %d\n", st.min_intensity);
            printf("  Intensidade max: %d\n", st.max_intensity);
            printf("  Contraste: %d\n", contrast);
        }

        if (!save_grayscale_image(&gray, "grayscale_images/output_gray.png"))
        {
            fprintf(stderr, "Falha ao salvar grayscale_images/output_gray.png\n");
        }
        else
        {
            printf("  Salvo como: grayscale_images/output_gray.png\n");
        }

        free_grayscale_image(&gray);
        free_image_data(&img);
        image_loader_cleanup();
        return 0;
    }

    // modo GUI
    GuiApp app;
    if (!gui_init(&app, "Imagem", "Painel", 900, 700, 420, 700))
    {
        fprintf(stderr, "Falha ao iniciar GUI\n");
        free_grayscale_image(&gray);
        free_image_data(&img);
        image_loader_cleanup();
        return 1;
    }

    // criar surface a partir do SDL_Surface original já carregado
    // se você já tem img.surface, use direto para textura
    SDL_Texture *tex = gui_make_texture_from_surface(app.ren_image, img.surface);
    int img_w = img.surface->w;
    int img_h = img.surface->h;

    int running = 1;
    while (running)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                running = 0;
            if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE)
                running = 0;
            if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_q || e.key.keysym.sym == SDLK_ESCAPE)
                    running = 0;
                if (e.key.keysym.sym == SDLK_s)
                {
                    if (save_grayscale_image(&gray, "output_image.png"))
                    {
                        printf("Salvo como output_image.png\n");
                    }
                    else
                    {
                        fprintf(stderr, "Falha ao salvar output_image.png\n");
                    }
                }
            }
        }

        int w_img, h_img;
        SDL_GetRendererOutputSize(app.ren_image, &w_img, &h_img);
        SDL_SetRenderDrawColor(app.ren_image, 12, 12, 12, 255);
        SDL_RenderClear(app.ren_image);
        gui_draw_texture_fit(app.ren_image, tex, w_img, h_img, img_w, img_h);
        SDL_RenderPresent(app.ren_image);

        int pw, ph;
        SDL_GetRendererOutputSize(app.ren_panel, &pw, &ph);
        SDL_SetRenderDrawColor(app.ren_panel, 28, 28, 28, 255);
        SDL_RenderClear(app.ren_panel);
        gui_draw_panel_background(app.ren_panel, pw, ph);
        SDL_RenderPresent(app.ren_panel);
    }

    SDL_DestroyTexture(tex);
    gui_shutdown(&app);
    free_grayscale_image(&gray);
    free_image_data(&img);
    image_loader_cleanup();
    return 0;
}