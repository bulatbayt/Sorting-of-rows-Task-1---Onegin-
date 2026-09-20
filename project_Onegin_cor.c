#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SIZE_ROWS 100
#define NUM_ROWS 100

int ReadFromFile (const char* name_file, const int size_rows, char* index[]);

void  PrintStrings (const int max_rows, const char* index[], const char* ReasonPrint);

char* my_strdup    (const char* buffer);

char* my_strcpy    (char* array, const char* string);

size_t my_strlen   (const char* string);

int my_strcmp_alpha_up (const char* s1, const char* s2);

void  FreeStrings  (char* index[], const int max_rows);

int CompareFuncUp (const void* StringLeft, const void* StringRight);

void buble_sort (char* index[], int max_rows);

int CompareFuncDown (const void* StringLeft, const void* StringRight);

int my_strcmp_alpha_down (const char* s1, const char* s2);

void change_index (char** index1, char** index2);/// 

int main(void)
{
    char* index[NUM_ROWS];

    int max_rows = ReadFromFile("Onegin.txt", SIZE_ROWS, index);

    if (max_rows <= 0)
    {
        fprintf(stderr, "Nothing was read from file.\n");
        return 1;
    }

    PrintStrings(max_rows, (const char**)index, "Check Input");

    qsort(index, max_rows, sizeof(char*), CompareFuncUp); 

    PrintStrings(max_rows, (const char**)index, "Check Sort UP");

    buble_sort(index, max_rows);

    PrintStrings(max_rows, (const char**)index, "Check Sort RAP");

    FreeStrings(index, max_rows);

    return 0;
}

int ReadFromFile(const char* name_file, const int size_rows, char* index[])
{
    assert(name_file != NULL);
    assert(index != NULL);

    FILE* onegin_file = fopen(name_file, "r");
    if (onegin_file == NULL)
    {
        perror("fopen"); // 
        return 0;
    }

    char buffer[SIZE_ROWS] = { };
    int  max_rows = 0;

    while (max_rows < size_rows && fgets(buffer, SIZE_ROWS, onegin_file) != NULL)
    {
        index[max_rows] = my_strdup(buffer);

        if (index[max_rows] == NULL)
        {
            printf ("ERROR"); 
            break;
        }

        max_rows++;
    }

    fclose(onegin_file);

    return max_rows;
}

void PrintStrings(const int max_rows, const char* index[], const char* ReasonPrint)
{
    assert(index != NULL);
    assert(ReasonPrint != NULL);

    printf("                   === %s ===\n", ReasonPrint);
    for (int i = 0; i < max_rows; i++)
    {
        printf("string №%d <Address = %p> <StrLen = %u>\n",i + 1, index[i], my_strlen(index[i]));
        printf("%s", index[i]);
    }
    printf("\n-----------------------------------------------------------------\n\n");
}

void FreeStrings(char* index[], const int max_rows)
{
    assert(index != NULL);

    for (int i = 0; i < max_rows; i++)
    {
        free(index[i]);
        index[i] = NULL;
    }
}

char* my_strdup(const char* buffer)
{
    assert(buffer != NULL);

    size_t str_size = my_strlen(buffer) + 1;

    char*  PointerStr = calloc(str_size, sizeof(char));

    if (PointerStr == NULL)
    {
        return NULL;
    }

    return my_strcpy(PointerStr, buffer); // скопирую сразу здесь из буфера в массив
}

char* my_strcpy(char* array, const char* string)
{
    assert(array != NULL);
    assert(string != NULL);

    size_t i = 0;
    for (; string[i] != '\0'; i++)
    {
        array[i] = string[i];
    }

    array[i] = '\0';

    return array;
}

size_t my_strlen(const char* string)
{
    assert(string != NULL);

    size_t value = 0;
    while (string[value] != '\0')
    {
        value++;
    }

    return value;
}

int my_strcmp_alpha_up(const char* s1, const char* s2) // 
{
    assert(s1 != NULL);
    assert(s2 != NULL);

    while (1000-7 > 0)
    {
        while (*s1 != '\0' && !isalpha(*s1)) 
        {
        s1++;
        }

        while (*s2 != '\0' && !isalpha(*s2))
        {
        s2++;
        }

        if (*s1 == '\0' || *s2 == '\0')
        {
            break;
        }

        int c1 = tolower(*s1);
        int c2 = tolower(*s2);

        if (c1 != c2)
        {
            return c1 - c2;
        }

        s1++;
        s2++;
    }

    return 0; //может сделать возвращать NAN
}


void buble_sort (char* index[], int max_rows)
{   
    assert (index != NULL);

    size_t amount_run = 0;

    for (size_t run = 0; run < max_rows - 1; run ++)
    {
        for (size_t i = 0; i < (size_t)max_rows - 1 - amount_run; i++)
        {
            if ( CompareFuncDown (index + i, index + i + 1) > 0)
            {
                change_index (index+i, index+i+1);
            }
        }

    amount_run++;
    }

}

int CompareFuncUp(const void* StringLeft, const void* StringRight) //qsort передает void*
{
    const char* s1 = *(const char* const*)StringLeft;
    const char* s2 = *(const char* const*)StringRight;

    return my_strcmp_alpha_up(s1, s2);
}

void change_index(char** p1, char** p2) //передавать указатели!!!
{
    char* tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}
int CompareFuncDown(const void* StringLeft, const void* StringRight) //qsort передает void*
{
    const char* s1 = *(const char* const*)StringLeft;
    const char* s2 = *(const char* const*)StringRight;

    return my_strcmp_alpha_down(s1, s2);
}

int   my_strcmp_alpha_down (const char* s1, const char* s2)
{
    assert(s1 != NULL);
    assert(s2 != NULL);

    int i1 = (int)my_strlen(s1);
    int i2 = (int)my_strlen(s2);

    while (1000-7 >0)
    {
        while (i1 >= 0 && !isalpha((unsigned char)s1[i1]))
        {
            i1--;
        }

        while (i2 >= 0 && !isalpha((unsigned char)s2[i2]))
        {
            i2--;
        }

        if (i1 < 0 || i2 < 0)
        {
            break;
        }

        int c1 = tolower(s1[i1]);
        int c2 = tolower(s2[i2]);

        if (c1 != c2)
        {
            return c1 - c2;
        }

        i1--;
        i2--;
    }

}