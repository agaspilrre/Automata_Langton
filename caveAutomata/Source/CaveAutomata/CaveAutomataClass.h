// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CaveAutomataClass.generated.h"

UCLASS()
class CAVEAUTOMATA_API ACaveAutomataClass : public AActor
{
	GENERATED_BODY()
	
public:	

	// Imagen de una c�lula viva
	UPROPERTY(Category = Spawning, EditAnywhere, BlueprintReadWrite)
		UClass* liveCellPrefab;

	// Imagen de una c�lula vac�a
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

	// N�mero m�ximo en la que se considera soledad
	const int lonely = 4;

	// N�mero de generaciones a la que se evolucionar� la poblaci�n inicial;
	const int numGenerationsEvolution = 5;

	// Lista de c�lulas
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

	// Regla local para decidir si una c�lula sigue viva
	void AutomataLocalRule();

	// Obtiene el n�mero de c�lulas vecina vivas de una posici�n pasada como par�metro
	int GetNeighbour(int cell_x, int cell_y);

	// Rellena la caverna
	void FillCave();

	//Intercepta pulsaci�n tecla
	UFUNCTION()
		void InputKey();

	//mueve la hormiga
	void moveAnt();

	//limpia y repinta el mapa
	void repaintCave();

};
