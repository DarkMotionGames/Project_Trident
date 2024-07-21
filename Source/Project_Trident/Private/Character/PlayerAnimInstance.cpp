// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerAnimInstance.h"
#include "Character/PlayerBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


void UPlayerAnimInstance::NativeInitializeAnimation()
{

	Super::NativeInitializeAnimation();

	PlayerCharacter = Cast<APlayerBase>(TryGetPawnOwner());

	if (PlayerCharacter)
	{
		PlayerMovement = PlayerCharacter->GetCharacterMovement();
	}

}



void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaTime)
{

	Super::NativeUpdateAnimation(DeltaTime);

	if (PlayerMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(PlayerMovement->Velocity);
		IsFalling = PlayerMovement->IsFalling();
		CharacterState = PlayerCharacter->GetCharacterState();
	}
}
