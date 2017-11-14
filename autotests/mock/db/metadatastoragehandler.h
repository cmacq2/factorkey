#ifndef FACTORKEY_AUTOTESTS_MOCKED_METADATASTORAGEHANDLER_H
#define FACTORKEY_AUTOTESTS_MOCKED_METADATASTORAGEHANDLER_H

#include "otp/db/metadatastoragehandler.h"
#include "otp/db/metadb.h"
#include "autotests/lib/trompeloeil.h"

namespace mock
{
    namespace storage
    {
        namespace db
        {
            class MockMetadataStorageHandler: public otp::storage::db::MetadataStorageHandler
            {
            public:
                MockMetadataStorageHandler(otp::storage::OTPTokenType type);
                virtual ~MockMetadataStorageHandler();
                MAKE_CONST_MOCK0(type, otp::storage::OTPTokenType());
                MAKE_CONST_MOCK0(schema, const QSet<QString>&());
                MAKE_CONST_MOCK3(saveMetaData, bool(const QString&, const QHash<QString,QVariant>&, otp::storage::db::MetadataDbManager *));
                MAKE_CONST_MOCK3(fetchMetaData, bool(const QString&, QHash<QString,QVariant>&, otp::storage::db::MetadataDbManager *));
                MAKE_CONST_MOCK4(pruneMetaData, bool(const QString&, const QSet<QString>&, const QSet<QString>&, otp::storage::db::MetadataDbManager *));
                MAKE_CONST_MOCK3(deleteMetaData, bool(const QString&, const QSet<QString>&, otp::storage::db::MetadataDbManager *));
                MAKE_CONST_MOCK3(resetMetaData, bool(const QString&, const QSet<QString>&, otp::storage::db::MetadataDbManager *));
                MAKE_CONST_MOCK0(keys, QSet<QString>());
                MAKE_CONST_MOCK1(isParamNameValid, bool(const QString&));
            };
        }
    }
}

#endif
