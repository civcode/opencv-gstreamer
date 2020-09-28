/*
* Sender.cpp: Sender for OpenCV_GStreamer example
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

const string k_host_name = "xavier.local";
const uint k_host_port = 7000;
const uint k_host_fps = 60;

// command line parameters
DEFINE_string(host, "xavier.local", "Hostname or IP of the transmitter.");
DEFINE_int32(port, 7000, "TCP port for transmission.");

int main(int argc, char **argv)
{
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    cout << "Connection: " << FLAGS_host << ":" << FLAGS_port << endl;

    VideoCapture cap(0);
	
    if (!cap.isOpened()) {
        cerr << "VideoCapture not opened" << endl;
        exit(-1);
    }

    //char buffer[256];
    //sprintf(buffer, "appsrc ! videoconvert ! video/x-raw,framerate=%d/1,width=640,height=480 ! videoscale ! videoconvert ! clockoverlay ! omxh265enc ! rtph265pay config-interval=1 pt=96 ! gdppay ! tcpserversink host=%s port=7000",
 
    ostringstream gst_pipeling;
    gst_pipeling << "appsrc ! videoconvert ! video/x-raw,framerate=" << k_host_fps << "/1,width=640,height=480 ! videoscale ! videoconvert ! clockoverlay ! omxh265enc ! rtph265pay config-interval=1 pt=96 ! gdppay ! tcpserversink host=" << FLAGS_host << " port=" << FLAGS_port; // << "\"";
 
    cout << "\n" << gst_pipeling.str() << endl;

	VideoWriter writer(
		//"appsrc ! videoconvert ! video/x-raw,format=YUY2,width=640,height=480,framerate=60/1 ! jpegenc ! rtpjpegpay ! udpsink host=127.0.0.1 port=5000", 
        //"appsrc ! videoconvert ! video/x-raw,framerate=60/1,width=640,height=480 ! videoscale ! videoconvert ! clockoverlay ! omxh265enc ! rtph265pay config-interval=1 pt=96 ! gdppay ! tcpserversink host=192.168.0.24 port=7000",
        gst_pipeling.str(),
        0,		// fourcc 
        60,		// fps
        Size(640, 480), 
        true);	// isColor

    if (!writer.isOpened()) {
        cerr << "VideoWriter not opened" << endl;
        exit(-1);
    }

    int cnt = 0;

    while (true) {

        Mat frame;

        cap.read(frame);

        //cout << "frame " << cnt++ << endl;

        writer.write(frame);
    }

    return 0;
}


