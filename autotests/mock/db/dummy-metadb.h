#ifndef FACTORKEY_AUTOTESTS_STUBBED_DUMMY_METADBH_H
#define FACTORKEY_AUTOTESTS_STUBBED_DUMMY_METADBH_H

#include "otp/db/metadb.h"
#include "otp/db/metadatastoragehandler.h"

namespace stubs
{
    namespace storage
    {
        namespace db
        {
            class DummyMetadataDbManager: public otp::storage::db::MetadataDbManager
            {
            public:
                DummyMetadataDbManager(const QString& connName);
                DummyMetadataDbManager(const QString& connName, const QHash<QString, enum otp::storage::OTPTokenType>& entries);
                DummyMetadataDbManager(const QString& connName, const QHash<int, QSharedPointer<otp::storage::db::MetadataStorageHandler>>& typeHandlers);
                DummyMetadataDbManager(const QString& connName, const QHash<int, QSharedPointer<otp::storage::db::MetadataStorageHandler>>& typeHandlers, const QHash<QString, enum otp::storage::OTPTokenType>& entries);
                DummyMetadataDbManager(const QString& connName, const QHash<int, QSharedPointer<otp::storage::db::MetadataStorageHandler>>& typeHandlers, const QHash<QString, enum otp::storage::OTPTokenType>& entries, bool openByDefault);

                const QStringList removedEntries(void) const;
                virtual ~DummyMetadataDbManager();
            public:
                virtual bool isOpened(void) const override;
                virtual QSqlDatabase open(void) override;
                virtual bool close(void) override;

                virtual bool readType(const QString& entry, QVariant& value) override;
                virtual bool remove(const QString& entry) override;
                virtual bool contains(const QString & entry) override;
                virtual bool entries(QStringList& entryList) override;
                virtual bool removeEntries(const QStringList& entryList) override;
                virtual bool removeAll(void) override;

                virtual const QSharedPointer<otp::storage::db::MetadataStorageHandler> handler(otp::storage::OTPTokenType type) override;
            private:
                const QHash<QString, enum otp::storage::OTPTokenType> m_entries;
                QStringList m_removed;
                bool m_opened = false;
            protected:
                virtual bool allowInitDb(void) const;
                virtual bool allowOpen(void) const;
                virtual bool allowClose(void) const;
                virtual bool allowReadType(void) const;
                virtual bool allowRemove(void) const;
                virtual bool allowContains(void) const;
                virtual bool allowEntries(void) const;
                virtual bool allowRemoveEntries(void) const;
                virtual bool allowRemoveAll(void) const;
                virtual bool allowHandler(void) const;
            };
        }
    }
}
#endif
