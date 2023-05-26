// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Attributes/LyraCurrencySet.h"

#include "Net/UnrealNetwork.h"
#include "UObject/CoreNetTypes.h"
#include "NativeGameplayTags.h"
#include "Messages/LyraVerbMessage.h"
#include "Messages/LyraVerbMessageHelpers.h"
#include "NativeGameplayTags.h"
#include "Components/GameFrameworkComponentManager.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameFramework/PlayerState.h"
#include "Engine/World.h"
class FLifetimeProperty;


UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Lyra_Elimination_Message, "Lyra.PickupGold.Message");




ULyraCurrencySet::ULyraCurrencySet()
{
	TotalGold = 0;
	GoldToDrop = 3;
}


void ULyraCurrencySet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ULyraCurrencySet, TotalGold, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ULyraCurrencySet, GoldToDrop, COND_OwnerOnly, REPNOTIFY_Always);
}


void ULyraCurrencySet::OnRep_TotalGold(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULyraCurrencySet, TotalGold, OldValue);
}

void ULyraCurrencySet::OnRep_GoldToDrop(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULyraCurrencySet, GoldToDrop, OldValue);
}


void ULyraCurrencySet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

}




