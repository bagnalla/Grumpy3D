#ifndef RTLCOMPILER_H
#define RTLCOMPILER_H

#include "DrawableObject.h"
#include <unordered_map>

namespace grumpy
{
    class ASTNode;
    class RTLProgram;

    enum RTLCompilerState : int
    {
        RTLCOMPILER_STATE_WAITING,
        RTLCOMPILER_STATE_COMPILING,
        RTLCOMPILER_STATE_DONE
    };

    class RTLCompiler : public puddi::DrawableObject
    {
    public:
        RTLCompiler(Object* par, ASTNode *root, RTLProgram *prog, float v, const std::string &mapPath);
        RTLCompiler(Object* par, ASTNode *root, RTLProgram *prog, float v, const std::string &mapPath, puddi::SchematicNode *schematic);
        void Update();
        void Start();
        void SetHomePosition(glm::vec4 v);
    private:
        ASTNode *astRoot;
        RTLProgram *rtlProgram;
        glm::vec4 homePosition;
        RTLCompilerState state;
        float velocity;
        size_t currentInstrIndex;
        std::vector<ASTNode*> astNodesVector;
        std::unordered_map<size_t, size_t> astRtlMap;

        void init(ASTNode *root, RTLProgram *prog, float v, const std::string &mapPath);
        void loadAstRtlMap(const std::string &mapPath);
        void createAstNodesVector();
        void addToNodesVectorRecursive(ASTNode *node);
    };
}

#endif
