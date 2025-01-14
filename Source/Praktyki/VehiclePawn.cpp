// Copyright 2023 Teyon. All Rights Reserved.


#include "VehiclePawn.h"
#include "ChaosVehicleMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h" 
#include "InputActionValue.h"
#include "Components/AudioComponent.h"
#include "Components/PointLightComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/DecalComponent.h"
#include "PraktykiGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

AVehiclePawn::AVehiclePawn()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 600;
	SpringArm->SetRelativeLocation(FVector(0, 0, 140));
	SpringArm->SetRelativeRotation(FRotator(-10, 0, 0));
	SpringArm->bUsePawnControlRotation = true;

	SpringArm2 = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm2"));
	SpringArm2->SetupAttachment(GetMesh());
	SpringArm2->TargetArmLength = 700;
	SpringArm2->SetRelativeLocation(FVector(0, 0, 140));
	SpringArm2->SetRelativeRotation(FRotator(-10, 0, 0));
	SpringArm2->bUsePawnControlRotation = true;

	SpringArm3 = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm3"));
	SpringArm3->SetupAttachment(RootComponent);
	SpringArm3->TargetArmLength = 0;
	SpringArm3->SetRelativeLocation(FVector(0, 0, 0));
	SpringArm3->SetRelativeRotation(FRotator(0, 0, 0));
	SpringArm3->bUsePawnControlRotation = false;
	
	Camera1 = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera1"));
	Camera1->SetupAttachment(SpringArm);

	Camera2 = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera2"));
	Camera2->SetupAttachment(SpringArm2);

	Camera3 = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera3"));
	Camera3->SetupAttachment(GetMesh(), FName("CarAttachedSpectatorWingCamera"));	
	
	Camera4 = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera4"));
	Camera4->SetupAttachment(SpringArm3);

	EngineSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Engine Sound"));
	EngineSound->SetupAttachment(GetMesh());

	ExhaustLeft = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Exhaust Left VFX"));
	ExhaustLeft->SetupAttachment(GetMesh(), FName("VfxMainExhaust"));

	ExhaustRight = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Exhaust Right VFX"));
	ExhaustRight->SetupAttachment(GetMesh(), FName("VfxSecondExhaust"));
	
	SteeringWheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SteeringWheel"));
	SteeringWheelMesh->SetupAttachment(GetMesh(), FName(TEXT("SteeringWheel")));

	RearLeftLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("RearLeftLight"));
	RearLeftLight->SetupAttachment(GetMesh(), FName(TEXT("RearLeftLight")));
	RearLeftLight->LightColor = FColor::Red;
	
	RearRightLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("RearRightLight"));
	RearRightLight->SetupAttachment(GetMesh(), FName(TEXT("RearRightLight")));
	RearRightLight->LightColor = FColor::Red;
	
	RearCenterLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("RearCenterLight"));
	RearCenterLight->SetupAttachment(GetMesh(), FName(TEXT("RearCenterLight")));
	RearCenterLight->LightColor = FColor::Red;

	NS_RR_Trail = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Rear Right Trail"));
	NS_RR_Trail->SetupAttachment(GetMesh(), FName(TEXT("WheelRearRightTireSocket")));	
	
	NS_RL_Trail = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Rear Left Trail"));
	NS_RL_Trail->SetupAttachment(GetMesh(), FName(TEXT("WheelRearLeftTireSocket")));

	NS_FL_Trail = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Front Left Trail"));
	NS_FL_Trail->SetupAttachment(GetMesh(), FName(TEXT("WheelFrontLeftTireSocket")));
	
	NS_FR_Trail = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Front Right Trail"));
	NS_FR_Trail->SetupAttachment(GetMesh(), FName(TEXT("WheelFrontRightTireSocket")));

	BumperFront = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bumper Front"));
	BumperFront->SetupAttachment(GetMesh(), FName(TEXT("bumper_front")));
	
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(GetMesh(), FName(TEXT("SK_Porsche_911_GT3_R_Bone")));
	
	HoodFront = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HoodFront"));
	HoodFront->SetupAttachment(GetMesh(), FName(TEXT("hood_front")));
	
	FenderLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FenderLeft"));
	FenderLeft->SetupAttachment(GetMesh(), FName(TEXT("fender_left")));

	FenderRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FenderRight"));
	FenderRight->SetupAttachment(GetMesh(), FName(TEXT("fender_right")));

	DoorRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorRight"));
	DoorRight->SetupAttachment(GetMesh(), FName(TEXT("door_right")));
	
	DoorLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorLeft"));
	DoorLeft->SetupAttachment(GetMesh(), FName(TEXT("door_left")));
	
	Window = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Window"));
	Window->SetupAttachment(GetMesh(), FName(TEXT("SK_Porsche_911_GT3_R_Bone")));	
	
	BootRear = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BootRear"));
	BootRear->SetupAttachment(GetMesh(), FName(TEXT("boot_rear")));
	
	BumperRear = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BumperRear"));
	BumperRear->SetupAttachment(GetMesh(), FName(TEXT("bumper_rear")));
	
	SpoilerBack = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpoilerBack"));
	SpoilerBack->SetupAttachment(GetMesh(), FName(TEXT("spoiler_back")));

	DamageMaterial = CreateDefaultSubobject<UMaterialInstance>("DamageMaterial");
	DamageLightMaterial = CreateDefaultSubobject<UMaterialInstance>("DamageLightMaterial");
	DamageWindowMaterial = CreateDefaultSubobject<UMaterialInstance>("DamageWindowMaterial");

	DamageTake = 0;
}

void AVehiclePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UChaosWheeledVehicleMovementComponent* vehicleComponent = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent());

	if (!vehicleComponent)
		return;

	float rpm = vehicleComponent->GetEngineRotationSpeed();

	EngineSound->SetFloatParameter(FName("RPM"), rpm);

	if (rpm > 1500 && rpm < 4500)
		IncreasedSmokeExhaust();
	else
		DecreasedSmokeExhaust();

	SteeringInput = vehicleComponent->GetSteeringInput();
	UpdateSteeringWheelRotation(SteeringInput);

	CurrentLapTime = GetWorld()->TimeSeconds;

	APraktykiGameModeBase* GameMode = GetWorld()->GetAuthGameMode<APraktykiGameModeBase>();
	if (GameMode)
		GameMode->LapManager(this, NumberOfLaps, CurrentLapTime);
}

void AVehiclePawn::BeginPlay()
{
	Super::BeginPlay();
	TurnRearLights(false);
	ActiveCameraIndex = 0;
	GetMesh()->OnComponentHit.AddDynamic(this, &AVehiclePawn::OnHit);
	this->OnTakeAnyDamage.AddDynamic(this, &AVehiclePawn::OnTakeDamage);
	GetMesh()->SetNotifyRigidBodyCollision(true);
	GetWorldTimerManager().SetTimer(MaxLapTimeHandle, this, &AVehiclePawn::TimeUp, MaxLapTime);
}

void AVehiclePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (auto PlayerController = Cast<APlayerController>(Controller))
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			Subsystem->AddMappingContext(InputMapping, 0);

	UEnhancedInputComponent* input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	input->BindAction(HandBrakeAction, ETriggerEvent::Triggered, this, &AVehiclePawn::OnHandBrakePressed);
	input->BindAction(HandBrakeAction, ETriggerEvent::Completed, this, &AVehiclePawn::OnHandBrakeReleased);
	input->BindAction(ThrottleAction, ETriggerEvent::Started, this, &AVehiclePawn::MoveForward);
	input->BindAction(ThrottleAction, ETriggerEvent::Completed, this, &AVehiclePawn::OnThrottleReleased);
	input->BindAction(BrakeAction, ETriggerEvent::Triggered, this, &AVehiclePawn::OnBrakePressed);
	input->BindAction(BrakeAction, ETriggerEvent::Completed, this, &AVehiclePawn::OnBrakeReleased);
	input->BindAction(LookAroundAction, ETriggerEvent::Triggered, this, &AVehiclePawn::LookAround);
	input->BindAction(LookUpDownAction, ETriggerEvent::Triggered, this, &AVehiclePawn::LookUpDown);
	input->BindAction(SteeringAction, ETriggerEvent::Triggered, this, &AVehiclePawn::Steering);
	input->BindAction(SteeringAction, ETriggerEvent::Completed, this, &AVehiclePawn::SteeringReleased);
	input->BindAction(SwitchCameraAction, ETriggerEvent::Triggered, this, &AVehiclePawn::SwitchCamera);
	input->BindAction(InteriorCameraLookAction, ETriggerEvent::Triggered, this, &AVehiclePawn::InteriorCameraLook);
	input->BindAction(InteriorCameraLookAction, ETriggerEvent::Completed, this, &AVehiclePawn::InteriorCameraLookReleased);
	input->BindAction(RestartAction, ETriggerEvent::Triggered, this, &AVehiclePawn::RestartLevel);
}



void AVehiclePawn::MoveForward(const FInputActionValue& value)
{
	GetVehicleMovementComponent()->SetThrottleInput(value.Get<float>());
}

void AVehiclePawn::OnBrakePressed(const FInputActionValue& value)
{
	GetVehicleMovementComponent()->SetBrakeInput(value.Get<float>() / 1.5);
	TurnRearLights(true);
	
	if (GetVehicleMovementComponent()->GetForwardSpeed() < 20)
		return;

	if(value.Get<float>() > 0  && !NS_FR_Trail->IsActive())
		ActivateTrails(false);

	if (value.Get<float>() == 0 && NS_FR_Trail->IsActive())
		DeactivateTrails();
}

void AVehiclePawn::OnBrakeReleased(const FInputActionValue& value)
{
	GetVehicleMovementComponent()->SetBrakeInput(0);
	TurnRearLights(false);
	DeactivateTrails();
}

void AVehiclePawn::LookAround(const FInputActionValue& value)
{
	AddControllerYawInput(value.Get<float>());
}

void AVehiclePawn::LookUpDown(const FInputActionValue& value)
{
	AddControllerPitchInput(value.Get<float>() / -10);
}

void AVehiclePawn::InteriorCameraLook()
{
	SpringArm3->bUsePawnControlRotation = true;
}

void AVehiclePawn::InteriorCameraLookReleased()
{
	SpringArm3->bUsePawnControlRotation = false;
}

void AVehiclePawn::Steering(const FInputActionValue& value)
{
	GetVehicleMovementComponent()->SetSteeringInput(value.Get<float>());
}

void AVehiclePawn::SteeringReleased()
{
	GetVehicleMovementComponent()->SetSteeringInput(0);
}

void AVehiclePawn::OnHandBrakePressed()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
	TurnRearLights(true);
	ActivateTrails(true);
}

void AVehiclePawn::OnHandBrakeReleased()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
	TurnRearLights(false);
	DeactivateTrails();
}

void AVehiclePawn::OnThrottleReleased()
{
	GetVehicleMovementComponent()->SetThrottleInput(0);

}

void AVehiclePawn::TurnRearLights(bool value)
{
	RearLeftLight->SetVisibility(value);
	RearCenterLight->SetVisibility(value);
	RearRightLight->SetVisibility(value);
}

void AVehiclePawn::IncreasedSmokeExhaust()
{
	ExhaustLeft->SetFloatParameter(FName("SpawnRate"), 150);
	ExhaustRight->SetFloatParameter(FName("SpawnRate"), 150);
}

void AVehiclePawn::DecreasedSmokeExhaust()
{
	ExhaustLeft->SetFloatParameter(FName("SpawnRate"), 20);
	ExhaustRight->SetFloatParameter(FName("SpawnRate"), 20);
}

