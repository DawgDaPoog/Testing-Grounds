// Fill out your copyright notice in the Description page of Project Settings.


#include "MyInfiniteTerrainGameMode.h"
#include "GameFramework/Volume.h"
#include "EngineUtils.h"
#include "ActorPool.h"


AMyInfiniteTerrainGameMode::AMyInfiniteTerrainGameMode() 
{
	ActorPool = CreateDefaultSubobject<UActorPool>(FName("Nav Mesh Bound Volume Pool"));

}

void AMyInfiniteTerrainGameMode::PopulateBoundsVolumePool()
{
	auto VolumeIterator = TActorIterator<AVolume>(GetWorld());
	while (VolumeIterator)
		{
			AddToPool(*VolumeIterator);
			++VolumeIterator;
		}
}


void AMyInfiniteTerrainGameMode::AddToPool(AVolume * VolumeToAdd)
{
	auto VolumeName = VolumeToAdd->GetName();
	if(VolumeName.Contains(FString("NavMesh"),ESearchCase::CaseSensitive,ESearchDir::FromStart))
		UE_LOG(LogTemp, Warning, TEXT("Found NavMesh: %s"), *VolumeToAdd->GetName());
}


