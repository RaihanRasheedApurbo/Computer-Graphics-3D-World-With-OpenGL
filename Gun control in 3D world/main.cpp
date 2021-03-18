#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <iostream>
#include <windows.h>
#include <glut.h>
#include<vector>
using namespace std;
#define pi (2*acos(0.0))

// double cameraHeight;
// double cameraAngle;
int drawgrid;
int drawaxes;
int secondaryAxes;
double angle;
// double shiftingAmount;
// double xAmount;
// double yAmount;
// double zAmount;
double rotationAmount;
// bool firstTime = true;
double angleQW;
double angleER;
double angleAS;
double angleDF;
double gunRotationAmount;
double gunRotationAmountINRedian;
vector<struct point> gunShots;


double rotationThreshHold = 45;

//drawss variables
double sphereRadius = 40;
double halfSphereRadius = 20;
double slices = 50;
double stacks = 50;
double cylinderHeight = 100;
double canonMouthHeight = 5;
int squareDistance = 600;
int squareLength = 300;
int bulletWidth = 5;


struct point
{
	double x,y,z;
};


struct transformations
{
    vector<vector<int>> operations;
};


struct point pos;
struct point u;
struct point l;
struct point r;
struct point u1;
struct point l1;
struct point r1;
struct point u2;
struct point l2;
struct point r2;
vector<transformations> bullets;

struct point vectorSum(struct point p1,struct point p2)
{
    struct point ans;
    ans.x = p1.x+p2.x;
    ans.y = p1.y+p2.y;
    ans.z = p1.z+p2.z;
    return ans;

}

struct point negateVector(struct point p1)
{
    struct point ans;
    ans.x = -p1.x;
    ans.y = -p1.y;
    ans.z = -p1.z;
    return ans;

}

struct point normalizeTOUnitVector(struct point p)
{
    double divisor = sqrt(p.x*p.x+p.y*p.y+p.z*p.z);
    p.x /= divisor;
    p.y /= divisor;
    p.z /= divisor;
    return p;
}

struct point rotateVector(struct point p1, struct point p2, double angle)
{
    struct point ans;

    ans.x = p1.x * cos(angle) + p2.x * sin(angle);
    ans.y = p1.y * cos(angle) + p2.y * sin(angle);
    ans.z = p1.z * cos(angle) + p2.z * sin(angle);

    double magnitude = sqrt(ans.x*ans.x+ans.y*ans.y+ans.z*ans.z);
    ans.x /= magnitude;
    ans.y /= magnitude;
    ans.z /= magnitude;

    return normalizeTOUnitVector(ans);


}

void printPoint(struct point p)
{

    cout<<p.x<<" "<<p.y<<" "<<p.z<<endl;
}

double getAngle(struct point p)
{
    double angle,x,y;
    x = p.x;
    y = p.y;
    if(x>0 && y>0) // first quad
    {
        angle = atan(y/x) * 180 / pi;

    }
    else if(x<0 && y>0) // first quad
    {
        angle =180 -  atan(y/-x) * 180 / pi;

    }
    else if(x<0 && y<0) // first quad
    {
        angle =180 +  atan(y/x) * 180 / pi;

    }
    else if(x>0 && y<0) // first quad
    {
        angle =360 -  atan(-y/x) * 180 / pi;

    }
    else if(x==0) // first quad
    {
        if(y>=0)
        {
            angle = 90;
        }
        else
        {
            angle = 270;
        }

    }
    else
    {
        if(x>=0)
        {
            angle = 0;
        }
        else
        {
            angle = 180;
        }
    }
    return angle;
}



void drawCylinder(double radius,double height,int slices,int stacks)
{
    struct point points[100][100];
	int i,j;
	double h;
	//generate points
	int r = radius;
	for(i=0;i<=stacks;i++)
	{
		h=height/stacks*i;

		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}

	for(i=0;i<stacks;i++)
	{
//        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
		    if(j%2==0)
            {
                glColor3f(1,1,1);
            }
            else
            {

                glColor3f(0,0,0);
            }
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}

}

void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
		    glColor3f(1.0, 0, 0);
			glVertex3f( 1000,0,0);
			glVertex3f( 0,0,0);
			glColor3f(0.25, 0, 0);
			glVertex3f( 0,0,0);
			glVertex3f(-1000,0,0);
            glColor3f(0, 0.25, 0);
			glVertex3f(0,-1000,0);
			glVertex3f( 0,0,0);
			glColor3f(0, 1, 0);
			glVertex3f( 0,0,0);
			glVertex3f(0, 1000,0);

            glColor3f(0, 0, 1.0);
			glVertex3f(0,0, 1000);
			glVertex3f( 0,0,0);
			glColor3f(0, 0.5, 0.5);
			glVertex3f( 0,0,0);
			glVertex3f(0,0,-1000);
		}glEnd();
	}
}

