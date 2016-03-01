#ifndef MODELGRAPH_H
#define MODELGRAPH_H

#include <vector>

namespace puddi
{
	class Object;

	struct ModelNode
	{
		ModelNode *parent;
		std::vector<ModelNode*> children;
		Object *object;
		bool parallel;

		ModelNode(ModelNode *par, Object *o);

		~ModelNode();

		void Update();

		void AddChild(ModelNode *child);

		void RemoveChild(ModelNode *child);
	};
}

#endif
