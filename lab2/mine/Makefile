objects = rd_parser.c
edit : $(objects)
	gcc -o test $(objects) lex.yy.c -lfl
rd_check.o : token.h node_type.h


.PHONY : clean
clean :
	-rm test