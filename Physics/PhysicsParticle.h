#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>
#include <chrono>
#include "../model3D/model3d.h"

namespace Koyu
{
	class PhysicsParticle
	{
	protected:
		glm::vec3 position;
		glm::vec3 velocity;
		glm::vec3 acceleration;
		float lifespan;
		model3D* model;
		const float GRAVITY = 10.f;

		void updatePosition(float deltaTime);
		void updateVelocity(float deltaTime);

	public:
		
		PhysicsParticle();
		PhysicsParticle(model3D* newModel);

		void setScale(float x, float y, float z);
		void setColor(glm::vec3 newColor);
		void setVelocity(glm::vec3 newVel);
		void setAcceleration(glm::vec3 newAccel);
		void setLifespan(float newLifespan);
		void update(float time);
		void draw();

		float getLifespan();
	};
}
