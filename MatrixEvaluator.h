//
// Created by arso on 20.3.23.
//

#ifndef PYTHONCPPEXTENSION_MATRIXEVALUATOR_H
#define PYTHONCPPEXTENSION_MATRIXEVALUATOR_H

//#include "stdlib.h"
#include <vector>
#include <tuple>
#include "AlphaBetaTree.h"
#include <string>

using namespace std;
class MatrixEvaluator{
public:
    MatrixEvaluator()= default;

    tuple<int,int> getBestMove(int **board, int player, int d){
        TreeNode root = TreeNode(board, player, d, 4, nullptr, nullptr, false);
        tuple<int,int> best_move = root.get_best_move();
        return best_move;
    }

private:
//    double weight_matrix[][];
    vector<vector<double>> weights;

};
#endif //PYTHONCPPEXTENSION_MATRIXEVALUATOR_H
