#!/bin/bash

#verific daca sunt specificati 2 parametri
if [ $# -ne 2 ]
then
    echo "Folosire: $0 <director> <caracter>!"
    exit 1
fi

director=$1
output_file=$2
total_caractere=0

#verific daca directorul exista
if [ ! -d "$director" ]
then
    echo "Directorul specificat nu exista."
    exit 1
fi

#golesc fisierul de output
>output_file.txt

#iterez prin toate fisierele.txt din director si subdirectoare
for fisier in "$director"/*.txt
do
    if [ -f "$fisier" ]
    then
	nr_caractere_per_fisier=$( wc -m <"$fisier" )
	(( nr_caractere_per_fisier-- ))
	total_caractere=$(( total_caractere + nr_caractere_per_fisier ))
        echo "$fisier - $nr_caractere_per_fisier" >>"$output_file"
    fi
done

echo >>"$output_file"
echo "$2">>"$output_file"
echo "TOTAL $total_caractere" >>"$output_file"