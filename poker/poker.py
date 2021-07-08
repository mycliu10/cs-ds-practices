import numpy as np
import random

    
    

class Card:
    suit_str_ = {0:"heart", 1:"diamond", 2:"spade", 3:"club"}
    number_str_ = {11:"J", 12:"Q", 13:"K", 1:"A"}
    number_str_.update({n:n.__str__() for n in range(2,11)})

    def __init__(self, number, suit):
        self.number_ = number
        self.suit_ = suit


    def __str__(self):
        return Read(self)


    def Read(self):
        s = " ".join( (self.suit_str_[self.suit_], self.number_str_[self.number_]) )
        
        return s




class CardSet:
    def __init__(self):
        self.cards_ = [Card(n, s) for n in range(1,14) for s in range(4)]
        self.num_cards_ = len(self.cards_)


    def Select(self, n=5):
        self.selected_indexes_ = random.sample(range(self.num_cards_), n)
        self.selected_cards_ = [self.cards_[i] for i in self.selected_indexes_]
        return self.selected_cards_


    def SelectMore(self, n=1):
        selected_more_indexes = random.sample([i for i in range(self.num_cards_) if i not in self.selected_indexes_], n)
        self.selected_indexes_ += selected_more_indexes
        self.selected_cards_ = [self.cards_[i] for i in self.selected_indexes_]
        
        return self.selected_cards_


    def Reselect(self, keep=None):
        if keep:
            num_keep = len(keep)
            num_selected = len(self.selected_indexes_)
            if num_keep==num_selected:
                return self.selected_cards_
            else:
                self.selected_indexes_ = [self.selected_indexes_[i] for i in keep]
                selected_more_indexes = random.sample([i for i in range(self.num_cards_) if i not in self.selected_indexes_], num_selected-num_keep)
                self.selected_indexes_ += selected_more_indexes
                self.selected_cards_ = [self.cards_[i] for i in self.selected_indexes_]
                return self.selected_cards_
        else:
            return Select()



class BlackJack:
    worths_ = {11:10, 12:10, 13:10}
    worths_.update({n:n for n in range(1,11)})

    def __init__(self, num_players=1):
        self.cards_ = CardSet()
        self.num_players_ = num_players
        # last index is for the dealer
        self.hands_ = [[] for n in range(self.num_players_+1)]


    def NewGame(self):
        self.__init__(num_players=self.num_players_)
        return


    def EvaluateOneCard(self, c, points=[0]):
        for i,point in enumerate(points):
            worth = self.worths_[c.number_]
            points[i] += worth
            if worth == 1:
                points.append(point+10)
        return points


    def EvaluateHand(self, hand):
        possible_points = [0]
        for c in hand:
            possible_points = self.EvaluateOneCard(c, possible_points)
        max_point = max(possible_points)
        
        return max_point 

    
    def PlayerMove(self, n, verbose=False):
        if_to_hit = True
        while if_to_hit:
            drawn_cards = self.cards_.SelectMore(1)
            self.hands_[n].append(drawn_cards[-1])
            points = [self.EvaluateHand(hand) for hand in self.hands_]
            print("".join( ("Dealer : ", points[-1].__str__()) ))
            for card in self.hands_[-1]:
                print(card.Read())
            print("".join( ("Player ", n.__str__(), ": ", points[n].__str__()) ))
            for card in self.hands_[n]:
                print(card.Read())
            if points[n] >= 21:
                if_to_hit = False
            else:
                s = input("$ Hit or Stand(s/S)?")
                if s=="s" or s=="S":
                    if_to_hit = False

 
    def Play(self):
        # one card for everybody
        drawn_cards = self.cards_.Select(self.num_players_+1)
        for card,hand in zip(drawn_cards,self.hands_):
            hand.append(card)

        # deal for each player
        for n in range(self.num_players_):
            if_to_hit = True
            while if_to_hit:
                drawn_cards = self.cards_.SelectMore(1)
                self.hands_[n].append(drawn_cards[-1])
                points = [self.EvaluateHand(hand) for hand in self.hands_]
                print("".join( ("Dealer : ", points[-1].__str__()) ))
                for card in self.hands_[-1]:
                    print(card.Read())
                print("".join( ("Player ", n.__str__(), ": ", points[n].__str__()) ))
                for card in self.hands_[n]:
                    print(card.Read())
                if points[n] >= 21:
                    if_to_hit = False
                else:
                    s = input("$ Hit or Stand(s/S)?")
                    if s=="s" or s=="S":
                        if_to_hit = False
        # dealer move
        if_to_hit = True
        while if_to_hit:
            drawn_cards = self.cards_.SelectMore(1)
            self.hands_[-1].append(drawn_cards[-1])
            points[-1] = self.EvaluateHand(self.hands_[-1])
            if points[-1] >= 17:
                if_to_hit = False
         
        print("".join( ("Dealer : ", points[-1].__str__()) ))
        for card in self.hands_[-1]:
            print(card.Read())
 
        
    



