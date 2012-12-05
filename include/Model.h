#ifndef ALTUMO_JET_MODELER_MODEL
#define ALTUMO_JET_MODELER_MODEL

#include <map>
#include <iostream>

#include "jet/Utf8String.h"
#include "jet/Exception.h"

#include "ModelField.h"


namespace jet{
namespace modeler{

    using namespace ::jet::modeler;

    using ::jet::Utf8String;
    using ::jet::Utf8StringComparator;


    class Model{

        public:
            Model( Utf8String name );
            ~Model();

            void setName( Utf8String name );
            Utf8String getName() const;


            void addField( ModelField *model_field );
            ModelField* getFieldByName( Utf8String const &field_name ) const;

            std::ostream& writeHeaderFile( std::ostream &output_stream ) const;
            std::ostream& writeImplementationFile( std::ostream &output_stream ) const;
            std::ostream& writeTestFile( std::ostream &output_stream ) const;

            friend std::ostream& operator<<( std::ostream &output_stream, const Model &output_model );

        protected:
            Utf8String name;
            ModelFieldMap fields;

    };


    typedef ::std::map<Utf8String,Model*,Utf8StringComparator> ModelMap;
    typedef ::std::pair<Utf8String,Model*> ModelMapPair;

}
}



#endif //ALTUMO_JET_MODELER_MODEL
