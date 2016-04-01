#include "RTLCompiler.h"
#include "RTL.h"
#include "GrumpyConfig.h"
#include "AST.h"
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace puddi;
using namespace std;

namespace grumpy
{
    // PUBLIC

    RTLCompiler::RTLCompiler(Object* par, ASTNode *root, RTLProgram *prog, float v, const std::string &mapPath) : DrawableObject(par)
    {
        init(root, prog, v, mapPath);
    }

    RTLCompiler::RTLCompiler(Object* par, ASTNode *root, RTLProgram *prog, float v, const std::string &mapPath, SchematicNode *schematic) : DrawableObject(par, schematic)
    {
        init(root, prog, v, mapPath);
    }

    void RTLCompiler::Update()
    {
        if (state == RTLCOMPILER_STATE_DONE)
        {
            if (position == homePosition)
                return;

            float moveAmount = velocity * GrumpyConfig::GetGameSpeedFactor();

			MoveToPoint(homePosition, moveAmount, nullptr);

            return;
        }

        if (state == RTLCOMPILER_STATE_COMPILING)
        {
            if (currentInstrIndex >= rtlProgram->instructions.size())
            {
                state = RTLCOMPILER_STATE_DONE;
                return;
            }

            ASTNode *targetNode = astNodesVector[astRtlMap[currentInstrIndex]];

            vec4 targetPosition = targetNode->GetWorldPosition();

            float moveAmount = velocity * GrumpyConfig::GetGameSpeedFactor();

			MoveToPoint(targetPosition, moveAmount, [&]()
			{
			    RTLInstruction *instr = rtlProgram->instructions[currentInstrIndex];
			    instr->UnCull();
                instr->SetWorldPosition(position + vec4(-instr->GetWidth() / 2.0f, 0.0f, 0.0f, 0.0f));

				if (++currentInstrIndex >= rtlProgram->instructions.size())
                {
                    state = RTLCOMPILER_STATE_DONE;
                    return;
                }
			});
        }
    }

    void RTLCompiler::Start()
    {
        currentInstrIndex = 0;
        state = RTLCOMPILER_STATE_COMPILING;
    }

    void RTLCompiler::SetHomePosition(glm::vec4 v)
	{
        homePosition = v;
	}

    // PRIVATE

    void RTLCompiler::init(ASTNode *root, RTLProgram *prog, float v, const std::string &mapPath)
    {
        astRoot = root;
        rtlProgram = prog;
        state = RTLCOMPILER_STATE_WAITING;
        velocity = v;
        currentInstrIndex = 0;
        loadAstRtlMap(mapPath);
        createAstNodesVector();
    }

    void RTLCompiler::loadAstRtlMap(const std::string &mapPath)
    {
        ifstream fin(mapPath);
        while (1)
        {
            size_t instr_index, node_index;
            fin >> instr_index >> node_index;
            if (fin.eof()) break;
            astRtlMap.emplace(instr_index, node_index);
            //cout << instr_index << " " << node_index << endl;
        }
        fin.close();
    }

    void RTLCompiler::createAstNodesVector()
    {
        astNodesVector.clear();

        addToNodesVectorRecursive(astRoot);

        sort(astNodesVector.begin(), astNodesVector.end(),
        [](ASTNode *a, ASTNode *b) -> bool
        {
            return a->GetParseIndex() < b->GetParseIndex();
        });
    }

    void RTLCompiler::addToNodesVectorRecursive(ASTNode *node)
    {
        astNodesVector.push_back(node);
        for (auto it = node->ChildNodes.begin(); it != node->ChildNodes.end(); ++it)
            addToNodesVectorRecursive(*it);
    }
}

