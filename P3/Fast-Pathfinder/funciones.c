#include "global.h"
#include "extern.h"

#define ENTRADA_1 "*vertices %i"
#define ENTRADA_2 "*Vertices %i"

/*
Note: it is possible that a bug avoid the use of the q parameter
(if q is different to n-1, the results are not the one expected)
*/


/**********************************************************************************/
/***************** LEE LOS VALORES DESDE EL FICHERO DE ENTRADA ********************/
/**********************************************************************************/

void Input(int argc, char *argv[]){

  FILE *fich;
  int i, j, k;
  char cadena[256];
  float aux;
  float minimo = -1.0;

  if(argc>5){
	printf("\n[ERROR] - Formato de entrada incorrecto.\n\t\t ./pfnet [-|fichero_de_entrada] [q [r [maximo]]]\n");
	printf("\t\t q: para indicar el n�mero de arcos max que puede tener un camino\n");
	printf("\t\t r: metrica de minkowski (1: la suma, 0: infinito)\n");
	printf("\t\t maximo: si maximo es 1 se calculan los caminos maximos y no se cambia la matriz de adyancia original. Si es 0, se calculan los caminos minimos (como en el Pathfinder original) y se cambia el signo de los pesos en la matriz de adyacencia\n");
	exit(1);
	}

  if (argc==1 || !strcmp(argv[1],"-"))
      fich=stdin;
  else
      {
	fich = fopen(argv[1],"r");
	if(fich==NULL){
	printf("\n[ERROR] - Apertura del fichero %s incorrecta.\n", argv[1]);
	exit(2);
	}
      }


  int ret = fscanf(fich, ENTRADA_1, &num_nodos);	//lee el numero de nodos
  printf(ENTRADA_1, num_nodos);		    //Mostramos por pantalla el numero de nodos

  if(argc >= 3)
  	q = atoi(argv[2]);
  else
  	q = num_nodos-1;

  if(argc >= 4)
  	r = atoi(argv[3]);
  else
  	r = 0;

  if(argc == 5)
  	maximo = atoi(argv[4]);
  else
  	maximo = 0;
	
  for (i=0; i<num_nodos+1;i++){		//obviamos la informacion de cada nodo
  	char* ret = fgets(cadena, 255, fich);
	printf("%s", cadena);
	}

#if ORIGINAL == 2
  // Allocation of predecessors[][]
  //reserva de memoria para las matrices de predecessors
  predecessors = Matriz_dinamica_char(num_nodos, num_nodos);
#endif

  // Allocation of pesos[][]
  //reserva de memoria para las matrices de pesos, distancias y pfnet
  pesos = Matriz_dinamica_float(num_nodos, num_nodos);

  saltos = Matriz_dinamica_int (num_nodos, num_nodos);

  // Allocation of pfnet[][] and pesos_originales[][]
  pfnet = Matriz_dinamica_float(num_nodos, num_nodos);
  pesos_originales = Matriz_dinamica_float(num_nodos, num_nodos);

  for(i=0; i<num_nodos; i++)		//Inicializamos la matriz de pesos
  	for(j=0; j<num_nodos; j++){
		pesos_originales[i][j] = 0.0;
		}

  // Open the .NET file
  open_net(fich);

  for(i=0;i<num_nodos;i++)		//Inicializamos la matriz pfnet[][]
  	for(j=0;j<num_nodos;j++){
		pfnet[i][j] = pesos_originales[i][j];
	}



  // Initialisation of the symmetry indicator
  is_sym = is_symmetrical();

  fclose(fich);
}


/***************************************************************************************/
/***************************** PROGRAMACI�N DINAMICA ***********************************/
/***************************************************************************************/

/***
Funci�n que selectiona el problema de caminos maximos o minimos.
***/

void Programacion_Dinamica_caminos_min_o_max (void){
	if(maximo==1)
		Programacion_Dinamica_caminos_maximos();
	else
		Programacion_Dinamica_caminos_minimos();
}



