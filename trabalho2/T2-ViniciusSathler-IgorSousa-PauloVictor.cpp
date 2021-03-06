/*
 * spider.cpp
 *
 *  Created on: 23 de out. de 2020
 *      Author: Paulo Victor, Vinicius Sathler, Igor Silva
 */

#include <bits/stdc++.h>

#include <GL/glut.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



#define BLUE 0, 0, 1
#define RED 1, 0, 0
#define GREEN 0, 1, 0
#define CYAN 0, 1, 1
#define YELLOW 1, 1, 0
#define BLACK 0, 0, 0
#define WHITE 1, 1, 1

#define NOCOLOR -1

using namespace std;

enum Estados{
	P1, P2, P3
};

enum Pernas{
	L1, L2, L3, L4, L5, L6, L7, L8
};

enum Dados{
	APOS, OPOS, INCPOS, DESVPOS, LEN1, INCART, LEN2
};

GLdouble const apos = 30;

GLdouble Val[8][3][7] = {
    //LEN1
    {
		//p1
		{
			//APOS, OPOS, INCPOS, DESVPOS, LEN1, INCART, LEN2
			apos, -60, 45, 0, 0.25, 90, 0
		},
		//p2
		{
			0, 0, 50, -10, 0, 0, 0
		},
		//p3
		{
			0, 0, 20, 10, 0, 0, 0
		}
	},
	//l2
	{
		//p1
		{
			//APOS, OPOS, INCPOS, DESVPOS, LEN1, INCART, LEN2
			apos, -20, 45, 0, 0.2, 60, 0
		},
		//p2
		{
			0, 0, 50, 10, 0, 0, 0
		},
		//p3
		{
			0, 0, 45, 0, 0, 0, 0
		}
	},
	//l3
	{
		//p1
		{
			//APOS, OPOS, INCPOS, DESVPOS, LEN1, INCART, LEN2
			apos, 20, 45, 0, 0.2, 60, 0
		},
		//p2
		{
			0, 0, 45, 0, 0, 0, 0
		},
		//p3
		{
			0, 0, 50, 10, 0, 0, 0
		}
	},
	//l4
	{
		//p1
		{
			//APOS, OPOS, INCPOS, DESVPOS, LEN1, INCART, LEN2
			apos, 45, 45, 0, 0.2, 60, 0
		},
		//p2
		{
			0, 0, 50, 10, 0, 0, 0
		},
		//p3
		{
			0, 0, 45, 0, 0, 0, 0
		}
	},
	//l5
	{
		//p1
		{
			//APOS, OPOS, INCPOS, DESVPOS, LEN1, INCART, LEN2
			apos, -120, 45, 0, 0.25, 90, 0
		},
		//p2
		{
			0, 0, 20, -10, 0, 0, 0
		},
		//p3
		{
			0, 0, 50, 10, 0, 0, 0
		}
	},
	//l6
	{
		//p1
		{
			//APOS, OPOS, INCPOS, DESVPOS, LEN1, INCART, LEN2
			apos, -160, 45, 0, 0.2, 60, 0
		},
		//p2
		{
			0, 0, 45, 0, 0, 0, 0
		},
		//p3
		{
			0, 0, 50, -10, 0, 0, 0
		}
	},
	//l7
	{
		//p1
		{
			//APOS, OPOS, INCPOS, DESVPOS, LEN1, INCART, LEN2
			apos, -200, 45, 0, 0.2, 60, 0
		},
		//p2
		{
			0, 0, 50, -10, 0, 0, 0
		},
		//p3
		{
			0, 0, 45, 0, 0, 0, 0
		}
	},
	//l8
	{
		//p1
		{
			//APOS, OPOS, INCPOS, DESVPOS, LEN1, INCART, LEN2
			apos, -225, 45, 0, 0.2, 60, 0
		},
		//p2
		{
			0, 0, 45, 0, 0, 0, 0
		},
		//p3
		{
			0, 0, 50, -10, 0, 0, 0
		}
	},
};

