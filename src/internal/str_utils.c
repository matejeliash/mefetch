#include <string.h>
#include<ctype.h>
#include<stdlib.h>
#include <stdbool.h>

// create new string  that is trimmed
// !!! malloc is used
char* trim( char* str){

    if (str == NULL || str[0] =='\0') return NULL;

    char* start = str;
    char* end = str + (strlen(str) -1);

    while(*start && isspace((unsigned char)*start)){
        start++;
    }

    if (start > end) return NULL;

    while(end > start && isspace((unsigned char)*end)){
        end--;
    }

    int just_char_len = end -start +1 ;
    char* result = malloc(just_char_len+1);
    if(!result)  return NULL;

    memcpy(result,start,just_char_len);
    result[just_char_len] = '\0';

    return result;

}





bool has_prefix(const char *str, const char *prefix) {
    if (!str || !prefix) return false;

    size_t len_prefix = strlen(prefix);
    size_t len_str = strlen(str);

    if (len_str < len_prefix) return false;       // prefix longer than string
    return strncmp(str, prefix, len_prefix) == 0; // compare first len_prefix chars
}
