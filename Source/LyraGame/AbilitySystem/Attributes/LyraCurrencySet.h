// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemComponent.h"
#include "Misc/AssertionMacros.h"
#include "UObject/Class.h"
#include "UObject/UObjectGlobals.h"

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/LyraAttributeSet.h"
#include "LyraCurrencySet.generated.h"

/**
 * 
 */
UCLASS()
class LYRAGAME_API ULyraCurrencySet : public ULyraAttributeSet
{
	GENERATED_BODY()
public:

	ULyraCurrencySet();

	ATTRIBUTE_ACCESSORS(ULyraCurrencySet, TotalGold);
	ATTRIBUTE_ACCESSORS(ULyraCurrencySet, GoldToDrop);

protected:

	UFUNCTION()
		void OnRep_TotalGold(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		void OnRep_GoldToDrop(const FGameplayAttributeData& OldValue);

private:

	// The base amount of damage to apply in the damage execution.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_TotalGold, Category = "Lyra|Combat", Meta = (AllowPrivateAccess = true))
		FGameplayAttributeData TotalGold;

	// The base amount of healing to apply in the heal execution.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_GoldToDrop, Category = "Lyra|Combat", Meta = (AllowPrivateAccess = true))
		FGameplayAttributeData GoldToDrop;
	
};
