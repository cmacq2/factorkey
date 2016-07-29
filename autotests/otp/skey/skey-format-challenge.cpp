
#include "otp/skey/skey.h"

#include <QTest>
#include <QtDebug>

class SKeyFormatChallengeTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testFormatChallengeMd4(void);
    void testFormatChallengeMd4_data(void);
    void testFormatChallengeMd5(void);
    void testFormatChallengeMd5_data(void);
    void testFormatChallengeSha1(void);
    void testFormatChallengeSha1_data(void);
    void testFormatInvalidHashChallenge(void);
    void testFormatInvalidHashChallenge_data(void);
    void testFormatInvalidSeedChallenge(void);
    void testFormatInvalidSeedChallenge_data(void);
private:
    void testFormatChallenge(const QCryptographicHash::Algorithm& h);
    void testInvalidFormatChallenge(const QCryptographicHash::Algorithm& h, const QString& s);
    void setupColumns(void);
};

void SKeyFormatChallengeTest::testInvalidFormatChallenge(const QCryptographicHash::Algorithm& h, const QString& s)
{
    quint64 rounds = 0;
    QString result;
    QVERIFY2(!otp::skey::formatSKeyChallenge(h, s, rounds, result), "Formatting should refuse invalid parameters");
}

void SKeyFormatChallengeTest::testFormatInvalidSeedChallenge(void)
{
    QFETCH(QString, seed);
    testInvalidFormatChallenge(QCryptographicHash::Sha1, seed);
}

void SKeyFormatChallengeTest::testFormatInvalidSeedChallenge_data(void)
{
    QTest::addColumn<QString>("seed");
    QTest::newRow(qPrintable(QLatin1String("seeds must not be empty"))) << QString(QLatin1String(""));
    QTest::newRow(qPrintable(QLatin1String("seeds must not be null values (empty)"))) << QString();
    QTest::newRow(qPrintable(QLatin1String("seeds should be at most 16 characters long"))) << QString(QLatin1String("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
    QTest::newRow(qPrintable(QLatin1String("seeds must not contain spaces"))) << QString(QLatin1String("s e e d"));
    QTest::newRow(qPrintable(QLatin1String("seeds should be within ASCII range"))) << QString(QStringLiteral("mêlée"));
    QTest::newRow(qPrintable(QLatin1String("seeds should not contain non-printable (control) characters"))) << QString(QStringLiteral("control\a\bchars"));
    QTest::newRow(qPrintable(QLatin1String("seeds should not contain other ASCII whitespace characters"))) << QString(QStringLiteral("line\r\nbreaks\ttab"));
    QTest::newRow(qPrintable(QLatin1String("seeds should not contain 'special' ASCII characters"))) << QString(QStringLiteral("$#@%!"));
}

void SKeyFormatChallengeTest::testFormatInvalidHashChallenge(void)
{
    QFETCH(int, hash);
    QString seed(QLatin1String("validseed"));
    testInvalidFormatChallenge((QCryptographicHash::Algorithm) hash, seed);
}

void SKeyFormatChallengeTest::testFormatInvalidHashChallenge_data(void)
{
    QTest::addColumn<int>("hash");
    QTest::newRow(qPrintable(QLatin1String("SHA 2 (224)"))) << (int) QCryptographicHash::Sha224;
    QTest::newRow(qPrintable(QLatin1String("SHA 2 (256)"))) << (int) QCryptographicHash::Sha256;
    QTest::newRow(qPrintable(QLatin1String("SHA 2 (384)"))) << (int) QCryptographicHash::Sha384;
    QTest::newRow(qPrintable(QLatin1String("SHA 2 (512)"))) << (int) QCryptographicHash::Sha512;
    QTest::newRow(qPrintable(QLatin1String("SHA 3 (224)"))) << (int) QCryptographicHash::Sha3_224;
    QTest::newRow(qPrintable(QLatin1String("SHA 3 (256)"))) << (int) QCryptographicHash::Sha3_256;
    QTest::newRow(qPrintable(QLatin1String("SHA 3 (384)"))) << (int) QCryptographicHash::Sha3_384;
    QTest::newRow(qPrintable(QLatin1String("SHA 3 (512)"))) << (int) QCryptographicHash::Sha3_512;
}

void SKeyFormatChallengeTest::setupColumns(void)
{
    QTest::addColumn<QString>("seed");
    QTest::addColumn<quint64>("rounds");
    QTest::addColumn<QString>("expected");
}

static void result(const char * expected, quint64 rounds, const char * seed)
{
    const QString s = QLatin1String(seed);
    const QString e = QLatin1String(expected);
    QTest::newRow(qPrintable(QLatin1String(expected))) << s << rounds << e;
}

void SKeyFormatChallengeTest::testFormatChallenge(const QCryptographicHash::Algorithm& h)
{
    QFETCH(QString, seed);
    QFETCH(quint64, rounds);
    QString result;
    QVERIFY2(otp::skey::formatSKeyChallenge(h, seed, rounds, result), "Formatting the challenge should succeed");
    QTEST(result, "expected");
}

void SKeyFormatChallengeTest::testFormatChallengeMd4(void)
{
    testFormatChallenge(QCryptographicHash::Md4);
}

void SKeyFormatChallengeTest::testFormatChallengeMd4_data(void)
{
    setupColumns();
    result("otp-md4 0 seed", 0, "seed");
    result("otp-md4 1 SomethingElse", 1, "SomethingElse");
}

void SKeyFormatChallengeTest::testFormatChallengeMd5(void)
{
    testFormatChallenge(QCryptographicHash::Md5);
}


void SKeyFormatChallengeTest::testFormatChallengeMd5_data(void)
{
    setupColumns();
    result("otp-md5 0 seed", 0, "seed");
    result("otp-md5 1 SomethingElse", 1, "SomethingElse");
}

void SKeyFormatChallengeTest::testFormatChallengeSha1(void)
{
    testFormatChallenge(QCryptographicHash::Sha1);
}

void SKeyFormatChallengeTest::testFormatChallengeSha1_data(void)
{
    setupColumns();
    result("otp-sha1 0 seed", 0, "seed");
    result("otp-sha1 1 SomethingElse", 1, "SomethingElse");
}

QTEST_APPLESS_MAIN(SKeyFormatChallengeTest)

#include "skey-format-challenge.moc"
