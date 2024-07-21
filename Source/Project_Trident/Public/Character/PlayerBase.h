// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Logging/LogMacros.h"
#include "InputActionValue.h"
#include "CharacterTypes.h"
#include "Interfaces/PickupInterface.h"
#include "PlayerBase.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class AItem;
class UAnimMontage;
class UHUDOverlay;
class ASoul;
class ATreasure;


DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config = Game)
class APlayerBase : public ABaseCharacter, public IPickupInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DodgeAction;


public:

	APlayerBase();

	virtual void Tick(float DeltaTime) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void SetOverlappingItem(AItem* Item) override;
	virtual void AddSoul(ASoul* Soul) override;
	virtual void AddGold(ATreasure* Gold) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExpSystem")
	int32 CurrentLevel = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExpSystem")
	int32 CurrentExp = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExpSystem")
	int32 ExpForNextLevel = 100;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExpSystem")
	float NextExpMultiplier = 1.5f;


protected:

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;



	////////////////////////////////////////////////////////

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for Interacting input */
	void Interact();

	/** Called for Attacking input */
	virtual void Attack() override;

	bool HasEnoughStamina();

	/** Called for Dodging input */
	void Dodge();

	/*
	Play Montage Function
	*/
	virtual void AttackEnd() override;
	virtual void DodgeEnd() override;
	virtual bool CanAttack() override;

	void PlayEquipMontage(const FName& SectionName);
	bool CanDisarm() const;
	bool CanArm();
	virtual void Die() override;
	void PlayDodge();

	UFUNCTION(BlueprintCallable)
	void Disarm();
	
	UFUNCTION(BlueprintCallable)
	void Arm();
	
	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	UFUNCTION(BlueprintCallable)
	void HitreactEnd();

	//virtual void Die() override;

private:

	void InitializeOverlayHud(APlayerController* PlayerController);
	void SetHealthHud();

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DodgeMontage;

	UPROPERTY()
	UHUDOverlay* HudOverlay;



public:

	//Public Getter functions
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
};