
#include "otp/skey/skey.h"

#include <QTest>
#include <QtDebug>

class SKeyChallengeParsingTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testValidMd4Samples(void);
    void testValidMd4Samples_data(void);
    void testValidMd5Samples(void);
    void testValidMd5Samples_data(void);
    void testValidSha1Samples(void);
    void testValidSha1Samples_data(void);
    void testInvalidHashSamples(void);
    void testInvalidHashSamples_data(void);
    void testInvalidRoundsSamples(void);
    void testInvalidRoundsSamples_data(void);
    void testInvalidSeedSamples(void);
    void testInvalidSeedSamples_data(void);
private:
    void testValidSamples(const QCryptographicHash::Algorithm& expectedHash);
    void testInvalidSamples(void);
};

void SKeyChallengeParsingTest::testInvalidHashSamples(void)
{
    testInvalidSamples();
}


void SKeyChallengeParsingTest::testInvalidHashSamples_data(void)
{
    QTest::addColumn<QString>("challenge");
    static const char * challenges[]
    {
        "otp-sha256 0 validseed",
        "otp-sha512 1 validseed",
        "otp-foo 2 validseed",
        "foo-md4 3 validseed",
        "bar-md5 4 validseed",
        "baz-sha 5 validseed",
        "OTP-md4 7 validseed",
        "OTP-md5 8 validseed",
        "OTP-sha 9 validseed",
        "otp_md4 10 validseed",
        "otp_md5 11 validseed",
        "otp_sha 12 validseed",
        "otp-sha 13 validseed"
    };
    for(int i = 0; i < 14; ++i)
    {
        const QString challenge = QLatin1String(challenges[i]);
        QTest::newRow(qPrintable(challenge)) << challenge;
    }
}

void SKeyChallengeParsingTest::testInvalidSeedSamples(void)
{
    testInvalidSamples();
}


void SKeyChallengeParsingTest::testInvalidSeedSamples_data(void)
{
    QTest::addColumn<QString>("challenge");
    static const char * challenges[]
    {
        "otp-sha 0 broken seed"
    };
    for(int i = 0; i < 1; ++i)
    {
        const QString challenge = QLatin1String(challenges[i]);
        QTest::newRow(qPrintable(challenge)) << challenge;
    }
}

void SKeyChallengeParsingTest::testInvalidRoundsSamples(void)
{
    testInvalidSamples();
}

void SKeyChallengeParsingTest::testInvalidRoundsSamples_data(void)
{
    QTest::addColumn<QString>("challenge");
    static const char * challenges[]
    {
        "otp-sha1 A validseed0",
        "otp-sha1 - validseed1",
        "otp-sha1 + validseed2",
        "otp-sha1 -1 validseed3",
        "otp-sha1 0x3 validseed4",
        "otp-sha1 3E7 validseed5",
        "otp-sha1 0b1 validseed6",
        "otp-sha1 \\x5 validseed7",
        "otp-sha1 x5 validseed8",
        "otp-sha1 4_5 validseed9",
        "otp-sha1 3.5 validseed10"
    };
    for(int i = 0; i < 11; ++i)
    {
        const QString challenge = QLatin1String(challenges[i]);
        QTest::newRow(qPrintable(challenge)) << challenge;
    }
}

void SKeyChallengeParsingTest::testInvalidSamples(void)
{
    QFETCH(QString, challenge);
    QCryptographicHash::Algorithm algorithm;
    QString seed;
    quint64 rounds;
    QVERIFY2(!otp::skey::parseSKeyChallenge(challenge, algorithm, seed, rounds), "Parsing of invalid challenges should fail");
}

void SKeyChallengeParsingTest::testValidMd5Samples(void)
{
    testValidSamples(QCryptographicHash::Md5);
}

void SKeyChallengeParsingTest::testValidSha1Samples(void)
{
    testValidSamples(QCryptographicHash::Sha1);
}

void SKeyChallengeParsingTest::testValidMd4Samples(void)
{
    testValidSamples(QCryptographicHash::Md4);
}

