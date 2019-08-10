//============================================================================================
// SACCJADE - Surrogate-Assisted Cooperative Coevolutionary version of JADE
//============================================================================================
// Name        : main.cpp
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

#define _USE_MATH_DEFINES
#include "CCDE.h"
#include "Benchmarks2013.h"
#include "Header.h"
#include "tclap/CmdLine.h"

using namespace TCLAP;

using namespace std;

// create new object of class with default setting
Fitness* generateFuncObj(int funcID)
{
	Fitness *fp = NULL;
	using namespace CEC2013;
	if (funcID == 1)       fp = new F1();
	else if (funcID == 2)  fp = new F2();
	else if (funcID == 3)  fp = new F3();
	else if (funcID == 4)  fp = new F4();
	else if (funcID == 5)  fp = new F5();
	else if (funcID == 6)  fp = new F6();
	else if (funcID == 7)  fp = new F7();
	else if (funcID == 8)  fp = new F8();
	else if (funcID == 9)  fp = new F9();
	else if (funcID == 10) fp = new F10();
	else if (funcID == 11) fp = new F11();
	else if (funcID == 12) fp = new F12();
	else if (funcID == 13) fp = new F13();
	else if (funcID == 14) fp = new F14();
	else if (funcID == 15) fp = new F15();
	else
	{
		cerr << "Fail to locate Specified Function Index" << endl;
		exit(-1);
	}
	
	return fp;
}


