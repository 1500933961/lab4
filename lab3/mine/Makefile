all: calc.tab.c lex.yy.c ast.c
	gcc lex.yy.c calc.tab.c -o calc.exe
lex.yy.c: calc.l
	flex calc.l
calc.tab.c: calc.y
	bison -d calc.y


clean:
	$(RM) *.o *.exe calc.tab.c calc.tab.h lex.yy.c
