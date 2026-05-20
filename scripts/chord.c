#include "chord.h"

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ============================================
// Constant definitions
// ============================================

#define STRING_COUNT 6

// Frets at which the diagram can start.
const int POSSIBLE_LOWEST_FRETS[] = {0, 2, 4, 6, 8, 11, 14};

// Minimum number of frets to draw.
const int MINIMUM_FRET_COUNT = 5;

// Dimensions
const float MARGIN = 15.0f;
const float HEADER_HEIGHT = 20.0f;
const float WIDTH_PER_STRING = 15.0f;
const float HEIGHT_PER_FRET = 20.0f;
const float FINGER_RADIUS = 6.5f;
const float MUTE_X_FONT_SIZE = 15.0f;
const float FRET_NUMBER_FONT_SIZE = 12.0f;

const float WIDTH = 5.0f * WIDTH_PER_STRING;

// ============================================
// Utility function definitions
// ============================================

int imax(int a, int b) { return a > b ? a : b; }
int imin(int a, int b) { return a < b ? a : b; }

// ============================================
// Function declarations
// ============================================

void GenerateChordDiagram(char const* ChordName, char const* ChordNotes, FILE* OutputFile);

void ParseChordWithNoSpaces(char const* ChordName, int Chord[STRING_COUNT]);
void ParseChordWithSpaces(char const* ChordName, int Chord[STRING_COUNT]);

char* GenerateChordID(char const* ChordName);
void GetFretLayout(int const Chord[STRING_COUNT], int* FretCount, int* LowestFret);
char const* StringIndexToName(int StringIndex);

void PrintAriaTitleAndDescription(FILE* OutputFile, int const Chord[STRING_COUNT], const char* ChordID, const char* ChordName);
void PrintHeader(FILE* OutputFile, int const Chord[STRING_COUNT], char const* ChordName);
void PrintFretNumber(FILE* OutputFile, int LowestFret);
void PrintFretGrid(FILE* OutputFile, int const Chord[STRING_COUNT], int FretCount, int LowestFret);
void PrintChordFingers(FILE* OutputFile, int const Chord[STRING_COUNT], int LowestFret);

// ============================================
// Function definitions
// ============================================

void GenerateChordDiagram(char const* ChordName, char const* ChordNotes, FILE* OutputFile) {
  int Chord[STRING_COUNT] = {0};

  if (strlen(ChordNotes) == STRING_COUNT) {
    ParseChordWithNoSpaces(ChordNotes, Chord);
  } else {
    ParseChordWithSpaces(ChordNotes, Chord);
  }

  int FretCount, LowestFret;
  GetFretLayout(Chord, &FretCount, &LowestFret);

  char* ChordID = GenerateChordID(ChordName);

  float TotalWidth = WIDTH + 2 * MARGIN;
  float TotalHeight = 2 * MARGIN + HEADER_HEIGHT + FretCount * HEIGHT_PER_FRET;

  fprintf(OutputFile,
          "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 %f %f\" "
          "role=\"img\" aria-labelledby=\"title-%s desc-%s\">\n",
          TotalWidth, TotalHeight, ChordID, ChordID);

  PrintAriaTitleAndDescription(OutputFile, Chord, ChordID, ChordName);
  PrintHeader(OutputFile, Chord, ChordName);
  PrintFretNumber(OutputFile, LowestFret);
  PrintFretGrid(OutputFile, Chord, FretCount, LowestFret);
  PrintChordFingers(OutputFile, Chord, LowestFret);

  fprintf(OutputFile, "</svg>\n");

  free(ChordID);
}

