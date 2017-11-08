#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "helper.h"
#include <string.h>

void reverse(char *str, int strlenn)
{

	// parallelize this function and make sure to call reverse_str()
	// on each processor to reverse the substring.

	int np, rank;
	int root = 0 ,i, *displs, *scounts;
	char rbuf[strlenn];
	int stringSize,stringMod;
	int pos = 0;
	MPI_Status status;


    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    	stringSize = strlenn/np;
    	stringMod = strlenn % np;
   		displs = (int *)malloc(np*sizeof(int));
        scounts = (int *)malloc(np*sizeof(int));
        for (i=0; i<np; ++i) {
            displs[i] = pos;

            if(i < stringMod)
            scounts[i] = stringSize+1;
            else
            	scounts[i] = stringSize;

            pos += scounts[i];
        }


    	MPI_Scatterv( str, scounts, displs, MPI_CHAR, &rbuf,strlenn, MPI_CHAR,root, MPI_COMM_WORLD);

    	if(rank==0){
    	char *mas;
	    	mas = malloc(sizeof(char)*strlenn);
	    	for(int oo = 1; oo < np; ++oo){
	    		MPI_Recv(mas,10000,MPI_CHAR,MPI_ANY_SOURCE,MPI_ANY_TAG, MPI_COMM_WORLD,&status);
	    		int mrank = status.MPI_SOURCE;
			memcpy(str + strlenn - scounts[mrank]-displs[mrank], mas, scounts[mrank]);
		}
         	 reverse_str(rbuf, scounts[rank]);
		 memcpy(str + strlenn - scounts[rank]-displs[rank], &rbuf, scounts[rank]);
    	}else{
		 reverse_str(rbuf, scounts[rank]);
		 MPI_Send(&rbuf,strlenn,MPI_CHAR,0,0, MPI_COMM_WORLD);
    	}

    	    free(scounts);
    	    free(displs);



}
