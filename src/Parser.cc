#include "Parser.h"


namespace jet{
namespace modeler{


    Parser::Parser(){

    }


    void Parser::parse( Utf8String *source_text ){

        if( source_text->isEmpty() ){
            return;
        }

        this->resetParser();

        this->source_text = *source_text;

        this->tokenize();

        this->stripWhitespaceTokens();
        this->assertNoUnknownTokens();

        this->current_token = this->token_stream.begin();

        //this->printTokens();

        this->parseTokens();

    }


    Parser::~Parser(){

        if( this->token_stream.size() > 0 ){

            vector<Token*>::iterator iterator;
            for( iterator = this->token_stream.begin(); iterator != this->token_stream.end(); iterator++ ){
                delete *iterator;
            }

        }

    }



    void Parser::resetParser(){

        // Reset simple member variables
            this->current_position = 0;
            this->source_text = Utf8String();


        // Free each of the Tokens and clear out the vector
            if( this->token_stream.size() > 0 ){
                vector<Token*>::iterator token_iterator;
                for( token_iterator = this->token_stream.begin(); token_iterator != this->token_stream.end(); token_iterator++ ){
                    delete *token_iterator;
                }
                this->token_stream.clear();
            }


        // Free each of the Models and clear out the map
            if( this->models.size() > 0 ){
                ModelMap::iterator model_iterator;
                for( model_iterator = this->models.begin(); model_iterator != this->models.end(); model_iterator++ ){
                    delete model_iterator->second;
                }
                this->models.clear();
            }


    }



    void Parser::printTokens(){

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



    void Parser::assertNoUnknownTokens(){

        Token *token;
        vector<Token*>::iterator iterator;

        for( iterator = this->token_stream.begin(); iterator != this->token_stream.end(); iterator++ ){

            token = *iterator;
            if( token->type == UNKNOWN ){
                this->error( "Unknown token.", token );
            }

        }

    }



    void Parser::stripWhitespaceTokens(){

        Token *token;
        vector<Token*>::iterator iterator;

        vector<Token*> *new_token_stream = new vector<Token*>;

        for( iterator = this->token_stream.begin(); iterator != this->token_stream.end(); iterator++ ){

            token = *iterator;
            if( token->type != SPACE && token->type != NEW_LINE && token->type != CARRIAGE_RETURN && token->type != TAB ){
                new_token_stream->push_back( token );
            }else{
                delete token;
            }

        }

        swap( this->token_stream, *new_token_stream );
        delete new_token_stream;

    }



    void Parser::tokenize(){

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

                case 0x0D:
                    this->addToken( new Token(CARRIAGE_RETURN, line_number, line_character_position) );
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

                case '\t':
                    this->addToken( new Token(TAB, line_number, line_character_position) );
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
                ( current_character >= 0x30 && current_character <= 0x39 ) ||  //numeric
                ( current_character == 0x5F )                                  //underscore
            ){

                if( this->token_stream.size() == 0 ){

                    //if a name starts the file
                    this->addToken( new Token(NAME, Utf8String(current_character), line_number, line_character_position) );
                    line_character_position++;
                    continue;

                }else{

                    Token *token = this->getEndToken();

                    if( token->type == NAME ){
                        //append it if a name token has already been started
                        token->contents += current_character;
                        line_character_position++;
                        continue;
                    }else{
                        //this is a new name token following another non-name token; start a new name
                        this->addToken( new Token(NAME, Utf8String(current_character), line_number, line_character_position) );
                        line_character_position++;
                        continue;
                    }

                }

            }

            this->addToken( new Token(UNKNOWN, Utf8String(current_character), line_number, line_character_position) );
            line_character_position++;


        }


    }



    void Parser::addToken( Token *token ){

        this->token_stream.push_back( token );
        //this->current_token = this->token_stream.back();

    }



    Token* Parser::getEndToken(){

        if( this->token_stream.size() == 0 ){
            throw new Exception( "Tried to get a token when none exist." );
        }
        return this->token_stream.back();

    }



    Token* Parser::getCurrentToken(){

        if( this->current_token == this->token_stream.end() ){
            throw new Exception( "getCurrentToken: End of token stream." );
        }

        return *this->current_token;

    }



    Token* Parser::getPreviousToken(){

        if( this->current_token == this->token_stream.begin() ){
            throw new Exception( "getPreviousToken: Beginning of token stream." );
        }

        return *( this->current_token - 1 );

    }



    void Parser::advanceOneToken(){

        this->current_token++;

        if( this->current_token == this->token_stream.end() ){
            throw new Exception( "advanceOneToken: End of token stream." );
        }

    }



    bool Parser::hasMoreTokens(){

        if( this->current_token + 1 == this->token_stream.end() ){
            return false;
        }else{
            return true;
        }

    }



    void Parser::error( const char error_message[] ){

        cout << "Error: " << error_message << endl;
        cout << "Line: " << this->getCurrentToken()->line_number << endl;
        cout << "Line character offset: " << this->getCurrentToken()->line_character_offset << endl;
        throw error_message;

    }



