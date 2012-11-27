
from lxml.etree import XML, tostring, parse, Element, XMLParser
from fd.util.xml import xml_element_from_file, xml_element_to_string


class XmlModel( object ):
    """
    XmlModel is a base class for all concrete models (and their containers) that have XML representation.

    XmlModels are entirely, internally represented with a single, in-memory lxml element. So, all accessors manipulate
    and read from the single lxml element. With this design, you can still manipulate many objects at once with the
    traditional lxml iterators, but still relate to the XML element as an object.

    All models "must" expect the self.element member variable to be accessed publicly.

    These models are intended to be in-memory and are only, incidentally, able to move to or from xml files, string or
    other lxml elements. These models are explicitly not part of an ORM (that is, they're not designed primarily to be
    mapped to a particular database; they're designed to represent XML in memory).
    """


    def __init__( self, xml_element = None, filename = None, xml_string = None ):
        """
        Constructor.
        May be constructed by:

            1. Reading from a file.
                OR
            2. Parsing an xml string.
                OR
            3. Enveloping an existing lxml element.
                OR
            4. By constructing the default empty element in the derived class' constructor. See xml.model.user.User.
               This is detected in the derived class constructor by self.element equalling None after this constructor
               has been invoked.

        in that order.
        """

        if filename is not None:

            self.element = xml_element_from_file( filename )

        elif xml_string is not None:

            self.element = XML( xml_string, XMLParser(remove_blank_text=True) )

        elif xml_element is not None:

            self.element = xml_element

        else:

            self.element = None



    def as_string( self ):
        """
        Serializes this xml element and returns it as a string.
        @return string
        """

        return xml_element_to_string( self.element )



    def save_to_file( self, full_filepath ):
        """
        Serializes this xml element and saves it to the provided filename (must be a full pathname).
        """

        with open( full_filepath, 'w+' ) as file:
            file.write( self.as_string() )



    def get_tag_by_name( self, tag_name, parent_element = None ):
        """
        Retrieves a single lxml element, that is a subelement of the main self.element, by tag name. If the tag does
        not exist, it will return None. Please use XmlModel.get_or_create_tag_by_name if you want it to automatically
        create it. If parent_element is not None, it will use that as the reference parent instead of self.element.
        """

        if parent_element is None:
            parent_element = self.element

        tag_name = tag_name.encode('utf-8')
        tag_element = parent_element.find( tag_name )

        if tag_element is None:
            return None

        return tag_element



    def get_or_create_tag_by_name( self, tag_name, parent_element = None ):
        """
        Retrieves a single lxml element, that is a subelement of the main self.element, by tag name. If the tag does
        not exist, it will create it and return it. If parent_element is not None, it will use that as the reference
        parent instead of self.element.
        """

        if parent_element is None:
            parent_element = self.element

        tag_name = tag_name.encode('utf-8')
        tag_element = parent_element.find( tag_name )

        if tag_element is None:
            tag_element = Element( tag_name )
            parent_element.append( tag_element )

        return tag_element



    def set_attribute_value( self, attribute_name, attribute_value ):
        """
        Sets an xml attribute on self.element.
        """

        self.element.attrib[ attribute_name.encode('utf-8') ] = attribute_value.encode( 'utf-8')



    def get_attribute_value( self, attribute_name ):
        """
        Retrieves the value of the given attribute on self.element.
        Returns None if the attribute does not exist.
        """

        try:

            return self.element.attrib[ attribute_name.encode('utf-8') ]

        except Exception, e:

            return None



    def set_tag_value( self, tag_name, tag_value, parent_element = None ):
        """
        Sets the xml text() of a given subelement of the main self.element, by tag name.
        If the tag does not exist, it will create it and set the text for that subelement.
        If parent_element is not None, it will use that as the reference parent instead of self.element.
        """

        if parent_element is None:
            parent_element = self.element

        tag_element = self.get_or_create_tag_by_name( tag_name, parent_element = parent_element )
        tag_element.text = tag_value.encode('utf-8')



    def get_tag_value( self, tag_name, parent_element = None ):
        """
        Retrieves the xml text() of a given subelement of the main self.element, by tag name, as a string.
        If the tag does not exist, it will return None.
        If parent_element is not None, it will use that as the reference parent instead of self.element.
        """

        if parent_element is None:
            parent_element = self.element

        tag_name = tag_name.encode('utf-8')
        tag_element = parent_element.find( tag_name )

        if tag_element is None:
            return None

        return tag_element.text


