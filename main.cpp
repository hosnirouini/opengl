#include<stdio.h>
#include<stdlib.h>
#include<GL/glew.h>
#include<GL/freeglut.h>

#if 1

#define INDEX_BUFFER 1
#ifdef INDEX_BUFFER
	#define NUM_VERTICES 6
	#define NUM_INDICES 9
#else
   #define NUM_VERTICES 9
#endif


#define BUFFER_OFFSET(i) ((char *)NULL + (i))
GLuint shaderprogramID;
GLuint vao = 0;
GLuint vbo;
GLuint positionID , colorID;
GLuint indexbufferID;

#pragma region SHADER_FUNCTIONS
static char* readFile(char* filename)
{
	FILE* file = fopen(filename ,"r");
	fseek(file,0,SEEK_END);
	long file_length = ftell(file);
	fseek(file,0,SEEK_SET);
	char* contents = new char[file_length+1];
	
	for(int i =0 ; i<file_length+1; i++)
	{
		contents[i] = 0;
	}
	fread(contents,1,file_length,file);
	contents[file_length+1] = '\0';
	fclose(file);
	return contents;
}

GLuint makevertexshader(char* sourcecode)
{
	GLuint vertexshaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexshaderID,1,(const GLchar**)&sourcecode,NULL);
	glCompileShader(vertexshaderID);
	return vertexshaderID;
}

GLuint makefragmentshader(char* sourcecode)
{
	
	GLuint fragmentshaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentshaderID,1,(const GLchar**)&sourcecode,NULL);
	glCompileShader(fragmentshaderID);
	return fragmentshaderID;
}

GLuint makeshaderprogram (GLuint vertexshaderID , GLuint fragmentshaderID)
{
	GLuint shaderID = glCreateProgram();
	glAttachShader(shaderID , vertexshaderID);
	glAttachShader(shaderID , fragmentshaderID);
	glLinkProgram(shaderID);
	return shaderID;
}

#pragma endregion SHADER_FUNCTIONS
void changeViewport(int w , int h)
{
	glViewport(0 ,0 ,w ,h);
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK , GL_TRIANGLES);
#ifdef INDEX_BUFFER
	glDrawElements(GL_TRIANGLES , NUM_INDICES , GL_UNSIGNED_INT,NULL);
#else
	glDrawArrays(GL_TRIANGLES , 0 ,3);
#endif
	glutSwapBuffers();
}

int main (int argc , char** argv)
{

	glutInit(&argc , argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
	glutInitWindowSize(800,600);
	glutCreateWindow("graphics");
	glutReshapeFunc(changeViewport);
	glutDisplayFunc(render);
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr ,"GLEW error");
		return (1);
	}

	

	GLfloat vertes[] = {-0.0f ,0.5f,0.0f,
		               -0.25f ,0.0f,0.0f,
						0.25f ,0.0f,0.0f,
	                    -0.5f ,-0.5f,0.0f,
						 0.0f ,-0.5f,0.0f,
						 0.5f ,-0.5f ,0.0f
	                    };
	GLfloat colors[] = {1.0f,0.0f,0.0f,1.0f,
	                    0.0f,1.0f,0.0f,1.0f,
	                    0.0f,0.0f,1.0f,1.0f,
	                    0.0f,0.0f,1.0f,1.0f,
	                    1.0f,0.0f,0.0f,1.0f,
	                    0.0f,1.0f,0.0f,1.0f};

	GLuint indices[] = {0,1,2,1,3,4,2,4,5};
	
	
	char* vertexshadersource   = readFile("vertexshader.vsh");
	char* fragmentshadersource = readFile("fragmentshader.fsh");
	GLuint  vertshaderID = makevertexshader(vertexshadersource);
	GLuint  fragshaderID = makefragmentshader(fragmentshadersource);
	shaderprogramID = makeshaderprogram (vertshaderID , fragshaderID);
	glUseProgram(shaderprogramID);
	glDeleteProgram(shaderprogramID);
	
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);

	glGenBuffers(1,&vbo);
	glBindBuffer(GL_ARRAY_BUFFER , vbo);

	glBufferData(GL_ARRAY_BUFFER , 6*7*sizeof(GLfloat) , NULL , GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER , 0,6*3*sizeof(GLfloat),vertes);
	glBufferSubData(GL_ARRAY_BUFFER ,6*3*sizeof(GLfloat) ,6*4*sizeof(GLfloat),colors);
#ifdef INDEX_BUFFER
	glGenBuffers(1,&indexbufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , indexbufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER , NUM_INDICES *sizeof(GLuint),indices,GL_STATIC_DRAW);
#endif
	
	positionID = glGetAttribLocation(shaderprogramID , "vposition");
	colorID    = glGetAttribLocation(shaderprogramID , "vcolor");

	glVertexAttribPointer(positionID,3,GL_FLOAT,GL_FALSE,0,0);
	glVertexAttribPointer(colorID,4,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(6*3*sizeof(GLfloat)));
	glUseProgram(shaderprogramID);
	glEnableVertexAttribArray(positionID);
	glEnableVertexAttribArray(colorID);
	
	glutMainLoop();
	system("pause");
	return (0);
}


#else
int main (int argc , char** argv)
{
	GLfloat colors[] = {1.0f,0.0f,0.0f,1.0f,
	                    0.0f,1.0f,0.0f,1.0f,
	                    0.0f,0.0f,1.0f,1.0f};
    printf("%\n",sizeof(colors));
	system("pause");
	return (0);
}
#endif
