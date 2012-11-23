

#include "Model.h"



namespace jet{
namespace modeler{

    using ::jet::Exception;
    using namespace std;

    Model::Model( Utf8String name )
        :name(name)
    {


    }

    Model::~Model(){


    }

    void Model::setName( Utf8String name ){

        this->name = name;

    }

    Utf8String Model::getName() const{

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



    ModelField* Model::getFieldByName( Utf8String const &field_name ) const{

        ModelFieldMap::const_iterator it = this->fields.find( field_name );

        if( it == this->fields.end() ){
            return NULL;
        }

        return it->second;

    }


    std::ostream& operator<<( std::ostream &output_stream, const Model &output_model ){

        ModelFieldMap::const_iterator it;
        ModelField *model_field;


        //Header file

            output_stream <<
            "#ifndef INCLUDE_GUARD_" << output_model.name << endl <<
            "#define INCLUDE_GUARD_" << output_model.name << endl <<
            endl <<
            "#include \"jet/Utf8String.h\"" << endl <<
            "#include \"jet/Exception.h\"" << endl <<
            endl <<
            endl <<
            "namespace jet{" << endl <<
            endl <<
            endl <<
            "    class " << output_model.name << "{" << endl <<
            endl <<
            "        public:" << endl <<
            "            " << output_model.name << "();" << endl <<
            "            ~" << output_model.name << "();" << endl << endl;


            for( it = output_model.fields.begin(); it != output_model.fields.end(); it++ ){

                model_field = it->second;

                output_stream << "            void set" << model_field->getName().toCamelCase() << "( " << model_field->getType() << " " << model_field->getName() << " );" << endl;
                output_stream << "            " << model_field->getType() << " get" << model_field->getName().toCamelCase() << "();" << endl;
                output_stream << endl;

            }


            output_stream << endl <<
            "        protected:"  << endl ;

                    for( it = output_model.fields.begin(); it != output_model.fields.end(); it++ ){

                        model_field = it->second;

                        output_stream << "            " << *model_field << endl;

                    }

            output_stream <<
            endl <<
            "    };" << endl <<
            endl <<
            endl <<
            "}" << endl <<
            "#endif //INCLUDE_GUARD_" << output_model.name << endl;





        //Implementation File

            output_stream << ""
            "#include \"" << output_model.name << ".h\"" << endl <<
            endl <<
            endl <<
            "namespace jet{" << endl <<
            endl <<
            endl;

            for( it = output_model.fields.begin(); it != output_model.fields.end(); it++ ){

                model_field = it->second;

                output_stream << "    void " << output_model.name << "::set" << model_field->getName().toCamelCase() << "( " << model_field->getType() << " " << model_field->getName() << " ){" << endl;
                output_stream << endl;
                output_stream << "        this->" << model_field->getName() << " = " << model_field->getName() << ";" << endl;
                output_stream << endl;
                output_stream << "    };" << endl;

                output_stream << endl;
                output_stream << endl;

                output_stream << "    " << model_field->getType() << " " << output_model.name << "::get" << model_field->getName().toCamelCase() << "(){" << endl;
                output_stream << endl;
                output_stream << "        return this->" << model_field->getName() << ";" << endl;
                output_stream << endl;
                output_stream << "    };" << endl;

                output_stream << endl;
                output_stream << endl;

            }

            output_stream <<
            "}" << endl <<
            endl;


        return output_stream;

    }




}
}
