
#include "otp/oath/generator.h"
#include "otp/oath/oath.h"
#include "otp/base32/base32.h"
#include "autotests/otp/storage/storage.h"

#include <QTest>
#include <QtDebug>

class TOTPStoragePrivate: public stubs::storage::DummyStoragePrivate
{
public:
    TOTPStoragePrivate(const QString& entryId, const QString& password, const QMap<QString,QVariant>& preset, QObject * parent = nullptr):
        stubs::storage::DummyStoragePrivate(entryId, otp::storage::OTPTokenType::TOTP, password, preset, parent) {}
    TOTPStoragePrivate(const QString& entryId, const QString& password, QObject * parent = nullptr):
        stubs::storage::DummyStoragePrivate(entryId, otp::storage::OTPTokenType::TOTP, password, parent) {}
    TOTPStoragePrivate(const QString& password, const QMap<QString,QVariant>& preset, QObject * parent = nullptr):
        stubs::storage::DummyStoragePrivate(otp::storage::OTPTokenType::TOTP, password, preset, parent) {}
    TOTPStoragePrivate(const QString& password, QObject * parent = nullptr):
        stubs::storage::DummyStoragePrivate(otp::storage::OTPTokenType::TOTP, password, parent) {}
    TOTPStoragePrivate(QObject * parent = nullptr):
        stubs::storage::DummyStoragePrivate(otp::storage::OTPTokenType::TOTP, QString(), parent) {}
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

    QMap<QString, QVariant> map;
    map.insert(otp::generator::TokenParameters::OTP_KEY_ENCODING_TYPE, (int) otp::generator::EncodingType::Base32);
    map.insert(otp::generator::GenericTokenParameters::OTP_HMAC_HASH_ALGORITHM, QVariant());
    map.insert(otp::oath::generator::GenericOTPParameters::OTP_ENCODER_TOKEN_LOCALE, QVariant());
    map.insert(otp::oath::generator::GenericOTPParameters::OTP_ENCODER_TOKEN_LENGTH, QVariant());
    map.insert(otp::oath::generator::TOTPTokenParameters::TOTP_TOKEN_EPOCH, QVariant());
    map.insert(otp::oath::generator::TOTPTokenParameters::TOTP_TOKEN_TIMESTEP, QVariant());

    auto stub = new TOTPStoragePrivate(secret, map);
    auto storage = new otp::storage::Storage(stub);
    auto params = otp::oath::generator::TOTPTokenParameters::create(storage);
    auto generator = otp::oath::generator::TOTPTokenParameters::generator(params, timeSteps * otp::oath::DEFAULT_TIMESTEP_MSEC);

    QString token;
    QVERIFY2(generator->generateToken(token), "Generating the token should succeed");
    QTEST(token, "rfc-test-vector");

    QList<enum otp::storage::OTPTokenType> typeResult;
    QList<QList<QVariant>> paramReads;
    stubs::storage::expect_param(paramReads, true, otp::oath::generator::TOTPTokenParameters::TOTP_TOKEN_EPOCH, QVariant());
    stubs::storage::expect_param(paramReads, true, otp::oath::generator::TOTPTokenParameters::TOTP_TOKEN_TIMESTEP, QVariant());
    stubs::storage::expect_param(paramReads, true, otp::generator::TokenParameters::OTP_KEY_ENCODING_TYPE, (int) otp::generator::EncodingType::Base32);
    stubs::storage::expect_param(paramReads, true, otp::generator::GenericTokenParameters::OTP_HMAC_HASH_ALGORITHM, QVariant());
    stubs::storage::expect_param(paramReads, true, otp::oath::generator::GenericOTPParameters::OTP_ENCODER_TOKEN_LOCALE, QVariant());
    stubs::storage::expect_param(paramReads, true, otp::oath::generator::GenericOTPParameters::OTP_ENCODER_TOKEN_LENGTH, QVariant());

    stub->check_read_param(paramReads);
    stub->check_no_write_param();
    stub->check_type(typeResult << otp::storage::OTPTokenType::TOTP);
    stub->check_no_read_token_type();
    stub->check_no_write_token_type();
    stub->check_no_write_password();
    stub->check_no_poll();
    stub->check_no_exists();
    stub->check_no_commit();

    generator->deleteLater();
    params->deleteLater();
    storage->deleteLater();
    stub->deleteLater();
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
