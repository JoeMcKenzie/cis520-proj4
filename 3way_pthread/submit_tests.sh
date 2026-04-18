#!/bin/bash

mkdir -p logs/cores logs/mem logs/nodes

cores_list=(1 2 4 8 16)
mem_list=(64M 128M 512M 1G 3G)
node_list=(1 2 4 8)

repeats=5   # number of runs per configuration

# -------------------------
# CORES TEST
# -------------------------
for cores in "${cores_list[@]}"
do
  for run in $(seq 1 $repeats)
  do
    sbatch \
      --nodes=1 \
      --cpus-per-task=$cores \
      --mem-per-cpu=512M \
      --output="logs/cores/out_cores${cores}_run${run}_%j.txt" \
      --error="logs/cores/err_cores${cores}_run${run}_%j.txt" \
      run_pthread.sh
  done
done

# -------------------------
# MEMORY TEST
# -------------------------
for mem in "${mem_list[@]}"
do
  for run in $(seq 1 $repeats)
  do
    sbatch \
      --nodes=1 \
      --cpus-per-task=8 \
      --mem-per-cpu=$mem \
      --output="logs/mem/out_mem${mem}_run${run}_%j.txt" \
      --error="logs/mem/err_mem${mem}_run${run}_%j.txt" \
      run_pthread.sh
  done
done

# -------------------------
# NODE TEST
# -------------------------
for nodes in "${node_list[@]}"
do
  for run in $(seq 1 $repeats)
  do
    sbatch \
      --nodes=$nodes \
      --cpus-per-task=8 \
      --mem-per-cpu=512M \
      --output="logs/nodes/out_nodes${nodes}_run${run}_%j.txt" \
      --error="logs/nodes/err_nodes${nodes}_run${run}_%j.txt" \
      run_pthread.sh
  done
done
