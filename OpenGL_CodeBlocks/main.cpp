#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <iostream>
#include <windows.h>
#include <glut.h>
using namespace std;
#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
double shiftingAmount;
double xAmount;
double yAmount;
double zAmount;
double rotationAmount;

struct point
{
	double x,y,z;
};

struct point pos;
struct point u;
struct point l;
struct point r;

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

    return ans;


}

void printPoint(struct point p)
{

    cout<<p.x<<" "<<p.y<<" "<<p.z<<endl;
}

void counterClockRotateOFLookVectorINYAxis()
{
    struct point perpendicular;
    // i  j  k
    // l1 l2 l3
    // u1 u2 u3
    // = i*(-l3) - j(0) + k(l1)
    perpendicular.x = l.y*u.z - l.z*u.y;
    perpendicular.y = -(l.x*u.z-l.z*u.x);
    perpendicular.z = l.x*u.y-l.y*u.x;
    printPoint(l);
    l.x = l.x * cos(rotationAmount) + perpendicular.x * sin(rotationAmount);
    l.y = l.y * cos(rotationAmount) + perpendicular.y * sin(rotationAmount);
    l.z = l.z * cos(rotationAmount) + perpendicular.z * sin(rotationAmount);

    printPoint(l);

    double magnitude = sqrt(l.x*l.x+l.y*l.y+l.z*l.z);
    l.x /= magnitude;
    l.y /= magnitude;
    l.z /= magnitude;

    printPoint(l);
    cout<<endl;
}

void clockRotateOFLookVectorINYAxis()
{
    struct point perpendicular;
    // i  j  k
    // l1 l2 l3
    // 0  1  0
    // = i*(-l3) - j(0) + k(l1)
    perpendicular.x = -(l.y*u.z - l.z*u.y);
    perpendicular.y = (l.x*u.z-l.z*u.x);
    perpendicular.z = -(l.x*u.y-l.y*u.x);

    printPoint(l);

    l.x = l.x * cos(rotationAmount) + perpendicular.x * sin(rotationAmount);
    l.y = l.y * cos(rotationAmount) + perpendicular.y * sin(rotationAmount);
    l.z = l.z * cos(rotationAmount) + perpendicular.z * sin(rotationAmount);

    printPoint(l);

    double magnitude = sqrt(l.x*l.x+l.y*l.y+l.z*l.z);
    l.x /= magnitude;
    l.y /= magnitude;
    l.z /= magnitude;

    printPoint(l);
    cout<<endl;
}

void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
		    glColor3f(1.0, 0, 0);
			glVertex3f( 1000,0,0);
			glVertex3f(-1000,0,0);
            glColor3f(0, 1.0, 0);
			glVertex3f(0,-1000,0);
			glVertex3f(0, 1000,0);
            glColor3f(0, 0, 1.0);
			glVertex3f(0,0, 1000);
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
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
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


void drawSS()
{
//    glColor3f(1,0,0);
//    drawSquare(20);
//
//    glRotatef(angle,0,0,1);
//    glTranslatef(110,0,0);
//    glRotatef(2*angle,0,0,1);
//    glColor3f(0,1,0);
//    drawSquare(15);
//
//    glPushMatrix();
//    {
//        glRotatef(angle,0,0,1);
//        glTranslatef(60,0,0);
//        glRotatef(2*angle,0,0,1);
//        glColor3f(0,0,1);
//        drawSquare(10);
//    }
//    glPopMatrix();
//
//    glRotatef(3*angle,0,0,1);
//    glTranslatef(40,0,0);
//    glRotatef(4*angle,0,0,1);
//    glColor3f(1,1,0);
//    drawSquare(5);


//    glColor3f(1,0,0);
//    drawSphere(100,50,50);
}

void keyboardListener(unsigned char key, int x,int y){
    struct point prevLookVector = l;
    struct point prevUpVector = u;
//    struct point prevRightVector = r;
	switch(key){

		case '1':
//			drawgrid=1-drawgrid;

            l = rotateVector(l,negateVector(r),rotationAmount);
            r = rotateVector(r,prevLookVector,rotationAmount);
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
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
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
    r = {1,0,0};
    l = {0,0,-1};


//    printPoint(l);
//    printf("%d %d %d",1,1,2);
    pos = {0,0,100};
    drawaxes = 1;
    shiftingAmount = 5;
    rotationAmount = (pi/18);
    xAmount = 0;
    yAmount = 0;
    zAmount = 0;



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
	gluPerspective(80,	1,	1,	1000.0);
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
