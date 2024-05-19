#include <iostream>
#include <vector>
#include <math.h>
#include <opencv2/opencv.hpp>

#include "matrix.h"
#include "matrixFunctions.h"

using namespace std;
using namespace cv;

void rotate(Matrix &cube, double rad1, double rad2, double rad3) {
    // rotation matrix in x-direction
    Matrix R_x(3, 3, new double*[3] {new double[3] {1,            0,            0}, 
                                     new double[3] {0,            cos(rad1),    -1*sin(rad1)}, 
                                     new double[3] {0,            sin(rad1),    cos(rad1)}});

    // rotation matrix in y-direction
    Matrix R_y(3, 3, new double*[3] {new double[3] {cos(rad2),    0,            sin(rad2)}, 
                                     new double[3] {0,            1,            0}, 
                                     new double[3] {-1*sin(rad2), 0,            cos(rad2)}});

    // rotation matrix in z-direction
    Matrix R_z(3, 3, new double*[3] {new double[3] {cos(rad3),    -1*sin(rad3), 0}, 
                                     new double[3] {sin(rad3),    cos(rad3),    0}, 
                                     new double[3] {0,            0,            1}});

    cube = R_z * R_y * R_x * cube;
}

void render_ortho(Matrix &cube, VideoWriter &video) {
    Mat frame(512, 512, CV_8UC3, Scalar(0, 0, 0)); // CV_8UC3 specifies 3 channels (rgb)

    // for(int i=0; i<8; i++) {
    //     circle(frame, Point(256 + (int)(cube[0][i]), 256 + (int)(cube[1][i])), 2, Scalar(0, 0, 255), 2);
    // }

    vector<Point> points;
    for(int i=0; i<8; i++) {
        points.push_back(Point(256 + (int)(cube[0][i]), 256 + (int)(cube[1][i])));
    }

    line(frame, Point(256 + (int)(cube[0][0]), 256 + (int)(cube[1][0])), 
                Point(256 + (int)(cube[0][1]), 256 + (int)(cube[1][1])), Scalar(255, 255, 255), 1);
    line(frame, Point(256 + (int)(cube[0][0]), 256 + (int)(cube[1][0])), 
                Point(256 + (int)(cube[0][2]), 256 + (int)(cube[1][2])), Scalar(255, 255, 255), 1);
    line(frame, Point(256 + (int)(cube[0][0]), 256 + (int)(cube[1][0])), 
                Point(256 + (int)(cube[0][4]), 256 + (int)(cube[1][4])), Scalar(255, 255, 255), 1);

    line(frame, points[0], points[1], Scalar(255, 255, 255), 1);
    line(frame, points[0], points[2], Scalar(255, 255, 255), 1);
    line(frame, points[0], points[4], Scalar(255, 255, 255), 1);

    line(frame, points[3], points[1], Scalar(255, 255, 255), 1);
    line(frame, points[3], points[2], Scalar(255, 255, 255), 1);
    line(frame, points[3], points[7], Scalar(255, 255, 255), 1);

    line(frame, points[6], points[2], Scalar(255, 255, 255), 1);
    line(frame, points[6], points[4], Scalar(255, 255, 255), 1);
    line(frame, points[6], points[7], Scalar(255, 255, 255), 1);

    line(frame, points[5], points[1], Scalar(255, 255, 255), 1);
    line(frame, points[5], points[4], Scalar(255, 255, 255), 1);
    line(frame, points[5], points[7], Scalar(255, 255, 255), 1);

    video.write(frame);
    waitKey(1); // waits for 1 ms
}

void render_pers(Matrix &cube, VideoWriter &video, double cam_dist, double proj_dist) {
    // x/y coord * (cam z dist - z coord) / (cam z dist - proj plane z dist)

    Mat frame(512, 512, CV_8UC3, Scalar(0, 0, 0));

    vector<Point> points;
    for(int i=0; i<8; i++) {
        points.push_back(Point(256 + (int)(cube[0][i] * ((cam_dist-proj_dist)/(cam_dist - cube[2][i]))),
                               256 + (int)(cube[1][i] * ((cam_dist-proj_dist)/(cam_dist - cube[2][i])))));
    }

    line(frame, points[0], points[1], Scalar(255, 255, 255), 1);
    line(frame, points[0], points[2], Scalar(255, 255, 255), 1);
    line(frame, points[0], points[4], Scalar(255, 255, 255), 1);

    line(frame, points[3], points[1], Scalar(255, 255, 255), 1);
    line(frame, points[3], points[2], Scalar(255, 255, 255), 1);
    line(frame, points[3], points[7], Scalar(255, 255, 255), 1);

    line(frame, points[6], points[2], Scalar(255, 255, 255), 1);
    line(frame, points[6], points[4], Scalar(255, 255, 255), 1);
    line(frame, points[6], points[7], Scalar(255, 255, 255), 1);

    line(frame, points[5], points[1], Scalar(255, 255, 255), 1);
    line(frame, points[5], points[4], Scalar(255, 255, 255), 1);
    line(frame, points[5], points[7], Scalar(255, 255, 255), 1);

    video.write(frame);
    waitKey(1);
}

int main(int argc, char* argv[]) {
    const double PI = atan(1)*4;

    // cube coordinates (in a default 2d array because that's how my matrix library was designed)
    double **cube_arr = new double*[3] {new double[8] {100, -100, 100, -100, 100, -100, 100, -100},
                                        new double[8] {100, 100, -100, -100, 100, 100, -100, -100},
                                        new double[8] {100, 100, 100, 100, -100, -100, -100, -100}};
    Matrix cube(3, 8, cube_arr);
    // cleanup(3, cube_arr);

    string type;
    cout << "Orthographic or perspective projection? (o or p): ";
    cin >> type;

    if(type != "p" && type != "o") {
        cout << "invalid type";
        return 0;
    }

    double rad1, rad2, rad3;
    cout << "Enter x-rotation, y-rotation, and z-rotation (in degress, sep. by spaces): ";
    cin >> rad1 >> rad2 >> rad3;

    // consider allowing the user to input the coords of the camera and plane (and the plane's normal vector).
    // this would make the math more complicated, but lead to an overall more flexible program.
    double cam_dist, proj_dist;
    if(type == "p") {
        cout << "Enter camera's z-distance from origin: ";
        cin >> cam_dist;
        cout << "Enter projection plane's z-distance from origin: ";
        cin >> proj_dist;
    }

    // i dont really know what a, v, c, and 1 mean...
    VideoWriter video("C:/Users/Deccan/OneDrive/Desktop/Projects/Coding/stuff/cube/out/cube.mp4",
                cv::VideoWriter::fourcc('a','v','c','1'), 24, Size(512, 512));

    cout << "Running...\n";
    for(int i=0; i<360; i++) {
        type == "o" ? render_ortho(cube, video) : render_pers(cube, video, cam_dist, proj_dist);
        rotate(cube, rad1*(PI/180), rad2*(PI/180), rad3*(PI/180));
    }

    video.release();
    cout << "Done!!" << endl;
    return 0;
}