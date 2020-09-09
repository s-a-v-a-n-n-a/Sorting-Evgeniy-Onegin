#include <stdio.h>
#include <locale.h>
#include <assert.h>

typedef long int LONG;

#define MAXSYMB 100
#define MAXLINES 1000

//-------------------------------------------------------------
// Copies one string to another
//
// Parameters:
//
// *line1[]        address of the line 1 of char
// *line2[]        address of the line 2 of char
//-------------------------------------------------------------
void strCopy(char *line1[], char *line2[]);

//-------------------------------------------------------------
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
//-------------------------------------------------------------
int  compStr(char *line1[], char *line2[]);

//-------------------------------------------------------------
// Prints the string in the file
//
// Parameters:
//
// *line[]        address of the massive of pointers on strings
//-------------------------------------------------------------
void putLine(char *line[]);

char getLine(char *line[], LONG num_line, int max_len, FILE *file);

void sortLines(char *line[], LONG low, LONG up);

void swapLines(char *line1[], LONG i, LONG j);


//void unit_tests_for_strcopy();

//void unit_tests_for_compstr();


int main(int argc, const char *argv[])
{
    setlocale(LC_ALL, "Rus");
    FILE *poem;

    if (argc > 0)
    {
        poem = fopen(argv[1], "r");
        if (poem == NULL)
            return 0;
    }


    char *line[MAXLINES];//массив указателей на char

    LONG nlines = 0;

    for (LONG kol = 0; ; (*line)++, kol++)//увеличивается указатель на нулевой элемент массива
        if (getLine(line, kol, MAXSYMB, poem) == EOF)//в функцию передаётся указатель на элемент kol
            break;

    sortLines(line, 0, nlines);

    putLine(line);

    fclose(poem);

    return 0;
}

void strCopy(char line1[], char line2[])//получает указатели на массивы из char
{
    while(*line1++ = *line2++)
        ;
}

int compStr(char line1[], char line2[])
{
    while(*line1++ == *line2++)
        ;
    if (*line1 == *line2 == NULL)
        return 0;
    else if (*line1 > *line2)
        return 1;
    else
        return -1;
}

void sortLines(char *line[], LONG low, LONG up)//функция получает указатель на массив указателей на char
{
    if (low >= up)
        return;

    LONG mid = (low + up)/2;
    LONG last = up;

    swapLines(line, low, mid);//функции передаётся указатель на массив указателей char
    mid = low;

    for (int i = low + 1; i <= up; i++)
    {
        if (compStr(line[mid], line[i]) > 0)
        {
            swapLines(line, mid, i);
            mid = i;
        }
    }

    sortLines(line, low, mid - 1);
    sortLines(line, mid + 1, up);
}

void putLine(char *line[])
{
    FILE *dictionary = fopen("Sorted.txt", "w");
    fputs(*line, dictionary);
    fclose(dictionary);
}

char getLine(char *line[], LONG num_line, int max_len, FILE *file)//функция получает указатель на массив
{
    LONG nsymb = 0;
    char c = EOF;
    while (nsymb++ < max_len && c != '\0')
    {
        c = (char)getc(file);
        printf("%c\n", c);
        *line[nsymb] = c;//по адресу указателя на элемент nsymb записывается c
        if (c == EOF)
            return c;
    }
    return c;
}

void swapLines(char *Str[], LONG i, LONG j)//функция получает указатели на массив указателей на char
{
    char *temp;//указатель на char
    strCopy(temp, Str[i]);
    strCopy(Str[i], Str[j]);//передаются указатели на char
    strCopy(Str[j], temp);
}
