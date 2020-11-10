#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs/imgcodecs.hpp"
#include "opencv2/videoio/videoio.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
    VideoCapture cap(argv[1]);
    if (!cap.isOpened()) {
        cerr << "Error: can't open " << argv[1] << endl;
        exit(1);
    }

    Mat frame;
    Mat gray(frame.rows, frame.cols, CV_8UC1);
    int index = 1; // frame number
    string filename;

    while (cap.read(frame)) {
        if (index < 10 ) {
            filename = "frame00" + to_string(index) + ".jpg";
        }
        else if (index >= 10 && index < 100) {
            filename = "frame0" + to_string(index) + ".jpg";
        }
        else {
            filename = "frame" + to_string(index) + ".jpg";
        }
        cvtColor(frame, gray, COLOR_BGR2GRAY, 1);
        imwrite("../files/gray-frames/" + filename, gray);
        cout << "write: " << filename << " to ../files/gray-frames/" << endl;
        index++;
    }

    return 0;
}
