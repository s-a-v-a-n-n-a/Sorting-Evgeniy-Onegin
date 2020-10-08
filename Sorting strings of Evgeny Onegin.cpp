/**
    \file
    Sortings of Shakspeare.\n
    This program reads the text and sorts it alphabetically
    from the beginnings and the endings of its lines.\n
    Prints alphabet sorting, rhyme sorting
    and the primary text in the file "Sorted.txt"

    Thank you for using this program!
    \authors Anna Savchuk
    \date    Last update was 10.09.20 at 02:02
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

//#define TESTING

const int MAXSYMB = 1000;

struct Ptrs
{
    char  *ptr ;
    size_t length;
};

typedef struct Ptrs Ptrs_t;

/*!
Counts the number of symbols in the file
@param[in]  *file     Input file

@param[out] len       The number symbols in file
*/
long int get_size_of_file(FILE* file);

/*!
Counts the number of lines in buffer
@param[in]  *file     Input file
@param[in]  length    Pointer on the number symbols in file

@param[out] buffer    A buffer with the whole text
*/
char *get_the_text(FILE* file, size_t* length);

/*!
Counts the number of lines in buffer
@param[in]  *buf      Buffer of the input
@param[in]  length    Length of buffer

@param[out] nlines    The number of using scanned strings
*/
long int get_num_lines(char* buf, size_t length);

/*!
Sets pointers on the begiinig of the each string
@param[in]  *buf       Buffer of the input
@param[in]  *ptr_mas   The address of the massive of structs with pointers on strings
@param[in]  length     Length of buffer

@param[out] n_lines    The number of using scanned strings
*/
long int get_lines(char* buf, Ptrs_t* ptr_mas, size_t length);

/*!
Sets pointers on the begiinig of the each string
@param[in]  *ptr_mas    The address of the massive of structs with pointers on strings
@param[in]  n_lines     The number of using lines
*/
void counting_sizes(Ptrs_t* ptr_mas, size_t n_lines);

/*!
Compares one string to another starting with the beginning of the line
@param[in]  line1  struct with the string 1 of char
@param[in]  line2  struct with the string 2 of char

@return 1 if the first string is bigger than the second string,
        0 if the first string is equal to the second string,
       -1 if the first string is smaller than the second string
*/
int  comp_str_in_alph(Ptrs_t line1, Ptrs_t line2);

/*!
Compares one string to another starting with the ending of the line
@param[in]  line1 struct with the string 1 of char
@param[in]  line2 struct with the string 2 of char

@return 1 if the first string is bigger than the second string,
        0 if the first string is equal to the second string,
       -1 if the first string is smaller than the second string
*/
int comp_str_in_ryhm(Ptrs_t line1, Ptrs_t line2);

/*!
Swaps two strings
@param[in]  *str       The address of the massive of structs with pointers on strings
@param[in]  i          The index of the first string
@param[in]  j          The index of the second string
*/
void swap_lines(Ptrs_t* str, size_t line1, size_t line2);

/*!
Sorts the strings by quick sort from the book of Kernighan and Ritchie
@param[in]  *line      The address of the massive of structs with pointers on strings
@param[in]  low        The first index of the sorting part
@param[in]  up         The last index of the sorting part
*/
void sort_lines(Ptrs_t* line, size_t low, size_t up, int (* cmp)(Ptrs_t line1, Ptrs_t line2));

/*!
Prints sorted strings in the file in alphabet order
@param[in]  *put_ptr  The address of the massive of sorted structs with pointers of strings
@param[in]  n_lines   Number of lines
@param[in]  out       File to write in it
*/
void put_line(Ptrs_t* put_ptr, const size_t n_lines, FILE* out);

/*!
Prints the massive of the strings in the file
@param[in]  *line   Massive of strings
@param[in]  sz      The length of the massive of strings
@param[in]  out     File to write in it
*/
void put_the_primary_line(char* line, size_t sz, FILE* out);

///Unit-tests for comparing from the beginnig
void unit_tests_for_comp_str_in_alph();

///Unit-tests for comparing from the ending
void unit_tests_for_comp_str_in_ryhm();

