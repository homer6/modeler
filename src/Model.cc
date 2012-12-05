

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


    std::ostream& Model::writeHeaderFile( std::ostream &output_stream ) const{

        ModelFieldMap::const_iterator it;
        ModelField *model_field;

        int x;

        //Header file

            output_stream <<
            "#ifndef INCLUDE_GUARD_" << this->name << endl <<
            "#define INCLUDE_GUARD_" << this->name << endl <<
            endl <<
            "#include \"jet/Utf8String.h\"" << endl <<
            "#include \"jet/Exception.h\"" << endl <<
            endl <<
            endl <<
            "namespace jet{" << endl <<
            endl <<
            endl <<
            "    class " << this->name << "{" << endl <<
            endl <<
            "        public:" << endl <<
            "            " << this->name << "();" << endl <<
            "            " << this->name << "( ";

            x = 0;
            for( it = this->fields.begin(); it != this->fields.end(); it++ ){
                if( x > 0 ){
                    output_stream << ", ";
                }
                model_field = it->second;
                output_stream << model_field->getType() << " " << model_field->getName();
                x++;
            }

            output_stream << " );" << endl <<
            "            ~" << this->name << "();" << endl << endl;


            for( it = this->fields.begin(); it != this->fields.end(); it++ ){

                model_field = it->second;

                output_stream << "            void set" << model_field->getName().toCamelCase() << "( " << model_field->getType() << " " << model_field->getName() << " );" << endl;
                output_stream << "            " << model_field->getType() << " get" << model_field->getName().toCamelCase() << "();" << endl;
                output_stream << endl;

            }


            output_stream << endl <<
            "        protected:"  << endl ;

                    for( it = this->fields.begin(); it != this->fields.end(); it++ ){

                        model_field = it->second;

                        output_stream << "            " << *model_field << endl;

                    }

            output_stream <<
            endl <<
            "    };" << endl <<
            endl <<
            endl <<
            "}" << endl <<
            "#endif //INCLUDE_GUARD_" << this->name << endl;

        return output_stream;

    }


    std::ostream& Model::writeImplementationFile( std::ostream &output_stream ) const{

        ModelFieldMap::const_iterator it;
        ModelField *model_field;

        int x = 0;


        //Implementation File

            output_stream << ""
            "#include \"jet/" << this->name << ".h\"" << endl <<
            endl <<
            endl <<
            "namespace jet{" << endl <<
            endl <<
            endl;


            output_stream <<
            endl << "    " << this->name << "::" << this->name << "()" << endl <<
            "        :";

            for( it = this->fields.begin(); it != this->fields.end(); it++ ){
                if( x > 0 ){
                    output_stream << ", ";
                }
                model_field = it->second;
                output_stream << model_field->getName() << "(" << model_field->getName() << ")";
                x++;
            }

            output_stream << endl <<
            "    {" << endl <<
            endl <<
            endl << "    }" <<
            endl <<
            endl <<
            endl << "    " << this->name << "::" << this->name << "( ";

            x = 0;
            for( it = this->fields.begin(); it != this->fields.end(); it++ ){
                if( x > 0 ){
                    output_stream << ", ";
                }
                model_field = it->second;
                output_stream << model_field->getType() << " " << model_field->getName();
                x++;
            }

            output_stream <<
            " )" <<
            endl << "        :";

            x = 0;
            for( it = this->fields.begin(); it != this->fields.end(); it++ ){
                if( x > 0 ){
                    output_stream << ", ";
                }
                model_field = it->second;
                output_stream << model_field->getName() << "(" << model_field->getName() << ")";
                x++;
            }

            output_stream <<
            endl << "    {" <<
            endl <<
            endl <<
            endl << "    }" <<
            endl <<
            endl <<
            endl;



            output_stream <<
            endl << "    " << this->name << "::~" << this->name << "(){" << endl <<
            endl <<
            endl << "    }" <<
            endl <<
            endl <<
            endl;


            for( it = this->fields.begin(); it != this->fields.end(); it++ ){

                model_field = it->second;

                output_stream << "    void " << this->name << "::set" << model_field->getName().toCamelCase() << "( " << model_field->getType() << " " << model_field->getName() << " ){" << endl;
                output_stream << endl;
                output_stream << "        this->" << model_field->getName() << " = " << model_field->getName() << ";" << endl;
                output_stream << endl;
                output_stream << "    }" << endl;

                output_stream << endl;
                output_stream << endl;

                output_stream << "    " << model_field->getType() << " " << this->name << "::get" << model_field->getName().toCamelCase() << "(){" << endl;
                output_stream << endl;
                output_stream << "        return this->" << model_field->getName() << ";" << endl;
                output_stream << endl;
                output_stream << "    }" << endl;

                output_stream << endl;
                output_stream << endl;

            }

            output_stream <<
            "}" << endl <<
            endl;

        return output_stream;

    }


    std::ostream& Model::writeTestFile( std::ostream &output_stream ) const{

        ModelFieldMap::const_iterator it;
        ModelField *model_field;

        Utf8String variable_name( this->name.toLowerCase() );


            output_stream << ""
            "#include \"jet/" << this->name << ".h\"" << endl <<
            endl <<
            endl <<
            "using namespace jet;" << endl <<
            endl <<
            "class " << this->name << "Test{" << endl <<
            endl <<
            endl <<
            "    public:" << endl <<
            "        void run(){" << endl <<
            endl <<
            "            " << this->name << " " << variable_name << ";" << endl <<
            endl;

            for( it = this->fields.begin(); it != this->fields.end(); it++ ){

                model_field = it->second;

                output_stream << "            " << variable_name << ".set" << model_field->getName().toCamelCase() << "( " << model_field->getName() << " );" << endl;
                output_stream << "            " << variable_name << ".get" << model_field->getName().toCamelCase() << "();" << endl;
                output_stream << endl;
                output_stream << endl;

            }

            output_stream <<
            "        }" << endl <<
            "};" << endl <<
            endl;

        return output_stream;

    }



    std::ostream& operator<<( std::ostream &output_stream, const Model &output_model ){

        output_stream << output_model.name << endl;

        return output_stream;

    }




}
}
