#include "Character/PlayerBase.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Items/Item.h"
#include "Weapons/WeaponBase.h"
#include "Animation/AnimMontage.h"
#include "Kismet/GameplayStatics.h"
#include "HUD/MainHUD.h"
#include "HUD/HUDOverlay.h"
#include "Components/AttributeComponent.h"
#include "Items/Soul.h"
#include "Items/Treasure.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);


APlayerBase::APlayerBase()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f); // ...at this rotation rate

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}


void APlayerBase::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("Player"));

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		InitializeOverlayHud(PlayerController);
		
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void APlayerBase::Tick(float DeltaTime)
{
	if (Attributes && HudOverlay)
	{
		Attributes->RegenStamina(DeltaTime);
		HudOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
		Attributes->RegenHealth(DeltaTime);
		HudOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}

void APlayerBase::InitializeOverlayHud(APlayerController* PlayerController)
{
	AMainHUD* MainHud = Cast<AMainHUD>(PlayerController->GetHUD());

	if (MainHud)
	{
		HudOverlay = MainHud->GetHUDOverlay();
		if (HudOverlay && Attributes)
		{
			HudOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
			HudOverlay->SetStaminaBarPercent(.7f);
			//HudOverlay->SetXpBarPercent(.4f);
			HudOverlay->SetGold(0);
			HudOverlay->SetSouls(0);
		}
	}
}

float APlayerBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	SetHealthHud();
	return DamageAmount;
}

void APlayerBase::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;
}

void APlayerBase::AddSoul(ASoul* Soul)
{
	if (Attributes && HudOverlay)
	{
		Attributes->AddSouls(Soul->GetSouls());
		HudOverlay->SetSouls(Attributes->GetSoul());
	}
}

void APlayerBase::AddGold(ATreasure* Gold)
{
	if (Attributes && HudOverlay)
	{
		Attributes->AddGold(Gold->GetGolds());
		HudOverlay->SetGold(Attributes->GetGold());
	}
}

void APlayerBase::SetHealthHud()
{
	if (HudOverlay && Attributes)
	{
		HudOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}

///////////////////////////

void APlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerBase::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerBase::Look);
		
		// Interacting
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &APlayerBase::Interact);
		
		// Attacking
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APlayerBase::Attack);
		
		// Dodging
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &APlayerBase::Dodge);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

//Movement Logic
void APlayerBase::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (ActionState != EActionState::EAS_Unoccupied) return;
	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

//Look and turn around logic
void APlayerBase::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

//Interaction
void APlayerBase::Interact()
{
	AWeaponBase* OverlappingWeapon = Cast<AWeaponBase>(OverlappingItem);
	if (OverlappingWeapon)
	{
		if (EquippedWeapon)
		{
			EquippedWeapon->Destroy();
		}
		OverlappingWeapon->Equip(GetMesh(), FName("RightHand_Socket"), this, this);
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		OverlappingItem = nullptr;
		EquippedWeapon = OverlappingWeapon;
	}
	else
	{
		if (CanDisarm())
		{
			PlayEquipMontage(FName("Unequipped"));
			CharacterState = ECharacterState::ECS_Unequipped;
			ActionState = EActionState::EAS_EquppingWeapon;
		}
		else if (CanArm())
		{
			PlayEquipMontage(FName("Equip"));
			CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
			ActionState = EActionState::EAS_EquppingWeapon;
		}
	}
}

//Attacking
void APlayerBase::Attack()
{
	if (!HasEnoughStamina())
	{
		return;
	}
	else if (CanAttack())
			{
				PlayAttackMontage();
				if (Attributes && HudOverlay)
				{
					Attributes->UseStamina(Attributes->GetAttackCost());
					HudOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
				}
				ActionState = EActionState::EAS_Attacking;

				/*if (HudOverlay && Attributes)
				{
					HudOverlay->SetXpBarPercent(Attributes->GetXpPercent());
				}*/
	}	
}

bool APlayerBase::HasEnoughStamina()
{
	return Attributes && Attributes->GetStamina() > Attributes->GetAttackCost();
}

void APlayerBase::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	SetCollisionOnWeapon(ECollisionEnabled::NoCollision);
	if (Attributes && Attributes->GetHealthPercent() > 0.f)
	{
		ActionState = EActionState::EAS_HitReaction;
	}
}

//Dodging
void APlayerBase::Dodge()
{
	if (ActionState!=EActionState::EAS_Unoccupied || !HasEnoughStamina())
	{
		return;
	}

		PlayDodge();	

	ActionState=EActionState::EAS_Dodge;
	if (Attributes)
	{
		Attributes->UseStamina(Attributes->GetAttackCost());
	}
}

//Check if player can attack or not
bool APlayerBase::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

//Check if player can disarm weapon or not 
bool APlayerBase::CanDisarm() const
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

//Check if player can arm weapon or not
bool APlayerBase::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState == ECharacterState::ECS_Unequipped &&
		EquippedWeapon;
}

void APlayerBase::Die()
{
	Super::Die();
	ActionState = EActionState::EAS_Dead;
	HudOverlay->RemoveFromParent();
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.2f);
}

void APlayerBase::PlayDodge()
{
	PlayMontageSection(DodgeMontage,FName("Default"));
}


//Disarm weapon from character
void APlayerBase::Disarm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

//Arm weapon to the player
void APlayerBase::Arm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHand_Socket"));
	}
}

// Set finish equipping state
void APlayerBase::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void APlayerBase::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

//void APlayerBase::Die()
//{
//	Super::Die();
//}



//Play equip animation
void APlayerBase::PlayEquipMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

//When not attacking set the Action state to be unequipped
void APlayerBase::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void APlayerBase::DodgeEnd()
{		
	Super::DodgeEnd();
	ActionState=EActionState::EAS_Unoccupied;
}
