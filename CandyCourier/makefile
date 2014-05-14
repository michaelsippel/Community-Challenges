SRCS = $(shell find -name '*.cpp')
OBJS = $(addsuffix .o,$(basename $(SRCS)))

INCLUDE = -I/usr/include/ -I../../oxygarum/include -Iinclude
LIBDIR  = -L/usr/X11R6/lib
LIBRARIES = -lX11 -lSDL2 -lGL -lGLU -lm -lSDL2_image

CC = g++
CFLAGS = -Wall $(INCLUDE)
OXY = ../../oxygarum/src/oxygarum.a
BIN = CandyCourier

$(BIN): $(OBJS)
	$(CC) -o $@ $^ $(LIBDIR)$(LIBRARIES) $(OXY)

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $^ 

clean:
	rm $(OBJS)
	rm $(BIN)