// Returns a malloc'ed string.
char* GenerateChordID(char const* ChordName) {
  srand(time(NULL));

  int ChordNameLen = strlen(ChordName);
  int RandomDigitCount = 10;
  int IDLen = ChordNameLen + RandomDigitCount + 2;

  char* ChordID = malloc(IDLen);
  strcpy(ChordID, ChordName);
  ChordID[ChordNameLen] = '-';
  for (int i = 0; i < RandomDigitCount; ++i) {
    int Digit = rand() % 10;
    ChordID[ChordNameLen + i + 1] = Digit + '0';
  }
  ChordID[IDLen - 1] = '\0';
  return ChordID;
}

// ChordName and Chord must both have length STRING_COUNT.
void ParseChordWithNoSpaces(char const* ChordName, int Chord[STRING_COUNT]) {
  for (int i = 0; i < STRING_COUNT; ++i) {
    if (ChordName[i] == 'x') {
      Chord[i] = -1;
    } else {
      Chord[i] = ChordName[i] - '0';
    }
  }
}

void ParseChordWithSpaces(char const* ChordName, int Chord[STRING_COUNT]) {
  char const* CurrentString = ChordName;
  for (int i = 0; i < STRING_COUNT; ++i) {
    while (isspace(*CurrentString) && *CurrentString != '\0') {
      ++CurrentString;
    }

    if (*CurrentString == 'x') {
      Chord[i] = -1;
      ++CurrentString;
    } else {
      char const* NextString;
      long ChordValue = strtol(CurrentString, (char**)&NextString, 10);

      Chord[i] = (int)ChordValue;

      CurrentString = NextString;
    }
  }
}

void GetFretLayout(int const Chord[STRING_COUNT], int* FretCount, int* LowestFret) {
  int minimum = INT_MAX;
  int maximum = 0;

  for (int i = 0; i < STRING_COUNT; ++i) {
    if (Chord[i] > 0) {
      minimum = imin(Chord[i], minimum);
      maximum = imax(Chord[i], maximum);
    }
  }

  int PossibleLowestFretsCount = LENGTH(POSSIBLE_LOWEST_FRETS);

  for (int i = 0; i < PossibleLowestFretsCount - 1; ++i) {
    if (maximum - POSSIBLE_LOWEST_FRETS[i] <= MINIMUM_FRET_COUNT || POSSIBLE_LOWEST_FRETS[i + 1] > minimum) {
      *LowestFret = POSSIBLE_LOWEST_FRETS[i];
      *FretCount = imax(maximum - *LowestFret, MINIMUM_FRET_COUNT);
      return;
    }
  }

  *LowestFret = POSSIBLE_LOWEST_FRETS[PossibleLowestFretsCount - 1];
  *FretCount = imax(maximum - *LowestFret, MINIMUM_FRET_COUNT);
}

char const* StringIndexToName(int StringIndex) {
  switch (StringIndex) {
    case 0:
      return "low E";
    case 1:
      return "A";
    case 2:
      return "D";
    case 3:
      return "G";
    case 4:
      return "B";
    case 5:
      return "high E";
    default:
      return "unknown";
  }
}

// https://data.europa.eu/apps/data-visualisation-guide/accessible-svg-and-aria
// Note that the title and description IDs must match those in
// GenerateChordDiagram(). TODO
void PrintAriaTitleAndDescription(FILE* OutputFile, int const Chord[STRING_COUNT], const char* ChordID, const char* ChordName) {
  fprintf(OutputFile, "<title id=\"title-%s\">%s</title>", ChordID, ChordName);
  fprintf(OutputFile, "<desc id=\"desc-%s\">%s chord, containing the following notes: ", ChordID, ChordName);

  for (int i = 0; i < STRING_COUNT; ++i) {
    fprintf(OutputFile, "%s string, ", StringIndexToName(i));
    if (Chord[i] == -1) {
      fprintf(OutputFile, "muted; ");
    } else if (Chord[i] == 0) {
      fprintf(OutputFile, "open; ");
    } else {
      fprintf(OutputFile, "fret %d; ", Chord[i]);
    }
  }

  fprintf(OutputFile, "</desc>");
}

