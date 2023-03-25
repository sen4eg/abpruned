from copy import deepcopy
from os import makedirs
from random import random, choice
from typing import Set

DEBUG_PRINT = False

def print_board(board):
    '''
    Function for printing game board to console.
    '''
    print("-------------<>>><")
    for x in range(len(board)):
        row_string = ''
        for y in range(len(board[x])):
            if board[x][y] == -1:
                row_string += ' -'
            else:
                row_string += ' ' + str(board[x][y])
        print(row_string)
    print("-------------<<<><")

def makedirs():
    res = []
    for j in range(-1, 2):
        for i in range(-1, 2):
            if i != 0 or j != 0:
                res.append(   (i, j)   )
    return res

dirs = makedirs()

def isInbound(coord):
    return coord >= 0 and coord < 8

def isOpponent(cell, color):
    return cell != -1 and cell != color

def isEmpty(cell):
    return cell == -1

def hasCorner(moves):
    return (0, 0) in moves or (0, 7) in moves or (7, 0) in moves or (7, 7) in moves

def hasEdges(moves):
    for move in moves:
        if move[0] == 0 and move[1] != 0 and move[1] != 7:
            return True
        if move[0] == 7 and move[1] != 0 and move[1] != 7:
            return True
        if move[1] == 0 and move[0] != 0 and move[0] != 7:
            return True
        if move[1] == 7 and move[0] != 0 and move[0] != 7:
            return True


def calculateMovesFor(board, color):
    possible_moves = set()
    for ix in range(8):
        for iy in range(8):
            cell = board[ix][iy]
            if(cell == color):
                x, y = ix, iy 
                for dir in dirs:
                    dx, dy = dir[0], dir[1]
                    tx, ty = x+dx, y+dy
                    hasMoved = False
                    while(isInbound(tx) and isInbound(ty) and isOpponent(board[tx][ty], color) ):
                        tx, ty = tx+dx, ty+dy
                        hasMoved = True
                    if(isInbound(tx) and isInbound(ty) and isEmpty(board[tx][ty]) and hasMoved):
                        possible_moves.add((tx, ty))
    return list(possible_moves)

def simulateMove(board, color, move):
    newBoard = deepcopy(board)
    x, y = move[0], move[1] 
    score_gain = 1
    for dir in dirs:
        affected_cells = []
        dx, dy = dir[0], dir[1]
        tx, ty = x+dx, y+dy
        hasMoved = False
        while(isInbound(tx) and isInbound(ty) and isOpponent(board[tx][ty], color) ):
            affected_cells.append( (tx, ty) )
            tx, ty = tx+dx, ty+dy
            hasMoved = True

        if(isInbound(tx) and isInbound(ty) and board[tx][ty] == color and hasMoved):
            for cell in affected_cells:
                newBoard[cell[0]][cell[1]] = color
                score_gain += 1

    newBoard[move[0]][move[1]] = color
    return (newBoard, score_gain)

class MyPlayer:
    '''Hrac analyzujici vsichni sve a protihracovi mozne tahy'''
    def __init__(self, my_color, opponent_color):
        self.color = my_color
        self.op_color = opponent_color
        self.name = "tretidmi"
    
    def move(self, board):
        possible_moves = calculateMovesFor(board, self.color)
        if(DEBUG_PRINT):
            print_board(board)
            print("-------------")
        hi_score = -999999999
        hi_move = possible_moves[0]
        for move in possible_moves:
            move_board, move_score = simulateMove(board, self.color, move)
            enemy_moves = calculateMovesFor(move_board, self.op_color)
            
            if(DEBUG_PRINT):
                print_board(move_board)
                print("-------------<>><")

            if(hasCorner(enemy_moves)):
                move_score -= 19
            if(hasEdges(enemy_moves)):
                move_score -= 8

            if move_board[3][3] == self.color:
                move_score+=2
            if move_board[3][4] == self.color:
                move_score+=2
            if move_board[4][3] == self.color:
                move_score+=2
            if move_board[4][4] == self.color:
                move_score+=2
            if move_board[2][2] == self.color:
                move_score+=2
            if move_board[5][2] == self.color:
                move_score+=2
            if move_board[2][5] == self.color:
                move_score+=2
            if move_board[5][5] == self.color:
                move_score+=2

            if move in [(0, 0), (0, 7), (7, 0), (7, 7)]:
                move_score+=16

            if move[0] == 0 and move[1] != 0 and move[1] != 7:
                move_score+=4
            if move[0] == 7 and move[1] != 0 and move[1] != 7:
                move_score+=4
            if move[1] == 0 and move[0] != 0 and move[0] != 7:
                move_score+=4
            if move[1] == 7 and move[0] != 0 and move[0] != 7:
                move_score+=4
                
            if move_score > hi_score:
                hi_score = move_score
                hi_move = move

        return hi_move
