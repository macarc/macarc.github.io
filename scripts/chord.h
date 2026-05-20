#ifndef GUITAR_CHORD_H
#define GUITAR_CHORD_H

#include <stdio.h>

#define LENGTH(ar) (sizeof(ar) / sizeof(ar[0]))

void GenerateChordDiagram(char const* ChordName, char const* ChordNotes, FILE* OutputFile);

#endif
