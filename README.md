# Extendible_hashing
Extendible hashing taking input from a file https://en.wikipedia.org/wiki/Extendible_hashing

for complie : gcc 140101072.c -o 140101072 -lm



run : ./140101072 <global depth> <bucket_size> <overflow option>  input.txt > ouput.txt
run : ./meracode <global depth> <bucket_size> <delete option>  input.txt > ouput.txt

overflow option  0 : no overflow
                  1 : overflow same as bucket
delete option   1: lazy delete
                2: bucket merge
                 3: directory merge


