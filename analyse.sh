#!/bin/sh
#SBATCH --mem=50gb
#SBATCH --ntasks-per-node=10
#SBATCH --nodes=1
#SBATCH --qos=blanca-ibg
#SBATCH --time=12:0:00
#SBATCH --output=/rc_scratch/emsa1620/GeneEvolve/output.txt
#SBATCH --error=/rc_scratch/emsa1620/GeneEvolve/error.txt

echo $1

echo $2

echo $3

./analyse --maxc-degree $degree --last-generation $generation --path $path