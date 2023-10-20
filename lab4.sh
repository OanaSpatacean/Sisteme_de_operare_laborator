#!/bin/bash

#cat mail.txt | grep -E "^[a-zA-Z0-9\.\-\_]+@[a-zA-Z]+\.[a-z]{2,3}$"

#cat propozitii.txt | grep -E "^[A-Z][A-Za-z0-9\,\ ]*\.{1}$" | grep -E -v "n[pb]" | grep -E -v "si[ ]*\," | grep -E -v "Si[ ]*\,"


while read line
do
    if [ "$(echo $line | grep -E "^[A-Z][A-Za-z0-9\,\ ]*\.{1}$" | grep -E -v "n[pb]" | grep -E -v "si[ ]*\," | grep -E -v "Si[ ]*\," )" ]
    then
	echo "it works"
    else
	echo "doesn't work"
    fi
done
