#ifndef RTL_H
#define RTL_H

#include "DrawableObject.h"
#include <string>
#include <vector>

namespace grumpy
{
    class RTLInstruction : public puddi::Object
    {
    public:
        RTLInstruction(Object *par, const std::string &instr, const std::string &font);
    private:
        size_t index;
        std::string characters;
        std::vector<puddi::DrawableObject*> glyphs;
    };
}

#endif
