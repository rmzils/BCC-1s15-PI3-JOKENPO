TARGETS = exemplo

CC = gcc

FLAGS = -std=c99

LIBS = -lm -lopencv_core -lopencv_highgui -lallegro -lallegro_main -lallegro_primitives

all: $(TARGETS)

camera.o: camera.c camera.h
	$(CC) $(FLAGS) $< -c

imagem.o: imagem.c imagem.h
	$(CC) $(FLAGS) $< -c

movimento.o: movimento.c movimento.h
	$(CC) $(FLAGS) $< -c

calibragem.o: calibragem.c calibragem.h
	$(CC) $(FLAGS) $< -c

disjoint.o: disjoint.c disjoint.h
	$(CC) $(FLAGS) $< -c

pilha.o: pilha.c pilha.h
	$(CC) $(FLAGS) $< -c

connected.o: connected.c connected.h
	$(CC) $(FLAGS) $< -c

jogada.o: jogada.c jogada.h
	$(CC) $(FLAGS) $< -c

exemplo: exemplo.c camera.o imagem.o movimento.o disjoint.o pilha.o connected.o calibragem.o jogada.o
	$(CC) $(FLAGS) $^ -o $@ $(LIBS)

clean:
	rm -f *~ *.o

destroy: clean
	rm -f $(TARGETS)