#pragma once
#include <cassert>
#include <SDL_keyboard.h>
#include <SDL_mouse.h>

#include "Math.h"
#include "Timer.h"

#include <iostream>

namespace geo
{
	struct Camera
	{
		Camera() = default;

		Camera(const Vector3& _origin, float _fovAngle) :
			origin{ _origin },
			fovAngle{ _fovAngle }
		{
		}


		Vector3 origin{};
		float fovAngle{ 45.f };
		float fovValue{ tanf(fovAngle * TO_RADIANS / 2.0f) };
		const float SPEED_ROTATION{ 0.2f };
		const float SPEED_TRANSLATION{ 10.f };

		Vector3 forward{ Vector3::UnitZ };
		Vector3 up{ Vector3::UnitY };
		Vector3 right{ Vector3::UnitX };

		float totalPitch{ 0.f };
		float totalYaw{ 0.f };

		const float fovIncrement{ 20.f };
		const float MAX_FOV{ 179.f };
		const float MIN_FOV{ 10.f };

		Matrix cameraToWorld{};

		int mouseX{}, mouseY{};

		Matrix CalculateCameraToWorld()
		{
			right = Vector3::Cross(Vector3::UnitY, forward);
			right.Normalize();

			up = Vector3::Cross(forward, right);
			up.Normalize();
			return 
			{
				{ right.x, right.y, right.z, 0 },
			  { up.x, up.y, up.z, 0 },
			  { forward.x, forward.y, forward.z, 0 },
			  { origin.x, origin.y, origin.z, 0 } 
			};
		}

		void Update(Timer* pTimer)
		{
			const float deltaTime = pTimer->GetElapsed();

			//Keyboard Input
			const uint8_t* pKeyboardState = SDL_GetKeyboardState(nullptr);
			
			if (pKeyboardState[SDL_SCANCODE_W] || pKeyboardState[SDL_SCANCODE_UP])
			{
				origin.x -= forward.x * SPEED_TRANSLATION * deltaTime;
				origin.y -= forward.y * SPEED_TRANSLATION * deltaTime;
				origin.z -= forward.z * SPEED_TRANSLATION * deltaTime;
			}
			if (pKeyboardState[SDL_SCANCODE_S] || pKeyboardState[SDL_SCANCODE_DOWN])
			{
				origin.x += forward.x * SPEED_TRANSLATION * deltaTime;
				origin.y += forward.y * SPEED_TRANSLATION * deltaTime;
				origin.z += forward.z * SPEED_TRANSLATION * deltaTime;
			}
			if (pKeyboardState[SDL_SCANCODE_D] || pKeyboardState[SDL_SCANCODE_RIGHT])
			{
				origin.x -= right.x * SPEED_TRANSLATION * deltaTime;
				origin.y -= right.y * SPEED_TRANSLATION * deltaTime;
				origin.z -= right.z * SPEED_TRANSLATION * deltaTime;
			}
			if (pKeyboardState[SDL_SCANCODE_A] || pKeyboardState[SDL_SCANCODE_LEFT])
			{
				origin.x += right.x * SPEED_TRANSLATION * deltaTime;
				origin.z += right.z * SPEED_TRANSLATION * deltaTime;
				origin.y += right.y * SPEED_TRANSLATION * deltaTime;
			}

			//Mouse Input
			const uint32_t mouseState = SDL_GetRelativeMouseState(&mouseX, &mouseY);

			bool holdingLeftMouseButton{ (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0 };
			bool holdingRightMouseButton{ (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0 };
			if (holdingRightMouseButton && holdingLeftMouseButton)
			{
				origin.y += up.y * mouseY * SPEED_TRANSLATION * deltaTime;
			}
			else if (holdingRightMouseButton)
			{
				totalPitch += mouseX * SPEED_ROTATION * deltaTime;
				totalYaw -= mouseY * SPEED_ROTATION * deltaTime;
			}
			else if (holdingLeftMouseButton)
			{
				origin.x += forward.x * mouseY * SPEED_TRANSLATION * deltaTime;
				origin.y += forward.y * mouseY * SPEED_TRANSLATION * deltaTime;
				origin.z += forward.z * mouseY * SPEED_TRANSLATION * deltaTime;

				totalPitch += mouseX * SPEED_ROTATION * deltaTime;
			}
			if (holdingLeftMouseButton != holdingRightMouseButton)
			{
				Matrix final{ Matrix::CreateRotationX(totalYaw) * Matrix::CreateRotationY(totalPitch) };
				forward = final.TransformVector(Vector3::UnitZ);
				forward.Normalize();
				cameraToWorld = CalculateCameraToWorld();
			}
		}
	};
}