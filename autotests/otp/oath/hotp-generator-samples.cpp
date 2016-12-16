
#include "otp/oath/generator.h"
#include "otp/base32/base32.h"
#include "autotests/mock/storage/storage.h"
#include "otp/oath/parameters.h"
#include "otp/parameters.h"

#include <QTest>
#include <QtDebug>

class HOTPStoragePrivate: public stubs::storage::FakeStoragePrivate
{
public:
    HOTPStoragePrivate(const QString& entryId, const QString& password, const QHash<QString,QVariant>& preset):
        stubs::storage::FakeStoragePrivate(entryId, otp::storage::OTPTokenType::HOTP, password, preset) {}
    HOTPStoragePrivate(const QString& entryId, const QString& password):
        stubs::storage::FakeStoragePrivate(entryId, otp::storage::OTPTokenType::HOTP, password) {}
    HOTPStoragePrivate(const QString& password, const QHash<QString,QVariant>& preset):
        stubs::storage::FakeStoragePrivate(otp::storage::OTPTokenType::HOTP, password, preset) {}
    HOTPStoragePrivate(const QString& password):
        stubs::storage::FakeStoragePrivate(otp::storage::OTPTokenType::HOTP, password) {}
    HOTPStoragePrivate():
        stubs::storage::FakeStoragePrivate(otp::storage::OTPTokenType::HOTP, QString()) {}
};

class HOTPGeneratorSamplesTest: public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testDefaults(void);
    void testDefaults_data(void);
};

void HOTPGeneratorSamplesTest::testDefaults(void)
{
    QFETCH(QString, secret);
    QFETCH(quint64, counter);

    QHash<QString, QVariant> map;
    map.insert(otp::parameters::key::ENCODING, (int) otp::generator::EncodingType::Base32);
    map.insert(otp::parameters::hashing::ALGORITHM, QVariant());
    map.insert(otp::oath::parameters::generic::LOCALE, QVariant());
    map.insert(otp::oath::parameters::generic::LENGTH, QVariant());
    map.insert(otp::oath::parameters::hotp::COUNTER, counter);

    QObject * parent = new QObject();

    QSharedPointer<HOTPStoragePrivate> stub(new HOTPStoragePrivate(secret, map));
    auto mock = new mock::storage::DelegatingMockStoragePrivate();
    mock->delegateToFake(stub);

    EXPECT_CALL(*mock, type()).Times(1);

    EXPECT_CALL(*mock, readPassword(testing::_)).Times(1);

    EXPECT_CALL(*mock, readParam(testing::Eq(otp::oath::parameters::hotp::COUNTER), testing::_)).Times(2);
    EXPECT_CALL(*mock, readParam(testing::Eq(otp::parameters::key::ENCODING), testing::_)).Times(1);
    EXPECT_CALL(*mock, readParam(testing::Eq(otp::parameters::hashing::ALGORITHM), testing::_)).Times(1);
    EXPECT_CALL(*mock, readParam(testing::Eq(otp::oath::parameters::generic::LOCALE), testing::_)).Times(1);
    EXPECT_CALL(*mock, readParam(testing::Eq(otp::oath::parameters::generic::LENGTH), testing::_)).Times(1);

    EXPECT_CALL(*mock, writeParam(testing::Eq(otp::oath::parameters::hotp::COUNTER), testing::Eq(counter +1))).Times(1);
    EXPECT_CALL(*mock, readTokenType(testing::_)).Times(0);
    EXPECT_CALL(*mock, writeTokenType(testing::_)).Times(0);
    EXPECT_CALL(*mock, writePassword(testing::_)).Times(0);
    EXPECT_CALL(*mock, poll()).Times(0);
    EXPECT_CALL(*mock, exists()).Times(0);
    EXPECT_CALL(*mock, commit()).Times(1).WillRepeatedly(testing::Return(true));

    auto storage = new otp::storage::Storage(mock, parent);
    auto params = otp::oath::generator::HOTPTokenParameters::create(storage, parent);
    auto generator = otp::oath::generator::HOTPTokenParameters::generator(params, parent);

    QString token;
    QVERIFY2(generator->generateToken(token), "Generating the token should succeed");
    QTEST(token, "rfc-test-vector");

    const QVariant resultingCounter = stub->rawStorage().value(otp::oath::parameters::hotp::COUNTER);
    QCOMPARE(resultingCounter.type(), QVariant::ULongLong);
    QCOMPARE(resultingCounter.toULongLong(), counter + 1);

    QVERIFY2(testing::Mock::VerifyAndClearExpectations(mock), "Interactions with the mock should match expectations");

    delete parent;
}

static void result(int k, const QString& secret, const char * expected)
{

    QByteArray output(expected, 6);

    QTest::newRow(qPrintable(QStringLiteral("RFC 4226 test vector, counter value = %1").arg(k))) << secret << (quint64) k << QString::fromLocal8Bit(output);
}

void HOTPGeneratorSamplesTest::testDefaults_data(void)
{
    static const char * corpus[10] {
        "755224",
        "287082",
        "359152",
        "969429",
        "338314",
        "254676",
        "287922",
        "162583",
        "399871",
        "520489"
    };
    static QLatin1String secret("12345678901234567890");

    QTest::addColumn<QString>("secret");
    QTest::addColumn<quint64>("counter");
    QTest::addColumn<QString>("rfc-test-vector");

    for(int k = 0; k < 10; ++k)
    {
        result(k, otp::base32::recode(secret), corpus[k]);
    }
}

QTEST_APPLESS_MAIN(HOTPGeneratorSamplesTest)

#include "hotp-generator-samples.moc"
