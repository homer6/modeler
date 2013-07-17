XmlModel = function( data_reference ){

    var me = this;

    var init = function(){
    };


    /**
     * Returns a (reference) object to the attribute with a name matching attribute_name
     *
     * @param string attribute_name
     * @returns Object | null
     */
    me.getAttribute = function( attribute_name ){

        if( data_reference._attributes ){
            for( i in data_reference._attributes ){

                var attribute = data_reference._attributes[i];

                if( attribute._name && attribute._name == attribute_name ) {
                    return attribute;
                }

            }
        }

        return null;

    };


    /**
     * Returns a the value of an attribute with a name matching attribute_name
     *
     * @param string attribute_name
     * @returns string | null
     */
    me.getAttributeValue = function( attribute_name ){

        var attribute = me.getAttribute( attribute_name );

        if( attribute && attribute._value ){
            return attribute._value;
        }

        return false;

    };


    /**
     * Sets the value of an attribute. If the attribute doesn't exist, it will be created.
     *
     * @param string attribute_name
     * @param string attribute_value
     * @returns self
     */
    me.setAttributeValue = function( attribute_name, attribute_value ){

        var attribute = me.getAttribute( attribute_name );

        // create attribute if it doesn't exist
        if( !attribute ){

            data_reference._attributes.push({
                _name: attribute_name,
                _value: attribute_value
            });

        } else {

            attribute._value = attribute_value;

        }

        return me;

    };


    /**
     * Removes an attribute by name.
     *
     * @param string attribute_name
     * @returns self
     */
    me.removeAttribute = function( attribute_name ){

        if( data_reference._attributes ){
            for( i in data_reference._attributes ){

                var attribute = data_reference._attributes[i];

                if( attribute._name && attribute._name == attribute_name ) {

                    data_reference._attributes.splice( i, 1);

                }

            }
        }

        return me;

    };


    /**
     * Returns a (reference) object to a child by tag.
     * If more than one child with the same tag exists, the first one is returned.
     *
     * @param string tag
     * @returns Object | null
     */
    me.getChild = function( tag ){

        if( data_reference._children ){
            for( i in data_reference._children ){

                var child = data_reference._children[i];

                if( child._tag && child._tag == tag ) {
                    return child;
                }

            }
        }

        return null;

    };


    /**
     * Returns the value of a text child or null if it doesn't exist or if it is not a text child.
     *
     * @param string tag
     * @returns string | null
     */
    me.getChildTextValue = function( tag ){

        var child = me.getChild( tag );

        if( child && child._text ){
            return child._text;
        }

        return null;

    };


    /**
     * Sets the value of a text child. If it doesn't exist, it will be created.
     *
     * @param string child_tag
     * @param string child_text_value
     * @returns self
     */
    me.setChildTextValue = function( child_tag, child_text_value ){

        var child = me.getChild( child_tag );

        // create child if it doesn't exist
        if( !child ){

            data_reference._children.push({
                _tag: child_tag,
                _attributes: [],
                _text: child_text_value
            });

        } else {

            child._text = child_text_value;

        }

        return me;

    };


    /**
     * Removes a child by tag.
     *
     * @param string tag
     * @returns self
     */
    me.removeChild = function( tag ){

        if( data_reference._children ){
            for( i in data_reference._children ){

                var child = data_reference._children[i];

                if( child._tag && child._tag == tag ) {

                    data_reference._children.splice( i, 1);

                }

            }
        }

        return me;

    };


    /**
     * Get children by tag
     *
     * @param string tag
     * @returns self
     */
    me.getChildrenByTag = function( tag ){

        var children = [];

        if( data_reference._children ){
            for( i in data_reference._children ){

                var child = data_reference._children[i];

                if( child._tag && child._tag == tag ) {

                    children.push( data_reference._children[i] );

                }

            }
        }

        return children;

    };


    /**
     * Get the text value of this object.
     *
     * @returns string
     */
    me.getTextValue = function(){

        if( data_reference._text && typeof(data_reference._text) ){
            return data_reference._text;
        }

        return null;

    };


    /**
     * Set the text value of this object.
     *
     * @param string text_value
     * @returns string
     */
    me.setTextValue = function( text_value ){

        data_reference._text = text_value;

        return me;

    };

    /**
     * Remove the text value of this object.
     *
     * @returns string
     */
    me.removeTextValue = function(){

        data_reference._text = null;

        return me;

    };


    init();

};