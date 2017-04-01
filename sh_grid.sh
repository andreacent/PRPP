#!/bin/sh

for X in "G0NoRPP" "G1NoRPP" "G2NoRPP" "G3NoRPP" "G4NoRPP" "G5NoRPP" "G6NoRPP" "G7NoRPP" "G8NoRPP" "G9NoRPP" "G10NoRPP" "G11NoRPP" "G12NoRPP" "G13NoRPP" "G14NoRPP" "G15NoRPP"
do
	echo $X
	timeout --signal=SIGINT 120m ./main instanciasPRPP/GRID/$X > resultados/$X.txt
done
