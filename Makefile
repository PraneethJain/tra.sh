all : src/main.o src/utils/base.o src/utils/command.o src/utils/procs.o src/utils/prompt.o src/commands/pastevents.o src/commands/peek.o src/commands/proclore.o src/commands/system.o src/commands/warp.o
	gcc src/main.o src/utils/*.o src/commands/*.o -o ./trash

src/main.o : src/main.c
	gcc -c src/main.c -o src/main.o

src/utils/base.o : src/utils/base.c
	gcc -c src/utils/base.c -o src/utils/base.o

src/utils/command.o : src/utils/command.c
	gcc -c src/utils/command.c -o src/utils/command.o

src/utils/procs.o : src/utils/procs.c
	gcc -c src/utils/procs.c -o src/utils/procs.o

src/utils/prompt.o : src/utils/prompt.c
	gcc -c src/utils/prompt.c -o src/utils/prompt.o

src/commands/pastevents.o : src/commands/pastevents.c
	gcc -c src/commands/pastevents.c -o src/commands/pastevents.o

src/commands/peek.o : src/commands/peek.c
	gcc -c src/commands/peek.c -o src/commands/peek.o

src/commands/proclore.o : src/commands/proclore.c
	gcc -c src/commands/proclore.c -o src/commands/proclore.o

src/commands/system.o : src/commands/system.c
	gcc -c src/commands/system.c -o src/commands/system.o

src/commands/warp.o : src/commands/warp.c
	gcc -c src/commands/warp.c -o src/commands/warp.o

clean:
	rm -rf src/*.o src/utils/*.o src/commands/*.o trash
