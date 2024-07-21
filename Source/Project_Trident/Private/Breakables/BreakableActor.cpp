// Fill out your copyright notice in the Description page of Project Settings.


#include "Breakables/BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Items/Treasure.h"

// Sets default values
ABreakableActor::ABreakableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BreakGeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Geometry Collection"));
	SetRootComponent(BreakGeometryCollection);
	BreakGeometryCollection->SetGenerateOverlapEvents(true);
	BreakGeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

// Called when the game starts or when spawned
void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

//Response on GET HIT by actor
void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{	
	if (bBroken) return;
	bBroken = true;
	UWorld* World = GetWorld();
	if (World && TreasureClasses.Num() > 0)
	{
		const int32 Selection = FMath::RandRange(0, TreasureClasses.Num() - 1);
		FVector Location = GetActorLocation();
		Location.Z += 75.f;
		World->SpawnActor<ATreasure>(TreasureClasses[Selection], Location, GetActorRotation());
	}
	
}

