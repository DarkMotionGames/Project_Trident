#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Character/CharacterTypes.h"
#include "Enemy.generated.h"

class UWHealthBarComponent;
class AAIController;
class UPawnSensingComponent;
class ASoul;


UCLASS()
class PROJECT_TRIDENT_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();

	//AActor Class
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;


	//Interface
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

	UPROPERTY(EditAnywhere, Category = "Combat")
	double AcceptanceCombatRadius = 50.f;

private:
	// AI Behavior

	void CheckPatrolTarget();
	void CheckCombatTarget();
	void PatrolTimerFinished();
	void ShowHealthBar();
	void HideHealthBar();
	void LooseIntrest();
	void StartPatroling();
	void ChaseTarget();
	bool IsAttacking() const;
	bool IsEngaged() const;
	bool IsOutsideCombatRadius();
	bool IsOutsideAttackRadius();
	bool IsInsideAttackRadius();
	bool IsDead();
	void StartAttackTimer();
	void ClearPatrolTimer();
	void ClearAttackTimer();
	void SpawnDefaultWeapon();
	void InitializeEnemy();


	UPROPERTY(VisibleAnywhere)
	UWHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;



	UPROPERTY(EditAnywhere, Category = "Combat")
	double CombatRadius = 500.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	double AttackRadius = 120.f;


	
	UPROPERTY(EditAnywhere, Category = "Combat")
	double PatrolRadius = 200.f;



	//Current Pattrol Target

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;

	UPROPERTY()
	AAIController* EnemyController;

	FTimerHandle PatrolTimer;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMinimum = 2.f;
	
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMaximum = 5.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWalkSpeed = 300.f;

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMin = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMax = 1.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float ChaseWalkSpeed = 490.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<ASoul> SoulClass;


protected:

	virtual void BeginPlay() override;
	virtual void Die() override;
	void SpawnSouls();
	virtual void Attack() override;
	virtual void HandleDamage(float DamageAmount) override;
	bool InTargetRange(AActor* Target, double AcceptanceRadius) const;
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();
	virtual bool CanAttack() override;
	virtual void AttackEnd() override;

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeaponBase> WeaponClass;


};