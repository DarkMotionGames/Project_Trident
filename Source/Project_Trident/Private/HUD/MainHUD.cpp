// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MainHUD.h"
#include "HUD/HUDOverlay.h"

void AMainHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && HUDOverlayClass)
		{
			HUDOverlay = CreateWidget<UHUDOverlay>(Controller, HUDOverlayClass);
			HUDOverlay->AddToViewport();
		}
	}

	
}
