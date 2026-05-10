// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OrbActor.generated.h"

class UNiagaraComponent;

UCLASS()
class UNDERCORE_API AOrbActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOrbActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> Niagara;

	/*
	 * 따라다닐 대상
	 */
	UPROPERTY(EditAnywhere, Category = "Orbit")
	TObjectPtr<AActor> TargetActor;

	/*
	 * 캐릭터 주변 Orbit 반지름
	 */
	UPROPERTY(EditAnywhere, Category = "Orbit")
	float OrbitRadius = 150.f;

	/*
	 * Orbit 속도
	 */
	UPROPERTY(EditAnywhere, Category = "Orbit")
	float OrbitSpeed = 90.f;

	/*
	 * 위아래 흔들리는 높이
	 */
	UPROPERTY(EditAnywhere, Category = "Orbit")
	float FloatAmplitude = 25.f;

	/*
	 * 위아래 흔들리는 속도
	 */
	UPROPERTY(EditAnywhere, Category = "Orbit")
	float FloatSpeed = 2.f;

	/*
	 * 대상 위치를 얼마나 부드럽게 따라갈지
	 */
	UPROPERTY(EditAnywhere, Category = "Orbit")
	float FollowInterpSpeed = 5.f;

private:

	float CurrentAngle = 0.f;
	float RunningTime = 0.f;

public:

	UFUNCTION(BlueprintCallable)
	void SetTargetActor(AActor* NewTarget);

};