void PrintHeader(FILE* OutputFile, int const Chord[STRING_COUNT], char const* ChordName) {
  float TitleX = MARGIN + WIDTH / 2.0f;
  float TitleY = MARGIN;
  fprintf(OutputFile,
          "<text x=\"%f\" y=\"%f\" font-family=\"sans-serif\" "
          "dominant-baseline=\"middle\" text-anchor=\"middle\">%s</text>",
          TitleX, TitleY, ChordName);

  for (int i = 0; i < STRING_COUNT; ++i) {
    if (Chord[i] == -1) {
      float x = MARGIN + i * WIDTH_PER_STRING;
      float y = MARGIN + HEADER_HEIGHT - MUTE_X_FONT_SIZE / 2.0f;
      fprintf(OutputFile,
              "<text x=\"%f\" y=\"%f\" font-family=\"sans-serif\" "
              "dominant-baseline=\"middle\" font-size=\"%f\" "
              "text-anchor=\"middle\">x</text>",
              x, y, MUTE_X_FONT_SIZE);
    }
  }
}

void PrintFretNumber(FILE* OutputFile, int LowestFret) {
  if (LowestFret > 0) {
    float x = MARGIN - FRET_NUMBER_FONT_SIZE / 4.0;
    float y = MARGIN + HEADER_HEIGHT + HEIGHT_PER_FRET * 0.5f;
    fprintf(OutputFile,
            "<text x=\"%f\" y=\"%f\" font-family=\"sans-serif\" font-size=\"%f\" "
            "dominant-baseline=\"middle\" text-anchor=\"end\">%d</text>",
            x, y, FRET_NUMBER_FONT_SIZE, LowestFret + 1);
  }
}

void PrintFretGrid(FILE* OutputFile, int const Chord[STRING_COUNT], int FretCount, int LowestFret) {
  float StrokeWidth = 2;

  fprintf(OutputFile, "<g class=\"frets\">");
  for (int i = 0; i < FretCount + 1; ++i) {
    float x1 = MARGIN;
    float x2 = MARGIN + WIDTH;
    float y1 = MARGIN + HEADER_HEIGHT + i * HEIGHT_PER_FRET;
    float y2 = y1;
    fprintf(OutputFile,
            "<line x1=\"%f\" x2=\"%f\" y1=\"%f\" y2=\"%f\" stroke=\"black\" "
            "stroke-width=\"%f\" />",
            x1, x2, y1, y2, StrokeWidth);
  }

  int TotalHeight = FretCount * HEIGHT_PER_FRET;

  for (int i = 0; i < STRING_COUNT; ++i) {
    float x1 = MARGIN + i * WIDTH_PER_STRING;
    float x2 = x1;
    float y1 = MARGIN + HEADER_HEIGHT - StrokeWidth / 2;
    float y2 = MARGIN + HEADER_HEIGHT + TotalHeight + StrokeWidth / 2;
    fprintf(OutputFile,
            "<line x1=\"%f\" x2=\"%f\" y1=\"%f\" y2=\"%f\" stroke=\"black\" "
            "stroke-width=\"%f\" />",
            x1, x2, y1, y2, StrokeWidth);
  }
  fprintf(OutputFile, "</g>");
}

void PrintChordFingers(FILE* OutputFile, int const Chord[STRING_COUNT], int LowestFret) {
  fprintf(OutputFile, "<g class=\"fingers\">");

  for (int i = 0; i < STRING_COUNT; ++i) {
    if (Chord[i] > 0) {
      float cx = MARGIN + i * WIDTH_PER_STRING;
      float cy = MARGIN + HEADER_HEIGHT + (Chord[i] - LowestFret - 0.5f) * HEIGHT_PER_FRET;
      fprintf(OutputFile, "<circle cx=\"%f\" cy=\"%f\" r=\"%f\" />", cx, cy, FINGER_RADIUS);
    }
  }

  fprintf(OutputFile, "</g>");
}
