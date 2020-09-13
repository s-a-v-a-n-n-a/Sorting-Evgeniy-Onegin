#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <assert.h>
#include <ctype.h>

typedef long int LONG;

#define MAXSYMB 100
#define MAXLINES 1000

//--------------------------------------------------------------------
// Compares one string to another
//
// Parameters:
//
// *line1[]        address of the string 1 of char
// *line2[]        address of the string 2 of char
//
// Returns:
//
//  1               if string 1 > string 2
//  0               if string 1 = string 2
// -1               if string 1 < string 2
//--------------------------------------------------------------------
int  compStr(char *line1[], char *line2[]);

//--------------------------------------------------------------------
// Prints unchanged massive of strings in the file
//
// Parameters:
//
// *line            massive of strings
// SiZe             length of the massive of strings
//--------------------------------------------------------------------
void putTheHoleLine(char *line, LONG SiZe);

//--------------------------------------------------------------------
// Prints sorted strings in the file
//
// Parameters:
//
// *ptr[]         address of the massive of sorted pointers on strings
// nlines         number of lines
//--------------------------------------------------------------------
void putLine(char *ptr[], const LONG nlines);

//--------------------------------------------------------------------
// Sets pointers on the begiinig of the each string
//
// Parameters:
//
// *buf           buffer of the input
// *ptr_mas[]     address of the massive of pointers on strings
//
// Returns:
//
// nlines         the number of scanned strings
//--------------------------------------------------------------------
LONG getLines(char *buf, char *ptr_mas[]);

//--------------------------------------------------------------------
// Sorts the strings
//
// Parameters:
//
// *line[]        address of the massive of pointers on strings
// low            the first index of the sorting part
// up             the last index of the sorting part
//--------------------------------------------------------------------
void sortLines(char *line[], LONG low, LONG up);

//--------------------------------------------------------------------
// Swaps two strings
//
// Parameters:
//
// *Str[]        address of the massive of pointers on strings
// i             the index of the first string
// j             the index of the second string
//--------------------------------------------------------------------
void swapLines(char *Str[], LONG i, LONG j);

void unit_tests_for_compstr();

void unit_tests_for_sortlines();

int main(int argc, const char *argv[])
{
    setlocale(LC_ALL, "Russian");
    FILE *poem;

    if (argc > 0)
    {
        poem = fopen(argv[1], "rb");
        if (poem == NULL)
            return 0;
    }

    LONG Len = 0;
    if (!fseek(poem, 0, SEEK_END))
         Len = ftell(poem);
    Len++;
    fseek(poem, 0, SEEK_SET);

    char *ptr_buf[MAXLINES] = {};
    char *buffer = (char *) calloc(Len, sizeof(char));

    LONG obj = fread(buffer, sizeof(char), Len, poem);

    assert(obj == Len - 1);

    buffer[Len - 1] = '\0';

    LONG nlines = 0;

    nlines = getLines(buffer, ptr_buf);

    printf("%d\n", nlines);

    unit_tests_for_compstr();
    unit_tests_for_sortlines();

    sortLines(ptr_buf, 0, nlines);

    putTheHoleLine(buffer, Len);

    printf("Write smth\n");

    putLine(ptr_buf, nlines);

    fclose(poem);

    free(buffer);

    return 0;
}

int compStr(char *line1, char *line2)
{
    while(*line1 == *line2 && *line1 != '\n' && *line2 != '\n')
    {
        line1++;
        line2++;

        while (!isalnum(*line1) && *line1 != '\n')
            line1++;
        while (!isalnum(*line2) && *line2 != '\n')
            line2++;
    }

    if      (isalnum(*line1) && isalnum(*line2) && *line1 > *line2)
        return  1;
    else if (isalnum(*line1) && isalnum(*line2) && *line1 < *line2)
        return -1;
    else return 0;
}

void unit_tests_for_compstr()
{
    assert(compStr("", "") == 0);

    assert(compStr("        ", "") == 0);

    assert(compStr(" ", "  ") == 0);

    assert(compStr(",", " ") == 0);

    assert(compStr("Meow\n", "M   eow\n") == 0);

    assert(compStr("Oh, My\n", "O h M y\n") == 0);

    assert(compStr("Hero\n", "Herb\n") > 0);

    assert(compStr("Hello, world\n", "Hello, my world\n") > 0);

    assert(compStr("My name is Anna\n", "My name is not Anna\n") < 0);
}

void sortLines(char *line[], LONG low, LONG up)
{
    if (low >= up)
        return;

    LONG last = low;

    swapLines(line, low, (low + up)/2);

    for (int i = low + 1; i <= up; i++)
    {
        if (compStr(line[low], line[i]) > 0)
        {
            last++;
            swapLines(line, last, i);
        }
    }
    swapLines(line, low, last);

    sortLines(line, low, last - 1);
    sortLines(line, last + 1, up);
}

void putTheHoleLine(char *line, LONG SiZe)
{
    FILE *directory = fopen("Text.txt", "wb");

    fwrite(line, sizeof(char), SiZe, directory);

    fclose(directory);
}

void putLine(char *(ptr[]), const LONG nlines)
{
    FILE *dictionary = fopen("Sorted.txt", "wb");

    LONG SiZe = 0;
    for (LONG i = 0; i < nlines; i++)
    {
        char *pt = ptr[i];
        while(*pt != '\n')
        {
            fputc(*pt, dictionary);
            printf("%c ", *pt);
            pt++;
        }
        fputc(*pt++, dictionary);
    }

    fclose(dictionary);
}
void unit_tests_for_sortlines()
{
    {
        char strinG1[4] = "ccc";
        char strinG2[4] = "aaa";
        char strinG3[4] = "bbb";
        char *ptr[3] = {strinG1, strinG2, strinG3};
        sortLines(ptr, 0, 2);
        assert(ptr[0] == strinG2);
    }

    {
        char strinG1[6] = "ccccc";
        char strinG2[4] = "ccc";
        char strinG3[4] = "ccc";
        char *ptr[3] = {strinG1, strinG2, strinG3};
        sortLines(ptr, 0, 2);
        //printf("%c %c %c\n", *ptr[0], *ptr[1], *ptr[2]);
        assert((ptr[0] == strinG3 || ptr[0] == strinG2) && ptr[2] == strinG1);
    }

    {
        char strinG1[5] = "heey";
        char strinG2[10] = "oh my god";
        char strinG3[11] = "oh, my god";
        char strinG4[12] = "oh, my gosh";
        char *ptr[4] = {strinG1, strinG2, strinG3, strinG4};
        sortLines(ptr, 0, 3);
        assert(ptr[0] == strinG1 && ptr[3] == strinG4);
    }
}

LONG getLines(char *buf, char *ptr_mas[])
{
    LONG nlines = 0;
    char c = EOF;

    *ptr_mas = buf;

    while (nlines < MAXLINES && c != '\0')
    {
        c = *buf;
        if (c == '\n')
        {
            nlines++;
            ptr_mas[nlines] = ++buf;
        }
        else
            buf++;
    }

    return nlines;
}

void swapLines(char *Str[], LONG i, LONG j)
{
    char *temp;

    temp = Str[i];
    Str[i] = Str[j];
    Str[j] = temp;
}
