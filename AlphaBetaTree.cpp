//
// Created by arso on 21.3.23.
//
using namespace std;
#include <string>
#include <tuple>
#include <vector>
#include <set>
#include <iostream>
#include "MyLittlePerceptron.cpp"


#define OP_MET 4
#define PL_MET 2
#define EP_MET 1

void print_board(int board[10][10], int player, int dim){
//    ofstream BoardFile("board.txt");
    cerr << "Pl:"<< player << endl;
    for(int i = 0; i < dim; i++){
        for(int j = 0; j < dim; j++){
            cerr << (board[i][j] < 0?"":" ") << board[i][j] << " ";
        }
        cerr << endl;
    }
//    out.close();
}

bool minMaxComparator(const double &a, const double &b, bool isMin){
    return (a<b) == isMin;
}

void process_tile(int x, int y, const int OP, const int PL, unsigned int &flag,
                  int &op_x, int &op_y, int current_place,
                  int &ix, int &iy);

bool inUpperBound(int x, int y, int dim);

//int **apply_move(int **board, pair<int, int> move, int dim, int pl);

bool inBound(int x, int y, int i);

vector<pair<int, int>> get_all_moves(int board[10][10], int player, int dim);

double evaluate(int board[10][10], int player, int dim, bool isMin, MultiLayerPerceptron *pPerceptron);

void apply_move(int oldBoard[10][10], int new_board[10][10], pair<int, int> move, int dim, int player);

void
ABprune(int board[10][10], int player, int dim, int depth, double &alpha, double &beta, double &v, pair<int, int> &res,
        bool isMin, MultiLayerPerceptron *pPerceptron) {
    if(!depth){
        v = evaluate(board, player, dim, isMin, pPerceptron);
        return;
    }
//    print_board(board, player, dim);

    vector<pair<int, int>> moves = get_all_moves(board, player, dim);

    if(moves.size() == 0){
        if(moves.size() == 0){
            // game overs here
            v = evaluate(board, player, dim, isMin, pPerceptron);
        }else{
            pair<int, int> temp;
            ABprune(board, player, dim, depth, alpha, beta, v, temp, !isMin, pPerceptron); // ensure not move like inverted node, shouldn't happen
        }
        return;
    }

    vector<double> move_scores(moves.size());
    int idx = 0;
    double a = alpha, b = beta, v1;
    for (int i = 0; i < moves.size() && b >= a; i++){
//        int **new_board = apply_move(board, moves[i], dim, player ^ isMin);
        int new_board[10][10];
//        print_board
//        print_board(board, dim, dim);
        apply_move(board, new_board, moves[i], dim, player ^ isMin);
//        print_board(new_board, dim, dim);
        pair<int, int> temp;
        ABprune(new_board, player, dim, depth - 1, a, b, v1, temp, !isMin, pPerceptron);
        move_scores[i] = v1;
        if(minMaxComparator(v1, move_scores[idx], isMin)){
            idx = i;
        }
        if(isMin && b > v1){
            b = v1;
        }
        if(!isMin && a < v1){
            a = v1;
        }
    }
//    cout << "IDX: " << idx << " PPOS:" << moves.size() << endl;
    v = move_scores[idx];
    res = moves[idx];
}

void apply_move(int oldBoard[10][10], int new_board[10][10], pair<int, int> move, int dim, int pl) {
    int op = 1 - pl;
    for (int i = 0; i < dim; i++){
        copy(oldBoard[i], oldBoard[i] + dim, new_board[i]);
    }

    for (int i = 0; i < 8; i++){
        int dx = ((i%4) != 0) * (-1 + 2 * (i < 4));
        int dy = ((i%4) != 2) * (-1 + 2 * (i%7 > 1));// damn, c is great cuz of this kind of math

        int x = move.first + dx;
        int y = move.second + dy;

        while(inBound(x, y, dim) && new_board[y][x] == op){
            x = x + dx;
            y = y + dy;
        }

        if (inBound(x, y, dim) && new_board[y][x] == pl){
            x = move.first;
            y = move.second;
            while(inBound(x, y, dim) && new_board[y][x] != pl){
                new_board[y][x] = pl;
                x = x + dx;
                y = y + dy;
            }
        }
    }

}

double evaluate(int board[10][10], int player, int dim, bool isMin, MultiLayerPerceptron *pPerceptron) {
    if (pPerceptron != nullptr){
        double res = pPerceptron->evaluateBoard(board, player, dim);
//        cout<< "res:" << res<<endl;
        return res;
    }
    int count[3] = {0};
    for (int y = 0; y < dim; y++){
        for(int x = 0; x < dim; x++){
            count[1 + board[y][x]]++;
        }
    }
    return count[player + 1] * 1.0 / count[2 - player];
}

vector<pair<int, int>> get_all_moves(int board[10][10], int player, int dim) { // this code chunk ended up to be ugly as hell, but it's my ugly as hell code chunk, also it may be about 2 times faster in 'some' cases then more straight forward implementation
//    vector<tuple<int, int>> res;
    set<pair<int, int>> moves;
    print_board(board, player, dim);
    const int OP = 1 - player, PL = player;
    for (int i = 0; i < 4; i++){
        int dx = i > 0, dy = 1 - (i % 3);
        int x = 0, y = 0 + 2 * (i > 1);
        if(i == 3){
            y = dim - y;
        }
        while(inBound(x + dx, y + dy, dim) ){
            unsigned int flag = 0; //   1 e 2 p 4 o
            int op_x,   op_y;
            int current_place;
            cout << "FROM: " << x << ":" << y << " ";

            while(inBound(x, y, dim)){
                int ix = -1, iy = -1;
                current_place = board[y][x];// xy? test!(notation in python exmpl files) shouldn't be an issue due to symmetricity
                process_tile(x, y, OP, PL, flag, op_x, op_y, current_place, ix, iy);
                if(ix != -1){
                    moves.insert(make_pair(ix, iy));
                }
                x = x+dx;
                y = y+dy;
            }
            cout << "TO: " << x << ":" << y << " " << endl;

            if(i < 2){
                if (dx){
                    y = y + dx;
                    x = 0;
                }
                else {
                    x = x + dy;
                    y = 0;
                }
            }else{
                int nx = x;
//                int ny = y;

                if(i % 2){
                    if(x == dim){
                        x = (dim - y) + 1;
                        y = 0;
                    } else{
                        x = 0;
                        y = dim - nx -1;
                    }
                }else{
                    if(x == dim){
                        x = y + 2;
                        y = dim - 1;
                    } else{
                        x = 0;
                        y = nx;
                    }
                }
            }
        }
    }
    return {moves.begin(), moves.end()};
}


void process_tile(int x, int y, const int OP, const int PL, unsigned int &flag, int &op_x, int &op_y,
                            int current_place, int &ix, int &iy){
    if (current_place == OP){
        flag = flag | OP_MET;
    } else if (current_place == PL){
        if(flag & OP_MET && flag & EP_MET){ // --110011-- -> -*110011
            ix = op_x;
            iy = op_y;
            flag = 0;
        }
        flag = PL_MET;
    } else { // EMPTY
        if (flag & OP_MET && flag & PL_MET) { // --110011-- -> --110011*-
            ix = x, iy = y;
        }
        op_x = x, op_y = y;
        flag = EP_MET;
    }
}
//
bool inUpperBound(int x, int y, int dim) {
    return y < dim && x < dim;
}
//

bool inBound(int x, int y, int dim) { return inUpperBound(x, y, dim) && x >= 0 && y >= 0; }

