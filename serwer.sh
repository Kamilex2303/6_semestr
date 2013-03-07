#!/bin/bash
while(true); do
    if [ -f "dane" ]
    then
	liczba=$(cat dane)
        rm dane
	liczba=$(($liczba+1))
	echo "$liczba" > wyniki
    fi
done

    
