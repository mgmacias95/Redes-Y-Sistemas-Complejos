#
#	MAKEFILE
#

# -g: Compilar
# -c: Permitir depuracion posterior

# For "valgrind -v --leak-check=yes --show-reachable=yes "
#CFLAGS =  -g -c -O0 -Wall

# Normal way
CFLAGS =  -c -O3 -Wall

# Ficheros .c

C = pfnet.c funciones.c routines.c

# Ficheros .o

O = pfnet.o funciones.o routines.o

# Compilacion inicial

.c.o:
	gcc $(CFLAGS) $(C)

# Compilacion final
mst-pathfinder:	pfnet.o $(O)
			gcc -o mst-pathfinder $(O) -lm

clean:
	rm -rf mst-pathfinder *.o
