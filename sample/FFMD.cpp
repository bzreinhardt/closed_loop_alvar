#include "CvTestbed.h"
#include "ControlClass.h"
#include "MarkerDetector.h"
#include "MultiMarker.h"
#include "GlutViewer.h"
#include "Shared.h"
#include "FFKalmanFilter.h"

#include <fstream>
#include <string>
#include <sys/time.h>
#include <sys/stat.h>
#include <ctime>

#include <zmq.h>

using namespace alvar;
using namespace std;

bool init=true;
bool initKF=true;
const double marker_size=0.1397; //test
const double scaley = 1.0313;
const double scalex = 1.0235;
Camera cam;
Drawable d[32];

std::ofstream file;

void *pContext;
void *pSocket;

//argument params
bool record = false;
bool userCalFile=false;
bool userVideoFile=false;
bool logPose = true;
bool displayPose = false;
bool displayLog = false;
bool displayGL = true;
bool userPicDir=false;
bool userRecordVideo=false;
bool multiMarker=false;
bool useKalmanFilter=false;
bool firstRun=true;
std::string userPicDirname;
std::string userVideoFilename;
std::string zeroMQPort = "tcp://*:5555";
std::string userMultiMarkerFilename;
std::stringstream calibrationFilename;
int selectedDevice = -1;
int videoWidth = 1280;
int videoHeight = 960;
int nof_markers = 5;

struct timeval prevTS;
CvMat* prevT; 
CvMat* prevE; 

MarkerDetector<MarkerData> marker_detector;
MultiMarker *multi_marker;
KalmanSensorOwn sensor(4,2);
KalmanOwn kalman(4); // x, y, dx, dy 

void filter_ekf(double x, double y, double *fx, double*fy, double* fdx, double* fdy) {
    if (initKF) {
        initKF = false;
        // R
        cvSetIdentity(sensor.R, cvScalar(100));
        // Q
        cvmSet(kalman.Q, 0, 0, 0.001);
        cvmSet(kalman.Q, 1, 1, 0.001);
        cvmSet(kalman.Q, 2, 2, 0.01);
        cvmSet(kalman.Q, 3, 3, 0.01);
        // P
        cvSetIdentity(kalman.P, cvScalar(100));
    }
    cvmSet(sensor.z, 0, 0, x);
    cvmSet(sensor.z, 1, 0, y);
    kalman.predict_update(&sensor, (unsigned long)(cv::getTickCount() / cv::getTickFrequency() * 1000));
    *fx = cvmGet(kalman.x, 0, 0);
    *fy = cvmGet(kalman.x, 1, 0);
    *fdx = cvmGet(kalman.x, 2, 0);
    *fdy = cvmGet(kalman.x, 3, 0);

}
  
