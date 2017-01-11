#ifndef FACTORKEY_SECRETS_H
#define FACTORKEY_SECRETS_H

#include <functional>
#include <QSharedPointer>
#include <QString>

namespace otp
{
    namespace storage
    {
        namespace secrets
        {
            class SecretsAPIProvider
            {
            public:
                typedef std::function<void(bool, const QString&, const QString&)> SecretAnswer;
                typedef std::function<void(bool, const QString&)> SecretConfirmation;
            public:
                virtual bool ask(const QString& entryId, const SecretAnswer answer) = 0;
                virtual bool tell(const QString& entryId, const QString& secret, const SecretConfirmation confirm) = 0;
                virtual bool open(void) = 0;
                virtual bool close(void) = 0;
                virtual bool isOpened(void) const = 0;
                virtual ~SecretsAPIProvider() = 0;
            };

            class SecretsAPIBuilder
            {
            public:
                virtual ~SecretsAPIBuilder();
                virtual QSharedPointer<SecretsAPIProvider> create(void) const;
            protected:
                virtual SecretsAPIProvider * build(void) const;
            };
        }
    }
}

#endif
