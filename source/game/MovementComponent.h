#ifndef GB_MOVEMENTCOMPONENT_H
#define GB_MOVEMENTCOMPONENT_H

#include "IComponent.h"
#include "Point.h"

namespace Direction
{
	static Point STATIONARY(0.0, 0.0);

	Point getNormalizedDirectionVector(Point fromActualPosition, Point toNode);
};

class MovementComponent : public IComponent
{
	public:
		MovementComponent(float maxSpeed, Point directionVector); 
		MovementComponent(float maxSpeed, float mass, float maxSteeringForce, float radius, float turnRate, Point directionVector); 
		virtual ~MovementComponent();
	public:
		virtual std::string name() const;
		
	public:
		float maxSpeed() const;
		float speed() const;
		Point directionVector() const;
		bool isMoving() const;
		float mass() const;
		float maxSteeringForce() const;
		float radius() const;
		float turnRate() const;

		void setSpeed(float speed);
		void setDirectionVector(Point directionVector);
		void setMovingStatus(bool isMoving);

	public:
		float m_maxSpeed;
		float m_speed;
		Point m_directionVector;
		bool m_isMoving;
		float m_mass;
		float m_maxSteeringForce;
		float m_radius;
		float m_turnRate;
};

#endif
