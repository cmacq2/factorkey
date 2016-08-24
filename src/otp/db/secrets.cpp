#include "secrets.h"

namespace otp
{
    namespace storage
    {
        namespace db
        {
            const QString WalletManager::WALLET_FOLDER = QStringLiteral("factorkey");
            WalletManager::WalletManager(const WId& window, const QString& walletName): m_window(window), m_walletName(walletName) {}

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

            bool WalletManager::open(void)
            {
                //             if(m_wallet)
                //             {
                //                 // todo emit signal about re-opening a wallet?
                //             }
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
        }
    }
}
