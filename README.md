How to compile, run, and schedule our code:

Compiling -

  PTHREADS:
  gcc -o pthread_maximum pthread_maximum.c -pthread

  MPI:
  mpicc -o mpi_maximum mpi_maximum.c

  OPENMP:
  gcc -o openmp_maximum openmp_maximum.c -fopenmp

Running -

  PTHREADS:
  ./pthread_maximum <num-threads>

  MPI:
  srun ./mpi_maximum

  OPENMP:
  ./openmp_maximum <num-threads>

Scheduling -

  Each subdirectory has a script submit_tests, which can be run with ./submit_tests
  This will schedule tests for:
  - thread/task configurations with other values fixed 
  - node configurations with other values fixed 
  - memory configurations with other values fixed 

  The data will be stored in a logs/ directory, which can be parsed into a CSV using
  ./data_parser.sh
