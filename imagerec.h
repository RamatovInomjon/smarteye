#ifndef IMAGEREC_H
#define IMAGEREC_H

#include <QCoreApplication>
#include <QDateTime>
#include <fstream>
#include <opencv2/opencv.hpp>
// Namespaces.
using namespace std;
using namespace cv;
using namespace cv::dnn;

// Constants.
const float INPUT_WIDTH = 640.0;
const float INPUT_HEIGHT = 640.0;
const float SCORE_THRESHOLD = 0.3;
const float NMS_THRESHOLD = 0.45;
const float CONFIDENCE_THRESHOLD = 0.4;

// Text parameters.
const float FONT_SCALE = 0.7;
const int FONT_FACE = FONT_HERSHEY_SIMPLEX;
const int THICKNESS = 1;

class ImageRec {
public:
  ImageRec();
  void draw_label(Mat &input_image, string label, int left, int top);
  vector<Mat> pre_process(Mat &input_image, Net &net);
  Mat post_process(Mat input_image, vector<Mat> &outputs,
                   const vector<string> &class_name);
  bool calc(Mat &frame);

private:
  // Colors.
  Net net;
  vector<string> class_list;
  Scalar BLACK = Scalar(0, 0, 0);
  Scalar BLUE = Scalar(255, 178, 50);
  Scalar YELLOW = Scalar(0, 255, 255);
  Scalar RED = Scalar(0, 0, 255);
};

#endif // IMAGEREC_H