map<Pernas, int> M = {
	{Pernas::L1, 0},
	{Pernas::L2, 1},
	{Pernas::L3, 0},
	{Pernas::L4, 1},
	{Pernas::L5, 1},
	{Pernas::L6, 0},
	{Pernas::L7, 1},
	{Pernas::L8, 0}
};


Estados estado;
int winHeight = 800, winWidth = 800, stacks = 500;
GLdouble r_torax = 0.2, r_abd = 0.3, r_eye = 0.05, h_spider = 0.1, vel = 0.2;
GLint direcao;
pair<GLdouble, GLdouble> pos;

GLuint texID[4];  // Texture ID's for the three textures.
char* textureFileNames[4] = {   // file names for the files from which texture images are loaded
            "textures/spider_fur.jpg",
			"textures/walltxt.jpg",
			"textures/floortxt.png",
			"textures/spider_belly.jpg"
       };
GLUquadricObj *quadricObj = gluNewQuadric();

void calcLegs(){
	GLdouble l1, inc_articulacao, inc_pos, pos_alpha, pos_omega, h_pos, hl, bl, l2, ol;
	for(int i=Pernas::L1; i <= Pernas::L8; i++){
		l1 = Val[i][Estados::P1][Dados::LEN1];
		inc_articulacao = Val[i][Estados::P1][Dados::INCART];
		inc_pos = Val[i][Estados::P1][Dados::INCPOS];
		pos_alpha = Val[i][Estados::P1][Dados::APOS];
		pos_omega = Val[i][Estados::P1][Dados::OPOS];
		h_pos = h_spider + (r_abd-r_torax) + r_torax*(1 - cos((90-pos_alpha)*M_PI/180.0));
		hl = l1*(sin(inc_pos*(M_PI/180.0)));
		bl = inc_pos + inc_articulacao - 90;
		l2 = (hl + h_pos)/(cos(bl*(M_PI/180.0)));
		Val[i][Estados::P1][Dados::LEN2] = l2;

		for(int j = Estados::P2; j <= Estados::P3; j++){
			inc_pos = Val[i][j][Dados::INCPOS];
			ol = (asin((h_pos + sin((inc_pos)*M_PI/180.0)*l1)/l2))*(180/M_PI);

			inc_articulacao = 180 - inc_pos - ol;

			if(M[Pernas(i)] == j%2){
				inc_articulacao += 20;
			}

			Val[i][j][Dados::INCART] = inc_articulacao;

		}
	}

	for(int i=Pernas::L1; i <= Pernas::L8; i++){
		for(int j = Estados::P2; j <= Estados::P3; j++){
			Val[i][j][Dados::LEN1] = Val[i][Estados::P1][Dados::LEN1];
			Val[i][j][Dados::LEN2] = Val[i][Estados::P1][Dados::LEN2];
			Val[i][j][Dados::APOS] = Val[i][Estados::P1][Dados::APOS];
			Val[i][j][Dados::OPOS] = Val[i][Estados::P1][Dados::OPOS];
		}
	}

}


