
#include "otp/oath/generator.h"
#include "otp/oath/oath.h"
#include "otp/base32/base32.h"
#include "autotests/mock/storage/storage.h"
#include "autotests/lib/trompeloeil.h"
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

    std::vector<autotests::integration::ExpectationViolation> violations;
    const auto old = autotests::integration::setup(violations);

    QHash<QString, QVariant> map;
    map.insert(otp::parameters::key::ENCODING, (int) otp::generator::EncodingType::Base32);
    map.insert(otp::parameters::hashing::ALGORITHM, QVariant());
    map.insert(otp::oath::parameters::generic::LOCALE, QVariant());
    map.insert(otp::oath::parameters::generic::LENGTH, QVariant());
    map.insert(otp::oath::parameters::totp::EPOCH, QVariant());
    map.insert(otp::oath::parameters::totp::TIMESTEP, QVariant());

    TOTPStoragePrivate stub(secret, map);
    auto mock = new mock::storage::MockStoragePrivate();

    REQUIRE_CALL(*mock, type()).TIMES(1).LR_RETURN(stub.type());
    REQUIRE_CALL(*mock, readPassword(trompeloeil::_)).TIMES(1).LR_RETURN(stub.readPassword(_1));
    REQUIRE_CALL(*mock, readParam(trompeloeil::eq(otp::oath::parameters::totp::EPOCH), trompeloeil::_)).TIMES(1).LR_RETURN(stub.readParam(_1, _2));
    REQUIRE_CALL(*mock, readParam(trompeloeil::eq(otp::oath::parameters::totp::TIMESTEP), trompeloeil::_)).TIMES(1).LR_RETURN(stub.readParam(_1, _2));
    REQUIRE_CALL(*mock, readParam(trompeloeil::eq(otp::parameters::key::ENCODING), trompeloeil::_)).TIMES(1).LR_RETURN(stub.readParam(_1, _2));
    REQUIRE_CALL(*mock, readParam(trompeloeil::eq(otp::parameters::hashing::ALGORITHM), trompeloeil::_)).TIMES(1).LR_RETURN(stub.readParam(_1,_2));
    REQUIRE_CALL(*mock, readParam(trompeloeil::eq(otp::oath::parameters::generic::LOCALE), trompeloeil::_)).TIMES(1).LR_RETURN(stub.readParam(_1,_2));
    REQUIRE_CALL(*mock, readParam(trompeloeil::eq(otp::oath::parameters::generic::LENGTH), trompeloeil::_)).TIMES(1).LR_RETURN(stub.readParam(_1,_2));

    QScopedPointer<otp::storage::Storage> storage(new otp::storage::Storage(mock));
    QScopedPointer< otp::oath::generator::TOTPTokenParameters> params( otp::oath::generator::TOTPTokenParameters::from(storage.data()));
    QScopedPointer<otp::generator::TokenGenerator> generator(params->generator(timeSteps * otp::oath::DEFAULT_TIMESTEP_MSEC));

    try {
        QString token;
        QVERIFY2(generator->generateToken(token), "Generating the token should succeed");
        QTEST(token, "rfc-test-vector");
    }
    catch(autotests::integration::ExpectationViolationException& ev)
    {
        const auto msg = ev.report();
        QFAIL(msg.data());
    }

    QVERIFY2(autotests::integration::verifyExpectations(violations), "Interactions with the mock should match expectations");
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
