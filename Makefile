system: clean main.o
	gcc main.o -lm -o run
	rm -rf *.o
	./run

main.o: main.c
	gcc -c main.c -o main.o -lm
clean:
	rm -rf *.o
