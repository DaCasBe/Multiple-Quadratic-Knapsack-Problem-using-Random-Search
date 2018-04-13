/*
 * main.cpp
 *
 * Fichero que define la función main para ejecutar experimentos sobre el Multiple Quadratic Knapsack Problem.
 * En particular, estos experimentos generan soluciones aleatorias sobre cuatro instancias y van almacenando la mejor hasta el momento.
 *
 * Forma parte del código esqueleto para el problema de las múltiples mochilas cuadráticas, ofrecido para las prácticas de la asignatura Metaheurísticas del Grado de Ingeniería Informática de la Universidad de Córdoba
 *
 * @author Carlos García cgarcia@uco.es
 */

//Definición de algunos parámetros de la experimentación
#define MAX_SECONS_PER_RUN 5
#define MAX_SOLUTIONS_PER_RUN 100000
#define NUM_RUNS 5


#include "MQKPEvaluator.h"
#include "MQKPInstance.h"
#include "MQKPSolGenerator.h"
#include "MQKPSolution.h"
#include "Timer.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cfloat>

using namespace std;


//Definición externa de las semillas para el generador de números aleatorios (en seeds.cpp)
extern unsigned int numSeeds;
extern unsigned int seeds[];

/**
 * Función que realiza la generación de soluciones aleatorias, durante MAX_SECONS_PER_RUN segundos y un máximo de MAX_SOLUTIONS_PER_RUN, para la instancia proporcionada
 * @param[out] results vector donde se almacenarán los valores fitness de las soluciones aleatorias que se generen
 * @param[in] insstance Instancia del problema de la mochila cuadrática múltiple
 */
void runARandomSearchExperiment(vector<double> &results, MQKPInstance &instance){

	MQKPSolution currentSolution(instance);
	Timer t;
	unsigned int numSolutions = 0;

	//Mientras no se acabe el tiempo y no se generen demasiadas soluciones, generar solución aleatoria y guardar el resultado
	while (t.elapsed_time(Timer::VIRTUAL) <= MAX_SECONS_PER_RUN && numSolutions < MAX_SOLUTIONS_PER_RUN){
		MQKPSolGenerator::genRandomSol(instance, currentSolution);
		double currentFitness = MQKPEvaluator::computeFitness(instance, currentSolution);
		results.push_back(currentFitness);
		numSolutions++;
	}
}

/**
 * Función que ejecuta todos los experimentos para los argumentos pasados al programa principal, en particular NUM_RUNS experimentos para cada instancia
 * @param[out] results matriz 3D donde se almacenarán los resultados. El primer índice será para la instancia considerada. El segundo para el experimento sobre dicha instancia. El tercero para la solución generada en dicho experimento
 * @param[in] mainArgs Argumentos de la función main (argv). En los argumentos vienen, desde el índice 1, <nombre del fichero de la instancia 1> <número de mochilas> <nombre del fichero de la instancia 2> <número de mochilas>...
 * @param[in] numMainArgs Número de argumentos de la función main (argc)
 */
void runExperiments(vector< vector< vector< double>* >* > &results, char **mainArgs, unsigned int numMainArgs){

	//Para cada instancia del problema
	for (unsigned int iInstance = 1; iInstance < numMainArgs; iInstance += 2){

		//Leer la instancia y crear la estructuras de datos necesarias para guardar los resultados
		MQKPInstance instance;
		vector< vector < double >* >* resultsOnThisInstance = new vector< vector< double >* >;
		results.push_back(resultsOnThisInstance);
		char *instanceName = mainArgs[iInstance];
		unsigned int numKnapsacks = atoi(mainArgs[iInstance + 1]);
		instance.readInstance(instanceName, numKnapsacks);

		//Lanzar NUM_RUNS ejecuciones sobre dicha instancia
		for (unsigned int r = 1; r <= NUM_RUNS && r < numSeeds; r++){
			srand(seeds[r]); //Inicialización del generador de números aleatorios al inicio de cada ejecución
			vector<double> *theseResults = new vector<double>;
			resultsOnThisInstance->push_back(theseResults);
			runARandomSearchExperiment(*theseResults, instance);
		}
	}
}

/**
 * Función que devuelve el número de soluciones en la ejecución con más soluciones
 * @param[in] results Matriz 2D con los resultados de las distintas ejecuciones. El primer índice es para las ejecuciones. El segundo es para las soluciones de una misma ejecución
 * @return Número de soluciones en la ejecución con más soluciones.
 */
unsigned int getLongestRunLength(vector< vector< double >* > &results){

	unsigned int maxLength = 0;
	unsigned int numRuns = results.size();

	for (unsigned int iRun = 0; iRun < numRuns; iRun++){

		unsigned int length = results.at(iRun)->size();
		if (length > maxLength){
			maxLength = length;
		}
	}

	return maxLength;
}

/**
 * Estructura para calcular y almacenar los datos de salida del programa, media de las soluciones de la iteración actual y media de las mejores soluciones desde el inicio hasta la iteración actual
 */
struct currentBestPair{
	double current;
	double best;
};

/**
 * Función que calcula las medidas de los resultados de varias ejecuciones.
 * @param[in] results Matriz 2D con los resultados de las distintas ejecuciones. El primer índice es para las ejecuciones. El segundo es para las soluciones de una misma ejecución
 * @param[out] means Vector donde se almacenan los resultados medios en datos de tipo currentBestPair, es decir, con la media de las soluciones generadas en la iteración actual y la media de las mejores soluciones generadas desde el inicio hasta la iteración actual
 * @return Devuelve el número de elementos en el vector de salida means
 */
