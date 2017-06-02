
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <time.h>
#include <conio.h>
#include "..\glm/vec3.hpp" 
#include "..\glm/vec4.hpp" 
#include "..\glm/mat4x4.hpp"
#include "..\glm/gtc/matrix_transform.hpp"
#include "..\GL\glew.h"
#include "..\GL\freeglut.h" 
#include "..\freeimage.h"

#include "shaderLoader.h" 
#include "..\GL\tinyfiledialogs.h"
#define SIZX 1366
#define SIZY 768

using namespace std;

//Source 1st:
// http://www.malgouyres.org/freeimagehowto 
//Source 2nd:
// dr. inz. Kamil Szostek - szkielet programu (tworzenie, przekazywanie parametrow i obsluga shaderow)

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

glm::mat4 M; //model 
glm::mat4 V; //view
glm::mat4 P;  //projection

int screen_width;
int screen_height;
float filtrValue = 0.5;
char const *file_open;
char const *file_save;
float red = 1.0;
float green = 1.0;
float blue = 1.0;
typedef struct {
	GLsizei width;
	GLsizei height;
}GLTextureSize;

double kameraX = 0;
double kameraZ = 0;
double kameraD = -1000;
double kameraKat = 0;
Point2 posMouse = { 0, 0 };
//VBO
GLuint vboId[3] = { 0 };
GLuint ilosc;
void tworzenieVBO();
GLuint programID = 0;
//textur3
GLuint tempTextureID = 0;
GLuint texID;

//
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
	glUseProgram(programID);

	glm::mat4 MVP = P*V*M;
	GLuint MVP_id = glGetUniformLocation(programID, "MVP"); 
	glUniformMatrix4fv(MVP_id, 1, GL_FALSE, &(MVP[0][0]));  

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);
	GLuint temp_id = glGetUniformLocation(programID, "texture");
	glUniform1i(temp_id, 1);
	temp_id = glGetUniformLocation(programID, "posCursor");
	glUniform2f(temp_id, posMouse.x, screen_height - 1 - posMouse.y);
	temp_id = glGetUniformLocation(programID, "value");
	glUniform1f(temp_id, filtrValue);
	temp_id = glGetUniformLocation(programID, "height");
	glUniform1f(temp_id, screen_height);
	temp_id = glGetUniformLocation(programID, "width");
	glUniform1f(temp_id, screen_width);
	temp_id = glGetUniformLocation(programID, "red");
	glUniform1f(temp_id, red);
	temp_id = glGetUniformLocation(programID, "green");
	glUniform1f(temp_id, green);
	temp_id = glGetUniformLocation(programID, "blue");
	glUniform1f(temp_id, blue);
	glEnableVertexAttribArray(0); 
	glBindBuffer(GL_ARRAY_BUFFER, vboId[0]);
	glVertexAttribPointer(
		0,                  
		3,                  
		GL_FLOAT,           
		GL_FALSE,           
		0,                 
		(void*)0            
	);

	glEnableVertexAttribArray(1); 
	glBindBuffer(GL_ARRAY_BUFFER, vboId[1]);
	glVertexAttribPointer(
		1,                  
		3,                  
		GL_FLOAT,           
		GL_FALSE,          
		0,                  
		(void*)0            
	);
	glEnableVertexAttribArray(2); 
	glBindBuffer(GL_ARRAY_BUFFER, vboId[2]);
	glVertexAttribPointer(
		1,                 
		3,                  
		GL_FLOAT,           
		GL_FALSE,           
		0,                  
		(void*)0            
	);

	glDrawArrays(GL_QUADS, 0, 4); 
	glDisableVertexAttribArray(0);  
	glUseProgram(0);                
}

void display() {
	int i = 0;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	V = glm::mat4(1.0f); 
	V = glm::translate(V, glm::vec3(0, 0, kameraD));
													 
	M = glm::mat4(1.0f); 
	rysujVBO();

	glutReportErrors();
	glutSwapBuffers();
}


