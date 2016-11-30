#ifndef FACTORKEY_METADB_H
#define FACTORKEY_METADB_H

#include "../storage/storage.h"
#include "metadatastoragehandler.h"

#include <QSqlDatabase>
#include <QString>
#include <QVariant>

namespace otp
{
    namespace storage
    {
        namespace db
        {
            class MetadataDbManager
            {
            public:
                static const QString METADATA_FOLDER;
                static const QString METADATA_DB_NAME;
            public:
                MetadataDbManager(const QString& connectionName);
                const QString& connectionName(void) const;
                virtual ~MetadataDbManager();
                virtual bool isOpened(void) const;
                virtual QSqlDatabase open(void);
                virtual bool close(void);

                virtual bool readType(const QString& entry, QVariant& value);
                virtual bool remove(const QString& entry);
                virtual bool contains(const QString & entry);
                virtual bool entries(QStringList& entryList);

                virtual const MetadataStorageHandler * handler(otp::storage::OTPTokenType type) const;
            private:
                const QString m_connectionName;
            };
        }
    }
}
#endif