void SKeyChallengeParsingTest::testValidSamples(const QCryptographicHash::Algorithm& expectedHash)
{
    QFETCH(QString, challenge);
    QFETCH(QString, expectedSeed);
    QFETCH(quint64, expectedRounds);

    QCryptographicHash::Algorithm actualHashAlgorithm;
    QString actualSeed;
    quint64 actualRounds;
    QVERIFY2(otp::skey::parseSKeyChallenge(challenge, actualHashAlgorithm, actualSeed, actualRounds), "Parsing of valid challenges should succeed");
    QCOMPARE(actualHashAlgorithm, expectedHash);
    QCOMPARE(actualRounds, expectedRounds);
    QCOMPARE(actualSeed, expectedSeed);
}

void SKeyChallengeParsingTest::testValidMd4Samples_data(void)
{
    QTest::addColumn<QString>("challenge");
    QTest::addColumn<QString>("expectedSeed");
    QTest::addColumn<quint64>("expectedRounds");
    static const char * challenges[7]
    {
        "otp-md4 0 0rounds",
        "otp-md4 1 seed",
        "otp-md4 2 SEED",
        "otp-md4 3 handle\tbroken",
        "otp-md4 4 !@#$%",
        "otp-md4 5 1",
        "otp-md4 6 HandleOverlyLongSeeds"
    };
    static const char * seeds[7]
    {
        "0rounds", "seed", "SEED", "handle\tbroken", "!@#$%", "1", "HandleOverlyLongSeeds"
    };

    for(int i = 0; i < 7; ++i)
    {
        const QString challenge = QLatin1String(challenges[i]);
        const QString seed =  QLatin1String(seeds[i]);
        QTest::newRow(qPrintable(challenge)) << challenge << seed << ((quint64) i);
    }
}

void SKeyChallengeParsingTest::testValidMd5Samples_data(void)
{
    QTest::addColumn<QString>("challenge");
    QTest::addColumn<QString>("expectedSeed");
    QTest::addColumn<quint64>("expectedRounds");
    static const char * challenges[7]
    {
        "otp-md5 0 0rounds",
        "otp-md5 1 seed",
        "otp-md5 2 SEED",
        "otp-md5 3 handle\tbroken",
        "otp-md5 4 !@#$%",
        "otp-md5 5 1",
        "otp-md5 6 HandleOverlyLongSeeds"
    };
    static const char * seeds[7]
    {
        "0rounds", "seed", "SEED", "handle\tbroken", "!@#$%", "1", "HandleOverlyLongSeeds"
    };

    for(int i = 0; i < 7; ++i)
    {
        const QString challenge = QLatin1String(challenges[i]);
        const QString seed =  QLatin1String(seeds[i]);
        QTest::newRow(qPrintable(challenge)) << challenge << seed << ((quint64) i);
    }
}

void SKeyChallengeParsingTest::testValidSha1Samples_data(void)
{
    QTest::addColumn<QString>("challenge");
    QTest::addColumn<QString>("expectedSeed");
    QTest::addColumn<quint64>("expectedRounds");
    static const char * challenges[7]
    {
        "otp-sha1 0 0rounds",
        "otp-sha1 1 seed",
        "otp-sha1 2 SEED",
        "otp-sha1 3 handle\tbroken",
        "otp-sha1 4 !@#$%",
        "otp-sha1 5 1",
        "otp-sha1 6 HandleOverlyLongSeeds"
    };
    static const char * seeds[7]
    {
        "0rounds", "seed", "SEED", "handle\tbroken", "!@#$%", "1", "HandleOverlyLongSeeds"
    };

    for(int i = 0; i < 7; ++i)
    {
        const QString challenge = QLatin1String(challenges[i]);
        const QString seed =  QLatin1String(seeds[i]);
        QTest::newRow(qPrintable(challenge)) << challenge << seed << ((quint64) i);
    }
}


QTEST_APPLESS_MAIN(SKeyChallengeParsingTest)

#include "skey-parse-challenge.moc"
