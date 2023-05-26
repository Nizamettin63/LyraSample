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

	UFUNCTION()
	void DropItems();

	UFUNCTION()
	void DropCurrency();

	UFUNCTION()
		void OwnerDeathStarted(AActor* Owner);


public:


	UFUNCTION()
	void InitializeWithAbilitySystem(ULyraAbilitySystemComponent* ASC);

protected:

	UPROPERTY()
	TObjectPtr<ULyraAbilitySystemComponent> LyraAbilitySystemComponent;


	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Lyra|Currency ")
	TSubclassOf<AActor> GoldToSpawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lyra|Character", Meta = (AllowPrivateAccess = "true"))
	ULyraHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lyra|Character", Meta = (AllowPrivateAccess = "true"))
	ALyraCharacter* LyraCharacter;

	// Currency set used by this component.
	UPROPERTY()
	TObjectPtr<const ULyraCurrencySet> CurrencySet;

};
