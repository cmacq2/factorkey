
#include "otp/skey/generator.h"
#include "autotests/otp/storage/storage.h"

#include <QTest>
#include <QtDebug>

class SKeyStoragePrivate: public stubs::storage::DummyStoragePrivate
{
public:
    SKeyStoragePrivate(const QString& entryId, const QString& password, const QMap<QString,QVariant>& preset, QObject * parent = nullptr):
    stubs::storage::DummyStoragePrivate(entryId, otp::storage::OTPTokenType::SKey, password, preset, parent) {}
    SKeyStoragePrivate(const QString& entryId, const QString& password, QObject * parent = nullptr):
    stubs::storage::DummyStoragePrivate(entryId, otp::storage::OTPTokenType::SKey, password, parent) {}
    SKeyStoragePrivate(const QString& password, const QMap<QString,QVariant>& preset, QObject * parent = nullptr):
    stubs::storage::DummyStoragePrivate(otp::storage::OTPTokenType::SKey, password, preset, parent) {}
    SKeyStoragePrivate(const QString& password, QObject * parent = nullptr):
    stubs::storage::DummyStoragePrivate(otp::storage::OTPTokenType::SKey, password, parent) {}
    SKeyStoragePrivate(QObject * parent = nullptr):
    stubs::storage::DummyStoragePrivate(otp::storage::OTPTokenType::SKey, QString(), parent) {}
};

class SKeyGeneratorSamplesTest: public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testHexSamples(void);
    void testHexSamples_data(void);
    void testWordsSamples(void);
    void testWordsSamples_data(void);
private:
    void setupColumns(void);
    void testGenerator(otp::skey::generator::SKeyEncodingType tokenFormat, const QVariant& dictionaryValue = QVariant());
};

void SKeyGeneratorSamplesTest::testGenerator(otp::skey::generator::SKeyEncodingType tokenFormat, const QVariant& dictionaryValue)
{
    QFETCH(QString, secret);
    QFETCH(QString, challenge);

    QMap<QString, QVariant> map;
    map.insert(otp::generator::TokenParameters::OTP_KEY_ENCODING_TYPE, (int) otp::generator::EncodingType::Text);
    map.insert(otp::generator::TokenParameters::OTP_KEY_ENCODING_CHARSET, QVariant());
    map.insert(otp::skey::generator::SKeyTokenParameters::OTP_SKEY_DICTIONARY_NAME, QVariant());
    map.insert(otp::skey::generator::SKeyTokenParameters::OTP_SKEY_ENCODING_TYPE, (int) tokenFormat);

    auto stub = new SKeyStoragePrivate(secret, map);
    auto storage = new otp::storage::Storage(stub);
    auto params = otp::skey::generator::SKeyTokenParameters::create(storage);
    auto generator = otp::skey::generator::SKeyTokenParameters::generator(params);

    QString token;
    QVERIFY2(params->setChallenge(challenge), "The challenge should be accepted");
    QVERIFY2(generator->generateToken(token), "Generating the token should succeed");
    QTEST(token, "rfc-test-vector");

    QList<enum otp::storage::OTPTokenType> typeResult;
    QList<QList<QVariant>> paramReads;
    stubs::storage::expect_param(paramReads, true, otp::generator::TokenParameters::OTP_KEY_ENCODING_TYPE, (int) otp::generator::EncodingType::Text);
    stubs::storage::expect_param(paramReads, true, otp::generator::TokenParameters::OTP_KEY_ENCODING_CHARSET, QVariant());
    stubs::storage::expect_param(paramReads, true, otp::skey::generator::SKeyTokenParameters::OTP_SKEY_ENCODING_TYPE, (int) tokenFormat);
    if(tokenFormat == otp::skey::generator::SKeyEncodingType::Words)
    {
        stubs::storage::expect_param(paramReads, true, otp::skey::generator::SKeyTokenParameters::OTP_SKEY_DICTIONARY_NAME, dictionaryValue);
    }

    stub->check_read_param(paramReads);
    stub->check_no_write_param();
    stub->check_type(typeResult << otp::storage::OTPTokenType::SKey);
    stub->check_no_read_token_type();
    stub->check_no_write_token_type();
    stub->check_no_write_password();
    stub->check_no_poll();
    stub->check_no_exists();
    stub->check_no_commit();

    generator->deleteLater();
    params->deleteLater();
    storage->deleteLater();
    stub->deleteLater();
}

