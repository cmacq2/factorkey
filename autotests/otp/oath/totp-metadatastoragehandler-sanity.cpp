#include "otp/oath/metadatastoragehandler.h"
#include "autotests/util/db/metadatastoragehandler-sanity.h"

#include <QTest>
#include <QtDebug>

class TOTPMetadataStorageHandlerSanityTest: public sanity::storage::db::MetadataStorageHandlerSanityTest
{
    Q_OBJECT
public:
    TOTPMetadataStorageHandlerSanityTest(QObject * parent = 0);
    virtual ~TOTPMetadataStorageHandlerSanityTest();
private Q_SLOTS:
    void checkBasicSanity(void);
    void checkParameters(void);
    void checkParameters_data(void);
private:
};

void TOTPMetadataStorageHandlerSanityTest::checkBasicSanity(void)
{
    sanity::storage::db::MetadataStorageHandlerSanityTest::basicSanity();
}

void TOTPMetadataStorageHandlerSanityTest::checkParameters(void)
{
    sanity::storage::db::MetadataStorageHandlerSanityTest::sanity();
}

void TOTPMetadataStorageHandlerSanityTest::checkParameters_data(void)
{
    sanity::storage::db::MetadataStorageHandlerSanityTest::sanity_data();
}

TOTPMetadataStorageHandlerSanityTest::TOTPMetadataStorageHandlerSanityTest(QObject * parent): MetadataStorageHandlerSanityTest(otp::oath::storage::db::TOTPMetadataStorageHandler::create(), parent) {}
TOTPMetadataStorageHandlerSanityTest::~TOTPMetadataStorageHandlerSanityTest() {}

QTEST_APPLESS_MAIN(TOTPMetadataStorageHandlerSanityTest)

#include "totp-metadatastoragehandler-sanity.moc"


