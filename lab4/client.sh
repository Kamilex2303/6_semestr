#!/bin/bash
if [ $# -lt 2 ]
then
  echo "Uzycie: ./client.sh serwer liczba"
  exit
fi

if [ ! -p /home/studinf/$1/serwerfifo ]
then
  echo "Serwer nieczynny"
  exit 1
fi

if [ ! -p ~/klientfifo ]
then
  mkfifo ~/klientfifo
fi

echo $HOME $2 > /home/studinf/$1/serwerfifo
liczba=`cat < ~/klientfifo`
echo "wynik: $liczba"
rm -f ~/klientfifo

