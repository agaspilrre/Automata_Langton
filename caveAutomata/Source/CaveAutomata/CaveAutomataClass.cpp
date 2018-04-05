// Fill out your copyright notice in the Description page of Project Settings.

#include "CaveAutomata.h"
#include "CaveAutomataClass.h"


// Sets default values
ACaveAutomataClass::ACaveAutomataClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACaveAutomataClass::BeginPlay()
{
	Super::BeginPlay();

	//InputComponent->BindAction("Generate", IE_Pressed, this, &ACaveAutomataClass::InputKey);
	GenerateCave();
	
}

// Called every frame
void ACaveAutomataClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (caveState == CaveState::ended) {
		CountIteractions++;
		if (CountIteractions > 2) {
			moveAnt();
			repaintCave();
			CountIteractions = 0;

		}
	}

}


void ACaveAutomataClass::InputKey()
{
	GenerateCave();



}


void ACaveAutomataClass::GenerateCave()
{
	caveState = CaveState::empty;
	
	cave.AddDefaulted(width * height);
	antPosition.AddDefaulted(width * height);

	cellList.Empty();
	antList.Empty();
	//Genera una población de células inicial
	RandomFillCave();

	//Evoluciona el sujeto de prueba
	//for (int index = 0; index < numGenerationsEvolution; index++)
		//AutomataLocalRule();

	caveState = CaveState::ended;
	FillCave();

}

void ACaveAutomataClass::RandomFillCave()
{

	FMath::RandInit(FPlatformTime::Seconds());

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			//En los bordes del mapa siempre se generan células vivas 
			/*
			if (x == 0 || x == width - 1 || y == 0 || y == height - 1)
			{
				cave[(y * width) + x] = 1;
			}
			else
			{*/
			if (x == width / 2 && y == height / 2) {

				//en esta casilla colocamos a la hormiga
				cave[(y * width) + x] = 0;
				//guardamos la posicion esta la hormiga
				antPosition[(y*width) + x] = 2;
			}

			else {
				if (FMath::FRandRange(0, 100) < randomFillPercent)
				{
					cave[(y * width) + x] = 1;
					antPosition[(y*width) + x] = 0;
				}
				else
				{
					cave[(y * width) + x] = 0;
					antPosition[(y*width) + x] = 0;
				}
			}
				
			//}
		}
	}

}

void ACaveAutomataClass::AutomataLocalRule()
{
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			//Obtiene la población de células vecinas
			int neighbourCellTiles = GetNeighbour(x, y);

			//Regla local, basada en el algoritmo:
			//https://github.com/SebLague/Procedural-Cave-Generation/tree/master/Episode%2001
			if (neighbourCellTiles > lonely)
				cave[(y * width) + x] = 1;
			else if (neighbourCellTiles < lonely)
				cave[(y * width) + x] = 0;

		}
	}

}

int ACaveAutomataClass::GetNeighbour(int cell_x, int cell_y)
{
	int cellCount = 0;

	//Recorre un grid de +-1 en sentido horizontal
	for (int neighbourX = cell_x - 1; neighbourX <= cell_x + 1; neighbourX++)
	{
		//Recorre un grid de +-1 en sentido vertical
		for (int neighbourY = cell_y - 1; neighbourY <= cell_y + 1; neighbourY++)
		{
			//Comprueba que no se salga de los límites del sujeto de prueba
			if (neighbourX >= 0 && neighbourX < width && neighbourY >= 0 && neighbourY < height)
			{
				if (neighbourX != cell_x || neighbourY != cell_y)
				{
					cellCount += cave[(neighbourY * width) + neighbourX];
				}
			}
			else
			{
				cellCount++;
			}
		}
	}

	return cellCount;
}

void ACaveAutomataClass::FillCave()
{
	//Solo se dibuja si el sujeto de prueba está completo
	if (caveState == CaveState::empty)
		return;

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			FVector* location = new FVector((-width * 100) / 2 + (x * 100) + 50, 0, (-height * 100) / 2 + (y * 100) + 50);
			FRotator* rotation = new FRotator(0, 0, 0);
			FActorSpawnParameters params;

			//cellList.Add(GetWorld()->SpawnActor(voidCellPrefab, location, rotation, params));

			if (cave[(y * width) + x] == 1)
			{
				cellList.Add(GetWorld()->SpawnActor(liveCellPrefab, location, rotation, params));
				//spriteComponent->UpdateInstanceColor((y * width) + x, FColor::Black);
			}
			else if(cave[(y * width) + x] == 0)
			{
				cellList.Add(GetWorld()->SpawnActor(voidCellPrefab, location, rotation, params));
			}

			 if(antPosition[(y * width)+x]==2){

				
				//especificar en otro array la posicion de la hormiga
				
				cellList.Add(GetWorld()->SpawnActor(antCellPrefab, location, rotation, params));
				//establezco inicialmente la hormiga mirando hacia el norte
				antDirection = AntDirection::NORTH;
			}
		}
	}


}



