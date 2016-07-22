#include "generator.h"
#include "../generator_p.h"
#include "oath.h"
#include "token.h"

namespace otp
{
    namespace oath
    {
        namespace generator
        {
            GenericOTPParameters::GenericOTPParameters(otp::generator::TokenParametersPrivate * d, QObject * parent) : otp::generator::GenericTokenParameters(d, parent) {}
            GenericOTPParameters::~GenericOTPParameters() {}

            bool GenericOTPParameters::tokenLocale(QLocale& locale) const
            {
                Q_D(const otp::generator::TokenParameters);
                QVariant val;
                if(d->lookup(otp::storage::Storage::OTP_ENCODER_TOKEN_LOCALE, val))
                {
                    if(val.isNull())
                    {
                        locale = QLocale::c();
                        return true;
                    }
                    else
                    {
                        const QString str = val.toString();
                        QLocale l;
                        if(otp::generator::internal::stringToLocale(str, l))
                        {
                            locale = l;
                            return true;
                        }
                    }
                }
                return false;
            }

            bool GenericOTPParameters::setTokenLocale(const QLocale& locale)
            {
                Q_D(otp::generator::TokenParameters);
                d->storage()->writeParam(otp::storage::Storage::OTP_ENCODER_TOKEN_LOCALE, QVariant(otp::generator::internal::localeToString(locale)));
                return true;
            }

            bool GenericOTPParameters::tokenLength(uint& length) const
            {
                Q_D(const otp::generator::TokenParameters);
                return otp::generator::internal::lookupNumericValue<uint>(d, otp::storage::Storage::OTP_ENCODER_TOKEN_LENGTH, length, otp::oath::DEFAULT_OTP_LENGTH);
            }

            bool GenericOTPParameters::setTokenLength(uint length)
            {
                Q_D(otp::generator::TokenParameters);
                return d->storage()->writeParam(otp::storage::Storage::OTP_ENCODER_TOKEN_LENGTH, QVariant(length));
            }

            HOTPTokenParameters::HOTPTokenParameters(otp::generator::TokenParametersPrivate * d, QObject * parent) : GenericOTPParameters(d, parent) {}
            HOTPTokenParameters::~HOTPTokenParameters() {}
            HOTPTokenParameters * HOTPTokenParameters::create(otp::storage::Storage * store, QObject * parent)
            {
                return store && store->type() == otp::storage::OTPTokenType::HOTP ? new HOTPTokenParameters(new otp::generator::TokenParametersPrivate(store), parent) : nullptr;
            }

            bool HOTPTokenParameters::tokenCounter(quint64 & count) const
            {
                Q_D(const otp::generator::TokenParameters);
                return otp::generator::internal::lookupNumericValue<quint64>(d, otp::storage::Storage::HOTP_TOKEN_COUNTER, count);
            }

            bool HOTPTokenParameters::setTokenCounter(quint64 count)
            {
                Q_D(otp::generator::TokenParameters);
                return d->storage()->writeParam(otp::storage::Storage::HOTP_TOKEN_COUNTER, QVariant(count));
            }

            TOTPTokenParameters::TOTPTokenParameters(otp::generator::TokenParametersPrivate * d, QObject * parent) : GenericOTPParameters(d, parent) {}
            TOTPTokenParameters::~TOTPTokenParameters() {}
            TOTPTokenParameters * TOTPTokenParameters::create(otp::storage::Storage * store, QObject * parent)
            {
                return store && store->type() == otp::storage::OTPTokenType::TOTP ? new TOTPTokenParameters(new otp::generator::TokenParametersPrivate(store), parent) : nullptr;
            }

            bool TOTPTokenParameters::tokenTimeStep(quint64 & timeStepMSec) const
            {
                Q_D(const otp::generator::TokenParameters);
                return otp::generator::internal::lookupNumericValue<quint64>(d, otp::storage::Storage::TOTP_TOKEN_TIMESTEP, timeStepMSec, otp::oath::DEFAULT_TIMESTEP_MSEC);
            }

            bool TOTPTokenParameters::setTokenTimeStep(quint64 timeStepMSec)
            {
                Q_D(otp::generator::TokenParameters);
                return d->storage()->writeParam(otp::storage::Storage::TOTP_TOKEN_TIMESTEP, QVariant(timeStepMSec));
            }

            bool TOTPTokenParameters::tokenEpoch(qint64 & epoch) const
            {
                Q_D(const otp::generator::TokenParameters);
                return otp::generator::internal::lookupNumericValue<qint64>(d, otp::storage::Storage::TOTP_TOKEN_EPOCH, epoch, otp::oath::DEFAULT_EPOCH);
            }

