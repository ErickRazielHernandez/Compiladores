a.out: lex.yy.o mips.tab.o main.o
	gcc main.o lex.yy.o mips.tab.o -ll

main.o: main.c mips.tab.h
	gcc -c main.c

lex.yy.o: lex.yy.c mips.tab.h
	gcc -c lex.yy.c

mips.tab.o: mips.tab.c
	gcc -c mips.tab.c

mips.tab.h: mips.y
	bison -d mips.y
	
lex.yy.c: mips.l
	flex mips.l

mips.tab.c: mips.y
	bison -d mips.y

clean:
	rm *.o lex.yy.c mips.tab.c mips.tab.h

run: a.out
	./a.out

test: test.txt
	cat test.txt | ./a.out