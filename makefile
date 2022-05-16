all: compile run

compile: 
	g++ sources/main.cpp sources/circuits.cpp sources/client.cpp sources/server.cpp -o main.o -ltfhe-spqlios-fma

run:
	./main.o

val:
	valgrind ./main.o

clean:
	rm main.o