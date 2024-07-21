// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributeComponent.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UAttributeComponent::RegenStamina(float DeltaTime)
{
	Stamina = FMath::Clamp(Stamina + StaminaRegenRate * DeltaTime, 0.f, MaxStamina);
}

void UAttributeComponent::RegenHealth(float DeltaTime)
{
	Health = FMath::Clamp(Health + HealthRegenRate * DeltaTime, 0.f, MaxHealth);
}

//Recive Damage
void UAttributeComponent::ReciveDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
}

void UAttributeComponent::UseStamina(float StaminaCost)
{
	Stamina = FMath::Clamp(Stamina - StaminaCost, 0.f, MaxStamina);
}

void UAttributeComponent::UseMana(float ManaCost)
{
	Mana = FMath::Clamp(Mana - ManaCost, 0.f, MaxMana);
}

//Getting Health Percentage
float UAttributeComponent::GetHealthPercent()
{
	return Health / MaxHealth;
}

float UAttributeComponent::GetStaminaPercent()
{
	return Stamina / MaxStamina;
}

float UAttributeComponent::GetXpPercent()
{
	return Mana / MaxMana;
}

void UAttributeComponent::AddSouls(int32 NumberOfSouls)
{
	Soul += NumberOfSouls;
}

void UAttributeComponent::AddGold(int32 NumberOfGold)
{
	Gold += NumberOfGold;
}

//Is Alive or not
bool UAttributeComponent::IsAlive()
{
	return Health > 0.f;
}