///Unit-tests for sorting
void unit_tests_for_sort_lines();

int main(int argc, const char* argv[])
{
/// If define is uncommented we want to test some functions
/*!
    We want to test comparing functions and the function of sorting
*/
    #ifdef TESTING
        unitTestsForCompstrAlph();
        unitTestsForCompstrRyhm();
        unitTestsForSortlines();

        return 0;
    #else
        FILE* input = NULL;

        if (argc > 1)
        {
            input = fopen(argv[1], "rb");
            if (input == NULL)
            {
                perror("");
                return EXIT_FAILURE;
            }
        }
        else
        {
            printf("Please, give a file for program!\n");
            return EXIT_FAILURE;
        }

        size_t len   = get_size_of_file(input);
        char *buffer = get_the_text(input, &len);

        long int n_lines = get_num_lines(buffer, len);

        Ptrs_t *ptr_buf = (Ptrs_t*) calloc(n_lines + 1, sizeof(Ptrs_t));

        assert (get_lines(buffer, ptr_buf, len) == n_lines);

        counting_sizes(ptr_buf, n_lines);

        FILE* output = fopen("Sorted.txt", "wb+");

        sort_lines(ptr_buf, 0, n_lines - 1, comp_str_in_alph);
        put_line(ptr_buf, n_lines - 1, output);

        sort_lines(ptr_buf, 0, n_lines - 1, comp_str_in_ryhm);
        put_line(ptr_buf, n_lines - 1, output);

        put_the_primary_line(buffer, len, output);

        fclose(input);
        fclose(output);

        free(buffer);
        free(ptr_buf);

        return 0;

    #endif
}

long int get_size_of_file(FILE* file)
{
    assert(file != NULL);

    long int len = 0;
    if (!fseek(file, 0, SEEK_END))
        len = ftell(file);
    len++;
    fseek(file, 0, SEEK_SET);

    return len;
}

char *get_the_text(FILE* file, size_t* length)
{
    assert(file != NULL && *length != 0);

    char* buffer = (char*) calloc(*length, sizeof(char));
    long int obj = fread(buffer, sizeof(char), *length, file);

    assert((size_t)obj == *length - 1);

    if (buffer[obj] != '\n')
    {
        (*length)++;
        buffer[*length - 2] = '\n';
    }

    buffer[*length - 1] = '\0';

    return buffer;
}

long int get_num_lines(char* buf, size_t length)
{
    assert(buf != NULL);

    long int n_lines = 0;
    char* ptr        = NULL;
    size_t len_tmp   = 0;

    do
    {
        ptr = strchr(buf, '\n');

        if (ptr != NULL)
        {
            len_tmp += ptr - buf + 1;

            if (len_tmp < length)
                n_lines++;

            buf = ptr + 1;
        }

    }while (ptr != NULL && len_tmp < length);

    return n_lines;
}

long int get_lines(char* buf, Ptrs_t* ptr_mas, size_t length)
{
    assert(buf != NULL && ptr_mas != NULL);

    size_t n_lines = 0;
    char* ptr      = NULL;
    size_t len_tmp = 0;

    (*ptr_mas).ptr = buf;

    do
    {
        ptr = strchr(buf, '\n');

        if (ptr != NULL)
        {
            len_tmp += ptr - buf;

            if (len_tmp < length)
            {
                n_lines++;
                ptr_mas++;
                buf = ptr + 1;
                (*ptr_mas).ptr = buf;
            }
        }

    }while (ptr != NULL && len_tmp < length);

    return n_lines;
}

void counting_sizes(Ptrs_t* ptr_mas, size_t n_lines)
{
    assert(ptr_mas != NULL && n_lines != 0);

    char *pt = NULL;
    for (size_t i = 0; i <= n_lines; i++)
    {
        pt = (ptr_mas[i]).ptr;
        while(*(pt++) != '\n' && ptr_mas[i].length < MAXSYMB)
        {
            (ptr_mas[i]).length++;
        }
    }
}

