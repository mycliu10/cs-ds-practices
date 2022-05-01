#include "image.hpp"

class ChannelOperation {
protected:
    std::map<int,double> coefs_;
    std::vector<std::map<int,double>> multi_coefs_;
public:
    ChannelOperation() {
        coefs_ = {{0,.299}, {1,.587}, {2,.114}};
    }
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
private:
    std::vector<std::vector<int>> channel_image_;
    std::vector<int> counters_;
public:
    TrueOperation() {
        coefs_ = {{0,.299}, {1,.587}, {2,.114}};
        channel_image_.resize(2);
        counters_.resize(2);
    }
    int compute(RawImage* ri, int r, int c, int left, int rgb) {
        if(channel_image_[left].size() > 0 && counters_[left]==ri->getWidth()*ri->getHeight()) {
            return channel_image_[left][r*ri->getWidth() + c];
        }

        channel_image_[left].resize(ri->getWidth()*ri->getHeight(), 0);

        int ans = ChannelOperation::compute(ri,r,c,left,rgb);
   
        channel_image_[left][r*ri->getWidth() + c] = ans;
        ++counters_[left];

        return ans;
    }
};
class OptimizedOperation : public ChannelOperation {
public:
    OptimizedOperation() {
        coefs_ = { {1,.7}, {2,.3}};
    }
};
class ColorOperation : public ChannelOperation {
public:
    ColorOperation() {}
    int compute(RawImage* ri, int r, int c, int left, int rgb) {
        return ri->getData(r,c,left,rgb);
    }
};
class ZeroOperation : public ChannelOperation {
public:
    ZeroOperation() {}
    int compute(RawImage* ri, int r, int c, int left, int rgb) {
        return 0;
    }
};
class MultiChannelOperation : public ChannelOperation {
public:
    MultiChannelOperation() {}
    virtual int compute(RawImage* ri, int r, int c, int left, int rgb) {
        double ans = 0.;
        for(auto& coef : multi_coefs_[3*left+rgb]) {
            ans += double(ri->getData(r,c,left, coef.first)) * coef.second;
        }
        if(ans < 0.) {
            return 0;
        }
        if(ans > 255.) {
            return 255;
        }
        return int(ans);
    }
};
class DuBoisOperation : public MultiChannelOperation {
public:
    DuBoisOperation() {
        multi_coefs_ = { {{0, .437}, {1, .449}, {2, .164}},
                         {{0, -0.062}, {1, -0.062}, {2, -0.024}}, 
                         {{0, -0.048}, {1, -0.050}, {2, -0.017}},
                         {{0, -0.011}, {1, -0.032}, {2, -0.007}},
                         {{0, 0.377}, {1, 0.761}, {2, 0.009}},
                         {{0, -0.026}, {1, -0.093}, {2, 1.234}},
                         };
    }
};
class RoscoluxOperation : public MultiChannelOperation {
public:
    RoscoluxOperation() {
        multi_coefs_ = { {{0, 0.3185}, {1, 0.0769}, {2, 0.0109}},
                         {{0, 0.1501}, {1, 0.0767}, {2, 0.0056}}, 
                         {{0, 0.0007}, {1, 0.0020}, {2, 0.0156}},
                         {{0, -0.011}, {1, -0.032}, {2, -0.007}},
                         {{0, 0.0174}, {1, 0.0484}, {2, 0.1402}},
                         {{0, 0.0184}, {1, 0.1807}, {2, 0.0458}},
                         {{0, 0.0286}, {1, 0.0991}, {2, 0.7662}},
                         };
    }
};

class AnalygraphGeneration {
private:
    RawImage* input_image_;
    cv::Mat output_image_;

    TrueOperation t_op_;
    OptimizedOperation o_op_;
    ColorOperation c_op_;
    ZeroOperation z_op_;
    DuBoisOperation db_op_;
    RoscoluxOperation r_op_;

    std::vector<ChannelOperation*> left_image_operation_;
    std::vector<ChannelOperation*> right_image_operation_;

    void make() {
        for(int r=0; r<input_image_->getHeight(); ++r) {
            for(int c=0; c<input_image_->getWidth(); ++c) {
                for(int rgb = 0; rgb < 2; ++rgb) {
                    output_image_.at<cv::Vec3b>(r,c)[2-rgb] = left_image_operation_[rgb]->compute(input_image_, r,c,0, rgb)
                                                            + right_image_operation_[rgb]->compute(input_image_, r,c,1, rgb);
                }
            }
        }
    }

public:
    AnalygraphGeneration(RawImage* ri) {
        input_image_ = ri;
        output_image_ = cv::Mat::zeros(input_image_->getHeight(),input_image_->getWidth(), input_image_->getType());
    }

    void makeTrue() {
        left_image_operation_ = {&t_op_, &z_op_, &z_op_};
        right_image_operation_ = {&z_op_, &z_op_, &t_op_};
        make();
    }

    void makeGray() {
        left_image_operation_ = {&t_op_, &z_op_, &z_op_};
        right_image_operation_ = {&z_op_, &t_op_, &t_op_};
        make();
    }

    void makeColor() {
        left_image_operation_ = {&c_op_, &z_op_, &z_op_};
        right_image_operation_ = {&z_op_, &c_op_, &c_op_};
        make();
    }

    void makeHalfColor() {
        left_image_operation_ = {&t_op_, &z_op_, &z_op_};
        right_image_operation_ = {&z_op_, &c_op_, &c_op_};
        make();
    }

    void makeOptimized() {
        left_image_operation_ = {&o_op_, &z_op_, &z_op_};
        right_image_operation_ = {&z_op_, &c_op_, &c_op_};
        make();
    }

    void makeDuBois() {
        left_image_operation_ = {&db_op_, &db_op_, &db_op_};
        right_image_operation_ = {&db_op_, &db_op_, &db_op_};
        make();
    }

    void makeRoscolux() {
        left_image_operation_ = {&r_op_, &r_op_, &r_op_};
        right_image_operation_ = {&r_op_, &r_op_, &r_op_};
        make();
    }



    void writeAnalygraph(std::string path) {
        cv::imwrite(path, output_image_);
    }
};
