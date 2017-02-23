#include "memory-metadata-test-utils.h"
#include "autotests/mock/db/dummy-metadatastoragehandler.h"
#include "otp/dummy/metadatastoragehandler.h"
#include "otp/skey/metadatastoragehandler.h"
#include "otp/steam/metadatastoragehandler.h"
#include "otp/oath/metadatastoragehandler.h"
#include "autotests/mock/db/dummy-metadb.h"
#include "autotests/mock/db/memory-metadb.h"

#include <QTest>

namespace test
{
    namespace storage
    {
        namespace db
        {
            QSharedPointer<otp::storage::db::MetadataDbManager> createMemoryMetaDb(const QString& connName)
            {

                stubs::storage::db::MemoryMetadataDbBuilder builder;
                builder.setConnectionName(connName);

                otp::dummy::storage::db::DummyMetadataStorageHandler::registerWith(builder);
                otp::skey::storage::db::SKeyMetadataStorageHandler::registerWith(builder);
                otp::steam::storage::db::SteamGuardMetadataStorageHandler::registerWith(builder);
                otp::oath::storage::db::TOTPMetadataStorageHandler::registerWith(builder);
                otp::oath::storage::db::HOTPMetadataStorageHandler::registerWith(builder);

                QSharedPointer<otp::storage::db::MetadataDbManager> uut = builder.create();
                return uut;
            }

            std::function<void(QSharedPointer<otp::storage::db::MetadataDbManager>)> createMetadataTest(
                const QString& entryId,
                const std::function<void(QSharedPointer<otp::storage::db::MetadataDbManager>, QSharedPointer<otp::storage::db::Metadata>)>& testCase,
                bool autoclose)
            {
                std::function<void(QSharedPointer<otp::storage::db::MetadataDbManager>)> inner([entryId, testCase] (QSharedPointer<otp::storage::db::MetadataDbManager> db) -> void
                {
                    QSharedPointer<otp::storage::db::Metadata> data(new otp::storage::db::Metadata(entryId, db));
                    testCase(db, data);
                });
                if(autoclose)
                {
                    return std::function<void(QSharedPointer<otp::storage::db::MetadataDbManager>)>([inner](QSharedPointer<otp::storage::db::MetadataDbManager> db) -> void
                    {
                        inner(db);
                        db->close();
                    });
                }
                else
                {
                    return inner;
                }
            }

            void dropSchema(QSharedPointer<otp::storage::db::MetadataDbManager> uut, bool& ok)
            {
                ok = (bool) uut;
                QVERIFY2(uut, "Constructing the MemoryMetadataDbManager should succeed");
                if(ok)
                {
                    std::function<bool(void)> fn([uut]() -> bool
                    {
                        const auto& conn = uut->open();
                        return conn.isValid() && conn.isOpen();
                    });
                    ok = fn();
                    if(ok)
                    {
                        auto memDb = static_cast<stubs::storage::db::MemoryMetadataDbManager*>(uut.data());
                        ok = (bool) memDb;
                        QVERIFY2(memDb, "The MemoryMetadataDbManager cast should succeed");

                        if(ok)
                        {
                            QVERIFY2((ok = memDb->drop()), "Dropping the schema should succeed");
                        }
                    }
                    uut->close();
                }
            }

            void runTest(QSharedPointer<otp::storage::db::MetadataDbManager> uut,
                         const std::function<void(QSharedPointer<otp::storage::db::MetadataDbManager>)>& test,
                         bool clean)
            {
                const std::function<void(QSharedPointer<otp::storage::db::MetadataDbManager>, bool&)> setup([](QSharedPointer<otp::storage::db::MetadataDbManager>, bool& ok) -> void
                {
                    ok = true;
                });
                runTest(uut, setup, test, clean);
            }

            void runTest(QSharedPointer<otp::storage::db::MetadataDbManager> uut,
                         const std::function<void(QSharedPointer<otp::storage::db::MetadataDbManager>, bool&)>& setup,
                         const std::function<void(QSharedPointer<otp::storage::db::MetadataDbManager>)>& test, bool clean)
            {
                bool droppedOK = true;
                if(clean)
                {
                    dropSchema(uut, droppedOK);
                    QVERIFY2(droppedOK, "Schema precondition should hold but drop() failed!");
                }
                if(droppedOK)
                {
                    bool setupOK = false;
                    setup(uut, setupOK);
                    if(setupOK) {
                        test(uut);
                    }
                    dropSchema(uut, droppedOK);
                    QVERIFY2(droppedOK, "Schema postcondition should hold but drop() failed!");
                }
            }
        }
    }
}
