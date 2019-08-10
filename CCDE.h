//============================================================================================
// SACCJADE - Surrogate-Assisted Cooperative Coevolutionary version of JADE
//============================================================================================
// Name        : CCDE.h
// Authors     : Giuseppe A. Trunfio - trunfio@uniss.it
//               Ivanoe De Falco
//               Antonio Della Cioppa
// Version     : v1.0
// Created on  : Gen 16, 2017
//
// More details on the following paper:
//
// De Falco, I., Della Cioppa, A., Trunfio, G.A. 
// 'Investigating Surrogate-assisted Cooperative Coevolution for Large-Scale Global Optimization', 
// submitted'
//=============================================================================================

#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cstdlib>
#include <random>
#include <algorithm>
#include <vector>
#include <list>
#include <map>
#include "Benchmarks2013.h"
#include "JADE.h"
#include "Decomposer.h"
#include "numeric"


#define __USE_MINGW_ANSI_STDIO 1


using namespace std;


class ConvPlotPoint
{
public:
    unsigned nfe;
    double f;
    double surrogateError;
    unsigned subcomponentSize;
    unsigned individuals;
    ConvPlotPoint(unsigned  _nfe, double _f, double _surrogateError) :
        nfe(_nfe), f(_f), surrogateError(_surrogateError)
    {};
};




/**
	@brief Main CCDE class.
	This class represents multiple swarms of particles which operate on the grouped directions of the main search space.
*/
class CCDE
{
    Fitness *fitness;

    ///Pseudorandom generator
    RandomEngine eng;

    uniform_real_distribution<double> unifRandom;

public:
    ///Create the CCDE object with the specified optimization parameters
    CCDE();

    ///Destroy the CCDE object
    ~CCDE();

    ///Perform the optimization
    void optimize(Fitness* f, 
		          unsigned int maxNumberOfEvaluations, 
		          unsigned sizeOfSubcomponents,
                  unsigned individualsPerSubcomponent,
                  vector<ConvPlotPoint> &convergence, 
		          int seed, 
		          typeOfSurrogate algType,
                  unsigned numItePerCycle);


    ///Returns the final fitness value
    double getFinalFitnessValue();

    void initPopulation(unsigned numOfIndividuals);
    void initContextVector();
    double computeFitnessValue(vector<double> &x);
    Decomposer *createDecomposer(unsigned sizeOfSubcomponents, unsigned individualsPerSubcomponent, typeOfSurrogate sType, bool random = false);
    void optimizeSubcomponents(Decomposer *dec, unsigned nGenPerIteration);

    ///Dimensionality of the search space
    unsigned problemDimension;

    ///Number of fitness evaluations
    unsigned numberOfEvaluations;
    unsigned maxNumberOfEvaluations;
    unsigned ite;

    //JADE parameters
    double JADE_c;
    double JADE_p;
    int JADE_mutationStrategy;

    ///Final global best fitness value
    double globalBesdouble;

    ///Final global best position and context vector
    vector<double> contextVector;

    double optimum;

    ///Current population
    vector< vector<double> > population;

    ///Fitnesses of population (only for initialization after sub-groups change)
    vector< double > fitnessValues;

    ///Lower limit for each dimension of the search space
    double lowerLimit;

    ///Upper limit for each dimension of the search space
    double upperLimit;

    ///index of the best individual
    unsigned int globalBestIndex;

    ///Last elapsed time
    double elapsedTime;

    unsigned functionIndex;

    vector<unsigned> subcomponentSizes;
    vector<unsigned> numIndividualsPerSubcomponents;
};

