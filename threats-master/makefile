all: main run

go: test run-test

main: 
	g++ src/main.cpp src/Math.cpp src/Client.cpp -o main.o -ltfhe-spqlios-fma

test:
	g++ src/main-test.cpp src/Math.cpp src/Client.cpp -o main-test.o -ltfhe-spqlios-fma

run:
	./main.o

run-test:
	./main-test.o

val:
	valgrind ./main.o

val-test:
	valgrind ./main-test.o

clean:
	rm main.o