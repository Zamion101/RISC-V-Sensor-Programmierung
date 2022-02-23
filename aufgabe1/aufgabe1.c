#include <stdio.h>
#include <stdint.h>

void println(char *string){
    printf("%s\n", string);
}


int main(){
    println("Hello,");
    print_paragraph("World");
    printf("\n!!!");
    return 0;
}

