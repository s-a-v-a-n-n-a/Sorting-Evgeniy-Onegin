/**
    \file
    Sortings of Shakspeare

    This program reads the text and sorts it alphabetically
    from the beginnings and the endings of its lines

    Prints alphabet sorting in the file "Sorted.txt"
    Prints rhyme sorting in the file "SortedinRhyme.txt"
    Prints the primary text in the file "Text.txt"

    Thank you for using this program!

    \authors Anna Savchuk

    \data    Last update was 09.21.20 at 13:34

    \warning Put the '\\n' at the end of the last text line in the file
    \warning (Press "Enter")
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

//#define TESTING

#define MAXSYMB 1000
#define MAXLINES 10000

struct ptrs_t
{
    char     *ptr ;
    long int Length;
};

typedef struct ptrs_t ptrs;

/*!
Counts the number of lines in buffer

@param[in]  *buf      Buffer of the input

@param[out] nlines    The number of using scanned strings
*/
long int getNumLines(char* buf);

/*!
Sets pointers on the begiinig of the each string

@param[in]  *buf       Buffer of the input
@param[in]  *ptr_mas   The address of the massive of structs with pointers on strings

@param[out] nlines     The number of using scanned strings
*/
long int getLines(char* buf, ptrs* ptr_mas);

/*!
Sets pointers on the begiinig of the each string

@param[in]  *ptr_mas   The address of the massive of structs with pointers on strings
@param[in]  nlines     The number of using lines
*/
void countingSizes(ptrs* ptr_mas, long int nlines);

/*!
Compares one string to another starting with the beginning of the line

@param[in]  line1  Struct with the string 1 of char
@param[in]  line2  Struct with the string 2 of char

@return 1 if the first string is bigger than the second string,
        0 if the first string is equal to the second string,
       -1 if the first string is smaller than the second string
*/
int  compStrAlph(ptrs line1, ptrs line2);

/*!
Compares one string to another starting with the ending of the line

@param[in]  line1 Struct with the string 1 of char
@param[in]  line2 Struct with the string 2 of char

@return 1 if the first string is bigger than the second string,
        0 if the first string is equal to the second string,
       -1 if the first string is smaller than the second string
*/
int compStrRyhm(ptrs line1, ptrs line2);

/*!
Swaps two strings

@param[in]  *Str       The address of the massive of structs with pointers on strings
@param[in]  i          The index of the first string
@param[in]  j          The index of the second string
*/
void swapLines(ptrs* Str, long int i, long int j);

/*!
Sorts the strings by quick sort from the book of Kernighan and Ritchie

@param[in]  *line      The address of the massive of structs with pointers on strings
@param[in]  low        The first index of the sorting part
@param[in]  up         The last index of the sorting part
*/
void sortLines(ptrs* line, long int low, long int up, int (* cmp)(ptrs line1, ptrs line2));

/*!
Prints sorted strings in the file in alphabet order

@param[in]  *put_ptr The address of the massive of sorted structs with pointers of strings
@param[in]  nlines   Number of lines
*/
void putLineAlph(ptrs* put_ptr, const long int nlines);

/*!
Prints sorted strings in the  in rhyme order

@param[in]  *put_ptr The address of the massive of sorted structs with pointers of strings
@param[inn  nlines   Number of lines
*/
void putLineRyhm(ptrs* put_ptr, const long int nlines);

/*!
Prints the massive of the strings in the file

@param[in]  *line   Massive of strings
@param[in]  SiZe    The length of the massive of strings
*/
void putThePrimaryLine(char* line, long int SiZe);

///Unit-tests for comparing from the beginnig
void unit_tests_for_compstrAlph();

///Unit-tests for comparing from the ending
void unit_tests_for_compStrRyhm();

///Unit-tests for sorting
void unit_tests_for_sortlines();

