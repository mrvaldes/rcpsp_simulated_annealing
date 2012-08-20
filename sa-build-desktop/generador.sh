#!/bin/bash

#~ #Generador de script para correr pruebas de simulated annealing. 
#~ #Permite setear archivos y parámetros de entrada del programa y de los
#~ #archivos de salida con resultados.

RESET=$(which reset)
TIME=$(which time)
TOUCH=$(which touch)
MKDIR=$(which mkdir)
ECHO=$(which echo)
DATE=$(which date)
RM=$(which rm)
READLINK=$(which readlink)
CURRENTDIR=$($READLINK -f ".")
DATADIR="$CURRENTDIR/data/"
OUTPUTDIR="$CURRENTDIR/output/"

#~ #Nombre de la prueba (y directorio donde guardará resultados)
NOMBRE="longsearch60"

#~ #Nombre del script para ejecutar la prueba
BATCHFILE="$NOMBRE.sh"
#~ BATCHFILE="launcher.sh"

#~ #FILES (en DATADIR) con dataset de instancias que serán probadas
#~ farray[0]="bulletin.sm"
#~ farray[0]="j301_1.sm"
farray[1]="j601_2.sm"
#~ farray[2]="j901_1.sm"
#~ farray[3]="j1201_1.sm"

#~ #MAX_it: máximo de iteraciones
#~ iarray[0]=1000
#~ iarray[1]=5000
#~ iarray[2]=10000
#~ iarray[3]=50000
#~ iarray[4]=100000
#~ iarray[3]=1000000
iarray[4]=10000000

#~ #T0: temperatura inicial
#~ tarray[0]=100
#~ tarray[1]=500
#~ tarray[2]=100
#~ tarray[3]=90
#~ tarray[4]=80
#~ tarray[5]=60
tarray[6]=10

#~ #ALPHA: ponderar la Tn+1=ALPHA*Tn por cada MAX_ml iteraciones.
#~ aarray[0]="0.999"
#~ aarray[1]="0.99"
aarray[2]="0.98"
#~ aarray[3]="0.97"
#~ aarray[4]="0.95"
#~ aarray[5]="0.90"
#~ aarray[6]="0.80"
#~ aarray[7]="0.60"

#~ #MAX_ml: # máx. de iteraciones bajo una misma temperatura, 
#~ #asignar acorde al espacio de búsqueda
#~ marray[0]=20
#~ marray[1]=50
#~ marray[2]=100
marray[3]=200
#~ marray[4]=500
#~ marray[1]=75
#~ marray[2]=150
#~ marray[3]=1000

#~ #MAX_rh: # máx. de cadenas sin mejora antes de realizar reheat.
#~ rarray[0]=25
#~ rarray[1]=50
rarray[2]=100
#~ rarray[3]=20
#~ rarray[4]=1000
#~ rarray[5]=5000
#~ rarray[6]=10000

#~ #n: # de ejecuciones con los mismos parámetros
narray=($(seq 1 10))
#~ narray[0]=1

#~ #SEED: semilla para generador de num. aleatorios
S=$($DATE +%M%S%N)
#~ S=1234567
#~ S=1343324020

#~ #Mostrar información de cada iteración del algoritmo
VFLAG=""
#~ VFLAG="-v"

INSTANCEDIR="$OUTPUTDIR$NOMBRE/"
INSTANCEBATCH="$OUTPUTDIR$BATCHFILE"
$MKDIR -p $INSTANCEDIR
$RM -f $INSTANCEBATCH
$TOUCH $INSTANCEBATCH
$ECHO '#!/bin/sh' >> $INSTANCEBATCH

for f in ${!farray[*]} 
do
	for i in ${!iarray[*]}
	do
		for t in ${!tarray[*]}
		do 
			for a in ${!aarray[*]}
			do 
				for m in ${!marray[*]}
				do 
					for r in ${!rarray[*]}
					do 
						for n in ${!narray[*]}
						do 
							FILEPATH="$INSTANCEDIR${farray[$f]}_${tarray[$t]}_${aarray[$a]}_${marray[$m]}_${rarray[$r]}_${iarray[$i]}_${narray[$n]}"
							CMD="$RESET;(exec 2>>$FILEPATH; $TIME $CURRENTDIR/sa -f $DATADIR${farray[$f]} -t ${tarray[$t]} -a ${aarray[$a]} -m ${marray[$m]} -r ${rarray[$r]} -i ${iarray[$i]} $VFLAG > $FILEPATH)"
							$ECHO $CMD >> $INSTANCEBATCH
						done
					done
				done
			done
		done

	done
done
