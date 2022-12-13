CFLAGS += -g -Wall -Werror -Wextra
all: corrector

corrector: archivo.o hash.o PalabraStruct.o utils.o glist.o main.o Reglas.o Validas.o 
	$(CC) $(CFLAGS) $^ -o $@ 

clean:
	rm -f *.o
	rm -f corrector

.PHONY: clean all
