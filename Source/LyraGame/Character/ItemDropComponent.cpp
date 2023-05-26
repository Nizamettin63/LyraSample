// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ItemDropComponent.h"

#include "LyraCharacter.h"
#include "Character/LyraHealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/LyraCurrencySet.h"


UItemDropComponent::UItemDropComponent(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	SetIsReplicatedByDefault(true);
}

void UItemDropComponent::InitializeWithAbilitySystem(ULyraAbilitySystemComponent* ASC)
{

	if (ASC)
	{
		LyraAbilitySystemComponent = ASC;
	}

	HealthComponent = GetOwner()->FindComponentByClass<ULyraHealthComponent>();
	if (HealthComponent)
	{
    	HealthComponent->OnDeathStarted.AddDynamic(this, &UItemDropComponent::OwnerDeathStarted);
	}

	CurrencySet = LyraAbilitySystemComponent->GetSet<ULyraCurrencySet>();
	
}

void UItemDropComponent::OwnerDeathStarted(AActor* Owner)
{

	DropCurrency();
	DropItems();
}


void UItemDropComponent::DropItems()
{
	// This section is available to drop item
 

}

void UItemDropComponent::DropCurrency()
{
	if (HasAuthority())
	{
		//Spawn as much gold as GoldToDrop Attibute 

		int GoldToDrop = CurrencySet->GetGoldToDrop();

		FVector SpawnLocation = GetOwner()->GetActorLocation();

		FActorSpawnParameters GoldSpawnParamaters;

		GoldSpawnParamaters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		for (int i = 0; i < GoldToDrop; i++)
		{
			GetWorld()->SpawnActor<AActor>(GoldToSpawn, SpawnLocation, FRotator(ForceInitToZero), GoldSpawnParamaters);
		}
	}
}


