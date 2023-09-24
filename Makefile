burningship: main.o burningship.o burningshipp.o bmpp.o
	gcc -Wall -o burningship bmpp.o burningshipp.o burningship.o main.o -lpthread

bmpp.o: src/bmp/bmpp.c
	gcc -Wall -c -O2 src/bmp/bmpp.c 

burningship.o: src/burningshipd/burningship.c
	gcc -Wall -c -O2 src/burningshipd/burningship.c

burningshipp.o: src/burningshipd/burningshipp.c
	gcc -Wall -c -O1 src/burningshipd/burningshipp.c

main.o: src/main.c
	gcc -Wall -c -O2 src/main.c

clean:
	rm *.o
