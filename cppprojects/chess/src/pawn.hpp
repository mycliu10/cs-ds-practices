#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <string>

using namespace std;

class Pawn {
private:
    bool is_white_;
    int r_;
    int c_;
    bool is_dead_;
    
public:
    Pawn(int is_white, int r, int c) {
        is_white_ = is_white;
        r_ = r;
        c_ = c;
        is_dead_ = false;
    }

    bool availableCaptures(vector<Pawn*>& board, vector<pair<int,int>>& moves) {
        if(is_dead_) {
            return false;
        }
        bool have_to_capture = false;
        int r_next = is_white_? r_+1 : r_-1;
        int n = r_ * 8 + c_;
        for(int c_next : {c_-1, c_+1}) {
            int n_next = r_next*8+c_next;
            if(c_next>=0 && c_next<8 && board[n_next]!=NULL && board[n_next]->is_white_!=is_white_) {
                moves.push_back( {n,n_next} );
                have_to_capture = true;
            }
        }
        return have_to_capture;
    }

    void availableMoves(vector<Pawn*>& board, vector<pair<int,int>>& moves) {
        if(is_dead_) {
            return;
        }
        int r_next = is_white_? r_+1 : r_-1;
        int n = r_ * 8 + c_;
        int c_next = c_;
        int n_next = r_next*8+c_next;
        if(board[n_next]==NULL) {
            moves.push_back( {n,n_next} );
        }
    }

    string getCoords() {
        string s = to_string(r_+1) + string(1,char('a'+c_));
        return s;
    }

    bool updateCoords(int n) {
        r_ = n / 8;
        c_ = n % 8;
        if(r_==0 || r_==7) {
            return true;
        }
        return false;
    }

    void die() {
        is_dead_ = true;
    }

    string getColor() {
        string s;
        if(is_white_) {
            s = "W";
        }
        else {
            s = "B";
        }
        return s;
    }

};
