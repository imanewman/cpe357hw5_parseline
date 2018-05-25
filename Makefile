CC = gcc
CFLAGS = -Wall -pedantic -g
OBJS = parseline.o parsefuncs.o
TOBJS = tests.o parseline.o parsefuncs.o
DEPS = parseline.h
NAME = parseline
UNIX4 = tnewma03@unix4.csc.calpoly.edu:~/cpe357/hw5
all : $(NAME)

$(NAME) : $(OBJS) $(DEPS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -lm

parseline.o : parseline.c parseline.h
	$(CC) $(CFLAGS) -c parseline.c

parsefuncs.o : parsefuncs.c parseline.h
	$(CC) $(CFLAGS) -c parsefuncs.c

tests.o : tests.c parseline.h
	$(CC) $(CFLAGS) -c tests.c

testm : $(DEPS) $(TOBS)
	$(CC) $(CFLAGS) -o tests $(TOBJS) -lm

testr : 
	./tests

testv :
	valgrind ./tests

grader :
	~getaylor-grader/tryAsgn5 > graded.txt

ssh : 
	ssh $(UNIX4)

scpf :
	scp ./* $(UNIX4)

scpm :
	scp *.c $(UNIX4)

handin : 
	handin getaylor-grader 357hw5-11 Makefile *.c parseline.h README

clean :
	rm *.o $(NAME)

#ctr x ctr a

