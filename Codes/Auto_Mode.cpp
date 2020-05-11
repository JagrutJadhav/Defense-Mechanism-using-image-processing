

/*
	*
	* Project Name: 	Defence Mechanism using Image Processing
	* Author List: 		Rahul Chandak, Jagrut Jadhav
	* Filename: 		Auto_Mode.cpp
	* Functions: 		main()
	* Global Variables:	none
	*
*/

#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <wiringPi.h>
#include <iostream>
#include <stdio.h>
#include <wiringSerial.h>

using namespace cv;
using namespace std;

/*
	* Function Name:	main()
	* Input:		    none
	* Output:		    X and Y coordinates of detected object from camera
	*
	* Logic:			Distributing the image into 4 quadrants using
						xdist = centrefacex (+/-) centrecols
						ydist = centrerows (+/-) centrefacey
						where,  xdist is horizontal distance of the detected object from the centre of the screen
								ydist is vertical distance of the detected object from the centre of the screen
								centrefacex is centre of object (x-coordinate)
								centrefacey is centre of object (y-coordinate)
                                centrerows is centre of screen (x-coordinate)
								centrecols is centre of screen (y-coordinate)

						Getting pixel values of the whole visible image in video capture and converting into degrees
						valx = img.cols / deg;
						valy = img.rows / deg;

						countx and county are the number of degrees the servos should move to intercept the object
						and are sent to the arduino using UART.

						black.xml file is the haarcascade file trained in opencv

	*Example Call:		Called automatically by the Operating System	
	*
	*/

