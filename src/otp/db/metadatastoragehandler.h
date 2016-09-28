#ifndef FACTORKEY_METADATASTORAGEHANDLER_H
#define FACTORKEY_METADATASTORAGEHANDLER_H

#include "../storage/storage.h"

#include <functional>

#include <QHash>
#include <QString>
#include <QVariant>

namespace otp
{
    namespace storage
    {
        namespace db
        {
            class MetadataStorageHandler
            {
            public:
                typedef std::function<MetadataStorageHandler*()> ConstructorFunction;
                static MetadataStorageHandler * createHandler(const otp::storage::OTPTokenType type);
            public:
                virtual ~MetadataStorageHandler();
                virtual otp::storage::OTPTokenType type(void) const;
                virtual bool saveMetaData(const QString& entryId, const QHash<QString,QVariant>& metadata) const;
                virtual bool saveParam(const QString& entryId, const QString& param, const QVariant& value) const;
                virtual bool fetchMetaData(const QString& entryId, QHash<QString,QVariant>& metadata) const;
                virtual bool fetchParam(const QString& entryId, const QString& param, QVariant& value) const;
                virtual bool deleteMetaData(const QString& entryId, const QStringList& keys) const;
                virtual bool deleteParam(const QString& entryId, const QString& key) const;
                virtual const QStringList keys(void) const;
                virtual bool isParamNameValid(const QString& param) const;
            protected:
                MetadataStorageHandler(otp::storage::OTPTokenType type);
            protected:
                static bool registerType(otp::storage::OTPTokenType type, const ConstructorFunction& ctor);
            private:
                static QHash<int,ConstructorFunction> paramCtorMap;
                const otp::storage::OTPTokenType m_type;
            };
        }
    }
}
#endif
