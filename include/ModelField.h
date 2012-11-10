#ifndef ALTUMO_JET_MODELER_MODEL_FIELD
#define ALTUMO_JET_MODELER_MODEL_FIELD

#include "jet/Utf8String.h"
#include "Model.h"

#include <map>


namespace jet{
namespace modeler{

    using ::jet::Utf8String;

    class ModelField{

        public:
            ModelField();

            void setName( Utf8String name );
            Utf8String getName() const;

            void setType( Utf8String type );
            Utf8String getType() const;



        protected:
            Utf8String name;
            Utf8String type;

    };

}
}



#endif //ALTUMO_JET_MODELER_MODEL_FIELD
