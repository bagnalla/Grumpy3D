#ifndef CAMERA_H
#define CAMERA_H

#include "Object.h"
#include <SDL2/SDL.h>

namespace puddi
{
	class Player;

	class Camera : public Object
	{
	public:
		Camera(Object *par);

		void Update();

		float GetZoomOut() const;

		void SetZoomOut(float z);

		vec4 GetLookDirection() const;

		void SetLookDirection(const vec3& lookDir);

		void LookAt(const vec4& pos);

		void RotateX(float x);

		void InputEvent(SDL_Event ev);

	private:
		bool holdingW, holdingA, holdingS, holdingD, holdingZ, holdingSpace;
		vec3 lookTheta; // rotation values of the camera
		vec4 lookDirection; // look direction vector of the camera
		mat4 lookRotation;
		float zoomOut; // zoom out value

		void move();

		void updateGameCamera();
	};
}

#endif