int comp_str_in_alph(Ptrs_t line1, Ptrs_t line2)
{
    while(!(*line1.ptr - *line2.ptr) && *line1.ptr != '\n' && *line2.ptr != '\n')
    {
        line1.ptr++;
        line2.ptr++;

        while (!isalpha(*line1.ptr) && *line1.ptr != '\n')
            line1.ptr++;
        while (!isalpha(*line2.ptr) && *line2.ptr != '\n')
            line2.ptr++;
    }

    if      (isalpha(*line1.ptr) && isalpha(*line2.ptr) && *line1.ptr > *line2.ptr)
        return  1;
    else if (isalpha(*line1.ptr) && isalpha(*line2.ptr) && *line1.ptr < *line2.ptr)
        return -1;
    else
        return 0;
}

int comp_str_in_ryhm(Ptrs_t line1, Ptrs_t line2)
{
    long int len1 = line1.length;
    long int len2 = line2.length;

    line1.ptr += line1.length;
    line2.ptr += line2.length;

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

void swap_lines(Ptrs_t* str, size_t line1, size_t line2)
{
    assert(str != NULL);

    char *temp     = str[line1].ptr;
    str[line1].ptr = str[line2].ptr;
    str[line2].ptr = temp;

    long int templen  = str[line1].length;
    str[line1].length = str[line2].length;
    str[line2].length = templen;
}

void sort_lines(Ptrs_t* line, size_t low, size_t up, int (*cmp)(Ptrs_t line1, Ptrs_t line2))
{
    assert(line != NULL);

    if ((long int)low - (long int)up >= 0)
        return;

    size_t last = low;

    swap_lines(line, low, (low + up)/2);

    for (size_t i = low + 1; i <= up; i++)
    {
        if (cmp(line[low], line[i]) > 0)
        {
            last++;
            swap_lines(line, last, i);
        }
    }
    swap_lines(line, low, last);

    sort_lines(line, low, last - 1, cmp);
    sort_lines(line, last + 1, up, cmp);
}

void put_line(Ptrs_t* put_ptr, const size_t n_lines, FILE* out)
{
    assert(put_ptr != NULL);
    assert(out     != NULL);

    char* pt = NULL;
    size_t l = 0;

    for (size_t i = 0; i <= n_lines; i++)
    {
        pt = put_ptr[i].ptr;
        l  = put_ptr[i].length + 1;

        fwrite(pt, sizeof(char), l, out);
    }
    fputc('\n', out);
}

void put_the_primary_line(char* line, size_t sz, FILE* out)
{
    assert(line != NULL);

    fwrite(line, sizeof(char), sz, out);
}

void unit_tests_for_comp_str_in_alph()
{
    {
        char str1[] = "";
        char str2[] = "";

        Ptrs string1 = {str1, 1};
        Ptrs string2 = {str2, 1};

        assert(comp_str_in_alph(string1, string2) == 0);
    }

    {
        char str1[] = "        ";
        char str2[] = "";

        Ptrs string1 = {str1, 9};
        Ptrs string2 = {str2, 1};

        assert(comp_str_in_alph(string1, string2) == 0);
    }

    {
        char str1[] = " ";
        char str2[] = "  ";

        Ptrs string1 = {str1, 2};
        Ptrs string2 = {str2, 3};

        assert(comp_str_in_alph(string1, string2) == 0);
    }

    {
        char str1[] = ",";
        char str2[] = " ";

        Ptrs string1 = {str1, 2};
        Ptrs string2 = {str2, 2};

        assert(comp_str_in_alph(string1, string2) == 0);
    }

    {
        char str1[] = "Meow\n";
        char str2[] = "M     eow\n";

        Ptrs string1 = {str1, 6};
        Ptrs string2 = {str2, 11};

        assert(comp_str_in_alph(string1, string2) == 0);
    }

    {
        char str1[] = "Hero\n";
        char str2[] = "Herb\n";

        Ptrs string1 = {str1, 6};
        Ptrs string2 = {str2, 6};

        assert(comp_str_in_alph(string1, string2) > 0);
    }

    {
        char str1[] = "Hello, world\n";
        char str2[] = "Hello, my world\n";

        Ptrs string1 = {str1, 14};
        Ptrs string2 = {str2, 17};

        assert(comp_str_in_alph(string1, string2) > 0);
    }

    {
        char str1[] = "My name is Anna\n";
        char str2[] = "My name is not Anna\n";

        Ptrs string1 = {str1, 17};
        Ptrs string2 = {str2, 21};

        assert(comp_str_in_alph(string1, string2) < 0);
    }
}

void unit_tests_for_comp_str_in_ryhm()
{
    {
        char str1[] = "";
        char str2[] = "";

        Ptrs string1 = {str1, 1};
        Ptrs string2 = {str2, 1};

        assert(comp_str_in_ryhm(string1, string2) == 0);
    }

    {
        char str1[] = "        ";
        char str2[] = "";

        Ptrs string1 = {str1, 9};
        Ptrs string2 = {str2, 1};

        assert(comp_str_in_ryhm(string1, string2) == 0);
    }

    {
        char str1[] = " ";
        char str2[] = "  ";

        Ptrs string1 = {str1, 2};
        Ptrs string2 = {str2, 3};

        assert(comp_str_in_ryhm(string1, string2) == 0);
    }

    {
        char str1[] = ",";
        char str2[] = " ";

        Ptrs string1 = {str1, 2};
        Ptrs string2 = {str2, 2};

        assert(comp_str_in_ryhm(string1, string2) == 0);
    }

    {
        char str1[] = "Meow\n";
        char str2[] = "M     eow\n";

        Ptrs string1 = {str1, 6};
        Ptrs string2 = {str2, 11};

        assert(comp_str_in_ryhm(string1, string2) == 0);
    }

    {
        char str1[] = "Hero\n";
        char str2[] = "Herb\n";

        Ptrs string1 = {str1, 6};
        Ptrs string2 = {str2, 6};

        assert(comp_str_in_ryhm(string1, string2) > 0);
    }

    {
        char str1[] = "Hello, world\n";
        char str2[] = "Hello, my world\n";

        Ptrs string1 = {str1, 14};
        Ptrs string2 = {str2, 17};

        assert(comp_str_in_ryhm(string1, string2) < 0);
    }

    {
        char str1[] = "My name is Anna\n";
        char str2[] = "My name is not Anna\n";

        Ptrs string1 = {str1, 17};
        Ptrs string2 = {str2, 21};

        assert(comp_str_in_ryhm(string1, string2) < 0);
    }
}

void unit_tests_for_sort_lines()
{
    {
        char str1[] = "ccc";
        char str2[] = "aaa";
        char str3[] = "bbb";

        Ptrs string1 = {str1, 4};
        Ptrs string2 = {str2, 4};
        Ptrs string3 = {str3, 4};

        Ptrs ptr[3] = {string1, string2, string3};

        sort_lines(ptr, 0, 2, comp_str_in_alph);
        assert(ptr[0].ptr == string2.ptr);
    }

    {
        char str1[] = "ccccc";
        char str2[] = "ccc";
        char str3[] = "ccc";

        Ptrs string1 = {str1, 6};
        Ptrs string2 = {str2, 4};
        Ptrs string3 = {str3, 4};

        Ptrs ptr[3] = {string1, string2, string3};

        sort_lines(ptr, 0, 2, comp_str_in_alph);
        assert((ptr[0].ptr == string3.ptr || ptr[0].ptr == string2.ptr) && ptr[2].ptr == string1.ptr);
    }

    {
        char str1[] = "heey";
        char str2[] = "oh my god";
        char str3[] = "oh, my god";
        char str4[] = "oh, my gosh";

        Ptrs string1 = {str1, 5 };
        Ptrs string2 = {str2, 10};
        Ptrs string3 = {str3, 11};
        Ptrs string4 = {str4, 12};

        Ptrs ptr[4] = {string1, string2, string3, string4};

        sort_lines(ptr, 0, 3, comp_str_in_alph);
        assert(ptr[0].ptr == string1.ptr && ptr[3].ptr == string4.ptr);
    }
}
