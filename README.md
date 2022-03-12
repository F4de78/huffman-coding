# Huffman encoding

Progetto realizzato per il corso di Teoria dell'informazione ed inferenza

## Scopo

Prendere un file di testo come input e comprimerlo utilizzato la codifica di Huffman per poi osservare il numero di bytes e l'entropia prima e dopo la codifica

## Esecuzione

Il programma é stato scritto in C++, testato su Linux x86_64 e compilato con GCC (g++) con il seguente comando:
```
g++ src/btree.cpp src/huffman.cpp -o huffman
```
E' possibile ottenere stampe di debug aggiungendo il flag ``` -DDEBUG ``` in fase di compilazione

## Funzionamento
### Test
Nella cartella ```input``` sono presenti dei file di testo per testare il funzionamento del programma

### File di output .info
Nel file .info viene inserito il numero di bit utilizzati per comprimere il
file,la codifica utilizzata per comprimere il file nel formato:
carattere:codifica$|$carattere:codifica$|$...
l'entropia, la lumnghezza attesa e le lunghezze (in byte e in bit) del file dato in input e il relativo file compresso

### File di output .huff
Nel file con estensione .huff viene salvato il file di input compresso con la codifica di huffman basata sulla frequenze dei caratteri del file in input.\\
Questo approccio richiede che insieme al file binario contenente il file compresso, sia anche fornito al decodificatore, la codifica utilizzata per la compressione (vedi .info).
Salvare la compressione ottenuta su un file porta a delle complicazioni, perch\'e non si puo' scrivere bit per bit la codifica sul file, in quanto la minima unit\'a scrivibile su un file \'e un byte.
Per ovviare a cio' infondo al file vengono aggiunti dei bit di "padding" per poter raggiungere un byte (se necessario) e viene comunicato insieme al file contenente la codifica utilizzata, il numero di bit effettivi utilizzati per la codifica.

## TODO
- Scrivere un Makefile per la compilazione
- Fare la parte di decodifica

## Licenza

