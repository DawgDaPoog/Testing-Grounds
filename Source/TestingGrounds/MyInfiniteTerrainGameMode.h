// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TestingGrounds.h"
#include "TestingGroundsGameMode.h"
#include "MyInfiniteTerrainGameMode.generated.h"

/**
*
*/

class AVolume;
class UActorPool;

UCLASS()
class TESTINGGROUNDS_API AMyInfiniteTerrainGameMode : public ATestingGroundsGameMode
{
	GENERATED_BODY()


public:
	AMyInfiniteTerrainGameMode();

	UFUNCTION(BlueprintCallable, Category = "Bounds Pool")
	void PopulateBoundsVolumePool();

private:
	void AddToPool(AVolume *VolumeToAdd);

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "NavMeshBoundsVolume Pool")
	UActorPool* ActorPool;
};
