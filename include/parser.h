#ifndef ALTUMO_JET_MODELER_PARSER
#define ALTUMO_JET_MODELER_PARSER


#include "jet/Utf8String.h"


namespace jet{
namespace modeler{

    using namespace jet;


    enum Token{
        LEFT_BRACE,
        RIGHT_BRACE
    };


    class Parser{

        public:
            void parse( Utf8String );


        protected:
            void get_token();
            void error( const char msg[] );
            void expression();

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

            Token current_token;

    };


} //end namespace modeler
} //end namespace jet


#endif //ALTUMO_JET_MODELER_PARSER
