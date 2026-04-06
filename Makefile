
src_files := src/main.c src/tokenizer.c
obj_files := $(patsubst src/%.c, obj/%.o, $(src_files))

CFLAGS := -ggdb -Wall -Wextra -Wpedantic

# note: $^ is all prerequisites, $< is first prerequisite

main: $(obj_files)
	gcc -o $@ $^ $(CFLAGS)

obj/%.o: src/%.c | obj
	gcc -o $@ -c $< $(CFLAGS)

obj:
	mkdir obj

.PHONY: clean

clean:
	rm -f main $(wildcard obj/*.o)
