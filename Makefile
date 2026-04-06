
src_files := $(wildcard src/*.c)
obj_files := $(patsubst src/%.c, obj/%.o, $(src_files))

CFLAGS := -ggdb -Wall -Wextra -Wpedantic -DDEBUG

# note: $^ is all prerequisites, $< is first prerequisite

main: $(obj_files)
	gcc -o $@ $^ $(CFLAGS)

obj/%.o: src/%.c | obj
	gcc -o $@ -c $< $(CFLAGS)

obj:
	mkdir obj

.PHONY: clean depend

clean:
	rm -f main $(wildcard obj/*.o)

depend:
	sed -i '/^# DEPENDENCIES$$/,$$d' Makefile
	echo '# DEPENDENCIES' >> Makefile
	gcc -MM $(src_files) > sed_temp.txt
	@# append build prefix to object file names
	sed -i -E "s#(\w+\.o:)#obj/\1#" sed_temp.txt
	@# remove weird ../ backtrack that gcc does with -MM
	sed -i -E "s#/\w+/\.\./#/#g" sed_temp.txt
	cat sed_temp.txt >> Makefile
	rm -f sed_temp.txt

# DEPENDENCIES
obj/main.o: src/main.c src/tokenizer.h src/node.h
obj/queue.o: src/queue.c src/queue.h src/node.h
obj/tokenizer.o: src/tokenizer.c src/tokenizer.h
