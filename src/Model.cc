

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

        output_stream << endl <<
        endl <<
        "Base" << this->name << " = function( data_reference ){" << endl <<
        endl <<
        endl <<
        "    if( !this._class_name ){" << endl <<
        "        this._class_name = 'Base" << this->name << "';" << endl <<
        "    }" << endl <<
        endl <<
        endl <<
        endl <<
        "    XmlModel.apply( this, arguments );" << endl <<
        endl <<
        "    var me = this;" << endl <<
        endl;


        // initialize user types (to maintain a reference to them)

            for( it = this->fields.begin(); it != this->fields.end(); it++ ){

                model_field = it->second;

                if( model_field->getType() == "attribute" || model_field->getType() == "text_body" || !model_field->isUserType() ){
                    continue;
                }

                output_stream <<
                "    var " << model_field->getName() << " = null;" << endl;

            }


        // Add default list of user-typed child nodes so that they can be overridden by the user-editable model

            output_stream << endl << endl <<
            "    me._child_node_names = {" << endl;

            vector<Utf8String> child_node_names;

            for( it = this->fields.begin(); it != this->fields.end(); it++ ){

                model_field = it->second;

                if( model_field->getType() == "attribute" || model_field->getType() == "text_body" || !model_field->isUserType() ){
                    continue;
                }

                child_node_names.push_back( "        " + model_field->getName() + ": '" + model_field->getName() + "'" );

            }

            output_stream << Utf8String(",\n").join( child_node_names ) << endl <<
            "    };" << endl;



        output_stream << endl << endl << endl;

        // call to initialization function
            output_stream <<
            "    me._Base" << this->name << " = function(){" << endl <<
            endl <<
            "        me._XmlModel.apply( this, arguments );" << endl <<
            endl << endl;

            // Hydrate user-typed members

                for( it = this->fields.begin(); it != this->fields.end(); it++ ){

                    model_field = it->second;

                    if( model_field->getType() == "attribute" || model_field->getType() == "text_body" || !model_field->isUserType() ){
                        continue;
                    }

                    output_stream <<
                    "        me.set" << model_field->getName().toCamelCase() << "( new " << model_field->getType() << "(me.getChild(me._child_node_names." << model_field->getName() << ")) );" << endl;

                }

            output_stream << endl <<
            "    };"
            << endl << endl << endl;


        // the getter, setter and deleter for each of the attributes

            for( it = this->fields.begin(); it != this->fields.end(); it++ ){

                model_field = it->second;

                //cout << this->name << ":" << model_field->getName() << endl;

                if( model_field->getType() != "attribute" ){
                    continue;
                }

                output_stream <<

                "    me.get" << model_field->getName().toCamelCase() << " = function(){" << endl <<
                "        return me.getAttributeValue( '" << model_field->getName() << "' );" << endl <<
                "    };" << endl <<
                endl <<
                endl <<
                endl <<


                "    me.set" << model_field->getName().toCamelCase() << " = function( " << model_field->getName() << " ){ " << endl <<
                "        return me.setAttributeValue( '" << model_field->getName() << "', " << model_field->getName() << " );" << endl <<
                "    };" << endl <<
                endl <<
                endl <<
                endl <<


                "    me.remove" << model_field->getName().toCamelCase() << " = function(){" << endl <<
                "        return me.removeAttribute( '" << model_field->getName() << "' );" << endl <<
                "    };" << endl <<
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

                "    me.get" << model_field->getName().toCamelCase() << " = function(){" << endl <<
                "        return me.getTextValue( '" << model_field->getName() << "' );" << endl <<
                "    };" << endl <<
                endl <<
                endl <<
                endl <<


                "    me.set" << model_field->getName().toCamelCase() << " = function( " << model_field->getName() << " ){ " << endl <<
                "        return me.setTextValue( '" << model_field->getName() << "', " << model_field->getName() << " );" << endl <<
                "    };" << endl <<
                endl <<
                endl <<
                endl <<


                "    me.remove" << model_field->getName().toCamelCase() << " = function(){" << endl <<
                "        return me.removeTextValue( '" << model_field->getName() << "' );" << endl <<
                "    };" << endl <<
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
                "    me.get" << model_field->getName().toCamelCase() << " = function(){" << endl;
                if( is_user_type ){
                    output_stream << "        return " << model_field->getName() << ";" << endl;
                }else{
                    output_stream << "        return me.getChildTextValue( '" << model_field->getName() << "' );" << endl;
                }
                output_stream <<
                "    };" << endl <<
                endl <<
                endl <<
                endl;



                output_stream <<
                "    me.set" << model_field->getName().toCamelCase() << " = function( new_" << model_field->getName() << " ){" << endl;
                if( is_user_type ){
                    output_stream << "        " << model_field->getName() << " = new_" << model_field->getName() << ";" << endl;
                }else{
                    output_stream << "        return me.setChildTextValue( '" << model_field->getName() << "' );" << endl;
                }
                output_stream <<
                "    };" << endl <<
                endl <<
                endl <<
                endl;



                output_stream <<
                "    me.remove" << model_field->getName().toCamelCase() << " = function(){" << endl;
                if( is_user_type ){
                    output_stream << "        " << model_field->getName() << " = null;" << endl;
                    output_stream << "        return me.removeChild( '" << model_field->getName() << "' );" << endl;
                }else{
                    output_stream << "        return me.removeChild( '" << model_field->getName() << "' );" << endl;
                }
                output_stream <<
                "    };" << endl <<
                endl <<
                endl <<
                endl;


            }

        output_stream <<
        "    if( me._class_name == 'Base" << this->name << "' ){" << endl <<
        "        me._Base" << this->name << "();" << endl <<
        "    }" << endl <<
        endl <<
        "};" << endl;

        return output_stream;

    }


    std::ostream& Model::writeModelFile( std::ostream &output_stream ) const{

        output_stream << endl <<
        endl <<
        this->name << " = function( data_reference ){" << endl <<
        endl <<
        endl <<
        "    if( !this._class_name ){" << endl <<
        "        this._class_name = '" << this->name << "';" << endl <<
        "    }" << endl <<
        endl <<
        endl <<
        endl <<
        "    Base" << this->name << ".apply( this, arguments );" << endl <<
        endl <<
        "    var me = this;" << endl <<
        endl << endl << endl;


        output_stream <<
        "    me._" << this->name << " = function(){" << endl <<
        endl <<
        "        me._Base" << this->name << ".apply( this, arguments );" << endl <<
        endl <<
        "    };"
        << endl << endl << endl;


        output_stream << endl <<
        "    if( me._class_name == '" << this->name << "' ){" << endl <<
        "        me._" << this->name << "();" << endl <<
        "    }" << endl <<
        endl <<
        "};" << endl;


        return output_stream;

    }


    std::ostream& Model::writeBaseListFile( std::ostream &output_stream ) const{

        ModelFieldMap::const_iterator it;
        ModelField *model_field;
        bool is_user_type;

        output_stream << endl <<
        endl <<
        "Base" << this->name << "List = function( data_reference ){" << endl <<
        endl <<
        endl <<
        endl <<
        "    if( !this._class_name ){" << endl <<
        "        this._class_name = 'Base" << this->name << "List';" << endl <<
        "    }" << endl <<
        endl <<
        endl <<
        endl <<
        "    XmlModel.apply( this, arguments );" << endl <<
        endl <<
        "    var me = this;" << endl <<
        "    var list_members = [];" << endl <<
        endl <<
        endl <<
        endl <<
        "    me._list_node_name = '" << this->name.toUnderscoreCase() << "';" << endl <<
        endl <<
        endl <<
        endl <<
        "    me._Base" << this->name << "List = function(){" << endl <<
        "        me._XmlModel.apply( this, arguments );" << endl <<
        endl <<
        "        var list_children = me.getChildrenByTag( me._list_node_name );" << endl <<
        endl <<
        "        for( var i in list_children ){" << endl <<
        "            list_members.push( new " << this->name << "(list_children[i]) );" << endl <<
        "        }" << endl <<
        "    };" << endl;


        output_stream << endl << endl << endl <<
        "    me.getAll = function(){" << endl <<
        endl <<
        "        return list_members;" << endl <<
        endl <<
        "    };" << endl <<
        endl <<
        endl <<
        endl <<



        "    me.prepend = function( " << this->name.toLowerCase() << " ){" << endl <<
        endl <<
        "        list_members.unshift( " << this->name.toLowerCase() << " );" << endl <<
        "        return me;" << endl <<
        endl <<
        "    };" << endl <<
        endl <<
        endl <<
        endl <<



        "    me.append = function( " << this->name.toLowerCase() << " ){" << endl <<
        endl <<
        "        list_members.push( " << this->name.toLowerCase() << " );" << endl <<
        "        return me;" << endl <<
        endl <<
        "    };" << endl <<
        endl <<
        endl <<
        endl <<



        "    me.getByIndex = function( index ){" << endl <<
        endl <<
        "        if( index >= list_members.length || index < 0 ){" << endl <<
        endl <<
        "            throw \"Out of bounds for getByIndex\";" << endl <<
        endl <<
        "        }" << endl <<
        endl <<
        "        return list_members[ index ];" << endl <<
        endl <<
        "    };" << endl <<
        endl <<
        endl <<
        endl <<



        "    /**" << endl <<
        "     * Calls \"callback\" once for each list member. It passes the member model as the first parameter" << endl <<
        "     * and its index as the second parameter." << endl <<
        "     * @param function callback( member, index )" << endl <<
        "     * @returns void" << endl <<
        "     */" << endl <<
        "    me.forEach = function( callback ){" << endl <<
        endl <<
        "        if( typeof(callback) != 'function' ){" << endl <<
        "            return false;" << endl <<
        "        }" << endl <<
        endl <<
        "        for( var i in list_members ){" << endl <<
        "            callback( list_members[i], i );" << endl <<
        "        }" << endl <<
        endl <<
        "    };" << endl <<
        endl <<
        endl <<
        endl;



        // find_by-like functions for each of the accessors

            for( it = this->fields.begin(); it != this->fields.end(); it++ ){

                model_field = it->second;

                //find_by methods don't make sense for user types
                    is_user_type = model_field->isUserType();
                    if( is_user_type ){
                        continue;
                    }

                output_stream <<


                "    me.findBy" << model_field->getName().toCamelCase() << " = function( " << model_field->getName() << " ){" << endl <<
                endl <<
                "        var results = [];" << endl <<
                "        for( var i in list_children ){" << endl <<
                "            if( list_children[i].get" << model_field->getName().toCamelCase() << "() == " << model_field->getName() << " ){" << endl <<
                "                results.push( list_children[i] );" << endl <<
                "            }" << endl <<
                "        }" << endl <<
                "        return results;" << endl <<
                endl <<
                "    };" << endl <<
                endl <<
                endl <<
                endl <<


                "    me.removeBy" << model_field->getName().toCamelCase() << " = function( " << model_field->getName() << " ){" << endl <<
                endl <<
                "        var new_array = [];" << endl <<
                "        for( var i in list_children ){" << endl <<
                "            if( list_children[i].get" << model_field->getName().toCamelCase() << "() != " << model_field->getName() << " ){" << endl <<
                "                new_array.push( list_children[i] );" << endl <<
                "            }" << endl <<
                "        }" << endl <<
                "        list_children = new_array;" << endl <<
                "        return me;" << endl <<
                endl <<
                "    };" << endl <<
                endl <<
                endl <<
                endl;


            }




        output_stream << endl <<
        "    if( me._class_name == 'Base" << this->name << "List' ){" << endl <<
        "        me._Base" << this->name << "List();" <<  endl <<
        "    }" << endl <<
        endl <<
        "};" << endl;

        return output_stream;

    }


    std::ostream& Model::writeListFile( std::ostream &output_stream ) const{

        output_stream << endl <<
        endl <<
        this->name << "List = function( data_reference ){" << endl <<
        endl <<
        endl <<
        "    if( !this._class_name ){" << endl <<
        "        this._class_name = '" << this->name << "List';" << endl <<
        "    }" << endl <<
        endl <<
        endl <<
        endl <<
        "    Base" << this->name << "List.apply( this, arguments );" << endl <<
        endl <<
        "    var me = this;" << endl <<
        endl <<
        endl <<
        endl <<
        "    me._" << this->name << "List = function(){" << endl <<
        "        me._Base" << this->name << "List.apply( this, arguments );" << endl <<
        "    };" << endl <<
        endl <<
        endl <<
        endl <<
        "    if( me._class_name == '" << this->name << "List' ){" << endl <<
        "        me._" << this->name << "List();" << endl <<
        "    }" << endl <<
        endl <<
        "};" << endl;

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
