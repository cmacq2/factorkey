#include "generator.h"
#include "../generator_p.h"
#include "../util/localeconversion.h"
#include "oath.h"
#include "token.h"
#include "parameters.h"

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
                if(d->lookup(otp::oath::parameters::generic::LOCALE, val))
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
                        if(otp::util::stringToLocale(str, l))
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
                d->storage()->writeParam(otp::oath::parameters::generic::LOCALE, QVariant(otp::util::localeToString(locale)));
                return true;
            }

            bool GenericOTPParameters::tokenLength(uint& length) const
            {
                Q_D(const otp::generator::TokenParameters);
                return otp::generator::internal::lookupNumericValue<uint>(d, otp::oath::parameters::generic::LENGTH, length, otp::oath::DEFAULT_OTP_LENGTH);
            }

            bool GenericOTPParameters::setTokenLength(uint length)
            {
                Q_D(otp::generator::TokenParameters);
                return d->storage()->writeParam(otp::oath::parameters::generic::LENGTH, QVariant(length));
            }

            HOTPTokenParameters::HOTPTokenParameters(otp::generator::TokenParametersPrivate * d, QObject * parent) : GenericOTPParameters(d, parent) {}
            HOTPTokenParameters::~HOTPTokenParameters() {}

            const bool HOTPTokenParameters::isRegistered = otp::generator::TokenParameters::registerType(otp::storage::OTPTokenType::HOTP, from);

            HOTPTokenParameters * HOTPTokenParameters::from(otp::storage::Storage * store, QObject * parent)
            {
                return isRegistered && store && store->type() == otp::storage::OTPTokenType::HOTP ? new HOTPTokenParameters(new otp::generator::TokenParametersPrivate(store), parent) : nullptr;
            }

            HOTPTokenParameters * HOTPTokenParameters::create(const QString& entryId, otp::storage::StorageProvider * provider, QObject * parent)
            {
                auto s = provider->create(entryId, otp::storage::OTPTokenType::HOTP);
                if(s)
                {
                    auto p = from(s, parent);
                    if(p)
                    {
                        return p;
                    }
                    else
                    {
                        delete s;
                    }
                }
                return nullptr;
            }

            bool HOTPTokenParameters::tokenCounter(quint64 & count) const
            {
                Q_D(const otp::generator::TokenParameters);
                return otp::generator::internal::lookupNumericValue<quint64>(d, otp::oath::parameters::hotp::COUNTER, count);
            }

            bool HOTPTokenParameters::setTokenCounter(quint64 count)
            {
                Q_D(otp::generator::TokenParameters);
                return d->storage()->writeParam(otp::oath::parameters::hotp::COUNTER, QVariant(count));
            }

            TOTPTokenParameters::TOTPTokenParameters(otp::generator::TokenParametersPrivate * d, QObject * parent) : GenericOTPParameters(d, parent) {}
            TOTPTokenParameters::~TOTPTokenParameters() {}

            const bool TOTPTokenParameters::isRegistered = otp::generator::TokenParameters::registerType(otp::storage::OTPTokenType::TOTP, from);

            TOTPTokenParameters * TOTPTokenParameters::from(otp::storage::Storage * store, QObject * parent)
            {
                return isRegistered && store && store->type() == otp::storage::OTPTokenType::TOTP ? new TOTPTokenParameters(new otp::generator::TokenParametersPrivate(store), parent) : nullptr;
            }

            TOTPTokenParameters * TOTPTokenParameters::create(const QString& entryId, otp::storage::StorageProvider * provider, QObject * parent)
            {
                auto s = provider->create(entryId, otp::storage::OTPTokenType::TOTP);
                if(s)
                {
                    auto p = from(s, parent);
                    if(p)
                    {
                        return p;
                    }
                    else
                    {
                        delete s;
                    }
                }
                return nullptr;
            }

            bool TOTPTokenParameters::tokenTimeStep(quint64 & timeStepMSec) const
            {
                Q_D(const otp::generator::TokenParameters);
                return otp::generator::internal::lookupNumericValue<quint64>(d, otp::oath::parameters::totp::TIMESTEP, timeStepMSec, otp::oath::DEFAULT_TIMESTEP_MSEC);
            }

            bool TOTPTokenParameters::setTokenTimeStep(quint64 timeStepMSec)
            {
                Q_D(otp::generator::TokenParameters);
                return d->storage()->writeParam(otp::oath::parameters::totp::TIMESTEP, QVariant(timeStepMSec));
            }

            bool TOTPTokenParameters::tokenEpoch(qint64 & epoch) const
            {
                Q_D(const otp::generator::TokenParameters);
                return otp::generator::internal::lookupNumericValue<qint64>(d, otp::oath::parameters::totp::EPOCH, epoch, otp::oath::DEFAULT_EPOCH);
            }

            bool TOTPTokenParameters::setTokenEpoch(const QDateTime& epoch)
            {
                return setTokenEpoch(epoch.toMSecsSinceEpoch());
            }

            bool TOTPTokenParameters::setTokenEpoch(qint64 unixTime)
            {
                Q_D(otp::generator::TokenParameters);
                return d->storage()->writeParam(otp::oath::parameters::totp::EPOCH, QVariant(unixTime));
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
                bool message(otp::token::Message& message) const override
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

                bool algorithm(otp::token::Algorithm& algo) const override
                {
                    auto p = qobject_cast<GenericOTPParameters *>(params());
                    return p && p->otpAlgorithm(algo);
                }

                bool encoder(otp::token::Encoder& encoder) const override
                {
                    auto p = qobject_cast<otp::oath::generator::HOTPTokenParameters *>(params());
                    return oathTokenEncoder(p, encoder);
                }

                bool updateStorage(void) override
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

            otp::generator::TokenGenerator * HOTPTokenParameters::generator(QObject * parent)
            {
                return new otp::generator::TokenGenerator(new HOTPTokenGeneratorPrivate(this), parent);
            }

            class TOTPTokenGeneratorPrivate: public otp::generator::TokenGeneratorPrivate
            {
            public:
                TOTPTokenGeneratorPrivate(otp::oath::generator::TOTPTokenParameters * p, bool overrideNow = false, qint64 currentMSec = 0) :
                    otp::generator::TokenGeneratorPrivate(p), m_overrideNow(overrideNow), m_currentMSec(currentMSec) {}
                bool message(otp::token::Message& message) const override
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

                bool algorithm(otp::token::Algorithm& algo) const override
                {
                    auto p = qobject_cast<GenericOTPParameters *>(params());
                    return p && p->otpAlgorithm(algo);
                }

                bool encoder(otp::token::Encoder& encoder) const override
                {
                    auto p = qobject_cast<otp::oath::generator::TOTPTokenParameters *>(params());
                    return oathTokenEncoder(p, encoder);
                }

            private:
                bool m_overrideNow;
                qint64 m_currentMSec;
            };

            otp::generator::TokenGenerator * TOTPTokenParameters::generator(QObject * parent)
            {
                return new otp::generator::TokenGenerator(new TOTPTokenGeneratorPrivate(this), parent);
            }

            otp::generator::TokenGenerator * TOTPTokenParameters::generator(qint64 currentMSec, QObject * parent)
            {
                return new otp::generator::TokenGenerator(new TOTPTokenGeneratorPrivate(this, true, currentMSec), parent);
            }
        }
    }
}