int main(int argc, const char* argv[])
{
/// If define is uncommented we want to test some functions
/*!
    We want to test comparing functions and the function of sorting
*/
    #ifdef TESTING
        unit_tests_for_compStrInv();
        unit_tests_for_compstr();
        unit_tests_for_sortlines();

        return 0;
    #else
        FILE* poem;

        if (argc > 1)
        {
            poem = fopen(argv[1], "rb");
            if (poem == NULL)
                return 0;
        }
        else
        {
            printf("Please, give a file for program!\n");
            return 0;
        }

        long int Len = 0;
        if (!fseek(poem, 0, SEEK_END))
            Len = ftell(poem);
        Len ++;
        fseek(poem, 0, SEEK_SET);

        char* buffer = (char*) calloc(Len, sizeof(char));

        long int obj = fread(buffer, sizeof(char), Len, poem);

        assert(obj == Len - 1);

        buffer[Len - 1] = '\0';

        long int nlines = 0;

        nlines = getNumLines(buffer);

        ptrs* ptr_buf = (ptrs*) calloc(nlines + 1, sizeof(ptrs));

        assert (getLines(buffer, ptr_buf) == nlines);

        countingSizes(ptr_buf, nlines);

        sortLines(ptr_buf, 0, nlines - 1, compStrAlph);
        putLineAlph(ptr_buf, nlines - 1);

        sortLines(ptr_buf, 0, nlines - 1, compStrRyhm);
        putLineRyhm(ptr_buf, nlines - 1);

        putThePrimaryLine(buffer, Len);

        fclose(poem);

        free(buffer);

        return 0;

    #endif
}

long int getNumLines(char* buf)
{
    long int nlines = 0;
    char c = EOF;

    while (nlines < MAXLINES && c != '\0')
    {
        c = * buf;
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

long int getLines(char* buf, ptrs* ptr_mas)
{
    long int nlines = 0;
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
        }
    }

    return nlines;
}

void countingSizes(ptrs* ptr_mas, long int nlines)
{
    char *pt = NULL;
    for (long int i = 0; i <= nlines; i++)
    {
        pt = (ptr_mas[i]).ptr;
        while(*(pt++) != '\n' && ptr_mas[i].Length < MAXSYMB)
            (ptr_mas[i]).Length++;
    }
}

int compStrAlph(ptrs line1, ptrs line2)
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
    else
        return 0;
}

int compStrRyhm(ptrs line1, ptrs line2)
{
    long int len1 = line1.Length;
    long int len2 = line2.Length;

    line1.ptr += line1.Length;
    line2.ptr += line2.Length;

    while (!isalpha(*line1.ptr) && len1 != 0)
    {
        len1--;
        line1.ptr--;
    }
    while (!isalpha(*line2.ptr) && len2 != 0)
    {
        len2--;
        line2.ptr--;
    }

    while(*line1.ptr == *line2.ptr && len1 != 0 && len2 != 0)
    {
        len1--;
        line1.ptr--;
        len2--;
        line2.ptr--;

        while (!isalpha(*line1.ptr) && len1 != 0)
        {
            len1--;
            line1.ptr--;
        }
        while (!isalpha(*line2.ptr) && len2 != 0)
        {
            len2--;
            line2.ptr--;
        }
    }

    if (len1 == 0 || len2 == 0)
    {
        line1.ptr++;
        line2.ptr++;
    }
    if      (*line1.ptr > *line2.ptr || (*line1.ptr == *line2.ptr && len1 > len2))
        return  1;
    else if (*line1.ptr < *line2.ptr || (*line1.ptr == *line2.ptr && len1 < len2))
        return -1;
    else
        return 0;
}

void swapLines(ptrs* Str, long int i, long int j)
{
    char *temp = Str[i].ptr;
    Str[i].ptr = Str[j].ptr;
    Str[j].ptr = temp;

    long int tempLen = Str[i].Length;
    Str[i].Length = Str[j].Length;
    Str[j].Length = tempLen;
}

