#include "otp/steam/metadatastoragehandler.h"
#include "autotests/otp/db/metadatastoragehandler-sanity.h"

#include <QTest>
#include <QtDebug>

class SteamGuardMetadataStorageHandlerSanityTest: public sanity::storage::db::MetadataStorageHandlerSanityTest
{
    Q_OBJECT
public:
    SteamGuardMetadataStorageHandlerSanityTest(QObject * parent = 0);
    virtual ~SteamGuardMetadataStorageHandlerSanityTest();
private Q_SLOTS:
    void checkBasicSanity(void);
    void checkParameters(void);
    void checkParameters_data(void);
private:
};

void SteamGuardMetadataStorageHandlerSanityTest::checkBasicSanity(void)
{
    sanity::storage::db::MetadataStorageHandlerSanityTest::basicSanity();
}

void SteamGuardMetadataStorageHandlerSanityTest::checkParameters(void)
{
    sanity::storage::db::MetadataStorageHandlerSanityTest::sanity();
}

void SteamGuardMetadataStorageHandlerSanityTest::checkParameters_data(void)
{
    sanity::storage::db::MetadataStorageHandlerSanityTest::sanity_data();
}

SteamGuardMetadataStorageHandlerSanityTest::SteamGuardMetadataStorageHandlerSanityTest(QObject * parent): MetadataStorageHandlerSanityTest(otp::steam::storage::db::SteamGuardMetadataStorageHandler::create(), parent) {}
SteamGuardMetadataStorageHandlerSanityTest::~SteamGuardMetadataStorageHandlerSanityTest() {}

QTEST_APPLESS_MAIN(SteamGuardMetadataStorageHandlerSanityTest)

#include "steam-metadatastoragehandler-sanity.moc"