static void result(const QString& challenge, const QString& secret, const QString& rfcTestVector)
{
    QString fmt(QStringLiteral("challenge='%1'; secret='%2'"));
    QTest::newRow(qPrintable(fmt.arg(challenge, secret))) << secret << challenge << rfcTestVector;
}
static void result(const char * challenge, const char * secret, const char * rfcTestVector)
{
    result(QLatin1String(challenge), QLatin1String(secret), QLatin1String(rfcTestVector));
}

void SKeyGeneratorSamplesTest::setupColumns(void)
{
    QTest::addColumn<QString>("secret");
    QTest::addColumn<QString>("challenge");
    QTest::addColumn<QString>("rfc-test-vector");
}

void SKeyGeneratorSamplesTest::testHexSamples(void)
{
    testGenerator(otp::skey::generator::SKeyEncodingType::Hex);
}

void SKeyGeneratorSamplesTest::testHexSamples_data(void)
{
    setupColumns();

    // MD4 samples
    result("otp-md4 0 TeSt", "This is a test.", "D1854218EBBB0B51");
    result("otp-md4 1 TeSt", "This is a test.", "63473EF01CD0B444");
    result("otp-md4 99 TeSt", "This is a test.", "C5E612776E6C237A");

    result("otp-md4 0 alpha1", "AbCdEfGhIjK", "50076F47EB1ADE4E");
    result("otp-md4 1 alpha1", "AbCdEfGhIjK", "65D20D1949B5F7AB");
    result("otp-md4 99 alpha1", "AbCdEfGhIjK", "D150C82CCE6F62D1");

    result("otp-md4 0 correct", "OTP's are good", "849C79D4F6F55388");
    result("otp-md4 1 correct", "OTP's are good", "8C0992FB250847B1");
    result("otp-md4 99 correct", "OTP's are good", "3F3BF4B4145FD74B");

    // MD5 samples
    result("otp-md5 99 AValidSeed", "A_Valid_Pass_Phrase", "85C43EE03857765B");

    result("otp-md5 0 TeSt", "This is a test.", "9E876134D90499DD");
    result("otp-md5 1 TeSt", "This is a test.", "7965E05436F5029F");
    result("otp-md5 99 TeSt", "This is a test.", "50FE1962C4965880");

    result("otp-md5 0 alpha1", "AbCdEfGhIjK", "87066DD9644BF206");
    result("otp-md5 1 alpha1", "AbCdEfGhIjK", "7CD34C1040ADD14B");
    result("otp-md5 99 alpha1", "AbCdEfGhIjK", "5AA37A81F212146C");

    result("otp-md5 0 correct", "OTP's are good", "F205753943DE4CF9");
    result("otp-md5 1 correct", "OTP's are good", "DDCDAC956F234937");
    result("otp-md5 99 correct", "OTP's are good", "B203E28FA525BE47");

    // SHA1 samples

    result("otp-sha1 0 TeSt", "This is a test.","BB9E6AE1979D8FF4");
    result("otp-sha1 1 TeSt", "This is a test.","63D936639734385B");
    result("otp-sha1 99 TeSt", "This is a test.", "87FEC7768B73CCF9");

    result("otp-sha1 0 alpha1", "AbCdEfGhIjK", "AD85F658EBE383C9");
    result("otp-sha1 1 alpha1", "AbCdEfGhIjK", "D07CE229B5CF119B");
    result("otp-sha1 99 alpha1", "AbCdEfGhIjK", "27BC71035AAF3DC6");

    result("otp-sha1 0 correct", "OTP's are good", "D51F3E99BF8E6F0B");
    result("otp-sha1 1 correct", "OTP's are good", "82AEB52D943774E4");
    result("otp-sha1 99 correct", "OTP's are good", "4F296A74FE1567EC");
}

