# Detect OS
ifeq ($(OS),Windows_NT)
    CC = gcc
    CFLAGS = -Wall -Wextra -std=c99 -g
    LIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
    TARGET_EXT = .exe
    RM = del /Q
    MKDIR = mkdir
    SEP = \\
else
    CC = gcc
    CFLAGS = -Wall -Wextra -std=c99 -g
    LIBS = -lSDL2 -lSDL2_image
    TARGET_EXT =
    RM = rm -rf
    MKDIR = mkdir -p
    SEP = /

    # macOS com Homebrew
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
        HOMEBREW_PREFIX := $(shell brew --prefix)
        CFLAGS += -I$(HOMEBREW_PREFIX)/include
        LIBS := -L$(HOMEBREW_PREFIX)/lib $(LIBS)
    endif
endif

# Diretórios
SRCDIR  = src
OBJDIR  = obj
BINDIR  = bin
IMAGEDIR = images

# Includes do projeto
INCLUDES = -I$(SRCDIR) -I$(SRCDIR)/core -I$(SRCDIR)/gui

# Fontes e objetos
SOURCES := $(wildcard $(SRCDIR)/*.c) \
           $(wildcard $(SRCDIR)/core/*.c) \
           $(wildcard $(SRCDIR)/gui/*.c)

# Ex.: src/core/image_loader.c -> obj/core/image_loader.o
OBJECTS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

TARGET  = $(BINDIR)$(SEP)image_loader_demo$(TARGET_EXT)

# Alvo padrão
all: directories $(TARGET)

# Cria diretórios necessários
directories:
ifeq ($(OS),Windows_NT)
	@if not exist "$(OBJDIR)" $(MKDIR) "$(OBJDIR)"
	@if not exist "$(OBJDIR)$(SEP)core" $(MKDIR) "$(OBJDIR)$(SEP)core"
	@if not exist "$(OBJDIR)$(SEP)gui" $(MKDIR) "$(OBJDIR)$(SEP)gui"
	@if not exist "$(BINDIR)" $(MKDIR) "$(BINDIR)"
else
	@$(MKDIR) $(OBJDIR) $(OBJDIR)/core $(OBJDIR)/gui $(BINDIR)
endif

# Link final
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LIBS)
	@echo "Build complete: $(TARGET)"

# Regra genérica de compilação preservando subpastas
# Garante que a subpasta de obj/ exista antes de compilar
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Limpeza
clean:
ifeq ($(OS),Windows_NT)
	@if exist "$(OBJDIR)" rmdir /S /Q "$(OBJDIR)"
	@if exist "$(BINDIR)" rmdir /S /Q "$(BINDIR)"
else
	$(RM) $(OBJDIR) $(BINDIR)
endif
	@echo "Clean complete"

# Instalação de dependências
install-deps-windows:
	pacman -S --needed mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_image

install-deps-vcpkg:
	vcpkg install sdl2 sdl2-image

install-deps-ubuntu:
	sudo apt-get update
	sudo apt-get install -y libsdl2-dev libsdl2-image-dev

install-deps-macos:
	brew install sdl2 sdl2_image

install-deps-arch:
	sudo pacman -S sdl2 sdl2_image

# Executar com uma imagem de teste
test: $(TARGET)
	@echo "Running with test image if available..."
ifeq ($(OS),Windows_NT)
	@if exist "$(IMAGEDIR)\\test.png" ( $(TARGET) "$(IMAGEDIR)\\test.png" ) else \
	if exist "$(IMAGEDIR)\\test.jpg" ( $(TARGET) "$(IMAGEDIR)\\test.jpg" ) else \
	( echo No test image found. Usage: $(TARGET) ^<image_file^> && $(TARGET) )
else
	@if [ -f "$(IMAGEDIR)/test.png" ]; then \
		./$(TARGET) "$(IMAGEDIR)/test.png"; \
	elif [ -f "$(IMAGEDIR)/test.jpg" ]; then \
		./$(TARGET) "$(IMAGEDIR)/test.jpg"; \
	else \
		echo "No test image found. Usage: ./$(TARGET) <image_file>"; \
		./$(TARGET); \
	fi
endif

# Builds convenientes
debug: CFLAGS += -DDEBUG -g3
debug: clean all

release: CFLAGS += -O3 -DNDEBUG
release: clean all

help:
	@echo "Targets:"
	@echo "  all        Build"
	@echo "  debug      Build com símbolos"
	@echo "  release    Build otimizado"
	@echo "  test       Roda com imagem de teste em images/"
	@echo "  clean      Limpa artefatos"
	@echo "  install-deps-*  Instala SDL2 e SDL2_image"

.PHONY: all clean directories test debug release help \
        install-deps-windows install-deps-vcpkg install-deps-ubuntu install-deps-macos install-deps-arch