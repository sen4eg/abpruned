//
// Created by arso on 21.3.23.
//

#ifndef MATRIXEVALUATOR_ALPHABETATREE_H
#define MATRIXEVALUATOR_ALPHABETATREE_H
using namespace std;
#include <string>
#include <tuple>
#include <vector>
#include <set>

#define EMPTY 0

#define OP_MET 4
#define PL_MET 2
#define EP_MET 1

class TreeNode{
public:
    TreeNode(int ** board, int player, int d, int depth, double *alpha, double *beta);

    tuple<int, int> get_best_move();

private:
    int **board;
    int player;
    int dim;
    int possible_moves_count=0;
    std::vector<TreeNode> childs;

    double alpha{}, beta{};// make global, extract reference
    vector<pair<int, int>> get_all_moves();

    void
    process_tile(int x, int y, const int OP, const int PL, unsigned int &flag, int &op_x, int &op_y, int current_place,
                 int &ix, int &iy) const;

    bool is_inbound(int x, int y);
};

TreeNode::TreeNode(int **board, int player, int d, int depth, double *alpha, double *beta) {
    if (alpha == NULL){
        // very init
    }

}

tuple<int, int> TreeNode::get_best_move() {
    vector<pair<int, int>> moves = get_all_moves();
}

vector<pair<int, int>> TreeNode::get_all_moves() { // this code chunk ended up to be ugly as hell, but it's my ugly as hell code chunk, also it may be about 2 times faster in 'some' cases then more straight forward implementation
//    vector<tuple<int, int>> res;
    set<pair<int, int>> moves;
    for (int i = 0; i < 4; i++){
        int dx = i > 0, dy = 1 - (i % 3);
        int x = 0, y = 0 + (dim - 1) * (i == 3);
        const int OP = 0, PL = 1;

        while( is_inbound(x + dx, y + dy) ){
            unsigned int flag; //   1 e 2 p 4 o
            int op_x,   op_y;
            int current = 0;
            int current_place;
            while(is_inbound(x, y)){
                int ix = -1, iy = -1;
                current_place = board[y][x];// xy? test!(notation in python exmpl files) shouldn't be an issue due to symmetricity
                process_tile(x, y, OP, PL, flag, op_x, op_y, current_place, ix, iy);
                if(ix != -1){
                    moves.insert(make_pair(ix, iy));
                }
                current++;
                x = x+dx;
                y = y+dy;
            }

            if(i < 2){
                if (dx) y = y + dx;
                else x = x + dy;
            }else{
                if(i % 2){
                    if(current < dim){
                        x = 0;
                        y = dim - current;
                    }else{
                        x = (current % dim) + 1;
                        y = 0;
                    }
                }else{
                    if(current < dim){
                        x = 0;
                    } else{
                        x = (current % dim) + 1;
                    }
                    y = dim - 1;
                }
            }
        }
    }
    return {moves.begin(), moves.end()};
}

void TreeNode::process_tile(int x, int y, const int OP, const int PL, unsigned int &flag, int &op_x, int &op_y,
                            int current_place, int &ix, int &iy) const {
    if (current_place == OP){
        flag = flag | OP_MET;
    } else if (current_place == PL){
        if(flag & (OP_MET | EP_MET)){ // --110011-- -> -*110011
            ix = op_x;
            iy = op_y;
            flag = 0;
        }
        flag = flag & (~OP_MET) | PL_MET;
    } else { // EMPTY
        if (flag & (OP_MET | PL_MET)) { // --110011-- -> --110011*-
            ix = x, iy = y;
        }
        op_x = x, op_y = y;

        flag = EP_MET;
    }
}

bool TreeNode::is_inbound(int x, int y) {
    return y < dim && x < dim;
}

#endif //MATRIXEVALUATOR_ALPHABETATREE_H
