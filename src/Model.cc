

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


    bool Model::hasNameAttribute() const{

        ModelField *field;

        field = this->getFieldByName( Utf8String("name") );

        if( field != NULL ){

            if( field->getType() == "attribute" ){

                return true;

            }

        }

        return false;

    }




    std::ostream& Model::writeBaseModelFile( std::ostream &output_stream ) const{

        ModelFieldMap::const_iterator it;
        ModelField *model_field;
        bool is_user_type;

        output_stream << endl << endl << endl <<
        endl <<
        "from lxml.etree import XML, tostring, parse, Element, XMLParser" << endl <<
        "from fd.model.xml_model import XmlModel" << endl;

        output_stream << endl <<
        "class Base" << this->name << "( XmlModel ):" << endl <<
        endl <<
        endl <<
        "    def __init__( self, xml_element = None, filename = None, xml_string = None ):" << endl <<
        endl <<
        "        XmlModel.__init__( self, xml_element = xml_element, filename = filename, xml_string = xml_string )" << endl <<
        endl <<
        "        if self.element is None:" <<
        endl <<
        "            self.element = self.create_default_element()" << endl <<
        endl <<
        endl <<
        endl <<
        "    def create_default_element( self ):" << endl <<
        endl <<
        "        element = Element( '" << this->name.toLowerCase() << "' )" << endl <<
        "        return element" << endl <<
        endl <<
        endl <<
        endl <<
        "    def as_dict( self ):" << endl <<
        "        \"\"\"" << endl <<
        "        Returns the lxml element converted to a json dict." << endl <<
        "        Requires \"pip install xmltodict\"" << endl << endl <<
        "        See: https://github.com/martinblech/xmltodict" << endl <<
        "        :return: dict" << endl <<
        "        \"\"\"" << endl << endl <<
        "        import xmltodict" << endl << endl <<
        "        return xmltodict.parse( self.as_string() )" << endl <<
        endl <<
        endl <<
        endl;


        // the getter, setter and deleter for each of the attributes

            for( it = this->fields.begin(); it != this->fields.end(); it++ ){

                model_field = it->second;

                //cout << this->name << ":" << model_field->getName() << endl;

                if( model_field->getType() != "attribute" ){
                    continue;
                }

                output_stream <<

                "    @property" << endl <<
                "    def " << model_field->getName() << "( self ):" << endl <<
                "        \"\"\"" << endl <<
                "        Gets the " << model_field->getName() << " attribute on this " << this->name << ".  Returns None if the attribute doesn't exist." << endl <<
                "        @return {string}" << endl <<
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
                "        @param {string} " << model_field->getName() << endl <<
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


                //other methods for models with the "name" attribute
                if( model_field->getName() != "name" ){
                    continue;
                }

                output_stream <<

                "    def __hash__( self ):" << endl <<
                "        \"\"\"" << endl <<
                "        Determines a hash integer based on the name field so that this model can be placed in a set." << endl <<
                "        @return {integer}" << endl <<
                "        \"\"\"" << endl <<
                endl <<
                "        return hash( self.name )" << endl <<
                endl <<
                endl <<
                endl;

            }


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
                "        @return {basestring}" << endl <<
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
                "        @param {basestring} " << model_field->getName() << endl <<
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


        // the getter, setter and deleter for each of the fields



            for( it = this->fields.begin(); it != this->fields.end(); it++ ){

                model_field = it->second;

                if( model_field->getType() == "attribute" || model_field->getType() == "text_body" ){
                    continue;
                }

                is_user_type = model_field->isUserType();

                output_stream <<

                "    @property" << endl <<
                "    def " << model_field->getName() << "( self ):" << endl <<
                "        \"\"\"" << endl <<
                "        Gets the " << model_field->getName() << " subelement on this " << this->name << ".  Returns None if the subelement doesn't exist." << endl <<
                "        @return {" << model_field->getType() << "}" << endl <<
                "        \"\"\"" << endl <<
                endl;

                is_user_type = model_field->isUserType();

                if( is_user_type ){

                    output_stream << "        from fd.model." << model_field->getType() << " import " << model_field->getType() << endl;
                    output_stream << "        return " << model_field->getType() << "( xml_element = self.get_or_create_tag_by_name('" << model_field->getName() << "') )" << endl;

                }else{

                    output_stream << "        return self.get_tag_value( '" << model_field->getName() << "' )" << endl;

                }

                output_stream <<
                endl <<
                endl <<
                endl <<






                "    @" << model_field->getName() << ".setter" << endl <<
                "    def " << model_field->getName() << "( self, " << model_field->getName() << " ):" << endl <<
                "        \"\"\"" << endl <<
                "        Sets the " << model_field->getName() << " subelement on this " << this->name << "." << endl <<
                "        @param {" << model_field->getType() << "} " << model_field->getName() << endl <<
                "        \"\"\"" << endl <<
                endl;

                if( is_user_type ){

                    output_stream << "        from fd.model." << model_field->getType() << " import " << model_field->getType() << endl;
                    output_stream << "        tag = self.get_tag_by_name( '" << model_field->getName() << "' )" << endl;

                    output_stream << "        if isinstance( " << model_field->getName() << ", XmlModel ):" << endl << endl;
                    output_stream << "            if tag is None: " << endl;
                    output_stream << "                self.element.append( " << model_field->getName() << ".element )" << endl;
                    output_stream << "            else: " << endl;
                    output_stream << "                tag = " << model_field->getName() << ".element" << endl << endl;

                    output_stream << "        elif isinstance( " << model_field->getName() << ", basestring ):" << endl << endl;
                    output_stream << "            xml_model = " << model_field->getType() << "( xml_string = " << model_field->getName() << " )" << endl;
                    output_stream << "            if tag is None: " << endl;
                    output_stream << "                self.element.append( xml_model.element )" << endl;
                    output_stream << "            else: " << endl;
                    output_stream << "                tag = xml_model.element" << endl << endl;

                    output_stream << "        else:" << endl << endl;
                    output_stream << "            raise Exception( 'Unexpected type for " << this->name << "." << model_field->getName() << "' )" << endl;

                }else{

                    output_stream << "        self.set_tag_value( '" << model_field->getName() << "', " << model_field->getName() << " )" << endl;

                }

                output_stream <<
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

        output_stream << endl << endl << endl <<
        endl <<
        "from base.Base" << this->name << " import Base" << this->name << endl <<
        endl <<
        "class " << this->name << "( Base" << this->name << " ):" << endl <<
        endl <<
        "    pass" << endl;

        return output_stream;

    }


    std::ostream& Model::writeBaseSetFile( std::ostream &output_stream ) const{

        ModelFieldMap::const_iterator it;
        ModelField *model_field;
        bool is_user_type;

        output_stream <<
        endl <<
        "import collections" << endl <<
        endl <<
        "from lxml.etree import XML, tostring, parse, Element, XMLParser" << endl <<
        "from fd.model.xml_model import XmlModel" << endl <<
        endl <<
        endl <<
        "class Base" << this->name << "Set( collections.MutableSet, XmlModel ):" << endl <<
        endl <<
        endl <<
        "    def __init__( self, xml_element = None, filename = None, xml_string = None, initvalue = () ):" << endl <<
        endl <<
        "        XmlModel.__init__( self, xml_element = xml_element, filename = filename, xml_string = xml_string )" << endl <<
        endl <<
        "        self.object_set = set()" << endl <<
        "        self.name_set = set()" << endl <<
        endl <<
        "        if self.element is None:" << endl <<
        "            self.element = self.create_default_element()" << endl <<
        "            for value in initvalue:" << endl <<
        "                self.add( value )" << endl <<
        endl <<
        "        else:" << endl <<
        "            self.reindex()" << endl <<
        endl <<
        endl <<
        endl <<
        "    def reindex( self ):" << endl <<
        endl <<
        "        from fd.model." << this->name << " import " << this->name << endl <<
        endl <<
        "        self.object_set = set()" << endl <<
        "        for item_element in self.element.iterchildren():" << endl <<
        "            item = " << this->name << "( xml_element = item_element )" << endl <<
        "            self.object_set.add( item )" << endl <<
        "            self.name_set.add( item.name )" << endl <<
        "            if hasattr( item, 'reindex' ):" << endl <<
        "                item.reindex()" << endl <<
        endl <<
        endl <<
        endl <<
        "    def create_default_element( self ):" << endl <<
        endl <<
        "        element = Element( '" << this->name.toLowerCase() << "s' )" << endl <<
        "        return element" << endl <<
        endl <<
        endl <<
        endl <<
        "    def add( self, item ):" << endl <<
        endl <<
        "        from fd.model." << this->name << " import " << this->name << endl <<
        endl <<
        "        if not isinstance( item, " << this->name << " ):" << endl <<
        "            raise Exception( '" << this->name << "Set.add expects a " << this->name << ".' )" << endl <<
        endl <<
        "        item_name = item.name" << endl <<
        "        if item_name is None:" << endl <<
        "            raise Exception( '" << this->name << " must have a name.' )" << endl <<
        endl <<
        "        if item_name in self.name_set:" << endl <<
        "            existing_element = self.get( item_name )" << endl <<
        "            self.remove( existing_element )" << endl <<
        endl <<
        "        self.object_set.add( item )" << endl <<
        "        self.name_set.add( item_name )" << endl <<
        "        self.element.append( item.element )" << endl <<
        endl <<
        endl <<
        endl <<
        "    def get( self, item_name ):" << endl <<
        endl <<
        "        if not isinstance( item_name, basestring ):" << endl <<
        "            raise Exception( '" << this->name << "Set.get expects a basestring.' )" << endl <<
        endl <<
        "        if item_name in self.name_set:" << endl <<
        "            for item in self.object_set:" << endl <<
        "                if item.name == item_name:" << endl <<
        "                    return item" << endl <<
        endl <<
        "        return None" << endl <<
        endl <<
        endl <<
        endl <<
        "    def discard( self, item ):" << endl <<
        endl <<
        "        from fd.model." << this->name << " import " << this->name << endl <<
        endl <<
        "        if not isinstance( item, " << this->name << " ):" << endl <<
        "            raise Exception( '" << this->name << "Set.discard expects a " << this->name << ".' )" << endl <<
        endl <<
        "        item_name = item.name" << endl <<
        "        if item_name is None:" << endl <<
        "            raise Exception( '" << this->name << " must have a name.' )" << endl <<
        endl <<
        "        if item_name in self.name_set:" << endl <<
        "            self.element.remove( item.element )" << endl <<
        "            self.object_set.discard( item )" << endl <<
        "            self.name_set.discard( item_name )" << endl <<
        endl <<
        endl <<
        endl <<
        "    @property" << endl <<
        "    def names( self ):" << endl <<
        endl <<
        "        temp_list = list( self.name_set )" << endl <<
        "        temp_list.sort()" << endl <<
        "        return temp_list" << endl <<
        endl <<
        endl <<
        endl <<
        "    def __iter__( self ):" << endl <<
        endl <<
        "        return iter( self.object_set )" << endl <<
        endl <<
        endl <<
        endl <<
        "    def __len__( self ):" << endl <<
        endl <<
        "        return len( self.object_set )" << endl <<
        endl <<
        endl <<
        endl <<
        "    def __contains__( self, item ):" << endl <<
        endl <<
        "        try:" << endl <<
        endl <<
        "            if isinstance( item, basestring ):" << endl <<
        "                return item in self.name_set" << endl <<
        endl <<
        "            from fd.model." << this->name << " import " << this->name << endl <<
        endl <<
        "            if not isinstance( item, " << this->name << " ):" << endl <<
        "                raise Exception( '" << this->name << "Set.__contains__ expects a " << this->name << ".' )" << endl <<
        endl <<
        "            item_name = item.name" << endl <<
        "            if item_name is None:" << endl <<
        "                raise Exception( '" << this->name << " must have a name.' )" << endl <<
        endl <<
        "            return item_name in self.name_set" << endl <<
        endl <<
        "        except AttributeError:" << endl <<
        endl <<
        "            return False" << endl <<
        endl <<
        endl <<
        endl <<
        "    def as_dict( self ):" << endl <<
        "        \"\"\"" << endl <<
        "        Returns the lxml element converted to a json dict (a list of dicts)." << endl <<
        "        Requires \"pip install xmltodict\"" << endl << endl <<
        "        See: https://github.com/martinblech/xmltodict" << endl <<
        "        :return: list" << endl <<
        "        \"\"\"" << endl << endl <<
        "        import xmltodict" << endl << endl <<
        "        results = []" << endl <<
        "        for sub_object in self.object_set:" << endl <<
        "            results.append( sub_object.as_dict() )" << endl <<
        endl <<
        "        return results" << endl <<
        endl <<
        endl <<
        endl <<
        "    def as_json_string( self ):" << endl <<
        "        \"\"\"" << endl <<
        "        Returns the lxml element converted to a json string." << endl <<
        "        Requires \"pip install xmltodict\"" << endl << endl <<
        "        See: https://github.com/martinblech/xmltodict" << endl <<
        "        :return: string" << endl <<
        "        \"\"\"" << endl << endl <<
        "        import json" << endl << endl <<
        "        return json.dumps( self.as_dict() )" << endl <<

        endl <<
        endl <<
        endl;



        // all of the find_by methods for each of the fields except "name"

        for( it = this->fields.begin(); it != this->fields.end(); it++ ){

            model_field = it->second;

            if( model_field->getType() == "attribute" && model_field->getName() == "name" ){
                //skip the name field
                continue;
            }

            //find_by methods don't make sense for user types
                is_user_type = model_field->isUserType();
                if( is_user_type ){
                    continue;
                }


            output_stream <<

            "    def find_by_" << model_field->getName() << "( self, " << model_field->getName() << " ):" << endl <<
            "        \"\"\"" << endl <<
            "        Returns a list of " << this->getName() << " models objects from this set.  Returns an empty list if none were found." << endl <<
            "        @return {list}" << endl <<
            "        \"\"\"" << endl <<
            endl <<
            "        found = list()" << endl <<
            "        for item in self.object_set:" << endl <<
            "            if item." << model_field->getName() << " == " << model_field->getName() << ":" << endl <<
            "                found.append( item )" << endl <<
            endl <<
            "        return found" << endl <<
            endl <<
            endl <<
            endl;

        }



        return output_stream;

    }


    std::ostream& Model::writeSetFile( std::ostream &output_stream ) const{

        output_stream << endl << endl << endl <<
        endl <<
        "from base.Base" << this->name << "Set import Base" << this->name << "Set" << endl <<
        endl <<
        "class " << this->name << "Set( Base" << this->name << "Set ):" << endl <<
        endl <<
        "    pass" << endl;

        return output_stream;

    }


    std::ostream& Model::writeBaseListFile( std::ostream &output_stream ) const{

        ModelFieldMap::const_iterator it;
        ModelField *model_field;
        bool is_user_type;

        output_stream <<
        endl <<
        "from lxml.etree import XML, tostring, parse, Element, XMLParser" << endl <<
        "from fd.model.xml_model import XmlModel" << endl <<
        endl <<
        "from fd.model." << this->name << " import " << this->name << endl <<
        endl <<
        endl <<
        "class Base" << this->name << "List( list, XmlModel ):" << endl <<
        endl <<
        endl <<
        "    def __init__( self, xml_element = None, filename = None, xml_string = None, initvalue = () ):" << endl <<
        endl <<
        "        XmlModel.__init__( self, xml_element = xml_element, filename = filename, xml_string = xml_string )" << endl <<
        endl <<
        "        self.object_list = list()" << endl <<
        endl <<
        "        for value in initvalue:" << endl <<
        "            self.append( value )" << endl <<
        endl <<
        "        if self.element is None:" << endl <<
        "            self.element = self.create_default_element()" << endl <<
        "        else:" << endl <<
        "            self.reindex()" << endl <<
        endl <<
        endl <<
        endl <<
        "    def reindex( self ):" << endl <<
        endl <<
        "        from fd.model." << this->name << " import " << this->name << endl <<
        endl <<
        "        self.object_list = list()" << endl <<
        "        for item_element in self.element.iterchildren():" << endl <<
        "            item = " << this->name << "( xml_element = item_element )" << endl <<
        "            self.object_list.append( item )" << endl <<
        "            if hasattr( item, 'reindex' ):" << endl <<
        "                item.reindex()" << endl <<
        endl <<
        endl <<
        endl <<
        "    def create_default_element( self ):" << endl <<
        endl <<
        "        element = Element( '" << this->name.toLowerCase() << "s' )" << endl <<
        "        return element" << endl <<
        endl <<
        endl <<
        endl <<
        "    def append( self, item ):" << endl <<
        endl <<
        "        if not isinstance( item, " << this->name << " ):" << endl <<
        "            raise Exception( '" << this->name << "List.add expects a " << this->name << ".' )" << endl <<
        endl <<
        "        self.object_list.append( item )" << endl <<
        "        self.element.append( item.element )" << endl <<
        endl <<
        endl <<
        endl <<
        "    def remove( self, item_index ):" << endl <<
        endl <<
        "        item_element = self.element.index( item_index )" << endl <<
        "        if item_element:" << endl <<
        "            self.element.remove( item_element )" << endl <<
        "            self.object_list.remove( item_index )" << endl <<
        endl <<
        endl <<
        endl <<
        "    def __iter__( self ):" << endl <<
        endl <<
        "        return iter( self.object_list )" << endl <<
        endl <<
        endl <<
        endl <<
        "    def __getitem__( self, item ):" << endl <<
        endl <<
        "        return self.object_list[ item ]" << endl <<
        endl <<
        endl <<
        endl <<
        "    def __len__( self ):" << endl <<
        endl <<
        "        return len( self.object_list )" << endl <<
        endl <<
        endl <<
        endl <<
        "    def as_dict( self ):" << endl <<
        "        \"\"\"" << endl <<
        "        Returns the lxml element converted to a json dict (a list of dicts)." << endl <<
        "        Requires \"pip install xmltodict\"" << endl << endl <<
        "        See: https://github.com/martinblech/xmltodict" << endl <<
        "        :return: list" << endl <<
        "        \"\"\"" << endl << endl <<
        "        import xmltodict" << endl << endl <<
        "        results = []" << endl <<
        "        for sub_object in self.object_list:" << endl <<
        "            results.append( sub_object.as_dict() )" << endl <<
        endl <<
        "        return results" << endl <<
        endl <<
        endl <<
        endl <<
        "    def as_json_string( self ):" << endl <<
        "        \"\"\"" << endl <<
        "        Returns the lxml element converted to a json string." << endl <<
        "        Requires \"pip install xmltodict\"" << endl << endl <<
        "        See: https://github.com/martinblech/xmltodict" << endl <<
        "        :return: basestring" << endl <<
        "        \"\"\"" << endl << endl <<
        "        import json" << endl << endl <<
        "        return json.dumps( self.as_dict() )" << endl <<
        endl <<
        endl <<
        endl;


        // all of the find_by methods for each of the fields except "name"

        for( it = this->fields.begin(); it != this->fields.end(); it++ ){

            model_field = it->second;

            if( model_field->getType() == "attribute" && model_field->getName() == "name" ){
                //skip the name field
                continue;
            }

            //find_by methods don't make sense for user types
                is_user_type = model_field->isUserType();
                if( is_user_type ){
                    continue;
                }

            output_stream <<

            "    def find_by_" << model_field->getName() << "( self, " << model_field->getName() << " ):" << endl <<
            "        \"\"\"" << endl <<
            "        Returns a list of " << this->getName() << " models objects from this list.  Returns an empty list if none were found." << endl <<
            "        @return {list}" << endl <<
            "        \"\"\"" << endl <<
            endl <<
            "        found = list()" << endl <<
            "        for item in self.object_list:" << endl <<
            "            if item." << model_field->getName() << " == " << model_field->getName() << ":" << endl <<
            "                found.append( item )" << endl <<
            endl <<
            "        return found" << endl <<
            endl <<
            endl <<
            endl;

        }



        return output_stream;

    }


    std::ostream& Model::writeListFile( std::ostream &output_stream ) const{

        output_stream << endl << endl << endl <<
        endl <<
        "from base.Base" << this->name << "List import Base" << this->name << "List" << endl <<
        endl <<
        "class " << this->name << "List( Base" << this->name << "List ):" << endl <<
        endl <<
        "    pass" << endl;

        return output_stream;

    }



    std::ostream& operator<<( std::ostream &output_stream, const Model &output_model ){

        output_stream << output_model.name << "{" << endl;
        ModelFieldMap::const_iterator it;
        ModelField *model_field;

        for( it = output_model.fields.begin(); it != output_model.fields.end(); it++ ){

            model_field = it->second;
            output_stream << "    " << *model_field << endl;

        }

        output_stream << "};" << endl << endl;

        return output_stream;

    }




}
}
