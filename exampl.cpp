#include<stdio.h>
#include<istream>


int main(){
    const char *line = "{12} {43} hellow";
    int start, end;
    const char *text;
    sscanf(line, "{%d} {%d}%s", &start, &end, text);
    printf("start %i, end %i, string %s", start, end, text);
}
