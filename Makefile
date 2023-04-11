exec = jsh

source = $(wildcard src/*.c)
objects = $(source:.c=.o)

cflags = 
lflags = 

$(exec): $(objects)
	gcc $(lflags) -o $@ $^

%.o: %.c
	gcc $(cflags) -c -o $@ $^

clean:
	@rm $(exec)
