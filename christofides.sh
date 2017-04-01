#!/bin/sh

for X in "P01NoRPP" "P02NoRPP" "P03NoRPP" "P04NoRPP" "P05NoRPP" "P06NoRPP" "P07NoRPP" "P08NoRPP" "P09NoRPP" "P10NoRPP" "P11NoRPP" "P12NoRPP" "P13NoRPP" "P14NoRPP" "P15NoRPP" "P16NoRPP" "P17NoRPP" "P18NoRPP" "P19NoRPP" "P20NoRPP"
do
	echo $X
	timeout --signal=SIGINT 120m ./main instanciasPRPP/CHRISTOFIDES/$X > resultados/$X.txt
done
