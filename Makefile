max: maxTweeter.o dict.o
	gcc -o max maxTweeter.o dict.o

maxTweeter.o: maxTweeter.c dict.h
	gcc -g -c maxTweeter.c

dict.o: dict.c dict.h
	gcc -g -c dict.c

clean:
	rm -rf max *.o
