#include <string>
#include <map>

#include "boost/preprocessor.hpp"

#define X_DEFINE_ENUM_WITH_STRING_CONVERSIONS_TOSTRING(r, data, elem)           \
    map[elem] = std::string(BOOST_PP_STRINGIZE(elem));

#define X_DEFINE_ENUM_WITH_STRING_CONVERSIONS_FROMSTRING(r, data, elem)         \
    map[std::string(BOOST_PP_STRINGIZE(elem))] = elem;

#define DEFINE_ENUM_WITH_STRING_CONVERSIONS(name, enumerators)                  \
    enum name {                                                                 \
        BOOST_PP_SEQ_ENUM(enumerators)                                          \
    };                                                                          \
                                                                                \
    static std::string ToString(name e)                                         \
    {                                                                           \
        std::map<name, std::string> map;                                        \
        BOOST_PP_SEQ_FOR_EACH(                                                  \
            X_DEFINE_ENUM_WITH_STRING_CONVERSIONS_TOSTRING,                     \
            name,                                                               \
            enumerators                                                         \
        )                                                                       \
        return map[e];                                                          \
    }                                                                           \
                                                                                \
    static name FromString(std::string string)                                  \
    {                                                                           \
        std::map<std::string, name> map;                                        \
        BOOST_PP_SEQ_FOR_EACH(                                                  \
            X_DEFINE_ENUM_WITH_STRING_CONVERSIONS_FROMSTRING,                   \
            name,                                                               \
            enumerators                                                         \
        )                                                                       \
        return map[string];                                                     \
    } 
