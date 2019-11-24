max: maxTweeter.o dictionary.o
	gcc -o max maxTweeter.o dictionary.o

maxTweeter.o: maxTweeter.c dictionary.h
	gcc -g -Wall -Werror -c maxTweeter.c

dictionary.o: dictionary.c dictionary.h
	gcc -g -Wall -Werror -c dictionary.c

clean:
	rm -rf max *.o
