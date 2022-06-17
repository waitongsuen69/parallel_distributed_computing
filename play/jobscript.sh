#!/bin/bash
# Configure the resources required
#SBATCH -n 1                # number of cores
#SBATCH --time=00:05:00     # time allocation, which has the format DD:HH:MM
#SBATCH --gres=gpu:1        # generic resource required (1 GPU)

alldata=(10000 30000 50000 70000 90000)
#alldata=("100" "1000" "10000" "100000")
#echo "in serial"
#for i in "${alldata[@]}"
#do
 # ./serial "$i"
#done

echo "in parallel"
for i in "${alldata[@]}"
do
  echo "\n$i"
  ./parallel "$i"
done
