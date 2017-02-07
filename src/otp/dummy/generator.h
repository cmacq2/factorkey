#ifndef FACTORKEY_DUMMY_GENERATOR_H
#define FACTORKEY_DUMMY_GENERATOR_H

#include "../generator.h"
#include "../storage/storageprovider.h"

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
                static DummyParameters * from(otp::storage::Storage * store, QObject * parent = 0);
                static DummyParameters * create(const QString& entryId, otp::storage::StorageProvider * provider, QObject * parent = 0);
            public:
                otp::generator::TokenGenerator * generator(QObject * parent = nullptr) override;
                bool setTokenMessage(const QString& value);
                bool setTokenMessageEncoding(const QTextCodec * codec);

                bool tokenMessage(QString& message) const;
                bool tokenMessageEncoding(QTextCodec ** codec) const;
                otp::generator::TokenGenerator * generator(QObject * parent) const;
                virtual ~DummyParameters();
            protected:
                DummyParameters(otp::generator::TokenParametersPrivate * d, QObject * parent = 0);
            private:
                static const bool isRegistered;
            };

        }
    }
}

#endif