void tworzenieVBO() {
	Point3 *wierzcholki;
	Point3 *kolory;
	wierzcholki = (Point3*)malloc(4 * sizeof(Point3));
	kolory = (Point3*)malloc(4 * sizeof(Point3)); 
	punktyRamki(wierzcholki);
	kolorRamki(kolory);

	if (vboId[0] == 0)
		glGenBuffers(1, &vboId[0]); 
	if (vboId[1] == 0)
		glGenBuffers(1, &vboId[1]); 

	glBindBuffer(GL_ARRAY_BUFFER, vboId[0]); 
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Point3), wierzcholki, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboId[1]); 
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Point3), kolory, GL_STATIC_DRAW); 

	free(kolory);
	free(wierzcholki); 
}




void loadTexture(const char* path)
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

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, textureData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
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



	//Source 3rg: http://hohohuhu.blogspot.com/2011/02/opengl-draw-rectangle.html

	glewInit(); 
	glutPassiveMotionFunc(ruchMyszy);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(rozmiar);		


	glEnable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

}
void getResolution(const char* path) {
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
	FreeImage_DeInitialise();
}
void ZapisObrazka() {
	try {
		FreeImage_Initialise();
		GLubyte * pixels = new GLubyte[3 * screen_width * screen_height];
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadPixels(0, 0, screen_width, screen_height, GL_BGR, GL_UNSIGNED_BYTE, pixels);
		FIBITMAP* Obraz = FreeImage_ConvertFromRawBits(pixels, screen_width, screen_height, 3 * screen_width, 24, 0x0000FF, 0xFF0000, 0x00FF00, false);
		bool succes = FreeImage_Save(FIF_JPEG, Obraz, file_save, JPEG_QUALITYSUPERB);
		if (succes) {
			tinyfd_messageBox("Plik zapisany!", "Obraz zostal pomyslnie zapisany!", "ok", "info", 1);
		}
		else {
			tinyfd_messageBox("ERROR!", "Obraz nie zostal zapisany!", "ok", "error", 1);
		}

		FreeImage_Unload(Obraz);
		delete[] pixels;
		FreeImage_DeInitialise();
	}
	catch (...) {
		std::cout << "Cos poszlo nie tak! :(" << std::endl;
	}
}


