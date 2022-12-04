#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <vector>

using namespace std;

class Game {
public:
    string winner_;
    string loser_;
    int winby_;

    Game(string a, int m, string b, int n) {
        winner_ = a;
        loser_ = b;
        winby_ = m - n;
    }
};


class BeatGraph {
public:
    unordered_map<string, vector<string>> graph_;
    BeatGraph() {

    }

    void add(Game game) {
        graph_[game.winner_].push_back(game.loser_);
    }

    void print() {
        for(auto& g : graph_) {
            cout << g.first << " : ";
            for(auto& l : g.second) {
                cout << l;
            }
            cout << endl;
        }
    }

    

    void findCompetitiveAgainst(string s, unordered_map<string,int>& visited) {
        if(visited[s] > 1) {
            return;
        }
        visited[s] += 1;

        for(auto& next : graph_[s]) {
            findCompetitiveAgainst(next, visited);
        }
        
    }
};

class Scoreboard {
public:
    vector<Game> games_;
    unordered_map<string, pair<int,int>> scores_;
    BeatGraph bg_;

    Scoreboard(string filename) {
        int m,n;
        string a,b;

        ifstream infile(filename);
        while(infile >> a >> m >> b >> n) {
            Game game(a, m, b, n);
            games_.push_back(game);
            if( scores_.find(a) == scores_.end() ) {
                scores_[a] = {game.winby_, 1};
            }
            else {
                scores_[a].first += game.winby_;
                scores_[a].second += 1;
            }
        }
    }

    void getMargins() {
        string best_team;
        double largest_margin = 0.;
        for(auto s : scores_) {
            double margin = double(s.second.first) / double(s.second.second);
            if(margin > largest_margin) {
                best_team = s.first;
                largest_margin = margin;
            }
        }
        cout << best_team << " with margin " << largest_margin << endl;
    }

    void getDominates() {
        // O(n)
        for(auto& g : games_) {
            bg_.add(g);
        }
        bg_.print();
        cout << endl;

        unordered_map<string, unordered_set<string>> beats;

        // O(n * n)
        for(auto& t : scores_) {
            unordered_map<string,int> visited;
            bg_.findCompetitiveAgainst(t.first, visited);
            cout << t.first << " : ";
            for(auto l : visited) {
                cout << l.first;
                beats[t.first].insert(l.first);
            }
            cout << endl;
        }

        // O(n*n)
        for(auto& winner : beats) {
            cout << winner.first << " : " ;
            for(auto& loser : winner.second) {
                if(beats[loser].find(winner.first) == beats[loser].end()) {
                    cout << loser;
                }
            }
            cout << endl;
        }


    }
};

int main() {

    Scoreboard sb("../data/sports.txt");
    sb.getMargins();
    sb.getDominates();
    


}

