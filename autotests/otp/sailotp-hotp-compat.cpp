
#include "otp/token.h"
#include "otp/base32.h"

#include <QTest>
#include <QtDebug>

class SailOTPHOTPCompatTest: public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase(void);
    void testSteam(void);
    void testSteam_data(void);
    void testHOTPDefault(void);
    void testHOTPDefault_data(void);
private:
    void test(const otp::Encoder& encoder);
    void setupDataColumns(void);
private:
    otp::Key m_key;
    otp::Algorithm m_algo;
};

void SailOTPHOTPCompatTest::initTestCase(void)
{
    m_key = otp::keyForAuthenticator();
    m_algo = otp::hmacAlgorithm();
}

void SailOTPHOTPCompatTest::setupDataColumns(void)
{
    QTest::addColumn<QString>("secret");
    QTest::addColumn<quint64>("counter");
    QTest::addColumn<QString>("sailotp");
}

void SailOTPHOTPCompatTest::test(const otp::Encoder& encoder)
{
    QFETCH(QString, secret);
    QFETCH(quint64, counter);
    const otp::Message msg([counter](void) -> QByteArray
    {
       return otp::hotpTokenMessage(counter);
    });

    qDebug() << "test:: key is:"<< secret;
    QTEST(otp::token(secret, msg, m_key, m_algo, encoder), "sailotp");
}

static void result(const char* caseName, const QString& secret, const quint64 counter, const QString& expected)
{
    QTest::newRow(qPrintable(QLatin1String(caseName))) << secret << counter << expected;
}

void SailOTPHOTPCompatTest::testSteam(void)
{
    const otp::Encoder e([](const QByteArray& t) -> QString
    {
        return otp::encodeSteamGuardToken(t);
    });
    test(e);
}

void SailOTPHOTPCompatTest::testHOTPDefault(void)
{
    const otp::Encoder e = otp::otpEncoder();
    test(e);
}

void SailOTPHOTPCompatTest::testSteam_data(void)
{
    setupDataColumns();
    result("steam, counter = 0", otp::base32::recode(QLatin1String("hello, brave new world!")), 0, QLatin1String("todo")); // 955109
}

void SailOTPHOTPCompatTest::testHOTPDefault_data(void)
{
    setupDataColumns();
    result("hotp, counter = 0", otp::base32::recode(QLatin1String("hello, brave new world!")), 1, QLatin1String("323049"));
}

QTEST_APPLESS_MAIN(SailOTPHOTPCompatTest)

#include "sailotp-hotp-compat.moc"