bool argDecoderRing(int argc, char *argv[]) {
  for(int i=0; i < argc; i++)
  {
    if(argv[i][0] == '-') 
    {
      if(i+1 > argc) {
        std::cout << "Incomplete argument: " << argv[i] << std::endl;
        return false;
      }
      switch(argv[i][1]) {
        case 'c':
          userCalFile = true;
          calibrationFilename << argv[i+1];
          break;
        case 'd':
          if((strncmp(argv[i+1],"true",5)==0) || (strncmp(argv[i+1],"True",5) == 0)) {
            displayGL = true;
          } else { 
            displayGL = false;
          }
          break;
        case 'e':
          zeroMQPort = "tcp://" + std::string(argv[i+1]);
          break;
        case 'f':
          userPicDir = true;
          userPicDirname = std::string(argv[i+1]);
          break;
        case 'k':
          if((strncmp(argv[i+1],"true",5)==0) || (strncmp(argv[i+1],"True",5) == 0)) {
            useKalmanFilter = true;
          }
          break; 
        case 'm':
          multiMarker = true;
          if(i+1 < argc) {
            if((argv[i+1][0] != '-')&&(!isdigit(argv[i+1][0]))) {
              userMultiMarkerFilename = std::string(argv[i+1]);
            }
          }
          break;
        case 'v':
          userVideoFile = true;
          userVideoFilename = std::string(argv[i+1]);
          break;
        case 'L':
          if((strncmp(argv[i+1],"true",5)==0) || (strncmp(argv[i+1],"True",5) == 0)) {
            logPose = true;
          } else {
            logPose = false;
          }
          break;
        case 'O':
          if((strncmp(argv[i+1],"true",5)==0) || (strncmp(argv[i+1],"True",5) == 0)) {
            displayPose = true;
          } else { 
            displayPose = false;
          }
          break;
        case 'P':
          {
            std::string arg=argv[i+1];
            int p = arg.find(':',0);
            std::string w = arg.substr(0,p);
            std::string h = arg.substr(p+1,arg.size());
            videoWidth = atoi(w.c_str());
            videoHeight = atoi(h.c_str());
          }
          break;
        case 'R':
          if((strncmp(argv[i+1],"true",5)==0) || (strncmp(argv[i+1],"True",5) == 0)) {
            record = true;
          } else
            record = false;
          break;
        case 'V':
          if((strncmp(argv[i+1],"true",5)==0) || (strncmp(argv[i+1],"True",5) == 0)) {
            userRecordVideo=true;
          } else
            userRecordVideo=false;
          break;
        case 'h':
        case '?':
        default:
          std::cout <<"Usage: ffmd [device id] [parameters]" << std::endl;
          std::cout <<"-c <filename>: specify calibration file" << std::endl;
          std::cout <<"-d <*true|false>: Display images screen, default is true" << std::endl;
          std::cout <<"-e <dev:port>: Specify 0mq device and port to talk over, ie eth0:12345. Default is *:5555" << std::endl;
          std::cout <<"-f <directory>: specify picture file directory to playback" << std::endl;
          std::cout <<"-h: display this help" << std::endl;
          std::cout <<"-k <true|*false>: enable ekf" << std::endl;
          std::cout <<"-m [mmaker.xml] : multimarker detection with option to load a multimarker configuration file." << std::endl;
          std::cout <<"-v <filename>: specify video file to playback" << std::endl;
          std::cout << "-P <width:height>: specify video resolution in pixel size, default is 1280:960." << std::endl;
          std::cout <<"-L <*true|false>: Log the positions to file, default is true" << std::endl;
          std::cout <<"-O <true|*false>: Display position data to screen, default is false" << std::endl;
          std::cout <<"-R <true|*false>: record images to pngs, default is false" << std::endl;
          std::cout <<"-V <true|*false>: record video to avi file, default is false" << std::endl;
          return false;
      }
    } else {
      selectedDevice = atoi(argv[i]);
    }
  }
  return true;
}

