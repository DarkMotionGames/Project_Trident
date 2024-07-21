// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class USoundBase;

enum class EItemState : uint8
{
	EIS_Hovering,
	EIS_Equipped
};

UCLASS()
class PROJECT_TRIDENT_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	

	AItem();
	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ItemMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SinValues")
	float Amplitude = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SinValues")
	float TimeConst = 5.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SinValues")
	float RunningTime;

	UFUNCTION(BlueprintPure)
	float TransformSine();

	virtual void SpawnPickupEffect();
	virtual void SpawnPickupSound();

	template<typename T>
	T Avg(T First, T Second);

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	EItemState ItemState = EItemState::EIS_Hovering;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComp;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UNiagaraComponent* ItemEffect;
	
	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* PickupSound;

private:

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* PickupEffect;
};



template<typename T>
inline T AItem::Avg(T First, T Second)
{
	return T();
}
