
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <time.h>

//funkcje algebry liniowej
#include "..\glm/vec3.hpp" // glm::vec3
#include "..\glm/vec4.hpp" // glm::vec4
#include "..\glm/mat4x4.hpp" // glm::mat4
#include "..\glm/gtc/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::perspective


#include "..\GL\glew.h"
#include "..\GL\freeglut.h" 
#include "..\freeimage.h"

#include "shaderLoader.h" //narzêdzie do ³adowania i kompilowania shaderów z pliku

using namespace std;

// http://www.malgouyres.org/freeimagehowto zrodlo
char shaderV[] = " ";

char shaderF[] = " ";

typedef struct Point2 {
	float x;
	float y;
}Point2;

typedef struct Point3 {
	float x;
	float y;
	float z;
}Point3;

//macierze
glm::mat4 M; //model- macierz modelu 
glm::mat4 V; //view - macierz œwiata
glm::mat4 P;  //projection - macierz projekcji, czyli naszej perspektywy

//int screen_width = textureSize();
int screen_width = 1366;
int screen_height = 768;
typedef struct {
	GLsizei width;
	GLsizei height;
}GLTextureSize;

double kameraX = 0;
double kameraZ = 0;
double kameraD = -1000;
double kameraKat = 0;
Point2 posMouse = { 0,0 };
//VBO
GLuint vboId[3] = { 0 };
GLuint ilosc;
void tworzenieVBO();
GLuint programID = 0;
//textura
GLuint tempTextureID = 0;
GLuint texID;
//od pana z zajec
void setup() {
	glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
}
void rozmiar(int width, int height)
{
	screen_width = width;
	screen_height = height;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, screen_width, screen_height);
	P = glm::perspective(glm::radians(60.0f), (GLfloat)screen_width / (GLfloat)screen_height, 1.0f, 1000.0f);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluPerspective(60.0f, (GLfloat)screen_width / (GLfloat)screen_height, 1.0f, 1000.0f);

	glutPostRedisplay();
}


void punktyRamki(Point3 *wierzcholki) {
	wierzcholki[0].x = screen_width;
	wierzcholki[0].y = screen_height;
	wierzcholki[0].z = 0;
	wierzcholki[1].x = screen_width;
	wierzcholki[1].y = -screen_height;
	wierzcholki[1].z = 0;
	wierzcholki[2].x = -screen_width;
	wierzcholki[2].y = -screen_height;
	wierzcholki[2].z = 0;
	wierzcholki[3].x = -screen_width;
	wierzcholki[3].y = screen_height;
	wierzcholki[3].z = 0;
}
void kolorRamki(Point3 *kolory) {
	kolory[0].x = 1;
	kolory[0].y = 1;
	kolory[0].z = 1;
	kolory[1].x = 1;
	kolory[1].y = 1;
	kolory[1].z = 1;
	kolory[2].x = 1;
	kolory[2].y = 1;
	kolory[2].z = 1;
	kolory[3].x = 1;
	kolory[3].y = 1;
	kolory[3].z = 1;
}

