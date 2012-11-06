#ifndef ALTUMO_JET_MODELER_PARSER
#define ALTUMO_JET_MODELER_PARSER


#include "jet/Utf8String.h"
#include "jet/Exception.h"

#include <vector>


namespace jet{
namespace modeler{

    using namespace jet;
    using namespace std;


    enum TokenType{
        NULL_TOKEN,
        LEFT_BRACE,
        RIGHT_BRACE,
        NEW_LINE,
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
                    case NULL_TOKEN:
                        return Utf8String( "Null Token" );
                    case LEFT_BRACE:
                        return Utf8String( "Left Brace" );
                    case RIGHT_BRACE:
                        return Utf8String( "Right Token" );
                    case NEW_LINE:
                        return Utf8String( "New Line" );
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


    class Parser{

        public:

            Parser(){

            }

            void parse( Utf8String *source_text ){

                if( source_text->isEmpty() ){
                    return;
                }

                this->resetParser();

                this->source_text = *source_text;

                this->tokenize();

                this->printTokens();

            }


            virtual ~Parser(){

                if( this->token_stream.size() > 0 ){

                    vector<Token*>::iterator iterator;
                    for( iterator = this->token_stream.begin(); iterator != this->token_stream.end(); iterator++ ){
                        delete *iterator;
                    }

                }

            }


        protected:

            void resetParser(){

                if( this->token_stream.size() > 0 ){
                    vector<Token*>::iterator iterator;
                    for( iterator = this->token_stream.begin(); iterator != this->token_stream.end(); iterator++ ){
                        delete *iterator;
                    }
                }
                this->current_position = 0;
                this->source_text = Utf8String();

            }


            void printTokens(){

                Token *token;
                vector<Token*>::iterator iterator;

                for( iterator = this->token_stream.begin(); iterator != this->token_stream.end(); iterator++ ){

                    token = *iterator;
                    if( token->type == NAME ){
                        cout << token->line_number << ":" << token->line_character_offset << " - " << Token::getTypeDescription(token->type) << " - Contents: " << token->contents << endl;
                    }else{
                        cout << token->line_number << ":" << token->line_character_offset << " - " << Token::getTypeDescription(token->type) << endl;
                    }

                }

            }




            void tokenize(){

                size_t number_of_characters = this->source_text.getSize();
                size_t line_number = 1;
                size_t line_character_position = 0;

                char current_character;

                for( size_t x = 0; x < number_of_characters; x++ ){

                    //cout << "Line: " << line_number << " Offset: " << line_character_position << endl;

                    this->current_position = x;

                    current_character = this->source_text.getAsciiCharacterAtIndex( x );

                    switch( current_character ){

                        case '\n':
                            line_number++;
                            line_character_position = 0;
                            this->addToken( new Token(NEW_LINE, line_number, line_character_position) );
                            line_character_position++;
                            continue;

                        case ';':
                            this->addToken( new Token(SEMI_COLON, line_number, line_character_position) );
                            line_character_position++;
                            continue;

                        case '{':
                            this->addToken( new Token(LEFT_BRACE, line_number, line_character_position) );
                            line_character_position++;
                            continue;

                        case '}':
                            this->addToken( new Token(RIGHT_BRACE, line_number, line_character_position) );
                            line_character_position++;
                            continue;

                        case ' ':
                            this->addToken( new Token(SPACE, line_number, line_character_position) );
                            line_character_position++;
                            continue;

                        case ',':
                            this->addToken( new Token(COMMA, line_number, line_character_position) );
                            line_character_position++;
                            continue;

                        case '<':
                            this->addToken( new Token(LESS_THAN, line_number, line_character_position) );
                            line_character_position++;
                            continue;

                        case '>':
                            this->addToken( new Token(GREATER_THAN, line_number, line_character_position) );
                            line_character_position++;
                            continue;

                    };

                    if(
                        ( current_character >= 0x41 && current_character <= 0x5A ) ||  //upper-case alpha
                        ( current_character >= 0x61 && current_character <= 0x7A ) ||  //lower-case alpha
                        ( current_character >= 0x30 && current_character <= 0x39 )     //numeric
                    ){

                        if( this->token_stream.size() == 0 ){

                            //if a name starts the file
                            this->addToken( new Token(NAME, Utf8String(current_character), line_number, line_character_position) );

                        }else{

                            Token *token = this->getCurrentToken();

                            if( token->type == NAME ){
                                //append it if a name token has already been started
                                token->contents += current_character;
                            }else{
                                //this is a new name token following another non-name token; start a new name
                                this->addToken( new Token(NAME, Utf8String(current_character), line_number, line_character_position) );
                            }

                        }

                    }

                    line_character_position++;


                }


            }


            void addToken( Token *token ){

                this->token_stream.push_back( token );
                //this->current_token = this->token_stream.back();

            }



            Token* getCurrentToken(){

                if( this->token_stream.size() == 0 ){
                    throw new Exception( "Tried to get a token when none exist." );
                }
                return this->token_stream.back();

            }


            void error( const char error_message[] ){

                cout << "Error: " << error_message << endl;
                throw error_message;

            }

            /*

            int accept( Token token ){

                if( current_token == token ){
                    get_token();
                    return 1;
                }
                return 0;

            }


            int expect( Token token ){

                if( accept(token) )
                    return 1;
                error("expect: unexpected token");
                return 0;

            }


            void expression(){

            }

            */



            Utf8String source_text;
            size_t current_position;

            vector<Token*>::iterator current_token;
            vector<Token*> token_stream;


    };


} //end namespace modeler
} //end namespace jet


#endif //ALTUMO_JET_MODELER_PARSER
