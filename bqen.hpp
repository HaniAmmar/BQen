#ifndef QENTEM_BQEN_H_
#define QENTEM_BQEN_H_

namespace BQen {

using Qentem::Digit;
using Qentem::String;
using Qentem::StringStream;
using Qentem::UInt;
using Qentem::ULong;

struct BQ_ZVAL : zval {
    bool IsArray() const noexcept {
        if (Z_TYPE_P(this) == IS_ARRAY) {
            return (Z_ARRVAL_P(this)->arData->key == nullptr);
        }

        return false;
    }

    bool IsObject() const noexcept {
        if (Z_TYPE_P(this) == IS_ARRAY) {
            return (Z_ARRVAL_P(this)->arData->key != nullptr);
        }

        return false;
    }

    inline bool IsString() const noexcept {
        return (Z_TYPE_P(this) == IS_STRING);
    }

    inline bool IsNumber() const noexcept {
        return ((Z_TYPE_P(this) == IS_LONG) || (Z_TYPE_P(this) == IS_DOUBLE));
    }

    ULong Size() const noexcept {
        if (Z_TYPE_P(this) == IS_ARRAY) {
            return static_cast<ULong>(Z_ARRVAL_P(this)->nNumUsed);
        }

        return 0;
    }

    const BQ_ZVAL *GetValue(ULong id) const {
        if ((Z_TYPE_P(this) == IS_ARRAY) && (Z_ARRVAL_P(this)->nNumUsed > id)) {
            return static_cast<const BQ_ZVAL *>(
                &(Z_ARRVAL_P(this)->arData + id)->val);
        }

        return nullptr;
    }

    const BQ_ZVAL *GetValue(const char *key, UInt length) const {
        return static_cast<const BQ_ZVAL *>(zend_hash_str_find(
            Z_ARRVAL_P(this), key, static_cast<size_t>(length)));
    }

    bool InsertKey(Qentem::StringStream &ss, Qentem::ULong index) const {
        if ((Z_TYPE_P(this) == IS_ARRAY) &&
            (Z_ARRVAL_P(this)->nNumUsed > index)) {

            zend_string *zstr = (Z_ARRVAL_P(this)->arData + index)->key;
            ss.Insert(zstr->val, zstr->len);
        }

        return false;
    }

    bool SetCharAndLength(const char *&  key,
                          Qentem::ULong &length) const noexcept {
        if (Z_TYPE_P(this) == IS_STRING) {
            key    = Z_STRVAL_P(this);
            length = static_cast<ULong>(Z_STRLEN_P(this));

            return true;
        }

        return false;
    }

    bool SetString(String &value) const noexcept {
        switch (Z_TYPE_P(this)) {
                // case IS_STRING: {
                //     value = String(Z_STRVAL_P(this),
                //                    static_cast<ULong>(Z_STRLEN_P(this)));
                //     return true;
                // }

            case IS_LONG: {
                value = Digit::NumberToString(Z_LVAL_P(this), 1);
                return true;
            }

            case IS_DOUBLE: {
                value = Digit::NumberToString(Z_DVAL_P(this), 1, 0, 14);
                return true;
            }

            case IS_TRUE: {
                value = String("true", 4);
                return true;
            }

            case IS_FALSE: {
                value = String("false", 5);
                return true;
            }

            case IS_NULL: {
                value = String("null", 4);
                return true;
            }

            default: {
            }
        }

        value.Clear();
        return false;
    }

    bool InsertString(StringStream &ss) const noexcept {
        switch (Z_TYPE_P(this)) {
            case IS_STRING: {
                ss.Insert(Z_STRVAL_P(this),
                          static_cast<ULong>(Z_STRLEN_P(this)));
                return true;
            }

            case IS_LONG: {
                ss += Digit::NumberToString(Z_LVAL_P(this), 1);
                return true;
            }

            case IS_DOUBLE: {
                ss += Digit::NumberToString(Z_DVAL_P(this), 1, 0, 14);
                return true;
            }

            case IS_TRUE: {
                ss.Insert("true", 4);
                return true;
            }

            case IS_FALSE: {
                ss.Insert("false", 5);
                return true;
            }

            case IS_NULL: {
                ss.Insert("null", 4);
                return true;
            }

            default: {
            }
        }

        return false;
    }

    bool GetNumber(double &value) const noexcept {
        switch (Z_TYPE_P(this)) {
            case IS_LONG: {
                value = static_cast<double>(Z_LVAL_P(this));
                return true;
            }

            case IS_DOUBLE: {
                value = Z_DVAL_P(this);
                return true;
            }

            case IS_STRING: {
                return Digit::StringToNumber(
                    value, Z_STRVAL_P(this),
                    static_cast<UInt>(Z_STRLEN_P(this)));
            }

            case IS_TRUE: {
                value = 1.0;
                return true;
            }

            case IS_NULL:
            case IS_FALSE: {
                value = 0.0;
                return true;
            }

            default: {
            }
        }

        value = 0.0;
        return false;
    }
};

} // namespace BQen

#endif
