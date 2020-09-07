output.txt : output.out inputsequence.txt
	./output.out < inputsequence.txt > output.txt

output.out : output.c
	gcc -o output.out output.c

output.c : menugen.out menu.dat
	./menugen.out < menu.dat > output.c    

menugen.out : A1_PES1201801972.c
	gcc -o menugen.out A1_PES1201801972.c

clean:
	rm -f menugen.out output.out

clear:
	rm -f menugen.out output.out output.c output.txt
