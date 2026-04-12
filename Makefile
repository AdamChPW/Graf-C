.SILENT:
	comp

comp: *.c
	cc -Wall -Wextra --pedantic *.c -o Algorytm -lm