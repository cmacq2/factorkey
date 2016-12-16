
#include "otp/oath/generator.h"
#include "otp/oath/oath.h"
#include "otp/base32/base32.h"
#include "autotests/mock/storage/storage.h"
#include "otp/oath/parameters.h"
#include "otp/parameters.h"

#include <QTest>
#include <QtDebug>

class TOTPStoragePrivate: public stubs::storage::FakeStoragePrivate
{
public:
    TOTPStoragePrivate(const QString& entryId, const QString& password, const QHash<QString,QVariant>& preset):
        stubs::storage::FakeStoragePrivate(entryId, otp::storage::OTPTokenType::TOTP, password, preset) {}
    TOTPStoragePrivate(const QString& entryId, const QString& password):
        stubs::storage::FakeStoragePrivate(entryId, otp::storage::OTPTokenType::TOTP, password) {}
    TOTPStoragePrivate(const QString& password, const QHash<QString,QVariant>& preset):
        stubs::storage::FakeStoragePrivate(otp::storage::OTPTokenType::TOTP, password, preset) {}
    TOTPStoragePrivate(const QString& password):
        stubs::storage::FakeStoragePrivate(otp::storage::OTPTokenType::TOTP, password) {}
    TOTPStoragePrivate():
        stubs::storage::FakeStoragePrivate(otp::storage::OTPTokenType::TOTP, QString()) {}
};

class TOTPGeneratorSamplesTest: public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testDefaults(void);
    void testDefaults_data(void);
};

void TOTPGeneratorSamplesTest::testDefaults(void)
{
    QFETCH(QString, secret);
    QFETCH(int, timeSteps);

    QHash<QString, QVariant> map;
    map.insert(otp::parameters::key::ENCODING, (int) otp::generator::EncodingType::Base32);
    map.insert(otp::parameters::hashing::ALGORITHM, QVariant());
    map.insert(otp::oath::parameters::generic::LOCALE, QVariant());
    map.insert(otp::oath::parameters::generic::LENGTH, QVariant());
    map.insert(otp::oath::parameters::totp::EPOCH, QVariant());
    map.insert(otp::oath::parameters::totp::TIMESTEP, QVariant());

    QObject * parent = new QObject();

    QSharedPointer<TOTPStoragePrivate> stub(new TOTPStoragePrivate(secret, map));
    auto mock = new mock::storage::DelegatingMockStoragePrivate();
    mock->delegateToFake(stub);

    EXPECT_CALL(*mock, type()).Times(1);

    EXPECT_CALL(*mock, readPassword(testing::_)).Times(1);

    EXPECT_CALL(*mock, readParam(testing::Eq(otp::oath::parameters::totp::EPOCH), testing::_)).Times(1);
    EXPECT_CALL(*mock, readParam(testing::Eq(otp::oath::parameters::totp::TIMESTEP), testing::_)).Times(1);
    EXPECT_CALL(*mock, readParam(testing::Eq(otp::parameters::key::ENCODING), testing::_)).Times(1);
    EXPECT_CALL(*mock, readParam(testing::Eq(otp::parameters::hashing::ALGORITHM), testing::_)).Times(1);
    EXPECT_CALL(*mock, readParam(testing::Eq(otp::oath::parameters::generic::LOCALE), testing::_)).Times(1);
    EXPECT_CALL(*mock, readParam(testing::Eq(otp::oath::parameters::generic::LENGTH), testing::_)).Times(1);

    EXPECT_CALL(*mock, writeParam(testing::_, testing::_)).Times(0);
    EXPECT_CALL(*mock, readTokenType(testing::_)).Times(0);
    EXPECT_CALL(*mock, writeTokenType(testing::_)).Times(0);
    EXPECT_CALL(*mock, writePassword(testing::_)).Times(0);
    EXPECT_CALL(*mock, poll()).Times(0);
    EXPECT_CALL(*mock, exists()).Times(0);
    EXPECT_CALL(*mock, commit()).Times(0);

    auto storage = new otp::storage::Storage(mock, parent);
    auto params = otp::oath::generator::TOTPTokenParameters::create(storage, parent);
    auto generator = otp::oath::generator::TOTPTokenParameters::generator(params, timeSteps * otp::oath::DEFAULT_TIMESTEP_MSEC, parent);

    QString token;
    QVERIFY2(generator->generateToken(token), "Generating the token should succeed");
    QTEST(token, "rfc-test-vector");

    QVERIFY2(testing::Mock::VerifyAndClearExpectations(mock), "Interactions with the mock should match expectations");

    delete parent;
}

static void result(int k, const QString& secret, const char * expected)
{

    QByteArray output(expected, 6);

    QTest::newRow(qPrintable(QStringLiteral("RFC 4226 test vector, # time steps = %1").arg(k))) << secret << k << QString::fromLocal8Bit(output);
}

void TOTPGeneratorSamplesTest::testDefaults_data(void)
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
    QTest::addColumn<int>("timeSteps");
    QTest::addColumn<QString>("rfc-test-vector");

    for(int k = 0; k < 10; ++k)
    {
        result(k, otp::base32::recode(secret), corpus[k]);
    }
}

QTEST_APPLESS_MAIN(TOTPGeneratorSamplesTest)

#include "totp-generator-samples.moc"
