sources := $(filter %.c,$(shell ls src))
objects := $(foreach word,$(subst .c,.o,$(sources)),obj/$(word))
sources := $(foreach word,$(sources),src/$(word))

.DELETE_ON_ERROR:
all: main

main: $(objects) main.c
	gcc main.c $(objects) -o main	

obj/%.o: src/%.c include/%.h
	gcc $< -o $@ -c

.PHONY: clean
clean:
	rm $(wildcard obj/*.o) main

run: main
	@./main
