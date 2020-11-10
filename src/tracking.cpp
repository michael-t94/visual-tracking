#include "opencv2/videoio/videoio.hpp"
#include "opencv2/tracking/tracking.hpp"
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
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

    // open video file
    VideoCapture cap(argv[2]);
    if (!cap.isOpened()) {
        cerr << "Error: can't open video file" << endl;
        return -1;
    }

    // read first frame
    Mat frame;
    cap.read(frame);

    // select initial bounding box
    // Rect2d bbox(1699, 277, 30, 64); // player 2
    // Rect2d bbox(1955, 1000, 83, 166); // player 1
    Rect2d bbox(1270, 702, 35, 100); // player 3
    // Rect2d bbox = selectROI(frame, false);

    // display bounding box
    rectangle(frame, bbox, Scalar(255, 0, 0), 2, 1);
    // imshow("Tracking", frame);
    // waitKey(10000);

    int index = 1; // frame number
    string filename = trackerType + "frame00" + to_string(index) + ".jpg";
    imwrite(filename, frame);
    cout << "writing " << filename << endl;
    index++;

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
        // cout << "fps: " << fps << endl;
        sum += fps;
        count++;

        if (ok) {
            // draw the tracked object
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

    cout << "Average FPS: " << sum / count << endl;
    return 0;
}
