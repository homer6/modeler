

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




    std::ostream& Model::writeBaseModelFile( std::ostream &output_stream ) const{

        ModelFieldMap::const_iterator it;
        ModelField *model_field;

        output_stream << endl << endl << endl <<
        endl <<
        "from lxml.etree import XML, tostring, parse, Element, XMLParser" << endl <<
        "from fd.util.xml import xml_element_from_file, xml_element_to_string" << endl <<
        "from fd.model.xml_model import XmlModel" << endl <<
        endl <<
        "class Base" << this->name << "( XmlModel ):" << endl <<
        endl <<
        endl <<
        "    def __init__( self, xml_element = None, filename = None, xml_string = None ):" << endl <<
        endl <<
        "        XmlModel.__init__( self, xml_element = xml_element, filename = filename, xml_string = xml_string )" << endl <<
        endl <<
        "        if self.element is None:" <<
        endl <<
        "            self.element = Element( '" << this->name.toLowerCase() << "' )" << endl;

        for( it = this->fields.begin(); it != this->fields.end(); it++ ){

            model_field = it->second;
            output_stream << "            self." << model_field->getName() << " = ''" << endl;

        }

        output_stream << endl << endl << endl;


        // the getter, setter and deleter for each of the attributes

            for( it = this->fields.begin(); it != this->fields.end(); it++ ){

                model_field = it->second;

                cout << this->name << ":" << model_field->getName() << endl;

                if( model_field->getType() != "attribute" ){
                    continue;
                }

                output_stream <<

                "    @property" << endl <<
                "    def " << model_field->getName() << "( self ):" << endl <<
                "        \"\"\"" << endl <<
                "        Gets the " << model_field->getName() << " attribute on this " << this->name << ".  Returns None if the attribute doesn't exist." << endl <<
                "        @return {" << model_field->getType() << "}" << endl <<
                "        \"\"\"" << endl <<
                endl <<
                "        return self.get_attribute_value( '" << model_field->getName() << "' )" << endl <<
                endl <<
                endl <<
                endl <<


                "    @" << model_field->getName() << ".setter" << endl <<
                "    def " << model_field->getName() << "( self, " << model_field->getName() << " ):" << endl <<
                "        \"\"\"" << endl <<
                "        Sets the " << model_field->getName() << " attribute on this " << this->name << "." << endl <<
                "        @param {" << model_field->getType() << "} " << model_field->getName() << endl <<
                "        \"\"\"" << endl <<
                endl <<
                "        self.set_attribute_value( '" << model_field->getName() << "', " << model_field->getName() << " )" << endl <<
                endl <<
                endl <<
                endl <<


                "    @" << model_field->getName() << ".deleter" << endl <<
                "    def " << model_field->getName() << "( self ):" << endl <<
                "        \"\"\"" << endl <<
                "        Deletes the " << model_field->getName() << " attribute on this " << this->name << "." << endl <<
                "        \"\"\"" << endl <<
                endl <<
                "        self.remove_attribute( '" << model_field->getName() << "' )" << endl <<
                endl <<
                endl <<
                endl;

            }

            it = this->fields.begin();


        // the getter, setter and deleter for each of the text_body (there should only be one though (with a name of "text"), if any)

            for( it = this->fields.begin(); it != this->fields.end(); it++ ){

                model_field = it->second;

                if( model_field->getType() != "text_body" ){
                    continue;
                }

                //model_field->setName( Utf8String("text") );

                output_stream <<

                "    @property" << endl <<
                "    def " << model_field->getName() << "( self ):" << endl <<
                "        \"\"\"" << endl <<
                "        Gets the " << model_field->getName() << " text body on this " << this->name << ".  Returns None if the text body doesn't exist." << endl <<
                "        @return {" << model_field->getType() << "}" << endl <<
                "        \"\"\"" << endl <<
                endl <<
                "        return self.element.text" << endl <<
                endl <<
                endl <<
                endl <<


                "    @" << model_field->getName() << ".setter" << endl <<
                "    def " << model_field->getName() << "( self, " << model_field->getName() << " ):" << endl <<
                "        \"\"\"" << endl <<
                "        Sets the " << model_field->getName() << " text body on this " << this->name << "." << endl <<
                "        @param {" << model_field->getType() << "} " << model_field->getName() << endl <<
                "        \"\"\"" << endl <<
                endl <<
                "        self.element.text = " << model_field->getName() << endl <<
                endl <<
                endl <<
                endl <<


                "    @" << model_field->getName() << ".deleter" << endl <<
                "    def " << model_field->getName() << "( self ):" << endl <<
                "        \"\"\"" << endl <<
                "        Deletes the " << model_field->getName() << " text body on this " << this->name << "." << endl <<
                "        \"\"\"" << endl <<
                endl <<
                "        del self.element.text" << endl <<
                endl <<
                endl <<
                endl;

            }

            it = this->fields.begin();

        // the getter, setter and deleter for each of the fields

            for( it = this->fields.begin(); it != this->fields.end(); it++ ){

                model_field = it->second;

                if( model_field->getType() == "attribute" || model_field->getType() == "text_body" ){
                    continue;
                }

                output_stream <<

                "    @property" << endl <<
                "    def " << model_field->getName() << "( self ):" << endl <<
                "        \"\"\"" << endl <<
                "        Gets the " << model_field->getName() << " subelement on this " << this->name << ".  Returns None if the subelement doesn't exist." << endl <<
                "        @return {" << model_field->getType() << "}" << endl <<
                "        \"\"\"" << endl <<
                endl <<
                "        return self.get_tag_value( '" << model_field->getName() << "' )" << endl <<
                endl <<
                endl <<
                endl <<


                "    @" << model_field->getName() << ".setter" << endl <<
                "    def " << model_field->getName() << "( self, " << model_field->getName() << " ):" << endl <<
                "        \"\"\"" << endl <<
                "        Sets the " << model_field->getName() << " subelement on this " << this->name << "." << endl <<
                "        @param {" << model_field->getType() << "} " << model_field->getName() << endl <<
                "        \"\"\"" << endl <<
                endl <<
                "        self.set_tag_value( '" << model_field->getName() << "', " << model_field->getName() << " )" << endl <<
                endl <<
                endl <<
                endl <<


                "    @" << model_field->getName() << ".deleter" << endl <<
                "    def " << model_field->getName() << "( self ):" << endl <<
                "        \"\"\"" << endl <<
                "        Deletes the " << model_field->getName() << " subelement on this " << this->name << "." << endl <<
                "        \"\"\"" << endl <<
                endl <<
                "        self.remove_tag( '" << model_field->getName() << "' )" << endl <<
                endl <<
                endl <<
                endl;

            }


        return output_stream;

    }


    std::ostream& Model::writeModelFile( std::ostream &output_stream ) const{

        //ModelFieldMap::const_iterator it;
        //ModelField *model_field;

        output_stream << endl << endl << endl <<
        endl <<
        "from Base" << this->name << " import Base" << this->name << endl <<
        endl <<
        "class " << this->name << "( Base" << this->name << " ):" << endl <<
        endl <<
        "    pass" << endl;

        return output_stream;

    }


    std::ostream& Model::writeBaseCollectionFile( std::ostream &output_stream ) const{

        //ModelFieldMap::const_iterator it;
        //ModelField *model_field;


        output_stream << "BaseCollectionFile contents" << endl;

        return output_stream;

    }


    std::ostream& Model::writeCollectionFile( std::ostream &output_stream ) const{

        //ModelFieldMap::const_iterator it;
        //ModelField *model_field;


        output_stream << "CollectionFile contents" << endl;

        return output_stream;

    }



    std::ostream& operator<<( std::ostream &output_stream, const Model &output_model ){

        /*
        output_stream << output_model.name << "{" << endl;
        ModelFieldMap::const_iterator it;
        ModelField *model_field;

        for( it = output_model.fields.begin(); it != output_model.fields.end(); it++ ){

            model_field = it->second;
            output_stream << "    " << *model_field << endl;

        }

        output_stream << "};" << endl << endl;

        return output_stream;

        */
        cout << "hello" << endl;



        return output_stream;

    }




}
}
