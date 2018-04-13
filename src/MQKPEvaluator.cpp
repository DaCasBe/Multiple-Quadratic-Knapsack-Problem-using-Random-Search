/*
 * MQKPEvaluator.cpp
 *
 * Fichero que define los métodos de la clase MQKPEvaluator. Forma parte del código esqueleto para el problema de las múltiples mochilas cuadráticas, ofrecido para las prácticas de la asignatura Metaheurísticas del Grado de Ingeniería Informática de la Universidad de Córdoba
 *
 * @author Carlos García cgarcia@uco.es
 */

#include "MQKPEvaluator.h"
#include "MQKPSolution.h"
#include "MQKPInstance.h"

double MQKPEvaluator::computeFitness(MQKPInstance &instance, MQKPSolution &solution){

	double fitness = 0;

	/*
	 * TODO Calcular el fitness de la solución de la siguiente forma
	 *   1. Obtener la máxima violación de las capacidades de las mochilas invocando la función de arriba
	 *   2. Si es mayor a 0, entonces devolvemos dicha mayor violación multiplicada por -1.
	 *   3. Si no, devolvemos la suma de los beneficios individuales y cuadráticos invocando a la función de arriba
	 */

	if(instance.getMaxCapacityViolation(solution) > 0){
	
		fitness = instance.getMaxCapacityViolation(solution) * -1;
	
	}else{

		fitness = instance.getSumProfits(solution);

	}

	return fitness;
}



