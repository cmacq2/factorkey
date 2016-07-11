#include "storage.h"
#include "storage_p.h"

namespace otp
{
    namespace storage
    {
        const QString Storage::OTP_TOKEN_TYPE              = QLatin1String("otp.token.type");
        const QString Storage::OTP_TOKEN_NAME              = QLatin1String("otp.token.name");
        const QString Storage::OTP_ENCODER_TOKEN_LOCALE    = QLatin1String("otp.encoder.locale");
        const QString Storage::OTP_ENCODER_TOKEN_LENGTH    = QLatin1String("otp.encoder.length");
        const QString Storage::OTP_KEY_ENCODING_CHARSET    = QLatin1String("otp.key.charset");
        const QString Storage::OTP_KEY_ENCODING_TYPE       = QLatin1String("otp.key.type");
        const QString Storage::OTP_HMAC_HASH_ALGORITHM     = QLatin1String("otp.hmac.hash");
        const QString Storage::HOTP_TOKEN_COUNTER          = QLatin1String("hotp.counter");
        const QString Storage::TOTP_TOKEN_EPOCH            = QLatin1String("totp.epoch");
        const QString Storage::TOTP_TOKEN_TIMESTEP         = QLatin1String("totp.timestep");
        const QString Storage::DUMMY_TOKEN_MESSAGE_VALUE   = QLatin1String("dummy.message.value");
        const QString Storage::DUMMY_TOKEN_MESSAGE_CHARSET = QLatin1String("dummy.message.charset");

        Storage::~Storage() {}

        const QString Storage::entryId(void) const
        {
            Q_D(const Storage);
            return d->entryId();
        }

        OTPTokenType Storage::type(void) const
        {
            Q_D(const Storage);
            return d->type();
        }

        bool Storage::writePassword(const QString& secret)
        {
            Q_D(Storage);
            return d->writePassword(secret);
        }

        bool Storage::readTokenType(OTPTokenType& value) const
        {
            Q_D(const Storage);
            return d->readTokenType(value);
        }

        bool Storage::writeTokenType(const OTPTokenType& value)
        {
            Q_D(Storage);
            return d->writeTokenType(value);
        }

        bool Storage::readParam(const QString& param, QVariant& value) const
        {
            Q_D(const Storage);
            return d->readParam(param, value);
        }

        bool Storage::readPassword(QString& secret) const
        {
            Q_D(const Storage);
            return d->readPassword(secret);
        }

        bool Storage::writeParam(const QString& param, const QVariant& value)
        {
            Q_D(Storage);
            return d->writeParam(param, value);
        }

        bool Storage::exists(void) const
        {
            Q_D(const Storage);
            return d->exists();
        }

        bool Storage::commit(void)
        {
            Q_D(Storage);
            return d->commit();
        }

        bool Storage::poll(void)
        {
            Q_D(Storage);
            return d->poll();
        }
    }
}
