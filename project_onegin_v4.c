#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <sys/stat.h> 

//#define SIZE_ROWS 100
#define NUM_ROWS 6823

struct File_info
{
    int max_rows;
    int long long size_file;

};


int ReadFromFile (const char* name_file, char* index[], char** buffer, struct File_info* Onegine);

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
 
    FILE* clearing_file_contents = fopen ("Onegin_write.txt", "w");

    fclose (clearing_file_contents);//нормально ли это для чистки?
    
    char* index[NUM_ROWS];

    char* buffer;

    Onegin.max_rows = ReadFromFile ("Onegin_full.txt", index, &buffer, &Onegin);

    //fprintf (file_write, "max rows = %d\n", Onegin.max_rows);

    if (Onegin.max_rows <= 0)
    {
        fprintf (stderr, "Nothing was read from file.\n");
        return 1;
    }

    PrintStrings (Onegin.max_rows, index, "Check INPUT");

    qsort (index, Onegin.max_rows, sizeof(char*), CompareFuncUp);

    PrintStrings (Onegin.max_rows, index, "Check Up Sort");

    buble_sort (index, Onegin.max_rows, CompareFuncDown); // сделать buble sort c указателем на функцию, а потом реализовать компаратор для Original

    PrintStrings (Onegin.max_rows, index, "Check Sort RAP");

    buble_sort (index, Onegin.max_rows, CompareFuncOriginal);

    PrintStrings (Onegin.max_rows, index, "Original Onegin");

    free (buffer);

    return 0;
}


int ReadFromFile (const char* name_file, char* index[], char** buffer, struct File_info* Onegin)
{
    assert (name_file != NULL);
    assert (index != NULL);

    FILE* onegin_file = fopen (name_file, "rb");

    //FILE* file_write = fopen ("Onegin_write.txt", "w+");

    int file_dis = fileno (onegin_file);

    if (onegin_file == NULL)
    {
        perror ("fopen"); 
        return 0;
    }

    struct stat stat_file;

    fstat (file_dis, &stat_file);

    (*Onegin).size_file = stat_file.st_size;

    *buffer = calloc ((*Onegin).size_file, sizeof(char)); // создать указатель на буффер в main е а здесь callocом веделить память 

    fread (*buffer, sizeof(char), (*Onegin).size_file, onegin_file); //теперь нужно расставить указатели index, нудно посчитать количество \n

    int max_rows = 0; 

    //fprintf (file_write,"Len buffer = %d\n", (*Onegin).size_file);

    index [0] = *buffer; 

    int str_index = 0;
    for (int i = 0; i < (*Onegin).size_file; i ++)
    {
    
        if ( *(*buffer + i) == '\n')
        {
            *(*buffer + i) = '\0';       
            max_rows ++; // тут работать с буффером или с указателями ??
            index [ max_rows] =  *buffer + i + 1;
        }
    }
    
    fclose (onegin_file);
    
    //fclose (file_write);

    return max_rows;
}


void PrintStrings (const int max_rows, char* index[], const char* ReasonPrint)
{
    assert (index != NULL);
    assert (ReasonPrint != NULL);

    FILE* file_write = fopen ("Onegin_write.txt", "a+");  
        
    fprintf (file_write, "\n-----------------------------------------------------------------\n\n");
    fprintf (file_write, "\n-----------------------------------------------------------------\n\n");
    fprintf (file_write, "\n-----------------------------------------------------------------\n\n");
    fprintf (file_write, "                   === %s ===\n", ReasonPrint);
    for (int i = 0; i < max_rows; i++)
    {
        //printf("string №%d <Address = %p> <StrLen = %u>\n",i + 1, index[i], my_strlen(index[i]));
        fprintf(file_write, "%s\n", index[i]);
    }
    fprintf (file_write, "\n-----------------------------------------------------------------\n\n");

    fclose (file_write);
}

size_t my_strlen (const char* string)
{
    assert (string != NULL);

    size_t value = 0;
    while (string[value] != '\0')
    {
        value++;
    }

    return value;
}

int CompareFuncUp (const void* StringLeft, const void* StringRight) //qsort передает void*
{
    const char* s1 = *(const char* const*)StringLeft;
    const char* s2 = *(const char* const*)StringRight;

    return my_strcmp_alpha_up (s1, s2);
}

int my_strcmp_alpha_up (const char* s1, const char* s2) 
{
    assert (s1 != NULL);
    assert (s2 != NULL);

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

        int c1 = tolower (*s1);
        int c2 = tolower (*s2);

        if (c1 != c2)
        {
            return c1 - c2;
        }

        if (c1 == c2)
        {
            return 0;
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
            if (ComparFunc (index + i, index + i + 1) > 0)
            {
                change_index (index+i, index+i+1);
            }
        }

    amount_run++;
    }

}

int CompareFuncDown (const void* StringLeft, const void* StringRight) //qsort передает void*
{
    const char* s1 = *(const char**) StringLeft;// раньше были void* в параметре функции
    const char* s2 = *(const char**) StringRight;

    return my_strcmp_alpha_down(s1, s2);
}

int   my_strcmp_alpha_down (const char* s1, const char* s2)
{
    assert (s1 != NULL);
    assert (s2 != NULL);

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

    return (int)s1 - (int)s2;
}