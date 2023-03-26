# import random_player
from game_board import GameBoard
import time, getopt, sys
from player_creator import create_player
import ai_player
# import naive_player
import reverso
import diemas
import diemasus

class HeadlessReversiCreator(object):
    '''
    Creator of the Reversi game without the GUI.
    '''

    def __init__(self, player1, player1_color, player2, player2_color, board_size=8):
        '''
        :param player1: Instance of first player
        :param player1_color: color of player1
        :param player2: Instance of second player
        :param player1_color: color of player2
        :param boardSize: Board will have size [boardSize x boardSize]
        '''
        self.board = GameBoard(board_size, player1_color, player2_color)
        self.player1 = player1
        self.player2 = player2
        self.current_player = self.player1
        self.current_player_color = player1_color
        self.player1_color = player1_color
        self.player2_color = player2_color

    def play_game(self):
        '''
        This function contains game loop that plays the game.
        '''
        # TODO: Time limit for move
        correct_finish = True
        while self.board.can_play(self.current_player_color):
            startTime = time.time()
            move = self.current_player.move(self.board.get_board_copy())
            endTime = time.time()
            moveTime = (endTime - startTime) * 1000
            if move is None:
                print('Player %d reurns None istead of a valid move. Move takes %.3f ms.' % (self.current_player_color, moveTime))
                correct_finish = False
                break
            else:
                print('Player %d wants move [%d,%d]. Move takes %.3f ms.' % (self.current_player_color, move[0], move[1], moveTime))

            move = (int(move[0]),int(move[1]))
            if self.board.is_correct_move(move, self.current_player_color):
                print('Move is correct')
                self.board.play_move(move, self.current_player_color)

            else:
                print('Player %d made the wrong move [%d,%d]' % (self.current_player_color, move[0], move[1]))
                correct_finish = False
                break

            self.change_player()
            if not self.board.can_play(self.current_player_color):
                print('No possible move for Player %d' % (self.current_player_color))
                self.change_player()
                if self.board.can_play(self.current_player_color):
                    print('Player %d plays again ' % (self.current_player_color))
                else:
                    print('Game over')


            self.board.print_board()
        if correct_finish:
            self.printFinalScore()
        else:
            print('Game over.')
            if self.current_player_color == self.player1_color:
                print('Winner is player %d.' % (self.player2_color))
            else:
                print('Winner is player %d.' % (self.player1_color))

    def play_game_silent(self):
        '''
        This function contains game loop that plays the game.
        '''
        # TODO: Time limit for move
        correct_finish = True
        time_tot = 0
        while self.board.can_play(self.current_player_color):
            startTime = time.time()
            move = self.current_player.move(self.board.get_board_copy())
            endTime = time.time()
            moveTime = (endTime - startTime) * 1000
            time_tot += moveTime
            if move is None:
                correct_finish = False
                break

            move = (int(move[0]),int(move[1]))
            if self.board.is_correct_move(move, self.current_player_color):
                self.board.play_move(move, self.current_player_color)

            else:
                correct_finish = False
                if getattr(self.current_player, 'me', None) is not None:
                    self.current_player.me.save_board("Error-" + str(int(time_tot)) + "-"+ str(int(moveTime))+".txt")
                break

            self.change_player()
            if not self.board.can_play(self.current_player_color):
                self.change_player()

            # self.board.print_board()
        # print(f"Timeforgame: {time_tot}")
        if(not correct_finish):
            return (0.3, 0.3)
        return self.getFinalScore(correct_finish)

    def change_player(self):
        '''
        Change the current_player
        '''
        if self.current_player == self.player1:
            self.current_player = self.player2
            self.current_player_color = self.player2_color
        else:
            self.current_player = self.player1
            self.current_player_color = self.player1_color
    def getFinalScore(self, correct):
        p1Stones = 0
        p2Stones = 0
        for x in range(self.board.board_size):
            for y in range(self.board.board_size):
                if self.board.board[x][y] == 0:
                    p1Stones += 1
                if self.board.board[x][y] == 1:
                    p2Stones += 1
        # if not correct:
        #     print("AAAAilldoitlater")
        return (p1Stones, p2Stones)
    def printFinalScore(self):
        p1Stones = 0
        p2Stones = 0
        for x in range(self.board.board_size):
            for y in range(self.board.board_size):
                if self.board.board[x][y] == 0:
                    p1Stones += 1
                if self.board.board[x][y] == 1:
                    p2Stones += 1

        print('\n\n-----------------------------\n')
        print('Final score:\n\nPlayer%d:Player%d\n\t[%d:%d]\n' % (self.player1_color, self.player2_color, p1Stones, p2Stones))
        if p1Stones > p2Stones:
            print('Player %d wins!' % (self.player1_color))
        elif p2Stones > p1Stones:
            print('Player %d wins!' % (self.player2_color))
        else:
            print('Draw')
        print('\n-----------------------------\n\n')

