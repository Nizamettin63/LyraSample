// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/LyraGameplayAbility_WallRun.h"

#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/Tasks/AbilityTask_WallRun.h"
#include "AbilitySystemBlueprintLibrary.h"



ULyraGameplayAbility_WallRun::ULyraGameplayAbility_WallRun()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}


void ULyraGameplayAbility_WallRun::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	ACharacter* CharacterAvatar = Cast<ACharacter>(ActorInfo->AvatarActor);

	UCapsuleComponent* CapsuleComponent = CharacterAvatar->GetCapsuleComponent();

	CapsuleComponent->OnComponentHit.AddDynamic(this, &ULyraGameplayAbility_WallRun::OnCapsuleComponentHit);
}

void ULyraGameplayAbility_WallRun::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	if (ActorInfo)
	{
		ACharacter* CharacterAvatar = Cast<ACharacter>(ActorInfo->AvatarActor);
		UCapsuleComponent* CapsuleComponent = CharacterAvatar->GetCapsuleComponent();

		CapsuleComponent->OnComponentHit.AddDynamic(this, &ULyraGameplayAbility_WallRun::OnCapsuleComponentHit);
	}

	Super::OnRemoveAbility(ActorInfo, Spec);
}

bool ULyraGameplayAbility_WallRun::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	ACharacter* AvatarCharacter = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	
	return AvatarCharacter && !AvatarCharacter->GetCharacterMovement()->IsMovingOnGround();
}

void ULyraGameplayAbility_WallRun::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor);

	UCharacterMovementComponent* CharacterMovement = Character->GetCharacterMovement();

	WallRunTickTask = UAbilityTask_WallRun::CreateWallRunTask(this, Character, CharacterMovement, WallRun_TraceObjectTypes);

	WallRunTickTask->OnFinished.AddDynamic(this, &ULyraGameplayAbility_WallRun::K2_EndAbility);
	WallRunTickTask->OnWallSideDetermened.AddDynamic(this, &ULyraGameplayAbility_WallRun::OnWallSideDetermened);

	WallRunTickTask->ReadyForActivation();
}

void ULyraGameplayAbility_WallRun::OnCapsuleComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->TryActivateAbility(GetCurrentAbilitySpec()->Handle);
	}
}

void ULyraGameplayAbility_WallRun::OnWallSideDetermened(bool bLeftSide)
{
	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());

	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();

	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();

	if (bLeftSide)
	{	
		AbilitySystemComponent->ApplyGameplayEffectToSelf(WallRunLeftSideEffectClass, 1.f, EffectContextHandle);
	}
	else
	{
		AbilitySystemComponent->ApplyGameplayEffectToSelf(WallRunRightSideEffectClass, 1.f, EffectContextHandle);
	}
}

void ULyraGameplayAbility_WallRun::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsValid(WallRunTickTask))
	{
		WallRunTickTask->EndTask();
	}

	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	/*
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(WallRunLeftSideEffectClass, AbilitySystemComponent);
		AbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(WallRunRightSideEffectClass, AbilitySystemComponent);
	}
	*/
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

