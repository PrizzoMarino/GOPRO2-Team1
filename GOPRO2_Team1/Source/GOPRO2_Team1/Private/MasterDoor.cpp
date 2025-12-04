#include "MasterDoor.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

AMasterDoor::AMasterDoor()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;
    SetReplicateMovement(true);

    // A scene component must be created due to the fact that, when using timelines to move the static meshes
    // the door might come back to the origin of the map (0,0,0) and perform the desired movement there
    // Instead we are creating a Scene Component and making the static mesh a Child of this scene component
    // which properties when used with Set Location To and Timelines does not reset it´s location and it´s better practice as well

    RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
    RootComponent = RootSceneComponent;

    DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
    DoorMesh->SetupAttachment(RootComponent);

    SetReplicateMovement(true);

    bIsOpen = false;
}

// Replication of the state of the door as well as the object itself
void AMasterDoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AMasterDoor, bIsOpen);
}

void AMasterDoor::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        for (AMasterButton* Btn : RequiredButtons)
        {
            if (Btn)
            {
                Btn->OnStateChanged.AddDynamic(this, &AMasterDoor::RegisterButtonPress);
            }
        }

        if (!bIsSequenceMode)
        {
            CheckDoorConditions(false);
        }
    }
}

// Check the conditions for the door to be opened comparing button states
void AMasterDoor::CheckDoorConditions(bool bIgnoredVal)
{
    if (!HasAuthority()) return;

    bool bShouldOpen = true;

    if (RequiredButtons.Num() == 0)
    {
        bShouldOpen = false;
    }
    else
    {
        for (AMasterButton* Btn : RequiredButtons)
        {

            if (!Btn || !Btn->bIsActive)
            {
                bShouldOpen = false;
                break;
            }
        }
    }


    if (bIsOpen != bShouldOpen)
    {
        bIsOpen = bShouldOpen;
        OnRep_IsOpen();
    }
}

// Function to register the sequence of button presses in order to check if they are correct.
// We create two arrays, one with default values defined by the developer in engine and another one defined by the buttons
// the second array will add the buttons int number set to them to the array and compare it to the default one
// if the array of buttons (sequence) is equal to the one imposed in the door, the sequence may continue
// if the sequence of buttons is not coincident with the one in the doors the array will get deleted and all the buttons reseted.

void AMasterDoor::RegisterButtonPress(AMasterButton* Button)
{
    if (!HasAuthority()) return;


    if (!bIsSequenceMode)
    {
        CheckDoorConditions(Button->bIsActive);
        return;
    }


    if (!Button->bIsActive) return;

    int32 PressedValue = Button->SequenceValue;
    if (PressedValue == 0) return;

    int32 ExpectedIndex = CurrentInputSequence.Num();

    if (ExpectedIndex < CorrectSequence.Num())
    {
        int32 ExpectedValue = CorrectSequence[ExpectedIndex];

        if (PressedValue == ExpectedValue)
        {
            CurrentInputSequence.Add(PressedValue);

            if (CurrentInputSequence.Num() == CorrectSequence.Num())
            {
                bIsOpen = true;
                OnRep_IsOpen();
            }
        }
        else
        {
            for (AMasterButton* Btn : RequiredButtons)
            {
                if (Btn)
                {
                    Btn->ForceResetButton();
                }
            }
            CurrentInputSequence.Empty();
            bIsOpen = false;
            OnRep_IsOpen();
            SequenceFailed();
        }
    }
}

// Replication of the door state
void AMasterDoor::OnRep_IsOpen()
{
    if (bIsOpen)
    {
        OpenDoor();
    }
    else
    {
        CloseDoor();
    }
}