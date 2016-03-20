#ifndef FACTORKEY_STORAGE_H
#define FACTORKEY_STORAGE_H

#include <QObject>
#include <QScopedPointer>
#include <QString>
#include <QVariant>

namespace otp
{

    enum class OTPTokenType: int
    {
        HOTP = 0,
        TOTP = 1,
        SteamGuard = 2,
        DummyHMAC = 3
    };

    class StoragePrivate;

    class Storage: public QObject
    {
    public:
        Q_OBJECT
    public:
        static const QString OTP_TOKEN_TYPE;
        static const QString OTP_TOKEN_NAME;
        static const QString OTP_ENCODER_TOKEN_LOCALE;
        static const QString OTP_ENCODER_TOKEN_LENGTH;
        static const QString OTP_KEY_ENCODING_CHARSET;
        static const QString OTP_HMAC_HASH_ALGORITHM;
        static const QString HOTP_TOKEN_COUNTER;
        static const QString TOTP_TOKEN_EPOCH;
        static const QString TOTP_TOKEN_TIMESTEP;
        static const QString DUMMY_TOKEN_MESSAGE_VALUE;
        static const QString DUMMY_TOKEN_MESSAGE_CHARSET;
    public:
        const QString entryId(void) const;
        OTPTokenType type(void) const;
        bool readPassword(QString& secret) const;
        bool writePassword(const QString& secret);
        bool readTokenType(OTPTokenType& value) const;
        bool writeTokenType(const OTPTokenType& value);
        bool readParam(const QString& param, QVariant& value) const;
        bool writeParam(const QString& param, const QVariant& value);

        bool exists(void) const;
        bool commit(void);
        bool poll(void);

        ~Storage();
    private:
        Q_DISABLE_COPY(Storage)
        const QScopedPointer<StoragePrivate> d_ptr;
        Q_DECLARE_PRIVATE(Storage)
        Storage(StoragePrivate * d, QObject * parent = 0);
    };

}
#endif
