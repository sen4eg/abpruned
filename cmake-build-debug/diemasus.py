from os import makedirs
from random import random, choice
from typing import Set

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

class MyPlayer:
    '''Taha nahodne'''
    def __init__(self, my_color, opponent_color):
        self.color = my_color
        self.op_color = opponent_color
        self.name = "tretidmi"
    
    def move(self, board):
        possible_moves = calculateMovesFor(board, self.color)
        return choice(possible_moves)
