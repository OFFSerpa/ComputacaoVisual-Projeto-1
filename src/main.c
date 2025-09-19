#include <stdio.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("%s caminho_da_imagem.ext\n", argv[0]);
        return 1;
    }

    printf("Imagem recebida -> %s\n", argv[1]);
    return 0;
}