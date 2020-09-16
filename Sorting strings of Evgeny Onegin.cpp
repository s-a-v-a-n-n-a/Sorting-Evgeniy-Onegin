#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <assert.h>
#include <ctype.h>

typedef long int LONG;

#define MAXSYMB 100
#define MAXLINES 1000

struct ptrs
{
    char *ptr = NULL;
    LONG Size = 0;
};

//-----------------------------------------------------------------------------------------------
// Compares one string to another starting with the ending of the line
//
// Parameters:
//
// line1            struct with the string 1 of char
// line2            struct with the string 2 of char
//
// Returns:
//
//  1               if string 1 > string 2
//  0               if string 1 = string 2
// -1               if string 1 < string 2
//-----------------------------------------------------------------------------------------------
int compStrInv(ptrs line1, ptrs line2);

//-----------------------------------------------------------------------------------------------
// Compares one string to another starting with the beginning of the line
//
// Parameters:
//
// line1            struct with the string 1 of char
// line2            struct with the string 2 of char
//
// Returns:
//
//  1               if string 1 > string 2
//  0               if string 1 = string 2
// -1               if string 1 < string 2
//-----------------------------------------------------------------------------------------------
int  compStr(ptrs line1, ptrs line2);

//-----------------------------------------------------------------------------------------------
// Prints unchanged massive of strings in the file
//
// Parameters:
//
// *line            massive of strings
// SiZe             length of the massive of strings
//-----------------------------------------------------------------------------------------------
void putTheWholeLine(char *line, LONG SiZe);

//--------------------------------------------------------------------
// Prints sorted strings in the file in alphabet order
//
// Parameters:
//
// *put_ptr       address of the massive of sorted structs with pointers of strings
// nlines         number of lines
//-----------------------------------------------------------------------------------------------
void putLineAlf(ptrs *put_ptr, const LONG nlines);

//-----------------------------------------------------------------------------------------------
// Prints sorted strings in the  in rhyme order
//
// Parameters:
//
// *put_ptr       address of the massive of sorted structs with pointers of strings
// nlines         number of lines
//-----------------------------------------------------------------------------------------------
void putLineRhm(ptrs *put_ptr, const LONG nlines);

//-----------------------------------------------------------------------------------------------
// Counts the number of lines in buffer
//
// Parameters:
//
// *buf           buffer of the input
//
// Returns:
//
// nlines         the number of scanned strings
//-----------------------------------------------------------------------------------------------
LONG length(char *buf);

//-----------------------------------------------------------------------------------------------
// Sets pointers on the begiinig of the each string
//
// Parameters:
//
// *buf           buffer of the input
// *ptr_mas       address of the massive of structs with pointers on strings
//
// Returns:
//
// nlines         the number of scanned strings
//-----------------------------------------------------------------------------------------------
LONG getLines(char *buf, ptrs *ptr_mas);

//-----------------------------------------------------------------------------------------------
// Sorts the strings
//
// Parameters:
//
// *line          address of the massive of structs with pointers on strings
// low            the first index of the sorting part
// up             the last index of the sorting part
//-----------------------------------------------------------------------------------------------
void sortLines(ptrs *line, LONG low, LONG up, int (*cmp)(ptrs line1, ptrs line2));

//-----------------------------------------------------------------------------------------------
// Swaps two strings
//
// Parameters:
//
// *Str          address of the massive of structs with pointers on strings
// i             the index of the first string
// j             the index of the second string
//-----------------------------------------------------------------------------------------------
void swapLines(ptrs *Str, LONG i, LONG j);

void unit_tests_for_compstr();

void unit_tests_for_sortlines();

int main(int argc, const char *argv[])
{
    //setlocale(LC_ALL, "Russian");
    FILE *poem;

    if (argc > 1)
    {
        poem = fopen(argv[1], "rb");
        if (poem == NULL)
            return 0;
    }
    else
    {
        printf("error");
        return EXIT_FAILURE;
    }

    LONG Len = 0;
    if (!fseek(poem, 0, SEEK_END))
         Len = ftell(poem);
    Len++;
    fseek(poem, 0, SEEK_SET);

    char *buffer = (char *) calloc(Len, sizeof(char));

    LONG obj = fread(buffer, sizeof(char), Len, poem);

    assert(obj == Len - 1);

    buffer[Len - 1] = '\0';

    LONG nlines = 0;

    nlines = length(buffer);

    ptrs *ptr_buf = (ptrs *) calloc(nlines + 1, sizeof(ptrs));

    assert (getLines(buffer, ptr_buf) == nlines);

    printf("%ld\n", nlines);

    //unit_tests_for_compstr();
    //unit_tests_for_sortlines();

    sortLines(ptr_buf, 0, nlines, compStr);

    for (long int i = 0; i <= nlines; i++)
        printf("%c\n", *ptr_buf[i].ptr);
    system("pause");

    putTheWholeLine(buffer, Len);

    printf("Write smth\n");

    putLineAlf(ptr_buf, nlines);

    sortLines(ptr_buf, 0, nlines, compStrInv);

    putLineRhm(ptr_buf, nlines);

    fclose(poem);

    free(buffer);

    return 0;
}

