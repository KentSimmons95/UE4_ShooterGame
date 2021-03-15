// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Gun.h"
#include "SimpleShooterGameModeBase.h"
#include "TimerManager.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);
}

bool AShooterCharacter::IsDead() const
{
	return Health <= 0;
}

float AShooterCharacter::GetHealthPercent() const
{
	return Health/MaxHealth;
}

int32 AShooterCharacter::GetCurrentAmmo() 
{
	return Gun->GetCurrentAmmo();
}

int32 AShooterCharacter::GetAmmoPool() 
{
	return Gun->GetAmmoPool();
}

void AShooterCharacter::SetAmmo(int32 PickupAmount) 
{
	Gun->SetAmmoPickup(PickupAmount);
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Mouse+Keyboard AxisBindings
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Lookup"), this, &AShooterCharacter::Lookup);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	//Gamepad AxisBindings
	PlayerInputComponent->BindAxis(TEXT("LookupRate"), this, &AShooterCharacter::LookupRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AShooterCharacter::LookRight);
	//ActionBindings
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Shoot);
	PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Pressed, this, &AShooterCharacter::StartReload);
}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(Health, DamageToApply);
	Health -= DamageToApply;

	//Death + EndGame logic
	if (IsDead())
	{
		ASimpleShooterGameModeBase *GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	return DamageToApply;
}

void AShooterCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::Lookup(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

void AShooterCharacter::LookRight(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

void AShooterCharacter::LookupRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

bool AShooterCharacter::GetIsCurrentlyReloading() 
{
	return IsCurrentlyReloading;
}

void AShooterCharacter::Shoot()
{
	if (GetIsCurrentlyReloading() == true)
	{
		return;
	} 
	Gun->PullTrigger();
}

void AShooterCharacter::Reload() 
{
	Gun->Reload();	
	IsCurrentlyReloading = false;
}

void AShooterCharacter::StartReload() 
{
	FTimerHandle RestartTimer;
	bool CanReload = Gun->GetCanReload();
	float ReloadTime = Gun->GetReloadTime();

	if (CanReload)
	{
		IsCurrentlyReloading = true;
		GetWorldTimerManager().SetTimer(RestartTimer, this, &AShooterCharacter::Reload, ReloadTime, false);
	}
}

