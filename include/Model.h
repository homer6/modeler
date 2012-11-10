#ifndef ALTUMO_JET_MODELER_MODEL
#define ALTUMO_JET_MODELER_MODEL

#include <map>

#include "jet/Utf8String.h"
#include "jet/Exception.h"

#include "ModelField.h"



namespace jet{
namespace modeler{

    using ::jet::Utf8String;
    using ::jet::Utf8StringComparator;
    class ModelField;

    typedef ::std::map<Utf8String,ModelField*,Utf8StringComparator> ModelFieldMap;
    typedef ::std::pair<Utf8String,ModelField*> ModelFieldMapPair;

    class Model{

        public:
            Model();
            ~Model();

            void setName( Utf8String name );
            Utf8String getName();


            void addField( ModelField *model_field );
            ModelField* getFieldByName( Utf8String const &field_name );


        protected:
            Utf8String name;
            ModelFieldMap fields;

    };

}
}



#endif //ALTUMO_JET_MODELER_MODEL
