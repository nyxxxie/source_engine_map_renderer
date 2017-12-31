CC=g++
CLAGS=-Wall -g
SRCDIR=src/
INCLUDES=-I./include
LIBS=-lglfw -lGL -lGLU -lglut -lpthread -lX11 -lXrandr -lXi -ldl
OBJ=main.o glad.o
OUTFILE=semr

%.o: $(SRCDIR)%.c
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

$(OUTFILE): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS) 

.PHONY: clean
clean:
	rm $(OBJ) $(OUTFILE)
