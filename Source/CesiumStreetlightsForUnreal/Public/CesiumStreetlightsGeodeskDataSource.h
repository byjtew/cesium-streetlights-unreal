#pragma once

#include "CoreMinimal.h"
#include "CesiumStreetlightsDataSource.h"
#include "Components/ActorComponent.h"
#include "CesiumStreetlightsGeodeskDataSource.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CESIUMSTREETLIGHTSFORUNREAL_API UCesiumStreetlightsGeodeskDataSource : public UCesiumStreetlightsDataSource
{
	GENERATED_BODY()

public:
	UCesiumStreetlightsGeodeskDataSource();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cesium Streetlights")
	FFilePath GolFilepath;

protected:
	virtual void RequestInternal(
		FCesiumStreetlightsRequestConfiguration             configuration,
		TFunction<void(FCesiumStreetlightsRequestResult&&)> on_results
	) override;

	TArray<FVector> LoadGeoPositions(const FCesiumStreetlightsRequestConfiguration& configuration) const;
};