void init(){
	estado = Estados::P1;
	direcao = 0;
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	calcLegs();
	glClearColor(1.0, 1.0, 1.0, 1.0);
//	glMatrixMode(GL_PROJECTION);
  //  glLoadIdentity();
//	glOrtho(-2, 2, -2, 2, -2, 2);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void drawScenario(){
	GLdouble mx, rx, dx, my, ry, dy;
	mx = pos.first;
	while(mx >= 4) mx -=4;
	while(mx < 0) mx += 4;
	rx = mx/4;
	dx = 4*(1-rx);

	my = pos.second;
	while(my >= 4) my -=4;
	while(my < 0) my += 4;
	ry = my/4;
	dy = 4*(1-ry);

	glPushMatrix();
        glBindTexture( GL_TEXTURE_2D, texID[2] );  // Bind texture #0 for use on the spider body.
        glColor3f(WHITE);
		glTranslated(0, 0, -max(r_abd, r_torax) - h_spider);
		
		//chao
		
		glColor3f(WHITE);
		glBegin(GL_QUADS);
			glTexCoord2d(rx, ry);
			glVertex3d(-2, -2, 0);
			glTexCoord2d(1, ry);
			glVertex3d(dx-2, -2, 0);
			glTexCoord2d(1, 1);
			glVertex3d(dx-2, dy-2, 0);
			glTexCoord2d(rx, 1);
			glVertex3d(-2, dy-2, 0);
		glEnd();

		glBegin(GL_QUADS);
			glTexCoord2d(0, ry);
			glVertex3d(dx-2, -2, 0);
			glTexCoord2d(rx, ry);
			glVertex3d(2, -2, 0);
			glTexCoord2d(rx, 1);
			glVertex3d(2, dy-2, 0);
			glTexCoord2d(0, 1);
			glVertex3d(dx-2, dy-2, 0);
		glEnd();

		glBegin(GL_QUADS);
			glTexCoord2d(rx, 0);
			glVertex3d(-2, dy-2, 0);
			glTexCoord2d(1, 0);
			glVertex3d(dx-2, dy-2, 0);
			glTexCoord2d(1, ry);
			glVertex3d(dx-2, 2, 0);
			glTexCoord2d(rx, ry);
			glVertex3d(-2, 2, 0);
		glEnd();

		glBegin(GL_QUADS);
			glTexCoord2d(0, 0);
			glVertex3d(dx-2, dy-2, 0);
			glTexCoord2d(rx, 0);
			glVertex3d(2, dy-2, 0);
			glTexCoord2d(rx, ry);
			glVertex3d(2, 2, 0);
			glTexCoord2d(0, ry);
			glVertex3d(dx-2, 2, 0);
		glEnd();

		
		
		glBindTexture( GL_TEXTURE_2D, texID[1] );  // Bind texture #0 for use on the spider body.
        glColor3f(WHITE);

		//parede 0 1
		
		glBegin(GL_QUADS);
			glTexCoord2d(rx, 0);
			glVertex3d(-2, 2, 0);
			glTexCoord2d(rx, 1);
			glVertex3d(-2, 2, 2);
			glTexCoord2d(1, 1);
			glVertex3d(dx-2, 2, 2);
			glTexCoord2d(1, 0);
			glVertex3d(dx-2, 2, 0);
		glEnd();
		
		glBegin(GL_QUADS);
			glTexCoord2d(0, 0);
			glVertex3d(dx-2, 2, 0);
			glTexCoord2d(0, 1);
			glVertex3d(dx-2, 2, 2);
			glTexCoord2d(rx, 1);
			glVertex3d(2, 2, 2);
			glTexCoord2d(rx, 0);
			glVertex3d(2, 2, 0);
		glEnd();
		
		//parede 0 -1
		glBegin(GL_QUADS);
			glTexCoord2d(rx, 0);
			glVertex3d(-2, -2, 0);
			glTexCoord2d(rx, 1);
			glVertex3d(-2, -2, 2);
			glTexCoord2d(1, 1);
			glVertex3d(dx-2, -2, 2);
			glTexCoord2d(1, 0);
			glVertex3d(dx-2, -2, 0);
		glEnd();
		
		glBegin(GL_QUADS);
			glTexCoord2d(0, 0);
			glVertex3d(dx-2, -2, 0);
			glTexCoord2d(0, 1);
			glVertex3d(dx-2, -2, 2);
			glTexCoord2d(rx, 1);
			glVertex3d(2, -2, 2);
			glTexCoord2d(rx, 0);
			glVertex3d(2, -2, 0);
		glEnd();

		
		//parede -1 0

		glBegin(GL_QUADS);
			glTexCoord2d(ry, 0);
			glVertex3d(-2, -2, 0);
			glTexCoord2d(ry, 1);
			glVertex3d(-2, -2, 2);
			glTexCoord2d(1, 1);
			glVertex3d(-2, dy-2, 2);
			glTexCoord2d(1, 0);
			glVertex3d(-2, dy-2, 0);
		glEnd();
		
		glBegin(GL_QUADS);
			glTexCoord2d(0, 0);
			glVertex3d(-2, dy-2, 0);
			glTexCoord2d(0, 1);
			glVertex3d(-2, dy-2, 2);
			glTexCoord2d(ry, 1);
			glVertex3d(-2, 2, 2);
			glTexCoord2d(ry, 0);
			glVertex3d(-2, 2, 0);
		glEnd();

		// parede 1 0

		glBegin(GL_QUADS);
			glTexCoord2d(ry, 0);
			glVertex3d(2, -2, 0);
			glTexCoord2d(ry, 1);
			glVertex3d(2, -2, 2);
			glTexCoord2d(1, 1);
			glVertex3d(2, dy-2, 2);
			glTexCoord2d(1, 0);
			glVertex3d(2, dy-2, 0);
		glEnd();
		
		glBegin(GL_QUADS);
			glTexCoord2d(0, 0);
			glVertex3d(2, dy-2, 0);
			glTexCoord2d(0, 1);
			glVertex3d(2, dy-2, 2);
			glTexCoord2d(ry, 1);
			glVertex3d(2, 2, 2);
			glTexCoord2d(ry, 0);
			glVertex3d(2, 2, 0);
		glEnd();
		
		
	glPopMatrix();
}

