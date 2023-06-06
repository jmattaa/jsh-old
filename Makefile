exec = jsh

source = $(wildcard src/*.c)
objects = $(source:.c=.o)

cflags = -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/sysprof-4 -pthread
lflags = -lglib-2.0 

$(exec): $(objects)
	gcc $(lflags) -o $@ $^

%.o: %.c
	gcc $(cflags) -c -o $@ $^

clean:
	@rm $(exec)
