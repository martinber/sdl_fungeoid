# CC = gcc
# CFLAGS = -g -Wall -Wextra -pedantic -std=c11
# LINKER_OPTS = -lSDL2 -lm
#
# OBJECTS = main.o
# HEADERS =
#
# .c.o:
	# $(CC) $(CFLAGS) -c $<
#
# all: $(OBJECTS)
	# $(CC) *.o -o main $(LINKER_OPTS)
#
# run: all
	# ./main
#
# clear:
	# rm -f *.o main

# ------------------------------------------------
# Generic Makefile
#
# Author: yanick.rochon@gmail.com
# Date  : 2011-08-10
#
# Changelog :
#   2010-11-05 - first version
#   2011-08-10 - added structure : sources, objects, binaries
#                thanks to http://stackoverflow.com/users/128940/beta
#   2017-04-24 - changed order of linker params
# ------------------------------------------------

# project name (generate executable with this name)
TARGET   = sdl_fungeoid

CC       = gcc
# compiling flags here
CFLAGS = -g -Wall -Wextra -pedantic -std=c11 -I.

LINKER   = gcc
# linking flags here
LFLAGS   = -Wall -I. -lm -lSDL2

# change these to proper directories where each file should be
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
