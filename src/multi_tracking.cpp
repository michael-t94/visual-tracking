#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
    string trackerTypes[] = {"BOOSTING", "MIL", "KCF", "TLD", "MEDIANFLOW", "MOSSE", "CSRT"}; 
    string trackerType = argv[1];

    // open video file
    VideoCapture cap(argv[2]);
    if (!cap.isOpened()) {
        cerr << "Error: can't open video file" << endl;
        return -1;
    }

    // read first frame
    Mat frame;
    cap.read(frame);

    // select initial bounding box(es)
    vector<Rect> ROIs;
    selectROIs("Multi-Tracking", frame, ROIs);

    //quit when the tracked object(s) is not provided
    if (ROIs.size() < 1) return 0;

    // container of the tracked objects
    vector<Rect2d> objects;

    // create a set of trackers
    vector<Ptr<Tracker>> trackers;
    for (size_t i = 0; i < ROIs.size(); i++)
    {
        Ptr<Tracker> tracker;
        if (trackerType == "BOOSTING") tracker = TrackerBoosting::create();
        if (trackerType == "MIL") tracker = TrackerMIL::create();
        if (trackerType == "KCF") tracker = TrackerKCF::create();
        if (trackerType == "TLD") tracker = TrackerTLD::create();
        if (trackerType == "MEDIANFLOW") tracker = TrackerMedianFlow::create();
        if (trackerType == "MOSSE") tracker = TrackerMOSSE::create();
        if (trackerType == "CSRT") tracker = TrackerCSRT::create();

        trackers.push_back(tracker);
        objects.push_back(ROIs[i]);
    }

    // display bounding box(es)
    for (size_t i = 0; i < objects.size(); i++) {
        rectangle(frame, objects[i], Scalar(255, 0, 0), 2, 1);
    }
    imshow("Multi-Tracking", frame);

    // create the multi-tracker
    MultiTracker multiTracker;
    // initialize the multi-tracker
    multiTracker.add(trackers, frame, objects);

    // do the tracking
    cout << "Start the tracking process, press ESC to quit." << endl;
    while (cap.read(frame)) {
        double timer = (double)getTickCount();
        // update the tracking result
        bool ok = multiTracker.update(frame);

        double fps = cvGetTickFrequency() / ((double)getTickCount() - timer) * 1000000;

        if (ok) {
            // draw the tracked object
            for(size_t i = 0; i < multiTracker.getObjects().size(); i++) {
                rectangle(frame, multiTracker.getObjects()[i], Scalar(255, 0, 0), 2, 1);
            }
        }
        else {
            putText(frame, "Tracking failure detected", Point(1832, 291), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 200), 2);
        }

        putText(frame, trackerType + " Tracker", Point(1832, 191), FONT_HERSHEY_SIMPLEX, 1, Scalar(50, 255, 255), 2);
        putText(frame, "FPS: " + to_string(fps), Point(1832, 241), FONT_HERSHEY_SIMPLEX, 1, Scalar(50, 255, 255), 2);

        // show image with the tracked object
        imshow("Multi-Tracking", frame);

        // quit on ESC button
        if (waitKey(1) == 27) break;
    }
}
