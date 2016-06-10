
#include "otp/oath/oath.h"

#include <QTest>
#include <QtDebug>

class TOTPCounterTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testSample(void);
    void testSample_data(void);
};

static void result(const char * testCase, qint64 now, quint64 count)
{
    QTest::newRow(qPrintable(QLatin1String(testCase))) << otp::oath::DEFAULT_EPOCH << now << otp::oath::DEFAULT_TIMESTEP_MSEC << count;
}

void TOTPCounterTest::testSample_data(void)
{
    static qint64 corpus[5] {
        0,
        15,
        otp::oath::DEFAULT_TIMESTEP_MSEC - 5,

        otp::oath::DEFAULT_TIMESTEP_MSEC,
        otp::oath::DEFAULT_TIMESTEP_MSEC + 25
    };

    QTest::addColumn<qint64>("epoch");
    QTest::addColumn<qint64>("now");
    QTest::addColumn<quint64>("step");
    QTest::addColumn<quint64>("counter");

    result("at default epoch", corpus[0], 0U);
    result("few ms after default epoch", corpus[1], 0U);
    result("few ms before step after default epoch", corpus[2], 0U);

    result("single step after default epoch", corpus[3], 1U);
    result("few ms + step after default epoch", corpus[4], 1U);

    QTest::newRow(qPrintable(QLatin1String("maximum value"))) << (qint64) 0x8000000000000000 << (qint64) 0x7fffffffffffffff << (quint64) 1U << (quint64) 0xffffffffffffffffU;
}

void TOTPCounterTest::testSample(void)
{
    QFETCH(qint64, epoch);
    QFETCH(qint64, now);
    QFETCH(quint64, step);

    QTEST(otp::oath::countTokens(now, epoch, step), "counter");
}

QTEST_APPLESS_MAIN(TOTPCounterTest)

#include "totp-counter.moc"
