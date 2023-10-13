#!/bin/bash

#verific daca sunt specificati 2 parametri
if [ $# -ne 2 ]
then
    echo "Folosire: $0 <director> <caracter>!"
    exit 1
fi

director=$1
caracter=$2

#verific daca directorul exista
if [ ! -d "$director" ]
then
    echo "Directorul specificat nu exista."
    exit 1
fi

#iterez prin toate fisierele.txt din director si subdirectoare
for fisier in "$director"/*
do
    if [ -f "$fisier" ]
    then
	echo "Fisier obisnuit: $fisier"
	if [[ "$fisier" == *.txt ]]
	then
	    echo "Fisier .txt: $fisier"
	   #verific caracterul specificat si setez dreptul corespunzator
	   if [ "$caracter" = "r" ]
	   then
	       chmod +r "$fisier"
	       echo "read"
	   elif [ "$caracter" = "w" ]
	   then
	       chmod +w "$fisier"
	       echo "write"
	   elif [ "$caracter" = "x" ]
	   then
	       chmod +x "$fisier"
	       echo "execute"
	   else
	       echo "Caracterul specificat nu este valid (r,w,x)."
	       exit 1
	   fi
	fi
    elif [ -d "$fisier" ]
    then
        bash "$0" "$fisier" "$2"	
    fi
done

echo "Drepturile au fost setate cu succes pentru toate fisierele .txt din director."