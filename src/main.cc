
#include <iostream>
#include <bitset>

#include "jet/Utf8String.h"


using namespace std;
using namespace jet;




template< class T >
void print_as_binary( T value, ostream& output_stream ){

    bitset< sizeof(T) * 8 > x( value );

    output_stream << x ;

}



int main( int argc, char** argv ){


    Utf8String my_string( "Hello", 5 );

    cout << my_string << endl;


    unsigned char current_byte = 0;

    unsigned int value = 0x80 >> 7;

    int shift_bytes = sizeof(unsigned int) - 1;

    current_byte = value;

    cout << shift_bytes << endl;

    //cout << value << endl;

    //cout << current_byte << endl;

    cout << "Value as Binary: ";
    print_as_binary( value, cout );
    cout << endl;

    cout << "Current Byte as Binary: ";
    print_as_binary( current_byte, cout );
    cout << endl;

    cout << "Shift Bytes as Binary: ";
    print_as_binary( shift_bytes, cout );
    cout << endl;

    cout << "Literal as Binary: ";
    print_as_binary( 0x01, cout );
    cout << endl;

    //print_as_binary( my_string, cout );

    return 0;

}