void AVehiclePawn::SwitchCamera()
{
	ActiveCameraIndex = (ActiveCameraIndex + 1) % 4;
	FRotator NewControlRotation = Controller->GetControlRotation();

	switch (ActiveCameraIndex)
	{
	case 0:
		Camera1->SetActive(true);
		Camera4->SetActive(false);
		NewControlRotation.Yaw = GetActorRotation().Yaw;
		NewControlRotation.Pitch = GetActorRotation().Pitch;
		Controller->SetControlRotation(NewControlRotation);
		break;
	case 1:
		Camera2->SetActive(true);
		Camera1->SetActive(false);
		NewControlRotation.Yaw = GetActorRotation().Yaw;
		NewControlRotation.Pitch = GetActorRotation().Pitch;
		Controller->SetControlRotation(NewControlRotation);
		break;
		
	case 2:
		Camera3->SetActive(true);
		Camera2->SetActive(false);
		break;	
	case 3:
		Camera4->SetActive(true);
		Camera3->SetActive(false);
		break;
	}
}

void AVehiclePawn::UpdateSteeringWheelRotation(float steeringInput)
{
	FRotator NewRotation = FRotator(0.f, 0.0f, SteeringInput * 45.f);
	SteeringWheelMesh->SetRelativeRotation(NewRotation);
}

void AVehiclePawn::ActivateTrails(bool isHandbrake)
{
	NS_RR_Trail->Activate();
	NS_RL_Trail->Activate();

	if (isHandbrake)
		return;

	NS_FR_Trail->Activate();
	NS_FL_Trail->Activate();
}

void AVehiclePawn::DeactivateTrails()
{
	NS_RR_Trail->Deactivate();
	NS_RL_Trail->Deactivate();
	NS_FR_Trail->Deactivate();
	NS_FL_Trail->Deactivate();
}

void AVehiclePawn::RestartLevel()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void AVehiclePawn::TimeUp()
{
	APraktykiGameModeBase* GameMode = GetWorld()->GetAuthGameMode<APraktykiGameModeBase>();

	if (GameMode)
		GameMode->TimeUp(this);
}

void AVehiclePawn::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->GetName() == TEXT("BP_Car_C_0") || OtherActor->ActorHasTag(FName(TEXT("Track"))))
		return;

	OnTakeDamage(this, 10, nullptr, GetController(), this);
}

void AVehiclePawn::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	
	if (DamageTake >= 0 && DamageTake <= 3) 
	{
		BumperFront->SetMaterial(0, DamageMaterial);
		HoodFront->SetMaterial(0, DamageMaterial);
		FenderLeft->SetMaterial(0, DamageMaterial);
		FenderRight->SetMaterial(0, DamageMaterial);
		FenderLeft->SetMaterial(2, DamageLightMaterial);
		FenderRight->SetMaterial(3, DamageLightMaterial);
	}
	
	if (DamageTake > 3 && DamageTake <= 5)
	{
		Body->SetMaterial(0, DamageMaterial);
		Body->SetMaterial(5, DamageWindowMaterial);
		DoorLeft->SetMaterial(0, DamageMaterial);
		DoorRight->SetMaterial(0, DamageMaterial);	
		DoorLeft->SetMaterial(3, DamageWindowMaterial);
		DoorRight->SetMaterial(3, DamageWindowMaterial);
		Window->SetMaterial(0, DamageWindowMaterial);

		if (UChaosWheeledVehicleMovementComponent* VehicleMovement = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement()))
			VehicleMovement->EngineSetup.MaxTorque = 250;
	}
	
	if (DamageTake > 6) 
	{
		BumperRear->SetMaterial(0, DamageMaterial);
		BumperRear->SetMaterial(2, DamageLightMaterial);
		BootRear->SetMaterial(0, DamageMaterial);
		SpoilerBack->SetMaterial(0, DamageMaterial);
		APraktykiGameModeBase* GameMode = GetWorld()->GetAuthGameMode<APraktykiGameModeBase>();

		if (GameMode) 
		{
			GameMode->DestroyedCar(this);
			GetVehicleMovementComponent()->SetThrottleInput(0);
		}
	}

	DamageTake++;
}
