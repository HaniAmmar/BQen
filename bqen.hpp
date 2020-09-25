#ifndef QENTEM_BQEN_H_
#define QENTEM_BQEN_H_

namespace BQen {

using Digit        = Qentem::Digit<char>;
using String       = Qentem::String<char>;
using StringStream = Qentem::StringStream<char>;
using Qentem::UInt;
using Qentem::ULong;

struct BQ_ZVAL : zval {
    inline bool IsArray() const noexcept {
        if (Z_TYPE_P(this) == IS_ARRAY) {
            return (Z_ARRVAL_P(this)->arData->key == nullptr);
        }

        return false;
    }

    inline bool IsObject() const noexcept {
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

    inline ULong Size() const noexcept {
        if (Z_TYPE_P(this) == IS_ARRAY) {
            return Z_ARRVAL_P(this)->nNumUsed;
        }

        return 0;
    }

    inline const BQ_ZVAL *GetValue(ULong id) const {
        if ((Z_TYPE_P(this) == IS_ARRAY) && (Z_ARRVAL_P(this)->nNumUsed > id)) {
            return static_cast<const BQ_ZVAL *>(
                &(Z_ARRVAL_P(this)->arData + id)->val);
        }

        return nullptr;
    }

    const BQ_ZVAL *GetValue(const char *key, UInt length) const {
        return static_cast<const BQ_ZVAL *>(
            zend_hash_str_find(Z_ARRVAL_P(this), key, length));
    }

    template <typename Number_T_>
    bool SetCharAndLength(const char *&key, Number_T_ &length) const noexcept {
        switch (Z_TYPE_P(this)) {
            case IS_STRING: {
                key    = Z_STRVAL_P(this);
                length = static_cast<Number_T_>(Z_STRLEN_P(this));

                return true;
            }

            case IS_TRUE: {
                key    = "true";
                length = 4;
                return true;
            }

            case IS_FALSE: {
                key    = "false";
                length = 5;
                return true;
            }

            case IS_NULL: {
                key    = "null";
                length = 4;
                return true;
            }

            default: {
                return false;
            }
        }
    }

    bool InsertString(StringStream &ss) const noexcept {
        switch (Z_TYPE_P(this)) {
            case IS_STRING: {
                ss.Insert(Z_STRVAL_P(this), Z_STRLEN_P(this));
                return true;
            }

            case IS_LONG: {
                Digit::NumberToStringStream(ss, Z_LVAL_P(this), 1);
                return true;
            }

            case IS_DOUBLE: {
                Digit::NumberToStringStream(ss, Z_DVAL_P(this), 1, 0, 14);
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
                return false;
            }
        }
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
                return false;
            }
        }
    }
};

} // namespace BQen

#endif
