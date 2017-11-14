#include "trompeloeil.h"
#include <QTest>

namespace autotests
{
    namespace integration
    {
        ExpectationViolation::ExpectationViolation(trompeloeil::severity severity, char const *file, unsigned long line, std::string const &msg): m_severity(severity), m_file(file), m_line(line), m_msg(msg) {}

        QByteArray ExpectationViolation::report(void) const
        {
            static const QString locationFmt(QLatin1String("%1:%2"));
            static const QString reportFmt(QLatin1String("Expectation violated at: %1\nMessage: %2"));
            static const QString unknown_location(QLatin1String("<location unknown/unreported>"));
            const auto location = m_line == 0 ? unknown_location: locationFmt.arg(m_file, m_line);
            const auto msg = reportFmt.arg(location, QString::fromStdString(m_msg));
            return msg.toLocal8Bit();
        }

        ExpectationViolationException:: ExpectationViolationException(const ExpectationViolation& v): QException(), m_violation(v) {}

        void  ExpectationViolationException::raise() const { throw *this; }

        ExpectationViolationException * ExpectationViolationException::clone() const { return new ExpectationViolationException(*this); }

        QByteArray  ExpectationViolationException::report(void) const
        {
            return m_violation.report();
        }

        ViolationsReporter setup(std::vector<ExpectationViolation>& violations)
        {
            return trompeloeil::set_reporter([&violations](trompeloeil::severity severity, char const *file, unsigned long line, std::string const &msg) -> void
            {
                const ExpectationViolation v(severity, file, line, msg);
                if(severity == trompeloeil::severity::fatal)
                {
                    throw ExpectationViolationException(v);
                }
                else
                {
                    violations.push_back(v);
                }
            });
        }

        bool verifyExpectations(const std::vector<ExpectationViolation>& violations)
        {
            for(const auto& v: violations)
            {
                const auto msg = v.report();
                QWARN(msg.data());
            }
            return violations.empty();
        }
    }
}
