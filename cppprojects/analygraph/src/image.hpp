#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <vector>
#include <unordered_set>
#include <map>
using std::cout;
using std::endl;


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


    int calculateLeft(std::vector<double>& coefs, int r, int c) {
        double ans = 0.;
        for(int i=0; i<coefs.size(); ++i) {
            ans += double(data_.at<cv::Vec3b>(r,c)[2-i]) * coefs[i];
//            cout << double(data_.at<cv::Vec3b>(r,c)[i]) << " " << ans << endl;
        }
        return int(ans);
    }



};

