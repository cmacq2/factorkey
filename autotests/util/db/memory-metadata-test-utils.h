#ifndef FACTORKEY_AUTOTESTS_TEST_UTILS_METADATA_H
#define FACTORKEY_AUTOTESTS_TEST_UTILS_METADATA_H

#include <functional>
#include <QDateTime>
#include <QString>
#include "otp/db/metadata.h"

namespace test
{
    namespace storage
    {
        namespace db
        {
            QSqlDatabase testPreamble(QSharedPointer<otp::storage::db::MetadataDbManager> db);

            QSharedPointer<otp::storage::db::MetadataDbManager> createMemoryMetaDb(const QString& connName);

            std::function<void(QSharedPointer<otp::storage::db::MetadataDbManager>)> createMetadataTest(
                const QString& entryId,
                const std::function<void(QSharedPointer<otp::storage::db::MetadataDbManager>, QSharedPointer<otp::storage::db::Metadata>)>& testCase,
                bool autoclose = true);
            void dropSchema(QSharedPointer<otp::storage::db::MetadataDbManager> uut, bool& ok);
            void runTest(QSharedPointer<otp::storage::db::MetadataDbManager> uut,
                         const std::function<void(QSharedPointer<otp::storage::db::MetadataDbManager>,bool&)>& setup,
                         const std::function<void(QSharedPointer<otp::storage::db::MetadataDbManager>)>& test,
                         bool clean = true);
            void runTest(QSharedPointer<otp::storage::db::MetadataDbManager> uut,
                         const std::function<void(QSharedPointer<otp::storage::db::MetadataDbManager>)>& test,
                         bool clean = true);
        }
    }
}

#endif
