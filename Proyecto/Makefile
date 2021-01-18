lex.yy.c: mips_lex.l
	    flex mips_lex.l

lex.yy.o: lex.yy.c
	    gcc -c lex.yy.c

main.o: main.c
	    gcc -c main.c

a.out: main.o lex.yy.o
	    gcc main.o lex.yy.o -lfl

clean:
	    rm -f main.o lex.yy.o a.out lex.yy.c

run: a.out
	    ./a.out

b.log: a.out a.in
	    ./a.out < a.in > b.log && vim b.log
