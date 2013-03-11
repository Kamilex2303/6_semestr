#!/usr/bin/env python
"""
  Generator drzewa katalogow.
  Uzycie: ./skrypt.py katalog max_glebokosc
       np ./skrypt.py katalog 5
"""

import os,sys,random

if len(sys.argv) < 3:
  print "Uzycie: ./skrypt.py katalog max_glebokosc"
  sys.exit(0)

kat = sys.argv[1]
maks = int(sys.argv[2])

pliki = "abc apple dog script something Wolverine John 123 smuggler xbox playstation leaves other stuff lorem Smith iphone entity universe xxx".split(" ")
katalogi = "box directory category container archive thing set collection array".split(" ")

def utworz_katalog(nazwa,glebokosc):
  if glebokosc >= maks or os.path.exists(nazwa): return
  os.mkdir(nazwa)
  i = 0
  n = random.randrange(0,8)
  while i < n:
    plik = open(nazwa+"/"+random.sample(pliki,1)[0], "w+")
    plik.close()
    i += 1
  i = 0
  n = random.randrange(2,5)
  while i < n:
    utworz_katalog(nazwa+"/"+random.sample(katalogi,1)[0],glebokosc+1)
    i += 1
  
utworz_katalog(kat,0)
