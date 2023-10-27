#!/bin/bash

#verific daca sunt specificati 2 parametri
if [ $# -ne 1 ]
then
    echo "Folosire: $0 <director>!"
    exit 1
fi

director=$1
countLegaturiSimbolice=0
rez=0

#verific daca directorul exista
if [ ! -d "$director" ]

then
    echo "Directorul specificat nu exista."
    exit 1
fi

#iterez prin toate fisierele.txt din director si subdirectoare
for fisier in "$director"/*
do
    if [ -L "$fisier" ]
    then
        (( countLegaturiSimbolice++ ))
    elif [ -f "$fisier" ]
    then
	#echo "Fisier obisnuit: $fisier"
	if [[ "$fisier" == *.txt ]]
	then
	    #echo "Fisier .txt: $fisier"
	    if [ "$(cat $fisier | grep -E "^[A-Z][A-Za-z0-9\,\ ]*\.{1}$" | grep -E -v "n[pb]" | grep -E -v "si[ ]*\," | grep -E -v "Si[ ]*\," )" ]	       
	    then
		echo "$fisier" >>outputLab5.txt
	    fi
	fi
    elif [ -d "$fisier" ]
    then
        rez=`bash "$0" "$fisier"`
	    countLegaturiSimbolice=$(( countLegaturiSimbolice + rez ))
    fi
done

echo "$countLegaturiSimbolice"
