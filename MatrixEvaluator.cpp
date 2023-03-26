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
        ABprune(board, player, d, depth, alp, bet, v, res, false, ml_agent);

        return res;
    }

    void HelloPy(){
        cout << "hi" << endl;
    }

    void debugBoard(int player, int depth, string board_file){
        ifstream BoardFile(board_file);
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
        ABprune(board, player, d, depth, alp, bet, v, res, false, ml_agent);

        cout << res.first << " " << res.second << endl;
    }

    boost::python::tuple calculateBestMove(boost::python::list matrix, int player, int max_depth){
        dim = boost::python::len(matrix);

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

    void createMLAgent(boost::python::list archi, boost::python::list activ){
        vector<int> arrc((boost::python::stl_input_iterator<int>(archi)),boost::python::stl_input_iterator<int>());
        vector<int> act((boost::python::stl_input_iterator<int>(activ)),boost::python::stl_input_iterator<int>());

        ml_agent = new MultiLayerPerceptron(arrc, act);
    }

    void mutate(double factor){
        if( ml_agent != nullptr ) ml_agent->mutate(factor, 0);
    }

    bool isMLassigned(){
        return ml_agent != nullptr;
    }

    void save_board(string path) const {
        ofstream BoardFile(path);
        for(int i = 0; i < dim; i++){
            for(int j = 0; j < dim; j++){
                BoardFile << board[i][j] << " ";
            }
            BoardFile << endl;
        }
        BoardFile.close();
    }

    MultiLayerPerceptron * getAgent(){
        return ml_agent;
    }

    void gene_sting(MatrixEvaluator me){
        this->ml_agent = new MultiLayerPerceptron(*(me.getAgent()));
    }

    void save_gene(string path){
        ml_agent->save(path);
    }

    ~MatrixEvaluator(){
        save_board("board-as.txt");
        free(ml_agent);
    }
private:
//    double weight_matrix[][];
//    vector<vector<double>> weights;
    MultiLayerPerceptron* ml_agent;
    int board[10][10];
    int dim;
//    static
};

BOOST_PYTHON_MODULE(MatrixEval){
    using namespace boost::python;
    class_<MatrixEvaluator>("MatrixEvaluator", init<>())
            .def("hi", &MatrixEvaluator::HelloPy)
            .def("calculateBestMove", &MatrixEvaluator::calculateBestMove)
            .def("createMLAgent", &MatrixEvaluator::createMLAgent)
            .def("isMLassigned", &MatrixEvaluator::isMLassigned)
            .def("mutate", &MatrixEvaluator::mutate)
            .def("save_board", &MatrixEvaluator::save_board)
            .def("gene_sting", &MatrixEvaluator::gene_sting)
            .def("save_gene", &MatrixEvaluator::save_gene)
            ;
}