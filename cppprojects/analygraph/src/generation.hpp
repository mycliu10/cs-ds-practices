#include "image.hpp"

class ChannelOperation {
protected:
    std::map<int,double> coefs_;
public:
    ChannelOperation() {}
    // left: 0, left image; 1, right image
    // rgb: 0, R; 1, G; 2 B
    virtual int compute(RawImage* ri, int r, int c, int left, int rgb) {
        double ans = 0.;
        for(auto coef : coefs_) {
            ans += double(ri->getData(r,c,left,coef.first)) * coef.second;
        }
        return int(ans);
    }
};
class TrueOperation : public ChannelOperation {
public:
    TrueOperation() {
        coefs_ = {{0,.299}, {1,.587}, {2,.114}};
    }
};
class ColorOperation : public ChannelOperation {
public:
    ColorOperation() {}
    int compute(RawImage* ri, int r, int c, int left, int rgb) {
        return ri->getData(r,c,left,rgb);
    }
};



class AnalygraphGeneration {
private:
    RawImage* input_image_;
    cv::Mat output_image_;
public:
    AnalygraphGeneration(RawImage* ri) {
        input_image_ = ri;
        output_image_ = cv::Mat::zeros(input_image_->getHeight(),input_image_->getWidth(), input_image_->getType());
    }

    void makeTrue() {
        TrueOperation to;
        for(int r=0; r<input_image_->getHeight(); ++r) {
            for(int c=0; c<input_image_->getWidth(); ++c) {
                output_image_.at<cv::Vec3b>(r,c)[2] = to.compute(input_image_, r,c,0, 0);
                output_image_.at<cv::Vec3b>(r,c)[1] = 0;
                output_image_.at<cv::Vec3b>(r,c)[0] = to.compute(input_image_, r,c,1, 2);
            }
        }
    }

    void writeAnalygraph(std::string path) {
        cv::imwrite(path, output_image_);
    }
};
