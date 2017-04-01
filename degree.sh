#!/bin/sh

for X in "D0NoRPP" "D1NoRPP" "D2NoRPP" "D3NoRPP" "D4NoRPP" "D5NoRPP" "D6NoRPP" "D7NoRPP" "D8NoRPP" "D9NoRPP" "D10NoRPP" "D11NoRPP" "D12NoRPP" "D13NoRPP" "D14NoRPP"
do
	echo $X
	timeout --signal=SIGINT 120m ./main instanciasPRPP/DEGREE/$X > resultados/$X.txt
done
