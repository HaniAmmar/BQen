#ifndef QENTEM_BQEN_H_
#define QENTEM_BQEN_H_

namespace BQen {

using Digit        = Qentem::Digit<char>;
using String       = Qentem::String<char>;
using StringStream = Qentem::StringStream<char>;
using Qentem::SizeT;

struct BQ_ZVAL : zval {
    using JSONotation_T_ = Qentem::JSON::JSONotation<char>;

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

    inline SizeT Size() const noexcept {
        if (Z_TYPE_P(this) == IS_ARRAY) {
            return Z_ARRVAL_P(this)->nNumUsed;
        }

        return 0;
    }

    inline const BQ_ZVAL *GetValue(SizeT index) const {
        if ((Z_TYPE_P(this) == IS_ARRAY) &&
            (Z_ARRVAL_P(this)->nNumUsed > index)) {
            const zval *val = &((Z_ARRVAL_P(this)->arData + index)->val);

            if ((val != nullptr) && (Z_TYPE_P(val) != IS_UNDEF)) {
                return static_cast<const BQ_ZVAL *>(val);
            }
        }

        return nullptr;
    }

    const BQ_ZVAL *GetValue(const char *key, SizeT length) const {
        if (Z_TYPE_P(this) == IS_ARRAY) {
            if (Z_ARRVAL_P(this)->arData->key != nullptr) {
                const zval *val =
                    zend_hash_str_find(Z_ARRVAL_P(this), key, length);
                if ((val != nullptr) && (Z_TYPE_P(val) != IS_UNDEF)) {
                    return static_cast<const BQ_ZVAL *>(val);
                }

                return nullptr;
            }

            SizeT index;

            if ((Digit::StringToNumber(index, key, length)) &&
                (Z_ARRVAL_P(this)->nNumUsed > index)) {
                const zval *val = &((Z_ARRVAL_P(this)->arData + index)->val);

                if ((val != nullptr) && (Z_TYPE_P(val) != IS_UNDEF)) {
                    return static_cast<const BQ_ZVAL *>(val);
                }
            }
        }

        return nullptr;
    }

    template <typename Number_T_>
    bool SetKeyCharAndLength(Number_T_ index, const char *&key,
                             Number_T_ &length) const noexcept {
        if ((Z_TYPE_P(this) == IS_ARRAY) &&
            (Z_ARRVAL_P(this)->arData->key != nullptr) &&
            (Z_ARRVAL_P(this)->nNumUsed > index)) {
            const zend_string *val = (Z_ARRVAL_P(this)->arData + index)->key;

            key    = val->val;
            length = val->len;
            return true;
        }

        return false;
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
                key    = JSONotation_T_::GetTrueString();
                length = JSONotation_T_::TrueStringLength;
                return true;
            }

            case IS_FALSE: {
                key    = JSONotation_T_::GetFalseString();
                length = JSONotation_T_::FalseStringLength;
                return true;
            }

            case IS_NULL: {
                key    = JSONotation_T_::GetNullString();
                length = JSONotation_T_::NullStringLength;
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
                ss.Insert(JSONotation_T_::GetTrueString(),
                          JSONotation_T_::TrueStringLength);
                return true;
            }

            case IS_FALSE: {
                ss.Insert(JSONotation_T_::GetFalseString(),
                          JSONotation_T_::FalseStringLength);
                return true;
            }

            case IS_NULL: {
                ss.Insert(JSONotation_T_::GetNullString(),
                          JSONotation_T_::NullStringLength);
                return true;
            }

            default: {
                return false;
            }
        }
    }

    template <typename Number_T_>
    bool SetNumber(Number_T_ &value) const noexcept {
        switch (Z_TYPE_P(this)) {
            case IS_LONG: {
                value = static_cast<Number_T_>(Z_LVAL_P(this));
                return true;
            }

            case IS_DOUBLE: {
                value = static_cast<Number_T_>(Z_DVAL_P(this));
                return true;
            }

            case IS_STRING: {
                return Digit::StringToNumber(
                    value, Z_STRVAL_P(this),
                    static_cast<SizeT>(Z_STRLEN_P(this)));
            }

            case IS_TRUE: {
                value = 1;
                return true;
            }

            case IS_NULL:
            case IS_FALSE: {
                value = 0;
                return true;
            }

            default: {
                return false;
            }
        }
    }

    template <typename Number_T_>
    bool GroupBy(zval &groupedValue, const char *key,
                 const Number_T_ length) const noexcept {
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
