// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorPool.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UActorPool::UActorPool()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

AActor * UActorPool::Checkout()
{
	UE_LOG(LogTemp, Warning, TEXT("Checkout: %s"), *GetName());
	if (Pool.Num() == 0) {
		UE_LOG(LogTemp, Error, TEXT("Nothing to check out. Returned nullptr"));
		return nullptr;
	}
	return Pool.Pop();
}

void UActorPool::Return(AActor * ActorToReturn)
{
	if(ActorToReturn != nullptr)
		Add(ActorToReturn);
}

void UActorPool::Add(AActor * ActorToAdd)
{
	if(ActorToAdd != nullptr)
		Pool.Push(ActorToAdd);
}
