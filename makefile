MYps: MYps.c options.c procParser.c
	gcc -o MYps MYps.c options.c procParser.c -Wall -Werror

clean:
	rm -rf *.o MYps