int compStrInv(ptrs line1, ptrs line2)
{
    LONG len1 = line1.Size;
    LONG len2 = line2.Size;

    line1.ptr += line1.Size;
    line2.ptr += line2.Size;

    while(*line1.ptr == *line2.ptr && len1 != 0 && len2 != 0)
    {
        len1--;
        len2--;

        while (!isalnum(*line1.ptr) && len1 != 0)
            len1--;
        while (!isalnum(*line2.ptr) && len2 != 0)
            len2--;
    }

    if (len1 == 0 || len2 == 0)
    {
        line1.ptr++;
        line2.ptr++;
    }
    if (*line1.ptr > *line2.ptr || (*line1.ptr == *line2.ptr && len1 > len2))
        return  1;
    else if (*line1.ptr < *line2.ptr || (*line1.ptr == *line2.ptr && len1 < len2))
        return -1;
    else
        return 0;
}

int compStr(ptrs line1, ptrs line2)
{
    while(*line1.ptr == *line2.ptr && *line1.ptr != '\n' && *line2.ptr != '\n')
    {
        line1.ptr++;
        line2.ptr++;

        while (!isalnum(*line1.ptr) && *line1.ptr != '\n')
            line1.ptr++;
        while (!isalnum(*line2.ptr) && *line2.ptr != '\n')
            line2.ptr++;
    }

    if      (isalnum(*line1.ptr) && isalnum(*line2.ptr) && *line1.ptr > *line2.ptr)
        return  1;
    else if (isalnum(*line1.ptr) && isalnum(*line2.ptr) && *line1.ptr < *line2.ptr)
        return -1;
    else return 0;
}
/*
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
*/
void sortLines(ptrs *line, LONG low, LONG up, int (*cmp)(ptrs line1, ptrs line2))
{
    if (low >= up)
        return;

    LONG last = low;

    swapLines(line, low, (low + up)/2);

    for (int i = low + 1; i <= up; i++)
    {
        if (cmp(line[low], line[i]) > 0)
        {
            last++;
            swapLines(line, last, i);
        }
    }
    swapLines(line, low, last);

    sortLines(line, low, last - 1, cmp);
    sortLines(line, last + 1, up, cmp);
}

void putTheWholeLine(char *line, LONG SiZe)
{
    FILE *directory = fopen("Text.txt", "wb");

    fwrite(line, sizeof(char), SiZe, directory);

    fclose(directory);
}

void putLineAlf(ptrs *put_ptr, const LONG nlines)
{
    FILE *dictionary = fopen("Sorted.txt", "wb");

    for (LONG i = 0; i <= nlines; i++)
    {
        char *pt = put_ptr[i].ptr;
        while(*pt != '\n')
        {
            fputc(*pt, dictionary);
            pt++;
        }
        fputc(*pt, dictionary);
    }

    fclose(dictionary);
}

void putLineRhm(ptrs *put_ptr, const LONG nlines)
{
    FILE *other = fopen("SortedinRhyme.txt", "wb");

    for (LONG i = 0; i <= nlines; i++)
    {
        char *pt = put_ptr[i].ptr;
        while(*pt != '\n')
        {
            fputc(*pt, other);
            pt++;
        }
        fputc(*pt, other);
    }

    fclose(other);
}
/*
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
*/
LONG length(char *buf)
{
    LONG nlines = 0;
    char c = EOF;

    while (nlines < MAXLINES && c != '\0')
    {
        c = *buf;
        if (c == '\n')
        {
            nlines++;
            buf++;
        }
        else
        {
            buf++;
        }
    }

    return nlines;
}

LONG getLines(char *buf, ptrs *ptr_mas)
{
    LONG nlines = 0;
    char c = EOF;

    (*ptr_mas).ptr = buf;

    while (nlines < MAXLINES && c != '\0')
    {
        c = *buf;
        if (c == '\n')
        {
            nlines++;
            ptr_mas++;
            (*ptr_mas).ptr = ++buf;
        }
        else
        {
            buf++;
            (*ptr_mas).Size++;
        }
    }

    return nlines;
}

void swapLines(ptrs *Str, LONG i, LONG j)
{
    char *temp;

    temp = Str[i].ptr;
    Str[i].ptr = Str[j].ptr;
    Str[j].ptr = temp;
}
