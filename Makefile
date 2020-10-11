$(menuc) : menugen.out $(menudat)
	./menugen.out < $(menudat) > $(menuc) 

menugen.out : menu_generator.c
	gcc -o menugen.out menu_generator.c

clean:
	rm -f menugen.out