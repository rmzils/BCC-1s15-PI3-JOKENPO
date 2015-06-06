TARGETS = main

CC = gcc

FLAGS = -std=c99

LIBS = -lm -lallegro -lallegro_main -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_image

all: $(TARGETS)

game.o: game.c game.h
	$(CC) $(FLAGS) $< -c

player.o: player.c player.h
	$(CC) $(FLAGS) $< -c

menu.o: menu.c menu.h
	$(CC) $(FLAGS) $< -c

main: main.c game.o player.o menu.o
	$(CC) $(FLAGS) $^ -o $@ $(LIBS)

clean:
	rm -f *~ *.o

destroy: clean
	rm -f $(TARGETS)