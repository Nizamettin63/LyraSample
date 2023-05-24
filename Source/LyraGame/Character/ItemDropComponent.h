// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystem/Attributes/LyraCurrencySet.h"

#include "CoreMinimal.h"
#include "Components/GameFrameworkComponent.h"
#include "ItemDropComponent.generated.h"



class ULyraHealthComponent;
class ULyraCurrencySet;
class ULyraAbilitySystemComponent;
class ALyraCharacter;

UCLASS()
class LYRAGAME_API UItemDropComponent : public UGameFrameworkComponent
{
	GENERATED_BODY()

private:

	UItemDropComponent(const FObjectInitializer& ObjectInitializer);


	void InitializeWithAbilitySystem(ULyraAbilitySystemComponent* ASC);

	void DropItems();
	void DropCurrency();

	void OwnerDeathStarted(AActor* Owner);


protected:

	UPROPERTY()
	TObjectPtr<ULyraAbilitySystemComponent> LyraAbilitySystemComponent;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lyra|Character", Meta = (AllowPrivateAccess = "true"))
	ULyraHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lyra|Character", Meta = (AllowPrivateAccess = "true"))
	ALyraCharacter* LyraCharacter;

	// Currency set used by this component.
	UPROPERTY()
	TObjectPtr<const ULyraCurrencySet> CurrencySet;

};
