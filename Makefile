src := $(shell cd src && find . -name '*.c' -or -name '*.s')
obj := $(addprefix obj/,$(src:.c=.o))
headers := $(shell find headers -name '*.h')

all: main

main: $(obj) main.c $(headers)
	gcc main.c $(obj) -o main

obj/%.o: src/%.c
	gcc $< -o $@ -c

.PHONY: clean
clean:
	@rm -f main *.o obj/*.o

run: main
	@./main
