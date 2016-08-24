#ifndef FACTORKEY_METADATA_DB_H
#define FACTORKEY_METADATA_DB_H

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
                virtual ~MetadataStorageHandler() = 0;
                virtual otp::storage::OTPTokenType type(void) const;
            protected:
                virtual bool writeTokenType(const otp::storage::OTPTokenType&) = 0;
                virtual bool writeParam(const QString&, const QVariant&) =0;
                virtual bool fetchMetaData(QHash<QString,QVariant>&) = 0;
            };

            typedef std::function<MetadataStorageHandler*()> MetadataHandlerConstructorFunction;
            bool registerMetadataHandlerType(otp::storage::OTPTokenType type, const MetadataHandlerConstructorFunction& ctor);
            MetadataStorageHandler * createMetadataHandler(const otp::storage::OTPTokenType type);

            class MetadataDbManager
            {
            public:
                static const QString METADATA_FOLDER;
                MetadataDbManager();
                virtual ~MetadataDbManager();
                bool isOpened(void) const;
                bool open(void);
                bool close(void);

            private:
            };
        }
    }
}

#endif