void rysujVBO()
{
	glUseProgram(programID); //u¿yj programu, czyli naszego shadera   

	glm::mat4 MVP = P*V*M;
	GLuint MVP_id = glGetUniformLocation(programID, "MVP"); // pobierz lokalizacjê zmiennej 'uniform' "MV" w programie
	glUniformMatrix4fv(MVP_id, 1, GL_FALSE, &(MVP[0][0]));     // wyœlij tablicê mv do lokalizacji "MV", która jest typu mat4   

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);
	GLuint temp_id = glGetUniformLocation(programID, "texture");
	glUniform1i(temp_id, 1);
	temp_id = glGetUniformLocation(programID, "posCursor");
	glUniform2f(temp_id, posMouse.x, screen_height -1 - posMouse.y);
	temp_id = glGetUniformLocation(programID, "height");
	glUniform1f(temp_id, screen_height);
	temp_id = glGetUniformLocation(programID, "width");
	glUniform1f(temp_id, screen_width);
	glEnableVertexAttribArray(0); // pierwszy buform atrybuów: wierzcho³ki
	glBindBuffer(GL_ARRAY_BUFFER, vboId[0]);
	glVertexAttribPointer(
		0,                  // atrybut 0. musi odpowiadaæ atrybutowi w programie shader
		3,                  // wielkoœæ (x,y,z)
		GL_FLOAT,           // typ
		GL_FALSE,           // czy znormalizowany [0-1]?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glEnableVertexAttribArray(1); // drugi bufor atrybuów: kolory
	glBindBuffer(GL_ARRAY_BUFFER, vboId[1]);
	glVertexAttribPointer(
		1,                  // atrybut 0. musi odpowiadaæ atrybutowi w programie shader
		3,                  // wielkoœæ (x,y,z)
		GL_FLOAT,           // typ
		GL_FALSE,           // czy znormalizowany [0-1]?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(2); // drugi bufor atrybuów: kolory
	glBindBuffer(GL_ARRAY_BUFFER, vboId[2]);
	glVertexAttribPointer(
		1,                  // atrybut 0. musi odpowiadaæ atrybutowi w programie shader
		3,                  // wielkoœæ (x,y,z)
		GL_FLOAT,           // typ
		GL_FALSE,           // czy znormalizowany [0-1]?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glDrawArrays(GL_QUADS, 0, 4); // Zaczynamy od 0 i rysujemy wszystkie wierzcho³ki

	glDisableVertexAttribArray(0);  // wy³¹czamy u¿ywanie zdefiniowanych atrybutów 
	glUseProgram(0);                // wy³¹czamy u¿ywanie programu (teraz mo¿emy znów rysowaæ po staremu)
}

void display() {
	int i = 0;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Kasowanie ekranu

	V = glm::mat4(1.0f); //glLoadIdentity();
	V = glm::translate(V, glm::vec3(0, 0, kameraD)); //glTranslated(0.0f, 0.0f, kameraD);
													 //V = glm::rotate(V, (float)glm::radians(kameraZ), glm::vec3(1, 0, 0)); //glRotatef(kameraZ, 1.0, 0.0, 0.0);
													 //V = glm::rotate(V, (float)glm::radians(kameraX), glm::vec3(0, 1, 0)); //glRotatef(kameraX, 0.0, 1.0, 0.0);
	M = glm::mat4(1.0f); //glLoadIdentity();
	rysujVBO();

	glutReportErrors();
	//glFlush();
	glutSwapBuffers();
}


void tworzenieVBO() {
	Point3 *wierzcholki;
	Point3 *kolory;
	wierzcholki = (Point3*)malloc(4 * sizeof(Point3)); //alokujemy pamiêæ na hoœcie, czyli RAM, nie VRAM
	kolory = (Point3*)malloc(4 * sizeof(Point3)); //alokujemy pamiêæ na hoœcie, czyli RAM, nie VRAM
	punktyRamki(wierzcholki);
	kolorRamki(kolory);

	if (vboId[0] == 0)
		glGenBuffers(1, &vboId[0]); // tworzenie bufora na karcie. Na hoœcie dostajemy tylko ID tego miejsca.
	if (vboId[1] == 0)
		glGenBuffers(1, &vboId[1]); // tworzenie bufora na karcie. Na hoœcie dostajemy tylko ID tego miejsca.

	glBindBuffer(GL_ARRAY_BUFFER, vboId[0]); // ustawianie bufora na aktualnie modyfikowany
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Point3), wierzcholki, GL_STATIC_DRAW); //wys³anie wierzcho³ków do V

	glBindBuffer(GL_ARRAY_BUFFER, vboId[1]); // ustawianie bufora na aktualnie modyfikowany
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Point3), kolory, GL_STATIC_DRAW); //wys³anie wierzcho³ków do VRAM, czyli do karty graficznej.

	free(kolory);
	free(wierzcholki); // usuwamy, bo wys³aliœmy ju¿ na kartê graficzn¹
}




