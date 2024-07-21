// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_TRIDENT_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void RegenStamina(float DeltaTime);
	void RegenHealth(float DeltaTime);

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Actor Stats")
	float Health = 100.f;
protected:
	virtual void BeginPlay() override;
		
private:

	
	UPROPERTY(EditAnywhere, Category = "Actor Stats")
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, Category = "Actor Stats")
	float Stamina = 100.f;
	
	UPROPERTY(EditAnywhere, Category = "Actor Stats")
	float MaxStamina = 100.f;

	UPROPERTY(EditAnywhere, Category = "Actor Stats")
	float Mana = 100.f;
	
	UPROPERTY(EditAnywhere, Category = "Actor Stats")
	float MaxMana = 100.f;
	
	UPROPERTY(EditAnywhere, Category = "Actor Stats")
	float AttackCost = 15.f;
	
	UPROPERTY(EditAnywhere, Category = "Actor Stats")
	int32 Gold;
	
	UPROPERTY(EditAnywhere, Category = "Actor Stats")
	int32 Soul;

	UPROPERTY(EditAnywhere, Category = "Actor Stats")
	float StaminaRegenRate = 8.f;
	
	UPROPERTY(EditAnywhere, Category = "Actor Stats")
	float HealthRegenRate = 1.f;

public:

	void ReciveDamage(float Damage);
	void UseStamina(float StaminaCost);
	void UseMana(float ManaCost);
	float GetHealthPercent();
	float GetStaminaPercent();
	float GetXpPercent();

	void AddSouls(int32 NumberOfSouls);
	void AddGold(int32 NumberOfGold);

	FORCEINLINE int32 GetGold() const { return Gold; }
	FORCEINLINE int32 GetSoul() const { return Soul; }
	FORCEINLINE float GetAttackCost() const { return AttackCost; }
	FORCEINLINE float GetStamina() const { return Stamina; }

	UFUNCTION(BlueprintCallable)
	bool IsAlive();
};
