#include "RTL.h"
#include "Font.h"

using namespace puddi;

namespace grumpy
{
    // PUBLIC

    RTLInstruction::RTLInstruction(Object *par, const std::string &instr, const std::string &font) : Object(par)
    {
        characters = instr;
        glyphs = Font::CreateGlyphString(this, font, characters);
    }

    // PRIVATE
}
