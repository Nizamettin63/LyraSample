// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Tasks/AbilityTask_WallRun.h"

#include "GameFrameWork/Character.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"

void UAbilityTask_WallRun::Activate()
{
	Super::Activate();

	FHitResult OnWallHit;

	//const FVector CurrentAcceleration = CharacterMovement->GetCurrentAcceleration();

	if (!FindRunnableWall(OnWallHit))
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnFinished.Broadcast();
		}
		EndTask();

		return;
	}

	OnWallSideDetermened.Broadcast(IsWallOnTheLeft(OnWallHit));

	CharacterOwner->Landed(OnWallHit);

	CharacterOwner->SetActorLocation(OnWallHit.ImpactPoint + OnWallHit.ImpactNormal * 60);

	CharacterMovement->SetMovementMode(EMovementMode::MOVE_Flying);

	CharacterMovement->GravityScale = 0.2f;

}

UAbilityTask_WallRun* UAbilityTask_WallRun::CreateWallRunTask(UGameplayAbility* OwningAbility, ACharacter* InCharacteOwner, 
	UCharacterMovementComponent* InCharacterMovement, TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes)
{
	UAbilityTask_WallRun* WallRunTask = NewAbilityTask<UAbilityTask_WallRun>(OwningAbility);

	WallRunTask->CharacterMovement = InCharacterMovement;
	WallRunTask->CharacterOwner = InCharacteOwner;
	WallRunTask->bTickingTask = true;
	WallRunTask->WallRun_TraceObjectTypes = TraceObjectTypes;

	return WallRunTask;
}

void UAbilityTask_WallRun::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);


	FHitResult OnWallHit;

	const FVector CurrentAcceleration = CharacterMovement->GetCurrentAcceleration();

	if (!FindRunnableWall(OnWallHit))
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnFinished.Broadcast();
		}
		EndTask();

		return;
	}

	const FRotator DirectionRotator = IsWallOnTheLeft(OnWallHit) ? FRotator(0, -90, 0) : FRotator(0, 90, 0);

	const FVector WallRunDirection = DirectionRotator.RotateVector(OnWallHit.ImpactNormal);

	const float CachedZ = CharacterMovement->Velocity.Z;

	CharacterMovement->Velocity = WallRunDirection * 700.f;
	CharacterMovement->Velocity.Z = 0;

	CharacterOwner->SetActorRotation(WallRunDirection.Rotation());

	CharacterMovement->SetPlaneConstraintEnabled(true);
	CharacterMovement->SetPlaneConstraintOrigin(OnWallHit.ImpactPoint);
	CharacterMovement->SetPlaneConstraintNormal(OnWallHit.ImpactNormal);
}


bool UAbilityTask_WallRun::FindRunnableWall(FHitResult& OnWallHit)
{
	const FVector CharacterLocation = CharacterOwner->GetActorLocation();

	const FVector ForwardVector = CharacterOwner->GetActorForwardVector();
	const FVector RightVector = CharacterOwner->GetActorRightVector();

	const float TraceLenght = CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius() + 30.f;

	TArray<AActor*> ActorsToIgnore = { CharacterOwner };

	FHitResult TraceHit;

	if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), CharacterLocation, CharacterLocation + ForwardVector * TraceLenght,
		WallRun_TraceObjectTypes, true, ActorsToIgnore, EDrawDebugTrace::ForDuration, TraceHit, true))
	{
		return false;
	}

	if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), CharacterLocation, CharacterLocation + -RightVector * TraceLenght,
		WallRun_TraceObjectTypes, true, ActorsToIgnore, EDrawDebugTrace::ForDuration, OnWallHit, true))
	{
		if (FVector::DotProduct(OnWallHit.ImpactNormal, RightVector) > 0.3f)
		{
			return true;
		}
	}

	if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), CharacterLocation, CharacterLocation + RightVector * TraceLenght,
		WallRun_TraceObjectTypes, true, ActorsToIgnore, EDrawDebugTrace::ForDuration, OnWallHit, true))
	{
		if (FVector::DotProduct(OnWallHit.ImpactNormal, -RightVector) > 0.3f)
		{
			return true;
		}
	}

	return false;
}

bool UAbilityTask_WallRun::IsWallOnTheLeft(const FHitResult& InWallHit) const
{
	return  FVector::DotProduct(CharacterOwner->GetActorRightVector(), InWallHit.ImpactNormal) > 0.0f;
}


void UAbilityTask_WallRun::OnDestroy(bool bInOwnerFinished)
{

	CharacterMovement->SetPlaneConstraintEnabled(false);
	CharacterMovement->SetMovementMode(EMovementMode::MOVE_Falling);
	CharacterMovement->GravityScale = 1.75;

	Super::OnDestroy(bInOwnerFinished);
}


