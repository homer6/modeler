#ifndef ALTUMO_JET_MODELER_PARSER
#define ALTUMO_JET_MODELER_PARSER


#include "jet/Utf8String.h"
#include "jet/Exception.h"

#include "Token.h"
#include "Model.h"

#include <vector>
#include <fstream>


namespace jet{
namespace modeler{

    using namespace std;
    using namespace ::jet;
    using namespace ::jet::modeler;


    class Parser{

        public:

            Parser();
            virtual ~Parser();

            void parse( Utf8String *source_text );

            void printModels();
            void writeModelFiles();


        protected:

            // Resets this object back to an empty parser
            void resetParser();


            // Tokenizing Methods

                void printTokens();
                void assertNoUnknownTokens();
                void stripWhitespaceTokens();
                void tokenize();
                void addToken( Token *token );
                Token* getEndToken();
                Token* getCurrentToken();
                Token* getPreviousToken();
                void advanceOneToken();
                bool hasMoreTokens();


            // Error Handling

                void error( const char error_message[] );
                void error( const char error_message[], Token *token );


            // Recursive Descent parser methods

                bool acceptTokenType( TokenType token_type );
                bool expectTokenType( TokenType token_type );

                void parseTokens();
                void model();
                void modelProperty();


            // Model Management

                Model* getCurrentModel() const;
                void addModel( Model *model );


            Utf8String source_text;
            size_t current_position;

            vector<Token*>::iterator current_token;
            vector<Token*> token_stream;

            ModelMap::iterator current_model;
            ModelMap models;



    };


} //end namespace modeler
} //end namespace jet


#endif //ALTUMO_JET_MODELER_PARSER