void SKeyGeneratorSamplesTest::testWordsSamples(void)
{
    testGenerator(otp::skey::generator::SKeyEncodingType::Words);
}

void SKeyGeneratorSamplesTest::testWordsSamples_data(void)
{
    setupColumns();

    result("otp-md4 0 TeSt", "This is a test.", "ROME MUG FRED SCAN LIVE LACE");
    result("otp-md4 1 TeSt", "This is a test.", "CARD SAD MINI RYE COL KIN");
    result("otp-md4 99 TeSt", "This is a test.", "NOTE OUT IBIS SINK NAVE MODE");

    result("otp-md4 0 alpha1", "AbCdEfGhIjK", "AWAY SEN ROOK SALT LICE MAP");
    result("otp-md4 1 alpha1", "AbCdEfGhIjK", "CHEW GRIM WU HANG BUCK SAID");
    result("otp-md4 99 alpha1", "AbCdEfGhIjK", "ROIL FREE COG HUNK WAIT COCA");

    result("otp-md4 0 correct", "OTP's are good", "FOOL STEM DONE TOOL BECK NILE");
    result("otp-md4 1 correct", "OTP's are good", "GIST AMOS MOOT AIDS FOOD SEEM");
    result("otp-md4 99 correct", "OTP's are good", "TAG SLOW NOV MIN WOOL KENO");

    result("otp-md5 0 TeSt", "This is a test.", "INCH SEA ANNE LONG AHEM TOUR");
    result("otp-md5 1 TeSt", "This is a test.", "EASE OIL FUM CURE AWRY AVIS");
    result("otp-md5 99 TeSt", "This is a test.", "BAIL TUFT BITS GANG CHEF THY");

    result("otp-md5 0 alpha1", "AbCdEfGhIjK", "FULL PEW DOWN ONCE MORT ARC");
    result("otp-md5 1 alpha1", "AbCdEfGhIjK", "FACT HOOF AT FIST SITE KENT");
    result("otp-md5 99 alpha1", "AbCdEfGhIjK", "BODE HOP JAKE STOW JUT RAP");

    result("otp-md5 0 correct", "OTP's are good", "ULAN NEW ARMY FUSE SUIT EYED");
    result("otp-md5 1 correct", "OTP's are good", "SKIM CULT LOB SLAM POE HOWL");
    result("otp-md5 99 correct", "OTP's are good", "LONG IVY JULY AJAR BOND LEE");

    result("otp-sha1 0 TeSt", "This is a test.", "MILT VARY MAST OK SEES WENT");
    result("otp-sha1 1 TeSt", "This is a test.", "CART OTTO HIVE ODE VAT NUT");
    result("otp-sha1 99 TeSt", "This is a test.", "GAFF WAIT SKID GIG SKY EYED");

    result("otp-sha1 0 alpha1", "AbCdEfGhIjK", "LEST OR HEEL SCOT ROB SUIT");
    result("otp-sha1 1 alpha1", "AbCdEfGhIjK", "RITE TAKE GELD COST TUNE RECK");
    result("otp-sha1 99 alpha1", "AbCdEfGhIjK", "MAY STAR TIN LYON VEDA STAN");

    result("otp-sha1 0 correct", "OTP's are good", "RUST WELT KICK FELL TAIL FRAU");
    result("otp-sha1 1 correct", "OTP's are good", "FLIT DOSE ALSO MEW DRUM DEFY");
    result("otp-sha1 99 correct", "OTP's are good", "AURA ALOE HURL WING BERG WAIT");
}

QTEST_APPLESS_MAIN(SKeyGeneratorSamplesTest)

#include "skey-generator-samples.moc"
