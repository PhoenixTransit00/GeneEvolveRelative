#!/bin/sh
#SBATCH --mem=50gb
#SBATCH --ntasks-per-node=10
#SBATCH --nodes=1
#SBATCH --qos=blanca-ibg
#SBATCH --time=48:0:00
#SBATCH --error=/rc_scratch/emsa1620/GeneEvolve/error.txt

ulimit -c 0 > /dev/null 2>&1
 
/work/KellerLab/Emmanuel/GeneEvolve/analyse --maxc-degree $degree \
											--last-generation $generation \
											--path $path \
											--num-generation $numgen
