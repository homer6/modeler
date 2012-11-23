#include "ModelField.h"




namespace jet{
namespace modeler{


    ModelField::ModelField(){


    }

    void ModelField::setName( Utf8String name ){

        this->name = name;

    }

    Utf8String ModelField::getName() const{

        return this->name;

    }

    void ModelField::setType( Utf8String type ){

        this->type = type;

    }

    Utf8String ModelField::getType() const{

        return this->type;

    }


    std::ostream& operator<<( std::ostream &output_stream, const ModelField &output_model_field ){

        output_stream << output_model_field.type << " " << output_model_field.name << ";";

        return output_stream;

    }


}
}
