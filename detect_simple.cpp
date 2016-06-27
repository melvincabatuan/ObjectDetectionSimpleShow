/****************************************************************************************************
This code is part of the code supplied with the OpenCV Blueprints book.
It was written by Steven Puttemans, who can be contacted via steven.puttemans[at]kuleuven.be

License can be found at https://github.com/OpenCVBlueprints/OpenCVBlueprints/blob/master/license.txt
*****************************************************************************************************
This is software for performing a simple detection using a trained cascade classifier model.

USAGE:
./detect_simple -model <model.xml> -testfiles <list.txt> -detections <output.txt>

Modified (MKC): Show detections and/or pause (using spacebar)
***********************************************************************************************/

#include <iostream>
#include <fstream>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/imgproc.hpp"

using namespace std;
using namespace cv;

int main( int argc, const char** argv )
{
    // If no parameters are give, then a usage template should be provided
    if(argc == 1){
	cout << "This is software for performing a simple detection using a trained cascade classifier model." << endl;
        cout << "USAGE ./detect_simple -model <model.xml> -testfiles <list.txt> -detections <output.txt>" << endl;
        return 0;
    }

    // Read in the input arguments
    string model = "";
    string testfiles = "";
    string detections = "";
    for(int i = 1; i < argc; ++i )
    {
        if( !strcmp( argv[i], "-model" ) )
        {
            model = argv[++i];
        }else if( !strcmp( argv[i], "-testfiles" ) ){
            testfiles = argv[++i];
        }else if( !strcmp( argv[i], "-detections" ) ){
            detections = argv[++i];
        }
    }

    // Start by initiating the detector
    CascadeClassifier detector(model);
    if(detector.empty()){
        cerr << "The model could not be loaded." << endl;
    }

    // Read images from the file
    ifstream images(testfiles.c_str());
    vector<string> filenames;
    string single_line;
    while( getline(images, single_line) ){
        filenames.push_back(single_line);
    }

    // Loop over images, perform detection and store result
    ofstream result(detections.c_str());
    char keypress;
    for(int i = 0; i < (int)filenames.size(); i++){
        Mat current_image, grayscale, equalize;

        // Read in image and perform preprocessing
        current_image = imread(filenames[i]);
        cvtColor(current_image, grayscale, CV_BGR2GRAY);
        equalizeHist(grayscale,equalize);

        // Perform detection
        // Retrieve detection certainty scores
        // Perform non maxima suppression
        vector<Rect> objects;
        detector.detectMultiScale(equalize, objects, 1.05, 1);

        // Loop over detections and store them correctly
        stringstream outputline;
        outputline << filenames[i] << " " << objects.size() << " ";
        for(int j = 0; j < (int)objects.size(); j++){
            outputline << objects[j].x << " " << objects[j].y << " " << objects[j].width << " " << objects[j].height << " ";
            // Draw rectangle
            rectangle(current_image, objects[j], Scalar(0, 255, 10), 5);
        }

        outputline << endl;
        result << outputline.str();
        cerr << "*";
        imshow("Current Image", current_image);
        keypress = waitKey(1500);	
        if( keypress == 32 ) waitKey(0);
		if( keypress == 27 ) break;		
    }
    return 0;
}
