#include "generation.hpp"

int main() {
//    std::string path = "../data/p0.jpeg";
    std::string path = "../data/p1.jpg";
    size_t last_dot = path.find_last_of(".");
    std::string filename = path.substr(0, last_dot);

    RawImage raw_image(path);
    AnalygraphGeneration ag(&raw_image);

    std::unordered_map<string,int> choice_table = {
        {"exit", -1},
        {"true", 1},
        {"gray", 2},
        {"color", 3},
        {"halfcolor", 4},
        {"optimized", 5},
        {"dubois", 6},
        {"roscolux", 7},
    };

    int choice = 0;
    while(1) {
        cout << "What is choice of analygraph generation method?" << endl;
        for(auto& c : choice_table) {
            cout << c.first << endl;
        }
        string user_input;
        cin >> user_input;
        auto it = choice_table.find(user_input);
        if(it == choice_table.end()) {
            continue;
        }
        else {
            choice = it->second;
        }
        if(choice < 0) {
            break;
        }
        switch(choice) {
            case(1):
                ag.makeColor();
                break;
            case(2):
                ag.makeGray();
                break;
            case(3):
                ag.makeColor();
                break;
            case(4):
                ag.makeHalfColor();
                break;
            case(5):
                ag.makeOptimized();
                break;
            case(6):
                ag.makeDuBois();
                break;
            case(7):
                ag.makeRoscolux();
                break;
        }
        string output_filename = filename + "_" + user_input + ".jpg";
        cout << "Generating " << output_filename << " ..." << endl;
        ag.writeAnalygraph(output_filename);
    }

}
