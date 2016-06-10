
#include "otp/base32/base32.h"
#include "otp/oath/oath.h"
#include "otp/oath/token.h"

#include <QTest>
#include <QtDebug>

class RFC4226TestVectorCompatTest: public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase(void);
    void testSample(void);
    void testSample_data(void);
private:
    otp::token::Key m_key;
    otp::token::Algorithm m_algo;
    otp::token::Encoder m_encoder;
};

void RFC4226TestVectorCompatTest::initTestCase(void)
{
    m_key = otp::oath::keyForAuthenticator();
    m_algo = otp::oath::hmacAlgorithm();
    m_encoder = otp::oath::oathEncoder();
}

void RFC4226TestVectorCompatTest::testSample(void)
{

    QFETCH(QString, secret);
    QFETCH(quint64, counter);
    const otp::token::Message msg([counter](void) -> QByteArray
    {
        return otp::oath::hotpTokenMessage(counter);
    });


    QTEST(otp::token::token(otp::base32::recode(secret), msg, m_key, m_algo, m_encoder), "rfc-test-vector");
}

static void result(int k, const QString& secret, const char * expected)
{

    QByteArray output(expected, 6);

    QTest::newRow(qPrintable(QStringLiteral("RFC 4226 test vector, counter value = %1").arg(k))) << secret << (quint64) k << QString::fromLocal8Bit(output);
}


void RFC4226TestVectorCompatTest::testSample_data(void)
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
        result(k, secret, corpus[k]);
    }
}

QTEST_APPLESS_MAIN(RFC4226TestVectorCompatTest)

#include "rfc4226-hotp-compat.moc"
