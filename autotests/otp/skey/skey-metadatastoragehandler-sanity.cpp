#include "otp/skey/metadatastoragehandler.h"
#include "autotests/otp/db/metadatastoragehandler-sanity.h"

#include <QTest>
#include <QtDebug>

class SKeyMetadataStorageHandlerSanityTest: public sanity::storage::db::MetadataStorageHandlerSanityTest
{
    Q_OBJECT
public:
    SKeyMetadataStorageHandlerSanityTest(QObject * parent = 0);
    virtual ~SKeyMetadataStorageHandlerSanityTest();
private Q_SLOTS:
    void checkBasicSanity(void);
    void checkParameters(void);
    void checkParameters_data(void);
private:
};

void SKeyMetadataStorageHandlerSanityTest::checkBasicSanity(void)
{
    sanity::storage::db::MetadataStorageHandlerSanityTest::basicSanity();
}

void SKeyMetadataStorageHandlerSanityTest::checkParameters(void)
{
    sanity::storage::db::MetadataStorageHandlerSanityTest::sanity();
}

void SKeyMetadataStorageHandlerSanityTest::checkParameters_data(void)
{
    sanity::storage::db::MetadataStorageHandlerSanityTest::sanity_data();
}

SKeyMetadataStorageHandlerSanityTest::SKeyMetadataStorageHandlerSanityTest(QObject * parent): MetadataStorageHandlerSanityTest(otp::skey::storage::db::SKeyMetadataStorageHandler::create(), parent) {}
SKeyMetadataStorageHandlerSanityTest::~SKeyMetadataStorageHandlerSanityTest() {}

QTEST_APPLESS_MAIN(SKeyMetadataStorageHandlerSanityTest)

#include "skey-metadatastoragehandler-sanity.moc"


