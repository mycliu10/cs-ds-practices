import sys
sys.path.append("../")
import numpy as np
import itertools
import random
import json
import myhelp


class KoB:

    def __init__(self, num_pairs, num_courts, num_games_per_pair):
        self._num_pairs = num_pairs
        self._num_courts = num_courts
        self._num_games_per_pair = num_games_per_pair



    def checkValid(self, game, valid_games, count_games=None, strict=0):
        game_set = set(game)
        is_valid = True
        if len(game_set) != 4:
            is_valid = False
        for valid_game in valid_games:
            valid_game_set = set(valid_game)
            check_set = valid_game_set.union( game_set )
            if strict==0:
                if len(check_set) <= 6:
                    is_valid = False
            elif strict==1:
                opponent = [ set([valid_game[0],valid_game[1]]), \
                             set([valid_game[2],valid_game[3]]), \
                             set([game[0],game[1]]),    \
                             set([game[2],game[3]]),    \
                           ]
                if     len(opponent[0].union(opponent[2])) <= 2 \
                    or len(opponent[0].union(opponent[3])) <= 2 \
                    or len(opponent[1].union(opponent[2])) <= 2 \
                    or len(opponent[1].union(opponent[3])) <= 2 :
                    is_valid = False

        if count_games is not None:                
            for n in game:
                if count_games[n] >= self._num_games_per_pair:
                    is_valid = False

        return is_valid



    def addGame(self, game, valid_games, count_games):
        idxmin = np.argmin( count_games )
        if idxmin in game:
            for n in game:
                count_games[n] += 1
            valid_games.append( game )
            return True

        return False


    def formGamesWithPolicy(self, num_segs, num_choices_per_seg, order=0):
        num_choices = 4 // num_choices_per_seg
        seg_size = self._num_pairs // num_segs
        games = []
        # inter-segment
        for seg_index in itertools.combinations(range(num_segs), num_choices):
            for inseg_index in itertools.product(range(seg_size), repeat=4):
                game = [ seg_index[n//num_choices_per_seg]*seg_size + inseg_index[n] for n in range(4) ]
                if order == 2:
                    def f(i):
                        if i % 2 == 0:
                            return i
                        else:
                            return self._num_pairs - i
                    game = [ f(i)  for i in game ]

                elif order == 1:
                    game = [ self._num_pairs-1-i for i in game ]
                
                gamelist = [ 
                            [game[0], game[1], game[2], game[3]],
                            [game[0], game[1], game[3], game[2]],
                            [game[2], game[1], game[0], game[3]],
                            ]
                games += gamelist

        return games



    def makeGamesByRound(self):
        self._games = []
        

    def makeGames(self):
        self._games = []
        count_games = np.zeros( self._num_pairs, dtype=int )
        
        args_list = [ 
#                      [9, 4, 0, 0],
#                      [9, 2, 0, 0],
#                      [4, 1, 0, 0],
                      [4, 1, 0, 0],
                      [4, 1, 1, 1],
#                      [4, 1, 2, 1],
#                      [4, 4, 0, 1],
#                      [4, 2, 0, 1],
#                      [4, 2, 1, 1],
#                      [4, 2, 2, 1],
#                      [9, 1, 1, 1],
#                      [9, 1, 2, 1],
#                      [9, 1, 0, 1],
#                      [1, 4, 1, 1],
#                      [1, 4, 0, 0],
#                      [1, 4, 1, 1],
#                      [1, 4, 1, 1],
                    ]

        for args in args_list:
            is_any_added = True
            while is_any_added:
                is_added = False
                games = self.formGamesWithPolicy(args[0], args[1], order=args[2])
                for game in games:
                    is_valid = self.checkValid(game, self._games, count_games=count_games, strict=args[3])
                    if is_valid:
                        is_added = is_added or self.addGame(game, self._games, count_games)
                is_any_added = is_added
        return self._games
            

    
    def checkGames(self):
        print( "Game-making results:" )
        for g in self._games:
            print( f"{g[0]:2} {g[3]:2} vs {g[1]:2} {g[2]:2}" )
        print( f"A total number of {len(self._games)} games." )
        for n in range(self._num_pairs):
            games = []
            teammates = []
            opponents = []
            for g in self._games:
                if n in g:
                    games.append( g )
                    if n in g[0:4:3]:
                        teammates += [i for i in g[0:4:3] if i != n]
                        opponents += g[1:3]
                    elif n in g[1:3]:
                        teammates += [i for i in g[1:3] if i != n] 
                        opponents += g[0:4:3] 
            opponents.sort()
            teammates.sort()
            mates = opponents + teammates
            print( f"pair {n:2} plays {len(games):2} games with/against m/m/m {min(mates):4} {np.median(mates):6} {max(mates):4}, encounters {len(set(mates))} pairs" )
            if len(set(teammates)) < self._num_games_per_pair:
                print( f"teammates are {teammates} and opponents are {opponents}" )


    def countByes(self, n, games):
        game_rank = []
        for i,g in enumerate(games):
            if n in g:
                game_rank.append( i )
        game_round = [ r//self._num_courts for r in game_rank]
        game_round.sort()
        if len(game_round) <= 1:
            minbyes = 0
            maxbyes = 0
        else:
            minbyes = 9999
            maxbyes = 0
            for r in range(1, len(game_round)):
                byes = game_round[r] - game_round[r-1] - 1
                if byes < minbyes:
                    minbyes = byes
                if byes > maxbyes:
                    maxbyes = byes

        return game_round, minbyes, maxbyes



    def scheduleGames(self):
        games_left = self._games.copy()
        random.shuffle( games_left )
        games_scheduled = []

        while len(games_left) > 0:
            round_scheduled_list = []
            index_game_list = []
            for index_game,game in enumerate(games_left):
                round_scheduled = -1
                for n in game:
                    for index_game_scheduled,game_scheduled in enumerate(games_scheduled):
                        if n in game_scheduled:
                            round_scheduled = max(round_scheduled, index_game_scheduled // self._num_courts)
#                if 31 in game and round_scheduled < 1:
#                    round_scheduled = 0

                round_scheduled_list.append( round_scheduled )
                index_game_list.append( index_game )

            round_scheduled_list, index_game_list = zip(*sorted(zip(round_scheduled_list, index_game_list)))
#            print( f"already scheduled {len(games_scheduled)} games:" )
#            print( games_scheduled )
#            print( round_scheduled_list)

#                kmax = len(games_scheduled)//self._num_courts
#                for k in range(kmax - 1):
#                    print(k, games_scheduled[k*self._num_courts:(k+1)*self._num_courts] )
#                print(kmax, games_scheduled[kmax*self._num_courts:] )
#                print( game, round_scheduled )
                
            round_now = len(games_scheduled)//self._num_courts
            index_game_to_add = index_game_list[0]
#            print( games_left[index_game_to_add], " is the game to add, current round is ", round_now)
            if round_scheduled_list[0] < round_now:
                games_scheduled.append( games_left[index_game_to_add] )
                games_left.pop( index_game_to_add )
            else:
                games_scheduled.append( [] )

        maxbyes = 0
        for n in range(self._num_pairs):    
            byes = self.countByes(n, games_scheduled)
            if byes[-1] > maxbyes:
                maxbyes = byes[-1]
        num_rounds = len(games_scheduled)//self._num_courts

        if maxbyes<=2:
            for n in range(self._num_pairs):    
                byes = self.countByes(n, games_scheduled)
                print(n, " -> ", byes )
 
        return maxbyes, num_rounds, games_scheduled


    def saveGameSchedule(self, maxbyes_allowed, maxrounds_allowed):
        maxbyes = 999
        num_rounds = 999
        while maxbyes > maxbyes_allowed or num_rounds > maxrounds_allowed-1:
            maxbyes, num_rounds, games_scheduled  = self.scheduleGames()
    
        with open("valid_game_schedule.json", "w") as f:
            json.dump(games_scheduled, f, indent=4)

        return games_scheduled


    def loadGameSchedule(self, filename):
        with open(filename, "r") as f:
            games_scheduled = json.load(f)
        
        self._games = games_scheduled
        return


    def makeExcel(self):
        from openpyxl import Workbook
        wb = Workbook()
        ws = wb.active

        for i in range(self._num_pairs):
            cellname = chr(66)  + (i+1+30).__str__()
            ws[cellname] = i+1

        game_excel_index_list = []
        counts = np.zeros(self._num_pairs, dtype=int)        
        for index_game,game in enumerate(self._games):
            n = index_game // self._num_courts
            r = index_game % self._num_courts
            cellname = chr(65) + (2*n+2).__str__()
            ws[cellname] = "round " + (n+1).__str__()
            cellname = chr(65+1 + 5*r + 2) + (2*n+2).__str__()
            ws[cellname] = "vs"

            game_excel_index = []
            for index_pair, pair in enumerate(game):
                if index_pair <=1:
                    cellname = chr(65+1 + 5*r + index_pair) + (2*n+2).__str__()
                else:
                    cellname = chr(65+2 + 5*r + index_pair) + (2*n+2).__str__()
                ws[cellname] = pair + 1
                game_excel_index.append( cellname )
            
                cellname = chr(65 + 2 + counts[pair])  + (pair+30+1).__str__()
                if index_pair <=1:
                    ws[cellname] = "=" + chr(65+1 + 5*r) + (2*n+3).__str__()
                else:
                    ws[cellname] = "=" + chr(65+4 + 5*r) + (2*n+3).__str__()
                counts[pair] += 1

            game_excel_index_list.append( game_excel_index )


        


        wb.save('schedule.xlsx')


def main():
    kob = KoB(24, 5, 9)

    kob.makeGames()
    kob.checkGames()
    kob.saveGameSchedule(2, 12)

    kob.loadGameSchedule("valid_game_schedule.json")
    kob.makeExcel()


if __name__=="__main__":
    main()
