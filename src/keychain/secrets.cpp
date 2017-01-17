#include "secrets.h"

#include <functional>
#include <QtDebug>

namespace otp
{
    namespace secrets
    {
        namespace keychain
        {
            KeychainManager::KeychainManager(const QString& serviceName) : otp::secrets::SecretsAPIProvider(), m_serviceName(serviceName), m_opened(false) {}

            bool KeychainManager::ask(const QString& entryId, const otp::secrets::SecretsAPIProvider::SecretAnswer answer)
            {
                auto job = new QKeychain::ReadPasswordJob(m_serviceName);
                job->setAutoDelete(false);
                job->setInsecureFallback(false);
                job->setKey(entryId);
                QObject::connect(job, &QKeychain::Job::finished, [job, answer, entryId](QKeychain::Job * j) -> void
                {
                    qDebug() << "Answering...";
                    if(j->error() == QKeychain::Error::NoError)
                    {
                        qDebug() << "OK:"<<entryId;
                        const QString pwd = job->textData();
                        answer(true, entryId, pwd);
                    }
                    else
                    {
                        qDebug()<< "NOT OK:" << entryId;
                        answer(false, entryId, QString());
                    }
                    j->deleteLater();
                });
                job->start();
                return true;
            }

            bool KeychainManager::tell(const QString& entryId, const QString& secret, const otp::secrets::SecretsAPIProvider::SecretConfirmation confirm)
            {

                auto job = new QKeychain::WritePasswordJob(m_serviceName);
                job->setAutoDelete(false);
                job->setInsecureFallback(false);
                job->setKey(entryId);
                job->setTextData(secret);
                QObject::connect(job, &QKeychain::Job::finished, [confirm, entryId](QKeychain::Job * j) -> void
                {
                    if(j->error() == QKeychain::Error::NoError)
                    {
                        confirm(true, entryId);
                    }
                    else
                    {
                        confirm(false, entryId);
                    }
                    j->deleteLater();
                });
                job->start();
                return true;
            }

            bool KeychainManager::isOpened(void) const
            {
                return m_opened;
            }

            bool KeychainManager::open(void)
            {
                m_opened = true;
                return true;
            }

            bool KeychainManager::close(void)
            {
                m_opened = false;
                return true;
            }

            void KeychainBuilder::setName(const QString& serviceName)
            {
                m_serviceName = serviceName;
            }

            KeychainManager * KeychainBuilder::build(void) const
            {
                return new KeychainManager(m_serviceName);
            }
        }
    }
}
