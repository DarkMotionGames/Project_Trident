// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/HitInterface.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class AWeaponBase;
class UAttributeComponent;
class UAnimMontage;

UCLASS()
class PROJECT_TRIDENT_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetCollisionOnWeapon(ECollisionEnabled::Type CollisionType);

	UFUNCTION(BlueprintCallable)
	bool IsAlive();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UAttributeComponent* Attributes;
protected:
	virtual void BeginPlay() override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual void Attack();
	virtual void Die();
	virtual void EnableRagdoll();
	void DirectionalHitReact(const FVector& ImpactPoint);
	virtual void HandleDamage(float DamageAmount);
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
	int32 PlayAttackMontage();
	void  StopAttackMontage();



	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* HitParticles;

	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);

	bool bIsRagdoll = false;

	/*
	Play Montage Function
	*/

	virtual bool CanAttack();



	UFUNCTION(BlueprintCallable)
	void PlayHitReactMontage(const FName& SectionName);


	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	UFUNCTION(BlueprintCallable)
	virtual void DodgeEnd();

	UFUNCTION(BlueprintCallable)
	FVector GetTranslationWarpTarget();

	UFUNCTION(BlueprintCallable)
	FVector GetRotationWarpTarget();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere, Category = "Combat")
	double WarpTargetDistance = 75.f;


	/**
	* Animation montages
	*/
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWeaponBase* EquippedWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FName> AttackMontageSections;
};