/***

Algorithm Floyd-Warshall

Modified to take account of the maximal path value corresponding to the
minimum edge value

Useful variables:
q - value of 'q' in the PFNet Algorithm
num_nodos - number of nodes
pfnet[][] - used to return the final matrix
pesos_originales[][] - original weight matrix (eq. to W(1) or D(1))
pesos[][] - weight matrix W

***/
void Programacion_Dinamica_caminos_maximos (void){
  int i,j,k;
  int beg;

  // Initialisation of pesos[][]
  for(i=0; i<num_nodos; i++){		//Inicializamos la matriz de pesos
  	for(j=0; j<num_nodos; j++){
		pesos[i][j] = pesos_originales[i][j];

		if (i==j){
			saltos[i][j]=0;
		} else {
			saltos[i][j]=1;
		}
	}
  }


  // Compute the weight matrix for each iteration
  // The result is stored in pesos[][]
  // Memory is saved by "forgetting" the weight matrix for iteration 1...(k-1) when we
  // are at the iteation k ( matrix (k) is needed for the computation of matrix (k) )

	for(k=0;k<num_nodos;k++)
	{
		for(i=0;i<num_nodos;i++)
		{
			if(i != k) {
			
				beg=i+1;	// Optimization
				for(j=beg;j<num_nodos;j++)
				{
				
					if(j != k) {

						// Modified Floyd-Warshall
						// Based on : pesos[k+1][i][j] = MAX( pesos[k][i][j] , MIN(pesos[k][i][k],pesos[k][k][j]) );

						//update_weight_maximum(i, j, k, pesos_originales[i][k], pesos[k][j], 1+saltos[k][j]);	// WAY 1 ( w(1) + w(k) )	// (0 % error)
						//update_weight_maximum(i, j, k, pesos[i][k], pesos_originales[k][j], saltos[i][k]+1);	// WAY 2 ( w(k) + w(1) )	// (0 % error)
						update_weight_maximum(i, j, k, pesos_originales[i][k], pesos_originales[k][j], 2);	// WAY 3 ( w(1) + w(1) )	// If removed, 3.1098153 % error
						update_weight_maximum(i, j, k, pesos[i][k], pesos[k][j], saltos[i][k]+saltos[k][j]);	// WAY 4 ( w(k) + w(k) )	// If removed, 14.0077821 % error

					} // j != k

				}  // for j

				//pesos[i][i] = 0;	// Optimization

			} // i != k
		} // for i

	}  // for k

	//Calculo de la PFNET a partir de las matrices de pesos y distancias
	// The "distancias[][]" matrix is replaced by pesos[][]
	// THIS PART IS FASTER THAN THE SECOND PART
	for(i=0;i<num_nodos;i++)
	{
		// Start at (i+1) if the matrix is symmetrical, or 0 if not.
		beg = (is_sym==1)?i+1:0;

		for(j=beg;j<num_nodos;j++){
			if(pesos[i][j]==pesos_originales[i][j] && pesos_originales[i][j]!=0.0)
				pfnet[i][j]=pesos_originales[i][j];
			else if (pesos_originales[i][j]!=0.0)
				pfnet[i][j] = FALSE;
		}
	}
}



void Programacion_Dinamica_caminos_minimos (void){
  int i,j,k;
  int beg;

  // Initialisation of pesos[][]
  for(i=0; i<num_nodos; i++){		//Inicializamos la matriz de pesos
  	for(j=0; j<num_nodos; j++){
		pesos[i][j] = -1*pesos_originales[i][j];

		if (i==j){
			saltos[i][j]=0;
		} else {
			saltos[i][j]=1;
		}
	}
  }


  // Compute the weight matrix for each iteration
  // The result is stored in pesos[][]
  // Memory is saved by "forgetting" the weight matrix for iteration 1...(k-1) when we
  // are at the iteation k ( matrix (k) is needed for the computation of matrix (k) )

	for(k=0;k<num_nodos;k++)
	{
		for(i=0;i<num_nodos;i++)
		{
			if(i != k) {
				beg=i+1;	// Optimization
				for(j=beg;j<num_nodos;j++)
				{

					if(j != k) {
						// Modified Floyd-Warshall
						// Based on : pesos[k+1][i][j] = MAX( pesos[k][i][j] , MIN(pesos[k][i][k],pesos[k][k][j]) );
						
						//update_weight_minimum(i, j, k, pesos_originales[i][k], pesos[k][j], 1+saltos[k][j]);	// WAY 1 ( w(1) + w(k) )	// (0 % error)
						//update_weight_minimum(i, j, k, pesos[i][k], pesos_originales[k][j], saltos[i][k]+1);	// WAY 2 ( w(k) + w(1) )	// (0 % error)
						update_weight_minimum(i, j, k, pesos_originales[i][k], pesos_originales[k][j], 2);	// WAY 3 ( w(1) + w(1) )	// If removed, 3.1098153 % error
						update_weight_minimum(i, j, k, pesos[i][k], pesos[k][j], saltos[i][k]+saltos[k][j]);	// WAY 4 ( w(k) + w(k) )	// If removed, 14.0077821 % error

					
					} // j != k

				}  // for j

				//pesos[i][i] = 0;	// Optimization
			} // i != k
		} // for i

	}  // for k

	//Calculo de la PFNET a partir de las matrices de pesos y distancias
	// The "distancias[][]" matrix is replaced by pesos[][]
	// THIS PART IS FASTER THAN THE SECOND PART
	for(i=0;i<num_nodos;i++)
	{
		// Start at (i+1) if the matrix is symmetrical, or 0 if not.
		beg = (is_sym==1)?i+1:0;
		for(j=beg;j<num_nodos;j++){
			if(pesos[i][j]==-1*pesos_originales[i][j] && pesos_originales[i][j]!=0.0)
				pfnet[i][j]=pesos_originales[i][j];
			else if (pesos_originales[i][j]!=0.0)
				pfnet[i][j] = FALSE;
		}
	}
}


/***

Test if a new minimum is obtained, and update the corresponding weight.

***/

