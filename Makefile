all : zap.o kill.o
	gcc -o zap zap.o kill.o
	

zap: zap.c 
	gcc -c -o zap.o zap.c

kill : kill.c
	gcc -c -o kill.o kill.c

clean:
	rm *.o zap
