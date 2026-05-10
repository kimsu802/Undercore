// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/OrbActor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraComponent.h"


// Sets default values
AOrbActor::AOrbActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Root);

	Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	Niagara->SetupAttachment(StaticMesh);

}

// Called when the game starts or when spawned
void AOrbActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		TargetActor = PlayerPawn;
	}

}

// Called every frame
void AOrbActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsValid(TargetActor))
	{
		return;
	}

	RunningTime += DeltaTime;

	CurrentAngle += OrbitSpeed * DeltaTime;

	const FVector TargetLocation = TargetActor->GetActorLocation();

	const float Radian = FMath::DegreesToRadians(CurrentAngle);

	const float OffsetX = FMath::Cos(Radian) * OrbitRadius;
	const float OffsetY = FMath::Sin(Radian) * OrbitRadius;

	const float FloatZ = FMath::Sin(RunningTime * FloatSpeed) * FloatAmplitude;

	FVector DesiredLocation;
	DesiredLocation.X = TargetLocation.X + OffsetX;
	DesiredLocation.Y = TargetLocation.Y + OffsetY;
	DesiredLocation.Z = TargetLocation.Z + 100.f + FloatZ;

	const FVector NewLocation =
		FMath::VInterpTo(
			GetActorLocation(),
			DesiredLocation,
			DeltaTime,
			FollowInterpSpeed);

	SetActorLocation(NewLocation);

	const FRotator LookAtRotation =
		UKismetMathLibrary::FindLookAtRotation(
			GetActorLocation(),
			TargetLocation);

	SetActorRotation(LookAtRotation);

}

void AOrbActor::SetTargetActor(AActor* NewTarget)
{
	TargetActor = NewTarget;
}

