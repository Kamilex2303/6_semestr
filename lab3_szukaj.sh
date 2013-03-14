#!/bin/bash

if [ $# -lt 2 ]
then
  echo "Uzycie: ./szukaj.sh szukany_plik katalog"
  exit 0
fi

if [ ! -d $2 ]
then
  echo "Katalog $2 nie istnieje"
  exit 0
fi

needle=$1
haystack=$2
if [ "$3" == "" ]
then
  first_recursion_level=true
fi

# wypisujemy znalezione pliki i liczymy je
found_count=0
files=`ls -1 $haystack`
for file in $files
do
  if [ "$file" == "$needle" ]
  then
    if [ -f $haystack/$file ]
    then
      echo "Znaleziono plik: $haystack/$file"
    else
      echo "Znaleziono katalog: $haystack/$file"
    fi
    found_count=$[$found_count+1]
  elif [ -d $haystack/$file ]
  then
    $0 $needle $haystack/$file false &
    wait $!
    found_count=$[$found_count+$?]
    #pids=("${pids[@]}" $!)
  fi
done

if [ ! $first_recursion_level ]
then
  exit $found_count
elif [ $found_count -eq 0 ]
then
  echo "Nie znaleziono $needle."
else
  echo "Ilosc znalezionych plikow: $found_count"
fi
