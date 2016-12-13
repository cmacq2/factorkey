#include "otp/dummy/metadatastoragehandler.h"
#include "autotests/util/db/metadatastoragehandler-sanity.h"

#include <QTest>
#include <QtDebug>

class DummyMetadataStorageHandlerSanityTest: public sanity::storage::db::MetadataStorageHandlerSanityTest
{
    Q_OBJECT
public:
    DummyMetadataStorageHandlerSanityTest(QObject * parent = 0);
    virtual ~DummyMetadataStorageHandlerSanityTest();
private Q_SLOTS:
    void checkBasicSanity(void);
    void checkParameters(void);
    void checkParameters_data(void);
private:
};

void DummyMetadataStorageHandlerSanityTest::checkBasicSanity(void)
{
    sanity::storage::db::MetadataStorageHandlerSanityTest::basicSanity();
}

void DummyMetadataStorageHandlerSanityTest::checkParameters(void)
{
    sanity::storage::db::MetadataStorageHandlerSanityTest::sanity();
}

void DummyMetadataStorageHandlerSanityTest::checkParameters_data(void)
{
    sanity::storage::db::MetadataStorageHandlerSanityTest::sanity_data();
}

DummyMetadataStorageHandlerSanityTest::DummyMetadataStorageHandlerSanityTest(QObject * parent): MetadataStorageHandlerSanityTest(otp::dummy::storage::db::DummyMetadataStorageHandler::create(), parent) {}
DummyMetadataStorageHandlerSanityTest::~DummyMetadataStorageHandlerSanityTest() {}

QTEST_APPLESS_MAIN(DummyMetadataStorageHandlerSanityTest)

#include "dummy-metadatastoragehandler-sanity.moc"



