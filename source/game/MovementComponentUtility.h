#ifndef GB_MOVEMENTCOMPONENTUTILITY_H
#define GB_MOVEMENTCOMPONENTUTILITY_H

#include "IDataStore.h"
#include "ObjectID.h"
#include "MovementComponent.h"

class MovementComponentUtility
{
	public:
	MovementComponentUtility();
	~MovementComponentUtility();

	static MovementComponent * getMovementComponent(IDataStore * dataStore, ObjectID id)
	{
		IComponent * component = dataStore->getComponent(id, "Movement");
		MovementComponent * movementComponent = dynamic_cast<MovementComponent *>(component);

		return movementComponent;
	}
};

#endif
