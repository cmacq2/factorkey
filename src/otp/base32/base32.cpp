#include "base32.h"

namespace otp
{
    namespace base32
    {
        static QChar pad(void)
        {
            static const QChar _pad = QLatin1Char('=');
            return _pad;
        }

        QChar encodeByte(int idx)
        {
            static const QChar alphabet[32] =
            {
                QLatin1Char('A'),
                QLatin1Char('B'),
                QLatin1Char('C'),
                QLatin1Char('D'),
                QLatin1Char('E'),
                QLatin1Char('F'),
                QLatin1Char('G'),
                QLatin1Char('H'),
                QLatin1Char('I'),
                QLatin1Char('J'),
                QLatin1Char('K'),
                QLatin1Char('L'),
                QLatin1Char('M'),
                QLatin1Char('N'),
                QLatin1Char('O'),
                QLatin1Char('P'),
                QLatin1Char('Q'),
                QLatin1Char('R'),
                QLatin1Char('S'),
                QLatin1Char('T'),
                QLatin1Char('U'),
                QLatin1Char('V'),
                QLatin1Char('W'),
                QLatin1Char('X'),
                QLatin1Char('Y'),
                QLatin1Char('Z'),
                QLatin1Char('2'),
                QLatin1Char('3'),
                QLatin1Char('4'),
                QLatin1Char('5'),
                QLatin1Char('6'),
                QLatin1Char('7')
            };
            return idx >= 0 && idx < 32 ? alphabet[idx] : pad();
        }

        int decodeByte(const QChar chr)
        {
            if(chr >= QLatin1Char('A') && chr <=  QLatin1Char('Z'))
            {
                return chr.toLatin1() - 'A';
            }
            else
            {
                if(chr >= QLatin1Char('2') && chr <= QLatin1Char('7'))
                {
                    return 26 + chr.toLatin1() - '2';
                }
                else
                {
                    return -1;
                }
            }
        }

        static inline void encode(const QByteArray& content, int offset, int size, QString& output)
        {
            const char * data = content.constData();
            QChar c1, c2, c3, c4, c5, c6, c7, c8;
            switch(size - offset)
            {
                case 0:
                    return;
                case 1:
                    c1 = encodeByte((data[offset] & 0xF8) >> 3); // 5 bits
                    c2 = encodeByte((data[offset] & 0x07) << 2); // 3 bits
                    c3 = (c4 = (c5 = (c6 = (c7 = (c8 = pad())))));
                    break;
                case 2:
                    c1 = encodeByte((data[offset] & 0xF8) >> 3); // 5 bits
                    c2 = encodeByte(((data[offset] & 0x07) << 2) | ((data[offset + 1] & 0xC0) >> 6) ); // 3 bits | 2 bits
                    c3 = encodeByte((data[offset +1] & 0x3E) >> 1); // 5 bits
                    c4 = encodeByte((data[offset +1] & 0x01) << 4); // 1 bit
                    c5 = (c6 = (c7 = (c8 = pad())));
                    break;
                case 3:
                    c1 = encodeByte((data[offset] & 0xF8) >> 3); // 5 bits
                    c2 = encodeByte(((data[offset] & 0x07) << 2) | ((data[offset + 1] & 0xC0) >> 6) ); // 3 bits | 2 bits
                    c3 = encodeByte((data[offset +1] & 0x3E) >> 1); // 5 bits
                    c4 = encodeByte(((data[offset +1] & 0x01) << 4) | ((data[offset + 2] & 0xF0) >> 4)); // 1 bit | 4 bits
                    c5 = encodeByte((data[offset + 2] & 0x0F) << 1); //  4 bits
                    c6 = (c7 = (c8 = pad()));
                    break;
                case 4:
                    c1 = encodeByte((data[offset] & 0xF8) >> 3); // 5 bits
                    c2 = encodeByte(((data[offset] & 0x07) << 2) | ((data[offset + 1] & 0xC0) >> 6) ); // 3 bits | 2 bits
                    c3 = encodeByte((data[offset +1] & 0x3E) >> 1); // 5 bits
                    c4 = encodeByte(((data[offset +1] & 0x01) << 4) | ((data[offset + 2] & 0xF0) >> 4)); // 1 bit | 4 bits
                    c5 = encodeByte(((data[offset + 2] & 0x0F) << 1) | ((data[offset + 3] & 0x80) >> 7)); // 4 bits | 1 bit
                    c6 = encodeByte((data[offset + 3] & 0x7C) >> 2); // 5 bits
                    c7 = encodeByte((data[offset + 3] & 0x03) << 3); // 2 bits
                    c8 = pad();
                    break;
                default: // 5 or more
                    if(offset >= size) return; // sanity check

                    c1 = encodeByte((data[offset] & 0xF8) >> 3); // 5 bits
                    c2 = encodeByte(((data[offset] & 0x07) << 2) | ((data[offset + 1] & 0xC0) >> 6) ); // 3 bits | 2 bits
                    c3 = encodeByte((data[offset +1] & 0x3E) >> 1); // 5 bits
                    c4 = encodeByte(((data[offset +1] & 0x01) << 4) | ((data[offset + 2] & 0xF0) >> 4)); // 1 bit | 4 bits
                    c5 = encodeByte(((data[offset + 2] & 0x0F) << 1) | ((data[offset + 3] & 0x80) >> 7)); // 4 bits | 1 bit
                    c6 = encodeByte((data[offset + 3] & 0x7C) >> 2); // 5 bits
                    c7 = encodeByte(((data[offset + 3] & 0x03) << 3) | ((data[offset + 4] & 0xE0) >> 5) ); // 2 bits | 3 bits
                    c8 = encodeByte((data[offset + 4] & 0x1F)); // 5 bits
                    break;
            }
            output += c1;
            output += c2;
            output += c3;
            output += c4;
            output += c5;
            output += c6;
            output += c7;
            output += c8;
        }

