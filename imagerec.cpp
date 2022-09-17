#include "imagerec.h"

ImageRec::ImageRec() {
  ifstream ifs(QCoreApplication::applicationDirPath().toStdString() +
               "/fire.names");
  string line;
  while (getline(ifs, line)) {
    class_list.push_back(line);
  }
  net = readNet(QCoreApplication::applicationDirPath().toStdString() +
                "/models/fire_best.onnx");
  // net.setPreferableBackend(DNN_BACKEND_CUDA);
  // net.setPreferableBackend(DNN_TARGET_CUDA);
}

// Draw the predicted bounding box.
void ImageRec::draw_label(Mat &input_image, string label, int left, int top) {
  // Display the label at the top of the bounding box.
  int baseLine;
  Size label_size =
      getTextSize(label, FONT_FACE, FONT_SCALE, THICKNESS, &baseLine);
  top = max(top, label_size.height);
  // Top left corner.
  Point tlc = Point(left, top);
  // Bottom right corner.
  Point brc =
      Point(left + label_size.width, top + label_size.height + baseLine);
  // Draw black rectangle.
  rectangle(input_image, tlc, brc, BLACK, FILLED);
  // Put the label on the black rectangle.
  putText(input_image, label, Point(left, top + label_size.height), FONT_FACE,
          FONT_SCALE, YELLOW, THICKNESS);
}

vector<Mat> ImageRec::pre_process(Mat &input_image, Net &net) {
  // Convert to blob.
  Mat blob;
  blobFromImage(input_image, blob, 1. / 255., Size(INPUT_WIDTH, INPUT_HEIGHT),
                Scalar(), true, false);
  net.setInput(blob);
  // Forward propagate.
  vector<Mat> outputs;
  net.forward(outputs, net.getUnconnectedOutLayersNames());
  return outputs;
}

Mat ImageRec::post_process(Mat input_image, vector<Mat> &outputs,
                           const vector<string> &class_name) {
  //  // Initialize vectors to hold respective outputs while unwrapping
  //  detections. vector<int> class_ids; vector<float> confidences; vector<Rect>
  //  boxes;

  //  // Resizing factor.
  //  float x_factor = input_image.cols / INPUT_WIDTH;
  //  float y_factor = input_image.rows / INPUT_HEIGHT;

  //  float *data = (float *)outputs[0].data;

  //  const int rows = 25200;
  //  // Iterate through 25200 detections.
  //  for (int i = 0; i < rows; ++i) {
  //    float confidence = data[4];
  //    // Discard bad detections and continue.
  //    if (confidence >= CONFIDENCE_THRESHOLD) {

  //      float *classes_scores = data + 5;
  //      // Create a 1x85 Mat and store class scores of 80 classes.
  //      Mat scores(1, class_name.size(), CV_32FC1, classes_scores);
  //      // Perform minMaxLoc and acquire index of best class score.
  //      Point class_id;
  //      double max_class_score;
  //      minMaxLoc(scores, 0, &max_class_score, 0, &class_id);
  //      // Continue if the class score is above the threshold.
  //      if (max_class_score > SCORE_THRESHOLD) {
  //        // Store class ID and confidence in the pre-defined respective
  //        vectors. confidences.push_back(confidence);
  //        class_ids.push_back(class_id.x);
  //        // Center.
  //        float cx = data[0];
  //        float cy = data[1];
  //        // Box dimension.
  //        float w = data[2];
  //        float h = data[3];
  //        // Bounding box coordinates.
  //        int left = int((cx - 0.5 * w) * x_factor);
  //        int top = int((cy - 0.5 * h) * y_factor);
  //        int width = int(w * x_factor);
  //        int height = int(h * y_factor);
  //        // Store good detections in the boxes vector.
  //        boxes.push_back(Rect(left, top, width, height));
  //      }
  //    }
  //    // Jump to the next column.
  //    data += (class_name.size() + 5);
  //  }

  //  // cout<<"box: "<<boxes.size();
  //  // Perform Non Maximum Suppression and draw predictions.
  //  vector<int> indices;
  //  NMSBoxes(boxes, confidences, SCORE_THRESHOLD, NMS_THRESHOLD, indices);

  //  for (int i = 0; i < indices.size(); i++) {
  //    int idx = indices[i];
  //    Rect box = boxes[idx];

  //    int left = box.x;
  //    int top = box.y;
  //    int width = box.width;
  //    int height = box.height;
  //    // Draw bounding box.
  //    rectangle(input_image, Point(left, top), Point(left + width, top +
  //    height),
  //              BLUE, 3 * THICKNESS);

  //    // Get the label for the class name and its confidence.
  //    string label = format("%.2f", confidences[idx]);
  //    label = class_name[class_ids[idx]] + ":" + label;
  //    qDebug() << QString::fromStdString(label);
  //    // Draw class labels.
  //    draw_label(input_image, label, left, top);
  //  }

  //  //  if (!indices.empty()) {
  //  //    QString path =
  //  //        QString("%1/camerarec/tmp/%2.jpg")
  //  //            .arg(QCoreApplication::applicationDirPath())
  //  // .arg(QDateTime::currentDateTimeUtc().toString("ddMMyyyy_hhmmss"));
  //  //    qDebug() << path;
  //  //    imwrite(path.toStdString(), input_image);
  //  //  }

  return input_image;
}

