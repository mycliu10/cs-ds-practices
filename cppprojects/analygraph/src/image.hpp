#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <vector>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;


class RawImage {
private:
    int cols_;
    int rows_;
    cv::Mat data_;
public:
    RawImage(std::string path) {
        data_ = cv::imread(path, cv::IMREAD_COLOR);
        if( !data_.empty() ) {
            cols_ = data_.cols / 2;
            rows_ = data_.rows;
            cout << "Image loaded from " << path << " with width " << cols_ << " and height "
                << rows_ << " and channel " << data_.channels() << " and type " << endl;
        }
    }

    int getWidth() {
        return cols_;
    }
    int getHeight() {
        return rows_;
    }
    int getData(int r, int c, int left, int rgb) {
        return data_.at<cv::Vec3b>(r,c+left*cols_)[2-rgb];
    }
    int getType() {
        return data_.type();
    }
};

