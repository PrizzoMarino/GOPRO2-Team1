#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MasterButton.h" 
#include "MasterDoor.generated.h"

UCLASS()
class GOPRO2_TEAM1_API AMasterDoor : public AActor
{
    GENERATED_BODY()

public:
    AMasterDoor();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class USceneComponent* RootSceneComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UStaticMeshComponent* DoorMesh;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Logic")
    TArray<AMasterButton*> RequiredButtons;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Logic|Sequence")
    TArray<int32> CorrectSequence;

    TArray<int32> CurrentInputSequence;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Logic|Sequence")
    bool bIsSequenceMode;

    UPROPERTY(ReplicatedUsing = OnRep_IsOpen, BlueprintReadOnly, Category = "State")
    bool bIsOpen;

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void CheckDoorConditions(bool bIgnoredVal);

    UFUNCTION()
    void OnRep_IsOpen();

    UFUNCTION(BlueprintImplementableEvent, Category = "Visuals")
    void OpenDoor();

    UFUNCTION(BlueprintImplementableEvent, Category = "Visuals")
    void CloseDoor();

    UFUNCTION()
    void RegisterButtonPress(AMasterButton* Button);

    UFUNCTION(BlueprintImplementableEvent)
    void SequenceFailed();

public:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};