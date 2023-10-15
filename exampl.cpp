// #include<stdio.h>
// #include<istream>
// #include<string.h>
// using namespace std;




#include <iostream>
#include <string>
#include <cstdio>

int convert(){
    const char *line = "P {12} {43}  \r\n";
    int start, end;
    char *text;
    sscanf(line, "%s {%d} {%d} ", text, &start, &end);
    printf("start %i, end %i, string %s\n", start, end, text);
    return 0;
}

int main() {
    // std::string data = "P {12} {43}  \r\n";
    // // const char *converted_data = data.c_str();
    // const char *converted_data = "P {12} {43}  \r\n";

    // std::cout << "Converted data: " << converted_data << std::endl;

    // int start, end;
    // char *text;
    // sscanf(converted_data, "%s {%d} {%d} ", text, &start, &end);
    // std::cout <<"start , end , string " << start << end << text<< std::endl;

    convert();

    return 0;
}
