#ifndef ALTUMO_JET_MODELER_TOKEN
#define ALTUMO_JET_MODELER_TOKEN


#include "jet/Utf8String.h"

#include <vector>


namespace jet{
namespace modeler{

    using namespace jet;
    using namespace std;


    enum TokenType{
        UNKNOWN,
        NULL_TOKEN,
        LEFT_BRACE,
        RIGHT_BRACE,
        NEW_LINE,
        CARRIAGE_RETURN,
        SEMI_COLON,
        NAME,
        SPACE,
        COMMA,
        LESS_THAN,
        GREATER_THAN
    };


    class Token{

        public:

            Token( TokenType type, size_t line_number, size_t line_character_offset ):
                type(type), contents(Utf8String()), line_number(line_number), line_character_offset(line_character_offset)
            {

            }

            Token( TokenType type, Utf8String contents, size_t line_number, size_t line_character_offset ):
                type(type), contents(contents), line_number(line_number), line_character_offset(line_character_offset)
            {

            }

            static Utf8String getTypeDescription( TokenType type ){

                switch( type ){
                    case UNKNOWN:
                        return Utf8String( "Unknown Token" );
                    case NULL_TOKEN:
                        return Utf8String( "Null Token" );
                    case LEFT_BRACE:
                        return Utf8String( "Left Brace" );
                    case RIGHT_BRACE:
                        return Utf8String( "Right Brace" );
                    case NEW_LINE:
                        return Utf8String( "New Line" );
                    case CARRIAGE_RETURN:
                        return Utf8String( "Carriage Return" );
                    case SEMI_COLON:
                        return Utf8String( "Semicolon" );
                    case NAME:
                        return Utf8String( "Name" );
                    case SPACE:
                        return Utf8String( "Space" );
                    case COMMA:
                        return Utf8String( "Comma" );
                    case LESS_THAN:
                        return Utf8String( "Less Than" );
                    case GREATER_THAN:
                        return Utf8String( "Greater Than" );
                    default:
                        return Utf8String( "Unknown" );
                }

            }


            TokenType type;
            Utf8String contents;
            size_t line_number;
            size_t line_character_offset;

    };


} //end namespace modeler
} //end namespace jet


#endif //ALTUMO_JET_MODELER_TOKEN
