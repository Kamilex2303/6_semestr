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

plik=$1
katalog=$2
if [ "$3" == "" ]
then
  pierwszy_raz=true
fi

# szukamy pliki; wywolujemy rekurencyjnie
pids=""
ilosc=0
files=`ls -1 $katalog`
for file in $files
do
  if [ "$file" == "$plik" ]
  then
    if [ -f $katalog/$file ]
    then
      echo "Znaleziono plik: $katalog/$file"
    else
      echo "Znaleziono katalog: $katalog/$file"
    fi
    ilosc=$[$ilosc+1]
  elif [ -d $katalog/$file ]
  then
    $0 $plik $katalog/$file xxx &
    pids="$pids $!"
  fi
done

# czekamy na zakonczenie procesow i dodajemy ich kody wyjsciowe do ilosci
for pid in $pids
do
  wait $pid
  ilosc=$[$ilosc+$?]
done

if [ ! $pierwszy_raz ]
then
  exit $ilosc
elif [ $ilosc -eq 0 ]
then
  echo "Nie znaleziono $plik."
else
  echo "Ilosc znalezionych plikow: $ilosc"
fi
