#!/bin/sh

for X in "R0NoRPP" "R1NoRPP" "R2NoRPP" "R3NoRPP" "R4NoRPP" "R5NoRPP" "R6NoRPP" "R7NoRPP" "R8NoRPP" "R9NoRPP" "R10NoRPP"
do
	echo $X
	timeout --signal=SIGINT 120m ./main instanciasPRPP/RANDOM/$X > resultados/$X.txt
done
