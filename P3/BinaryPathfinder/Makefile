#
#	MAKEFILE
#

# -g: Compilar
# -c: Permitir depuracion posterior

# For "valgrind -v --leak-check=yes --show-reachable=yes "
#CFLAGS =  -g -c -O0

# Normal way
CFLAGS =  -c -O3

# Ficheros .c

C = binary.c routines.c

# Ficheros .o

O = binary.o routines.o

# Compilacion inicial

.c.o:
	gcc $(CFLAGS) $(C)

# Compilacion final
binary-pathfinder:	binary.o $(O)
			gcc -o binary-pathfinder $(O) -lm

clean:
	rm -rf binary-pathfinder *.o
