OBJECTS = main.o text.o object.o vector.o physics.o shapes.o target.o
run: all
	./a.out
all: $(OBJECTS)
	gcc $(OBJECTS) `sdl2-config --cflags --libs` -lSDL2_ttf -lSDL2_image -lm -ldl

main.o: main.c defs.h
	gcc -c main.c `sdl2-config --cflags --libds` -lSDL2_ttf

text.o: text.c text.h defs.h
	gcc -c text.c

object.o: object.c object.h defs.h
	gcc -c object.c

vector.o: vector.c vector.h defs.h
	gcc -c vector.c

physics.o: physics.c physics.h defs.h
	gcc -c physics.c

shapes.o: shapes.c shapes.h defs.h
	gcc -c shapes.c

target.o: target.c target.h defs.h
	gcc -c target.c

clean:
	rm *.o