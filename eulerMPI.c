// TotientRance.c - Sequential Euler Totient Function (C Version)
// compile: gcc -Wall -O -o TotientRange TotientRange.c
// run:     ./TotientRange lower_num uppper_num

// Greg Michaelson 14/10/2003
// Patrick Maier   29/01/2010 [enforced ANSI C compliance]

// This program calculates the sum of the totients between a lower and an
// upper limit using C longs. It is based on earlier work by:
// Phil Trinder, Nathan Charles, Hans-Wolfgang Loidl and Colin Runciman

#include <stdio.h>
#include <mpi.h>
#include <time.h>


long hcf(long x, long y)
{
  long t;

  while (y != 0) {
    t = x % y;
    x = y;
    y = t;
  }
  return x;
}



int relprime(long x, long y)
{
  return hcf(x, y) == 1;
}


// euler n = length (filter (relprime n) [1 .. n-1])

long euler( long pieceOfWork )
{

  long subSum = 0;
  long index = 0;
  long length, i;

  long arrayOfWork[pieceOfWork];

    for(i=0; i<=pieceOfWork; i++)
      arrayOfWork[i] = 0;


    MPI_Status status;
    MPI_Recv(&arrayOfWork , pieceOfWork+1, MPI_LONG, 0, 0, MPI_COMM_WORLD, &status);


    for( index=0; index<=pieceOfWork; index++ )
    {

      length = 0;
      for( i=0; i< *(arrayOfWork+index); i++)
      {
          if( relprime( *(arrayOfWork+index), i+1 ) )
            length++;
      }

      subSum+=length;
    }


  MPI_Send(&subSum, 1 , MPI_LONG, 0 , 0 , MPI_COMM_WORLD);
}


// sumTotient lower upper = sum (map euler [lower, lower+1 .. upper])

long sumTotient(long lower, long upper, long pieceOfWork)
{
  long i = 0;
  long j = 0;
  long sum = 0;
  long result = 0;
  long arrayOfWork[pieceOfWork];

  MPI_Status status;

  for(i=0; i<=pieceOfWork; i++)
    arrayOfWork[i] = i+1;


 MPI_Send(&arrayOfWork, pieceOfWork+1, MPI_LONG, 1/*rank*/, 0 , MPI_COMM_WORLD);
 MPI_Recv(&result, 1, MPI_LONG, 1/*rank*/ , 0 , MPI_COMM_WORLD, &status);

 sum = sum + result ;

 return sum;
}


int main(int argc, char ** argv)
{

  long lower, upper;
  long sumTotients = 0;
  long pieceOfWork = 0;

  int processes, rank;


  if (argc != 3) {
    printf("not 2 arguments\n");
    return 1;
  }

  sscanf(argv[1], "%ld", &lower);
  sscanf(argv[2], "%ld", &upper);


  MPI_Init(&argc , &argv );

  MPI_Comm_size (MPI_COMM_WORLD, &processes);
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);

  //int rest = ( ( upper-lower )%((upper-lower ) /(processes-1) ) );
  pieceOfWork = (upper-lower) / (processes-1);

  printf("pieceOfWork = %ld\n", pieceOfWork);
  //printf("rest %d\n", rest);

  if (rank == 0)
  {
    sumTotients = sumTotient (lower, upper, pieceOfWork) ;
    printf("\n\t + Sum of Totients  between [%ld..%ld] is %ld\n\n", lower, upper, sumTotients);
  }
  else {

      euler(pieceOfWork);
  }

  MPI_Finalize();

  return 0;
}
