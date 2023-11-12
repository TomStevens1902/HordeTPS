// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyControllerBase.h"

#include "HordeTPS/Player/CharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"


AEnemyControllerBase::AEnemyControllerBase(const FObjectInitializer& ObjectInitializer)
{
	SetupPerceptionSystem();
}


void AEnemyControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if(const AEnemyBase* EnemyPawn = Cast<AEnemyBase>(InPawn))
	{
		if(UBehaviorTree* BehaviorTree = EnemyPawn->GetBehaviourTree())
		{
			UBlackboardComponent* BlackboardComponent;
			UseBlackboard(BehaviorTree->BlackboardAsset, BlackboardComponent);
			Blackboard = BlackboardComponent;
			RunBehaviorTree(BehaviorTree);
		}
	}
}
void AEnemyControllerBase::SetupPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (SightConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
		SightConfig->SightRadius = 1000.f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 200.f;
		SightConfig->PeripheralVisionAngleDegrees = 60.f;
		SightConfig->SetMaxAge(0.0f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = -1.0f;
		SightConfig->NearClippingRadius = 0.0f;
		SightConfig->PointOfViewBackwardOffset = 0.0f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		//GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyControllerBase::OnSightStimulus);
		GetPerceptionComponent()->OnTargetPerceptionInfoUpdated.AddDynamic(this, &AEnemyControllerBase::OnSightStimulus);
	}
}


void AEnemyControllerBase::OnSightStimulus(const FActorPerceptionUpdateInfo& UpdateInfo)
{
	if (UpdateInfo.Target == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", UpdateInfo.Stimulus.WasSuccessfullySensed());
	}
}
//void AEnemyControllerBase::OnSightStimulus(AActor* Actor, FAIStimulus const Stimulus)
//{
//	//auto* const Character = Cast<ACharacterBase>(Actor)
//	GEngine->AddOnScreenDebugMessage(132, 1.f, Stimulus.WasSuccessfullySensed() ? FColor::Green : FColor::Red, TEXT("call"));
//	if (Actor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
//	{
//		GEngine->AddOnScreenDebugMessage(131, 10.F, Stimulus.WasSuccessfullySensed() ? FColor::Green : FColor::Red, TEXT("seen"));
//		GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed());
//	}
//}