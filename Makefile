all: main.o prompt.o base.o command.o warp.o system.o peek.o proclore.o procs.o
	gcc main.o prompt.o base.o command.o warp.o system.o peek.o proclore.o procs.o -o trash

main.o: main.c
	gcc -c main.c -o main.o

prompt.o: prompt.c
	gcc -c prompt.c -o prompt.o

base.o: base.c
	gcc -c base.c -o base.o

command.o: command.c
	gcc -c command.c -o command.o

warp.o: warp.c
	gcc -c warp.c -o warp.o

system.o: system.c
	gcc -c system.c -o system.o

peek.o: peek.c
	gcc -c peek.c -o peek.o

proclore.o: proclore.c
	gcc -c proclore.c -o proclore.o

procs.o: procs.c
	gcc -c procs.c -o procs.o

clean:
	rm *.o
	rm trash