        QString encode(const QByteArray& content)
        {
            QString output = QLatin1String("");
            int size = content.size();

            int capacity = size / 5;
            if(size % 5)
            {
                capacity ++;
            }
            capacity *= 8;
            output.reserve(capacity);

            for(int k = 0; k < size; k += 5)
            {
                encode(content, k, size, output);
            }

            return output;
        }

        static inline int determineCapacity(int encodedBytes, int accountFor, int lastBytes)
        {
            return 5 * (encodedBytes - accountFor) / 8  + lastBytes;
        }

        static inline bool decode(const QChar * encoded, int offset, int size, int idx, int capacity, char * output)
        {
            int i1, i2, i3, i4, i5, i6, i7, i8;
            switch(size - offset)
            {
                /* invalid sequence lengths */
                case 0:
                case 1:
                case 3:
                case 6:
                    return false;
                case 2:
                    if(idx >= capacity // sanity check
                        || ((i1 = decodeByte(encoded[offset])) < 0) || (i2 = decodeByte(encoded[offset + 1])) < 0
                        || (i2 & 0xE0) != 0) return false;
                    output[idx] = (char) (((i1 << 3) & 0xF8) | ((i2 >> 2) & 0x07));
                    break;
                case 4:
                    if(idx >= (capacity - 1) // sanity check
                        || (i1 = decodeByte(encoded[offset])) < 0 || (i2 = decodeByte(encoded[offset + 1])) < 0
                        || (i3 = decodeByte(encoded[offset + 2])) < 0 || (i4 = decodeByte(encoded[offset + 3])) < 0
                        || (i4 & 0xEF) != 0) return false;
                    output[idx] = (char) (((i1 << 3) & 0xF8) | ((i2 >> 2) & 0x07));
                    output[idx + 1] = (char) (((i2 << 6) & 0xC0) | ((i3 << 1) & 0x3E) | ((i4 >> 4) & 0x01));
                    break;
                case 5:
                    if(idx >= (capacity - 2) // sanity check
                        || (i1 = decodeByte(encoded[offset])) < 0 || (i2 = decodeByte(encoded[offset + 1])) < 0
                        || (i3 = decodeByte(encoded[offset + 2])) < 0 || (i4 = decodeByte(encoded[offset + 3])) < 0
                        || (i5 = decodeByte(encoded[offset + 4])) < 0
                        || (i5 & 0xE1) != 0) return false;
                    output[idx] = (char) (((i1 << 3) & 0xF8) | ((i2 >> 2) & 0x07));
                    output[idx + 1] = (char) (((i2 << 6) & 0xC0) | ((i3 << 1) & 0x3E) | ((i4 >> 4) & 0x01));
                    output[idx + 2] = (char) (((i4 << 4) & 0xF0) | ((i5 >> 1) & 0x0F));
                    break;
                case 7:
                    if(idx >= (capacity - 3) // sanity check
                        || (i1 = decodeByte(encoded[offset])) < 0 || (i2 = decodeByte(encoded[offset + 1])) < 0
                        || (i3 = decodeByte(encoded[offset + 2])) < 0 || (i4 = decodeByte(encoded[offset + 3])) < 0
                        || (i5 = decodeByte(encoded[offset + 4])) < 0 || (i6 = decodeByte(encoded[offset + 5])) < 0
                        || (i7 = decodeByte(encoded[offset + 6])) < 0
                        || (i7 & 0xE7) != 0) return false;
                    output[idx] = (char) (((i1 << 3) & 0xF8) | ((i2 >> 2) & 0x07));
                    output[idx + 1] = (char) (((i2 << 6) & 0xC0) | ((i3 << 1) & 0x3E) | ((i4 >> 4) & 0x01));
                    output[idx + 2] = (char) (((i4 << 4) & 0xF0) | ((i5 >> 1) & 0x0F));
                    output[idx + 3] = (char) (((i5 << 7) & 0x80) | ((i6 << 2) & 0x7C) | ((i7 >> 3) & 0x03));
                    break;
                default: // 8 or more
                    if(offset >= size || idx >= (capacity - 4) // sanity check
                        || (i1 = decodeByte(encoded[offset])) < 0 || (i2 = decodeByte(encoded[offset + 1])) < 0
                        || (i3 = decodeByte(encoded[offset + 2])) < 0 || (i4 = decodeByte(encoded[offset + 3])) < 0
                        || (i5 = decodeByte(encoded[offset + 4])) < 0 || (i6 = decodeByte(encoded[offset + 5])) < 0
                        || (i7 = decodeByte(encoded[offset + 6])) < 0 || (i8 = decodeByte(encoded[offset + 7])) < 0) return false;
                    output[idx] = (char) (((i1 << 3) & 0xF8) | ((i2 >> 2) & 0x07));
                    output[idx + 1] = (char) (((i2 << 6) & 0xC0) | ((i3 << 1) & 0x3E) | ((i4 >> 4) & 0x01));
                    output[idx + 2] = (char) (((i4 << 4) & 0xF0) | ((i5 >> 1) & 0x0F));
                    output[idx + 3] = (char) (((i5 << 7) & 0x80) | ((i6 << 2) & 0x7C) | ((i7 >> 3) & 0x03));
                    output[idx + 4] = (char) (((i7 << 5) & 0xE0) | (i8 & 0x1F));
                    break;
            }
            return true;
        }

