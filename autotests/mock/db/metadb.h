#ifndef FACTORKEY_AUTOTESTS_MOCKED_METADBH_H
#define FACTORKEY_AUTOTESTS_MOCKED_METADBH_H

#include "otp/db/metadb.h"
#include "otp/db/metadatastoragehandler.h"
#include "autotests/lib/trompeloeil.h"

namespace mock
{
    namespace storage
    {
        namespace db
        {
            class MockMetadataDbManager: public otp::storage::db::MetadataDbManager
            {
            public:
                MockMetadataDbManager(const QString& connectionName,
                                      const QHash<int, QSharedPointer<otp::storage::db::MetadataStorageHandler>>& handlers,
                                      const otp::storage::db::MetadataDbManager::InitDb& configure);
                virtual ~MockMetadataDbManager();

                MAKE_CONST_MOCK0(isOpened, bool(void));
                MAKE_MOCK0(open, QSqlDatabase(void));
                MAKE_MOCK0(close, bool(void));

                MAKE_MOCK2(readType, bool(const QString& entry, QVariant& value));
                MAKE_MOCK1(removeEntries, bool(const QStringList& entryList));
                MAKE_MOCK0(removeAll, bool(void));
                MAKE_MOCK1(remove, bool(const QString& entry));
                MAKE_MOCK1(contains, bool(const QString & entry));
                MAKE_MOCK1(entries, bool(QStringList& entryList));

                MAKE_MOCK1(handler, const QSharedPointer<otp::storage::db::MetadataStorageHandler>(otp::storage::OTPTokenType type));

                MAKE_CONST_MOCK0(supportedHandlers, QList<otp::storage::OTPTokenType>(void));
                MAKE_CONST_MOCK1(supports, bool(otp::storage::OTPTokenType));
            };


        }
    }
}
#endif