def produce_players(pl_module, amount, board_size, arch = None, actimel = [], genodesc = ""):
    result = []
    for i in range(amount):
        player = create_player(pl_module.MyPlayer, 0, 1, board_size)
        if arch != None:
            player.me.createMLAgent(arch, actimel)
        player.name = genodesc + "-" + str(i)
        result.append(player)

    return result


def updateFit(player, amount):
    player.fit = player.fit + amount

def setFit(player, amount):
    player.fit = amount


def nulifyFit():
    global player
    for player in players:
        setFit(player, 0)


def runTourment(players):
    for i in range(len(players)):
        for j in range(len(players)):
            if i == j:
                continue
            p1_color, p2_color = (0, 1) if i < j else (1, 0)
            players[i].my_color, players[i].opponent_color = p1_color, p2_color
            players[j].my_color, players[j].opponent_color = p2_color, p1_color

            game = HeadlessReversiCreator(players[i], p1_color, players[j], p2_color, board_size)
            res = game.play_game_silent()
            # print(res)
            if res[0] == res[1]:
                updateFit(players[j], 0.5)
                updateFit(players[i], 0.5)
            else:
                if res[0] > res[1]:
                    updateFit(players[i], 1)
                else:
                    updateFit(players[j], 1)


def genocode(players, limit = 350):
    new_players=[]
    mutation_factor = 0.0005
    mutation_factor_up = 0.0001
    not_ai = ["NAI", "REV", "RAN", "DIE"]
    for player in players:
        if player.name[:3] in not_ai:
            mutation_factor = mutation_factor + mutation_factor_up
        else:
            player.me.mutate(mutation_factor)
        new_players.append(player)

    return new_players


def print_report(players, epoch):
    print("Printing for " + str(epoch) + " epoch")
    for player in players:
        print(f"{player.name}:{player.fit}")


def save_agentos(players, epoch):
    count = 0
    for player in players:
        if getattr(player, 'me', None) is not None:

            name = "nn/EP" + str(epoch) + "_" + player.name + str(count) + ".bin"

            player.me.save_gene(name)
            count+=1


if __name__ == "__main__":
    # (choices,args) = getopt.getopt(sys.argv[1:],"")
    p1_color = 0
    p2_color = 1
    board_size = 8

    epoch_count = 10000
    printout_thre = 10

    importsCorrect = True
    colors = [p1_color, p2_color]
    players =  produce_players(ai_player, 2, board_size, [64,32,16,1],[0,0,1], "AAB1")
    players += produce_players(diemas, 2, board_size, None, [0,1,2], "DIE")
    players += produce_players(ai_player, 2, board_size, [64,32,16,1],[1,1,2], "BBC1")
    players += produce_players(ai_player, 2, board_size, [64,32,16,1],[1,2,1], "BCB1")
    players += produce_players(ai_player, 2, board_size, [64,32,16,1],[1,2,2], "BCC1")
    players += produce_players(ai_player, 2, board_size, [64,32,16,1],[0,0,2], "AAC1")
    # players += produce_players(diemasus, 10, board_size, None, [], "SUS")

    # players += produce_players(ai_player, 5, board_size, [64,16,32,16,1], "OWO")
    # players += produce_players(ai_player, 5, board_size, [64,48,32,16,1], "GRA")
    # players += produce_players(reverso, 2, board_size, None, "REV")
    # players += produce_players(diemasus, 5, board_size, None, "RAN")
    # players += produce_players(diemas, 5, board_size, None, "DIE")

    # players = produce_players(ai_player, 5, board_size, None, "NAI")

    for i in range(epoch_count):
        nulifyFit()
        runTourment(players)
        players.sort(key=lambda x: -x.fit)
        if (i % printout_thre) == 0:
            print_report(players, i)
            save_agentos(players, i)
        players = genocode(players)

