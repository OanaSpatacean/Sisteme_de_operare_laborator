#!/bin/bash

#verifica numărul de argumente
if [ $# -ne 1 ]; then
  echo "Utilizare: <caracter>";
  exit -1;
fi

#caracterul specificat de utilizator
c=$1
#contorul de propoziții corecte
nr_propozitii_corecte=0

#citeste liniile de la intrarea standard
while read -r linie
do #verifica dacă linia este corectă
    if [[ $linie =~ ^[A-Z] ]] #verifica daca linia incepe cu litera mare
    then
        if [[ $linie =~ ^[A-Za-z0-9\,\ \.!\?]+$ ]] #verifica daca linia contine doar caractere alfanumerice, spatii, virgule, puncte, semne de exclamare sau semne de intrebare
        then
            if [[ $linie =~ [!\?\.]$ ]] #verifica daca linia se termina cu semnul intrebarii, semnul exclamarii sau punct
            then
                if ! [[ $linie =~ ,\ *si ]]  #verifica daca linia nu contine virgula inainte de "si"
                then
                    if [[ $linie == *$c* ]] #verifica daca linia contine caracterul specificat
                    then
                        nr_propozitii_corecte=$((nr_propozitii_corecte + 1));
                    fi
                fi
            fi
        fi
    fi
done

#returneaza numărul de propoziții corecte
exit $nr_propozitii_corecte;