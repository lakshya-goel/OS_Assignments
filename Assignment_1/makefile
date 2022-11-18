all: cat date ls mkdir rm main compile

cat: cat.c
	gcc cat.c -o cat

date: date.c
	gcc date.c -o date

ls: ls.c
	gcc ls.c -o ls

mkdir: mkdir.c
	gcc mkdir.c -o mkdir

rm: rm.c
	gcc rm.c -o rm

main: main.c
	gcc main.c -o main -lpthread

compile: main
	./main
