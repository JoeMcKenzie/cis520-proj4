#!/bin/bash
#SBATCH --job-name=scorecard_mpi
#SBATCH --output=logs/out_%j.txt
#SBATCH --error=logs/err_%j.txt
#SBATCH --time=00:30:00
#SBATCH --partition=batch.q
#SBATCH --constraint=moles

#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --mem-per-cpu=128M

echo "=============================="
echo "Job ID: $SLURM_JOB_ID"
echo "Nodes: $SLURM_JOB_NUM_NODES"
echo "Tasks: $SLURM_NTASKS"
echo "CPUs per Task: $SLURM_CPUS_PER_TASK"
echo "Mem per CPU: $SLURM_MEM_PER_CPU"
echo "=============================="

# Run with timing
/usr/bin/time -v mpirun -np $SLURM_NTASKS ./mpi_maximum > /dev/null