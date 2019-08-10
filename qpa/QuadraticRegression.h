//============================================================================================
// SACCJADE - Surrogate-Assisted Cooperative Coevolutionary version of JADE
//============================================================================================
// Name        : QuadraticRegression.h
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


#ifndef QuadraticRegression_hpp
#define QuadraticRegression_hpp

#include <vector>

class QuadraticRegression 
{

public:
	QuadraticRegression(unsigned dim, unsigned np);	
	QuadraticRegression(std::vector< std::vector<double> > &_points, std::vector< double > &_values);
   ~QuadraticRegression();
    void setData(std::vector< std::vector<double> > &_points, std::vector< double > &_values);
    void solve(double **Mat, double *U, int neq);
    int maxModInColumnindex(double **A, int rows, int column, int starting_column);
    double evaluate(std::vector<double> &point);
    std::vector<double> evaluateGradient(std::vector<double> &point);
	void reset() { ready = false; };
         
    int dim;
    int nt;
    int numPoints;   
    double **X;   //
    double **B;   //
    double *c;
	bool ready;
   //double *mean;
   //double *loBound;
   //double *hiBound;
};

#endif