            bool TOTPTokenParameters::setTokenEpoch(const QDateTime& epoch)
            {
                return setTokenEpoch(epoch.toMSecsSinceEpoch());
            }

            bool TOTPTokenParameters::setTokenEpoch(qint64 unixTime)
            {
                Q_D(otp::generator::TokenParameters);
                return d->storage()->writeParam(otp::storage::Storage::TOTP_TOKEN_EPOCH, QVariant(unixTime));
            }

            bool algorithm(const otp::generator::GenericTokenParameters * params, otp::token::Algorithm& algo)
            {
                QCryptographicHash::Algorithm h;
                if(params && params->hashAlgorithm(h))
                {
                    algo =  otp::oath::token::hmacAlgorithm(h);
                    return true;
                }
                return false;
            }

            static bool oathTokenEncoder(otp::oath::generator::GenericOTPParameters * p, otp::token::Encoder& encoder)
            {
                QLocale l;
                uint length;
                if(p && p->tokenLocale(l) && p->tokenLength(length))
                {
                    encoder = otp::token::Encoder(otp::oath::token::oathEncoder(length, l));
                    return true;
                }
                return false;
            }

            class HOTPTokenGeneratorPrivate: public otp::generator::TokenGeneratorPrivate
            {
            public:
                HOTPTokenGeneratorPrivate(otp::oath::generator::HOTPTokenParameters * p) : otp::generator::TokenGeneratorPrivate(p) {}
                bool message(otp::token::Message& message) const
                {
                    quint64 count;
                    auto p = qobject_cast<otp::oath::generator::HOTPTokenParameters *>(params());
                    if(p && p->tokenCounter(count))
                    {
                        message = otp::token::Message([count](void) -> QByteArray
                        {
                            return otp::oath::hotpTokenMessage(count);
                        });
                        return true;
                    }
                    return false;
                }

                bool algorithm(otp::token::Algorithm& algo) const
                {
                    return otp::oath::generator::algorithm(qobject_cast<GenericOTPParameters *>(params()), algo);
                }

                bool encoder(otp::token::Encoder& encoder) const
                {
                    auto p = qobject_cast<otp::oath::generator::HOTPTokenParameters *>(params());
                    return oathTokenEncoder(p, encoder);
                }

                bool updateStorage(void)
                {
                    quint64 count;
                    auto p = qobject_cast<otp::oath::generator::HOTPTokenParameters *>(params());
                    if(p && p->tokenCounter(count))
                    {
                        return (count + 1) > count && p->setTokenCounter(count + 1) && p->commit();
                    }
                    return false;
                }
            };

            otp::generator::TokenGenerator * HOTPTokenParameters::generator(HOTPTokenParameters * params, QObject * parent)
            {
                return new otp::generator::TokenGenerator(new HOTPTokenGeneratorPrivate(params), parent);
            }

            class TOTPTokenGeneratorPrivate: public otp::generator::TokenGeneratorPrivate
            {
            public:
                TOTPTokenGeneratorPrivate(otp::oath::generator::TOTPTokenParameters * p, bool overrideNow = false, qint64 currentMSec = 0) :
                    otp::generator::TokenGeneratorPrivate(p), m_overrideNow(overrideNow), m_currentMSec(currentMSec) {}
                bool message(otp::token::Message& message) const
                {
                    auto p = qobject_cast<otp::oath::generator::TOTPTokenParameters *>(params());
                    qint64 epoch;
                    quint64 ival;
                    if(p && p->tokenEpoch(epoch) && p->tokenTimeStep(ival))
                    {
                        message = m_overrideNow ? otp::oath::token::totpMessage(m_currentMSec, epoch, ival): otp::oath::token::totpMessage(epoch, ival);
                        return true;
                    }

                    return false;
                }

                bool algorithm(otp::token::Algorithm& algo) const
                {
                    return otp::oath::generator::algorithm(qobject_cast<GenericOTPParameters *>(params()), algo);
                }

                bool encoder(otp::token::Encoder& encoder) const
                {
                    auto p = qobject_cast<otp::oath::generator::TOTPTokenParameters *>(params());
                    return oathTokenEncoder(p, encoder);
                }

            private:
                bool m_overrideNow;
                qint64 m_currentMSec;
            };

            otp::generator::TokenGenerator * TOTPTokenParameters::generator(TOTPTokenParameters * params, QObject * parent)
            {
                return new otp::generator::TokenGenerator(new TOTPTokenGeneratorPrivate(params), parent);
            }

            otp::generator::TokenGenerator * TOTPTokenParameters::generator(TOTPTokenParameters * params, qint64 currentMSec, QObject * parent)
            {
                return new otp::generator::TokenGenerator(new TOTPTokenGeneratorPrivate(params, true, currentMSec), parent);
            }
        }
    }
}
