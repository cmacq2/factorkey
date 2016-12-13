#ifndef FACTORKEY_METADATA_DB_H
#define FACTORKEY_METADATA_DB_H

#include "../storage/storage.h"
#include "metadb.h"
#include "metadatastoragehandler.h"

#include <QScopedPointer>
#include <QSharedPointer>
#include <QHash>
#include <QString>
#include <QVariant>

namespace otp
{
    namespace storage
    {
        namespace db
        {

            class Metadata {
            public:
                Metadata(const QString& entryId, QSharedPointer<MetadataDbManager> db);

                bool readTokenType(otp::storage::OTPTokenType& type) const;
                bool readParam(const QString& param, QVariant& value) const;

                bool writeTokenType(const otp::storage::OTPTokenType& type);
                bool writeParam(const QString& param, const QVariant& value);

                bool remove(void);
                bool commit(void);
                bool poll(void);
                bool exists(void) const;
            private:
                bool establishTokenType(otp::storage::OTPTokenType& type);
                bool createHandler(const otp::storage::OTPTokenType& type);
                bool convertHandler(const QSharedPointer<MetadataStorageHandler> newHandler);
                bool containsType(const QHash<QString,QVariant>& data) const;
                bool haveType(void) const;
                bool typeWritten(void) const;
            private:
                const QString& m_entryId;
                otp::storage::OTPTokenType m_typeRead;
                otp::storage::OTPTokenType m_typeWrite;
                QHash<QString, QVariant> m_dataRead;
                QHash<QString, QVariant> m_dataWrite;
                QSharedPointer<const MetadataStorageHandler> m_typeHandler;
                QSharedPointer<MetadataDbManager> m_dbManager;
            };
        }
    }
}

#endif