bool ImageRec::calc(Mat &frame) {
  vector<Mat> detections = pre_process(frame, net);

  //  frame = post_process(frame.clone(), detections, class_list);

  // Initialize vectors to hold respective outputs while unwrapping detections.
  vector<int> class_ids;
  vector<float> confidences;
  vector<Rect> boxes;

  // Resizing factor.
  float x_factor = frame.cols / INPUT_WIDTH;
  float y_factor = frame.rows / INPUT_HEIGHT;

  float *data = (float *)detections[0].data;

  const int rows = 25200;
  // Iterate through 25200 detections.
  for (int i = 0; i < rows; ++i) {
    float confidence = data[4];
    // Discard bad detections and continue.
    if (confidence >= CONFIDENCE_THRESHOLD) {

      float *classes_scores = data + 5;
      // Create a 1x85 Mat and store class scores of 80 classes.
      Mat scores(1, class_list.size(), CV_32FC1, classes_scores);
      // Perform minMaxLoc and acquire index of best class score.
      Point class_id;
      double max_class_score;
      minMaxLoc(scores, 0, &max_class_score, 0, &class_id);
      // Continue if the class score is above the threshold.
      if (max_class_score > SCORE_THRESHOLD) {
        // Store class ID and confidence in the pre-defined respective vectors.
        confidences.push_back(confidence);
        class_ids.push_back(class_id.x);
        // Center.
        float cx = data[0];
        float cy = data[1];
        // Box dimension.
        float w = data[2];
        float h = data[3];
        // Bounding box coordinates.
        int left = int((cx - 0.5 * w) * x_factor);
        int top = int((cy - 0.5 * h) * y_factor);
        int width = int(w * x_factor);
        int height = int(h * y_factor);
        // Store good detections in the boxes vector.
        boxes.push_back(Rect(left, top, width, height));
      }
    }
    // Jump to the next column.
    data += (class_list.size() + 5);
  }

  // cout<<"box: "<<boxes.size();
  // Perform Non Maximum Suppression and draw predictions.
  vector<int> indices;
  NMSBoxes(boxes, confidences, SCORE_THRESHOLD, NMS_THRESHOLD, indices);

  for (int i = 0; i < indices.size(); i++) {
    int idx = indices[i];
    Rect box = boxes[idx];

    int left = box.x;
    int top = box.y;
    int width = box.width;
    int height = box.height;
    // Draw bounding box.
    rectangle(frame, Point(left, top), Point(left + width, top + height), BLUE,
              3 * THICKNESS);

    // Get the label for the class name and its confidence.
    string label = format("%.2f", confidences[idx]);
    label = class_list[class_ids[idx]] + ":" + label;
//    qDebug() << QString::fromStdString(label);
    // Draw class labels.
    draw_label(frame, label, left, top);
  }

  if (!indices.empty())
    return true;
  else
    return false;
  //      QString path =
  //          QString("%1/camerarec/tmp/%2.jpg")
  //              .arg(QCoreApplication::applicationDirPath())
  //              .arg(QDateTime::currentDateTimeUtc().toString("ddMMyyyy_hhmmss"));
  //      qDebug() << path;
  //      imwrite(path.toStdString(), input_image);
}

//  QDateTime stTime = QDateTime::currentDateTime();
//  qDebug() << "Dif Time: " << stTime.msecsTo(QDateTime::currentDateTime());
//  if (detections.empty()) {
//    return false;
//  }
//  return true;
//}

//  vector<double> layersTimes;
//  double freq = getTickFrequency() / 1000;
//  double t = net.getPerfProfile(layersTimes) / freq;
//  string label = format("Inference time : %.2f ms", t);
//  cout << "label: " << label;
//  // putText(img, label, Point(20, 40), FONT_FACE, FONT_SCALE, RED);
//  // imshow("Output", img);
//  // waitKey(0);
