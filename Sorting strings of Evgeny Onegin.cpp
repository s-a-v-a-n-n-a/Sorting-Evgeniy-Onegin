#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

//#define TESTING

typedef long int LONG;

#define MAXSYMB 1000
#define MAXLINES 10000

//struct with the pointer on the beginnig of the line and its size (length)
struct ptrs
{
    char *ptr = NULL;
    LONG Length = 0;
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
void putTheWholeLine(char* line, LONG SiZe);

//--------------------------------------------------------------------
// Prints sorted strings in the file in alphabet order
//
// Parameters:
//
// *put_ptr       address of the massive of sorted structs with pointers of strings
// nlines         number of lines
//-----------------------------------------------------------------------------------------------
void putLineAlf(ptrs* put_ptr, const LONG nlines);

//-----------------------------------------------------------------------------------------------
// Prints sorted strings in the  in rhyme order
//
// Parameters:
//
// *put_ptr       address of the massive of sorted structs with pointers of strings
// nlines         number of lines
//-----------------------------------------------------------------------------------------------
void putLineRhm(ptrs* put_ptr, const LONG nlines);

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
LONG length(char* buf);

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
LONG getLines(char* buf, ptrs* ptr_mas);

//-----------------------------------------------------------------------------------------------
// Sorts the strings
//
// Parameters:
//
// *line          address of the massive of structs with pointers on strings
// low            the first index of the sorting part
// up             the last index of the sorting part
//-----------------------------------------------------------------------------------------------
void sortLines(ptrs* line, LONG low, LONG up, int (* cmp)(ptrs line1, ptrs line2));

//-----------------------------------------------------------------------------------------------
// Swaps two strings
//
// Parameters:
//
// *Str          address of the massive of structs with pointers on strings
// i             the index of the first string
// j             the index of the second string
//-----------------------------------------------------------------------------------------------
void swapLines(ptrs* Str, LONG i, LONG j);

void unit_tests_for_compStrInv();

void unit_tests_for_compstr();

void unit_tests_for_sortlines();

int main(int argc, const char* argv[])
{
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
            printf("error");
            return EXIT_FAILURE;
        }

        LONG Len = 0;
        if (!fseek(poem, 0, SEEK_END))
            Len = ftell(poem);
        Len++;
        fseek(poem, 0, SEEK_SET);

        char* buffer = (char*) calloc(Len, sizeof(char));

        LONG obj = fread(buffer, sizeof(char), Len, poem);

        assert(obj == Len - 1);

        buffer[Len - 1] = '\0';

        LONG nlines = 0;

        nlines = length(buffer);

        ptrs* ptr_buf = (ptrs*) calloc(nlines + 1, sizeof(ptrs));

        assert (getLines(buffer, ptr_buf) == nlines);

        sortLines(ptr_buf, 0, nlines - 1, compStr);

        putTheWholeLine(buffer, Len);

        putLineAlf(ptr_buf, nlines - 1);

        sortLines(ptr_buf, 0, nlines - 1, compStrInv);

        putLineRhm(ptr_buf, nlines - 1);

        fclose(poem);

        free(buffer);

        return 0;

    #endif
}

int compStrInv(ptrs line1, ptrs line2)
{
    LONG len1 = line1.Length;
    LONG len2 = line2.Length;
    
    while(*line1.ptr != '\n')
    {
        line1.ptr++;
    }

    while(*line2.ptr != '\n')
    {
        line2.ptr++;
    }
    
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
    else
        return 0;
}

void putTheWholeLine(char* line, LONG SiZe)
{
    FILE* directory = fopen("Text.txt", "wb");

    fwrite(line, sizeof(char), SiZe, directory);

    fclose(directory);
}

void putLineAlf(ptrs* put_ptr, const LONG nlines)
{
    FILE* dictionary = fopen("Sorted.txt", "wb");

    for (LONG i = 0; i <= nlines; i++)
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

void putLineRhm(ptrs* put_ptr, const LONG nlines)
{
    FILE* other = fopen("SortedinRhyme.txt", "wb");

    for (LONG i = 0; i <= nlines; i++)
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

LONG length(char* buf)
{
    LONG nlines = 0;
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

LONG getLines(char* buf, ptrs* ptr_mas)
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
            (*ptr_mas).Length++;
        }
    }

    return nlines;
}

void sortLines(ptrs* line, LONG low, LONG up, int (* cmp)(ptrs line1, ptrs line2))
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

void swapLines(ptrs* Str, LONG i, LONG j)
{
    char* temp;

    temp = Str[i].ptr;
    Str[i].ptr = Str[j].ptr;
    Str[j].ptr = temp;
}

void unit_tests_for_compStrInv()
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

        assert(compStr(string1, string2) == 0);
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

        assert(compStr(string1, string2) == 0);
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

        assert(compStr(string1, string2) == 0);
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

        assert(compStr(string1, string2) == 0);
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

        assert(compStr(string1, string2) == 0);
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

        assert(compStr(string1, string2) > 0);
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

        assert(compStr(string1, string2) < 0);
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

        assert(compStr(string1, string2) < 0);
    }
}

void unit_tests_for_compstr()
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

        assert(compStr(string1, string2) == 0);
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

        assert(compStr(string1, string2) == 0);
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

        assert(compStr(string1, string2) == 0);
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

        assert(compStr(string1, string2) == 0);
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

        assert(compStr(string1, string2) == 0);
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

        assert(compStr(string1, string2) > 0);
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

        assert(compStr(string1, string2) > 0);
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

        assert(compStr(string1, string2) < 0);
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

        sortLines(ptr, 0, 2, compStr);
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

        sortLines(ptr, 0, 2, compStr);
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

        sortLines(ptr, 0, 3, compStr);
        assert(ptr[0].ptr == string1.ptr && ptr[3].ptr == string4.ptr);
    }
}
