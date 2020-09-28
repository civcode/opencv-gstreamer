/*
* Receiver.cpp: Receiver for OpenCV_GStreamer example
*
* Copyright (C) 2019 Simon D. Levy
*
* MIT License
*/

#include "pch.h"
#include <gflags/gflags.h>

#include <opencv2/opencv.hpp>
using namespace cv;

#include <iostream>
using namespace std;

const string k_host_name = "nano.local";
const uint k_host_port = 7000;
const uint k_host_fps = 60;

// command line parameters
DEFINE_string(host, "xavier.local", "Hostname or IP of the transmitter.");
DEFINE_int32(port, 7000, "TCP port for transmission.");

int main(int argc, char **argv)
{
    // The sink caps for the 'rtpjpegdepay' need to match the src caps of the 'rtpjpegpay' of the sender pipeline
    // Added 'videoconvert' at the end to convert the images into proper format for appsink, without
    // 'videoconvert' the receiver will not read the frames, even though 'videoconvert' is not present
    // in the original working pipeline
	//VideoCapture cap("udpsrc port=5000 ! application/x-rtp,media=video,payload=26,clock-rate=90000,encoding-name=JPEG,framerate=30/1 ! rtpjpegdepay ! jpegdec ! videoconvert ! appsink", 
 //           CAP_GSTREAMER);

    gflags::ParseCommandLineFlags(&argc, &argv, true);

    cout << "Connection: " << FLAGS_host << ":" << FLAGS_port << endl;

    ostringstream gst_pipeline;

    //gst_pipeline << "tcpclientsrc host=" << k_host_name << " port=" << k_host_port << " ! gdpdepay ! rtph265depay ! avdec_h265 ! videoconvert ! appsink";
    gst_pipeline << "tcpclientsrc host=" << FLAGS_host << " port=" << FLAGS_port << " ! gdpdepay ! rtph265depay ! avdec_h265 ! nvvidconv ! appsink";

	VideoCapture cap(gst_pipeline.str(), CAP_GSTREAMER);
    
	if (!cap.isOpened()) {
        cerr << "VideoCapture not opened" << endl;
        exit(-1);
    }

    uint cnt = 0;    
    while (int key = cv::waitKey(1) != 'q') {

        Mat frame;

        cap.read(frame);

        //if (cnt++ % 10 == 0)
        imshow("receiver", frame);
        //cout << "received frame " << cnt++ << endl;

        if (key == 27) {
            break;
        }
    }

    return 0;
}
