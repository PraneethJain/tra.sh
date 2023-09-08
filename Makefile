CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRCS = src/main.c src/commands/activities.c src/commands/bg.c src/commands/fg.c src/commands/pastevents.c src/commands/peek.c src/commands/ping.c src/commands/proclore.c src/commands/seek.c src/commands/system.c src/commands/warp.c src/utils/base.c src/utils/command.c src/utils/signals.c src/utils/procs.c src/utils/prompt.c 
OBJS = $(SRCS:.c=.o)
EXE = trash

DBGDIR = debug
DBGEXE = $(DBGDIR)/$(EXE)
DBGOBJS = $(addprefix $(DBGDIR)/, $(OBJS))
DBGCFLAGS = -g -O0 -fsanitize=address,undefined -D DEBUG

RELDIR = release
RELEXE = $(RELDIR)/$(EXE)
RELOBJS = $(addprefix $(RELDIR)/, $(OBJS))
RELCFLAGS = -O3

all: prep release

debug: $(DBGEXE)

$(DBGEXE): $(DBGOBJS)
	$(CC) $(CFLAGS) $(DBGCFLAGS) -o $(DBGEXE) $^

$(DBGDIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $(DBGCFLAGS) -o $@ $<

release: $(RELEXE)

$(RELEXE): $(RELOBJS)
	$(CC) $(CFLAGS) $(RELCFLAGS) -o $(RELEXE) $^

$(RELDIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $(RELCFLAGS) -o $@ $<

prep:
	@mkdir -p $(DBGDIR) $(RELDIR)
	@mkdir -p $(DBGDIR)/src $(RELDIR)/src
	@mkdir -p $(DBGDIR)/src/commands $(RELDIR)/src/commands
	@mkdir -p $(DBGDIR)/src/utils $(RELDIR)/src/utils

remake: clean all

clean:
	rm -f $(RELEXE) $(RELOBJS) $(DBGEXE) $(DBGOBJS)

