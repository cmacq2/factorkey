#ifndef FACTORKEY_AUTOTEST_MOCKED_STORAGE_H
#define FACTORKEY_AUTOTEST_MOCKED_STORAGE_H

#include <QObject>
#include <QSharedPointer>

#include "otp/secrets/secrets.h"
#include "lib/googlemock.h"

#include <QHash>

using ::testing::_;
using ::testing::Invoke;

namespace stubs
{
    namespace secrets
    {
        class SecretsAPIDelay: public QObject
        {
            Q_OBJECT
        public:
            typedef std::function<void(void)> Response;
        private Q_SLOTS:
            void wakeup(void);
        public:
            SecretsAPIDelay(const Response& response, int msec = 0);
            bool sleep(void);
        private:
            const Response m_reply;
            const int m_msec;
            bool m_started;
        };

        class FakeSecretsAPIProvider: public otp::secrets::SecretsAPIProvider
        {
        public:
            FakeSecretsAPIProvider(const QHash<QString, QString>& secrets, int delayMsec = 0, bool permitSecretUpdate = false);
            virtual ~FakeSecretsAPIProvider();

            bool ask(const QString& entryId, const otp::secrets::SecretsAPIProvider::SecretAnswer answer);
            bool tell(const QString& entryId, const QString& secret, const otp::secrets::SecretsAPIProvider::SecretConfirmation confirm);
            bool open(void);
            bool close(void);
            bool isOpened(void) const;

            const QHash<QString, QString>& originalSecrets(void) const;
            const QHash<QString, QString>& newSecrets(void) const;
            const QHash<QString, QString> updatedSecrets(void) const;
        protected:
            virtual bool allowAsk(void) const;
            virtual bool allowTell(void) const;
            virtual bool allowOpen(void) const;
            virtual bool allowClose(void) const;
        private:
            const QHash<QString, QString> m_secrets;
            QHash<QString, QString> m_newSecrets;
            int m_delay;
            bool m_permitUpdate;
            bool m_open;
        };

        class FakeSecretsAPIBuilder: public otp::secrets::SecretsAPIBuilder
        {
        public:
            otp::secrets::SecretsAPIProvider * build(void) const;
            void setProvider(otp::secrets::SecretsAPIProvider * fake);
        private:
            otp::secrets::SecretsAPIProvider * m_fake;
        };
    }
}

namespace mock
{
    namespace secrets
    {
        class MockSecretsAPIProvider: public otp::secrets::SecretsAPIProvider
        {
        public:
            MOCK_METHOD2(ask, bool(const QString&, const otp::secrets::SecretsAPIProvider::SecretAnswer));
            MOCK_METHOD3(tell, bool(const QString& entryId, const QString& secret, const otp::secrets::SecretsAPIProvider::SecretConfirmation confirm));
            MOCK_METHOD0(open, bool(void));
            MOCK_METHOD0(close, bool(void));
            MOCK_CONST_METHOD0(isOpened, bool(void));
        };

        class DelegatingMockSecretsAPIProvider: public otp::secrets::SecretsAPIProvider
        {
        public:
            MOCK_METHOD2(ask, bool(const QString&, const otp::secrets::SecretsAPIProvider::SecretAnswer));
            MOCK_METHOD3(tell, bool(const QString& entryId, const QString& secret, const otp::secrets::SecretsAPIProvider::SecretConfirmation confirm));
            MOCK_METHOD0(open, bool(void));
            MOCK_METHOD0(close, bool(void));
            MOCK_CONST_METHOD0(isOpened, bool(void));
            bool delegateToFake(const QSharedPointer<otp::secrets::SecretsAPIProvider> fake);
        private:
            QSharedPointer<SecretsAPIProvider> m_fake;
        };
    }
}
#endif