void sortLines(ptrs* line, long int low, long int up, int (* cmp)(ptrs line1, ptrs line2))
{
    if (low >= up)
        return;

    long int last = low;

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

void putLineAlph(ptrs* put_ptr, const long int nlines)
{
    FILE* dictionary = fopen("Sorted.txt", "wb");

    for (long int i = 0; i <= nlines; i++)
    {
        char* pt = put_ptr[i].ptr;

        while(*pt != '\n')
        {
            fputc(*pt, dictionary);
            pt++;
        }

        fputc(*pt, dictionary);
    }

    fclose(dictionary);
}

void putLineRyhm(ptrs* put_ptr, const long int nlines)
{
    FILE* other = fopen("SortedinRhyme.txt", "wb");

    for (long int i = 0; i <= nlines; i++)
    {
        char* pt = put_ptr[i].ptr;

        while(*pt != '\n')
        {
            fputc(*pt, other);
            pt++;
        }
        fputc(*pt, other);
    }

    fclose(other);
}

void putThePrimaryLine(char* line, long int SiZe)
{
    FILE* directory = fopen("Text.txt", "wb");

    fwrite(line, sizeof(char), SiZe, directory);

    fclose(directory);
}

void unit_tests_for_compstrAlph()
{
    {
        char str1[] = "";
        char str2[] = "";

        ptrs string1;
        string1.ptr = str1;
        string1.Length = 1;

        ptrs string2;
        string1.ptr = str2;
        string1.Length = 1;

        assert(compStrAlph(string1, string2) == 0);
    }

    {
        char str1[] = "        ";
        char str2[] = "";

        ptrs string1;
        string1.ptr = str1;
        string1.Length = 9;

        ptrs string2;
        string1.ptr = str2;
        string1.Length = 1;

        assert(compStrAlph(string1, string2) == 0);
    }

    {
        char str1[] = " ";
        char str2[] = "  ";

        ptrs string1;
        string1.ptr = str1;
        string1.Length = 2;

        ptrs string2;
        string1.ptr = str2;
        string1.Length = 3;

        assert(compStrAlph(string1, string2) == 0);
    }

    {
        char str1[] = ",";
        char str2[] = " ";

        ptrs string1;
        string1.ptr = str1;
        string1.Length = 2;

        ptrs string2;
        string1.ptr = str2;
        string1.Length = 2;

        assert(compStrAlph(string1, string2) == 0);
    }

    {
        char str1[] = "Meow\n";
        char str2[] = "M     eow\n";

        ptrs string1;
        string1.ptr = str1;
        string1.Length = 6;

        ptrs string2;
        string1.ptr = str2;
        string1.Length = 11;

        assert(compStrAlph(string1, string2) == 0);
    }

    {
        char str1[] = "Hero\n";
        char str2[] = "Herb\n";

        ptrs string1;
        string1.ptr = str1;
        string1.Length = 6;

        ptrs string2;
        string1.ptr = str2;
        string1.Length = 6;

        assert(compStrAlph(string1, string2) > 0);
    }

    {
        char str1[] = "Hello, world\n";
        char str2[] = "Hello, my world\n";

        ptrs string1;
        string1.ptr = str1;
        string1.Length = 14;

        ptrs string2;
        string1.ptr = str2;
        string1.Length = 17;

        assert(compStrAlph(string1, string2) > 0);
    }

    {
        char str1[] = "My name is Anna\n";
        char str2[] = "My name is not Anna\n";

        ptrs string1;
        string1.ptr = str1;
        string1.Length = 17;

        ptrs string2;
        string1.ptr = str2;
        string1.Length = 21;

        assert(compStrAlph(string1, string2) < 0);
    }
}

void unit_tests_for_compStrRyhm()
{
    {
        char str1[] = "";
        char str2[] = "";

        ptrs string1;
        string1.ptr = str1;
        string1.Length = 1;

        ptrs string2;
        string1.ptr = str2;
        string1.Length = 1;

        assert(compStrRyhm(string1, string2) == 0);
    }

    {
        char str1[] = "        ";
        char str2[] = "";

        ptrs string1;
        string1.ptr = str1;
        string1.Length = 9;

        ptrs string2;
        string1.ptr = str2;
        string1.Length = 1;

        assert(compStrRyhm(string1, string2) == 0);
    }

    {
        char str1[] = " ";
        char str2[] = "  ";

        ptrs string1;
        string1.ptr = str1;
        string1.Length = 2;

        ptrs string2;
        string1.ptr = str2;
        string1.Length = 3;

        assert(compStrRyhm(string1, string2) == 0);
    }

    {
        char str1[] = ",";
        char str2[] = " ";

        ptrs string1;
        string1.ptr = str1;
        string1.Length = 2;

        ptrs string2;
        string1.ptr = str2;
        string1.Length = 2;

        assert(compStrRyhm(string1, string2) == 0);
    }

    {
        char str1[] = "Meow\n";
        char str2[] = "M     eow\n";

        ptrs string1;
        string1.ptr = str1;
        string1.Length = 6;

        ptrs string2;
        string1.ptr = str2;
        string1.Length = 11;

        assert(compStrRyhm(string1, string2) == 0);
    }

    {
        char str1[] = "Hero\n";
        char str2[] = "Herb\n";

        ptrs string1;
        string1.ptr = str1;
        string1.Length = 6;

        ptrs string2;
        string1.ptr = str2;
        string1.Length = 6;

        assert(compStrRyhm(string1, string2) > 0);
    }

    {
        char str1[] = "Hello, world\n";
        char str2[] = "Hello, my world\n";

        ptrs string1;
        string1.ptr = str1;
        string1.Length = 14;

        ptrs string2;
        string1.ptr = str2;
        string1.Length = 17;

        assert(compStrRyhm(string1, string2) < 0);
    }

    {
        char str1[] = "My name is Anna\n";
        char str2[] = "My name is not Anna\n";

        ptrs string1;
        string1.ptr = str1;
        string1.Length = 17;

        ptrs string2;
        string1.ptr = str2;
        string1.Length = 21;

        assert(compStrRyhm(string1, string2) < 0);
    }
}

void unit_tests_for_sortlines()
{
    {
        char str1[] = "ccc";
        char str2[] = "aaa";
        char str3[] = "bbb";

        ptrs string1;
        string1.ptr = str1;
        string1.Length = 4;

        ptrs string2;
        string1.ptr = str2;
        string1.Length = 4;

        ptrs string3;
        string1.ptr = str3;
        string1.Length = 4;

        ptrs ptr[3] = {string1, string2, string3};

        sortLines(ptr, 0, 2, compStrAlph);
        assert(ptr[0].ptr == string2.ptr);
    }

    {
        char str1[] = "ccccc";
        char str2[] = "ccc";
        char str3[] = "ccc";

        ptrs string1;
        string1.ptr = str1;
        string1.Length = 6;

        ptrs string2;
        string1.ptr = str2;
        string1.Length = 4;

        ptrs string3;
        string1.ptr = str3;
        string1.Length = 4;

        ptrs ptr[3] = {string1, string2, string3};

        sortLines(ptr, 0, 2, compStrAlph);
        assert((ptr[0].ptr == string3.ptr || ptr[0].ptr == string2.ptr) && ptr[2].ptr == string1.ptr);
    }

    {
        char str1[] = "heey";
        char str2[] = "oh my god";
        char str3[] = "oh, my god";
        char str4[] = "oh, my gosh";

        ptrs string1;
        string1.ptr = str1;
        string1.Length = 5;

        ptrs string2;
        string1.ptr = str2;
        string1.Length = 10;

        ptrs string3;
        string1.ptr = str3;
        string1.Length = 11;

        ptrs string4;
        string1.ptr = str4;
        string1.Length = 12;

        ptrs ptr[4] = {string1, string2, string3, string4};

        sortLines(ptr, 0, 3, compStrAlph);
        assert(ptr[0].ptr == string1.ptr && ptr[3].ptr == string4.ptr);
    }
}
