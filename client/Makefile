SRCS = \
	src/attribute_set.c \
	src/attribute_set_bool.c \
	src/attribute_set_char.c \
	src/attribute_set_float.c \
	src/attribute_set_string.c \
	src/attribute_set_int.c \
	src/connection.c \
	src/data.c \
	src/echo.c \
	src/echo_error.c \
	src/echo_warn.c

OBJS = $(patsubst src/%.c, build/%.o, $(SRCS))




CC = avr-gcc
FLAGS = -Wall -O2

MACROS = \
	-DNDEBUG

INCLUDES =
LIBS =

CFLAGS = $(FLAGS) $(MACROS) $(INCLUDES)




all: librmonitor.a

librmonitor.a: mkdir $(OBJS)
	@ avr-ar -r build/$@ $(OBJS) $(LIBS)

$(OBJS): build/%.o: src/%.c
	@ $(CC) -c $(CFLAGS) -o $@ $<

mkdir:
	@ mkdir -p build

clean:
	@ rm -f rmonitor.a $(OBJS)