class Poker:

    def __init__(self):
        self.cards_ = CardSet()
        #
        #   1 jacks or better
        #   2   two pairs
        #   3   three of a kind
        #   4   straight
        #   5   flush
        #   6   full house
        #   7   four of a kind
        #   8   straight flush
        #   9   royal flush
        self.reward_names_ = {
            0: "none",
            1: "jacks or better",
            2: "two pairs",
            3: "three of a kind",
            4: "straight",
            5: "flush",
            6: "full house",
            7: "four of a kind",
            8: "straight flush",
            9: "royal flush",
        }
        self.rewards_ = {0:0, 1:1, 2:2, 3:3, 4:4, 5:6, 6:9, 7:25, 8:50, 9:250}
        
        print("-------------------------------------")
        print("| Reward Table: cost 1 to play once |")
        for key,value in self.rewards_.items():
            print("|    {:<16} =     {:>4}    |".format(self.reward_names_[key], value.__str__()))
        print("-------------------------------------")



    def Evaluate(self, hand):
        number = {}
        suit = {}
        for c in hand:
            if c.number_+1 in number.keys():
                number[c.number_+1] += 1
            else:
                number[c.number_+1] = 1
            if c.suit_ in suit.keys():
                suit[c.suit_] += 1
            else:
                suit[c.suit_] = 1

        # none 
        combo = 0

        # flush
        if len(suit) == 1:
            combo = 5

        num_2 = 0
        num_3 = 0
        num_4 = 0
        is_job = 0
        for key,value in number.items():
            if value >= 4:
                num_4 += 1
            elif value >= 3:
                num_3 += 1
            elif value >= 2:
                num_2 += 1
                if key >= 11:
                    is_job += 1
        # four of a kind
        if num_4 >= 1:
            combo = 7
        elif num_3 >= 1:
            # full house
            if num_2 >= 1:
                combo = 6
            # three of a kind
            else:
                combo = 3
        elif num_2 >= 2:
            # two pairs
            combo = 2
        elif num_2 >= 1:
            # job
            if is_job >= 1:
                combo = 1
        else:
            maxn = max(number.keys())
            minn = min(number.keys())
            if (maxn-minn) == 4:
                if combo == 5:
                    # royal flush
                    if maxn == 12:
                        combo = 9
                    # straigh flush
                    else:
                        combo = 8
                # straight
                else:
                    combo = 4

        return combo, self.rewards_[combo]-1, self.reward_names_[combo]


            


        print(number, suit)


    def Draw(self):
        hand = self.cards_.Select()
        reward = self.Evaluate(hand)
        return reward

    def Play(self):
        hand = self.cards_.Select()
        s = "    ".join( (c.Read() for c in hand) )
        print(s)
        reward = self.Evaluate(hand)
        print(reward[2], reward[1])

        keep = input("Which are you keeping? (0, 1, 2, 3, 4) ")
        keep = [int(x) for x in keep.split(" ")]
        hand = self.cards_.Reselect(keep)
        s = "    ".join( (c.Read() for c in hand) )
        print(s)

        reward = self.Evaluate(hand)
        print(reward[2], reward[1])
        print("-------------------------------------")


class Scoreboard:
    def __init__(self, f, expect):
        self.f_ = f
        self.counts_ = {key:0 for key in expect.keys()}
        self.names_  = expect.copy()
        self.total_count_ = 0
        self.gain_ = 0

    def Run(self):
        while True:
            r = self.f_()
            self.counts_[r[0]] += 1
            self.total_count_ += 1
            self.gain_ += r[1]

            if self.total_count_ % 2598960 == 0:
                print("Total Count = ", self.total_count_, " gain = ", self.gain_)
                for key,value in self.counts_.items():
                    print("{:<16} {} {}".format(self.names_[key], value, value/self.total_count_))


game = BlackJack()
#scoreboard = Scoreboard(game.Draw, game.reward_names_)
#scoreboard.Run()
#while True:
game.Play()