void drawSecondaryAxes()
{
    if(secondaryAxes==1)
    {
        glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
		    glColor3f(1.0, 0, 0);
			glVertex3f( 1000,0,0);
			glVertex3f( 0,0,0);
			glColor3f(0.25, 0, 0);
			glVertex3f( 0,0,0);
			glVertex3f(-1000,0,0);
            glColor3f(0, 0.25, 0);
			glVertex3f(0,-1000,0);
			glVertex3f( 0,0,0);
			glColor3f(0, 1, 0);
			glVertex3f( 0,0,0);
			glVertex3f(0, 1000,0);

            glColor3f(0, 0, 1.0);
			glVertex3f(0,0, 1000);
			glVertex3f( 0,0,0);
			glColor3f(0, 0.5, 0.5);
			glVertex3f( 0,0,0);
			glVertex3f(0,0,-1000);
		}glEnd();

    }


}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,2);
		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);
	}glEnd();
}


void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}





void drawCanonMouth(double radius,int slices,int stacks,double height)
{
    struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=height/stacks*i;
		r=radius+h*h/4;
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points

	for(i=0;i<stacks;i++)
	{
//        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
		    if(j%2==0)
            {
                glColor3f(1,1,1);
            }
            else
            {

                glColor3f(0,0,0);
            }
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
//                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
//				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
//				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
//				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points

	for(i=0;i<stacks;i++)
	{
//        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
		    if(j%2==0)
            {
                glColor3f(1,1,1);
            }
            else
            {

                glColor3f(0,0,0);
            }
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}


void drawHalfSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points

	for(i=0;i<stacks;i++)
	{
//        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
		    if(j%2==0)
            {
                glColor3f(1,1,1);
            }
            else
            {

                glColor3f(0,0,0);
            }
			glBegin(GL_QUADS);{
			    //upper hemisphere

				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
//                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
//				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
//				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
//				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}


void drawHalfSphere2(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=-h;
		}
	}
	//draw quads using generated points

	for(i=0;i<stacks;i++)
	{
//        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
		    if(j%2==0)
            {
                glColor3f(1,1,1);
            }
            else
            {

                glColor3f(0,0,0);
            }
			glBegin(GL_QUADS);{
			    //upper hemisphere

				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
//                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
//				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
//				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
//				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}


void drawSS()
{


    glPushMatrix();







    glRotatef(90,0,1,0);
    glRotatef(angleQW,0,1,0);

    drawHalfSphere2(sphereRadius,slices,stacks);
    glRotatef(angleER,1,0,0);
    drawHalfSphere(sphereRadius,slices,stacks);


    glRotatef(-90,0,1,0);
    glTranslatef(sphereRadius,0,0);
    glRotatef(angleAS,0,0,-1);
    glRotatef(angleDF,-1,0,0);
//    drawSecondaryAxes();
    glTranslatef(halfSphereRadius,0,0);
    glRotatef(-90,0,1,0);

    drawHalfSphere(halfSphereRadius,slices,slices);




    glTranslatef(0,0,-cylinderHeight);

    drawCylinder(halfSphereRadius,cylinderHeight,slices,slices);

    glTranslatef(0,0,-cylinderHeight);

    glRotatef(180,0,1,0);





    drawCanonMouth(halfSphereRadius,slices,stacks,canonMouthHeight);



    glPopMatrix();

    // drawing bullets in system 2
    glColor3f(1,0,0);
    for(int i=0;i<bullets.size();i++)
    {
        glPushMatrix();
        vector<vector<int>> &t = bullets[i].operations;
        for(int j=0;j<t.size();j++)
        {
            vector<int> &r = t[j];
            if(r.size()==4)
            {
                glRotatef(r[0],r[1],r[2],r[3]);
            }
            else if(r.size()==3)
            {
                glTranslatef(r[0],r[1],r[2]);
            }
        }

        glTranslatef(0,0,-440);
        drawSquare(5);
        glPopMatrix();
    }



    glTranslatef(squareDistance,0,0);
    glRotatef(90,0,1,0);
    glColor3f(0,1,0);
    drawSquare(squareLength);

    glRotatef(-90,0,1,0);
    glTranslatef(-squareDistance,0,0);



    glPopMatrix();

    //***************** drawing bullet system1 *************************

//    glColor3f(1,1,1);
//    glBegin(GL_LINES);
//    {
//        glVertex3f(0,0,-200);
//        glVertex3f(0+l1.x*50,0+l1.y*50,-200+l1.z*50);
//    }
//    glEnd();
//    for(int i=0;i<gunShots.size();i++)
//    {
//        struct point p2 = gunShots[i];
//        glColor3f(1,0,0);
//
//        glBegin(GL_QUADS);
//        {
//            glVertex3f(p2.x,p2.y+bulletWidth,p2.z+bulletWidth);
//            glVertex3f(p2.x,p2.y+bulletWidth,p2.z-bulletWidth);
//            glVertex3f(p2.x,p2.y-bulletWidth,p2.z-bulletWidth);
//            glVertex3f(p2.x,p2.y-bulletWidth,p2.z+bulletWidth);
//        }
//        glEnd();
//    }
////    struct point p2 = {sphereRadius*l2.x,sphereRadius*l2.y,sphereRadius*l2.z};
//    struct point p2 = {sphereRadius,0,0};
//    int scallingFactor = squareDistance-sphereRadius;
//    glColor3f(1,0,0);
//    glBegin(GL_LINES);
//    {
//        glVertex3f(p2.x,p2.y,p2.z);
//        glVertex3f(p2.x+l1.x*scallingFactor,p2.y+l1.y*scallingFactor,p2.z+l1.z*scallingFactor);
//    }
//    glEnd();




}

void keyboardListener(unsigned char key, int x,int y){
    struct point prevLookVector = l;
    struct point prevUpVector = u;
    struct point prevLook1Vector = l1;
    struct point prevUp1Vector = u1;
    struct point prevLook2Vector = l2;
    struct point prevUp2Vector = u2;
//    cout<<key<<endl;
//    struct point prevRightVector = r;
	switch(key){

		case '1':
//			drawgrid=1-drawgrid;
//            printPoint(l);
            l = rotateVector(l,negateVector(r),rotationAmount);
            r = rotateVector(r,prevLookVector,rotationAmount);
//            printPoint(l);
//            cout<<"hello"<<endl;
			break;

        case '2':
//			drawgrid=1-drawgrid;
            l = rotateVector(l,r,rotationAmount);
            r = rotateVector(r,negateVector(prevLookVector),rotationAmount);
			break;

        case '3':
//			drawgrid=1-drawgrid;
            l = rotateVector(l,u,rotationAmount);
            u = rotateVector(u,negateVector(prevLookVector),rotationAmount);
			break;

        case '4':
//			drawgrid=1-drawgrid;
            l = rotateVector(l,negateVector(u),rotationAmount);
            u = rotateVector(u,prevLookVector,rotationAmount);
			break;


        case '5':
//			drawgrid=1-drawgrid;
            u = rotateVector(u,negateVector(r),rotationAmount);
            r = rotateVector(r,prevUpVector,rotationAmount);
			break;

        case '6':
//			drawgrid=1-drawgrid;
            u = rotateVector(u,r,rotationAmount);
            r = rotateVector(r,negateVector(prevUpVector),rotationAmount);
            break;

        case 'x':
            secondaryAxes = 1-secondaryAxes;
            break;

        case 'q':
            if(angleQW+gunRotationAmount<=rotationThreshHold)
            {
                angleQW += gunRotationAmount;
                l1 = rotateVector(l1,negateVector(r1),gunRotationAmountINRedian);
                r1 = rotateVector(r1,prevLook1Vector,gunRotationAmountINRedian);
                l2 = rotateVector(l2,negateVector(r2),gunRotationAmountINRedian);
                r2 = rotateVector(r2,prevLook2Vector,gunRotationAmountINRedian);
            }



            break;

        case 'w':
            if(angleQW-gunRotationAmount>=-rotationThreshHold)
            {
                angleQW -= gunRotationAmount;
                l1 = rotateVector(l1,r1,gunRotationAmountINRedian);
                r1 = rotateVector(r1,negateVector(prevLook1Vector),gunRotationAmountINRedian);
                l2 = rotateVector(l2,r2,gunRotationAmountINRedian);
                r2 = rotateVector(r2,negateVector(prevLook2Vector),gunRotationAmountINRedian);
//                printPoint(l1);

            }


            break;

        case 'e':
            if(angleER-gunRotationAmount>=-rotationThreshHold)
            {
                angleER -= gunRotationAmount;
                l1 = rotateVector(l1,u1,gunRotationAmountINRedian);
                u1 = rotateVector(u1,negateVector(prevLook1Vector),gunRotationAmountINRedian);
                l2 = rotateVector(l2,u2,gunRotationAmountINRedian);
                u2 = rotateVector(u2,negateVector(prevLook2Vector),gunRotationAmountINRedian);
            }



            break;

        case 'r':
            if(angleER+gunRotationAmount<=rotationThreshHold)
            {
                angleER += gunRotationAmount;
                l1 = rotateVector(l1,negateVector(u1),gunRotationAmountINRedian);
                u1 = rotateVector(u1,prevLook1Vector,gunRotationAmountINRedian);
                l2 = rotateVector(l2,negateVector(u2),gunRotationAmountINRedian);
                u2 = rotateVector(u2,prevLook2Vector,gunRotationAmountINRedian);

            }

            break;

        case 'a':
            if(angleAS-gunRotationAmount>=-rotationThreshHold)
            {
                angleAS -= gunRotationAmount;
                l1 = rotateVector(l1,u1,gunRotationAmountINRedian);
                u1 = rotateVector(u1,negateVector(prevLook1Vector),gunRotationAmountINRedian);
            }

            break;

        case 's':
            if(angleAS+gunRotationAmount<=rotationThreshHold)
            {
                angleAS += gunRotationAmount;
                l1 = rotateVector(l1,negateVector(u1),gunRotationAmountINRedian);
                u1 = rotateVector(u1,prevLook1Vector,gunRotationAmountINRedian);
            }

            break;

        case 'd':
            if(angleDF-gunRotationAmount>=-rotationThreshHold)
            {
                angleDF -= gunRotationAmount;
            }

            break;
        case 'f':
            if(angleDF+gunRotationAmount<=rotationThreshHold)
            {
                angleDF += gunRotationAmount;
            }

            break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
//			cameraHeight -= 3.0;

            pos = vectorSum(pos,negateVector(l));
			break;
		case GLUT_KEY_UP:		// up arrow key
//			cameraHeight += 3.0;
            pos = vectorSum(pos,l);
			break;

		case GLUT_KEY_RIGHT:
//			cameraAngle += 0.03;
            pos = vectorSum(pos,r);
			break;
		case GLUT_KEY_LEFT:
//			cameraAngle -= 0.03;
            pos = vectorSum(pos,negateVector(r));
			break;

		case GLUT_KEY_PAGE_UP:
		    pos = vectorSum(pos,u);
			break;
		case GLUT_KEY_PAGE_DOWN:
            pos = vectorSum(pos,negateVector(u));
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:

			if(state == GLUT_DOWN){	// 2 times?? in ONE click? -- solution is checking DOWN or UP
				double x = squareDistance;
				struct point p2 = {sphereRadius,0,0};
				double t = (x-p2.x)/l1.x;
				double y = p2.y + t * l1.y;
				double z = p2.z + t * l1.z;
//				cout<<y<<" "<<z<<endl;
				if(y<=squareLength && y>=-squareLength && z<=squareLength && z>=-squareLength)
                {
//                    cout<<"hit"<<endl;
                    gunShots.push_back({x,y,z});   // *****************for system 1
//                    vector<vector<int>> operations;
                    struct transformations t;
                    t.operations.push_back({90,0,1,0});
                    t.operations.push_back({angleQW,0,1,0});
                    t.operations.push_back({angleER,1,0,0});
                    t.operations.push_back({-90,0,1,0});
                    t.operations.push_back({sphereRadius,0,0});
                    t.operations.push_back({angleAS,0,0,-1});
                    t.operations.push_back({angleDF,-1,0,0});
                    t.operations.push_back({halfSphereRadius,0,0});
                    t.operations.push_back({-90,0,1,0});
                    t.operations.push_back({0,0,-cylinderHeight});


                    bullets.push_back(t); // ********************for system 2

                }


			}
			break;

		case GLUT_RIGHT_BUTTON:
		    if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}

			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
//	gluLookAt(0,0,200,	0,0,0,	0,1,0);
	gluLookAt(pos.x,pos.y,pos.z,	pos.x+l.x,pos.y+l.y,pos.z+l.z,	u.x,u.y,u.z);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
//	drawGrid();

    //glColor3f(1,0,0);
    //drawSquare(10);

    drawSS();

    //drawCircle(30,24);

    //drawCone(20,50,24);

	//drawSphere(30,24,20);




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
//	angle+=0.5;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
//	drawgrid=0;
//	drawaxes=1;
//	cameraHeight=150.0;
//	cameraAngle=1.0;
//	angle=0;
    u = {0,1,0};
    r = {0,0,1};
    l = {1,0,0};

    u1 = {0,1,0};
    l1 = {1,0,0};
    r1 = {0,0,1};


    u2 = {0,1,0};
    l2 = {1,0,0};
    r2 = {0,0,1};

//    printPoint(l);
//    printf("%d %d %d",1,1,2);
    pos = {-300,100,0};
    drawaxes = 1;
    // shiftingAmount = 5;
    rotationAmount = (pi/18); // 10 degree written in redian
    // xAmount = 0;
    // yAmount = 0;
    // zAmount = 0;
    secondaryAxes = 1;
    angleQW = 0;
    angleER = 0;
    angleAS = 0;
    angleDF = 0;
    gunRotationAmount = 5; // 5 degree
    gunRotationAmountINRedian = (pi/18) /2 ; // 5 degree in redian  0.08715


	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	2000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
