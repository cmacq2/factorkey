
#include "otp/skey/skey.h"

#include <QTest>
#include <QtDebug>

class SKeyValidateSeedTest: public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testSample(void);
    void testSample_data(void);
};

void SKeyValidateSeedTest::testSample(void)
{
    QFETCH(QString, seed);
    QTEST((int) otp::skey::validateSKeySeed(seed), "error");
}

static void result(const char * testCase, const QString& seed, otp::skey::SKeySeedError err)
{
    QTest::newRow(qPrintable(QLatin1String(testCase))) << seed << ((int) err);
}

void SKeyValidateSeedTest::testSample_data(void)
{
    QTest::addColumn<QString>("seed");
    QTest::addColumn<int>("error");

    /*
     * Populate with test corpus from RFC 2289
     * See: https://tools.ietf.org/html/rfc2289
     */

    result("Valid seed", QLatin1String("AValidSeed"), otp::skey::SKeySeedError::Conformant);
    result("Seed must not contain any spaces", QLatin1String("A Seed"), otp::skey::SKeySeedError::Invalid);
    result("Seeds MUST not be empty", QLatin1String(""), otp::skey::SKeySeedError::Invalid);
    result("Seeds MUST not be empty", QString(), otp::skey::SKeySeedError::Invalid);
    result("Seeds SHOULD not be between 1 and 16 chars in size (got 17)", QLatin1String("LengthOfSeventeen"), otp::skey::SKeySeedError::Acceptable);
    result("Seed must be purely alphanumeric (but non-alphanumeric chars can be 'accepted')", QLatin1String("Length_Okay"), otp::skey::SKeySeedError::Acceptable);
}

QTEST_APPLESS_MAIN(SKeyValidateSeedTest)

#include "skey-validate-seed.moc"