#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include "routines.h"

#define TRUE	1.0
#define FALSE	0.0
#define OUTPUT_MATRIX   1
#define OUTPUT_EDGES    2
#define OUTPUT_ARCS     3


/* VARIABLES */
extern int q;		//parametro q para indicar el n�mero de arcos max que puede tener un camino
extern int r;		//parametro r para indicar la metrica de minkowski a usar
extern int maximo;		//tipo de calculo a efectuar (caminos maximos o minimos)
extern float **pfnet;	//si pfnet[i][j]=TRUE hay un enlace entre los nodos i y j
extern int num_nodos;	//numero de nodos de la matriz
extern float **pesos;		//matriz de pesos asociados a los arcos
extern float **pesos_originales;
extern char **predecessors;		// Matrix of the predecessor nodes (values are 1-based)
extern int is_sym;		// 1 if the original matrix is symmetrical


/* FUNCIONES Y PROCEDIMIENTOS*/
extern float ** Matriz_dinamica_float(int nfil, int ncol);	//reserva memoria para una matriz de flotantes
extern void Libera_matriz_dinamica_float (float **matriz, int nfils);	//libera memoria de una matriz de flotantes
extern char ** Matriz_dinamica_char(int nfil, int ncol);	//reserva memoria para una matriz de char
extern void Libera_matriz_dinamica_char (char **matriz, int nfils);	//libera memoria de una matriz de char
extern void Input(int argc, char *argv[]);	//coge los datos de entrada y los guarda en las respectivas variables
extern void Programacion_Dinamica ();	//Funci�n que resuelve el problema mediante programaci�n dinamica
extern void update_weight(int i, int j, float minimo, int nsaltos);	// Update the weight if a new minimum is obtained
extern void open_net(FILE* fich);
extern int is_symmetrical(void);
extern void save_net(float** pfnet, FILE* fich, int type);