unsigned int computeMeans(vector< vector<double>* > &results, vector<currentBestPair> &means){
	//Obtener el número de soluciones en la ejecución más larga
	unsigned int longestRunLength = getLongestRunLength(results);
	unsigned int numRuns = results.size();
	currentBestPair previousMean;
	previousMean.best = -DBL_MAX;

	//Calcular la media
	for (unsigned int iSolutions = 0; iSolutions < longestRunLength; iSolutions++){
		currentBestPair currentMean;
		currentMean.current = 0;
		currentMean.best = 0;
		double value;

		if (iSolutions == 0){

			for (unsigned int iRun = 0; iRun < numRuns; iRun++){

				unsigned int length = results.at(iRun)->size();
				if (length <= iSolutions){
					value = results.at(iRun)->back();
					currentMean.current += value;
				}else{
					value = results.at(iRun)->at(iSolutions);
					currentMean.current += value;
				}

				currentMean.best += value;
			}
		}
		else {
			for (unsigned int iRun = 0; iRun < numRuns; iRun++){

				unsigned int length = results.at(iRun)->size();
				if (length <= iSolutions){
					value = results.at(iRun)->back();
					currentMean.current += value;
				}else{
					value = results.at(iRun)->at(iSolutions);
					currentMean.current += value;
				}

				currentMean.best += max(value, previousMean.best);
			}
		}

		currentMean.current /= numRuns;
		currentMean.best /= numRuns;
		means.push_back(currentMean);
		previousMean.best = currentMean.best;
	}

	return longestRunLength;
}

/**
 * Función que libera la memoria de un vector 2D
 * @param[in] array vector 2D que se va a liberar de memoria
 */
template <typename T>
void free2Darray(vector< vector<T>* > &array){

	int numVectors = array.size();

	for (int i = 0; i < numVectors; i++){
		array.at(i)->clear();
		delete (array.at(i));
	}

	array.clear();
}

/**
 * Función que libera la memoria de un vector 3D
 * @param[in] array matriz 3D que se va a liberar de memoria
 */
template <typename T>
void free3Darray(vector< vector <vector<T>*>*> &array){

	int numArrays = array.size();

	for (int i = 0; i < numArrays; i++){
		free2Darray(*(array.at(i)));
		delete (array.at(i));
	}

	array.clear();
}

/**
 * Función main que ejecuta la experimentación. Generación de soluciones aleatorias para las instancias
 *
 * Finalmente se escribe en la salida estandar los resultados generados en forma de matriz. Por cada instancia se generan dos columnas, una con los resultados
 * medios de las soluciones generadas en cada iteración y otra con los resultados medios de las mejores soluciones generadas desde el inicio hasta dicha iteración
 */
int main(int argc, char **argv) {

	if (argc < 3 || (argc % 2) != 1){
		cout << argv[0] << " (<problem instance files> <numKnapsacks>)+" << endl;
		return 0;
	}



	//////////////////////////
	//Ejecución de experimentos
	//////////////////////////
	unsigned int numInstances = (argc - 1) / 2;
	vector< vector< vector< double >* >* > allTheResults;
	//En la matriz allTheResults se almacenarán todos los valores de fitness generados
	//Es tridimensional
	//El primer índice recorre las instancias de los problemas que se han abordado
	//El segundo índice recorre las ejecuciones que se han hecho sobre la misma instancia
	//El tercer índice recorre las soluciones que se han generado en la misma ejecución

	runExperiments(allTheResults, argv, argc);




	////////////////////////////
	//Calculo de los valores medios
	////////////////////////////
	unsigned int overallMaxNumResults = 0;
	vector< vector< currentBestPair>* > meanResults;
	//En la matriz meanResults se almacenarán los resultados pero haciendo la media en cada iteración de la búsqueda (Media de las primeras soluciones de las diferentes ejecuciones, igual de las segundas soluciones, etc.)
	//Es bidimensional
	//El primer índice recorre las instancias de los problemas que se han abordado
	//El segundo índice recorre los valores medios calculados por cada iteración del método

	//Para cada instancia del problema, calcular sus medias
	for (unsigned int iInstance = 0; iInstance < numInstances; iInstance++){

		vector< vector< double >* > *resultsThisInstance = allTheResults.at(iInstance);
		vector< currentBestPair > *theseMeans = new vector<currentBestPair>;
		meanResults.push_back(theseMeans);
		unsigned int numResults = computeMeans(*resultsThisInstance, *theseMeans);

		if (numResults > overallMaxNumResults){
			overallMaxNumResults = numResults;
		}
	}



	//////////////////////
	// Impresión de resultados
	//////////////////////
	for (int iInstance = 1; iInstance < argc; iInstance += 2){
		cout << argv[iInstance] << "_" << argv[iInstance + 1] << "_best\t";
		cout << argv[iInstance] << "_" << argv[iInstance + 1] << "_current\t";
	}
	cout << endl;

	for (unsigned int iIteration = 0; iIteration < overallMaxNumResults; iIteration++){
		for (unsigned int iInstance = 0; iInstance < numInstances; iInstance++){
			cout << meanResults.at(iInstance)->at(iIteration).best << "\t";
			cout << meanResults.at(iInstance)->at(iIteration).current << "\t";
		}
		cout << endl;
	}




	//////////////////////
	// Liberar memoria
	//////////////////////
	free3Darray(allTheResults);
	free2Darray(meanResults);

	return 0;
}