    void Parser::error( const char error_message[], Token *token ){

        cout << "Error: " << error_message << endl;
        cout << "Line: " << token->line_number << endl;
        cout << "Line character offset: " << token->line_character_offset << endl;
        throw error_message;

    }



    bool Parser::acceptTokenType( TokenType token_type ){

        if( this->getCurrentToken()->type == token_type ){

            if( this->hasMoreTokens() ){
                this->advanceOneToken();
                return true;
            }

        }
        return false;

    }



    bool Parser::expectTokenType( TokenType token_type ){

        if( this->acceptTokenType(token_type) ){
            return true;
        }
        this->error( "Expect: unexpected token" );
        return false;

    }


    Model* Parser::getCurrentModel() const{

        if( this->models.size() == 0 ){
            throw new Exception( "Model list is empty. getCurrentModel failed." );
        }

        return this->current_model->second;

    }


    void Parser::addModel( Model *model ){

        Utf8String model_name = model->getName();
        if( model_name.isEmpty() ){
            throw new Exception( "Cannot add a model with an empty name." );
        }

        this->current_model = this->models.insert( this->models.end(), ModelMapPair(model_name, model) );

    }


    void Parser::printModels(){

        Model *model;
        ModelMap::iterator model_map_iterator;

        for( model_map_iterator = this->models.begin(); model_map_iterator != this->models.end(); model_map_iterator++ ){

            model = model_map_iterator->second;
            cout << *model;

        }

    }


    void Parser::writeModelFiles(){

        Model *model;
        ModelMap::iterator iterator;

        Utf8String *base_model_filename;
        Utf8String *model_filename;
        Utf8String *base_collection_filename;
        Utf8String *collection_filename;
        Utf8String *collection_suffix;

        bool create_derived_model;
        bool create_derived_collection;

        Directory base_directory;
        base_directory.appendToPath( Utf8String("/base") );

        if( !base_directory.exists() ){
            base_directory.create();
        }


        for( iterator = this->models.begin(); iterator != this->models.end(); iterator++ ){

            model = iterator->second;

            fstream base_model_file, model_file, base_collection_file, collection_file;

            collection_suffix = new Utf8String("List");

            base_model_filename = new Utf8String( base_directory.getFullPath() + Utf8String("/") + Utf8String("Base") + model->getName() + Utf8String(".js") );
            model_filename = new Utf8String( model->getName() + Utf8String(".js") );
            base_collection_filename = new Utf8String( base_directory.getFullPath() + Utf8String("/") + Utf8String("Base") + model->getName() + *collection_suffix + Utf8String(".js") );
            collection_filename = new Utf8String( model->getName() + *collection_suffix + Utf8String(".js") );

            if( File::exists(*model_filename) ){
                create_derived_model = false;
            }else{
                create_derived_model = true;
            }

            if( File::exists(*collection_filename) ){
                create_derived_collection = false;
            }else{
                create_derived_collection = true;
            }

            base_model_file.open( base_model_filename->getCString(), fstream::out );
            if( create_derived_model ){
                model_file.open( model_filename->getCString(), fstream::out );
            }
            base_collection_file.open( base_collection_filename->getCString(), fstream::out );
            if( create_derived_collection ){
                collection_file.open( collection_filename->getCString(), fstream::out );
            }

            model->writeBaseModelFile( base_model_file );
            if( create_derived_model ){
                model->writeModelFile( model_file );
            }

            model->writeBaseListFile( base_collection_file );
            if( create_derived_collection ){
                model->writeListFile( collection_file );
            }

            base_model_file.close();
            if( create_derived_model ){
                model_file.close();
            }
            base_collection_file.close();
            if( create_derived_collection ){
                collection_file.close();
            }

            delete base_model_filename;
            delete model_filename;
            delete base_collection_filename;
            delete collection_filename;
            delete collection_suffix;

        }

    }


    void Parser::parseTokens(){

        this->model();

        while( this->acceptTokenType(SEMI_COLON) ){
            this->model();
        }

    }



    void Parser::model(){

        if( this->acceptTokenType(NAME) ){

            //cout << "Model: " << this->getPreviousToken()->contents << endl;
            this->addModel( new Model(this->getPreviousToken()->contents) );

            this->expectTokenType( LEFT_BRACE );
            this->modelProperty();
            while( this->acceptTokenType(SEMI_COLON) ){
                this->modelProperty();
            }
            this->expectTokenType( RIGHT_BRACE );

        }

    }



    void Parser::modelProperty(){

        if( this->acceptTokenType(NAME) ){

            ModelField *field = new ModelField();

            field->setType( this->getPreviousToken()->contents );

            this->expectTokenType(NAME);
            field->setName( this->getPreviousToken()->contents );

            this->getCurrentModel()->addField( field );

        }

    }




}
}
