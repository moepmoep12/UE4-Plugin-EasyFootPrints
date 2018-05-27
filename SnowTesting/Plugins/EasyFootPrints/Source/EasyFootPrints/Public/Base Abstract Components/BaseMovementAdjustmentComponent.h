
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BaseMovementAdjustmentComponent.generated.h"


class UFootPrintComponent;

/* The abstract base class for MovementAdjustmentComponents. A MovementAdjustmentComponent changes the owners movement based on the material he's walking on.
* The class can be parent to blueprints and methods can be overriden in blueprint
*/

UCLASS(ClassGroup = (EasyFootPrints), meta = (IsBlueprintBase = "true"), abstract)
class EASYFOOTPRINTS_API UBaseMovementAdjustmentComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	// pointer to the owners movement component
	UPROPERTY()
		UMovementComponent* MovementComponent;

public:
	/// <summary>
	/// Adjusts the movement of the character
	/// </summary>
	/// <param name="depth">the depth of the material the character is walking on.</param>
	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "Adjust Movement", Keywords = "adjust movement footprint plugin"), Category = "EasyFootPrints")
		void adjustMovement(float depth);

	virtual void adjustMovement_Implementation(float depth) { adjustMovement(depth); };

	/* 
	* Called when the owner (e.g. a player) leaves a plugin-landscape. Used to restore orignal movement
	*/
	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "Reset Movement", Keywords = "reset movement footprint plugin"), Category = "EasyFootPrints")
		void resetMovement();

	virtual void resetMovement_Implementation() { resetMovement(); };


	// Stores the pointer to the owners MovementComponent for further adjustment
	inline virtual void initComponent(UMovementComponent* MovementComponent) 
	{
		this->MovementComponent = MovementComponent;
	};

};
