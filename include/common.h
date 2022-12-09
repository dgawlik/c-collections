#include <stdlib.h>
#include <stdarg.h>
#include <string.h>


char* string_concat(char* lhs, char* rhs)
{
    int len_lhs = strlen(lhs);
    int len_rhs = strlen(rhs);

    char* new_str = (char*) malloc((len_lhs+len_rhs+1)*sizeof(char));
    strcpy(new_str, lhs); 
    new_str = strcat(new_str, rhs); 

    return new_str;
}
