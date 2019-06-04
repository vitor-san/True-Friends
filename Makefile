all:
	gcc -o true_friends graph.c -g
run:
	./true_friends
zip:
	zip trab_grafos.zip graph.c Makefile
