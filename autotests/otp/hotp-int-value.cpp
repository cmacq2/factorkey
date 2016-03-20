
#include "otp/token.h"

#include <QTest>
#include <QtDebug>

class HOTPIntValueTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testSample(void);
    void testSample_data(void);
};

void HOTPIntValueTest::testSample_data(void)
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

    QTest::addColumn<QByteArray>("token");
    QTest::addColumn<quint32>("tokenValue");

    QTest::newRow(qPrintable(QLatin1String("empty token"))) << QByteArray(corpus[0], 0) << 0x00000000U;

    QTest::newRow(qPrintable(QLatin1String("single byte, as valid offset"))) << QByteArray(corpus[1], 1) << 0x00000000U;
    QTest::newRow(qPrintable(QLatin1String("single byte, as oob offset"))) << QByteArray(corpus[2], 1) << 0x00000000U;
    QTest::newRow(qPrintable(QLatin1String("single byte, as valid offset + high order bits"))) << QByteArray(corpus[3], 1) << 0x000000F0U;
    QTest::newRow(qPrintable(QLatin1String("single byte, as oob offset + high order bits"))) << QByteArray(corpus[4], 1) << 0x00000000U;

    QTest::newRow(qPrintable(QLatin1String("token byte + oob offset"))) << QByteArray(corpus[5], 2) << 0x00000000U;
    QTest::newRow(qPrintable(QLatin1String("token byte + valid offset"))) << QByteArray(corpus[6], 2) << 0x00000200U;
    QTest::newRow(qPrintable(QLatin1String("token byte + valid offset to self"))) << QByteArray(corpus[7], 2) << 0x00000001U;


    QTest::newRow(qPrintable(QLatin1String("token byte + oob offset + high order bits"))) << QByteArray(corpus[8], 2) << 0x00000000U;
    QTest::newRow(qPrintable(QLatin1String("token byte + valid offset + high order bits"))) << QByteArray(corpus[9], 2) << 0x00000270U;
    QTest::newRow(qPrintable(QLatin1String("token byte + valid offset to self + high order bits"))) << QByteArray(corpus[10], 2) << 0x00000031U;

    QTest::newRow(qPrintable(QLatin1String("token bytes + offset mid"))) << QByteArray(corpus[11], 3) << 0x00003101U;
    QTest::newRow(qPrintable(QLatin1String("token bytes + offset mid + high order bits"))) << QByteArray(corpus[12], 3) << 0x00003131U;
    QTest::newRow(qPrintable(QLatin1String("token bytes + valid offset"))) << QByteArray(corpus[13], 3) << 0x0003ff00U;

    QTest::newRow(qPrintable(QLatin1String("token bytes + valid offset -> max value"))) << QByteArray(corpus[14], 5) << 0x7fffffffU;
    QTest::newRow(qPrintable(QLatin1String("token bytes + valid offset -> max value (high order bit masked)"))) << QByteArray(corpus[15], 5) << 0x7fffffffU;
}

void HOTPIntValueTest::testSample(void)
{
    QFETCH(QByteArray, token);
    QTEST(otp::hotpTokenValue(token), "tokenValue");
}

QTEST_APPLESS_MAIN(HOTPIntValueTest)

#include "hotp-int-value.moc"
