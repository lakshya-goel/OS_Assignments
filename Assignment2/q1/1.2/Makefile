all: first second third q compile

first: first.sh
	chmod u+x first.sh

second: second.sh
	chmod u+x second.sh
  
third: third.sh
	chmod u+x third.sh

q: q2.c
	gcc q2.c -o q

compile: q
	./q
