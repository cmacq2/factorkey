#include "otp/oath/metadatastoragehandler.h"
#include "autotests/util/db/metadatastoragehandler-sanity.h"

#include <QTest>
#include <QtDebug>

class HOTPMetadataStorageHandlerSanityTest: public sanity::storage::db::MetadataStorageHandlerSanityTest
{
    Q_OBJECT
public:
    HOTPMetadataStorageHandlerSanityTest(QObject * parent = 0);
    virtual ~HOTPMetadataStorageHandlerSanityTest();
private Q_SLOTS:
    void checkBasicSanity(void);
    void checkParameters(void);
    void checkParameters_data(void);
private:
};

void HOTPMetadataStorageHandlerSanityTest::checkBasicSanity(void)
{
    sanity::storage::db::MetadataStorageHandlerSanityTest::basicSanity();
}

void HOTPMetadataStorageHandlerSanityTest::checkParameters(void)
{
    sanity::storage::db::MetadataStorageHandlerSanityTest::sanity();
}

void HOTPMetadataStorageHandlerSanityTest::checkParameters_data(void)
{
    sanity::storage::db::MetadataStorageHandlerSanityTest::sanity_data();
}

HOTPMetadataStorageHandlerSanityTest::HOTPMetadataStorageHandlerSanityTest(QObject * parent): MetadataStorageHandlerSanityTest(otp::oath::storage::db::HOTPMetadataStorageHandler::create(), parent) {}
HOTPMetadataStorageHandlerSanityTest::~HOTPMetadataStorageHandlerSanityTest() {}

QTEST_APPLESS_MAIN(HOTPMetadataStorageHandlerSanityTest)

#include "hotp-metadatastoragehandler-sanity.moc"

