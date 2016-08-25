
#include "otp/oath/generator.h"
#include "otp/base32/base32.h"
#include "autotests/otp/storage/storage.h"

#include <QTest>
#include <QtDebug>

class HOTPStoragePrivate: public stubs::storage::DummyStoragePrivate
{
public:
    HOTPStoragePrivate(const QString& entryId, const QString& password, const QHash<QString,QVariant>& preset, QObject * parent = nullptr):
        stubs::storage::DummyStoragePrivate(entryId, otp::storage::OTPTokenType::HOTP, password, preset, parent) {}
    HOTPStoragePrivate(const QString& entryId, const QString& password, QObject * parent = nullptr):
        stubs::storage::DummyStoragePrivate(entryId, otp::storage::OTPTokenType::HOTP, password, parent) {}
    HOTPStoragePrivate(const QString& password, const QHash<QString,QVariant>& preset, QObject * parent = nullptr):
        stubs::storage::DummyStoragePrivate(otp::storage::OTPTokenType::HOTP, password, preset, parent) {}
    HOTPStoragePrivate(const QString& password, QObject * parent = nullptr):
        stubs::storage::DummyStoragePrivate(otp::storage::OTPTokenType::HOTP, password, parent) {}
    HOTPStoragePrivate(QObject * parent = nullptr):
        stubs::storage::DummyStoragePrivate(otp::storage::OTPTokenType::HOTP, QString(), parent) {}

    bool impl_commit(void)
    {
        return true;
    }
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
    map.insert(otp::generator::TokenParameters::OTP_KEY_ENCODING_TYPE, (int) otp::generator::EncodingType::Base32);
    map.insert(otp::generator::GenericTokenParameters::OTP_HMAC_HASH_ALGORITHM, QVariant());
    map.insert(otp::oath::generator::GenericOTPParameters::OTP_ENCODER_TOKEN_LOCALE, QVariant());
    map.insert(otp::oath::generator::GenericOTPParameters::OTP_ENCODER_TOKEN_LENGTH, QVariant());
    map.insert(otp::oath::generator::HOTPTokenParameters::HOTP_TOKEN_COUNTER, counter);

    auto stub = new HOTPStoragePrivate(secret, map);
    auto storage = new otp::storage::Storage(stub);
    auto params = otp::oath::generator::HOTPTokenParameters::create(storage);
    auto generator = otp::oath::generator::HOTPTokenParameters::generator(params);

    QString token;
    QVERIFY2(generator->generateToken(token), "Generating the token should succeed");
    QTEST(token, "rfc-test-vector");

    QList<bool> commitResult;
    QList<enum otp::storage::OTPTokenType> typeResult;
    QList<QList<QVariant>> paramWrites;
    stubs::storage::DummyStoragePrivate::expect_param(paramWrites, true, otp::oath::generator::HOTPTokenParameters::HOTP_TOKEN_COUNTER, counter + 1);

    QList<QList<QVariant>> paramReads;
    stubs::storage::DummyStoragePrivate::expect_param(paramReads, true, otp::oath::generator::HOTPTokenParameters::HOTP_TOKEN_COUNTER, counter);
    stubs::storage::DummyStoragePrivate::expect_param(paramReads, true, otp::generator::TokenParameters::OTP_KEY_ENCODING_TYPE, (int) otp::generator::EncodingType::Base32);
    stubs::storage::DummyStoragePrivate::expect_param(paramReads, true, otp::generator::GenericTokenParameters::OTP_HMAC_HASH_ALGORITHM, QVariant());
    stubs::storage::DummyStoragePrivate::expect_param(paramReads, true, otp::oath::generator::GenericOTPParameters::OTP_ENCODER_TOKEN_LOCALE, QVariant());
    stubs::storage::DummyStoragePrivate::expect_param(paramReads, true, otp::oath::generator::GenericOTPParameters::OTP_ENCODER_TOKEN_LENGTH, QVariant());
    stubs::storage::DummyStoragePrivate::expect_param(paramReads, true, otp::oath::generator::HOTPTokenParameters::HOTP_TOKEN_COUNTER, counter);

    stub->check_read_param(paramReads);
    stub->check_write_param(paramWrites);
    stub->check_type(typeResult << otp::storage::OTPTokenType::HOTP);
    stub->check_no_read_token_type();
    stub->check_no_write_token_type();
    stub->check_no_write_password();
    stub->check_no_poll();
    stub->check_no_exists();
    stub->check_commit(commitResult << true);

    const QVariant resultingCounter = stub->rawStorage().value(otp::oath::generator::HOTPTokenParameters::HOTP_TOKEN_COUNTER);
    QCOMPARE(resultingCounter.type(), QVariant::ULongLong);
    QCOMPARE(resultingCounter.toULongLong(), counter + 1);

    generator->deleteLater();
    params->deleteLater();
    storage->deleteLater();
    stub->deleteLater();
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
