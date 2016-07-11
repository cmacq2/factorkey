#ifndef FACTORKEY_DUMMY_GENERATOR_H
#define FACTORKEY_DUMMY_GENERATOR_H

#include "../generator.h"

namespace otp
{
    namespace dummy
    {
        namespace generator
        {

            class DummyParameters: public otp::generator::GenericTokenParameters
            {
                Q_OBJECT
            public:
                static otp::generator::TokenGenerator * generator(DummyParameters * params, QObject * parent = 0);
                static DummyParameters * create(otp::storage::Storage * store, QObject * parent = 0);
            public:
                otp::storage::OTPTokenType type(void) const;
                bool setTokenMessage(const QString& value);
                bool setTokenMessageEncoding(const QTextCodec * codec);

                bool tokenMessage(QString& message) const;
                bool tokenMessageEncoding(QTextCodec ** codec) const;
                virtual ~DummyParameters();
            protected:
                DummyParameters(otp::generator::TokenParametersPrivate * d, QObject * parent = 0);
            };

        }
    }
}

#endif
