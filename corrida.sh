#!/bin/sh

for X in "P01NoRPP" "P02NoRPP" "P03NoRPP"
do
	echo $X
	timeout --signal=SIGINT 150m ./main instanciasPRPP/CHRISTOFIDES/$X > resultados/$X.txt
done