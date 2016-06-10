
#include "otp/skey/skey.h"

#include <QTest>
#include <QtDebug>

class SKeyTokenValueTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testValidMd4Sample(void);
    void testValidMd4Sample_data(void);
    void testValidMd5Sample(void);
    void testValidMd5Sample_data(void);
    void testValidSha1Sample(void);
    void testValidSha1Sample_data(void);
private:
    void setupValidColumns(void);
    void testValid(const QCryptographicHash::Algorithm& hash);
};

void SKeyTokenValueTest::setupValidColumns(void)
{
    QTest::addColumn<QByteArray>("token");
    QTest::addColumn<quint64>("expected");
}

void SKeyTokenValueTest::testValid(const QCryptographicHash::Algorithm& hash)
{
    QFETCH(QByteArray, token);
    QTEST(otp::skey::sKeyTokenValue(token, hash), "expected");
}

void SKeyTokenValueTest::testValidMd5Sample(void)
{
    testValid(QCryptographicHash::Md5);
}

void SKeyTokenValueTest::testValidMd4Sample(void)
{
    testValid(QCryptographicHash::Md4);
}

void SKeyTokenValueTest::testValidSha1Sample(void)
{
    testValid(QCryptographicHash::Sha1);
}

static QByteArray prepareCleanToken(int length)
{
    QByteArray a;
    a.reserve(length);
    for(int i = 0; i < length; ++i)
    {
        a += '\0';
    }
    return a;
}

static void result(const char * data, const QByteArray& token, quint64 expected)
{
    QTest::newRow(qPrintable(QLatin1String(data))) << token << expected;
}

void SKeyTokenValueTest::testValidMd5Sample_data(void)
{
    setupValidColumns();
    QByteArray a(prepareCleanToken(16));
    char * data;

    result("test NULL case", a, 0U);

    /*
     * Output of MD4/MD5 hash is assumed to be 'little' endian'.
     * By keeping the last 64 bits 'clean', i.e. 0, the effect should be
     * that final output should match the first 64 bits
     * when it is interpreted as a LE 64 bit unsigned integer.
     */
    a = prepareCleanToken(16);
    data = a.data();
    data[7] = '\xA';
    data[6] = '\xD';
    data[5] = '\x9';
    data[4] = '\x30';
    data[3] = '\x48';
    data[2] = '\x38';
    data[1] = '\x49';
    data[0] = '\x50';
    result("tokens are interpreted as 64 bit big endian", a, 0x5049384830090D0AU);

    /*
     * Adding the fact that the high bits are folded over into the low by xoring,
     * swapping the order of "clean" and "marked" bits should yield the same result.
     */
    a = prepareCleanToken(16);
    data = a.data();
    data[15] = '\xA';
    data[14] = '\xD';
    data[13] = '\x9';
    data[12] = '\x30';
    data[11] = '\x48';
    data[10] = '\x38';
    data[9] = '\x49';
    data[8] = '\x50';
    result("hi bits are folded over (xor)", a, 0x5049384830090D0AU);

    /*
     * Equal lo & hi bits should cancel each other out.
     */
    a = prepareCleanToken(16);
    data = a.data();
    data[7] = '\xA';
    data[6] = '\xD';
    data[5] = '\x9';
    data[4] = '\x30';
    data[3] = '\x48';
    data[2] = '\x38';
    data[1] = '\x49';
    data[0] = '\x50';
    data[15] = '\xA';
    data[14] = '\xD';
    data[13] = '\x9';
    data[12] = '\x30';
    data[11] = '\x48';
    data[10] = '\x38';
    data[9] = '\x49';
    data[8] = '\x50';
    result("xoring equal bits cancels out", a, 0);

    a = prepareCleanToken(16);
    data = a.data();
    data[7] = '\xA';
    data[6] = '\xD';
    data[5] = '\x9';
    data[4] = '\x30';
    data[3] = '\x48';
    data[2] = '\x38';
    data[1] = '\x49';
    data[0] = '\xFF';
    result("token values are unsigned", a, 0xFF49384830090D0AU);

}

void SKeyTokenValueTest::testValidMd4Sample_data(void)
{
    /* as
     * MD4 tokens 'look the same' as MD5
     * and truncation to 64 bits S/Key values is also defined identically for both algorithms
     * in RFC 2289 (Appendix A).
     * Therefor identical token values should resolve to the same 64 bit S/Key value, and the
     * MD 5 test corpus should apply equally well to the MD 4 case.
     */
    testValidMd5Sample_data();
}

