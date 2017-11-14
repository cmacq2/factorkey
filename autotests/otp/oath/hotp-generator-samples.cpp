
#include "otp/oath/generator.h"
#include "otp/base32/base32.h"
#include "autotests/mock/storage/storage.h"
#include "autotests/lib/trompeloeil.h"
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

    std::vector<autotests::integration::ExpectationViolation> violations;
    const auto old = autotests::integration::setup(violations);

    QHash<QString, QVariant> map;
    map.insert(otp::parameters::key::ENCODING, (int) otp::generator::EncodingType::Base32);
    map.insert(otp::parameters::hashing::ALGORITHM, QVariant());
    map.insert(otp::oath::parameters::generic::LOCALE, QVariant());
    map.insert(otp::oath::parameters::generic::LENGTH, QVariant());
    map.insert(otp::oath::parameters::hotp::COUNTER, counter);

    HOTPStoragePrivate stub(secret, map);
    auto mock = new mock::storage::MockStoragePrivate();

    REQUIRE_CALL(*mock, type()).TIMES(1).LR_RETURN(stub.type());
    REQUIRE_CALL(*mock, readPassword(trompeloeil::_)).TIMES(1).LR_RETURN(stub.readPassword(_1));
    REQUIRE_CALL(*mock, readParam(trompeloeil::eq(otp::oath::parameters::hotp::COUNTER), trompeloeil::_)).TIMES(2).LR_RETURN(stub.readParam(_1, _2));
    REQUIRE_CALL(*mock, readParam(trompeloeil::eq(otp::parameters::key::ENCODING), trompeloeil::_)).TIMES(1).LR_RETURN(stub.readParam(_1, _2));
    REQUIRE_CALL(*mock, readParam(trompeloeil::eq(otp::parameters::hashing::ALGORITHM), trompeloeil::_)).TIMES(1).LR_RETURN(stub.readParam(_1,_2));
    REQUIRE_CALL(*mock, readParam(trompeloeil::eq(otp::oath::parameters::generic::LOCALE), trompeloeil::_)).TIMES(1).LR_RETURN(stub.readParam(_1,_2));
    REQUIRE_CALL(*mock, readParam(trompeloeil::eq(otp::oath::parameters::generic::LENGTH), trompeloeil::_)).TIMES(1).LR_RETURN(stub.readParam(_1,_2));

    REQUIRE_CALL(*mock, writeParam(trompeloeil::eq(otp::oath::parameters::hotp::COUNTER), trompeloeil::eq(counter + 1))).TIMES(1).LR_RETURN(stub.writeParam(_1, _2));
    REQUIRE_CALL(*mock, commit()).TIMES(1).RETURN(true);

    QScopedPointer<otp::storage::Storage> storage(new otp::storage::Storage(mock));
    QScopedPointer<otp::oath::generator::HOTPTokenParameters> params(otp::oath::generator::HOTPTokenParameters::from(storage.data()));
    QScopedPointer<otp::generator::TokenGenerator> generator(params->generator());

    try {
        QString token;
        QVERIFY2(generator->generateToken(token), "Generating the token should succeed");
        QTEST(token, "rfc-test-vector");

        const QVariant resultingCounter = stub.rawStorage().value(otp::oath::parameters::hotp::COUNTER);
        QCOMPARE(resultingCounter.type(), QVariant::ULongLong);
        QCOMPARE(resultingCounter.toULongLong(), counter + 1);
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
