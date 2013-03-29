#include "MovementComponent.h"

Point Direction::getNormalizedDirectionVector(Point fromActualPosition, Point toNode)
{
	double distance = calculateDistance(fromActualPosition, toNode);
	Point vector = Point (toNode.X() - fromActualPosition.X(), toNode.Y() - fromActualPosition.Y());
	Point normalizedDirectionVector = Point(vector.X()/distance, vector.Y()/distance);

	return normalizedDirectionVector;
}

MovementComponent::MovementComponent(float maxSpeed, Point directionVector) :
	m_maxSpeed(maxSpeed),
	m_speed(0.0),
	m_directionVector(directionVector),
	m_isMoving(false),
	m_mass(1.0),
	m_maxSteeringForce(1.0),
	m_radius(.5),
	m_turnRate(.2)
{
}

MovementComponent::MovementComponent(float maxSpeed, float mass, float maxSteeringForce, float radius, float turnRate, Point directionVector) :
	m_maxSpeed(maxSpeed),
	m_speed(0.0),
	m_directionVector(directionVector),
	m_isMoving(false),
	m_mass(mass),
	m_maxSteeringForce(maxSteeringForce),
	m_radius(radius),
	m_turnRate(turnRate)
{
}

MovementComponent::~MovementComponent()
{
}

std::string MovementComponent::name() const
{
	return "Movement";
}

float MovementComponent::maxSpeed() const
{
	return m_maxSpeed;
}

float MovementComponent::speed() const
{
	return m_speed;
}

Point MovementComponent::directionVector() const
{
	return m_directionVector;
}

bool MovementComponent::isMoving() const
{
	return m_isMoving;
}

float MovementComponent::mass() const
{
	return m_mass;
}

float MovementComponent::maxSteeringForce() const
{
	return m_maxSteeringForce;
}

float MovementComponent::radius() const
{
	return m_radius;
}

float MovementComponent::turnRate() const
{
	return m_turnRate;
}


void MovementComponent::setSpeed(float speed)
{
	m_speed = speed;
	if (speed > m_maxSpeed)
	{
		m_speed = m_maxSpeed;
	}
}

void MovementComponent::setDirectionVector(Point directionVector)
{
	m_directionVector = directionVector;
}

void MovementComponent::setMovingStatus(bool isMoving)
{
	m_isMoving = isMoving;
}
