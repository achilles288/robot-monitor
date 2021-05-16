SRCS = \
	src/attribute_set.c \
	src/attribute_set_bool.c \
	src/attribute_set_char.c \
	src/attribute_set_float.c \
	src/attribute_set_string.c \
	src/attribute_set_int.c \
	src/connection.c \
	src/connection_rf24.c \
	src/connection_spi.c \
	src/connection_spi_default.c \
	src/data.c \
	src/echo.c \
	src/echo_error.c \
	src/echo_warn.c

OBJS = $(patsubst src/%.c, build/%.o, $(SRCS))




PREFIX ?= arm-none-eabi-
CC = $(PREFIX)gcc
AR = $(PREFIX)ar
FLAGS = -Wall -O2

MACROS = \
	-DNDEBUG

INCLUDES =
LIBS =

CFLAGS = $(FLAGS) $(MACROS) $(INCLUDES)




all: librmonitor.a

librmonitor.a: mkdir $(OBJS)
	@ $(AR) -r build/$@ $(OBJS) $(LIBS)

$(OBJS): build/%.o: src/%.c
	@ $(CC) -c $(CFLAGS) -o $@ $<

mkdir:
	@ mkdir -p build

clean:
	@ rm -f rmonitor.a $(OBJS)
