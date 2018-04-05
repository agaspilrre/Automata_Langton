// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CaveAutomataClass.generated.h"

UCLASS()
class CAVEAUTOMATA_API ACaveAutomataClass : public AActor
{
	GENERATED_BODY()
	
public:	

	// Imagen de una célula viva
	UPROPERTY(Category = Spawning, EditAnywhere, BlueprintReadWrite)
		UClass* liveCellPrefab;

	// Imagen de una célula vacía
	UPROPERTY(Category = Spawning, EditAnywhere, BlueprintReadWrite)
		UClass* voidCellPrefab;

	// Imagen de celula con el sprite de la hormiga
	UPROPERTY(Category = Spawning, EditAnywhere, BlueprintReadWrite)
		UClass* antCellPrefab;

	// Ancho de la caverna
	UPROPERTY(Category = Spawning, EditAnywhere, BlueprintReadWrite)
		int width;

	// Alto de la caverna
	UPROPERTY(Category = Spawning, EditAnywhere, BlueprintReadWrite)
		int height;

	// Porcentaje de relleno de la caverna
	UPROPERTY(Category = Spawning, EditAnywhere, BlueprintReadWrite)
		int randomFillPercent;

private:
	// Array que representa la caverna
	TArray<int> cave;

	//Array que representa la posicion de la hormiga en la caverna
	TArray<int>antPosition;

	//Estado de la averna
	enum CaveState { empty, ended };
	CaveState caveState;

	int Count = 0;
	int CountIteractions = 0;

	//direcciones que ha de tomar la hormiga
	enum AntDirection {NORTH,SOUTH,EST,WEST};
	AntDirection antDirection;

	// Número máximo en la que se considera soledad
	const int lonely = 4;

	// Número de generaciones a la que se evolucionará la población inicial;
	const int numGenerationsEvolution = 5;

	// Lista de células
	TArray<AActor*> cellList;

	//Array que representa a la hormiga
	TArray<AActor*>antList;

public:
	// Sets default values for this actor's properties
	ACaveAutomataClass();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// Genera la caverna
	void GenerateCave();

	// Rellena el mapa aleatoriamente
	void RandomFillCave();

	// Regla local para decidir si una célula sigue viva
	void AutomataLocalRule();

	// Obtiene el número de células vecina vivas de una posición pasada como parámetro
	int GetNeighbour(int cell_x, int cell_y);

	// Rellena la caverna
	void FillCave();

	//Intercepta pulsación tecla
	UFUNCTION()
		void InputKey();

	//mueve la hormiga
	void moveAnt();

	//limpia y repinta el mapa
	void repaintCave();

};
