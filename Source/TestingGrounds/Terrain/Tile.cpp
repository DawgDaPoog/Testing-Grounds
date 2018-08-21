// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "ActorPool.h"
#include "NavigationSystem.h"
#include "GameFramework/Pawn.h"

// Sets default values
ATile::ATile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale )
{
	TArray<FSpawnPosition> SpawnPositions = RandomSpawnPositions(MinSpawn, MaxSpawn, MinScale, MaxScale, Radius);

	for (FSpawnPosition SpawnPosition : SpawnPositions)
	{
		PlaceActor(ToSpawn, SpawnPosition);
	}
}

void ATile::PlaceAIPawns(TSubclassOf<APawn> ToSpawn, int MinSpawn, int MaxSpawn, float Radius)
{
	TArray<FSpawnPosition> SpawnPositions = RandomSpawnPositions(MinSpawn, MaxSpawn, 1.f, 1.f, Radius);

	for (FSpawnPosition SpawnPosition : SpawnPositions)
	{
		PlaceAIPawn(ToSpawn, SpawnPosition);
	}
}


TArray<FSpawnPosition> ATile::RandomSpawnPositions(int MinSpawn, int MaxSpawn, float MinScale, float MaxScale, float Radius)
{
	TArray<FSpawnPosition> SpawnPositions;
	int NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	for (int i = 0; i < NumberToSpawn; i++) {
		FSpawnPosition SpawnPosition;
		SpawnPosition.Scale = FMath::RandRange(MinScale, MaxScale);
		SpawnPosition.Rotation = FMath::RandRange(-180.0f, 180.0f);
		if (FindEmptyLocation(SpawnPosition.Location, Radius * SpawnPosition.Scale))
			SpawnPositions.Add(SpawnPosition);
	}
	return SpawnPositions;
}

void ATile::SetPool(UActorPool * InPool)
{
	ActorPoolReference = InPool;
	PositionNavMeshBoundsVolume();
}

void ATile::PositionNavMeshBoundsVolume()
{
	NavMeshBoundsVolume = ActorPoolReference->Checkout();
	if (NavMeshBoundsVolume == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Not Enough Actors in Pool"));
		return;
	}
	else
		NavMeshBoundsVolume->SetActorLocation(GetActorLocation() + FVector(2000.0f, 0.f, 200.0f));
	
	FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld())->Build();
}



// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
}

void ATile::Destroyed()
{
	Super::Destroyed();
	for (AActor* Actor : SpawnedActors) {
		Actor->Destroy();
	}

	ActorPoolReference->Return(NavMeshBoundsVolume);
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

bool ATile::CanSpawnAtLocation(FVector Location, float Radius)
{
	FHitResult HitResult;
	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		Location,
		Location,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Radius)
	);
	FColor ResultColor = HasHit ? FColor::Red : FColor::Green;

	// Uncomment next line to draw debug lines for clarifying spawn check radii
	//DrawDebugSphere(GetWorld(), Location, Radius, 20, ResultColor, true, 100);
	return !HasHit;
}

bool ATile::FindEmptyLocation(FVector& OutLocation, float Radius)
{
	FVector Min(GetActorLocation().X, GetActorLocation().Y - 2000, GetActorLocation().Z + 130);
	FVector Max(GetActorLocation().X + 4000, GetActorLocation().Y + 2000, GetActorLocation().Z + 130);
	FBox Bounds(Min, Max);
	const static int MAX_ATTEMPTS = 30;
	for (size_t i = 0; i < MAX_ATTEMPTS; i++) {
		FVector SpawnPoint = FMath::RandPointInBox(Bounds);
		bool HasNotHit = CanSpawnAtLocation(SpawnPoint, Radius);
		if (HasNotHit) {
			OutLocation = SpawnPoint;
			return true;
		}
	}
	return false; 

}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnPosition& SpawnPosition)
{
	AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn, SpawnPosition.Location, FRotator(0, SpawnPosition.Rotation, 0), FActorSpawnParameters());
	if (Spawned != nullptr) {
		Spawned->SetActorScale3D(FVector(SpawnPosition.Scale));
		SpawnedActors.Add(Spawned);
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, false));
	}
}

void ATile::PlaceAIPawn(TSubclassOf<APawn> ToSpawn, FSpawnPosition & SpawnPosition)
{
	APawn* Spawned = GetWorld()->SpawnActor<APawn>(ToSpawn, SpawnPosition.Location, FRotator(0, SpawnPosition.Rotation, 0), FActorSpawnParameters());
	if (Spawned != nullptr) {
		Spawned->SpawnDefaultController();
		Spawned->Tags.Add(FName("Enemy"));
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, false));
	}
}

