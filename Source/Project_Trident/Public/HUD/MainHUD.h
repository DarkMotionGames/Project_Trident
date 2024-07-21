// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

class UHUDOverlay;

/**
 * 
 */
UCLASS()
class PROJECT_TRIDENT_API AMainHUD : public AHUD
{
	GENERATED_BODY()

public: 

	FORCEINLINE UHUDOverlay* GetHUDOverlay() const { return HUDOverlay; }

private:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHUDOverlay> HUDOverlayClass;

	UPROPERTY()
	UHUDOverlay* HUDOverlay;
	
protected:

	virtual void BeginPlay() override;
};
