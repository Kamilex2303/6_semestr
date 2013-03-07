#!/bin/bash
echo "Podaj liczbe"
read liczba
echo "$liczba" > dane
while (true); do
    if [ -f "wyniki" ]
    then
	wynik=$(cat wyniki)
	rm wyniki
	echo "$wynik"
	break
    fi
done
