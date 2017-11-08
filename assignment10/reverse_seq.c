#include <stdio.h>
#include <stdlib.h>
#include "helper.h"

void reverse(char *str, int strlen)
{
    //reverse_str(str, strlen);
    char ch;
    for(int i = 0; i < strlen / 2; ++i){
    	ch = str[i];
    	str[i] = str[strlen - i - 1];
    	str[strlen - i - 1] = ch;
//    	swap(str[i], str[strlen - i - 1]);
    }
}
