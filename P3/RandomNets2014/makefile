#
#	MAKEFILE
#

# -g: Compilar
# -c: Permitir depuracion posterior

# For "valgrind -v --leak-check=yes --show-reachable=yes "
#CFLAGS =  -g -c -O0

# Normal way
CFLAGS =  -c -O4

# Ficheros .c

C = create-mod.c

# Ficheros .o

O = create-mod.o

# Compilacion inicial

.c.o:
	gcc $(CFLAGS) $(C)

# Compilacion final
genera_red_ponderada_aleatoria:	create-mod.o $(O)
			gcc -o create-mod $(O) -lm

clean:
			rm -rf create-mod *.o
