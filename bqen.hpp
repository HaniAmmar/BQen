#include <new>
#include "JSON.hpp"
#include "Template.hpp"
#include "StringView.hpp"

#ifndef _QENTEM_BQEN_H
#define _QENTEM_BQEN_H

namespace BQen {

using Digit = Qentem::Digit;

using Qentem::QNumber64;
using Qentem::QNumberType;
using Qentem::SizeT;
using Qentem::SizeT32;
using Qentem::SizeT64;

struct BQ_ZVAL : zval {
    using JSONotation = Qentem::JSONUtils::JSONotation_T<char>;

    inline bool IsArray() const noexcept {
        return ((Z_TYPE_P(this) == IS_ARRAY) && (HT_IS_PACKED(Z_ARRVAL_P(this))));
    }

    inline bool IsObject() const noexcept {
        return ((Z_TYPE_P(this) == IS_ARRAY) && (!HT_IS_PACKED(Z_ARRVAL_P(this))));
    }

    inline bool IsString() const noexcept {
        return (Z_TYPE_P(this) == IS_STRING);
    }

    inline bool IsNumber() const noexcept {
        return ((Z_TYPE_P(this) == IS_LONG) || (Z_TYPE_P(this) == IS_DOUBLE));
    }

    inline SizeT Size() const noexcept {
        if (Z_TYPE_P(this) == IS_ARRAY) {
            return Z_ARRVAL_P(this)->nNumOfElements;
        }

        return 0;
    }

    inline SizeT Length() const noexcept {
        if (Z_TYPE_P(this) == IS_STRING) {
            return Z_STRLEN_P(this);
        }

        return 0;
    }

    inline const BQ_ZVAL *GetValue(SizeT index) const {
        if (index < Size()) {
            const zval *val = nullptr;

            if (IsArray()) {
                val = ((Z_ARRVAL_P(this)->arPacked + index));
            } else if (IsObject()) {
                val = &((Z_ARRVAL_P(this)->arData + index)->val);
            }

            if ((val != nullptr) && (Z_TYPE_P(val) != IS_UNDEF)) {
                return static_cast<const BQ_ZVAL *>(val);
            }
        }

        return nullptr;
    }

    const BQ_ZVAL *GetValue(const char *key, SizeT length) const {
        const zval *val = nullptr;

        if (IsArray()) {
            SizeT index;
            Digit::FastStringToNumber(index, key, length);

            if (index < Size()) {
                val = ((Z_ARRVAL_P(this)->arPacked + index));
            }
        } else if (IsObject()) {
            val = zend_hash_str_find(Z_ARRVAL_P(this), key, length);
        }

        if ((val != nullptr) && (Z_TYPE_P(val) != IS_UNDEF)) {
            return static_cast<const BQ_ZVAL *>(val);
        }

        return nullptr;
    }

    bool SetCharAndLength(const char *&key, SizeT &length) const noexcept {
        switch (Z_TYPE_P(this)) {
            case IS_STRING: {
                key    = Z_STRVAL_P(this);
                length = Z_STRLEN_P(this);

                return true;
            }

            case IS_TRUE: {
                key    = JSONotation::TrueString;
                length = JSONotation::TrueStringLength;
                return true;
            }

            case IS_FALSE: {
                key    = JSONotation::FalseString;
                length = JSONotation::FalseStringLength;
                return true;
            }

            case IS_NULL: {
                key    = JSONotation::NullString;
                length = JSONotation::NullStringLength;
                return true;
            }

            default: {
                return false;
            }
        }
    }

    void SetValueAndKey(SizeT index, const BQ_ZVAL *&value, Qentem::StringView<char> &key) const noexcept {
        if (IsObject()) {
            const zval *item = &((Z_ARRVAL_P(this)->arData + index)->val);
            value            = nullptr;

            if ((item != nullptr) && (Z_TYPE_P(item) != IS_UNDEF)) {
                value = static_cast<const BQ_ZVAL *>(item);
                key   = Qentem::StringView<char>{Z_STRVAL_P(this), Z_STRLEN_P(this)};
            }
        }
    }

    template <typename StringStream_T>
    using CopyValueToStringFunction_T = void(StringStream_T, const char *, SizeT);

    template <typename StringStream_T, typename StringFunction_T = CopyValueToStringFunction_T<StringStream_T>>
    bool CopyValueTo(StringStream_T             &stream,
                     const Digit::RealFormatInfo format = Digit::RealFormatInfo{Qentem::Config::DoublePrecision},
                     StringFunction_T           *string_function = nullptr) const {
        switch (Z_TYPE_P(this)) {
            case IS_STRING: {
                if (string_function != nullptr) {
                    string_function(stream, Z_STRVAL_P(this), Z_STRLEN_P(this));
                } else {
                    stream.Write(Z_STRVAL_P(this), Z_STRLEN_P(this));
                }

                return true;
            }

            case IS_LONG: {
                Digit::NumberToString(stream, Z_LVAL_P(this));
                return true;
            }

            case IS_DOUBLE: {
                Digit::NumberToString(stream, Z_DVAL_P(this), format);
                return true;
            }

            case IS_TRUE: {
                stream.Write(JSONotation::TrueString, JSONotation::TrueStringLength);
                return true;
            }

            case IS_FALSE: {
                stream.Write(JSONotation::FalseString, JSONotation::FalseStringLength);
                return true;
            }

            case IS_NULL: {
                stream.Write(JSONotation::NullString, JSONotation::NullStringLength);
                return true;
            }

            default: {
                return false;
            }
        }
    }

    inline QNumberType GetNumberType() const noexcept {
        switch (Z_TYPE_P(this)) {
            case IS_LONG: {
                if (Z_LVAL_P(this) >= 0) {
                    return QNumberType::Natural;
                }

                return QNumberType::Integer;
            }

            case IS_DOUBLE: {
                return QNumberType::Real;
            }

            default:
                return QNumberType::NotANumber;
        }
    }

    QNumberType SetNumber(QNumber64 &value) const noexcept {
        switch (Z_TYPE_P(this)) {
            case IS_LONG: {
                value = QNumber64(Z_LVAL_P(this));

                if (Z_LVAL_P(this) >= 0) {
                    return QNumberType::Natural;
                }

                return QNumberType::Integer;
            }

            case IS_DOUBLE: {
                value = QNumber64(Z_DVAL_P(this));
                return QNumberType::Real;
            }

            case IS_STRING: {
                size_t offset = 0;
                return Digit::StringToNumber(value, Z_STRVAL_P(this), offset, Z_STRLEN_P(this));
            }

            case IS_TRUE: {
                value = QNumber64{1};
                return QNumberType::Natural;
            }

            case IS_NULL:
            case IS_FALSE: {
                value = QNumber64{0};
                return QNumberType::Natural;
            }

            default: {
                return QNumberType::NotANumber;
            }
        }
    }

    bool GroupBy(zval &groupedValue, const char *key, const SizeT length) const noexcept {
        // It's easier to do it using PHP code.
        (void)groupedValue;
        (void)key;
        (void)length;
        return false;
    }

    void Sort(bool ascend = true) {
        (void)ascend;
        // It's easier to do it using PHP code.
    }
};

} // namespace BQen

#endif
