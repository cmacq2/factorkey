#include "otp/base32.h"

#include <QTest>
#include <QtDebug>

class Base32EncodingCompatibilityTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testSample(void);
    void testSample_data(void);
};

void Base32EncodingCompatibilityTest::testSample(void)
{
    QFETCH(QByteArray, message);
    QTEST(otp::base32::encode(message), "base32");

    bool ok = false;
    QCOMPARE(otp::base32::decode(otp::base32::encode(message), &ok), message);
    QVERIFY2(ok, "Expected decoding to succeed.");
}

static void result(const char * testCase, const char * data, int length, const QString& expected)
{
    QTest::newRow(qPrintable(QLatin1String(testCase))) << QByteArray(data, length) << expected;
}

void Base32EncodingCompatibilityTest::testSample_data(void)
{
    static const char corpus[8][5] = {
        { 'A', 'B', 'C', 'D', '\xA' },
        { '?', 'A', 'B', 'C', 'D' },
        { '2', '0', '1', '6' },
        { '\x0', '\x1', '\x2' },
        { '=', '=' },
        { '?' },
        { '8' },
        {}
    };

    QTest::addColumn<QByteArray>("message");
    QTest::addColumn<QString>("base32");

    result("'ABCD\\n'", corpus[0], 5, QLatin1String("IFBEGRAK"));
    result("'?ABCD'", corpus[1], 5, QLatin1String("H5AUEQ2E"));
    result("'2016'", corpus[2], 4, QLatin1String("GIYDCNQ="));
    result("'\\x1\\x2\\x3'", corpus[3], 3, QLatin1String("AAAQE==="));
    result("'=='", corpus[4], 2, QLatin1String("HU6Q===="));
    result("'?'", corpus[5], 1, QLatin1String("H4======"));
    result("'8'", corpus[6], 1, QLatin1String("HA======"));
    result("''", corpus[7], 0, QLatin1String(""));
}

QTEST_APPLESS_MAIN(Base32EncodingCompatibilityTest)

#include "base32-encoding-compat.moc"
