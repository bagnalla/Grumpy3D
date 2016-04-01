#ifndef TYPECHECKER_H
#define TYPECHECKER_H

#include "DrawableObject.h"

namespace grumpy
{
    class ASTNode;
    class RTLCompiler;

    enum TypeCheckerState : int
    {
        TYPECHECKER_STATE_WAITING,
        TYPECHECKER_STATE_TYPECHECKING,
        TYPECHECKER_STATE_DONE
    };

    class TypeChecker : public puddi::DrawableObject
    {
    public:
        TypeChecker(Object* par, ASTNode *root);
		TypeChecker(Object* par, ASTNode *root, puddi::SchematicNode *schematic);

        void Update();

        float GetVelocity() const;
        void SetVelocity(float v);

		void SetHomePosition(glm::vec4 v);

		void SetRTLCompiler(RTLCompiler *c);

		void Start();

    private:
        ASTNode *astRoot;
        std::vector<ASTNode*> nodesVector;
        size_t currentNodeIndex;
        float velocity;
        TypeCheckerState state;
        glm::vec4 homePosition;
        RTLCompiler *rtlCompiler;

        void init(ASTNode *root);
        void createNodesVector();
        void addToNodesVectorRecursive(ASTNode *node);
        void finish();
    };
}

#endif
