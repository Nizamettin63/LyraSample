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
	

	LyraCharacter = Cast<ALyraCharacter>(GetOwner());

	HealthComponent = GetOwner()->FindComponentByClass<ULyraHealthComponent>();

	HealthComponent->OnDeathStarted.AddDynamic(this, &ThisClass::OwnerDeathStarted);

	CurrencySet = LyraAbilitySystemComponent->GetSet<ULyraCurrencySet>();
	
}

void UItemDropComponent::OwnerDeathStarted(AActor* Owner)
{
	check(Owner);

	DropItems();

	DropCurrency();
}


void UItemDropComponent::DropItems()
{
	// This section is available to drop item
 

}

void UItemDropComponent::DropCurrency()
{
	float GoldToDrop = CurrencySet->GetGoldToDrop();

}

