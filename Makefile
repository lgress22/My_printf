.PHONY: run

charcount: my_printf
	gcc -Wall -Wextra -Werror -o my_printf my_printf.c
	


run: my_printf
	./my_printf
	

clean:
	rm -f my_printf