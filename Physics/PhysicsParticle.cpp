#include "PhysicsParticle.h"

namespace Koyu
{
	PhysicsParticle::PhysicsParticle()
	{
		std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());
		std::uniform_int_distribution<> randomXZVel(-75, 75);
		std::uniform_int_distribution<> randomYVel(50, 200);
		std::uniform_int_distribution<> randomLifespan(1, 10);

		this->position = glm::vec3(0.f, -350.f, 0.f);
		this->velocity = glm::vec3(
						randomXZVel(rng),
						randomYVel(rng),
						randomXZVel(rng)
		);

		this->acceleration = glm::vec3(0.f, -GRAVITY, 0.f);
		this->lifespan = randomLifespan(rng);
		model = nullptr;
	}

	PhysicsParticle::PhysicsParticle(model3D* newModel) : PhysicsParticle::PhysicsParticle()
	{
		std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());
		std::uniform_int_distribution<> randomColor(0, 100);

		model = newModel;
		this->setColor(glm::vec3(
							(float(randomColor(rng)) / 100),
							(float(randomColor(rng)) / 100),
							(float(randomColor(rng)) / 100)
							)
		);
	}

	void PhysicsParticle::setScale(float x, float y, float z)
	{
		model->setScale(x, y, z);
	}

	void PhysicsParticle::setColor(glm::vec3 newColor)
	{
		model->setColor(newColor);
	}

	void PhysicsParticle::setVelocity(glm::vec3 newVel)
	{
		velocity = newVel;
	}

	void PhysicsParticle::setAcceleration(glm::vec3 newAccel)
	{
		acceleration = newAccel;
	}

	void PhysicsParticle::setLifespan(float newLifespan)
	{
		lifespan = newLifespan;
	}

	void PhysicsParticle::destroy()
	{
		isDestroyed = true;
	}

	bool PhysicsParticle::IsDestroyed()
	{
		return isDestroyed;
	}

	void PhysicsParticle::updatePosition(float deltaTime)
	{
		this->position = this->position + (this->velocity * deltaTime) + ((1.0f / 2.0f) * (this->acceleration * deltaTime * deltaTime));
	}

	void PhysicsParticle::updateVelocity(float deltaTime)
	{
		this->velocity = this->velocity + (this->acceleration * deltaTime);
	}

	void PhysicsParticle::update(float time)
	{
		this->updatePosition(time);
		this->updateVelocity(time);

		this->lifespan -= time;

		model->updatePosition(this->position);
	}

	void PhysicsParticle::draw()
	{
		model->draw();
	}
	float PhysicsParticle::getLifespan()
	{
		return lifespan;
	}
}
