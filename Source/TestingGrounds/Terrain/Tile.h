// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

class UActorPool;

UCLASS()
class TESTINGGROUNDS_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	UFUNCTION(BlueprintCallable, Category = "Generating")
	void PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float Radius =200, float MinScale = 1, float MaxScale = 1);

	UFUNCTION(BlueprintCallable, Category = "Pool")
	void SetPool(UActorPool* InPool);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	bool CanSpawnAtLocation(FVector Location, float Radius);
	bool FindEmptyLocation(FVector& OutLocation, float Radius);
	void PlaceActor(TSubclassOf<AActor>, FVector SpawnPoint, float Scale);
	void PositionNavMeshBoundsVolume();

	TArray<AActor*> SpawnedActors;

	UActorPool* ActorPoolReference;
};