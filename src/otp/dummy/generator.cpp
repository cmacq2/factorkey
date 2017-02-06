#include "generator.h"
#include "../generator_p.h"
#include "../oath/generator.h"
#include "dummy.h"
#include "parameters.h"

namespace otp
{
    namespace dummy
    {
        namespace generator
        {
            DummyParameters::DummyParameters(otp::generator::TokenParametersPrivate * d, QObject * parent) : otp::generator::GenericTokenParameters(d, parent) {}
            DummyParameters::~DummyParameters() {}

            const bool DummyParameters::isRegistered = otp::generator::TokenParameters::registerType(otp::storage::OTPTokenType::DummyHMAC, from);

            DummyParameters * DummyParameters::from(otp::storage::Storage * store, QObject * parent)
            {
                return isRegistered && store && store->type() == otp::storage::OTPTokenType::DummyHMAC ? new DummyParameters(new otp::generator::TokenParametersPrivate(store), parent) : nullptr;
            }

            DummyParameters * DummyParameters::create(const QString& entryId, otp::storage::StorageProvider * provider, QObject * parent)
            {
                auto s = provider->create(entryId, otp::storage::OTPTokenType::DummyHMAC);
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

            bool DummyParameters::tokenMessage(QString & message) const
            {
                Q_D(const otp::generator::TokenParameters);
                QVariant str;
                if(d->lookup(otp::dummy::parameters::MESSAGE, str) && !str.isNull())
                {
                    message = str.toString();
                    return true;
                }
                return false;
            }

            bool DummyParameters::setTokenMessage(const QString& message)
            {
                Q_D(otp::generator::TokenParameters);
                if(message.isNull())
                {
                    return false;
                }
                else
                {
                    d->storage()->writeParam(otp::dummy::parameters::MESSAGE, QVariant(message));
                    return true;
                }
            }

            bool DummyParameters::tokenMessageEncoding(QTextCodec ** codec) const
            {
                Q_D(const otp::generator::TokenParameters);
                return d->lookupCodec(otp::dummy::parameters::CHARSET, codec);
            }

            bool DummyParameters::setTokenMessageEncoding(const QTextCodec * codec)
            {
                Q_D(otp::generator::TokenParameters);
                return d->setCodec(otp::dummy::parameters::CHARSET, codec);
            }

            class DummyTokenGeneratorPrivate: public otp::generator::TokenGeneratorPrivate
            {
            public:
                DummyTokenGeneratorPrivate(DummyParameters * p) : otp::generator::TokenGeneratorPrivate(p) {}
                bool message(otp::token::Message& message) const
                {
                    DummyParameters * p = qobject_cast<DummyParameters *>(params());
                    QString str;
                    QTextCodec * codec = nullptr;
                    if(p && p->tokenMessage(str) && p->tokenMessageEncoding(&codec))
                    {
                        message = otp::token::textMessage(str, codec);
                        return true;
                    }

                    return false;
                }

                bool algorithm(otp::token::Algorithm& algo) const
                {
                    auto p = qobject_cast<otp::generator::GenericTokenParameters *>(params());
                    return p && p->otpAlgorithm(algo);
                }

                bool encoder(otp::token::Encoder& encoder) const
                {
                    encoder = otp::token::Encoder(otp::dummy::encodeDummyFormat);
                    return true;
                }
            };

            otp::generator::TokenGenerator * DummyParameters::generator(QObject * parent)
            {
                return new otp::generator::TokenGenerator(new DummyTokenGeneratorPrivate(this), parent);
            }
        }
    }
}
