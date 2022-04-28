#include "generation.hpp"

int main() {
//    std::string path = "../data/p0.jpeg";
    std::string path = "../data/p1.jpg";

    RawImage raw_image(path);

    AnalygraphGeneration ag(&raw_image);

    ag.makeTrue();

    std::string output_path = "p0.jpg";
    ag.writeAnalygraph(output_path);
}
