
#include <iostream>
#include <bitset>

#include "jet/Utf8String.h"
#include "jet/File.h"

#include "parser.h"


using namespace std;
using namespace jet;
using namespace jet::modeler;


template< class T >
void print_as_binary( T value, ostream& output_stream ){

    bitset< sizeof(T) * 8 > x( value );

    output_stream << x ;

}


void show_usage(){

    cout << "Usage: jm <filename>" << endl;

}




int main( int argc, char** argv ){


    if( argc < 2 ){
        show_usage();
        return 1;
    }

    Utf8String *filename = new Utf8String( argv[1] );

    File *file = new File( *filename );

    Utf8String *contents = new Utf8String;
    *contents = file->getContents();

    Parser *parser = new Parser;
    parser->parse( *contents );


    delete parser;
    delete contents;
    delete file;
    delete filename;



    /*
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

    */





    return 0;

}
