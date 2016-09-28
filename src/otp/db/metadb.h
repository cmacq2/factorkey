#ifndef FACTORKEY_METADB_H
#define FACTORKEY_METADB_H

#include "../storage/storage.h"
#include "metadatastoragehandler.h"

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
            public:
                MetadataDbManager();
                virtual ~MetadataDbManager();
                virtual bool isOpened(void) const;
                virtual bool open(void);
                virtual bool close(void);

                virtual bool readType(const QString& entry, QVariant& value);
                virtual bool remove(const QString& entry);
                virtual bool contains(const QString & entry);
                virtual bool entries(QStringList& entryList);

                virtual QSharedPointer<MetadataStorageHandler> getHandler(otp::storage::OTPTokenType type);

            private:
            };
        }
    }
}
#endif
