

#include "Model.h"



namespace jet{
namespace modeler{

    using ::jet::Exception;

    Model::Model(){


    }

    Model::~Model(){


    }

    void Model::setName( Utf8String name ){

        this->name = name;

    }

    Utf8String Model::getName(){

        return this->name;

    }


    void Model::addField( ModelField *model_field ){

        ModelFieldMap::iterator it;
        Utf8String field_name = model_field->getName();
        if( field_name.isEmpty() ){
            throw new Exception( "Cannot add a field with an empty field name." );
        }


        it = this->fields.end();
        this->fields.insert( it, ModelFieldMapPair( field_name, model_field ) );

    }



    ModelField* Model::getFieldByName( Utf8String const &field_name ){

        ModelFieldMap::iterator it;

        it = this->fields.find( field_name );

        if( it == this->fields.end() ){
            return NULL;
        }

        return it->second;

    }


}
}
