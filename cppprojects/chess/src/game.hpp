#include "pawn.hpp"

class Game {
private:
    vector<Pawn*> board_;
    bool is_white_turn_;
    unordered_set<Pawn*> whites_;
    unordered_set<Pawn*> blacks_;
    string winner_;

public:
    bool is_over_;

    Game() {
        board_.resize(8*8, NULL);
        for(int c = 0; c < 8; ++c) {
            board_[1*8 + c] = new Pawn(true, 1, c);
            whites_.insert(board_[1*8 + c]);
            board_[6*8 + c] = new Pawn(false, 6, c);
            blacks_.insert(board_[6*8 + c]);
        }
        is_white_turn_ = true;
        is_over_ = false;
        cout << " xxxxxxxxx game start xxxxxxxxx " << endl;
        cout << " xxxxxx B = black piece xxxxxxx " << endl;
        cout << " xxxxxx W = white piece xxxxxxx " << endl;
    }
    ~Game() {
        for(auto p : whites_) {
            delete p;
        }
        for(auto p : blacks_) {
            delete p;
        }
    }

    void makeTurn() {
        unordered_set<Pawn*> moved;
        bool have_to_capture = true;
        while(have_to_capture) {
            have_to_capture = makeSimpleTurn(moved);
        }
        is_white_turn_ = !is_white_turn_;
    }

    bool makeSimpleTurn(unordered_set<Pawn*>& moved) {
        vector<pair<int,int>> moves;
        unordered_set<Pawn*>* pawns = is_white_turn_? &whites_ : & blacks_;
        // get available captures
        bool have_to_capture = false;
        for(auto p : *pawns) {
            if(moved.find(p) == moved.end()) {
                have_to_capture = p->availableCaptures(board_, moves) || have_to_capture;
            }
        }
        // get available non-captures
        if(!have_to_capture) {
            for(auto p : *pawns) {
                if(moved.find(p) == moved.end()) {
                    p->availableMoves(board_, moves);
                }
            }
        }
        
        // victory condition (no moves)
        if(moves.size()==0) {
            winner_ = is_white_turn_? "White" : "Black";
            winner_ += " lost for running out of moves!";
            is_over_ = true;
            return false;
        }
        
        // output for user 
        cout << "+++++++++++++++++++++++++++++++++++" << endl;
        is_white_turn_? cout << "white" : cout << "black";
        cout << " has " << moves.size() << " available moves." << endl;
        for(auto p : moved) {
            cout << p->getCoords() << " ";
        }
        if(moved.size()>0) {
            cout << "already moved in this turn by captueing. Getting a free move." << endl;
        }
        int counter = 0;
        for(auto m : moves) {
            int r = m.first / 8;
            int c = m.first % 8;
            cout << "option " << counter++ << ": " << (r+1) << char(c+'a') << " -> ";
            r = m.second / 8;
            c = m.second % 8;
            cout << (r+1) << char(c+'a') << endl;
        }
        printBoard();

        // input from user
        int option;
        bool success_move = false;
        while(!success_move) {
            cout << "Choose your option (by number)..." << endl;
            cin >> option;
            if(option >= 0 && option < moves.size()) {
                success_move = true;
            }
        }

        // make a move
        if(board_[moves[option].second] != NULL) {
            board_[moves[option].second]->die();
        }
        board_[moves[option].second] = board_[moves[option].first];
        bool is_last_block = board_[moves[option].second]->updateCoords(moves[option].second);
        moved.insert(board_[moves[option].second]);
        board_[moves[option].first] = NULL;

        // victory condition (last block)
        if(is_last_block) {
            winner_ = is_white_turn_? "White" : "Black";
            winner_ += " won for going into the last block!";
            is_over_ = true;
            return false;
        }

        return have_to_capture;
    }

    void printBoard() {
        cout << "-------------------" << endl;
        for(int r=7; r >= 0; --r) {
            cout << r+1 << "|";
            for(int c=0; c < 8; ++c) {
                if(board_[r*8+c]==NULL) {
                    cout << "  ";
                }
                else {
                    cout << board_[r*8+c]->getColor() << " ";
                }
            }
            cout << "|" << endl;
        }
        cout << "  ";
        for(int c=0; c < 8; ++c) {
            cout << char('a'+c) << " ";
        }
        cout << endl;
        cout << "-------------------" << endl;
    }

    void announceWinner() {
        cout << winner_ << endl;
    }

};
