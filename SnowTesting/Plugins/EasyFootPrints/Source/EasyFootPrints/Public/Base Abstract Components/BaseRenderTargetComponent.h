
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RenderTargetValues.h"
#include "BaseRenderTargetComponent.generated.h"

class UFootPrintComponent;

/* The abstract base class for RenderTargetComponents. A RenderTargetComponent draws a material onto a Rendertarget, a texture that used as a displacement map for tessellation displacement.
* The class can be parent to blueprints and methods can be overriden in blueprint
*/

UCLASS(ClassGroup = (EasyFootPrints), meta = (IsBlueprintBase = "true"), abstract)
class EASYFOOTPRINTS_API UBaseRenderTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	/* Used for drawing the FootPrintShape onto a RenderTarget
	@FootPrintShape: the material that is used for drawing
	@RenderTargetValues: a struct with data for calculating position, size etc. for drawing
	@ShapeTransform: transform that describes size, scale and rotation of the footprint shape
	*/
	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "Draw on RenderTarget", Keywords = "render target footprint plugin"), Category = "EasyFootPrints")
		void drawOnRenderTarget(UMaterialInterface * FootPrintShape, struct FRenderTargetValues& RenderTargetValues, FTransform ShapeTransform);

	virtual void drawOnRenderTarget_Implementation(UMaterialInterface * FootPrintShape, struct FRenderTargetValues& RenderTargetValues, FTransform ShapeTransform) {
		drawOnRenderTarget(FootPrintShape,RenderTargetValues,ShapeTransform);
	}




};
