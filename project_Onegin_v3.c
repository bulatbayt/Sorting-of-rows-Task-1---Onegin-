#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <sys/stat.h> 

#define SIZE_ROWS 100
#define NUM_ROWS 100

struct File_info
{
    int max_rows;
    int size_file;

};

//char* my_strdup (const char* buffer);

int ReadFromFile (const char* name_file, char* index[], char** buffer);

void  PrintStrings (const int max_rows, char* index[], const char* ReasonPrint);

char* my_strcpy    (char* array, const char* string);

size_t my_strlen   (const char* string);

int my_strcmp_alpha_up (const char* s1, const char* s2);

int CompareFuncUp (const void* StringLeft, const void* StringRight);

void buble_sort (char* index[], int max_rows, int (*ComparFunc)(const void*, const void*));

int CompareFuncDown (const void* StringLeft, const void* StringRight);

int my_strcmp_alpha_down (const char* s1, const char* s2);

void change_index (char** index1, char** index2);

int CompareFuncOriginal (const void* PointerLeft, const void* PointerRight);


int main(void)
{
    struct File_info Onegin = { }; //заполнить структуру важными данными о файле
    
    char* index[NUM_ROWS];

    char* buffer;

    int max_rows = ReadFromFile("Onegin.txt", index, &buffer);

    printf ("max rows = %d\n", max_rows);

    if (max_rows <= 0)
    {
        fprintf(stderr, "Nothing was read from file.\n");
        return 1;
    }

    PrintStrings (max_rows, index, "Check INPUT");

    qsort (index, max_rows, sizeof(char*), CompareFuncUp);

    PrintStrings (max_rows, index, "Check Up Sort");

    buble_sort(index, max_rows, CompareFuncDown); // сделать buble sort c указателем на функцию, а потом реализовать компаратор для Original

    PrintStrings(max_rows, index, "Check Sort RAP");

    //сортировка указателей по возрастанию 

    buble_sort (index, max_rows, CompareFuncOriginal);

    PrintStrings(max_rows, index, "Original Onegin");

    free (buffer);

    return 0;
}


int ReadFromFile(const char* name_file, char* index[], char** buffer)
{
    assert (name_file != NULL);
    assert (index != NULL);

    FILE* onegin_file = fopen(name_file, "rb");
    int file_dis = fileno (onegin_file);

    if (onegin_file == NULL)
    {
        perror("fopen"); 
        return 0;
    }

    struct stat stat_file;

    fstat (file_dis, &stat_file);

    int Len_buffer = stat_file.st_size;

    *buffer = calloc ( Len_buffer, sizeof(char)); // создать указатель на буффер в main е а здесь callocом веделить память 

    fread (*buffer, sizeof(char), Len_buffer, onegin_file); //теперь нужно расставить указатели index, нудно посчитать количество \n

    int max_rows = 0; 

    printf ("Len buffer = %d\n", Len_buffer);

    index [0] = *buffer; 

    int str_index = 0;
    for (int i = 0; i < Len_buffer; i ++)
    {
    
        if ( *(*buffer + i) == '\n')
        {
            *(*buffer + i) = '\0';       
            max_rows ++; // тут работать с буффером или с указателями ??
            index [ max_rows] =  *buffer + i + 1;
        }
    }
    
    fclose(onegin_file);

    printf ("index 1: %s\n", index[0]);
    
    return max_rows;
}

/*char* my_strdup(const char* buffer)
{
    assert(buffer != NULL);

    size_t str_size = my_strlen(buffer) + 1;

    char*  PointerStr = calloc(str_size, sizeof(char));

    if (PointerStr == NULL)
    {
        return NULL;
    }

    return my_strcpy(PointerStr, buffer); // скопирую сразу здесь из буфера в массив
}*/

void PrintStrings(const int max_rows, char* index[], const char* ReasonPrint)
{
    assert(index != NULL);
    assert(ReasonPrint != NULL);

    printf("                   === %s ===\n", ReasonPrint);
    for (int i = 0; i < max_rows; i++)
    {
        printf("string №%d <Address = %p> <StrLen = %u>\n",i + 1, index[i], my_strlen(index[i]));
        printf("%s\n", index[i]);
    }
    printf("\n-----------------------------------------------------------------\n\n");
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

int CompareFuncUp(const void* StringLeft, const void* StringRight) //qsort передает void*
{
    const char* s1 = *(const char* const*)StringLeft;
    const char* s2 = *(const char* const*)StringRight;

    return my_strcmp_alpha_up(s1, s2);
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

    return (int)NAN; //может возвращать NAN
}

void buble_sort (char* index[], int max_rows, int (*ComparFunc)(const void*, const void*))
{   
    assert (index != NULL);

    size_t amount_run = 0;

    for (size_t run = 0; run < max_rows - (size_t)1; run ++)
    {
        for (size_t i = 0; i < (size_t)max_rows - 1 - amount_run; i++)
        {
            if ( ComparFunc (index + i, index + i + 1) > 0)
            {
                change_index (index+i, index+i+1);
            }
        }

    amount_run++;
    }

}

int CompareFuncDown(const void* StringLeft, const void* StringRight) //qsort передает void*
{
    const char* s1 = *(const char**) StringLeft;// раньше были void* в параметре функции
    const char* s2 = *(const char**) StringRight;

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
        while (i1 >= 0 && !isalpha(s1[i1]))
        {
            i1--;
        }

        while (i2 >= 0 && !isalpha(s2[i2]))
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
 /* какое значение возвращать, чтобы убрать варнинг вариант :
 abort;
 return 0;*/
}

void change_index(char** p1, char** p2) //передавать указатели!!!
{
    char* tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}

int CompareFuncOriginal (const void* PointerLeft, const void* PointerRight)
{
    const char* s1 = *(const char**) PointerLeft; // БАГ БЫЛ ЗДЕСЬ . Было  const char* s1 = (const char*) PointerLeft;
    const char* s2 = *(const char**) PointerRight;

    /*printf("\n\n<s1:%p>\n<s2:%p>\n\n", s1, s2);
    printf("\n\n<s1:%d>\n<s2:%d>\n\n", s1, s2);
    */

    return (int)s1 - (int)s2;
}