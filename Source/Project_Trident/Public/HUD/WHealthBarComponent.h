// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "WHealthBarComponent.generated.h"

class UHealthBar;

/**
 * 
 */
UCLASS()
class PROJECT_TRIDENT_API UWHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	void SetUpHealthPercent(float Percent);

public:
	UPROPERTY()
	UHealthBar* HealthBarWidget;
};
