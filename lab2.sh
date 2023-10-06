#!/bin/sh

#1
if [ $# -gt 3 ]
then
    echo "numar corect de parametrii (mai mult de 3): $#"
fi

#2
filename="$1"
dirname="$2"
shift 2

count=0
for arg in "$@"
do
    if test "$arg" -gt 10 
    then
	count=$((count + 1))
    fi
done
echo "numere mai mari ca 10: $count"

#3
sum=0
for arg in "$@"
do
    sum=$((sum + arg))
done
echo "suma numerelor din linia de comanda: $sum"

#4
digitcount=$( echo $sum | wc -m )
digitcount=$((digitcount - 1)) #scad caracterul pt newline
echo "digitcount: $digitcount"

#5
if [ -f "$filename" ]
then
    echo "$count $sum" >"$filename"
fi

#6
for file in "$dir"/*.txt
do
    if [ -f "$filename" ]
    then
       echo "continutul lui $filename: "
       cat "$filename"
    fi
done