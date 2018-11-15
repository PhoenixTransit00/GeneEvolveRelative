# GeneEvolveRelative
This program search for relatives for every pair of individuals in the specified generation of the output file of GeneEvolve.

The outpuf file contains all the pairs of individuals with at least one relative.

The command to run the script with sbathc is:
sbatch --export=degree=7,generation=28,path=/rc_scratch/emsa1620/GeneEvolve/Output_files10000/ analyse.sh
where degree is the highest degree for a relative, generation is the generation in the output of geneevolve to consider, and path is the oath where the output files of GeneEvolve are.

The pseucode is accessible here:
https://fr.overleaf.com/read/mthsfbscwqbc
