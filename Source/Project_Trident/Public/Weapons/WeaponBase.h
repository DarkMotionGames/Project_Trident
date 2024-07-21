// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "WeaponBase.generated.h"

class UBoxComponent;
/**
 * 
 */
UCLASS()
class PROJECT_TRIDENT_API AWeaponBase : public AItem
{
	GENERATED_BODY()
	
public:
	AWeaponBase();
	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);

	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);

	TArray<AActor*> IgnoreActors;

protected:

	virtual void BeginPlay() override;


	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void ExecuteBoxHit(FHitResult& BoxHit);

	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);

private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon Poperties")
	UBoxComponent* WeaponBox;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;

	UPROPERTY(EditAnywhere, Category = "Weapon Poperties")
	float DamageAmount = 10.f;

	UPROPERTY(EditAnywhere, Category = "Weapon Poperties")
	FVector BoxTraceExtent = FVector(4.f, 4.f, 4.f);

	UPROPERTY(EditAnywhere, Category = "Weapon Poperties")
	bool ShowBoxDebug = false;
public:
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }
};