int main()
{
	wiringPiSetup();
	int fd ;
    int centrefacex, centrefacey, centrecols, centrerows , valx , valy , xdist , ydist ;
	int deg = 12;
	int motionx = 0 , motiony =0;

	if ((fd = serialOpen ("/dev/ttyUSB0",9600)) < 0)   //Checking if serial UART communication is established
	{
		fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
		return 1 ;
    }
 
	VideoCapture cap(0);
	cap.open(0);										//Opening webcam
	Mat img;
	CascadeClassifier face_cascade;
	face_cascade.load("black.xml");
	for (;;)
	{
		cap >> img;		
		cap.retrieve(img, CAP_OPENNI_BGR_IMAGE);
		resize(img, img, Size(900,630));
		vector<Rect> faces;
		face_cascade.detectMultiScale(img, faces, 1.1, 1 ,30| CASCADE_SCALE_IMAGE, Size(150, 150));

		valx = img.cols / deg;
		valy = img.rows / deg;
		
		rectangle(img, Point(((img.cols) / 2) - 5, ((img.rows) / 2) - 5), Point(((img.cols) / 2) + 5, ((img.rows) / 2) + 5), Scalar(255, 0, 0), 2, 8);
		
		for (unsigned i = 0; i < faces.size(); i++)
		{ 

			rectangle(img, faces[i], Scalar(255, 0, 0), 2, 1);

			centrecols = ((img.cols) / 2);                        //getting centre of screen x-coordinate
			centrerows = ((img.rows) / 2);                        //getting centre of screen y-coordinate
			centrefacex = (faces[i].x + (faces[i].width) / 2);    //getting centre of object x-coordinate
			centrefacey = (faces[i].y + (faces[i].height) / 2);   //getting centre of object y-coordinate

			if ((centrefacex > centrecols) && (centrefacey < centrerows))			// I quadrant
			{
				xdist = centrefacex - centrecols;
				ydist = centrerows - centrefacey;
				//cout << xdist << " " << ydist << endl;
				if(motionx <= xdist)
				{
					while (motionx <= xdist)
					{
						motionx = motionx + valx;
						countx -= 1;
						if (motionx==xdist)
							break;
					}
				}
				
				else 
					if (motionx > xdist && motionx > 0)
					{
						while (motionx >= xdist)
						{	
							motionx = motionx-valx; 
							countx += 1;
							if (motionx==xdist)
							break;
						}
					}
				if(motiony <= ydist)
				{
					while (motiony <= ydist)
					{ 
						motiony = motiony + valy;
						county += 1;
						if (motiony==ydist)
							break;
					}
				}
				else 
				if (motiony >= ydist && motiony > 0)
				{
				    while (motiony >= ydist)
				    {	
						motiony = motiony -valy;
						county -= 1;
						if (motiony==ydist)
							break;
					}
				}
				
				//cout<<"countx = "<<countx<<"----"<<"county = "<<county<<"---- motionx = "<<motionx <<"----"<<"motiony = "<<motiony<< endl;
				serialPutchar (fd, countx) ;
				serialPutchar (fd, county) ;
				cout<<" countx = "<< countx <<"county = "<< county <<endl;
			}
				
			//////////////////////////////////////////
				
			else if ((centrefacex < centrecols) && (centrefacey < centrerows))		//II quadrant
			{
				xdist = centrefacex - centrecols;
				ydist = centrerows - centrefacey;

				//cout << xdist << " " << ydist << endl;
				if(motionx >= xdist)
				{
				   while (motionx >= xdist)
				   {
					  motionx = motionx - valx;
					  countx +=1;
					  if (motionx==xdist)
							break;
				   }
			    } 
			    else if(motionx <= xdist && motionx < 0)
			    {
					while (motionx <= xdist)
					{
						motionx = motionx + valx;
						countx -= 1;
						if (motionx==xdist)
							break;
					}
				}
				if (motiony <= ydist)
				{	
					while (motiony <= ydist)
					{
						motiony = motiony + valy;
						county += 1;
						if (motiony==ydist)
							break;
					}	
				}
				else if (motiony >= ydist && motiony > 0)
				{
					while (motiony >= ydist)
					{
						motiony = motiony - valy;
						county -= 1;
						if (motiony==ydist)
							break;
					}
				}
				//cout<<"countx = "<<countx<<"----"<<"county = "<<county<<"---- motionx = "<<motionx <<"----"<<"motiony = "<<motiony<< endl;
				
				  serialPutchar (fd, countx) ;
				  serialPutchar (fd, county) ;
				  cout<<" countx = "<< countx <<"county = "<< county <<endl;
			}
			
			//////////////////////////////////////////
			
			else if ((centrefacex < centrecols) && (centrefacey > centrerows))			 //III quadrant
			{
				xdist = centrefacex - centrecols;
				ydist = centrerows - centrefacey;
				//cout << xdist << " " << ydist << endl;
				if(motionx >= xdist)
				{
				while (motionx >= xdist)
					{
						motionx = motionx - valx;
						countx +=1;
						if (motionx==xdist)
								break;
					}
				}
				else if (motionx <= xdist && motionx <0)
				{
					while (motionx <= xdist )
					{
						motionx = motionx + valx;
						countx -=1;
						if (motionx==xdist)
							break;
					}
				}
				if (motiony >= ydist)
				{
				while (motiony >= ydist)
				{
					motiony = motiony - valy;
					county -= 1;
					if (motiony==ydist)
							break;
				}}
				else if (motiony <= ydist && motiony < 0)
				{
					while(motiony <= ydist)
					{
						motiony = motiony +valy;
						county += 1;
						if (motiony==ydist)
							break;
					}
					
				}
				//cout<<"countx = "<<countx<<"----"<<"county = "<<county<<"---- motionx = "<<motionx <<"----"<<"motiony = "<<motiony<< endl;
				serialPutchar (fd, countx) ;
				serialPutchar (fd, county) ;
				cout<<" countx = "<< countx <<"county = "<< county <<endl;
			}
			
			/////////////////////////////////
			
			else if ((centrefacex > centrecols) && (centrefacey > centrerows))				// IV quadrant
			{
				xdist = centrefacex - centrecols;
				ydist = centrerows - centrefacey;
				if (motionx <= xdist){
					
				while (motionx <= xdist)
					{
						motionx = motionx + valx;
						countx -=1;
						if (motionx==xdist)
							break;
					}
				}
				else if (motionx >= xdist && motionx >0)
				{
					while (motionx >= xdist)
					{
						motionx =motionx - valx;
						countx += 1;
						if (motionx==xdist)
							break;
					}
				}
				if (motiony >= ydist)
				{
					while (motiony >= ydist)
					{
						motiony = motiony - valy;
						county -= 1;
						if (motiony==ydist)
							break;
					}
				}
				else if (motiony <= ydist && motiony <0)
				{
					while (motiony <= ydist)
					{
						motiony = motiony + valy;
						county += 1;
						if (motiony==ydist)
							break;
					}
				}
				//cout<<"countx = "<<countx<<"----"<<"county = "<<county<<"---- motionx = "<<motionx <<"----"<<"motiony = "<<motiony<< endl;
				serialPutchar (fd, countx) ;
				serialPutchar (fd, county) ;
				cout<<" countx = "<< countx <<"county = "<< county <<endl;
			}
           }
			
		imshow("wooohooo", img);
		if (waitKey(20) == 27)
		{
			break;
		}
	}
	return 0;
}
