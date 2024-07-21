// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HUDOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


void UHUDOverlay::SetHealthBarPercent(float Percent)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(Percent);
	}
}

void UHUDOverlay::SetStaminaBarPercent(float Percent)
{
	if (StaminaBar)
	{
		StaminaBar->SetPercent(Percent);
	}
}

//void UHUDOverlay::SetXpBarPercent(float Percent)
//{
//	if (XpBar)
//	{
//		XpBar->SetPercent(Percent);
//	}
//}

void UHUDOverlay::SetGold(int32 Coin)
{
	if (Coins)
	{
		Coins->SetText(FText::FromString(FString::Printf(TEXT("%d"), Coin)));
	}
}

void UHUDOverlay::SetSouls(int32 Soul)
{
	if (Souls)
	{
		Souls->SetText(FText::FromString(FString::Printf(TEXT("%d"), Soul)));
	}
}
