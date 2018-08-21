// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"


USTRUCT()
struct FSpawnPosition
{
	GENERATED_USTRUCT_BODY()

	FVector Location;
	float Scale;
	float Rotation;
};

class UActorPool;
class APawn;

UCLASS()
class TESTINGGROUNDS_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	UFUNCTION(BlueprintCallable, Category = "Generating")
	void PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float Radius =200, float MinScale = 1, float MaxScale = 1);

	UFUNCTION(BlueprintCallable, Category = "Generating")
	void PlaceAIPawns(TSubclassOf<APawn> ToSpawn, int MinSpawn, int MaxSpawn, float Radius = 200);

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
	void PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnPosition& SpawnPosition);
	void PlaceAIPawn(TSubclassOf<APawn> ToSpawn, FSpawnPosition& SpawnPosition);
	void PositionNavMeshBoundsVolume();

	TArray<FSpawnPosition> RandomSpawnPositions(int MinSpawn, int MaxSpawn, float MinScale, float MaxScale, float Radius);

	AActor* NavMeshBoundsVolume;
	TArray<AActor*> SpawnedActors;
	UActorPool* ActorPoolReference;
};
