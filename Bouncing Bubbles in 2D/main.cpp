#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>
#include<ctime>
#include<bits/stdc++.h>
using namespace std;


#define pi (2*acos(0.0))


struct point
{
	double x,y,z;
};

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
double zCoord;
vector<struct point> bubbles;
int numberOfBubbles;  // total bubbles that would appear
int currentBubbles;  // currently appeared bubble
struct point startingPosition;
int startingTime;
vector<struct point> directionVectors;
int bubbleAppearingInterval;
double bubbleRadius;
double halfSquareLength;
vector<bool> insideCircle;
double innerCircleRedius;
int lastReflectionTime;
bool pause;
double speed;
double speedIncrement;

struct point reflect(struct point perpendicular, struct point directionVector)
{
    struct point newDirectionVector;
    double dotProduct = perpendicular.x*directionVector.x+perpendicular.y*directionVector.y+perpendicular.z*directionVector.z;
    newDirectionVector.x = directionVector.x - 2*(dotProduct)*perpendicular.x;
    newDirectionVector.y = directionVector.y - 2*(dotProduct)*perpendicular.y;
    newDirectionVector.z = directionVector.z - 2*(dotProduct)*perpendicular.z;
    return newDirectionVector;
}

struct point normalizeTOUnitVector(struct point p)
{
    double divisor = sqrt(p.x*p.x+p.y*p.y+p.z*p.z);
    struct point ret = {p.x/divisor,p.y/divisor,p.z/divisor};
    return ret;
}

double distance(struct point p1, struct point p2)
{
    return sqrt(pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2)+pow(p1.z-p2.z,2));

}

void printPoint(struct point p)
{
    double mod = sqrt(p.x*p.x+p.y*p.y+p.z*p.z);
    cout<<p.x<<" "<<p.y<<" "<<p.z<<" "<<mod<<endl;
}

void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
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



