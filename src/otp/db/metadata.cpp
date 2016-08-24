#include "metadata.h"

#include <QMutex>

namespace otp
{
    namespace storage
    {
        namespace db
        {
            static QHash<int,MetadataHandlerConstructorFunction> paramCtorMap;
            bool registerMetadataHandlerType(otp::storage::OTPTokenType type, const MetadataHandlerConstructorFunction& ctor)
            {
                static QMutex m;
                QMutexLocker lock(&m);

                int t = (int) type;
                if(paramCtorMap.contains(t))
                {
                    return false;
                }
                else
                {
                    paramCtorMap.insert(t, ctor);
                    return true;
                }
            }

            MetadataStorageHandler * createMetadataHandler(const otp::storage::OTPTokenType type)
            {
                const int t = (int) type;
                if(!paramCtorMap.contains(t))
                {
                    return nullptr;
                }
                else
                {
                    auto ctor = paramCtorMap.value(t);
                    return ctor ? ctor() : nullptr;
                }
            }
        }
    }
}
