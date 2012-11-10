#ifndef ALTUMO_JET_MODELER_MY_MODEL_FIELD
#define ALTUMO_JET_MODELER_MY_MODEL_FIELD

#include <map>
#include <iostream>

#include "jet/Utf8String.h"


namespace jet{
namespace modeler{

    using ::jet::Utf8String;

    using namespace ::jet;
    using namespace ::jet::modeler;



    class ModelField{

        public:
            ModelField();

            void setName( Utf8String name );
            Utf8String getName() const;

            void setType( Utf8String type );
            Utf8String getType() const;

            friend std::ostream& operator<<( std::ostream &output_stream, const ModelField &output_model_field );

        protected:
            Utf8String name;
            Utf8String type;

    };



    typedef ::std::map<Utf8String,ModelField*,Utf8StringComparator> ModelFieldMap;
    typedef ::std::pair<Utf8String,ModelField*> ModelFieldMapPair;

}
}



#endif //ALTUMO_JET_MODELER_MY_MODEL_FIELD
