// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveSpawnPoint.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
AWaveSpawnPoint::AWaveSpawnPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Spawn Area"));
	SetRootComponent(SpawnBox);
}

FVector AWaveSpawnPoint::FindSpawnPoint()
{
	int Count{ 0 };
	bool ValidSpawnPoint{ false }; 
	while (!ValidSpawnPoint)
	{
		auto RandomPoint = GetRandomPointInBox();
		FVector CapsuleEndOffset{ 0,0,90 };
		FVector CapsuleStartOffset{ 0,0,35 };
		ETraceTypeQuery CapsuleTraceQuery{};
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypeQuery;
		ObjectTypeQuery.Add(EObjectTypeQuery::ObjectTypeQuery3);
		TArray<AActor*> ActorsToIgnore{ this };
		FHitResult HitResults{};

		
		UKismetSystemLibrary::CapsuleTraceSingleForObjects(
			GetWorld(), 
			RandomPoint + CapsuleStartOffset,
			RandomPoint + CapsuleEndOffset, 
			34.f,
			90.f, 
			ObjectTypeQuery, 
			false, 
			ActorsToIgnore, 
			EDrawDebugTrace::None, 
			HitResults,
			true,
			FColor::Red,
			FColor::Green,
			5.F);

		if (!HitResults.bBlockingHit)
		{
			return RandomPoint;
		}
		else if (Count >= 10)
		{
			return FVector::ZeroVector;
		}
		else
		{
			Count++;
		}
	}
	return FVector::ZeroVector;
}

FVector AWaveSpawnPoint::GetRandomPointInBox()
{
	FVector SpawnOffset{ 0,0,90 };
	// Sets up the variables needed for the line trace.
	FHitResult Hit;
	FRotator Rot = UKismetMathLibrary::MakeRotFromX(FVector::DownVector);
	FVector Start = UKismetMathLibrary::RandomPointInBoundingBox(SpawnBox->GetComponentLocation(), SpawnBox->GetScaledBoxExtent());
	FVector End = Start + (Rot.Vector() * (SpawnBox->GetScaledBoxExtent().Z * 2.f));
	FCollisionQueryParams TraceParams;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypeQuery;
	ObjectTypeQuery.Add(EObjectTypeQuery::ObjectTypeQuery1);
	ObjectTypeQuery.Add(EObjectTypeQuery::ObjectTypeQuery2);
	TraceParams.bTraceComplex = true;

	// Perform a line trace
	GetWorld()->LineTraceSingleByObjectType(Hit, Start, End, ObjectTypeQuery, TraceParams);

	DrawDebugLine(GetWorld(), Start, Hit.Location, FColor::Blue, false, 5.f);
	return (Hit.Location + SpawnOffset);
}  