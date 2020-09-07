output.txt : output.out inputsequence.txt
	./output.out < inputsequence.txt > output.txt

output.out : output.c
	gcc -o output.c output.out

output.c : menugen.out menu.dat
	./menugen.out < menu.dat > output.c    

menugen.out : A1_PES1201801972.c
	gcc -o A1_PES1201801972.c menugen.out
