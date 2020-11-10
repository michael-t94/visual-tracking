#include "opencv2/videoio/videoio.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/tracking/tracking.hpp"

using namespace cv;
using namespace cv::ml;
using namespace std;

int main(int argc, char **argv)
{
    // open video file
    VideoCapture cap(argv[2]);
    if (!cap.isOpened()) {
        cerr << "Error: can't open " << argv[2] << "." << endl;
        exit(1);
    }

    // create hog descriptor
    Size win_size(64, 128), bsize(16, 16), bstride(8, 8), csize(8, 8);
    HOGDescriptor hog(win_size, bsize, bstride, csize, 9, 1, -1, HOGDescriptor::L2Hys, 0.2, true);
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
    // hog.load(argv[3]);

    // create tracker
    string trackerTypes[] = {"BOOSTING", "MIL", "KCF", "TLD", "MEDIANFLOW", "MOSSE", "CSRT"};
    string trackerType = argv[1];
    Ptr<Tracker> tracker;
    if (trackerType == "BOOSTING") tracker = TrackerBoosting::create();
    if (trackerType == "MIL") tracker = TrackerMIL::create();
    if (trackerType == "KCF") tracker = TrackerKCF::create();
    if (trackerType == "TLD") tracker = TrackerTLD::create();
    if (trackerType == "MEDIANFLOW") tracker = TrackerMedianFlow::create();
    if (trackerType == "MOSSE") tracker = TrackerMOSSE::create();
    if (trackerType == "CSRT") tracker = TrackerCSRT::create();

    // read first frame
    Mat frame;
    cap.read(frame);

    vector<Rect> detections;
    vector<double> foundWeights;

    // detecting in the first frame
    hog.detectMultiScale(frame, detections, foundWeights);

    for (size_t j = 0; j < detections.size(); j++)
    {
        // cout << "Target position: (" << detections[j].x << ", " << detections[j].y << ")" << endl;
        // cout << "Bounding box size: " << detections[j].width << "x" << detections[j].height << endl;
        // Scalar color = Scalar(0, foundWeights[j] * foundWeights[j] * 200, 0);
        // rectangle(frame, detections[j], color, 2, 1);
        rectangle(frame, detections[j], Scalar(255, 0, 0), 2, 1);
    }

    // imshow("Tracking", frame);
    // cout << detections[0].x << " " << detections[0].y << " " << detections[0].width << "x" << detections[0].height << endl;

    int index = 1; // frame number
    string filename = trackerType + "frame00" + to_string(index) + ".jpg";
    imwrite(filename, frame);
    cout << "writing " << filename << endl;
    index++;

    Rect2d bbox(detections[0]);
    // initialize tracker
    tracker->init(frame, bbox);

    double sum = 0;
    int count = 0;
    // start tracking
    while (cap.read(frame)) {
        if (index < 10 ) {
            filename = trackerType + "frame00" + to_string(index) + ".jpg";
        }
        else if (index >= 10 && index < 100) {
            filename = trackerType + "frame0" + to_string(index) + ".jpg";
        }
        else {
            filename = trackerType + "frame" + to_string(index) + ".jpg";
        }
        double timer = (double)getTickCount();
        // update tracking result
        bool ok = tracker->update(frame, bbox);

        double fps = getTickFrequency() / ((double)getTickCount() - timer);
        sum += fps;
        count++;

        if (ok) {
            /* draw the tracked object */
            rectangle(frame, bbox, Scalar(255, 0, 0), 2, 1);
        }
        else {
            putText(frame, "Tracking failure detected", Point(1832, 291), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 200), 2);
        }

        putText(frame, trackerType + " Tracker", Point(1832, 191), FONT_HERSHEY_SIMPLEX, 1, Scalar(50, 255, 255), 2);
        putText(frame, "FPS: " + to_string(fps), Point(1832, 241), FONT_HERSHEY_SIMPLEX, 1, Scalar(50, 255, 255), 2);

        imwrite(filename, frame);
        cout << "writing " << filename << endl;
        index++;
        // imshow("Tracking", frame);
        // int k = waitKey(1);
        // if (k == 27) break;
    }
    // imwrite("../files/demo/daimler_hog64x128gamma.jpg", frame);
    // waitKey();

    cout << "Average FPS: " << sum / count << endl;
    return 0;
}
