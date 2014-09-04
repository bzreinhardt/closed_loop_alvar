#include "CvTestbed.h"
#include "MarkerDetector.h"
#include "GlutViewer.h"
#include "Shared.h"

#include <fstream>
#include <string>
#include <sys/time.h>

#include <zmq.h>

using namespace alvar;
using namespace std;

bool init=true;
//const int marker_size=15;
const double marker_size=0.1397; //test
const double scaley = 1.0313;
const double scalex = 1.0235;
Camera cam;
Drawable d[32];
std::stringstream calibrationFilename;

std::ofstream file;

void *pContext;
void *pSocket;

void videocallback(IplImage *image,struct timeval* timestamp)
{
    static IplImage *rgba;
    bool flip_image = (image->origin?true:false);
    if (flip_image) {
        cvFlip(image);
        image->origin = !image->origin;
    }

    if (init) {
        init = false;
        cout<<"Loading calibration: "<<calibrationFilename.str();
        if (cam.SetCalib(calibrationFilename.str().c_str(), image->width, image->height)) {
            cout<<" [Ok]"<<endl;
        } else {
            cam.SetRes(image->width, image->height);
            cout<<" [Fail]"<<endl;
        }
        double p[16];
        cam.GetOpenglProjectionMatrix(p,image->width,image->height);
        GlutViewer::SetGlProjectionMatrix(p);
        for (int i=0; i<32; i++) {
            //d[i].SetScale(marker_size);
            d[i].SetScale(marker_size*2); //test
        }
        rgba = CvTestbed::Instance().CreateImageWithProto("RGBA", image, 0, 4);
    }
    static MarkerDetector<MarkerData> marker_detector;
    marker_detector.SetMarkerSize(marker_size); // for marker ids larger than 255, set the content resolution accordingly
    //static MarkerDetector<MarkerArtoolkit> marker_detector;
    //marker_detector.SetMarkerSize(2.8, 3, 1.5);


    // Here we try to use RGBA just to make sure that also it works...
    //cvCvtColor(image, rgba, CV_RGB2RGBA);
    marker_detector.Detect(image, &cam, true, true);
    GlutViewer::DrawableClear();
    //test
    CvMat* t = cvCreateMat(4,1,CV_64F); 
    CvMat* q = cvCreateMat(4,1,CV_64F); 
    CvMat* e = cvCreateMat(3,1,CV_64F); 

    for (size_t i=0; i<marker_detector.markers->size(); i++) {
        if (i >= 32) break;
        
        Pose p = (*(marker_detector.markers))[i].pose;
	//test
        std::cout << "id: " << (*marker_detector.markers)[i].GetId() << std::endl;
        // 'x' is right, 'y' is down, and 'z' is forward.
        // I believe units are in meters, however I'm just guessing
        p.GetTranslation(t);
	std::cout << "translation:" <<
                     " x " << t->data.db[0]/scalex <<
                     " y " << t->data.db[1]/scaley <<
                     " z " << t->data.db[2] <<
                     std::endl;
        p.GetEuler(e);
        std:: cout << "rotation:" <<
                     " x " << e->data.db[0] <<
                     " y " << e->data.db[1] <<
                     " z " << e->data.db[2] <<
                     std::endl;
        p.GetQuaternion(q);
        std:: cout << "rotation:" <<
                     " x " << q->data.db[0] <<
                     " y " << q->data.db[1] <<
                     " z " << q->data.db[2] <<
                     " w " << q->data.db[3] <<
                     std::endl << std::endl;
        p.GetEuler(e);
        if(file.is_open())
        {
          file << timestamp->tv_sec << "." << std::setfill('0') << std::setw(6) << timestamp->tv_usec << "," <<
                  (*marker_detector.markers)[i].GetId() << "," <<
                  t->data.db[0]/scalex << "," <<
                  t->data.db[1]/scaley << "," <<
                  t->data.db[2] << "," <<
                  e->data.db[0] << "," <<
                  e->data.db[1] << "," <<
                  e->data.db[2] << "," <<
                  q->data.db[0] << "," <<
                  q->data.db[1] << "," <<
                  q->data.db[2] << "," <<
                  q->data.db[3] << 
                  std::endl;
        }
        std::stringstream posedata;
        posedata << "pose::" <<
                 timestamp->tv_sec << "." << std::setfill('0') << std::setw(6) << timestamp->tv_usec << "," <<
                 t->data.db[0]/scalex << "," <<
                 t->data.db[1]/scaley << "," <<
                 t->data.db[2] << "," <<
                 q->data.db[0] << "," <<
                 q->data.db[1] << "," <<
                 q->data.db[2] << "," <<
                 q->data.db[3];
        std::string strpose = posedata.str();
        zmq_send(pSocket,strpose.c_str(),strpose.size(),ZMQ_DONTWAIT);

        p.GetMatrixGL(d[i].gl_mat);

        int id = (*(marker_detector.markers))[i].GetId();
        double r = 1.0 - double(id+1)/32.0;
        double g = 1.0 - double(id*3%32+1)/32.0;
        double b = 1.0 - double(id*7%32+1)/32.0;
        d[i].SetColor(r, g, b);

        GlutViewer::DrawableAdd(&(d[i]));
    }

    if (flip_image) {
        cvFlip(image);
        image->origin = !image->origin;
    }
}

