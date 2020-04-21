#include <opencv2/core.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void DFT1(Mat& input, Mat& mag, Mat& phase) {
    Mat padded;
    int m = getOptimalDFTSize(input.rows);
    int n = getOptimalDFTSize(input.cols);
    copyMakeBorder(input, padded, 0, m - input.rows, n - input.cols, BORDER_CONSTANT, 0);
    Mat planes[] = { Mat_<float>(padded),Mat::zeros(padded.size(),CV_32F) };
    Mat complexI;
    cv::merge(planes, 2, complexI);
    dft(complexI, complexI);
    split(complexI, planes);
    mag = planes[0](Rect(0, 0, input.cols, input.rows));
    phase = planes[1](Rect(0, 0, input.cols, input.rows));
    cv::log(1.0 + mag, mag);
    normalize(mag, mag, 0, 1, NORM_MINMAX);
    //normalize(phase, phase, 0, 1, NORM_MINMAX);
}
int main(int argc, char* argv[]) {
    Mat im1 = imread("D://A1.jpg", 0);
    VideoCapture cap("C://A.mp4",0);
    if (!cap.isOpened())
    {
        cout << "Loi khi doc file " << argv[1];
        return -1;
    }
    while (1)
    {
        Mat frame, mag, phase, frame_gray;
        cap >> frame;
        if (frame.empty())
            break;
        cvtColor(frame, frame_gray, COLOR_RGB2GRAY);
        imshow("Frame doc duoc ", frame);
        DFT1(frame_gray, mag, phase);
        imshow("Magnitude", mag);
        imshow("Phase", phase);
        char c = (char)waitKey(25);
        if (c == 27)
            break;
    }
    cap.release();
    destroyAllWindows();
    return 0;
}