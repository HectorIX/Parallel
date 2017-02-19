#include <stdio.h>
#include <mpi.h>
void main (int argc, char * argv[])
{
  int err, size, rank;
  MPI_Status status;
  float data[2];
  //err = MPI_Init(&argc, &argv);

  err = MPI_Init(&argc, &argv);
  err = MPI_Comm_size(MPI_COMM_WORLD, &size);
  err = MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if ( rank == 0 ) {

    data[0] = 1.0, data[1] = 2.0;
    MPI_Send(data, 2, MPI_FLOAT, 1, 1230, MPI_COMM_WORLD);
    printf("My rank = %d\n", rank );

  } else if ( rank == 1 ) {
    //printf("%d\n", rank );
    MPI_Recv(data, 2, MPI_FLOAT, 0, 1230, MPI_COMM_WORLD, &status);
    //printf(”%d: a[0]=%f a[1]=%f\n”, rank, a[0], a[1]);
    printf("My rank = %d\n%f\n%f\n\n", rank, data[0], data[1] );

  }

  err = MPI_Finalize();

}
