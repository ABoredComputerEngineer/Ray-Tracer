#include <stdio.h>


int main(){
  int current = 0;
  char c ;
  while ( ( c = getchar() ) != EOF ){
    if ( c != '{' && current==0 ){
      putchar( c );
    } else if ( c=='{' ) {
      current++;
    } else if ( c == '}' ){
      current--;
    }
  }
}