void SKeyTokenValueTest::testValidSha1Sample_data(void)
{
    setupValidColumns();
    QByteArray a(prepareCleanToken(20));
    char * data;

    result("test NULL case", a, 0U);

    /*
     * Output of SHA-1 hash is not 'little' endian', however the folding/truncation
     * step is expected to swap the byte order.
     * By keeping the last 96 bits 'clean', i.e. 0, the effect should be
     * that final output should match the first 64 bits
     * when it is interpreted as a LE 64 bit unsigned integer after byte swapping.
     */
    a = prepareCleanToken(20);
    data = a.data();
    data[4] = '\xA';
    data[5] = '\xD';
    data[6] = '\x9';
    data[7] = '\x30';
    data[0] = '\x48';
    data[1] = '\x38';
    data[2] = '\x49';
    data[3] = '\x50';
    result("byte swapping conversion applies; token is effectively little endian", a, 0x5049384830090D0AU);


    /*
     * Adding the fact that the middle bits are folded over into the low by xoring,
     * swapping the order of "clean" and "marked" bits should yield the same result.
     */
    a = prepareCleanToken(20);
    data = a.data();
    data[12] = '\xA';
    data[13] = '\xD';
    data[14] = '\x9';
    data[15] = '\x30';
    data[8] = '\x48';
    data[9] = '\x38';
    data[10] = '\x49';
    data[11] = '\x50';
    result("mid bits are folded over (xor)", a, 0x5049384830090D0AU);


    /*
     * Hi bits are folded similarly to the mid bits,
     * except that the uppermost 32 bits are folded over to the lo bits.
     * Effectively the high 64 bits are split into 2 x 32 bits with order inverted.
     */
    a = prepareCleanToken(20);
    data = a.data();
    data[16] = '\x48';
    data[17] = '\x38';
    data[18] = '\x49';
    data[19] = '\x50';
    data[12] = '\xA';
    data[13] = '\xD';
    data[14] = '\x9';
    data[15] = '\x30';
    result("hi bits are folded over (xor); effectively split as 2 x 32 bits", a, 0x5049384830090D0AU);

    /*
     * Equal lo & mid bits should cancel each other out.
     */
    a = prepareCleanToken(20);
    data = a.data();
    data[15] = '\xA';
    data[14] = '\xD';
    data[13] = '\x9';
    data[12] = '\x30';
    data[11] = '\x48';
    data[10] = '\x38';
    data[9] = '\x49';
    data[8] = '\x50';
    data[7] = '\xA';
    data[6] = '\xD';
    data[5] = '\x9';
    data[4] = '\x30';
    data[3] = '\x48';
    data[2] = '\x38';
    data[1] = '\x49';
    data[0] = '\x50';
    result("xoring equal bits cancels out (lo, mid)", a, 0);

    a = prepareCleanToken(20);
    data = a.data();
    data[7] = '\xA';
    data[6] = '\xD';
    data[5] = '\x9';
    data[4] = '\x30';
    data[3] = '\x48';
    data[2] = '\x38';
    data[1] = '\x49';
    data[0] = '\x50';
    data[19] = '\x48';
    data[18] = '\x38';
    data[17] = '\x49';
    data[16] = '\x50';
    data[15] = '\xA';
    data[14] = '\xD';
    data[13] = '\x9';
    data[12] = '\x30';
    result("xoring equal bits cancels out (lo, hi), taking 'splitting' of hi bits into account", a, 0U);

    a = prepareCleanToken(20);
    data = a.data();
    data[7] = '\xA';
    data[6] = '\xD';
    data[5] = '\x9';
    data[4] = '\x30';

    data[11] = '\x48';
    data[10] = '\x38';
    data[9] = '\x49';
    data[8] = '\x50';

    data[15] = '\xA';
    data[14] = '\xD';
    data[13] = '\x9';
    data[12] = '\x30';

    data[19] = '\x48';
    data[18] = '\x38';
    data[17] = '\x49';
    data[16] = '\x50';
    result("xoring equal bits cancels out (mid, hi), taking 'splitting' of hi bits into account", a, 0U);


    a = prepareCleanToken(20);
    data = a.data();

    data[11] = '\xA';
    data[10] = '\xD';
    data[9] = '\x9';
    data[8] = '\x30';

    data[3] = '\xA';
    data[2] = '\xD';
    data[1] = '\x9';
    data[0] = '\x30';

    data[19] = '\xA';
    data[18] = '\xD';
    data[17] = '\x9';
    data[16] = '\x30';

    data[7] = '\x48';
    data[6] = '\x38';
    data[5] = '\x49';
    data[4] = '\x50';

    data[15] = '\x48';
    data[14] = '\x38';
    data[13] = '\x49';
    data[12] = '\x50';
    result("xoring equal bits cancels out (lo, mid, hi), taking 'splitting' of hi bits into account", a, 0x0A0D093000000000U);
}


QTEST_APPLESS_MAIN(SKeyTokenValueTest)

#include "skey-token-value.moc"