void ACaveAutomataClass::moveAnt() {


	
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			//buscamos en que lugar del tablero se encuentra la hormiga
			if (antPosition[(y * width) + x] == 2 && Count==0) {

				Count++;//contador que controlar que solo entre una vez x este if xcada iteraccion
				//si la hormiga esta encima de una casilla blanca PONER UN CONTADOR PARA CONTROLAR QUE ENTRA SOLO UNA VEZ POR CADA LLAMADA SI NO PUEDE CREAR PROBLEMAS
				if (cave[(y * width) + x] == 0) {

					//la casilla cambia a color negro
					cave[(y * width) + x] = 1;
					//gira 90 grados izq 
						//si esta mirando hacia el norte, el estado es igual a oeste
					if (antDirection == AntDirection::NORTH) {
						antDirection = AntDirection::WEST;

						antPosition[(y * width) + x] = 0;
						//si avanzo al oeste x-1
						antPosition[(y * width) + (x-1)] = 2;

					}
						

						//si esta mirando hacia el oeste, el estado es igual a sur
					else if (antDirection == AntDirection::WEST) {
						antDirection = AntDirection::SOUTH;
						antPosition[(y * width) + x] = 0;
						//sumo la y para que la hormiga se situe una casilla mas abajo
						antPosition[((y+1) * width) + x] = 2;
					}
						
						//si esta mirando hacia el sur,el estado es igual a este
					else if (antDirection == AntDirection::SOUTH) {
						antDirection = AntDirection::EST;
						antPosition[(y * width) + x] = 0;
						//si miro hacia el este avanzo en esa direccion sumandole 1 a la X
						antPosition[(y * width) + (x+1)] = 2;
					}
						
						//si esta mirando hacia el este, el estado es igual a norte
					else if (antDirection == AntDirection::EST) {
						antDirection = AntDirection::NORTH;
						//pongo esta casilla hormiga a cero para indicar que ya no esta en esa posicion
						antPosition[(y * width) + x] = 0;
						//resto la y ya que la hormiga la muevo una casilla mas arriba
						antPosition[((y-1) * width) + x] = 2;
					}
						
					//avanza un cuadrado
						//si el estado es norte resto y
						//si el estado es oeste resto x
						//si el estado es sur sumo y
						//si el estado es este sumo x
				}

				//si la hormiga esta encima de una casilla negra
				else {
					//cambia la celda a color blanco
					cave[(y * width) + x] = 0;
					//gira 90 grados a la derecha
						//Si miramos al norte la direccion es igual a Este
					if (antDirection == AntDirection::NORTH) {
						antDirection = AntDirection::EST;
						antPosition[(y * width) + x] = 0;
						antPosition[(y * width) + (x+1)] = 2;

					}
						//Si miramos hacia el Este la direccion es igual a sur
					else if (antDirection == AntDirection::EST) {
						antDirection = AntDirection::SOUTH;
						antPosition[(y * width) + x] = 0;
						antPosition[((y+1) * width) + x] = 2;
					}
						//Si miramos hacia el Sur la direccion es igual a Oeste
					else if (antDirection == AntDirection::SOUTH) {
						antDirection = AntDirection::WEST;
						antPosition[(y * width) + x] = 0;
						antPosition[(y * width) + (x-1)] = 2;

					}
						//Si miramos hacia el Oeste la direccion es igual norte
					else if (antDirection == AntDirection::WEST) {
						antDirection = AntDirection::NORTH;
						antPosition[(y * width) + x] = 0;
						antPosition[((y-1) * width) + x] = 2;

					}
					//avanza un cuadrado en la direccion que mira

				}
			}
		}
	}

	Count = 0;//reseteamos contador

}



void ACaveAutomataClass::repaintCave() {

	//limpiar las instancias de la caverna anterior?¿?¿?
	//cellList.Empty();
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			cellList[(y * width) + x]->Destroy();
		}
	}
	cellList.Empty();
	//destroy actor
	//volver a pintar el mapa
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			FVector* location = new FVector((-width * 100) / 2 + (x * 100) + 50, 0, (-height * 100) / 2 + (y * 100) + 50);
			FRotator* rotation = new FRotator(0, 0, 0);
			FActorSpawnParameters params;

			if (cave[(y * width) + x] == 1)
			{
				cellList.Add(GetWorld()->SpawnActor(liveCellPrefab, location, rotation, params));
			}
			else if (cave[(y * width) + x] == 0)
			{
				cellList.Add(GetWorld()->SpawnActor(voidCellPrefab, location, rotation, params));
			}

			if (antPosition[(y * width) + x] == 2) {


				//especificar en otro array la posicion de la hormiga

				cellList.Add(GetWorld()->SpawnActor(antCellPrefab, location, rotation, params));
				
			}
		}
	}


}


