TARGET   = sdl_fungeoid
CC       = gcc
LINKER   = gcc

# I add the SDL2 directory so I can #include <SDL.h> instead of
# #include <SDL2/SDL.h>. Because on the Android version I don't have the headers
# inside a SDL2/ directory.
CFLAGS = -g -Wall -Wextra -pedantic -std=c11 -isystem /usr/include/SDL2/

LFLAGS   = -Wall -lm -lSDL2 -lSDL2_image -lSDL2_ttf

# Directories
SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm       = rm -f


$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $(OBJECTS) $(LFLAGS) -o $@
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

.PHONY: clean
clean:
	@$(rm) $(OBJECTS)
	@echo "Cleanup complete!"

.PHONY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
	@echo "Executable removed!"

run: $(BINDIR)/$(TARGET)
	$(BINDIR)/$(TARGET)
