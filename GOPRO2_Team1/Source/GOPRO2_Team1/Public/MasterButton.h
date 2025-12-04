#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MasterButton.generated.h"


UENUM(BlueprintType)
enum class EButtonType : uint8
{
    Momentary   UMETA(DisplayName = "Hold"),
    OneShot     UMETA(DisplayName = "One Press")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnButtonStateChanged, class AMasterButton*, PressedButton);

UCLASS()
class GOPRO2_TEAM1_API AMasterButton : public AActor
{
    GENERATED_BODY()

public:

    AMasterButton();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UBoxComponent* TriggerBox;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UStaticMeshComponent* ButtonMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    EButtonType ButtonType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config|Sequence")
    int32 SequenceValue;

    UPROPERTY(ReplicatedUsing = OnRep_IsActive, BlueprintReadOnly, Category = "State")
    bool bIsActive;

    UPROPERTY(BlueprintAssignable)
    FOnButtonStateChanged OnStateChanged;

    UFUNCTION(BlueprintCallable, Category = "Logic")
    void ForceResetButton();

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UFUNCTION()
    void OnRep_IsActive();

    UFUNCTION(BlueprintImplementableEvent, Category = "Visuals")
    void UpdateVisuals(bool bActive);

public:

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};