void loadTexture(char* path)
{
	FreeImage_Initialise();
	FIBITMAP* bitmap = FreeImage_Load(FIF_JPEG, path);
	if (bitmap == NULL)
	{
		printf("File error... :(\n");
		getchar();
		exit(-1);
	}
	GLubyte* textureData = FreeImage_GetBits(bitmap);

	int imageWidth = FreeImage_GetWidth(bitmap);
	int imageHeight = FreeImage_GetHeight(bitmap);

	//int imageWidth =  textureSize(bitmap);
	//int imageHeight = textureSize(bitmap);


	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, textureData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	FreeImage_DeInitialise();
}
void ruchMyszy(int x, int y)
{
	posMouse.x = x;
	posMouse.y = y;
}
void idle()
{
	glutPostRedisplay();
}
void wyswietlanieOkna(int argc, char** argv) {
	GLuint VertexArrayID;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(screen_width, screen_height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("OBROBKA ZDJEC");



	//fragment i funkcja display: http://hohohuhu.blogspot.com/2011/02/opengl-draw-rectangle.html

	glewInit(); //init rozszerzeszeñ OpenGL z biblioteki GLEW
	glutPassiveMotionFunc(ruchMyszy);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(rozmiar);		// def. obs-ugi zdarzenia resize (GLUT)

									//glEnable(GL_LIGHT0);
									//glEnable(GL_LIGHT1);
									//glShadeModel(GL_SMOOTH);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glPointSize(3.0f);
	//ustawianie obiektów atrybutów wierzcho³ków

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

}
void getResolution(char* path){
	FreeImage_Initialise();
	FIBITMAP* bitmap = FreeImage_Load(FIF_JPEG, path);
	if (bitmap == NULL)
	{
		printf("File error... :(\n");
		getchar();
		exit(-1);
	}
	screen_width = FreeImage_GetWidth(bitmap);
	screen_height = FreeImage_GetHeight(bitmap);
}
void ObslugaKlawiszy(char key,int argc, char** argv) {
	
	switch (key)
	{

	case 'q':    /* Esc - koniec */
		exit(1);
		break;
	case '2': /* Efekt latarki */
		getResolution("example3.jpg");
		wyswietlanieOkna(argc, argv);
		programID = loadShaders("vertex_shader.glsl", "fragment_shader.glsl", 1);
		loadTexture("example3.jpg");
		printf("Tekstura wczytana!\n");

		tworzenieVBO();

		break;
	case '3': /* efet binaryzacji */
		getResolution("example.jpg");
		wyswietlanieOkna(argc, argv);
		programID = loadShaders("vertex_shader2.glsl", "fragment_shader2.glsl", 1);
		loadTexture("example.jpg");
		printf("Tekstura wczytana!\n");

		tworzenieVBO();

		break;
	case '4': /* efekt rozmycia*/
		getResolution("example3.jpg");
		wyswietlanieOkna(argc, argv);
		programID = loadShaders("vertex_shader3.glsl", "fragment_shader3.glsl", 1);
		loadTexture("example3.jpg");
		printf("Tekstura wczytana!\n");

		tworzenieVBO();

		break;

	case '5': /* efekt BW*/
		getResolution("example6.jpg");
		printf("Resolution: %d x %d\n", screen_width, screen_height);
		wyswietlanieOkna(argc, argv);
		programID = loadShaders("vertex_shader4.glsl", "fragment_shader4.glsl", 1);
		loadTexture("example6.jpg");
		printf("Tekstura wczytana!\n");

		tworzenieVBO();

		break;

	case '6': /* Przyciemnienie*/
		getResolution("example7.jpg");
		wyswietlanieOkna(argc, argv);
		programID = loadShaders("vertex_shader5.glsl", "fragment_shader5.glsl", 1);
		loadTexture("example7.jpg");
		printf("Tekstura wczytana!\n");

		tworzenieVBO();

		break;
	case '7': /* Sepia*/
		getResolution("example.jpg");
		wyswietlanieOkna(argc, argv);
		programID = loadShaders("vertex_shader6.glsl", "fragment_shader6.glsl", 1);
		loadTexture("example.jpg");
		printf("Tekstura wczytana!\n");

		tworzenieVBO();

		break;
	case '8': /* Negatyw*/
		getResolution("example.jpg");
		wyswietlanieOkna(argc, argv);
		programID = loadShaders("vertex_shader7.glsl", "fragment_shader7.glsl", 1);
		loadTexture("example.jpg");
		printf("Tekstura wczytana!\n");

		tworzenieVBO();

		break;
	case '9': /* Pastele*/
		getResolution("example5.jpg");
		wyswietlanieOkna(argc, argv);
		programID = loadShaders("vertex_shader8.glsl", "fragment_shader8.glsl", 1);
		loadTexture("example5.jpg");
		printf("Tekstura wczytana!\n");

		tworzenieVBO();

		break;
	case '0': /* Implozja*/
		getResolution("example.jpg");
		wyswietlanieOkna(argc, argv);
		programID = loadShaders("vertex_shader9.glsl", "fragment_shader9.glsl", 1);
		loadTexture("example.jpg");
		printf("Tekstura wczytana!\n");

		tworzenieVBO();

		break;
	case '1': /* Kolorowanie*/
		getResolution("example7.jpg");
		wyswietlanieOkna(argc, argv);
		programID = loadShaders("vertex_shader1.glsl", "fragment_shader1.glsl", 1);
		loadTexture("example7.jpg");
		printf("Tekstura wczytana!\n");

		tworzenieVBO();

		break;
	}
}

int main(int argc, char **argv) {



		printf(" WERSJA MOJEGO OPENGL :   ------  %s     ------ \n", glGetString(GL_VERSION));
		//miejsce w ktorym okreslamy co ladujemy
		printf("\n------------> MENU ------------\n\n Wcisnij odpowiedni klawisz aby uzyskac efekt \n \n 1.\t Kolorowanka\n 2.\t Latarka \n 3.\t Uwydatnienie krawedzi \n 4.\t Rozmycie  \n 5.\t Bialo-czarne\n 6. \t Przyciemnianie obrazu \n 7. \t Sepia\n 8.\t Negatyw\n 9.\t Pastele\n 0.\t Implozja \n\n q.\t WYJSCIE Z PROGRAMU\n\n");
		//dodany switch: do uruchamiania shaderow
		char id;
		cin >> id;
		ObslugaKlawiszy(id, argc, argv);


	//end

	glutMainLoop();
	
	return 0;
}