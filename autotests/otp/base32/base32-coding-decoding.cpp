#include "otp/base32/base32.h"

#include <QTest>
#include <QtDebug>

class Base32CodingDecodingTest: public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testSample(void);
    void testSample_data(void);
};

static int determinePadding(int sz)
{
    switch(sz % 5)
    {
        case 1:
            return 6;
        case 2:
            return 4;
        case 3:
            return 3;
        case 4:
            return 1;
        default:
            return 0;
    }
}

void Base32CodingDecodingTest::testSample(void)
{
    QFETCH(QByteArray, message);
    QString encoded = otp::base32::encode(message);
    QCOMPARE(encoded.size() % 8, 0);
    QCOMPARE(encoded.size(), 8 * ((message.size() % 5 ? 1 : 0) + message.size() / 5));

    int plen = determinePadding(message.size());
    QString pad(QLatin1String(""));
    for(int i = 0; i < plen; ++i)
    {
        pad += QLatin1Char('=');
    }
    QCOMPARE(encoded.right(plen), pad);
    QCOMPARE(encoded.lastIndexOf(QLatin1Char('='), plen == 0 ? -1 : -plen), plen == 0 ? -1 : encoded.size() - plen);

    bool ok = false;
    QCOMPARE(otp::base32::decode(encoded, &ok), message);
    QVERIFY2(ok, "Expected decoding to succeed");
}

static void result(const QString& testCase, int len)
{
    QByteArray prefix;
    for(int k = 0; k < (len -1); ++k)
    {
        prefix += (char) k;
    }
    for(int k = 0; k < 256; ++k)
    {
        QByteArray message = prefix;
        message += (char) k;
        QTest::newRow(qPrintable(testCase.arg(message.size()).arg(QString::fromLocal8Bit(message.toHex())))) << message;
    }
}

void Base32CodingDecodingTest::testSample_data(void)
{
    QTest::addColumn<QByteArray>("message");
    QTest::newRow(qPrintable(QLatin1String("empty message"))) << QByteArray();
    result(QLatin1String("size: %1: ... 0x%2"), 1);
    result(QLatin1String("size: %1: ... 0x%2"), 2);
    result(QLatin1String("size: %1: ... 0x%2"), 3);
    result(QLatin1String("size: %1: ... 0x%2"), 4);
    result(QLatin1String("size: %1: ... 0x%2"), 5);
}


QTEST_APPLESS_MAIN(Base32CodingDecodingTest)

#include "base32-coding-decoding.moc"