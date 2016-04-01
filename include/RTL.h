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
        void Update();
        void SetAssignedPosition(const glm::vec4 pos);
        void SetVelocity(float v);
        float GetWidth() const;
    private:
        size_t index;
        std::string characters;
        std::vector<puddi::DrawableObject*> glyphs;
        glm::vec4 assignedPosition;
        float velocity;
    };

    class RTLProgram : public puddi::Object
    {
    public:
        RTLProgram(Object *par, const std::string &f, const std::string &path);

        std::vector<RTLInstruction*> instructions;
    private:
        std::string font;
        float instr_velocity;

        void loadFromFile(const std::string &path);
    };
}

#endif
