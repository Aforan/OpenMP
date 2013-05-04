all: main.c

main.c:
	mpicc src/*.c -o twit.exe

clean:
	rm twit.exe