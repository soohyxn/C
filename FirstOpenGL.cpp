#include <Windows.h>
#include <cstdio>
#include <gl/GLUT.H>
#include <cmath>

int numT;
int numV;

float* vx;
float* vy;
float* vz;

int* a;
int* b;
int* c;

float vx_max = -100000.0f;
float vy_max = -100000.0f;
float vz_max = -100000.0f;

float vx_min = 100000.0f;
float vy_min = 100000.0f;
float vz_min = 100000.0f;

float lx;
float ly;
float lz;
float lmax;

float xc;
float yc;
float zc;

float tx = 0;
float ty = 0;
float tz = 0;

float sx = 0;
float sy = 0;
float sz = 0;

float angle = 0;
float rx = 0;
float ry = 0;
float rz = 0;

unsigned char k;
unsigned char vertex;
unsigned char triangle;
unsigned char model;

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glScalef(1.0f / lmax, 1.0f / lmax, 1.0f / lmax);
	glTranslatef(-xc, -yc, -zc);
	
	switch (k)
	{
	case '4':
		glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
		break;
	case '5':
		gluPerspective(50, 10, -20, 20);
		break;
	case '6':
		glFrustum(-10.0, 10.0, -10.0, 10.0, 1.5, 20.0);
		break;
	}

	glShadeModel(model);

	glTranslatef(tx, ty, tz);
	glScalef(sx, sy, sz);
	glRotatef(angle, rx, ry, rz);

	glBegin(vertex);
	for (int Vidx = 0; Vidx < numV; Vidx++)
		glVertex3f(vx[Vidx], vy[Vidx], vz[Vidx]);
	glEnd();

	glBegin(triangle);
	for (int Tindex = 0; Tindex < numT; Tindex++)
	{
		glVertex3f(vx[a[Tindex]], vy[a[Tindex]], vz[a[Tindex]]);
		glVertex3f(vx[b[Tindex]], vy[b[Tindex]], vz[b[Tindex]]);
		glVertex3f(vx[c[Tindex]], vy[c[Tindex]], vz[c[Tindex]]);
	}
	glEnd();

	glutSwapBuffers();
}
void Mykeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '1':
		vertex = GL_POINT;
		triangle = NULL;
		break;
	case '2':
		vertex = NULL;
		triangle = GL_LINE_LOOP;
		break;
	case '3':
		vertex = NULL;
		triangle = GL_TRIANGLES;
		break;
	case 'f':
		vertex = NULL;
		triangle = GL_TRIANGLES;
		model = GL_FLAT;
		break;
	case 'v':
		vertex = NULL;
		triangle = GL_TRIANGLES;
		model = GL_SMOOTH;
		break;
	case '4':
		k = '4';
		break;
	case '5':
		k = '5';
		break;
	case '6':
		k = '6';
		break;
	case 'w':
		ty += 1.0f;
		break;
	case 's':
		ty -= 1.0f;
		break;
	case 'a':
		tx -= 1.0f;
		break;
	case 'd':
		tx += 1.0f;
		break;
	case '+':
		sx += 0.1f;
		sy += 0.1f;
		sz += 0.1f;
		break;
	case '-':
		sx -= 0.1f;
		sy -= 0.1f;
		sz -= 0.1f;
		break;
	case 'y':
		angle += 20;
		rx -= 1.0f;
		break;
	case 'b':
		angle += 20;
		rx += 1.0f;
		break;
	case 'g':
		angle += 20;
		ry -= 1.0f;
		break;
	case 'h':
		angle += 20;
		ry += 1.0f;
		break;
	case 'n':
		angle += 20;
		rz -= 1.0f;
		break;
	case 'm':
		angle += 20;
		rz += 1.0f;
		break;
	}
	glutPostRedisplay();
}
int main(int argc, char** argv)
{
	char str[255];
	int dummy;

	FILE* fp = fopen("c:\\what_is_this.off", "r");

	fscanf(fp, "%s ", str);
	fscanf(fp, "%d %d %d", &numV, &numT, &dummy);

	vx = new float[numV];
	vy = new float[numV];
	vz = new float[numV];

	a = new int[numT];
	b = new int[numT];
	c = new int[numT];

	int i;
	for (i = 0; i < numV; i++)
	{
		fscanf(fp, "%f %f %f", &vx[i], &vy[i], &vz[i]);

		if (vx[i] > vx_max)
			vx_max = vx[i];
		if (vx[i] < vx_min)
			vx_min = vx[i];
		if (vy[i] > vy_max)
			vy_max = vy[i];
		if (vy[i] < vy_min)
			vy_min = vy[i];
		if (vz[i] > vz_max)
			vz_max = vz[i];
		if (vz[i] < vz_min)
			vz_min = vz[i];
	}
	for (i = 0; i < numT; i++)
	{
		fscanf(fp, "%d %d %d %d",&dummy, &a[i], &b[i], &c[i]);
	}
	lx = vx_max - vx_min;
	ly = vy_max - vy_min;
	lz = vz_max - vz_min;
	lmax = max(max(lx, ly), lz);

	xc = (vx_max + vx_min) / 2.0f;
	yc = (vy_max + vy_min) / 2.0f;
	zc = (vz_max + vz_min) / 2.0f;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("example");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(Mykeyboard);
	glutMainLoop();
	return 0;
}