int main(int argc, char *argv[])
{
    try {
        bool userCalFile=false;
        bool userVideoFile=false;
        std::string uniqueName;
        
        pContext = zmq_ctx_new();
        pSocket = zmq_socket(pContext, ZMQ_PUB);
        zmq_bind(pSocket,"tcp://*:5555");

        // Output usage message
        std::string filename(argv[0]);
        filename = filename.substr(filename.find_last_of('\\') + 1);
        std::cout << "SampleMarkerDetector" << std::endl;
        std::cout << "====================" << std::endl;
        std::cout << std::endl;
        std::cout << "Description:" << std::endl;
        std::cout << "  This is an example of how to detect 'MarkerData' markers using" << std::endl;
        std::cout << "  'MarkerDetector' and visualize them using 'GlutViewer'. In the" << std::endl;
        std::cout << "  SampleMarkerDetector window, various debug information is shown" << std::endl;
        std::cout << "  about the detected markers. The coordinate axes and a virtual cube" << std::endl;
        std::cout << "  are superimposed onto the markers to visualize the detected pose." << std::endl;
        std::cout << "  For each marker, a small image of the marker content is displayed" << std::endl;
        std::cout << "  at the origin and the marker number is displayed at one of the" << std::endl;
        std::cout << "  corners. At the opposing corner, the error estimation percentages" << std::endl;
        std::cout << "  'MARGIN_ERROR' and 'DECODE_ERROR' (red) or 'TRACK_ERROR' (dark red)" << std::endl;
        std::cout << "  are displayed." << std::endl;
        std::cout << std::endl;
        std::cout << "  In the AR window, squares are drawn over the marker positions using" << std::endl;
        std::cout << "  OpenGL. In the VR window, the squares are drawn with respect to the" << std::endl;
        std::cout << "  camera coordinate frame. The viewpoint can be modified by dragging" << std::endl;
        std::cout << "  with the left and right mouse buttons." << std::endl;
        std::cout << std::endl;
        std::cout << "Usage:" << std::endl;
        std::cout << "  " << filename << " [device] [calibration file] [recorded file]" << std::endl;
        std::cout << std::endl;
        std::cout << "    device    integer selecting device from enumeration list (default 0)" << std::endl;
        std::cout << "              highgui capture devices are prefered" << std::endl;
        std::cout << "    calibration file    optional, name of file to save the calibration" << std::endl;
        std::cout << "                        parameters" << std::endl;
        std::cout << "    recorded file    optional, name of saved video file" << std::endl;
        std::cout << "                     if this option is used, the device number is ignored" << std::endl;

        std::cout << std::endl;
        std::cout << "Keyboard Shortcuts:" << std::endl;
        std::cout << "  q: quit" << std::endl;
        std::cout << std::endl;

        // Initialize file logger
        std::string logName;
        std::stringstream logTime;
        struct timeval t;
        gettimeofday(&t,NULL);
        logTime << t.tv_sec;
        logName = "log_" + logTime.str() + ".csv";
        file.open(logName.c_str());
        if(file.is_open()) {
          //file.precision(std::numeric_limits<double>::digits10);
          file << "\"timestamp\",\"marker\",\"x\",\"y\",\"z\",\"rx\",\"ry\",\"rz\",\"qx\",\"qy\",\"qz\",\"qw\"" << std::endl;
        }
        // Initialise GlutViewer and CvTestbed
        GlutViewer::Start(argc, argv, 640, 480);
        CvTestbed::Instance().SetVideoCallback(videocallback);

        // Create capture object from camera (argv[1] is a number) or from file (argv[1] is a string)
        Capture *cap;
        if ((argc > 1) && (!isdigit(argv[1][0]))) {
            // Manually create capture device and initialize capture object
            CaptureDevice device("file", argv[1]);
            cap = CaptureFactory::instance()->createCapture(device);
            uniqueName = "file";
        }
        else {    
            // Enumerate possible capture plugins
            CaptureFactory::CapturePluginVector plugins = CaptureFactory::instance()->enumeratePlugins();
            if (plugins.size() < 1) {
                std::cout << "Could not find any capture plugins." << std::endl;
                return 0;
            }

            // Display capture plugins
            std::cout << "Available Plugins: ";
            outputEnumeratedPlugins(plugins);
            std::cout << std::endl;

            if(argc > 2)
              userCalFile = true;
            if(argc > 3)
              userVideoFile = true;

            // Enumerate possible capture devices
            CaptureFactory::CaptureDeviceVector devices = CaptureFactory::instance()->enumerateDevices();
           
            if(userVideoFile) {
              CaptureDevice file("file",argv[3],"filecal");
              devices.push_back(file);
            }

            if (devices.size() < 1) {
                std::cout << "Could not find any capture devices." << std::endl;
                return 0;
            }

            // Check command line argument for which device to use
            int selectedDevice = defaultDevice(devices);
            if (argc > 1) {
                selectedDevice = atoi(argv[1]);
            }
            if (userVideoFile) {
              selectedDevice = (int)devices.size() - 1;
            }
            if (selectedDevice >= (int)devices.size()) {
                selectedDevice = defaultDevice(devices);
            }
            
            // Display capture devices
            std::cout << "Enumerated Capture Devices:" << std::endl;
            outputEnumeratedDevices(devices, selectedDevice);
            std::cout << std::endl;
            
            // Create capture object from camera
            cap = CaptureFactory::instance()->createCapture(devices[selectedDevice]);
            uniqueName = devices[selectedDevice].uniqueName();
        }

        // Handle capture lifecycle and start video capture
        // Note that loadSettings/saveSettings are not supported by all plugins
        if (cap) {
            std::stringstream settingsFilename;
            settingsFilename << "camera_settings_" << uniqueName << ".xml";
            if(userCalFile) {
              calibrationFilename << argv[2];
            } else {
              calibrationFilename << "camera_calibration_" << uniqueName << ".xml";
            }

            
            cap->start();
            cap->setResolution(640, 480);
            
            if (cap->loadSettings(settingsFilename.str())) {
                std::cout << "Loading settings: " << settingsFilename.str() << std::endl;
            }

            std::stringstream title;
            title << "SampleMarkerDetector (" << cap->captureDevice().captureType() << ")";
            CvTestbed::Instance().RecordImages(true);
            CvTestbed::Instance().StartVideo(cap, title.str().c_str());
            if (cap->saveSettings(settingsFilename.str())) {
                std::cout << "Saving settings: " << settingsFilename.str() << std::endl;
            }
            cap->stop();
            delete cap;
        }
        else if (CvTestbed::Instance().StartVideo(0, argv[0])) {
        }
        else {
            std::cout << "Could not initialize the selected capture backend." << std::endl;
        }

        if(file.is_open()) {
          file.close();
        }
        return 0;
    }
    catch (const std::exception &e) {
        std::cout << "Exception: " << e.what() << endl;
    }
    catch (...) {
        std::cout << "Exception: unknown" << std::endl;
    }
}
