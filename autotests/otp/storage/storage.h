#ifndef FACTORKEY_AUTOTESTS_STUBBED_STORAGE_H
#define FACTORKEY_AUTOTESTS_STUBBED_STORAGE_H

#include "otp/storage/storage_p.h"

#include <QList>
#include <QMap>
#include <QSignalSpy>
#include <QString>
#include <QVariant>

namespace stubs
{
    namespace storage
    {
        class DummyStoragePrivate: public QObject, public otp::storage::internal::FailingStoragePrivate
        {
            Q_OBJECT
        private:
            QString m_entryId;
            otp::storage::OTPTokenType m_type;
            QString m_password;
            QMap<QString,QVariant> m_map;
        private:
            void init(void);
        private:
            QSignalSpy * m_spy_writePassword = nullptr;
            QSignalSpy * m_spy_readTokenType = nullptr;
            QSignalSpy * m_spy_writeTokenType = nullptr;
            QSignalSpy * m_spy_readParam = nullptr;
            QSignalSpy * m_spy_readPassword = nullptr;
            QSignalSpy * m_spy_writeParam = nullptr;
            QSignalSpy * m_spy_exists = nullptr;
            QSignalSpy * m_spy_commit = nullptr;
            QSignalSpy * m_spy_poll = nullptr;
            QSignalSpy * m_spy_type = nullptr;
            QSignalSpy * m_spy_entryId = nullptr;
        public:
            QSignalSpy * spy_writePassword(void) const;
            QSignalSpy * spy_readTokenType(void) const;
            QSignalSpy * spy_writeTokenType(void) const;
            QSignalSpy * spy_readParam(void) const;
            QSignalSpy * spy_readPassword(void) const;
            QSignalSpy * spy_writeParam(void) const;
            QSignalSpy * spy_exists(void) const;
            QSignalSpy * spy_commit(void) const;
            QSignalSpy * spy_poll(void) const;
            QSignalSpy * spy_type(void) const;
            QSignalSpy * spy_entryId(void) const;
        public:
            const QMap<QString,QVariant>& rawStorage(void) const;
        protected:
            virtual bool allowWriteParam(void) const;
            virtual bool allowReadParam(void) const;
            virtual bool allowReadPassword(void) const;
            virtual bool allowWritePassword(void) const;
            virtual bool allowReadTokenType(void) const;
            virtual bool allowWriteTokenType(void) const;

            virtual QString impl_entryId(void) const;
            virtual otp::storage::OTPTokenType impl_type(void) const;
            virtual bool impl_writePassword(const QString& password);
            virtual bool impl_readTokenType(otp::storage::OTPTokenType& type) const;
            virtual bool impl_writeTokenType(const otp::storage::OTPTokenType& type);
            virtual bool impl_readParam(const QString& param, QVariant& value) const;
            virtual bool impl_readPassword(QString& password) const;
            virtual bool impl_writeParam(const QString& param, const QVariant& value);
            virtual bool impl_exists(void) const;
            virtual bool impl_commit(void);
            virtual bool impl_poll(void);

        Q_SIGNALS:
            void notify_entryId(QString entryId) const;
            void notify_type(otp::storage::OTPTokenType type) const;
            void notify_writePassword(bool ok, const QString& password);
            void notify_readTokenType(bool ok, enum otp::storage::OTPTokenType type) const;
            void notify_writeTokenType(bool ok, const enum otp::storage::OTPTokenType type);
            void notify_readParam(bool ok, const QString& param, const QVariant& value) const;
            void notify_readPassword(bool ok, const QString& password) const;
            void notify_writeParam(bool ok, const QString& param, const QVariant& value);
            void notify_exists(bool ok) const;
            void notify_commit(bool ok);
            void notify_poll(bool ok);
        public:
            DummyStoragePrivate(const QString& entryId, otp::storage::OTPTokenType type, const QString& password, const QMap<QString,QVariant>& preset, QObject * parent = nullptr);
            DummyStoragePrivate(const QString& entryId,  otp::storage::OTPTokenType type, const QString& password, QObject * parent = nullptr);
            DummyStoragePrivate(otp::storage::OTPTokenType type, const QString& password, const QMap<QString,QVariant>& preset, QObject * parent = nullptr);
            DummyStoragePrivate(otp::storage::OTPTokenType type, const QString& password, QObject * parent = nullptr);
            virtual ~DummyStoragePrivate();
        public:
            QString entryId(void) const;
            otp::storage::OTPTokenType type(void) const;
            bool writePassword(const QString& password);
            bool readTokenType(otp::storage::OTPTokenType& type) const;
            bool writeTokenType(const otp::storage::OTPTokenType& type);
            bool readParam(const QString& param, QVariant& value) const;
            bool readPassword(QString& password) const;
            bool writeParam(const QString& param, const QVariant& value);
            bool exists(void) const;
            bool commit(void);
            bool poll(void);
        public:
            void check_entryId(const QList<QString>& callArgs) const;
            void check_no_entryId(void) const;
            void check_type(const QList<enum otp::storage::OTPTokenType>& callArgs) const;
            void check_no_type(void) const;
            void check_no_write_token_type(void) const;
            void check_write_token_type(const QList<QList<QVariant>>& callArgs) const;
            void check_read_token_type(const QList<QList<QVariant>>& callArgs) const;
            void check_no_read_token_type(void) const;
            void check_no_write_password(void) const;
            void check_write_password(const QList<QList<QVariant>>& callArgs) const;
            void check_read_password(const QList<QList<QVariant>>& callArgs) const;
            void check_no_write_param(void) const;
            void check_write_param(const QList<QList<QVariant>>& callArgs) const;
            void check_read_param(const QList<QList<QVariant>>& callArgs) const;
            void check_no_commit(void) const;
            void check_commit(const QList<bool>& callArgs) const;
            void check_no_poll(void) const;
            void check_poll(const QList<bool>& callArgs) const;
            void check_no_exists(void) const;
            void check_exists(const QList<bool>& callArgs) const;
        };

        QList<QList<QVariant>>& expect_token_type(QList<QList<QVariant>>& callStack, bool status, enum otp::storage::OTPTokenType type);
        QList<QList<QVariant>>& expect_password(QList<QList<QVariant>>& callStack, bool status, const QString& secret);
        QList<QList<QVariant>>& expect_param(QList<QList<QVariant>>& callStack, bool status, const QString& param, const QVariant& value);

    }
}

#endif
