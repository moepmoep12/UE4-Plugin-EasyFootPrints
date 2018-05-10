#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysMaterial_EasyFootPrints.h"
#include "BasePollutionComponent.generated.h"

class UMaterialInterface;

/* The abstract base class for PollutionComponents. Every foot has its own PollutionComponent. It determines the amount of pollution a foot has and how much it increases/decreases.
* The class can be parent to blueprints and methods can be overriden in blueprint
*/

UCLASS(ClassGroup = (EasyFootPrints), meta = (IsBlueprintBase = "true"), abstract)
class EASYFOOTPRINTS_API UBasePollutionComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	// the amount of pollution of a foot
	UPROPERTY(meta = (ClampMin = "0.0", ClampMax = "1.0"))
		float Pollution = 0.0f;

public:

	/** Increases the amount of pollution of the foot this component belongs to
	@PhysMat: The physical material of the material the character is walking on */
	UFUNCTION(BluePrintNativeEvent, meta = (DisplayName = "Increase Foot Pollution", Keywords = "foot pollution increase footprint plugin"), Category = "EasyFootPrints")
		void increasePollution(UPhysMaterial_EasyFootPrints* PhysMat);

	virtual void increasePollution_Implementation(UPhysMaterial_EasyFootPrints* PhysMat) { increasePollution(PhysMat); };

	/** Spawns a pollution footprint as a decal actor with the specified material and transform
	@transform The transform for spawning the decal
	@Material: the material that will be used by the decal-actor
	@World: the current world to spawn in */
	UFUNCTION(BluePrintNativeEvent, meta = (DisplayName = "Create Pollution Footprint", Keywords = "foot pollution create footprint plugin"), Category = "EasyFootPrints")
		void createPollutionFootPrint(FTransform transform, UMaterialInstanceDynamic* Material, UWorld* World);

	virtual void createPollutionFootPrint_Implementation(FTransform transform, UMaterialInstanceDynamic* Material, UWorld* World) { 
		createPollutionFootPrint(transform,Material,World); 
	};

	/** Returns whether the foot is polluted  */
	UFUNCTION(BluePrintNativeEvent, meta = (DisplayName = "Has Pollution", Keywords = "foot pollution  footprint plugin"), Category = "EasyFootPrints")
		 bool  hasPollution() const;

	virtual bool hasPollution_Implementation() const { return hasPollution(); };

};
