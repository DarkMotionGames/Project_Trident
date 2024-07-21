// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDOverlay.generated.h"

class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECT_TRIDENT_API UHUDOverlay : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetHealthBarPercent(float Percent);
	void SetStaminaBarPercent(float Percent);
	//void SetXpBarPercent(float Percent);
	void SetGold(int32 Gold);
	void SetSouls(int32 Soul);

private:

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaBar;
	
	/*UPROPERTY(meta = (BindWidget))
	UProgressBar* XpBar;*/

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Coins;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Souls;
};
