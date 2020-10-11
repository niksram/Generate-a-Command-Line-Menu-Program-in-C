patternfile=hello.dat
menuinput=inputsequence.txt
cfile=A1_PES1201801972.c

output.txt : output.out $(menuinput)
	./output.out < $(menuinput) > output.txt

output.out : output.c
	gcc -o output.out output.c

output.c : menugen.out $(patternfile)
	./menugen.out < $(patternfile) > output.c    

menugen.out : $(cfile)
	gcc -o menugen.out $(cfile)

clean:
	rm -f menugen.out output.out

clear:
	rm -f menugen.out output.out output.c output.txt