void ObslugaKlawiszy(char key, int argc, char**argv) {



	switch (key)
	{

	case 'q':    /* Esc - end */
	case 'Q':
		exit(1);
		break;
	case '0': /* Implosion*/
	{

		programID = loadShaders("vertex_shader.glsl", "fragment_shader9.glsl", 1);
		break;
	}

	case '1':/* Strange colors*/
	{

		programID = loadShaders("vertex_shader.glsl", "fragment_shader1.glsl", 1);
		break;
	}

	case '2': /* flashlight */
	{

		programID = loadShaders("vertex_shader.glsl", "fragment_shader.glsl", 1);

		break;
	}

	case '3': /* binary */
	{

		programID = loadShaders("vertex_shader.glsl", "fragment_shader2.glsl", 1);

		break;
	}
	case '4': /* blur*/
	{

		programID = loadShaders("vertex_shader.glsl", "fragment_shader3.glsl", 1);

		break;
	}
	case '5': /* bw*/
	{

		programID = loadShaders("vertex_shader.glsl", "fragment_shader4.glsl", 1);

		break;
	}
	case '6': /* dark*/
	{

		programID = loadShaders("vertex_shader.glsl", "fragment_shader5.glsl", 1);
		break;
	}
	case '7': /* Sepia*/
	{

		programID = loadShaders("vertex_shader.glsl", "fragment_shader6.glsl", 1);
		break;
	}
	case '8': /* CMY*/
	{

		programID = loadShaders("vertex_shader.glsl", "fragment_shader7.glsl", 1);

		break;
	}
	case '9': /* Pastels*/
	{

		programID = loadShaders("vertex_shader.glsl", "fragment_shader8.glsl", 1);

		break;
	}
	case '`':
	{
		programID = loadShaders("vertex_shader.glsl", "fragment_shader10.glsl", 1);
		break;
	}
	case '+':
	{
		if (filtrValue < 1) filtrValue += 0.01;
		break;
	}
	case '-':
	{
		if (filtrValue > 0) filtrValue -= 0.01;
		break;
	}
	case 'R':
	case 'r':
	{
		if (red < 1) red += 0.01;
		break;
	}
	case 'T':
	case 't':
	{
		if (green < 1) green += 0.01;
		break;
	}
	case 'Y':
	case 'y':
	{
		if (blue < 1) blue += 0.01;
		break;
	}
	case 'F':
	case 'f':
	{
		if (red > 0) red -= 0.01;
		break;
	}
	case 'G':
	case 'g':
	{
		if (green > 0) green -= 0.01;
		break;
	}
	case 'H':
	case 'h':
	{
		if (blue > 0) blue -= 0.01;
		break;
	}
	case 'S':
	case 's':
	{
		ZapisObrazka();
		break;
	}
	default:
	{
		printf("ZLA OPCJA \n");
		break;
	}

	}


}
void klawisz(GLubyte key, int x, int y)
{
	ObslugaKlawiszy(key, 0, 0);
}
int main(int argc, char **argv) {

	char const * filtryOdczytu[3] = { "*.jpg", "*.png", "*.bmp" };
	file_open = tinyfd_openFileDialog("Wybierz zdjecie do edycji", "", 3, filtryOdczytu, "Image files (.jpg, .png, .bmp)", false);
	if (!file_open) {
		tinyfd_messageBox("Zamykam", "Nazwa pliku do odczytu nie zostala podana!", "ok", "info", 1);
		exit(0);
	}
	char const * filtrZapisu[1] = { "*.jpg" };
	const char* file_save_tmp = tinyfd_saveFileDialog("Zapisz jako... ", "", 1, filtrZapisu, "Image files (.jpg)");
	if (!file_save_tmp) {
		tinyfd_messageBox("Zamykam", "Nazwa pliku do zapisu nie zostala podana!", "ok", "info", 1);
		exit(0);
	}
	const char *ext = file_save_tmp + strlen(file_save_tmp) - 4;
	if (strcmp(ext, ".jpg") == 0)
		file_save = file_save_tmp;
	else
	{
		char tmp[100] = { 0 };
		strcpy_s(tmp, file_save_tmp);
		strcat_s(tmp, ".jpg");
		file_save = tmp;
	}
	printf("Plik zapisze sie pod nazwa: %s\n", file_save);

	printf("\n------------> MENU ------------\n\n Wcisnij odpowiedni klawisz aby uzyskac efekt \n \n 1.\t Kolorowanka\n 2.\t Latarka \n 3.\t Uwydatnienie krawedzi \n 4.\t Rozmycie  \n 5.\t Bialo-czarne\n 6. \t Przyciemnianie obrazu \n 7. \t Sepia\n 8.\t Negatyw\n 9.\t Pastele\n 0.\t Implozja \n '`'.\t Manual\n\n q.\t WYJSCIE Z PROGRAMU\n\n");
	printf("\n\tZapis do pliku: S/s (rozszerzenie *.jpg-nie trzeba go podawac w nazwie) \n\tZmiana parametru filtru: +/- (oprocz BW i sepii)\n\n\tFiltr startowy \'`\':\n\tZwiekszanie wartosci kolorow: RTY/rty \n\tZmniejszanie wartosci kolorow: FGH/fgh  \n\tZmiana parametru alfa: +/-\n\n");

	getResolution(file_open);
	wyswietlanieOkna(argc, argv);
	programID = loadShaders("vertex_shader.glsl", "fragment_shader10.glsl", 1);

	loadTexture(file_open);

	tworzenieVBO();

	glutKeyboardFunc(klawisz);
	glutMainLoop();
}