        QByteArray decode(const QString& encoded, bool * ok)
        {
            QByteArray result;
            int size = encoded.size(), capacity = size;

            if(size % 8)
            {
                * ok = false;
                return result;
            }

            while(size > 0 && encoded[size -1] == pad())
            {
                size --;
            }

            switch(capacity - size)
            {
                case 0:
                    capacity = determineCapacity(size, 0, 0);
                    break;
                case 1:
                    capacity = determineCapacity(size, 7, 4);
                    break;
                case 3:
                    capacity = determineCapacity(size, 5, 3);
                    break;
                case 4:
                    capacity = determineCapacity(size, 4, 2);
                    break;
                case 6:
                    capacity = determineCapacity(size, 2, 1);
                    break;
                default:
                    * ok = false;
                    return result;
            }

            result.resize(capacity);

            char * output = result.data();
            const QChar * input = encoded.constData();

            for(int k = 0, i = 0; k < size; k += 8, i += 5)
            {
                if(!decode(input, k, size, i, capacity, output))
                {
                    * ok = false;
                    return QByteArray();
                }
            }

            * ok = true;
            return result;
        }

        QString recode(const QString& content, const QTextCodec * codec)
        {
            return encode(codec ? codec->fromUnicode(content) : content.toUtf8());
        }
    }
}