void drawLeg(Pernas perna){
    GLdouble l1 = Val[perna][estado][Dados::LEN1],
	inc_articulacao = Val[perna][estado][Dados::INCART], 
	inc_pos = Val[perna][estado][Dados::INCPOS],
	pos_alpha = Val[perna][estado][Dados::APOS],
	pos_omega = Val[perna][estado][Dados::OPOS],
	l2 = Val[perna][estado][Dados::LEN2],
	desv_pos = Val[perna][estado][Dados::DESVPOS];

	//cout << l2 << endl;

    glPushMatrix();
    glRotated(pos_omega, 0, 0, 1.0);
    glRotated(pos_alpha, 0, 1.0, 0);
    glTranslatef(-r_torax, 0, 0);

	glRotated(-pos_alpha, 0, 1.0, 0);
	glRotated(-desv_pos, 0, 0, 1.0);
	glRotated(-inc_pos, 0, 1.0, 0);
    glTranslated(-l1, 0, 0);
    glRotated(180-inc_articulacao, 0, 1.0, 0);
    glTranslated(-l2/2, 0, 0);
    glPushMatrix();
        glScaled(l2, 0.02, 0.02);
        glutSolidCube(1.0);
    glPopMatrix();
    glTranslated(l2/2, 0, 0);
    glRotated(-(180-inc_articulacao), 0, 1.0, 0);
    glTranslated(l1/2, 0, 0);
    glPushMatrix();
        glScaled(l1, 0.02, 0.02);
        glutSolidCube(1.0);
    glPopMatrix();
    glTranslated(l1/2, 0, 0);

    glRotated(inc_pos, 0, 1.0, 0);
	glRotated(desv_pos, 0, 0, 1.0);
	glRotated(pos_alpha, 0, 1.0, 0);
    
	glTranslated(r_torax, 0, 0);
    
	glRotated(-pos_alpha, 0, 1.0, 0);
    glRotated(-pos_omega, 0, 0, 1.0);
    glPopMatrix();
}

void drawEyes(GLdouble alpha, GLdouble beta){
	glPushMatrix();
		glRotated(beta, 0, 0, 1.0);
		glRotated(alpha, 0, 1.0, 0);
		glTranslated(-r_torax, 0, 0);
		glColor3f(BLACK);
		glutSolidSphere(r_eye, 20, 20);
		glTranslated(r_torax, 0, 0);
		glRotated(alpha, 0, 1.0, 0);
		glRotated(beta, 0, 0, 1.0);
    glPopMatrix();
}

