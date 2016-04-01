#include "RTL.h"
#include "Font.h"
#include "Util.h"
#include "Puddi.h"
#include "GrumpyConfig.h"
#include <iostream>

using namespace puddi;
using namespace std;

namespace grumpy
{
    // RTLInstruction PUBLIC

    RTLInstruction::RTLInstruction(Object *par, const std::string &instr, const std::string &font) : Object(par)
    {
        characters = instr;
        glyphs = Font::CreateGlyphString(this, font, characters);
        velocity = 0.0f;
    }

    void RTLInstruction::Update()
    {
        if (position == assignedPosition)
            return;

        float moveAmount = velocity * GrumpyConfig::GetGameSpeedFactor();

        MoveToPoint(assignedPosition, moveAmount);
    }

    void RTLInstruction::SetAssignedPosition(const glm::vec4 pos)
    {
        assignedPosition = pos;
    }

    void RTLInstruction::SetVelocity(float v)
    {
        velocity = v;
    }

    float RTLInstruction::GetWidth() const
    {
        if (!glyphs.size()) return 0.0f;
        return glyphs.size() * glyphs[0]->GetScaleX();
    }

    // RTLInstruction PRIVATE

    /* nothing */

    // RTLProgram PUBLIC

    RTLProgram::RTLProgram(Object *par, const std::string &f, const std::string &path) : Object(par)
    {
        font = f;
        instr_velocity = 0.01f;
        loadFromFile(path);
    }

    // RTLProgram PRIVATE

    void RTLProgram::loadFromFile(const std::string &path)
    {
        string code = string(Util::ReadAllBytes(path).data());

        stringstream ss(code);
        string instr = "";
        int instr_count = 0;
        while (!ss.eof())
        {
            getline(ss, instr);
            if (ss.eof()) break;
            if (instr == "") continue;
            auto rtl_instr = new RTLInstruction(this, instr, font);
            //rtl_instr->Translate(vec4(0.0f, 20.0f, -(instr_count++), 0.0f));
            rtl_instr->SetAssignedPosition(vec4(0.0f, 0.0f, -(instr_count++), 0.0f));
            rtl_instr->SetVelocity(instr_velocity);
            rtl_instr->Cull();
            instructions.push_back(rtl_instr);
            //cout << instr << endl;
        }
    }
}
