#ifndef QENTEM_BQEN_TREE_H_
#define QENTEM_BQEN_TREE_H_

namespace BQen {

using Qentem::Digit;
using Qentem::String;
using Qentem::StringStream;
using Qentem::UInt;
using Qentem::ULong;

struct BString : zend_string {
    inline const char *Char() const noexcept {
        return ZSTR_VAL(this);
    }

    inline ULong Length() const noexcept {
        return static_cast<ULong>(ZSTR_LEN(this));
    }
};

struct BTree : zval {
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

    const BTree *GetValue(ULong id) const {
        if ((Z_TYPE_P(this) == IS_ARRAY) && (Z_ARRVAL_P(this)->nNumUsed > id)) {
            return static_cast<const BTree *>(
                &(Z_ARRVAL_P(this)->arData + id)->val);
        }

        return nullptr;
    }

    const BTree *GetValue(const char *key, UInt length) const {
        return static_cast<const BTree *>(zend_hash_str_find(
            Z_ARRVAL_P(this), key, static_cast<size_t>(length)));
    }

    const BString *GetKey(ULong id) const {
        if ((Z_TYPE_P(this) == IS_ARRAY) && (Z_ARRVAL_P(this)->nNumUsed > id)) {
            return static_cast<const BString *>(
                (Z_ARRVAL_P(this)->arData + id)->key);
        }

        return nullptr;
    }

    const char *Char() const noexcept {
        if (Z_TYPE_P(this) == IS_STRING) {
            return Z_STRVAL_P(this);
        }

        return nullptr;
    }

    ULong Length() const noexcept {
        if (Z_TYPE_P(this) == IS_STRING) {
            return static_cast<ULong>(Z_STRLEN_P(this));
        }

        return 0;
    }

    bool SetString(String &value) const noexcept {
        switch (Z_TYPE_P(this)) {
            case IS_STRING: {
                value = String(Z_STRVAL_P(this),
                               static_cast<ULong>(Z_STRLEN_P(this)));
                return true;
            }

            case IS_LONG: {
                value = Digit::NumberToString(Z_LVAL_P(this), 1);
                return true;
            }

            case IS_DOUBLE: {
                value = Digit::NumberToString(Z_DVAL_P(this), 1, 0, 14);
                return true;
            }

            case IS_TRUE: {
                value = String("true");
                return true;
            }

            case IS_FALSE: {
                value = String("false");
                return true;
            }

            case IS_NULL: {
                value = String("null");
                return true;
            }

            default: {
                value.Clear();
                return false;
            }
        }
    }

    bool InsertString(StringStream &ss) const noexcept {
        switch (Z_TYPE_P(this)) {
            case IS_STRING: {
                ss.Add(Z_STRVAL_P(this), static_cast<ULong>(Z_STRLEN_P(this)));
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
                ss.Add("true", 4);
                return true;
            }

            case IS_FALSE: {
                ss.Add("false", 5);
                return true;
            }

            case IS_NULL: {
                ss.Add("null", 4);
                return true;
            }

            default: {
                return false;
            }
        }
    }

    double GetNumber() const noexcept {
        switch (Z_TYPE_P(this)) {
            case IS_LONG: {
                return static_cast<double>(Z_LVAL_P(this));
            }

            case IS_DOUBLE: {
                return Z_DVAL_P(this);
            }

            default: {
                return 0;
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
                value = 0.0;
                return false;
            }
        }
    }
};

} // namespace BQen

#endif
