/**
 * @file "Core/Events/Public/Variant.hpp"
 *
 * @author Oriol Manzano
 */

#include <string>
#ifndef VARIANT_H_
#define VARIANT_H_

namespace ez
{
    struct Variant
    {
        enum 
        {
            TYPE_CHAR,
            TYPE_INT,
            TYPE_DOUBLE, 
            TYPE_FLOAT,
            TYPE_BOOL,
            TYPE_STRING_ID // number of unique types
        }_type;

        union
        {
            char _asChar;
            int _asInt;
            double _asDouble;
            float _asFloat;
            bool _asBool;
            char _asString;
        };
    };
}
#endif
