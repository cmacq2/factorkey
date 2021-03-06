#include "secrets.h"

#include <QTimer>

namespace stubs
{
    namespace secrets
    {
        SecretsAPIDelay::SecretsAPIDelay(const SecretsAPIDelay::Response& response, int msec) : QObject(), m_reply(response), m_msec(msec) {}

        bool SecretsAPIDelay::sleep(void)
        {
            if(m_started || m_msec < -1)
            {
                return false;
            }

            if(m_msec >= 0)
            {
                QTimer::singleShot(m_msec, this, &SecretsAPIDelay::wakeup);
            }

            return (m_started = true);
        }

        void SecretsAPIDelay::wakeup(void)
        {
            m_reply();
            deleteLater();
        }

        FakeSecretsAPIProvider::FakeSecretsAPIProvider(const QHash<QString, QString>& secrets, int delayMsec, bool permitSecretUpdate) :
            otp::secrets::SecretsAPIProvider(), m_secrets(secrets), m_delay(delayMsec), m_permitUpdate(permitSecretUpdate), m_open(false) {}
        FakeSecretsAPIProvider::~FakeSecretsAPIProvider() {}

        bool FakeSecretsAPIProvider::allowOpen(void) const
        {
            return true;
        }

        bool FakeSecretsAPIProvider::allowClose(void) const
        {
            return true;
        }

        bool FakeSecretsAPIProvider::allowTell(void) const
        {
            return true;
        }

        bool FakeSecretsAPIProvider::allowAsk(void) const
        {
            return true;
        }

        bool FakeSecretsAPIProvider::ask(const QString& entryId, const otp::secrets::SecretsAPIProvider::SecretAnswer answer)
        {
            if(m_open && allowAsk())
            {
                const auto delay = new SecretsAPIDelay([this, answer, entryId](void) -> void
                {
                    if(m_newSecrets.contains(entryId))
                    {
                        answer(true, entryId, m_newSecrets.value(entryId));
                    }
                    else
                    {
                        if(m_secrets.contains(entryId))
                        {
                            answer(true, entryId, m_secrets.value(entryId));
                        }
                        else
                        {
                            answer(false, entryId, QString());
                        }
                    }
                }, m_delay);
                return delay->sleep();
            }
            return false;
        }

        bool FakeSecretsAPIProvider::tell(const QString& entryId, const QString& secret, const otp::secrets::SecretsAPIProvider::SecretConfirmation confirm)
        {
            if(m_open && allowTell())
            {
                const auto delay = new SecretsAPIDelay([this, confirm, secret, entryId](void) -> void
                {
                    if(m_permitUpdate)
                    {
                        m_newSecrets.insert(entryId, secret);
                        confirm(true, entryId);
                    }
                    else
                    {
                        confirm(false, entryId);
                    }
                }, m_delay);
                return delay->sleep();
            }
            return false;
        }

        const QHash<QString, QString>& FakeSecretsAPIProvider::originalSecrets(void) const
        {
            return m_secrets;
        }

        const QHash<QString, QString>& FakeSecretsAPIProvider::newSecrets(void) const
        {
            return m_newSecrets;
        }

        const QHash<QString, QString> FakeSecretsAPIProvider::updatedSecrets(void) const
        {
            QHash<QString, QString> result;

            for(const auto origId : m_secrets.keys())
            {
                result.insert(origId, m_secrets.value(origId));
            }

            for(const auto newId : m_newSecrets.keys())
            {
                result.insert(newId, m_newSecrets.value(newId));
            }

            return result;
        }

        bool FakeSecretsAPIProvider::isOpened(void) const
        {
            return m_open;
        }

        bool FakeSecretsAPIProvider::open(void)
        {
            if(allowOpen())
            {
                m_open=true;
                return true;
            }
            return false;
        }

        bool FakeSecretsAPIProvider::close(void)
        {
            if(allowClose())
            {
                m_open=false;
                return true;
            }
            return false;
        }

        void FakeSecretsAPIBuilder::setProvider(otp::secrets::SecretsAPIProvider * fake)
        {
            m_fake = fake;
        }

        otp::secrets::SecretsAPIProvider * FakeSecretsAPIBuilder::build(void) const
        {
            return m_fake;
        }
    }
}

namespace mock
{
    namespace secrets
    {
        MockSecretsAPIProvider::MockSecretsAPIProvider() {}
        MockSecretsAPIProvider::~MockSecretsAPIProvider() {}
    }
}