void videocallback(IplImage *image,struct timeval* timestamp)
{
    static IplImage *rgba;
    Pose pose;
    bool flip_image = (image->origin?true:false);
    double dx,dy,dz;
    CvMat* dE = cvCreateMat(3,1,CV_64F); 

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
        if(displayGL)
          GlutViewer::SetGlProjectionMatrix(p);
        if(multiMarker) {

          vector<int> id_vector;
          for(int i = 0; i < nof_markers; ++i)
            id_vector.push_back(i);

          // We make the initialization for MultiMarkerBundle using a fixed marker field (can be printed from ALVAR.pdf)
          marker_detector.SetMarkerSize(marker_size/3.885552808);
          marker_detector.SetMarkerSizeForId(0, marker_size/1.942776404);
          multi_marker = new MultiMarker(id_vector);
          bool mminit = true;
          if(userMultiMarkerFilename.size() > 0) {
            if(!multi_marker->Load(userMultiMarkerFilename.c_str(), alvar::FILE_FORMAT_XML))
              mminit = false;
          } 
          if(mminit) {
            pose.Reset();
            multi_marker->PointCloudAdd(0, marker_size*2, pose);
            pose.SetTranslation(-marker_size*2.5, +marker_size*1.5, 0);
            multi_marker->PointCloudAdd(1, marker_size, pose);
            pose.SetTranslation(+marker_size*2.5, +marker_size*1.5, 0);
            multi_marker->PointCloudAdd(2, marker_size, pose);
            pose.SetTranslation(-marker_size*2.5, -marker_size*1.5, 0);
            multi_marker->PointCloudAdd(3, marker_size, pose);
            pose.SetTranslation(+marker_size*2.5, -marker_size*1.5, 0);
            multi_marker->PointCloudAdd(4, marker_size, pose); 
          }
        } else {
          marker_detector.SetMarkerSize(marker_size); // for marker ids larger than 255, set the content resolution accordingly
        }
        for (int i=0; i<32; i++) {
            d[i].SetScale(marker_size*2); //test
        }
        rgba = CvTestbed::Instance().CreateImageWithProto("RGBA", image, 0, 4);
        prevT = cvCreateMat(4,1,CV_64F); 
        prevE = cvCreateMat(3,1,CV_64F); 
    }

    if(displayGL)
      GlutViewer::DrawableClear();

    if(multiMarker) {
      double error=-1;
      if (marker_detector.Detect(image, &cam, true, false, 0.0)) {
         /*
          if (detect_additional) {
              error = multi_marker->Update(marker_detector.markers, &cam, pose);
              multi_marker->SetTrackMarkers(marker_detector, &cam, pose, visualize ? image : NULL);
              marker_detector.DetectAdditional(image, &cam, (visualize == 1));
          } */
/*
          if (visualize == 2)
              error = multi_marker->Update(marker_detector.markers, &cam, pose, image);
          else*/
              error = multi_marker->Update(marker_detector.markers, &cam, pose);
      
        static Marker foo;
        foo.SetMarkerSize(marker_size);
        if ((error >= 0) && (error < 5))
        {
          foo.pose = pose;
        }
        foo.Visualize(image, &cam);

        CvMat* t = cvCreateMat(4,1,CV_64F); 
        CvMat* q = cvCreateMat(4,1,CV_64F); 
        CvMat* e = cvCreateMat(3,1,CV_64F); 

        pose.GetTranslation(t);
        pose.GetEuler(e);
        pose.GetQuaternion(q);

        if(firstRun) {
          firstRun = false;
          prevTS = *timestamp;
          *prevT = *t;
          *prevE = *e;
          dx = 0.0;
          dy = 0.0;
          dz = 0.0;
          dE->data.db[0] = 0;
          dE->data.db[1] = 0;
          dE->data.db[2] = 0;
        } else {
          double dt = timestamp->tv_sec - prevTS.tv_sec + (timestamp->tv_usec - prevTS.tv_usec)/1000000.0;
          dx = (t->data.db[0] - prevT->data.db[0])/dt;
          dy = (t->data.db[1] - prevT->data.db[1])/dt;
          dz = (t->data.db[2] - prevT->data.db[2])/dt;
          dE->data.db[0] = (e->data.db[0]-prevE->data.db[0])/dt;
          dE->data.db[1] = (e->data.db[1]-prevE->data.db[1])/dt;
          dE->data.db[2] = (e->data.db[2]-prevE->data.db[2])/dt;
          *prevT = *t;
          *prevE = *e;
          prevTS = *timestamp;
        }

        if(useKalmanFilter) {
          double x,y;
          filter_ekf(t->data.db[0],t->data.db[1],&x,&y,&dx,&dy);
          t->data.db[0] = x;
          t->data.db[1] = y;
        }
        if(displayPose) {
          std::cout << "id: 0" << std::endl;
          // 'x' is right, 'y' is down, and 'z' is forward.
          // I believe units are in meters, however I'm just guessing
          std::cout << "translation:" <<
                       " x " << t->data.db[0]/scalex <<
                       " y " << t->data.db[1]/scaley <<
                       " z " << t->data.db[2] <<
                       std::endl;
          std:: cout << "rotation:" <<
                        " x " << e->data.db[0] <<
                        " y " << e->data.db[1] <<
                        " z " << e->data.db[2] <<
                        std::endl;
          std:: cout << "rotation:" <<
                        " x " << q->data.db[0] <<
                        " y " << q->data.db[1] <<
                        " z " << q->data.db[2] <<
                        " w " << q->data.db[3] <<
                        std::endl;
          std:: cout << "translation velocity:" <<
                        " dx " << dx <<
                        " dy " << dy <<
                        " dz " << dz <<
                        std::endl;
          std:: cout << "rotation velocity:" <<
                        " rx " << dE->data.db[0] <<
                        " ry " << dE->data.db[1] <<
                        " rz " << dE->data.db[2] <<
                        std::endl << std::endl;
        }

        if(file.is_open() && logPose)
        {
          file << timestamp->tv_sec << "." << std::setfill('0') << std::setw(6) << timestamp->tv_usec << "," <<
                  "0" << "," <<
                  t->data.db[0]/scalex << "," <<
                  t->data.db[1]/scaley << "," <<
                  t->data.db[2] << "," <<
                  e->data.db[0] << "," <<
                  e->data.db[1] << "," <<
                  e->data.db[2] << "," <<
                  q->data.db[0] << "," <<
                  q->data.db[1] << "," <<
                  q->data.db[2] << "," <<
                  q->data.db[3] << "," <<
                  dx << "," <<
                  dy << "," <<
                  dz << "," <<
                  dE->data.db[0] << "," <<
                  dE->data.db[1] << "," <<
                  dE->data.db[2] << 
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
                 q->data.db[3] << "," <<
                 dx << "," <<
                 dy << "," <<
                 dz << "," <<
                 dE->data.db[0] << "," <<
                 dE->data.db[1] << "," <<
                 dE->data.db[2];  
        std::string strpose = posedata.str();
        zmq_send(pSocket,strpose.c_str(),strpose.size(),ZMQ_DONTWAIT);
        double r = 1.0 - double(17)/32.0;
        double g = 1.0 - double(17)/32.0;
        double b = 1.0 - double(17)/32.0;
        d[0].SetColor(r, g, b);

        if(displayGL)
              GlutViewer::DrawableAdd(&(d[0]));

      }
    } else {
      // Here we try to use RGBA just to make sure that also it works...
      //cvCvtColor(image, rgba, CV_RGB2RGBA);
      marker_detector.Detect(image, &cam, true, true);

      CvMat* t = cvCreateMat(4,1,CV_64F); 
      CvMat* q = cvCreateMat(4,1,CV_64F); 
      CvMat* e = cvCreateMat(3,1,CV_64F); 

      for (size_t i=0; i<marker_detector.markers->size(); i++) {
          if (i >= 32) break;
        
          pose = (*(marker_detector.markers))[i].pose;
          pose.GetTranslation(t);
          pose.GetEuler(e);
          pose.GetQuaternion(q);
          if(firstRun) {
            firstRun = false;
            prevTS = *timestamp;
            *prevT = *t;
            *prevE = *e;
            dx = 0.0;
            dy = 0.0;
            dz = 0.0;
            dE->data.db[0] = 0;
            dE->data.db[1] = 0;
            dE->data.db[2] = 0;
          } else {
            double dt = timestamp->tv_sec - prevTS.tv_sec + (timestamp->tv_usec - prevTS.tv_usec)/1000000.0;
            dx = (t->data.db[0] - prevT->data.db[0])/dt;
            dy = (t->data.db[1] - prevT->data.db[1])/dt;
            dz = (t->data.db[2] - prevT->data.db[2])/dt;
            dE->data.db[0] = (e->data.db[0]-prevE->data.db[0])/dt;
            dE->data.db[1] = (e->data.db[1]-prevE->data.db[1])/dt;
            dE->data.db[2] = (e->data.db[2]-prevE->data.db[2])/dt;
            *prevT = *t;
            *prevE = *e;
            prevTS = *timestamp;
          }

          if(useKalmanFilter) {
            double x,y;
            filter_ekf(t->data.db[0],t->data.db[1],&x,&y,&dx,&dy);
            t->data.db[0] = x;
            t->data.db[1] = y;
          }
          if(displayPose) {
            std::cout << "id: " << (*marker_detector.markers)[i].GetId() << std::endl;
            // 'x' is right, 'y' is down, and 'z' is forward.
            // I believe units are in meters, however I'm just guessing
	    std::cout << "translation:" <<
                         " x " << t->data.db[0]/scalex <<
                         " y " << t->data.db[1]/scaley <<
                         " z " << t->data.db[2] <<
                         std::endl;
            std:: cout << "rotation:" <<
                          " x " << e->data.db[0] <<
                          " y " << e->data.db[1] <<
                          " z " << e->data.db[2] <<
                          std::endl;
            std:: cout << "rotation:" <<
                          " x " << q->data.db[0] <<
                          " y " << q->data.db[1] <<
                          " z " << q->data.db[2] <<
                          " w " << q->data.db[3] <<
                          std::endl;
            std:: cout << "translation velocity:" <<
                          " dx " << dx <<
                          " dy " << dy <<
                          " dz " << dz <<
                          std::endl;
            std:: cout << "rotation velocity:" <<
                          " rx " << dE->data.db[0] <<
                          " ry " << dE->data.db[1] <<
                          " rz " << dE->data.db[2] <<
                          std::endl << std::endl;
          }

          if(file.is_open() && logPose)
          {
	//modified by Ben to record motor speeds as well
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
                    q->data.db[3] << "," <<
                    dx << "," <<
                    dy << "," <<
                    dz << "," <<
                    dE->data.db[0] << "," <<
                    dE->data.db[1] << "," <<
                    dE->data.db[2] << "," <<
		    ControlClass::Instance().GetSpeed(1) << "," <<
		    ControlClass::Instance().GetSpeed(2) <<	 
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
                   q->data.db[3] << "," <<
                   dx << "," <<
                   dy << "," <<
                   dz << "," <<
                   dE->data.db[0] << "," <<
                   dE->data.db[1] << "," <<
                   dE->data.db[2]; 
          std::string strpose = posedata.str();
          zmq_send(pSocket,strpose.c_str(),strpose.size(),ZMQ_DONTWAIT);

          pose.GetMatrixGL(d[i].gl_mat);

          int id = (*(marker_detector.markers))[i].GetId();
          double r = 1.0 - double(id+1)/32.0;
          double g = 1.0 - double(id*3%32+1)/32.0;
          double b = 1.0 - double(id*7%32+1)/32.0;
          d[i].SetColor(r, g, b);

          if(displayGL)
            GlutViewer::DrawableAdd(&(d[i]));
      }
    }
	
    if (flip_image) {
        cvFlip(image);
        image->origin = !image->origin;
    }
/**send commands - make sure that you have updated the ControlClass
 with the appropriate data and calculated the command before doing
 this**/
ControlClass::Instance().SendCmd();
}

int main(int argc, char *argv[])
{
    try {
        std::string uniqueName;
        
        pContext = zmq_ctx_new();
        pSocket = zmq_socket(pContext, ZMQ_PUB);
        zmq_bind(pSocket,zeroMQPort.c_str());

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

        //Parse Ar
        if(!argDecoderRing(argc,argv)) {
          return 0;
        }

        if(logPose || record || userRecordVideo) {
          // Initialize file logger
          std::string logName;
          std::stringstream logTime;
          std::time_t result = std::time(NULL);
          std::string time = std::string(std::asctime(std::localtime(&result)));
          logTime << time.c_str()[20] << time.c_str()[21] << time.c_str()[22] << time.c_str()[23] <<
                     "_" << time.c_str()[4] << time.c_str()[5] << time.c_str()[6] <<
                     "_" << time.c_str()[8] << time.c_str()[9] <<
                     "_" << time.c_str()[11] << time.c_str()[12] << 
                     "_" << time.c_str()[14] << time.c_str()[15] <<
                     "_" << time.c_str()[17] << time.c_str()[18];
          mkdir(logTime.str().c_str(),0777);
          CvTestbed::Instance().SetRecordDirectory(logTime.str());
          if(logPose) {
            logName = logTime.str() + "/PoseLog.csv";
            file.open(logName.c_str());
            if(file.is_open()) {
//edited by Ben to add speed1 and speed2 to the header
              file << "\"timestamp\",\"marker\",\"x\",\"y\",\"z\",\"rx\",\"ry\",\"rz\",\"qx\",\"qy\",\"qz\",\"qw\",\"dx\",\"dy\",\"dz\",\"rx\",\"ry\",\"rz\",\"speed1\",\"speed2\"" << std::endl;
            }
          }
        }

        // Create capture object from camera (argv[1] is a number) or from file (argv[1] is a string)
        Capture *cap;
        if (userVideoFile) {
            // Manually create capture device and initialize capture object
            CaptureDevice device("file", userVideoFilename.c_str());
            cap = CaptureFactory::instance()->createCapture(device);
            uniqueName = "file";
        } else if(userPicDir) {
            // Manually create capture device and initialize capture object
            CaptureDevice device("png", userPicDirname.c_str());
            cap = CaptureFactory::instance()->createCapture(device);
            uniqueName = "png";
        } else {
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

            // Enumerate possible capture devices
            CaptureFactory::CaptureDeviceVector devices = CaptureFactory::instance()->enumerateDevices();
           
            if(userVideoFile) {
              CaptureDevice file("file",userVideoFilename,"filecal");
              devices.push_back(file);
            }

            if (devices.size() < 1) {
                std::cout << "Could not find any capture devices." << std::endl;
                return 0;
            }

            // Check command line argument for which device to use
            if(selectedDevice == -1)
              selectedDevice = defaultDevice(devices);
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
            if(!userCalFile) {
              calibrationFilename << "camera_calibration_" << uniqueName << ".xml";
            }

            //cap->setResolution(videoWidth, videoHeight);
            cap->start();
            cap->setResolution(videoWidth, videoHeight);
            // Initialise GlutViewer and CvTestbed
            if(displayGL)
              GlutViewer::Start(0, NULL, 640, 480);
            CvTestbed::Instance().SetVideoCallback(videocallback);
            CvTestbed::Instance().SetDisplay(displayGL);
            
            if (cap->loadSettings(settingsFilename.str())) {
                std::cout << "Loading settings: " << settingsFilename.str() << std::endl;
            }

            std::stringstream title;
            title << "SampleMarkerDetector (" << cap->captureDevice().captureType() << ")";
            CvTestbed::Instance().RecordImages(record);
            CvTestbed::Instance().RecordVideo(userRecordVideo);
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
