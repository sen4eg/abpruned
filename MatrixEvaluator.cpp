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
#include <fstream>
#define INF std::numeric_limits<double>::infinity()
#include "MyLittlePerceptron.cpp"

using namespace std;
class MatrixEvaluator{
public:
    MatrixEvaluator(){
        ml_agent = nullptr;
    }

    pair<int,int> getBestMove(int board[10][10], int player, int d, int depth){
        ofstream BoardFile("board.txt");
        for(int i = 0; i < d; i++){
            for(int j = 0; j < d; j++){
                BoardFile << board[i][j] << " ";
            }
            BoardFile << endl;
        }

        BoardFile.close();
        double alp = -INF;
        double bet = INF;
        double v;
        pair<int, int> res;
        ABprune(board, player, d, depth, alp, bet, v, res, false);

        return res;
    }

    void HelloPy(){
        cout << "hi" << endl;
    }

    void debugBoard(int player, int depth){
        ifstream BoardFile("board.txt");
        int c;
        vector<int> data;
        while(BoardFile >> c){
            data.push_back(c);
        }
        int d = 8;
        if(data.size() != 64){
            d = data.size() > 64? 10:6;
        }
        int board[10][10];// fix it less prob

        for (int i = 0; i < data.size(); i++){
            board[i/d][i%d] = data[i];
        }
        double alp = -INF;
        double bet = INF;
        double v;

        pair<int,int> res;
        ABprune(board, player, d, depth, alp, bet, v, res, false);

        cout << res.first << " " << res.second << endl;
    }

    boost::python::tuple calculateBestMove(boost::python::list matrix, int player, int max_depth){
        int dim = boost::python::len(matrix);
        int board[10][10];
        for (int i = 0; i < dim; i++){
            boost::python::list row = boost::python::extract<boost::python::list>(matrix[i]);
            vector<int> vec((boost::python::stl_input_iterator<int>(row)),boost::python::stl_input_iterator<int>());
            for(int j = 0; j < dim; j++){
                board[i][j] = vec[j];
            }
        }
        auto p = getBestMove(board, player, dim, max_depth);
        return boost::python::make_tuple(p.second, p.first);
    }

    void createMLAgent(boost::python::list archi, string name){
        vector<int> vec((boost::python::stl_input_iterator<int>(archi)),boost::python::stl_input_iterator<int>());
        ml_agent = new MultiLayerPerceptron(vec, vector<int>(vec.size()));
    }

    void isMLassigned() const{
        if (ml_agent != nullptr){
            cout<<"DA"<<endl;
        }else{
            cout<<"NET"<<endl;
        }
    }
private:
//    double weight_matrix[][];
    vector<vector<double>> weights;
    MultiLayerPerceptron* ml_agent;
//    static
};

BOOST_PYTHON_MODULE(MatrixEval){
    using namespace boost::python;
    class_<MatrixEvaluator>("MatrixEvaluator", init<>())
            .def("hi", &MatrixEvaluator::HelloPy)
            .def("calculateBestMove", &MatrixEvaluator::calculateBestMove)
            .def("createMLAgent", &MatrixEvaluator::createMLAgent)
            .def("isMLassigned", &MatrixEvaluator::isMLassigned)
        ;
}