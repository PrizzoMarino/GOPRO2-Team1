#include "MasterButton.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h" 


AMasterButton::AMasterButton()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;

    // Create object with a Trigger Box
    ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
    RootComponent = ButtonMesh;


    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    TriggerBox->SetupAttachment(RootComponent);
    TriggerBox->SetCollisionProfileName(TEXT("Trigger"));


    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AMasterButton::OnOverlapBegin);
    TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AMasterButton::OnOverlapEnd);


    ButtonType = EButtonType::Momentary;
    bIsActive = false;
}

void AMasterButton::BeginPlay()
{
    Super::BeginPlay();
}

// Function that replicates the state of the Button to every player as well as the Button Itself.
void AMasterButton::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AMasterButton, bIsActive);
}

// Check if the actors pressing the Button are Valid "Activators" with a tag
bool IsValidActivator(AActor* Actor)
{
    if (!Actor) return false;

    return Actor->ActorHasTag(TEXT("Activator"));
}

// Function to check functionality on Overlap
void AMasterButton::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!HasAuthority()) return;

    if (!IsValidActivator(OtherActor)) return;

    if (ButtonType == EButtonType::OneShot && bIsActive) return;

    bIsActive = true;
    OnStateChanged.Broadcast(this);
    OnRep_IsActive();
}

// Same as overlap begin but whenever an actor exits the overlap check what should happen
void AMasterButton::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (!HasAuthority()) return;
    if (!IsValidActivator(OtherActor)) return;

    if (ButtonType == EButtonType::Momentary)
    {
        TArray<AActor*> OverlappingActors;
        TriggerBox->GetOverlappingActors(OverlappingActors);


        bool bStillSomeoneInside = false;
        for (AActor* Actor : OverlappingActors)
        {
            if (Actor != OtherActor && IsValidActivator(Actor))
            {
                bStillSomeoneInside = true;
                break;
            }
        }

        if (!bStillSomeoneInside)
        {
            bIsActive = false;
            OnStateChanged.Broadcast(this);
            OnRep_IsActive();
        }
    }
}

// Function to reset the button automatically if working with sequences and One Shot
void AMasterButton::ForceResetButton()
{

    if (!HasAuthority()) return;


    if (ButtonType == EButtonType::OneShot)
    {
        bIsActive = false;
        OnRep_IsActive();
    }

}

// Updated state of the button replicated
void AMasterButton::OnRep_IsActive()
{
    UpdateVisuals(bIsActive);
}