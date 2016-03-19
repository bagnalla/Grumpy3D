/**
 * @file ModelGraph.h
 * @author Alexander Bagnall
 * @brief Tree structure used to maintain the transform matrices of
 * objects.
 *
 * This exists as a separate tree from the UpdateGraph tree because
 * the computation of transform matrices is always parallelizable
 * whereas arbritrary update code is not. Parallel computation is
 * disabled by default but if you know about the structure of your
 * object hierarchy you can choose appropriate nodes in the tree to
 * enable parallel computation of their children.
 */

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
