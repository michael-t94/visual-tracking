#include <iostream>
#include "opencv2/core/core.hpp" // data structures and arithmetic routines
#include "opencv2/videoio/videoio.hpp" // video I/O

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
    if (argc != 6) {
        cerr << "Usage: " << argv[0] << " filepath x y width height" << endl;
        return -1;
    }

    int x = stoi(argv[2]), y = stoi(argv[3]), dwidth = stoi(argv[4]), dheight = stoi(argv[5]);

    // open video file
    VideoCapture cap(argv[1]);
    if (!cap.isOpened()) {
        cerr << "Error: can't open " << argv[1] << endl;
        return -1;
    }

    // get the fps
    double fps = cap.get(CAP_PROP_FPS);
    // get the size of the video frame
    int width = cap.get(CAP_PROP_FRAME_WIDTH), height = cap.get(CAP_PROP_FRAME_HEIGHT);

    cout << "FPS: " << fps
         << "\nSize: " << width << "x" << height << endl;

    Mat src;
    Rect cropRegion(x, y, dwidth, dheight);

    // create a video writer
    VideoWriter out;
    out.open("../files/videos/output.mp4", VideoWriter::fourcc('H', '2', '6', '4'), fps, Size(dwidth, dheight));

    if (!out.isOpened()) {
        cerr << "Error: can't open file to write" << endl;
        return -1;
    }

    // write frames
    while (cap.read(src)) {
        Mat dst(src, cropRegion);
        out.write(dst);
    }
    cout << "DONE" << endl;

    return 0;
}
