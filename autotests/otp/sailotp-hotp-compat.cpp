
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
    if(secret.endsWith(QLatin1Char('=')))
    {
        QEXPECT_FAIL("", "SailOTP does something odd when padding is involved", Continue);
    }

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
    result("steam, steam_alphabet (1)", QLatin1String("STEAMBCDFGHIJKLNOPQRUVWXYZ234567"), 0x0000000002e7ce2f, QLatin1String("N93YR"));
    result("steam, steam_alphabet (2)", QLatin1String("STEAMBCDFGHIJKLNOPQRUVWXYZ234567"), 0x0000000002e7ce35, QLatin1String("MT57J"));
    result("steam, steam_alphabet (3)", QLatin1String("STEAMBCDFGHIJKLNOPQRUVWXYZ234567"), 0x0000000002e7cea3, QLatin1String("WT25W"));
    result("steam, steam_alphabet (4)", QLatin1String("STEAMBCDFGHIJKLNOPQRUVWXYZ234567"), 0x0000000002e7cea5, QLatin1String("P7G7F"));
    result("steam, steam_alphabet (5)", QLatin1String("STEAMBCDFGHIJKLNOPQRUVWXYZ234567"), 0x0000000002e7cea8, QLatin1String("C4VM7"));
    result("steam, steam_alphabet (6)", QLatin1String("STEAMBCDFGHIJKLNOPQRUVWXYZ234567"), 0x0000000002e7cf43, QLatin1String("56M54"));
    result("steam, steam_alphabet (7)", QLatin1String("STEAMBCDFGHIJKLNOPQRUVWXYZ234567"), 0x0000000002e7cfd5, QLatin1String("H9J7W"));
    result("steam, steam_alphabet (8)", QLatin1String("STEAMBCDFGHIJKLNOPQRUVWXYZ234567"), 0x0000000002e7cfda, QLatin1String("2N8RH"));
    result("steam, steam_alphabet (9)", QLatin1String("STEAMBCDFGHIJKLNOPQRUVWXYZ234567"), 0x0000000002e7cfe1, QLatin1String("GGKCG"));
    result("steam, steam_alphabet (10)", QLatin1String("STEAMBCDFGHIJKLNOPQRUVWXYZ234567"), 0x0000000002e7df67, QLatin1String("CB2M4"));
    result("steam, steam_alphabet (11)", QLatin1String("STEAMBCDFGHIJKLNOPQRUVWXYZ234567"), 0x0000000002e7df6f, QLatin1String("GVBQ4"));
    result("steam, steam_alphabet (12)", QLatin1String("STEAMBCDFGHIJKLNOPQRUVWXYZ234567"), 0x0000000002e7df7f, QLatin1String("MXXD3"));
    result("steam, steam_alphabet (13)", QLatin1String("STEAMBCDFGHIJKLNOPQRUVWXYZ234567"), 0x0000000002e7df87, QLatin1String("KT248"));
    result("steam, steam_alphabet (14)", QLatin1String("STEAMBCDFGHIJKLNOPQRUVWXYZ234567"), 0x0000000002e7dfb0, QLatin1String("39G97"));
}

void SailOTPHOTPCompatTest::testHOTPDefault_data(void)
{
    setupDataColumns();
    result("hotp, 'ABCDEFGHIJKLMNOPQRSTUVWXYZ234567' [0]", QLatin1String("ABCDEFGHIJKLMNOPQRSTUVWXYZ234567"), 0, QLatin1String("081962"));
    result("hotp, 'ABCDEFGHIJKLMNOPQRSTUVWXYZ234567' [1]", QLatin1String("ABCDEFGHIJKLMNOPQRSTUVWXYZ234567"), 1, QLatin1String("408553"));
    result("hotp, 'ABCDEFGHIJKLMNOP' [0]", QLatin1String("ABCDEFGHIJKLMNOP"), 0, QLatin1String("827178"));
    result("hotp, 'ABCDEFGHIJKLMNOP' [1]", QLatin1String("ABCDEFGHIJKLMNOP"), 1, QLatin1String("317963"));
    result("hotp, 'YZ234567QQ======' [0]", QLatin1String("YZ234567QQ======"), 0, QLatin1String("103820"));
    result("hotp, 'YZ234567QQ======' [1]", QLatin1String("YZ234567QQ======"), 1, QLatin1String("947745"));
    result("hotp, 'ZZPLURALZ9ALPHA=' [0]", QLatin1String("ZZPLURALZ9ALPHA="), 0, QLatin1String("556415"));
    result("hotp, 'ZZPLURALZ9ALPHA=' [1]", QLatin1String("ZZPLURALZ9ALPHA="), 1, QLatin1String("687812"));
    result("hotp, 'OAKTREEFIBERS===' [0]", QLatin1String("OAKTREEFIBERS==="), 0, QLatin1String("008223"));
    result("hotp, 'OAKTREEFIBERS===' [1]", QLatin1String("OAKTREEFIBERS==="), 1, QLatin1String("525482"));
    result("hotp, 'QUETZALCOATL====' [0]", QLatin1String("QUETZALCOATL===="), 0, QLatin1String("839139"));
    result("hotp, 'QUETZALCOATL====' [1]", QLatin1String("QUETZALCOATL===="), 1, QLatin1String("871480"));
    result("hotp, counter = 0", otp::base32::recode(QLatin1String("hello, brave new world!")), 0, QLatin1String("009733"));// QLatin1String("323049"));
}

QTEST_APPLESS_MAIN(SailOTPHOTPCompatTest)

#include "sailotp-hotp-compat.moc"

