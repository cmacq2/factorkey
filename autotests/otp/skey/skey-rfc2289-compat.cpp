
#include "otp/skey/skey.h"

#include <QByteArray>
#include <QCryptographicHash>

#include <QTest>
#include <QtDebug>

class SKeyRFC2289CompatTest: public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testMd4Sample(void);
    void testMd4Sample_data(void);
    void testMd5Sample(void);
    void testMd5Sample_data(void);
    void testSha1Sample(void);
    void testSha1Sample_data(void);
private:
    void setupColumns(void);
    void test(const QCryptographicHash::Algorithm& hash);
};

void SKeyRFC2289CompatTest::testMd4Sample(void)
{
    test(QCryptographicHash::Md4);
}

void SKeyRFC2289CompatTest::testMd5Sample(void)
{
    test(QCryptographicHash::Md5);
}

void SKeyRFC2289CompatTest::testSha1Sample(void)
{
    test(QCryptographicHash::Sha1);
}

void SKeyRFC2289CompatTest::test(const QCryptographicHash::Algorithm& hash)
{
    QFETCH(QByteArray, key);
    QFETCH(QString, seed);
    QFETCH(quint64, rounds);
    const QByteArray seedMsg = otp::skey::sKeySeedMessage(seed);
    const QByteArray rawToken = otp::skey::sKey(key, seedMsg, hash, rounds);
    QTEST(otp::skey::sKeyTokenValue(rawToken, hash), "expected");
}

static void result(const char * key, const  char * seed, quint64 rounds, quint64 expected)
{
    const QByteArray k(key);
    const QString s = QLatin1String(seed);
    const QString fmt = QLatin1String("{ key = '%1'; seed = '%2'; rounds = %3 }");
    const QString testCase = fmt.arg(QLatin1String(key)).arg(s).arg(rounds);

    QTest::newRow(qPrintable(testCase)) << k << s << rounds << expected;
}

void SKeyRFC2289CompatTest::setupColumns(void)
{
    QTest::addColumn<QByteArray>("key");
    QTest::addColumn<QString>("seed");
    QTest::addColumn<quint64>("rounds");
    QTest::addColumn<quint64>("expected");
}

void SKeyRFC2289CompatTest::testMd4Sample_data(void)
{
    setupColumns();
    result("This is a test.", "TeSt", 0U, 0xD1854218EBBB0B51U);
    result("This is a test.", "TeSt", 1U, 0x63473EF01CD0B444U);
    result("This is a test.", "TeSt", 99U, 0xC5E612776E6C237AU);

    result("AbCdEfGhIjK", "alpha1", 0U, 0x50076F47EB1ADE4EU);
    result("AbCdEfGhIjK", "alpha1", 1U, 0x65D20D1949B5F7ABU);
    result("AbCdEfGhIjK", "alpha1", 99U, 0xD150C82CCE6F62D1U);

    result("OTP's are good", "correct", 0U, 0x849C79D4F6F55388U);
    result("OTP's are good", "correct", 1U, 0x8C0992FB250847B1U);
    result("OTP's are good", "correct", 99U, 0x3F3BF4B4145FD74BU);
}

void SKeyRFC2289CompatTest::testMd5Sample_data(void)
{
    setupColumns();
    result("A_Valid_Pass_Phrase", "AValidSeed", 99U, 0x85C43EE03857765BU);

    result("This is a test.", "TeSt", 0U, 0x9E876134D90499DDU);
    result("This is a test.", "TeSt", 1U, 0x7965E05436F5029FU);
    result("This is a test.", "TeSt", 99U, 0x50FE1962C4965880U);

    result("AbCdEfGhIjK", "alpha1", 0U, 0x87066DD9644BF206U);
    result("AbCdEfGhIjK", "alpha1", 1U, 0x7CD34C1040ADD14BU);
    result("AbCdEfGhIjK", "alpha1", 99U, 0x5AA37A81F212146CU);

    result("OTP's are good", "correct", 0U, 0xF205753943DE4CF9U);
    result("OTP's are good", "correct", 1U, 0xDDCDAC956F234937U);
    result("OTP's are good", "correct", 99U, 0xB203E28FA525BE47U);
}

void SKeyRFC2289CompatTest::testSha1Sample_data(void)
{
    setupColumns();
    result("This is a test.", "TeSt", 0U, 0xBB9E6AE1979D8FF4U);
    result("This is a test.", "TeSt", 1U, 0x63D936639734385BU);
    result("This is a test.", "TeSt", 99U, 0x87FEC7768B73CCF9U);

    result("AbCdEfGhIjK", "alpha1", 0U, 0xAD85F658EBE383C9U);
    result("AbCdEfGhIjK", "alpha1", 1U, 0xD07CE229B5CF119BU);
    result("AbCdEfGhIjK", "alpha1", 99U, 0x27BC71035AAF3DC6U);

    result("OTP's are good", "correct", 0U, 0xD51F3E99BF8E6F0BU);
    result("OTP's are good", "correct", 1U, 0x82AEB52D943774E4U);
    result("OTP's are good", "correct", 99U, 0x4F296A74FE1567ECU);
}

QTEST_APPLESS_MAIN(SKeyRFC2289CompatTest)

#include "skey-rfc2289-compat.moc"