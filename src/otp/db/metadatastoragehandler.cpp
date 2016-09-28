#include "metadatastoragehandler.h"

#include <QMutex>

namespace otp
{
    namespace storage
    {
        namespace db
        {
            MetadataStorageHandler::MetadataStorageHandler(otp::storage::OTPTokenType type): m_type(type) {}
            MetadataStorageHandler::~MetadataStorageHandler() {}

            bool MetadataStorageHandler::isParamNameValid(const QString& param) const
            {
                return false;
            }

            bool MetadataStorageHandler::saveMetaData(const QString& entryId, const QHash<QString,QVariant>& metadata) const
            {
                return false;
            }

            bool MetadataStorageHandler::saveParam(const QString& entryId, const QString& param, const QVariant& value) const
            {
                return false;
            }

            bool MetadataStorageHandler::fetchParam(const QString& entryId, const QString& param, QVariant& value) const
            {
                return false;
            }

            bool MetadataStorageHandler::fetchMetaData(const QString& entryId, QHash<QString,QVariant>& metadata) const
            {
                return false;
            }

            bool MetadataStorageHandler::deleteMetaData(const QString& entryId, const QStringList& keys) const
            {
                return false;
            }

            bool MetadataStorageHandler::deleteParam(const QString& entryId, const QString& key) const
            {
                return false;
            }

            const QStringList MetadataStorageHandler::keys(void) const
            {
                return QStringList();
            }

            bool MetadataStorageHandler::registerType(otp::storage::OTPTokenType type, const ConstructorFunction& ctor)
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

            MetadataStorageHandler * MetadataStorageHandler::createHandler(const otp::storage::OTPTokenType type)
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

            otp::storage::OTPTokenType MetadataStorageHandler::type(void) const
            {
                return m_type;
            }
        }
    }
}
