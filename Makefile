
src_files := main.c
obj_files := $(patsubst %.c, obj/%.o, $(src_files))

CFLAGS := -ggdb -Wall -Wextra -Wpedantic

# note: $^ is all prerequisites, $< is first prerequisite

main: $(obj_files)
	gcc -o $@ $^ $(CFLAGS)

obj/%.o: %.c | obj
	gcc -o $@ -c $< $(CFLAGS)

obj:
	mkdir obj

.PHONY: clean

clean:
	rm -f main $(wildcard obj/*.o)
