
#include "otp/oath/oath.h"
#include "otp/oath/token.h"

#include <QTest>
#include <QtDebug>

class DefaultEncoderSamplesTest: public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase(void);
    void testSample(void);
    void testSample_data(void);
private:
    otp::token::Encoder m_uut;
};


static void result(const char* caseName, const QByteArray& token, const QString& raw, QChar zeroDigit, int length = otp::oath::DEFAULT_OTP_LENGTH)
{
    QTest::newRow(qPrintable(QLatin1String(caseName))) << token << length << raw.rightJustified(length, zeroDigit);
}

void DefaultEncoderSamplesTest::testSample_data(void)
{
    static const char corpus[][16] = {
        {},

        { '\x0' },
        { '\x1' },
        { '\xF0' },
        { '\x31' },

        {'\x2', '\x2' },
        {'\x2', '\x0' },
        {'\x2', '\x1' },


        {'\x2', '\x42' },
        {'\x2', '\x70' },
        {'\x2', '\x31' },


        { '\x3', '\x31', '\x1' },
        { '\x3', '\x31', '\x31' },
        { '\x3', '\xff', '\x0' },

        { '\x7f', '\xff', '\xff', '\xff', '\x0' },
        { '\xff', '\xff', '\xff', '\xff', '\x0' }

    };

    QTest::addColumn<QByteArray>("sample");
    QTest::addColumn<int>("length");
    QTest::addColumn<QString>("value");

    QChar zeroDigit = QLocale::c().zeroDigit();

    result("empty token", QByteArray(corpus[0], 0), QString(), zeroDigit);

    result("single byte, as valid offset", QByteArray(corpus[1], 1), QString(), zeroDigit);
    result("single byte, as oob offset", QByteArray(corpus[2], 1), QString(), zeroDigit);
    result("single byte, as valid offset + high order bits", QByteArray(corpus[3], 1), QLatin1String("240"), zeroDigit);
    result("single byte, as oob offset + high order bits", QByteArray(corpus[4], 1), QString(), zeroDigit);

    result("token byte + oob offset", QByteArray(corpus[5], 2), QString(), zeroDigit);
    result("token byte + valid offset", QByteArray(corpus[6], 2), QLatin1String("512"), zeroDigit);
    result("token byte + valid offset to self", QByteArray(corpus[7], 2), QLatin1String("1"), zeroDigit);

    result("token byte + oob offset + high order bits", QByteArray(corpus[8], 2), QString(), zeroDigit);
    result("token byte + valid offset + high order bits", QByteArray(corpus[9], 2), QLatin1String("624"), zeroDigit);
    result("token byte + valid offset to self + high order bits", QByteArray(corpus[10], 2), QLatin1String("49"), zeroDigit);

    result("token bytes + offset mid", QByteArray(corpus[11], 3), QLatin1String("12545"), zeroDigit);
    result("token bytes + offset mid + high order bits", QByteArray(corpus[12], 3), QLatin1String("12593"), zeroDigit);
    result("token bytes + valid offset", QByteArray(corpus[13], 3), QLatin1String("261888"), zeroDigit, 6);

    result("token bytes + valid offset -> max value", QByteArray(corpus[14], 5), QLatin1String("483647"), zeroDigit, 6);
    result("token bytes + valid offset -> max value (high order bit masked)", QByteArray(corpus[15], 5), QLatin1String("483647"), zeroDigit, 6);
}

void DefaultEncoderSamplesTest::initTestCase(void)
{
    m_uut = otp::oath::token::oathEncoder();
}

void DefaultEncoderSamplesTest::testSample(void)
{
    QFETCH(QByteArray, sample);
    QString result = m_uut(sample);
    QTEST(result.size(), "length");
    QTEST(result, "value");
}

QTEST_APPLESS_MAIN(DefaultEncoderSamplesTest)

#include "default-encoder-samples.moc"

