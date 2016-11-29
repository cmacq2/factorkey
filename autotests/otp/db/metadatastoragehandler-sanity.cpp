#include "metadatastoragehandler-sanity.h"

#include <QTest>
#include <QtDebug>

namespace sanity
{
    namespace storage
    {
        namespace db
        {
            MetadataStorageHandlerSanityTest::MetadataStorageHandlerSanityTest(const otp::storage::db::MetadataStorageHandler* uut, QObject * parent): QObject(parent), m_uut(uut) {}
            MetadataStorageHandlerSanityTest::~MetadataStorageHandlerSanityTest() {}

            void MetadataStorageHandlerSanityTest::sanity_data(void)
            {
                QTest::addColumn<QString>("param");
                if(m_uut)
                {
                    const auto all( m_uut->keys());

                    for(const auto k: all)
                    {
                        QTest::newRow(qPrintable(k)) << k;
                    }
                }
            }

            bool MetadataStorageHandlerSanityTest::checkColumnName(const QString& column)
            {
                return !column.isNull() && !column.isEmpty();
            }

            bool MetadataStorageHandlerSanityTest::checkTableName(const QString& table)
            {
                return !table.isNull() && !table.isEmpty();
            }

            bool MetadataStorageHandlerSanityTest::checkParamName(const QString& param)
            {
                return !param.isNull() && !param.isEmpty();
            }

            void MetadataStorageHandlerSanityTest::sanity(void)
            {
                QFETCH(QString, param);
                QVERIFY2(param != otp::storage::db::MetadataStorageHandler::OTP_ENTRY_ID, "The OTP_ENTRY_ID parameter name is reserved");
                QVERIFY2(m_uut->isParamNameValid(param), "Parameter name should be considered 'valid'");
                QVERIFY2(checkParamName(param), "Parameter name should be 'sane'");

                const auto table = m_uut->tableForParam(param);
                QVERIFY2(checkTableName(table), "A parameter should map to a 'sane' table name");

                const auto columnsInTable( m_uut->columnsInTable(table));
                QVERIFY2(columnsInTable.size() > 1, "A table should have at least one actual column (field value) in addition to the join column (OTP_ENTRY_ID)");

                bool paramFoundAsColumn = false;
                for(const auto c: columnsInTable)
                {
                    const auto p = m_uut->columnToParam(c);
                    paramFoundAsColumn = p == param;
                    if(paramFoundAsColumn)
                    {
                        QVERIFY2(checkColumnName(c), "The column name for the parameter should be 'sane'");
                        break;
                    }
                }
                QVERIFY2(paramFoundAsColumn, "There should be a column for the parameter in its table");
            }

            void MetadataStorageHandlerSanityTest::basicSanity(void)
            {
                QVERIFY2(m_uut, "Expected to have a valid MetadataStorageHandler, maybe creating it/mapping parameters failed?");
                QVERIFY2(!m_uut->isParamNameValid(otp::storage::db::MetadataStorageHandler::OTP_ENTRY_ID), "The reserved OTP_ENTRY_ID column should not be marked as 'valid'");
                QVERIFY2(m_uut->isParamNameValid(otp::storage::Storage::OTP_TOKEN_TYPE), "The OTP_TOKEN_TYPE parameter should be considered 'valid'");
                QCOMPARE(m_uut->tableForParam(otp::storage::Storage::OTP_TOKEN_TYPE), otp::storage::db::MetadataStorageHandler::OTP_ENTRY_TABLE);
            }
        }
    }
}

#include "metadatastoragehandler-sanity.moc"
