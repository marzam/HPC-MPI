#!/bin/bash
#Execute aqui pelo script.
#Parâmetros
#  ./jacobi m32.txt v32.txt 32 100 1
#            |       |       |  |  |-> salva o vetor X no arquivo solucao.txt
#            |       |       |  |----> quantidade de iterações do método
#            |       |       |-------> tamanho da matriz - matriz quadrada
#            |       |---------------> arquivo com o vetor B
#            |-----------------------> arquivo com o ,atrix A
time  ./jacobi m32.txt v32.txt 32 100 1