void update_weight_maximum(int i, int j, int k, float weight_ik, float weight_kj, int salto_alternativo) {

	float distancia_alternativa;
	
	//fprintf(stderr, "[%d,%d,%d] minimo = %f\n", i, j, k, minimo);
				
	//fprintf(stderr, "[%d,%d,%d] nsaltos = saltos[%d][%d]+saltos[%d][%d] = %d + %d = %d\n", i, j, k,
	//				i, k, k, j,
	//				saltos[i][k], saltos[k][j],
	//				nsaltos);
		
	if (salto_alternativo<=q) {
		if (r==1)
			distancia_alternativa = weight_ik + weight_kj;
		else // r=infinito
			distancia_alternativa = MIN(weight_ik,weight_kj);

		if (distancia_alternativa > pesos[i][j]) {
			pesos[i][j] = pesos[j][i] = distancia_alternativa;
			saltos[i][j] = saltos[j][i] = salto_alternativo;
		}
	} // if (salto_alternativo<=q)
}

void update_weight_minimum(int i, int j, int k, float weight_ik, float weight_kj, int salto_alternativo) {

	float distancia_alternativa;
	
	if (salto_alternativo<=q) {
		if (r==1)
			distancia_alternativa = weight_ik + weight_kj;
		else // r=infinito
			distancia_alternativa = MAX(weight_ik,weight_kj);

		if (distancia_alternativa < pesos[i][j]) {
			pesos[i][j] = pesos[j][i] = distancia_alternativa;
			saltos[i][j] = saltos[j][i] = salto_alternativo;
		}

	} // if (salto_alternativo<=q)
}


/*********************************************************************************/
/************************ RESERVA DINAMICA DE MATRICES REALES ********************/
/*********************************************************************************/


float * * Matriz_dinamica_float(int nfils, int ncols)
{

   float **matriz;
   int i,j,error=0;
   /*reservo memoria para un vector de nfil punteros a enteros largos*/
   if((matriz=(float **)calloc(nfils,sizeof(float *)))==NULL){
       printf("Error en la asignacion de memoria");
       exit(3);}

   else
   /*para cada componente del vector (puntero a entero largo) reservo memoria para
   un vector de ncols de enteros largos*/
       for(i=0;(i<nfils) && !error;i++)
          if((matriz[i]=(float*)calloc(ncols,sizeof(float)))==NULL)
          {
             printf("Error en la asignacion de memoria\n");
             error=1;
             for(j=i-1;j>=0;j--)   /* si no se puede reservar, libero la memoria que*/
                free(matriz[j]);   /* habia reservado previamente */
             free(matriz);
             matriz=NULL;          /* igualo el puntero que controla la matriz a NULL */
          }
    return(matriz);                /* devuelvo el puntero que controla la matriz*/
}


void Libera_matriz_dinamica_float (float **matriz, int nfils)
{
   int i;

   for(i=0;i<nfils;i++)
	free (matriz[i]);
   free (matriz);
}


char * * Matriz_dinamica_char(int nfils, int ncols)
{

   char **matriz;
   int i,j,error=0;
   /*reservo memoria para un vector de nfil punteros a enteros largos*/
   if((matriz=(char **)calloc(nfils,sizeof(char *)))==NULL){
       printf("Error en la asignacion de memoria");
       exit(3);}

   else
   /*para cada componente del vector (puntero a entero largo) reservo memoria para
   un vector de ncols de enteros largos*/
       for(i=0;(i<nfils) && !error;i++)
          if((matriz[i]=(char*)calloc(ncols,sizeof(char)))==NULL)
          {
             printf("Error en la asignacion de memoria\n");
             error=1;
             for(j=i-1;j>=0;j--)   /* si no se puede reservar, libero la memoria que*/
                free(matriz[j]);   /* habia reservado previamente */
             free(matriz);
             matriz=NULL;          /* igualo el puntero que controla la matriz a NULL */
          }
    return(matriz);                /* devuelvo el puntero que controla la matriz*/
}


void Libera_matriz_dinamica_char (char **matriz, int nfils)
{
   int i;

   for(i=0;i<nfils;i++)
	free (matriz[i]);
   free (matriz);
}


int * * Matriz_dinamica_int(int nfils, int ncols)
{

	int **matriz;
	int i,j,error=0;
	/*reservo memoria para un vector de nfil punteros a enteros largos*/
	if((matriz=(int **)calloc(nfils,sizeof(int *)))==NULL){
		printf("Error en la asignacion de memoria");
		exit(3);}

	else
	/*para cada componente del vector (puntero a entero largo) reservo memoria para
	 un vector de ncols de enteros largos*/
		for(i=0;(i<nfils) && !error;i++)
			if((matriz[i]=(int*)calloc(ncols,sizeof(int)))==NULL)
			{
				printf("Error en la asignacion de memoria\n");
				error=1;
				for(j=i-1;j>=0;j--)   /* si no se puede reservar, libero la memoria que*/
					free(matriz[j]);   /* habia reservado previamente */
				free(matriz);
				matriz=NULL;          /* igualo el puntero que controla la matriz a NULL */
			}
    return(matriz);                /* devuelvo el puntero que controla la matriz*/
}


void Libera_matriz_dinamica_int (int **matriz, int nfils)
{
	int i;

	for(i=0;i<nfils;i++)
		free (matriz[i]);
	free (matriz);
}
