#ifndef OBJECT_H
#define OBJECT_H

#include "Material.h"
#include "UpdateGraph.h"
#include "ModelGraph.h"
#include <vector>
#include <functional>
#include "GlmHeaders.h"

namespace puddi
{
	// flags for keeping track of changes to the object
	enum MatrixChanged
	{
		TRANSLATION_CHANGED = 1 << 0,
		ROTATIONX_CHANGED = 1 << 1,
		ROTATIONY_CHANGED = 1 << 2,
		ROTATIONZ_CHANGED = 1 << 3,
		SCALE_CHANGED = 1 << 4,
		PARENT_CHANGED = 1 << 5
	};

	enum IgnoreParentModel
	{
		IGNORE_PARENT_TRANSLATION = 1 << 0,
		IGNORE_PARENT_ROTATIONX = 1 << 1,
		IGNORE_PARENT_ROTATIONY = 1 << 2,
		IGNORE_PARENT_ROTATIONZ = 1 << 3,
		IGNORE_PARENT_SCALE = 1 << 4
	};
	inline IgnoreParentModel operator|(IgnoreParentModel a, IgnoreParentModel b)
	{
		return static_cast<IgnoreParentModel>(static_cast<int>(a) | static_cast<int>(b));
	}
	inline IgnoreParentModel operator&(IgnoreParentModel a, IgnoreParentModel b)
	{
		return static_cast<IgnoreParentModel>(static_cast<int>(a) & static_cast<int>(b));
	}

	class Object
	{
	public:
		Object(Object *parent);
		Object(Object *parent, bool childrenUpdateInParallel, bool childrenModelUpdateInParallel);

		virtual ~Object();

		virtual void Update();

		int UpdateModel();

		void MoveToPoint(const glm::vec4 p, float moveAmount, std::function<void()> callback = nullptr);

		void PassDownParentModel(Object *c) const;

		vec4 GetPosition() const;
		vec4 GetWorldPosition() const;

		void SetPosition(const vec4& position);

		void Translate(const vec4& translation);

		void SetCenterOfRotation(const vec4& c);

		float GetRotationX() const;
		void SetRotationX(float rotationX);
		void RotateX(float x);

		float GetRotationY() const;
		void SetRotationY(float rotationY);
		void RotateY(float y);

		float GetRotationZ() const;
		void SetRotationZ(float rotationZ);
		void RotateZ(float z);

		void LookAt(const vec4& point);

		float GetScaleX() const;
		virtual void SetScaleX(float scaleX);
		void ScaleX(float scaleFactorX);

		float GetScaleY() const;
		virtual void SetScaleY(float scaleY);
		void ScaleY(float scaleFactorY);

		float GetScaleZ() const;
		virtual void SetScaleZ(float scaleZ);
		void ScaleZ(float scaleFactorZ);

		virtual void SetScale(float scale);
		virtual void SetScale(float scaleX, float scaleY);
		virtual void SetScale(float scaleX, float scaleY, float scaleZ);

		void Scale(float scaleFactor);

		mat4 GetModel() const;

		mat4 GetFinalModel() const;

		//mat4 GetFinalInverseModel() const;

		mat4 GetRotationMatrix() const;

		void SetParentModel(const mat4& p);

		void SetParentModel(const mat4& pTranslation, const mat4& pRotationX, const mat4& pRotationY, const mat4& pRotationZ, const mat4& pScale);

		/*void SetParentInverseModel(const mat4& p);

		void SetParentInverseModel(const mat4& pTranslation, const mat4& pRotationX, const mat4& pRotationY, const mat4& pRotationZ, const mat4& pScale);*/

		void AddIgnoreParentModelFlag(IgnoreParentModel ignoreFlag);

		void AddChild(Object *c);

		void RemoveChild(Object *c);

		UpdateNode* GetUpdateNode() const;

		ModelNode* GetModelNode() const;

		virtual void Cull();

		virtual void UnCull();

		bool GetCulled() const;

		int GetNumberOfChildren() const;

	protected:
		vec4 position; // position of the object in world space
		vec4 oldPosition;
		mat4 translationMatrix; // current translation matrix
		mat4 oldTranslationMatrix; // previous translation matrix
		//mat4 translationInverseMatrix;

		vec3 theta; // rotation about the three axes
		mat4 rotationMatrix;
		mat4 rotationXMatrix; // current rotationX matrix
		mat4 rotationYMatrix; // current rotationY matrix
		mat4 rotationZMatrix; // current rotationZ matrix

		//mat4 rotationInverseMatrix;
		//mat4 rotationXInverseMatrix;
		//mat4 rotationYInverseMatrix;
		//mat4 rotationZInverseMatrix;

		vec4 centerOfRotation;
		mat4 centerOfRotationTranslationMatrix;
		bool useCenterOfRotation;

		vec3 scale; // scale in the three axes
		mat4 scaleMatrix; // current scale matrix
		//mat4 scaleInverseMatrix;

		unsigned int changedFlags; // flags for tracking changes to the object
		unsigned int ignoreParentModelFlags;
		mat4 model; // model-view matrix
		//mat4 inverseModel;

		mat4 parentModel; // the parent transform matrix
		//mat4 parentInverseModel;
		mat4 finalModel; // the product of parentModel and model
		//mat4 finalInverseModel;

		std::vector<Object*> children; // children of the object

		ModelNode *modelNode;
		UpdateNode *updateNode;

		bool culled;

		int updateModel();
	};
}

#endif
