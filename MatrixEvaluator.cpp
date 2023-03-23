//
// Created by arso on 20.3.23.
//

//#include "stdlib.h"
#include <vector>
#include <tuple>
#include "AlphaBetaTree.cpp"
#include <string>
#include <limits>
#include <iostream>
#include <boost/python.hpp>

#define INF std::numeric_limits<double>::infinity()

using namespace std;
class MatrixEvaluator{
public:
    MatrixEvaluator()= default;

    static pair<int,int> getBestMove(int **board, int player, int d){
        cout << "hi" << endl;
        double alp = -INF;
        double bet = INF;
        double v;
        auto res = ABprune(board, player, d, 5, alp, bet, v, false);

        return res;
    }
    static void HelloPy(){
        cout << "hi" << endl;
//        double alp = -INF;
//        double bet = INF;
//        double v;
//        auto res = ABprune(board, player, d, 5, alp, bet, v, false);
//
//        return res;
    }
    static boost::tuple<int,int> calculateBestMove(boost::python::list matrix, int player, int max_depth){
        int dim = boost::python::len(matrix);
        int ** board = new int *[dim];
        for (int i = 0; i < dim; i++){
            board[i] = new int[dim];
            boost::python::list row = boost::python::extract<boost::python::list>(matrix[i]);
            vector<int> vec((boost::python::stl_input_iterator<int>(row)),boost::python::stl_input_iterator<int>());
            for(int j = 0; j < dim; j++){
                board[i][j] = vec[j];
            }
        }


    }


private:
//    double weight_matrix[][];
    vector<vector<double>> weights;

};

BOOST_PYTHON_MODULE(MatrixEval){
    using namespace boost::python;
    class_<MatrixEvaluator>("MatrixEvaluator")
            .def("hi", &MatrixEvaluator::HelloPy)
            .def("calculateBestMove", &MatrixEvaluator::calculateBestMove)
}