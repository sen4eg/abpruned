from copy import deepcopy


class MyPlayer:
    '''Hrac zkousy odhadnout nejhorsi pozici protihracu'''

    def __init__(self, my_color, opponent_color):
        self.mc = my_color
        self.oc = opponent_color
        self.name = "vaginars"

    def move(self, board):
        possible_moves = self.fetch_moves(board)
        flipped = []
        cases = []

        for m in possible_moves:
            flipped.append(self.count_flipped(board, m, self.mc))
            cases.append(self.make_move(board, m, self.mc))
            i = m
            if i[0] == 0 or i[0] == 8 and i[1] == 0 or i[1] == 8:
                return m
        enemy_moves = []
        for c in cases:
            loc = self.fetch_moves(c)
            case_res = 0
            for i in loc:
                if i[0] == 0 or i[0] == 8 and i[1] == 0 or i[1] == 8:
                    case_res += 250
                else:
                    case_res += self.count_flipped(c,i,self.oc)

            enemy_moves.append(case_res)

        return possible_moves[enemy_moves.index(min(enemy_moves))]


    def make_move(self, board, move, color):
        new_board = deepcopy(board)
        mc = color
        oc = 1 - color
        dirs = [(0, 1), (1, -1), (1, 0), (-1, 0), (0, -1), (1, 1), (-1, -1), (-1, 1)]
        to_flip = []
        for s in dirs:
            x, y = move[0], move[1]
            dx, dy = s[0], s[1]
            x, y = x + dx, y + dy
            linear_flip = []
            while 0 <= y < len(board) and 0 <= x < len(board) and board[x][y] == oc:
                linear_flip.append((x, y))
                x, y = x + dx, y + dy
                if 0 <= y < len(board) and 0 <= x < len(board) and board[x][y] == mc:
                    to_flip += linear_flip

        for p in to_flip:
            new_board[p[0]][p[1]] = mc

        return new_board

    def count_flipped(self, board, move, color):
        mc = color
        oc = (1 - color)

        count = 0

        dirs = [(0, 1), (1, -1), (1, 0), (-1, 0), (0, -1), (1, 1), (-1, -1), (-1, 1)]
        for s in dirs:
            x, y = move[0], move[1]
            dx, dy = s[0], s[1]
            x, y = x + dx, y + dy
            dir_count = 0
            while 0 <= y < len(board) and 0 <= x < len(board) and board[x][y] == oc:
                x, y = x + dx, y + dy
                dir_count += 1
                if 0 <= y < len(board) and 0 <= x < len(board) and board[x][y] == mc:
                    count += dir_count

        return count

    def fetch_moves(self, board):
        # let's find all our pawn's and iterate thru them to find all possible moves
        pawns = []
        for x in range(len(board)):
            for y in range(len(board[0])):
                if (board[x][y] == self.mc):
                    pawns.append((x, y))

        # all possible directions to check move possiblity
        dirs = [(0, 0), (-1, 0), (0, 1), (0, -1), (1, 1), (-1, -1), (1, -1), (-1, 1)]
        moves = []
        for p in pawns:
            for s in dirs:
                x, y = p[0], p[1]
                intersected = False
                x, y = x + s[0], y + s[1]
                while (0 <= x < len(board)) and (0 <= y < len(board[0])):
                    if (board[x][y] == self.oc):
                        intersected = True
                    if (board[x][y] == -1):
                        if (intersected):
                            moves.append((x, y))
                        break
                    if (board[x][y] == self.mc):
                        break
                    x, y = x + s[0], y + s[1]

        return moves

