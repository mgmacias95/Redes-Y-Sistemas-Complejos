/*
Objective: to create random .NET files (printed in the output)
Parameters:
- size: the size of the matrix
- symmetry: 0 for a non symmetrical (directed) matrix, 1 for a symmetrical (undirected) matrix
- diagonal_value: the value in the diagonal meaning there is no link (for instance, 0)
- weight_type: 0 for integers, 1 for real numbers
- min/max: range of the random values
- edge_prob: probability p to generate an edge between each two nodes

<program> <size> <symmetry> <diagonal_value> <weight_type> <min> <max> <edge_prob>
*/

#include <stdlib.h>
#include <stdio.h>


#define SIZE_MAX 1000000

#define MAX(X,Y) (((X)>(Y))?(X):(Y))

// Global parameters
int size, symmetry;
float diagonal_value, vmin, vmax, edge_prob;


float aleaf()
{
  return (float)rand()/RAND_MAX;
}

float aleafmm(float min, float max)
{
  return min+(max-min)*aleaf();
}

int aleaimm(int min, int max)
{
  return (int)(min+(max-min)*aleaf());
}

/**************************************************\
Compute a random weighted network using the given parameters.
\**************************************************/
void random_weighted_network(float** tab) {
  float weight;
  int i, j;
  for(i=0;i<size;i++)
  {
    for(j=(symmetry?i:0);j<size;j++)
    {
      if(i==j) weight=diagonal_value;
      else {
            if(aleaf()<=edge_prob) weight=aleafmm(vmin, vmax);
            else weight=diagonal_value;
            }
      tab[i][j] = weight;
      if(symmetry)
        tab[j][i] = weight;
    }
  }
}




int main(int argc, char** argv)
{
  int i, j, type, beg;
  float **tab;

  if(argc!=8)
  {
    fprintf(stderr, "Usage: %s <size> <symmetry> <diagonal-value> <weight-type> <min> <max> <edge-prob>\n", argv[0]);
    exit(1);
  }


//  struct timeval timer;
//  gettimeofday(&timer, 0);
//  srand(timer.tv_usec);

  // Obtain the parameters
  size = atoi(argv[1]);
  symmetry = atoi(argv[2]);
  diagonal_value = atof(argv[3]);
  type = atoi(argv[4]);
  vmin = atoi(argv[5]);
  vmax = atoi(argv[6]);
  edge_prob = atof(argv[7]);

  printf("*vertices %d\n", size);
  for(i=0;i<size;i++)
    printf("%d \"%d\" ellipse x_fact 1.22866290766363 y_fact 1.22866290766363 fos 3.5 bw 0.0  ic Emerald\n", i+1, i+1);

  // Initialize the graph
  tab = (float**) malloc(size*size*sizeof(float));
  for(i=0;i<size;i++)
    tab[i] = (float*) malloc(size*sizeof(float));
  for(i=0;i<size;i++)
    for(j=0;j<size;j++)
       tab[i][j] = 0.0;


  // Compute the matrix
  random_weighted_network(tab);


/************Codigo de io_net.c***************
    case OUTPUT_EDGES:
    // Print the "*edges" matrix
    fprintf(fich, "*edges\n");
    for(i=0; i<num_nodos; i++)
    	for(j=i+1; j<num_nodos; j++)
    		if (pfnet[i][j]!=FALSE)
  		    fprintf(fich, "%d %d %" NDIGITS "f\n",i+1,j+1,pfnet[i][j]);
  break;

**********************************************/

/*************Codigo modificado por mi********/
 // Print the matrix

    // Print the "*edges" matrix
    #define FALSE	0.0

    printf("*edges\n");

    for(i=0; i<size; i++)
     {
      if (symmetry)
         beg=i+1;
      else
         beg=0;

      for(j=beg; j<size; j++)
         if (tab[i][j]!=FALSE)
            if (type==1)
    	       printf("%d %d %.5f\n",i+1,j+1,tab[i][j]);
            else
               printf("%d %d %d\n",i+1,j+1,tab[i][j]);
     }
 /**/



/**************Codigo original de create.c

  // Print the "*matrix" matrix
  //printf("*matrix\n");

  for(i=0;i<size;i++)
  {
    for(j=0;j<size;j++)
    {
      if(type==0)
        printf("%d ", (int)tab[i][j]);
      else
        printf("%f ", tab[i][j]);
    }
    printf("\n");
  }
  */


  for(i=0;i<size;i++)
    free(tab[i]);
  free(tab);

  exit(0);
}
