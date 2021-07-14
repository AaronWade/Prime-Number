//MPI program using 4 nodes that finds the number of prime numbers in n
//where n is 25^(rank + 2). Node 0 outputs the time take for each node
//to complete

#include <iostream>
#include <mpi.h>
#include <ctime>

//Prime counter function============================================
int primeCounter(int n){
  int count = 0;

  //loop through all numbers up to n and check if it is a prime numbers
  //if a number is only divisable by itself and 1 it is prime.
  for(int a = 2; a < n; a++){
    bool isPrime = true;
    for(int c = 2; c * c <= a; c++){
      if(a % c == 0){
        isPrime = false;
        break;
      }
    }
    if(isPrime)count++;
  }
  return count;
}

//Function for finding time elapsed during primeCounter================
double durationTime(int rank){
  int power = rank + 2;
  int result = 25;

  //25 ^ (rank + 2)
  while (power != 0){
    result = result * 25;
    power--;
  }

  //get the current time
  double start = MPI_Wtime();
  //calculate all prime numbers of result
  primeCounter(result);
  //get the current time
  double end = MPI_Wtime();

  //get the duration of primeCounter calculation
  double duration = double(end - start);

  return duration;
}

//main=================================================================
int main(int argc, char **argv){
  //initialise MPI
  MPI_Init(NULL, NULL);

  //get the size and rank of the world
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  //variable to hold duration time
  double duration;

  //if the rank of the node is 0 go to master
  if (rank == 0){
    //Get the duration time of primeCounter function
    duration = durationTime(rank);

    //(Address of sent data, Number of elements to be sent, Sent data type, Address of recieved data, Number of elemets to be recieved, Recieved data type, root, comm);
    MPI_Gather(&duration, 1, MPI_DOUBLE, &duration, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    //Only print out duration time at rank 0
    std::cout <<"Rank "<< rank <<" primeCounter duration = " <<duration <<std::endl;
  }
  //if rank is anything other than 0 go to slave
  else{
    duration = durationTime(rank);

    //(Address of sent data, Number of elements to be sent, Sent data type, Address of recieved data, Number of elemets to be recieved, Recieved data type, root, comm);
    MPI_Gather(&duration, 1, MPI_DOUBLE, &duration, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  }

  //finalise MPI and return control to the OS
  MPI_Finalize();
  return 0;
}