void drawSpiderBody(){
    glPushMatrix();
        //glEnable(GL_TEXTURE_2D);
          // Bind texture #0 for use on the spider body.
        glColor3f(WHITE);



		glRotated(direcao, 0, 0, 1.0);
        
		glRotated(180, 1.0, 0, 0);
		glRotated(180, 0, 0, 1.0);

		drawEyes(-30, -60);
		drawEyes(-30, -120);

        for(int i = Pernas::L1; i <= Pernas::L8; i++) drawLeg(Pernas(i));

		glColor3f(WHITE);

		glBindTexture( GL_TEXTURE_2D, texID[3] );
		gluSphere(quadricObj, r_torax, 20, 20);
		gluQuadricTexture(quadricObj, GL_TRUE);
		//glutWireSphere(r_torax, 20, 20);

        glTranslated(0, -r_abd-r_torax, 0);
        gluQuadricDrawStyle(quadricObj, GLU_FILL);
        
	
		//glutWireSphere(r_abd, 20, 20);
		
        gluSphere(quadricObj, r_abd, 20, 20);
        gluQuadricTexture(quadricObj, GL_TRUE);
        
    glPopMatrix();

}

void display(){
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
    glViewport(0, winHeight/2, winWidth/2, winHeight/2);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glFrustum(-2, 2, -2, 2, 0.4, 3);
	gluLookAt(0, 1.0, 0.5, 0, 0, -1, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	drawSpiderBody();
    drawScenario();
    
    glViewport(winWidth/2, winHeight/2, winWidth/2, winHeight/2);
    glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glFrustum(-2, 2, -2, 2, 0.4, 3);
	gluLookAt(1.0, 0.0, 0.5, 0, 0, -1, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    drawSpiderBody();
	drawScenario();
    
    glViewport(0, 0, winHeight/2, winWidth/2);
    glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glFrustum(-2, 2, -2, 2, 0.4, 3);
	gluLookAt(0.7, 0.7, 0.5, 0, 0, -1, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	drawSpiderBody();
	drawScenario();

    glViewport(winWidth/2, 0,  winWidth/2, winHeight/2);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glFrustum(-2, 2, -2, 2, 1, 3);
	gluLookAt(0, 0, 2, 0, 0, -1, 0, 1, 0);
    drawSpiderBody();
	drawScenario();

    glFlush();
}

void loadTextures() {
	int width, height, nrChannels;
	unsigned char *data;

	glGenTextures(4, texID);

	for(int i = 0; i < 4;i++)
	{
        glBindTexture(GL_TEXTURE_2D, texID[i]);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load and generate the texture

        data = stbi_load(textureFileNames[i], &width, &height, &nrChannels, 0);

        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            //glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri  (GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE );
        }
        else
        {
            printf("Failed to load texture\n");
        }
        stbi_image_free(data);
	}
}

void keyboard(unsigned char key, int x, int y){
	switch(key){
		case '1':
			estado = Estados::P1;
			break;
		case '2':
			estado = Estados::P2;
			break;
		case '3':
			estado = Estados::P3;
			break;
	}
	glutPostRedisplay();
}

void specialKeyboard(int key, int x, int y){
	switch(key){
		case GLUT_KEY_UP:
			if(estado == Estados::P2) estado = Estados::P3;
			else estado = Estados::P2;
			pos.first = pos.first + vel*sin(((360-direcao)%360)*(M_PI/180));
			pos.second = pos.second + vel*cos(((360-direcao)%360)*(M_PI/180));
			break;
		case GLUT_KEY_DOWN:
			if(estado == Estados::P2) estado = Estados::P3;
			else estado = Estados::P2;
			pos.first = pos.first - vel*sin(((360-direcao)%360)*(M_PI/180));
			pos.second = pos.second - vel*cos(((360-direcao)%360)*(M_PI/180));
			break;
		case GLUT_KEY_RIGHT:
			estado = Estados::P1;
			direcao = ((int)direcao - 5)%360;
			break;
		case GLUT_KEY_LEFT:
			estado = Estados::P1;
			direcao = (direcao + 5)%360;
			break;
		default:
			estado = Estados::P1;
			break;
	}
	glutPostRedisplay();
}


int main(int argc, char** argv){
    glutInit(&argc, argv);      
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(200,200);
    glutInitWindowSize(winWidth,winHeight);
    glutCreateWindow("Trabalho 2");
    init();
    loadTextures();
    glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeyboard);
	//glDepthFunc(GL_ALWAYS);
    //glutMouseFunc(mouse);
    //glutTimerFunc(50, update, 0);
    glutMainLoop();
    return EXIT_SUCCESS;
}