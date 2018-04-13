/*
 * MQKPInstance.cpp
 *
 * Fichero que define los métodos de la clase MQKPInstance. Forma parte del código esqueleto para el problema de las múltiples mochilas cuadráticas, ofrecido para las prácticas de la asignatura Metaheurísticas del Grado de Ingeniería Informática de la Universidad de Córdoba
 *
 * @author Carlos García cgarcia@uco.es
 */

#include "MQKPInstance.h"
#include "MQKPSolution.h"

//Includes añadidos
#include <fstream>
#include <iostream>
#include <stdio.h>

MQKPInstance::MQKPInstance(){
	_numKnapsacks=0;
	_numObjs=0;
	_profits.resize(0);
	_weights.resize(0);
	_capacities.resize(0);
}

MQKPInstance::~MQKPInstance(){
	//TODO completar

	for(int i = 0; i < _numObjs; i++){
		_profits[i].clear();
	}

	_profits.clear();
	_weights.clear();
	_capacities.clear();
}

double MQKPInstance::getMaxCapacityViolation(MQKPSolution &solution) {

	double *sumWeights = new double[this->_numKnapsacks + 1];

	//Inicializar la suma de los pesos de cada mochila
	for (int j = 1; j <= getNumKnapsacks(); j++) {
		sumWeights[j] = 0;
	}

	for (int i = 0; i < getNumObjs(); i++) {

		/*TODO Completar
		 * 1. Obtener la mochila en la que se encuentra el objeto i-ésimo
		 * 2. Si se encuentra en una mochila válida (mayor que 0), incrementar con el peso del objeto el valor correspondiente en sumWeights.
		 */

		if(solution.whereIsObject(i) != 0){
			sumWeights[solution.whereIsObject(i)] = sumWeights[solution.whereIsObject(i)] + _weights[i];
		}

	}

	double maxCapacityViolation = 0; //Inicializamos la máxima violación de alguna mochila a 0, que significa que no hay ninguna violación

	for (int j = 1; j <= this->_numKnapsacks; j++) {

		/*TODO Completar
		 * 1. Calcular la violación en la mochila j-ésima
		 * 2. Actualizar maxCapacityViolation en su caso
		 */

		if((sumWeights[j] - _capacities[j]) > maxCapacityViolation){
			maxCapacityViolation = sumWeights[j] - _capacities[j];
		}

	}

	delete[] sumWeights;
	return maxCapacityViolation;
}

double MQKPInstance::getSumProfits(MQKPSolution &solution) {

	double sumProfits = 0.;

	/*TODO COMPLETAR
	 * Doble bucle para cada par de objetos
	 * Todo objeto incluido en alguna mochila (> 0) debe sumar su beneficio individual
	 * Todo par de objetos incluidos en la misma mochila (y > 0) debe sumar su beneficio conjunto. IMPORTANTE, sumar los pares (i,j) sólo una vez, es decir, si se suma (i, j), no se debe sumar (j, i)
	 */

	for(int i = 0; i < getNumObjs()-1; i++){

		if(solution.whereIsObject(i) != 0){		
			sumProfits +=  _profits[i][i];		//Suma de beneficio individual
		
			for(int j = i+1; j < getNumObjs(); j++){  //Poniendo j=i+1 solo recorre hasta la diagonal haciendo que no se sume [i][j] y [j][i]

				if(solution.whereIsObject(i) == solution.whereIsObject(j)){
					sumProfits = sumProfits + _profits[i][j];
				}
			}
		}
	}


	if(solution.whereIsObject(getNumObjs()-1) != 0){			//Se suma el profit del ultimo elemento si existe
		sumProfits = sumProfits + _profits[getNumObjs()-1][getNumObjs()-1];
	}

	return sumProfits;
}

void MQKPInstance::readInstance(char *filename, int numKnapsacks){
	/*
	 * TODO completar esta función:
	 *   1. leer el número de objetos
	 *   2. reservar la memoria de vectores y matrices
	 *   3. leer beneficios y pesos de los objetos según lo comentado arriba
	 *   4. Calcular las capacidades de las mochilas:
	 *      . Calcular la suma de los pesos de todos los objetos
	 *      . Multiplicar por 0.8
	 *      . Dividir el resultado anterior entre el número de mochilas. Eso será la capacidad de cada mochila
	 */
	
	_numKnapsacks=numKnapsacks;
	std::ifstream f;

	f.open(filename);

	std::string leido;
	double aux;
	f>>leido;//1ºlinea

	// 1. leer el número de objetos
	f>>aux;//2ºlinea
	_numObjs=aux;

	// 2. reservar la memoria de vectores y matrices
	//Inicializar a 0
	_profits.resize(_numObjs);
	for(int i=0;i<_numObjs;i++)
		_profits[i].resize(_numObjs);

	_weights.resize(_numObjs);
	_capacities.resize(numKnapsacks);
	 //3. leer beneficios y pesos de los objetos según lo comentado arriba
	//Diagonal
	for(int i=0; i<_numObjs;i++){
		f>>aux;

		_profits[i][i]=aux;
	}
	//Mitad superior derecha
	for(int j=0;j<_numObjs-1;j++){

		for(int i=j+1; i<_numObjs;i++){
			f>>aux;

			_profits[j][i]=aux;
			_profits[i][j]=aux;
		}
	}
	f>>aux;//Linea que no necesitamos
	f>>aux;//Linea que no necesitamos
	for(int i=0; i<_numObjs;i++){
		f>>aux;
		_weights[i]=aux;
	}
	f.close();
	// 4. Calcular las capacidades de las mochilas
	//Calcular la suma de los pesos de todos los objetos
	int suma=0;
	for(int i=0; i<_numObjs;i++){
		suma+=_weights[i];
	}
	//Multiplicar por 0.8
	suma=suma*0.8;
	//Dividir el resultado anterior entre el número de mochilas.Eso será la capacidad de cada mochila
	suma=suma/getNumKnapsacks();
	_capacities[0]=-1;
	for(int i=1;i<=getNumKnapsacks();i++)
		_capacities[i]=suma;
}
