// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WallRun.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWallRunWallSideDetermenedDelegate, bool, bLeftSide);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWallRunFinishedDelegate);


class ACharacter;
class UCharacterMovementComponent;
class UGameplayAbility;

UCLASS()
class LYRAGAME_API UAbilityTask_WallRun : public UAbilityTask
{
	GENERATED_BODY()
	

public:

	UPROPERTY(BlueprintAssignable)
	FOnWallRunFinishedDelegate OnFinished;


	UPROPERTY(BlueprintAssignable)
	FOnWallRunWallSideDetermenedDelegate OnWallSideDetermened;

	UFUNCTION(BlueprintCallable, Category = "AbilityTask", meta = (HiddenPine = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UAbilityTask_WallRun* CreateWallRunTask(UGameplayAbility* OwningAbility, ACharacter* InCharacteOwner,
		UCharacterMovementComponent* InCharacterMovement, TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes);


	virtual void Activate() override;

	virtual void OnDestroy(bool bInOwnerFinished) override;

	virtual void TickTask(float DeltaTime) override;

protected:

	UCharacterMovementComponent* CharacterMovement = nullptr;

	ACharacter* CharacterOwner = nullptr;

	TArray<TEnumAsByte<EObjectTypeQuery>> WallRun_TraceObjectTypes;

	bool FindRunnableWall(FHitResult& OnWallHit);

	bool IsWallOnTheLeft(const FHitResult& InWallHit) const;

};
