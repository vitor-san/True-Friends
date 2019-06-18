all:
	gcc -o true_friends main.c user.c graph.c tuple.c -g
run:
	./true_friends
debug:
	gdb true_friends
zip:
	zip trab_grafos.zip main.c user.h user.c graph.h graph.c Makefile
