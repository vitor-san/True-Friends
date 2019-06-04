all:
	gcc -o true_friends main.c graph.c user.c -g
run:
	./true_friends
zip:
	zip trab_grafos.zip main.c graph.h graph.c Makefile
