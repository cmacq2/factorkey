#include "storage_p.h"

namespace otp
{
    StoragePrivate::~StoragePrivate() {}

    QString StoragePrivate::entryId(void) const
    {
        return QString();
    }

    OTPTokenType StoragePrivate::type(void) const
    {
        return OTPTokenType::DummyHMAC;
    }

    bool StoragePrivate::writePassword(const QString&)
    {
        return false;
    }

    bool StoragePrivate::readTokenType(OTPTokenType&) const
    {
        return false;
    }

    bool StoragePrivate::writeTokenType(const OTPTokenType&)
    {
        return false;
    }

    bool StoragePrivate::readParam(const QString&, QVariant&) const
    {
        return false;
    }

    bool StoragePrivate::readPassword(QString&) const
    {
        return false;
    }

    bool StoragePrivate::writeParam(const QString&, const QVariant&) const
    {
        return false;
    }

    bool StoragePrivate::exists(void) const
    {
        return false;
    }

    bool StoragePrivate::commit(void)
    {
        return false;
    }

    bool StoragePrivate::poll(void)
    {
        return false;
    }

    StorageProviderPrivate::~StorageProviderPrivate() {}

    bool StorageProviderPrivate::open(void)
    {
        bool open = isOpened();
        if(!open)
        {
            return openBackend();
        }
        return open;
    }

    bool StorageProviderPrivate::close(void)
    {
        if(isOpened())
        {
            return closeBackend();
        }
        return true;
    }

    bool StorageProviderPrivate::contains(const QString& entry)
    {
        QStringList l;
        if(entries(l))
        {
            return l.indexOf(entry) >= 0;
        }
        return false;
    }

    bool StorageProviderPrivate::entries(QStringList&)
    {
        return false;
    }

    Storage * StorageProviderPrivate::lookup(const QString& entry)
    {
        return open() ? doLookup(entry) : nullptr;
    }

    Storage * StorageProviderPrivate::create(const QString& entry, OTPTokenType type)
    {
        if(open())
        {
            if(contains(entry))
            {
                Storage * s = lookup(entry);
                if(s && s->type() == type)
                {
                    return s;
                }
            }
            else
            {
                return doCreate(entry, type);
            }
        }
        return nullptr;
    }

    Storage * StorageProviderPrivate::doLookup(const QString&)
    {
        return nullptr;
    }

    Storage * StorageProviderPrivate::doCreate(const QString&, OTPTokenType)
    {
        return nullptr;
    }

    bool StorageProviderPrivate::isOpened(void)
    {
        return false;
    }

    bool StorageProviderPrivate::openBackend(void)
    {
        return false;
    }

    bool StorageProviderPrivate::closeBackend(void)
    {
        return false;
    }

    namespace internal
    {
        const QString WalletManager::WALLET_FOLDER = QStringLiteral("factorkey"); //= QLatin1String("factorkey");
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