void drawCircle(double radius,int segments, struct point p)
{
    int i;
    struct point points[100];
//    glColor3f(0.7,0.7,0.7);
    //generate points
    double centerX = p.x;
    double centerY = p.y;
    for(i=0;i<=segments;i++)
    {
        points[i].x=centerX+radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=centerY+radius*sin(((double)i/(double)segments)*2*pi);
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

void drawCircle(double radius,int segments)
{
    drawCircle(radius, segments, {0,0,0});
//    int i;
//    struct point points[100];
//    glColor3f(1,1,1);
//    generate points
//
//    for(i=0;i<=segments;i++)
//    {
//        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
//        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
//    }
//    draw segments using generated points
//    for(i=0;i<segments;i++)
//    {
//        glBegin(GL_LINES);
//        {
//			glVertex3f(points[i].x,points[i].y,0);
//			glVertex3f(points[i+1].x,points[i+1].y,0);
//        }
//        glEnd();
//    }
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

    glPushMatrix();
    glColor3f(0,1,0);
    glBegin(GL_LINES);
    {
        glVertex3f(-halfSquareLength,halfSquareLength,zCoord);
        glVertex3f(halfSquareLength,halfSquareLength,zCoord);

        glVertex3f(-halfSquareLength,halfSquareLength,zCoord);
        glVertex3f(-halfSquareLength,-halfSquareLength,zCoord);

        glVertex3f(-halfSquareLength,-halfSquareLength,zCoord);
        glVertex3f(halfSquareLength,-halfSquareLength,zCoord);

        glVertex3f(halfSquareLength,-halfSquareLength,zCoord);
        glVertex3f(halfSquareLength,halfSquareLength,zCoord);
    }
    glEnd();
    glColor3f(1,0,0);
    drawCircle(innerCircleRedius,90);

//    drawCircle(10,10,{-100,-100});
    glColor3f(1,1,1);
    for(int i=0;i<currentBubbles;i++)
    {
        drawCircle(bubbleRadius,20,bubbles[i]);
    }




}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			drawgrid=1-drawgrid;
			break;

        case 'p':
            if(pause)
            {
                pause = false;
            }
            else
            {
                pause = true;
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
            if(speed>speedIncrement)
            {
                speed -= speedIncrement;
            }

			break;
		case GLUT_KEY_UP:		// up arrow key
//			cameraHeight += 3.0;
            if(speed<12*speedIncrement)
            {
                speed += speedIncrement;
            }
			break;

		case GLUT_KEY_RIGHT:
//			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
//			cameraAngle -= 0.03;
			break;

		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
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
//			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
//				drawaxes=1-drawaxes;
//			}
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
	gluLookAt(0,0,200,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

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
//	angle+=0.05;
    if(pause)
    {
        return;
    }
    if(currentBubbles<5)
    {
        int currentTime = time(0);
        if(currentTime-startingTime > bubbleAppearingInterval)
        {
            startingTime = currentTime;
//            cout<<currentTime<<endl;
            currentBubbles += 1;
        }
    }

    for(int i=0;i<currentBubbles;i++)
    {
        struct point p = bubbles[i];
        struct point directionVector = directionVectors[i];

        p.x += directionVector.x*speed;
        p.y += directionVector.y*speed;
        p.z += directionVector.z*speed;
        struct point newDirectionVector;
        struct point perpendicular;
        bool reflection = false;
        if(insideCircle[i]==false)
        {
            double distanceFromOrigin = distance(p,{0,0,0});
            if(distanceFromOrigin<innerCircleRedius-bubbleRadius)
            {
                insideCircle[i] = true;
            }
            else if(p.x+bubbleRadius>halfSquareLength && directionVector.x > 0)
            {

                perpendicular = {1,0,0};
                newDirectionVector = reflect(perpendicular,directionVector);
                newDirectionVector = normalizeTOUnitVector(newDirectionVector);
                reflection = true;
            }
            else if(p.x-bubbleRadius<(-halfSquareLength) && directionVector.x < 0)
            {
                perpendicular = {-1,0,0};
                newDirectionVector = reflect(perpendicular,directionVector);
                newDirectionVector = normalizeTOUnitVector(newDirectionVector);
                reflection = true;
            }
            else if(p.y+bubbleRadius>halfSquareLength && directionVector.y > 0)
            {
                perpendicular = {0,1,0};
                newDirectionVector = reflect(perpendicular,directionVector);
                newDirectionVector = normalizeTOUnitVector(newDirectionVector);
                reflection = true;
            }
            else if(p.y-bubbleRadius<(-halfSquareLength) && directionVector.y < 0)
            {
                perpendicular = {0,-1,0};
                newDirectionVector = reflect(perpendicular,directionVector);
                newDirectionVector = normalizeTOUnitVector(newDirectionVector);
                reflection = true;
            }
        }
        else
        {
            double distanceFromOrigin = distance(p,{0,0,0});
            if(distanceFromOrigin>innerCircleRedius-bubbleRadius)
            {


//                    lastReflectionTime = currentTime;
                    perpendicular = {-p.x,-p.y,-p.z};

                    perpendicular = normalizeTOUnitVector(perpendicular);


                    newDirectionVector = reflect(perpendicular,directionVector);
                    newDirectionVector = normalizeTOUnitVector(newDirectionVector);
                    reflection = true;




            }
        }
        if(reflection)
        {
            directionVectors[i] = newDirectionVector;
        }
        bubbles[i] = p;

    }
    // r = d-2(d*n)n;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=0;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;
	zCoord = 0;
    numberOfBubbles =5;
    startingPosition = {-100,-100,zCoord};
    startingTime = time(0);
    srand(time(0));
    bubbleAppearingInterval = 5; // in seconds
    bubbleRadius = 15;
    halfSquareLength = 120;
    innerCircleRedius = 90;
    lastReflectionTime = time(0);
    pause = false;
    speed = 1;
    speedIncrement = 0.25;

    for(int i=0;i<numberOfBubbles;i++)
    {
        bubbles.push_back(startingPosition);
        struct point temp = {rand(),rand(),zCoord};
        temp = normalizeTOUnitVector(temp);
        directionVectors.push_back(temp);
        insideCircle.push_back(false);
    }
    int currentBubbles = 0;

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
