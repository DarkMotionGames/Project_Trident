// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponBase.h"
#include "Character/PlayerBase.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/HitInterface.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"

AWeaponBase::AWeaponBase()
{
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	WeaponBox->SetupAttachment(GetRootComponent());
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	BoxTraceStart->SetupAttachment(GetRootComponent());

	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());
}

//BeginPlay
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::OnBoxOverlap);
}


// Equiping State
void AWeaponBase::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	AttachMeshToSocket(InParent, InSocketName);
	ItemState = EItemState::EIS_Equipped;

	if (ItemEffect)
	{
		ItemEffect->Deactivate();
	}
}

//Attachin Weapon mesh to parent socket
void AWeaponBase::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
	if (SphereComp)
	{
		SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

//Weapon on overlap event
void AWeaponBase::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetOwner()->ActorHasTag(TEXT("Enemy")) && OtherActor->ActorHasTag(TEXT("Enemy")))
	{
		return;
	}
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetOwner());

	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	FHitResult BoxHit;
	UKismetSystemLibrary::BoxTraceSingle(this, Start, End, BoxTraceExtent, BoxTraceStart->GetComponentRotation(), ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, ShowBoxDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None, BoxHit, true);
	
	if (BoxHit.GetActor())
	{
		if (GetOwner()->ActorHasTag(TEXT("Enemy")) && BoxHit.GetActor()->ActorHasTag(TEXT("Enemy")))
		{
			return;
		}
		UGameplayStatics::ApplyDamage(BoxHit.GetActor(), DamageAmount, GetInstigator()->GetController(), this, UDamageType::StaticClass());

		ExecuteBoxHit(BoxHit);
		IgnoreActors.AddUnique(BoxHit.GetActor());

		CreateFields(BoxHit.ImpactPoint);

	}
}

void AWeaponBase::ExecuteBoxHit(FHitResult& BoxHit)
{
	IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
	if (HitInterface)
	{
		HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint, GetOwner());

	}
}
