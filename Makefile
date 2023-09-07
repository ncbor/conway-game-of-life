CC := gcc

CFLAGS := `sdl2-config --libs --cflags` -lSDL2_ttf -lm

SRCS := conwaygl.c

EXEC := conwaygl

all: $(EXEC)
$(EXEC): $(SRCS) Makefile
	$(CC) $(SRCS) $(CFLAGS) -o $@
	./$(EXEC)

clean: $(EXEC)
	rm -f $(EXEC)
