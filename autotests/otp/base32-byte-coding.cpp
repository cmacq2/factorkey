#include "otp/base32.h"

#include <QTest>
#include <QtDebug>

class Base32ByteCodingTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testInvalidEncodeSample(void);
    void testInvalidEncodeSample_data(void);
    void testInvalidDecodeSample(void);
    void testInvalidDecodeSample_data(void);
    void testValidSample(void);
    void testValidSample_data(void);
};


static void result(const QString& testCase, int byte, QChar expected)
{
    QTest::newRow(qPrintable(testCase)) << byte << expected;
}

static void result(const QString& testCase, int byte)
{
    QTest::newRow(qPrintable(testCase.arg(byte, 2, 16, QLatin1Char('0')))) << byte;
}

static void result(const char * testCase, QChar encoded)
{
    QTest::newRow(qPrintable(QLatin1String(testCase))) << encoded;
}

void Base32ByteCodingTest::testInvalidDecodeSample(void)
{
    QFETCH(QChar, encoded);
    QCOMPARE(otp::base32::decodeByte(encoded), -1);
}

void Base32ByteCodingTest::testInvalidDecodeSample_data(void)
{
    QTest::addColumn<QChar>("encoded");
    result("Padding char: '='", QLatin1Char('='));
    result("Space char: ' '", QLatin1Char(' '));
    result("Sentinels in numeric range: '/'", QLatin1Char('/'));
    result("Gap in numeric range: '0'", QLatin1Char('0'));
    result("Gap in numeric range: '1'", QLatin1Char('1'));
    result("Gap in numeric range: '8'", QLatin1Char('8'));
    result("Gap in numeric range: '9'", QLatin1Char('9'));
    result("Sentinels in numeric range: ':'", QLatin1Char(':'));
    result("Sentinels in alpha range: '@'", QLatin1Char('@'));
    result("Sentinels in alpha range: '['", QLatin1Char('['));
    result("Alpha range is uppercase: 'a'", QLatin1Char('a'));
    result("Alpha range is uppercase: 'z'", QLatin1Char('z'));
}

void Base32ByteCodingTest::testInvalidEncodeSample(void)
{
    QFETCH(int, byte);
    QCOMPARE(otp::base32::decodeByte(otp::base32::encodeByte(byte)), -1);
}

void Base32ByteCodingTest::testInvalidEncodeSample_data(void)
{
    QTest::addColumn<int>("byte");
    for(int i = 2; i < 16; ++i)
    {
        result(QLatin1String("Invalid high bits: 0x%1"), (i << 4) | i);
    }
    result(QLatin1String("Not a byte, too large: 0x%1 (256)"), 256);
    result(QLatin1String("Signed values are invalid: 0x%1 (-1)"), -1);
    result(QLatin1String("Signed values are invalid: 0x%1 (-16)"), -16);
}

void Base32ByteCodingTest::testValidSample(void)
{
    QFETCH(int, byte);
    int orig = byte;
    QTEST(otp::base32::encodeByte(byte), "encoded");
    QCOMPARE(otp::base32::decodeByte(otp::base32::encodeByte(byte)), orig);
}

void Base32ByteCodingTest::testValidSample_data(void)
{
    QTest::addColumn<int>("byte");
    QTest::addColumn<QChar>("encoded");
    for(int i = 0; i < 32; ++i)
    {
        QChar c = i >= 26 ? QLatin1Char((char) ('2' + i - 26)) : QLatin1Char((char) ('A' + i));
        result(QStringLiteral("Valid byte: 0x%1, encoded as: '%2'").arg(i, 2, 16, QLatin1Char('0')).arg(c), i, c);
    }
}


QTEST_APPLESS_MAIN(Base32ByteCodingTest)

#include "base32-byte-coding.moc"

