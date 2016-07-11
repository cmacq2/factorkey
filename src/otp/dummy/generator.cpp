#include "generator.h"
#include "../generator_p.h"
#include "../oath/generator.h"
#include "dummy.h"

namespace otp
{
    namespace dummy
    {
        namespace generator
        {
            DummyParameters::DummyParameters(otp::generator::TokenParametersPrivate * d, QObject * parent) : otp::generator::GenericTokenParameters(d, parent) {}
            DummyParameters::~DummyParameters() {}
            DummyParameters * DummyParameters::create(otp::storage::Storage * store, QObject * parent)
            {
                return store && store->type() == otp::storage::OTPTokenType::DummyHMAC ? new DummyParameters(new otp::generator::TokenParametersPrivate(store), parent) : nullptr;
            }

            bool DummyParameters::tokenMessage(QString & message) const
            {
                Q_D(const otp::generator::TokenParameters);
                QVariant str;
                if(d->lookup(otp::storage::Storage::DUMMY_TOKEN_MESSAGE_VALUE, str) && !str.isNull())
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
                    d->storage()->writeParam(otp::storage::Storage::DUMMY_TOKEN_MESSAGE_VALUE, QVariant(message));
                    return true;
                }
            }

            bool DummyParameters::tokenMessageEncoding(QTextCodec ** codec) const
            {
                Q_D(const otp::generator::TokenParameters);
                return d->lookupCodec(otp::storage::Storage::DUMMY_TOKEN_MESSAGE_CHARSET, codec);
            }

            bool DummyParameters::setTokenMessageEncoding(const QTextCodec * codec)
            {
                Q_D(otp::generator::TokenParameters);
                return d->setCodec(otp::storage::Storage::DUMMY_TOKEN_MESSAGE_CHARSET, codec);
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
                    return otp::oath::generator::algorithm(qobject_cast<otp::oath::generator::GenericOTPParameters *>(params()), algo);
                }

                bool encoder(otp::token::Encoder& encoder) const
                {
                    encoder = otp::token::Encoder(otp::dummy::encodeDummyFormat);
                    return true;
                }
            };

            otp::generator::TokenGenerator * DummyParameters::generator(DummyParameters * params, QObject * parent)
            {
                return new otp::generator::TokenGenerator(new DummyTokenGeneratorPrivate(params), parent);
            }
        }
    }
}