//******************************************************************************************/
//
//
//
//******************************************************************************************/
void optimization(int argc, char* argv[])
{   
	unsigned int functionIndex;
	typeOfSurrogate sType; //allowed: {sNone, sGP, sQPA, sRBFN, sSVR}
	unsigned int numRep;
	unsigned int numItePerCycle;
	unsigned int problemDimension;
	unsigned int sizeOfSubcomponents;
	unsigned int numOfIndividuals;
	unsigned int numberOfEvaluations;
    
	vector<int> seeds;
    unsigned maxNumRep = 100;
    for (unsigned i = 0; i < maxNumRep; ++i)
        seeds.push_back(i);

	//parsing SACCJADE command line
	try
	{
		CmdLine cmd("SACCJADE - Surrogate-assisted Cooperative Coevolution for Large-Scale Optimization of Computationally Expensive Objective Functions", ' ', "1.0");

		ValueArg<unsigned int> functionArg("f", "function", "function to optimize [1-15]", false, 1, "int");
		cmd.add(functionArg);

		ValueArg<unsigned int> surrogateArg("m", "metamodel", "type of fitness metamodel [0->none; 1->GP; 2->QPA; 3->RBFN; 4->SVR]", false, 4, "int");
		cmd.add(surrogateArg);		

		ValueArg<unsigned int> repArg("r", "repetitions", "number of independent repetitions [1-100]", false, 1, "int");
		cmd.add(repArg);

		ValueArg<unsigned int> iteArg("i", "iterations", "number of JADE iterations per cycle", false, 5, "int");
		cmd.add(iteArg);		

		//ValueArg<unsigned int> dimArg("d", "dimension", "problem dimension", false, 1000, "int");
		//cmd.add(dimArg);		

		ValueArg<unsigned int> sdimArg("s", "subdim", "size of subcomponents", false, 5, "int");
		cmd.add(sdimArg);		

		ValueArg<unsigned int> npopArg("p", "popsize", "number of individuals in each subcomponent", false, 10, "int");
		cmd.add(npopArg);		

		ValueArg<unsigned int> feArg("e", "fevals", "allowed number of exact fitness evaluations", false, 10*1000, "int");
		cmd.add(feArg);		

		cmd.parse(argc, argv);
		
		functionIndex = functionArg.getValue();
		sType = (typeOfSurrogate)surrogateArg.getValue();
		numRep = repArg.getValue();
		numItePerCycle = iteArg.getValue();
		problemDimension = 1000;// dimArg.getValue();
		sizeOfSubcomponents = sdimArg.getValue();
		numOfIndividuals = npopArg.getValue();
		numberOfEvaluations = feArg.getValue();

	}
    catch (ArgException& e)
    {
	   cout << "ERROR: " << e.error() << " " << e.argId() << endl;
    }

		
	if (sType == sNone)
        cout << "Using CCJADE" << endl;
    else if (sType == sGP)
        cout << "SACCJADE with Gausiann Process" << endl;
    else if (sType == sQPA)
        cout << "SACCJADE with Quadratic Polynomial Local Approsimation" << endl;
    else if (sType == sRBFN)
        cout << "SACCJADE with Radial Basis Function Network" << endl;
	else if (sType == sSVR)
		cout << "SACCJADE with Support Vector Regression" << endl;
    else
    {
        cerr << "unknown surrogate" << endl;
        exit(1);
    }

	if ( functionIndex < 1 || functionIndex>15 )
	{
		cerr << "function index out of allowed bounds [1..15]" << endl;
		exit(1);
	}
	
	Fitness* f = generateFuncObj(functionIndex);

	if (f == NULL)
	{
		cerr << "Unable to create fitness function" << endl;
		exit(1);
	}
	
	if (problemDimension<1 || problemDimension>1000)
	{
		cerr << "problem dimension must be in [1..1000]" << endl;
		exit(1);
	}

	if (sizeOfSubcomponents<1 || sizeOfSubcomponents>problemDimension)
	{
		cerr << "problem dimension must be in [1..problem dimension]" << endl;
		exit(1);
	}

    cout << "Optimizing f" << functionIndex << endl;
	cout << "Problem dimension = " << problemDimension << endl;
    cout << "Number of iterations per cycle = " << numItePerCycle << endl;    	
	cout << "The problem is decomposed in " << problemDimension / sizeOfSubcomponents << " equal subcomponents of size " << sizeOfSubcomponents << endl;
    cout << "Number of individuals per subcomponent = " << numOfIndividuals << endl;
	cout << "Number of repetitions = " << numRep << endl;
	cout << "Allowed number of exact function evaluations = " << numberOfEvaluations << endl;

    double time = 0;
    vector< vector<ConvPlotPoint> > convergences;
    for (unsigned k = 0; k < numRep; ++k)
    {
        vector<ConvPlotPoint> convergence;
        CCDE ccde;
        int seed = seeds[k];
        ccde.optimize(f, numberOfEvaluations, sizeOfSubcomponents, numOfIndividuals,
                      convergence, seed, sType, numItePerCycle);
        time = ccde.elapsedTime;
        convergences.push_back(convergence);
    }

    char fName[256];
    FILE *file;

    if (convergences.size() == 0) return;
    if (convergences[0].size() == 0) return;

    if (sType == sNone)
		sprintf_s(fName, "convplot_f%d_dec%d_popsize%d_CCJADE.csv", functionIndex + 1, sizeOfSubcomponents, numOfIndividuals);
    else if (sType == sGP)
		sprintf_s(fName, "convplot_f%d_dec%d_popsize%d_GP_SACCJADE.csv", functionIndex + 1, sizeOfSubcomponents, numOfIndividuals);
    else if (sType == sQPA)
		sprintf_s(fName, "convplot_f%d_dec%d_popsize%d_QPA_SACCJADE.csv", functionIndex + 1, sizeOfSubcomponents, numOfIndividuals);
    else if (sType == sRBFN)
		sprintf_s(fName, "convplot_f%d_dec%d_popsize%d_RBFN_SACCJADE.csv", functionIndex + 1, sizeOfSubcomponents, numOfIndividuals);
	else if (sType == sSVR)
		sprintf_s(fName, "convplot_f%d_dec%d_popsize%d_RBFN_SACCJADE.csv", functionIndex + 1, sizeOfSubcomponents, numOfIndividuals);

    fopen_s(&file, fName, "wt");
    vector<ConvPlotPoint> averageConvergence;

    int maxSize = 0;
    int idOfMaxSize = 0;
    for (unsigned q = 0; q < convergences.size(); ++q)
        if (convergences[q].size() > maxSize)
        {
            maxSize = convergences[q].size();
            idOfMaxSize = q;
        }

    for (unsigned q = 0; q < maxSize; ++q)
    {
        for (unsigned k = 0; k < numRep; ++k)
        {
            if (q < convergences[k].size())
            {
                fprintf(file, "%d; %.8Le; ", convergences[k][q].nfe, convergences[k][q].f);
            }
            else
            {
                fprintf(file, "; ;");
            }
        }
        fprintf(file, "\n");
    }
    fclose(file);

	system("pause");
}


//******************************************************************************************/
//
//
//
//******************************************************************************************/
int main(int argc, char* argv[])
{
    optimization(argc, argv);
}

