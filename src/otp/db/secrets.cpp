#include "secrets.h"

namespace otp
{
    namespace storage
    {
        namespace db
        {
            const QString WalletManager::WALLET_FOLDER = QStringLiteral("factorkey");
            WalletManager::WalletManager(const WId& window, const QString& walletName): m_window(window), m_walletName(walletName) {}

            bool WalletManager::ask(const QString& entryId, const otp::storage::secrets::SecretsAPIProvider::SecretAnswer answer)
            {
                QString password;
                if(readPassword(entryId, password))
                {
                    answer(true, entryId, password);
                    return true;
                }
                else
                {
                    answer(false, entryId, QString());
                    return false;
                }
            }

            bool WalletManager::tell(const QString& entryId, const QString& secret, const otp::storage::secrets::SecretsAPIProvider::SecretConfirmation confirm)
            {
                if(writePassword(entryId, secret))
                {
                    confirm(true, entryId);
                    return true;
                }
                else
                {
                    confirm(false, entryId);
                    return false;
                }
            }

            bool WalletManager::entries(QStringList& entryList)
            {
                if(isOpened())
                {
                    entryList = m_wallet->entryList();
                    return true;
                }
                return false;
            }

            bool WalletManager::contains(const QString& entry)
            {
                return open() && m_wallet->hasEntry(entry);
            }

            bool WalletManager::isOpened(void) const
            {
                return m_wallet && m_wallet->isOpen();
            }

            bool WalletManager::writePassword(const QString& entryId, const QString& secret)
            {
                return (isOpened() || open()) && m_wallet->writePassword(entryId, secret) == 0;
            }

            bool WalletManager::readPassword(const QString& entryId, QString& secret)
            {
                QString result;
                if((isOpened() || open()) && m_wallet->readPassword(entryId, result) == 0)
                {
                    secret = result;
                    return true;
                }
                return false;
            }

            bool WalletManager::open(void)
            {
                m_wallet.reset(KWallet::Wallet::openWallet(m_walletName, m_window));
                if(m_wallet)
                {
                    m_wallet->createFolder(WALLET_FOLDER);
                    if(m_wallet->hasFolder(WALLET_FOLDER))
                    {
                        if(m_wallet->setFolder(WALLET_FOLDER))
                        {
                            return true;
                        }
                    }
                }

                // errors! close and report failure
                if(m_wallet)
                {
                    close();
                }
                return false;
            }

            bool WalletManager::close(void)
            {
                if(m_wallet)
                {
                    int error = m_wallet->closeWallet(m_walletName, false);
                    m_wallet.reset(nullptr);
                    return error == 0;
                }
                return true;
            }

            WalletManager::~WalletManager()
            {
                close();
            }

            void WalletBuilder::setWindow(const WId& window)
            {
                m_window = window;
            }

            void WalletBuilder::setName(const QString& walletName)
            {
                m_walletName = walletName;
            }

            WalletManager * WalletBuilder::build(void) const
            {
                return new WalletManager(m_window, m_walletName);
            }
        }
    }
}
