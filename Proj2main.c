// Your Name:  Joseph Camarena
// Your email: JC0588321@swccd.edu
// Your ID:    0588321

#include <stdio.h>  	// printf
#include <stdlib.h>   // malloc
#include <string.h>   // memset
#include <time.h>	    // srand(time)

int **malloc2d( FILE* inFile );
int* malloc1d();
void calcSums(int** array2d, int* sums );
void print_sumListMinPath( int* sumList );
void free2d(int** array2d );
int ncols, nrows;

//max number of characters to read in a line.
//MAXN=5 means 4 chars are read, then a \0 added in fgets.  See reference for functionality

#define  MAXN  100L
int main(int argc, char* argv[])
{
    char lineRead[MAXN];
    double dvalRead;
    // file to be read must be in project directory
    FILE* inFile = fopen(argv[1], "r");

    if( inFile == NULL) {
		printf("file not found \n");
		exit(1);
    }

    fscanf(inFile,"%s",lineRead);	//String 'NCOLS' from data
    fscanf(inFile,"%d",&ncols);		//data value of NCOLS from data

    fscanf(inFile,"%s",lineRead);	//String 'NROWS' from data
    fscanf(inFile,"%d",&nrows);		//data value of NROWS from data

/************** READ:  xllcorner  and  -123.9417****************/
    fscanf(inFile,"%s",lineRead);
    fscanf(inFile,"%lf",&dvalRead);
/************** READ:  yllcorner  and  32.991666666667 ***********/
    fscanf(inFile,"%s",lineRead);
    fscanf(inFile,"%lf",&dvalRead);
/************** READ:  cellsize  and   0.016666666667***********/
    fscanf(inFile,"%s",lineRead);
    fscanf(inFile,"%lf",&dvalRead);


  int** array2d = malloc2d( inFile );
  int* sumList = malloc1d();

  calcSums( array2d, sumList );

  print_sumListMinPath( sumList );

  free2d( array2d );
  free( sumList );
  fclose( inFile );

  return 0;

}// end main

int** malloc2d( FILE* inFile ) {

  int** array2d = (int **)malloc(nrows * sizeof(int*));

  int i,j;
  for ( i=0; i<nrows; i++ ) {

  	array2d[i] = (int *)malloc(ncols * sizeof(int));

  	for ( j = 0; j < ncols; j++) {

	  	fscanf( inFile, "%d", &array2d[i][j]);

    }
  }

  return array2d;

}

int* malloc1d() {

  int* sumList = (int *)malloc(nrows * sizeof(int));

  memset(sumList, 0, nrows*sizeof(int));

  return sumList;

}

void print_sumListMinPath( int* sumList ) {

  int min = sumList[0];
  int savedRow = 0;
  int row;

  for( row=1; row<nrows; row++ ) {

  	if( sumList[row] < min) {

	  	min = sumList[row];
	  	savedRow = row;

    }

  }//end for loop

  printf("Lowest elevation-change sum: %d\n", min);
  printf("Starting row used to calculate sum: %d\n", savedRow);

}

void free2d( int** array2d ){

  int row;

  for( row=0;row<nrows; row++ ) {

    free( array2d[row] );

  }

  free(array2d);

}

/*
 *  Receives 2d matrix that represents topography
 *  and uses this data to calculate an elevation path sum for each
 *  row.
 *  Input:  array2d[][]
 *  Output: sum[]
 *
 */
void calcSums(int** array2d, int* sums) {

   int row;

   for( row=0; row<nrows; row++ ) {

    int cost = 0;
    int j, i = row;
    int diff_top, diff_fwd, diff_btm;

    for( j=0 ; j<ncols-1 ; j++ ) {

      //if(on first row)
      if( i == 0 ) {

        diff_fwd = abs(array2d[i][j] - array2d[i][j+1]);
        diff_btm = abs(array2d[i][j] - array2d[i+1][j+1]);

        if( diff_fwd == diff_btm || diff_fwd<diff_btm) {

          cost += diff_fwd;

        }

        else {

          cost += diff_btm;
          i++;

        }

      }//end if(on first row)

      //if(on last row)
      else if( i == nrows - 1) {

        diff_top = abs(array2d[i][j] - array2d[i-1][j+1]);
        diff_fwd = abs(array2d[i][j] - array2d[i][j+1]);

        if( diff_fwd == diff_top || diff_fwd<diff_top) {

          cost += diff_fwd;

        }

        else {

          cost += diff_top;
          i--;

        }

      }//end if(on last row)

      //if(in middle rows)
      else {

        diff_top = abs(array2d[i][j] - array2d[i-1][j+1]);
        diff_fwd = abs(array2d[i][j] - array2d[i][j+1]);
        diff_btm = abs(array2d[i][j] - array2d[i+1][j+1]);

        /*
         * flag1 == top and fwd values are equal
         * flag2 == top and bottom values are equal
         * flag3 == forward and bottom values are equal
         * init all flags to 0 (false)
         */

        int flag1 = 0, flag2 = 0, flag3 = 0;
        int flag_count = 0;

       //check to see if the values are equal
       if( diff_top==diff_fwd) {

         flag1 = 1;
         flag_count++;

       }

       if( diff_top==diff_btm ) {

         flag2 = 1;
         flag_count++;

       }

       if( diff_fwd==diff_btm ) {

         flag3 = 1;
         flag_count++;

       }

       // if there are ties with forward value or fwd
       // value is the minimum, move forward
       if( flag_count>=2 || flag1 || flag3 || (diff_fwd<diff_top && diff_fwd<diff_btm)) {

         cost += diff_fwd;

       }

       // if there's a tie between top and bottom values,
       // select a random movement up/down
       else if( flag2 ) {

         srand(time(NULL)); //seeds the random function
         cost += diff_top;  //can also use diff_btm as they're equal

         if( rand()%2 == 1) {

           i--;     //path goes upwards

         }
         else {

           i++;     //path goes downward

         }
       }

       //if top value is the minimum, move up
       else if( diff_top<diff_fwd && diff_top<diff_btm) {

         cost += diff_top;
         i--;

       }

       //if bottom value is the miminum, move down
       else {

         cost += diff_btm;
         i++;

       }

      }//end if(in middle rows)

    }//end COLS traversal

		sums[row] = cost;  //places sums in this array. One sum per row

	}//end row traversal

}//end calcSums function
