system: clean agora_vai.o
	gcc agora_vai.o -lm -o run
	rm -rf *.o
	./run

main.o: agora_vai.c
	gcc -c agora_vai.c -o agora_vai.o -lm
clean:
	rm -rf *.o

