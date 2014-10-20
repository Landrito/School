#define GLM_FORCE_RADIANS

#include <GL/glew.h> // glew must be included before the main gl libs
#include <GL/glut.h> // doing otherwise causes compiler shouting
#include <iostream>
#include <stdio.h>
#include <chrono>
#include <vector>
#include <fstream>
using namespace std;

#include <Magick++.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

#include "ShaderLoader.h" //Gets the shader files


//Planet struct
struct celestial
{
    float rotSpeed;
    float revSpeed;
    float radius;
    float axialTilt;
    float revRadius;
    float revTilt;
    int parent;
    char imgPath[30];
    glm::mat4 model;
    glm::mat4 translation;
    glm::vec3 position;
};

//struct functions
void loadSystemAttributes( const char fileName[32], vector<celestial>&  returnSystem);
//Global Constant
const float SCALING_FACTOR = 696000;//Suns radius should be unit length
const float EARTHS_DEG_PER_SEC = 0.00001145833;
const float MOON_DEG_PER_SEC = 0.000154321;

// camera global variables
float eyeX = 10.0;
float eyeY = 10.0;
float eyeZ = 10.0;

float focX = 0.0;
float focY = 0.0;
float focZ = 0.0;

float distance = 7;

//Global List of celestial bodies
vector<celestial> solarSystem;

//--Data typescd s
//This object will define the attributes of a vertex(position, color, etc...)
struct Vertex
{
    GLfloat position[3];
    GLfloat uv[2];
};

//--Evil Global variables
//Just for this example!
int w = 640, h = 480;// Window size
GLuint program;// The GLSL program handle
GLuint vbo_geometry;// VBO handle for our geometry
GLuint * text = NULL;
int numVertices;
int numPlanets = 0;

Magick::Blob * m_blob = NULL;


//uniform locations
GLint loc_mvpmat;// Location of the modelviewprojection matrix in the shader

//attribute locations
GLint loc_position;
GLint texAttrib;

//transform matrices
//glm::mat4 model;//obj->world each object should have its own model matrix
glm::mat4 view;//world->eye
glm::mat4 projection;//eye->clip
glm::mat4 mvp;//premultiplied modelviewprojection

//spin/rotation flags
int spinCube = 1;
int rotationDirection = 1;
float rotationSpeed = 1;
int planetID = 0;

//--GLUT Callbacks
void render();
void update();
void reshape(int n_w, int n_h);
void keyboard(unsigned char key, int x_pos, int y_pos);
void mouse(int button, int state, int x_pos, int y_pos);
void menu(int id);

//--Resource management
bool initialize(char * objPath, char * imgPath);
void cleanUp();

//--Random time things
float getDT();
std::chrono::time_point<std::chrono::high_resolution_clock> t1,t2;

Vertex * loadOBJ( const char * path );


//--Main
int main(int argc, char **argv)
{
    // Initialize glut
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(w, h);
    // Name and create the Window
    glutCreateWindow("Solar System");

    // Create menu
    glutCreateMenu(menu);
    glutAddMenuEntry("Start Spin", 1);
    glutAddMenuEntry("Stop Spin", 2);
    glutAddMenuEntry("Exit", 3);  
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    // Now that the window is created the GL context is fully set up
    // Because of that we can now initialize GLEW to prepare work with shaders
    GLenum status = glewInit();
    if( status != GLEW_OK)
    {
        std::cerr << "[F] GLEW NOT INITIALIZED: ";
        std::cerr << glewGetErrorString(status) << std::endl;
        return -1;
    }

    // Set all of the callbacks to GLUT that we need
    glutDisplayFunc(render);// Called when its time to display
    glutReshapeFunc(reshape);// Called if the window is resized
    glutIdleFunc(update);// Called if there is nothing else to do
    glutKeyboardFunc(keyboard);// Called if there is keyboard input
    glutMouseFunc(mouse);

    // Initialize all of our resources(shaders, geometry)
    bool init = initialize(argv[1], argv[2]);
    if(init)
    {
        t1 = std::chrono::high_resolution_clock::now();
        glutMainLoop();
    }

    // Clean up after ourselves
    cleanUp();
    return 0;
}

//--Implementations
void render()
{
    //--Render the scene

    //clear the screen
    glClearColor(0.0, 0.0, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);

    //Render each celestial body
   for(int i = 0; i < numPlanets; i++)
{
	//premultiply the matrix for this example
    mvp = projection * view * solarSystem[i].model;

    //enable the shader program
    glUseProgram(program);

    //upload the matrix to the shader
    glUniformMatrix4fv(loc_mvpmat, 1, GL_FALSE, glm::value_ptr(mvp));

    //set up the Vertex Buffer Object so it can be drawn
    glEnableVertexAttribArray(loc_position);
    glEnableVertexAttribArray(texAttrib);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_geometry);
    
    glBindTexture(GL_TEXTURE_2D, text[i]);


    //set pointers into the vbo for each of the attributes(position and color)
    glVertexAttribPointer( loc_position,//location of attribute
                           3,//number of elements
                           GL_FLOAT,//type
                           GL_FALSE,//normalized?
                           sizeof(Vertex),//stride
                           0);//offset

    glVertexAttribPointer( texAttrib,
                           2,
                           GL_FLOAT,
                           GL_FALSE,
                           sizeof(Vertex),
                           (void*)offsetof(Vertex,uv));

    glDrawArrays(GL_TRIANGLES, 0, numVertices);//mode, starting index, count

    //clean up
    glDisableVertexAttribArray(loc_position);
    glDisableVertexAttribArray(texAttrib);
}
    //swap the buffers
    glutSwapBuffers();
}

void update()
{
static float angle = 0.0;
static float spin = 0.0;
float dt = getDT();

angle += dt * 2 * M_PI * spinCube;
spin += dt * 2 * M_PI * spinCube;

glm::mat4 translation;
glm::mat4 rotation;
glm::mat4 scaling;

for(int i = 0; i < numPlanets; i++)
{
    glm::mat4 parentModel;
	int parentPlanet = solarSystem[i].parent;
    solarSystem[i].model = glm::mat4(1.0f);

    if (parentPlanet == 0)
    {
        parentModel = glm::mat4(1.0f);
    }
    else
    {
        parentModel = solarSystem[parentPlanet].model;
    }

    translation = glm::translate( parentModel, 
        glm::vec3( solarSystem[i].revRadius * sin(angle * solarSystem[i].revSpeed), solarSystem[i].revRadius * sin(angle * solarSystem[i].revSpeed)*tan(-1*solarSystem[i].revTilt), solarSystem[i].revRadius * cos(angle * solarSystem[i].revSpeed) ) );

	//translation = parentModel * glm::translate( glm::mat4(1.0f), 
	//	glm::vec3( solarSystem[i].revRadius * sin(angle * solarSystem[i].revSpeed), 0.0, solarSystem[i].revRadius * cos(angle * solarSystem[i].revSpeed) ) );


    solarSystem[i].position = glm::vec3( solarSystem[i].revRadius * sin(angle * solarSystem[i].revSpeed), solarSystem[i].revRadius * sin(angle * solarSystem[i].revSpeed)*tan(-1*solarSystem[i].revTilt), solarSystem[i].revRadius * cos(angle * solarSystem[i].revSpeed) ) ;
    solarSystem[i].model *= translation;

}



view = glm::lookAt( glm::vec3( (eyeX * solarSystem[planetID].radius) + solarSystem[planetID].position.x, (eyeY * solarSystem[planetID].radius) + solarSystem[planetID].position.y, (eyeZ * solarSystem[planetID].radius) + solarSystem[planetID].position.z), // this is the camera position 
               solarSystem[planetID].position,  // this the focus point
               glm::vec3( 0.0, 1.0, 0.0 ) ); // y is up



for(int i = 0; i < numPlanets; i++)
{
    rotation = glm::rotate( glm::mat4(1.0f), spin * solarSystem[i].rotSpeed, glm::vec3( sin(solarSystem[i].axialTilt), cos(solarSystem[i].axialTilt), 0.0) );
    solarSystem[i].model *= rotation;
}

for(int i = 0; i < numPlanets; i++)
{

    if(i != 12)
        scaling = glm::scale(glm::mat4(1.0f), glm::vec3( solarSystem[i].radius, solarSystem[i].radius, solarSystem[i].radius ) );
    else 
        scaling = glm::scale(glm::mat4(1.0f), glm::vec3( 0.7 * solarSystem[i].radius, 0.001 * solarSystem[i].radius, 0.7 * solarSystem[i].radius ) );
    
    solarSystem[i].model *= scaling;
}
    // Update the state of the scene
    glutPostRedisplay();//call the display callback
}


void reshape(int n_w, int n_h)
{
    w = n_w;
    h = n_h;
    //Change the viewport to be correct
    glViewport( 0, 0, w, h);
    //Update the projection matrix as well
    //See the init function for an explaination
    projection = glm::perspective(45.0f, float(w)/float(h), 0.01f, 100.0f);

}

void keyboard(unsigned char key, int x_pos, int y_pos)
{
    // Handle keyboard input
    if(key == 27)//ESC
    {
        exit(0);
    }
    // Handle planet switches
    if(key == 48)  //'0' Sun
    {
          planetID = 0;
    }
    if(key == 49)  //'1' Mercury
    {
          planetID = 1;
    }
    if(key == 50)  //'2' Venus
    {
          planetID = 2;
    }
    if(key == 51)  //'3' 3rd Rock from the Sun  
    {
          planetID = 3;
    }
    if(key == 52)  //'4' Mars
    {
          planetID = 5;
    }
    if(key == 53)  //'5' Jupiter
    {
          planetID = 8;
    }
    if(key == 54)  //6 Saturn
    {
          planetID = 11;
    }
    if(key == 55)  //'7' Uranis
    {
          planetID = 14;
    }
    if(key == 56) //'8' Neptune
    {
          planetID = 17;
    }
    if(key == 57)  //'9' Pluto
    {
          planetID = 20;
    }
}

void mouse(int button, int state, int x_pos, int y_pos)
{
  if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
      rotationDirection *= -1; //Change the direction the cube is rotating
  }
  
}

void menu(int id)
{
  switch(id)
  {
    case 1:
      spinCube = 1; //start spinning case
      break;

    case 2:
      spinCube = 0; //stop spinning case
      break;

    case 3: 
      exit(0); //exit case
      break;
  }
}


bool initialize(char * objPath, char * imgPath)
{
    // Initialize Loaders for shaders
    ShaderLoader vertShader, fragShader;

    // Initialize the solarsystem
    loadSystemAttributes("../bin/solarSystem.txt",solarSystem);
    m_blob = new Magick::Blob[ solarSystem.size() ];
    text = new GLuint[ solarSystem.size() ];


    //for( unsigned int i = 0; i < solarSystem->size(); i++ )
      //  solarSystem->at(i).createTextureBuffer();

    // Initialize basic geometry and shaders for this example
    //this defines a cube, this is why a model loader is nice
    //you can also do this with a draw elements and indices, try to get that working
    Vertex * geometry = loadOBJ("../bin/sphere.obj");



    // Create a Vertex Buffer object to store this vertex info on the GPU
    glGenBuffers(1, &vbo_geometry);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_geometry);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), geometry, GL_STATIC_DRAW);

    //--Geometry done
glGenTextures( (int)(solarSystem.size()), text );
   for(unsigned int i = 0; i < solarSystem.size(); i++)
{

    Magick::Image * m_pImage = new Magick::Image( solarSystem[i].imgPath );
    m_pImage->write(&m_blob[i], "RGBA");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, text[i]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_pImage->columns(), m_pImage->rows(), 0, 
                    GL_RGBA, GL_UNSIGNED_BYTE, m_blob[i].data() );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    delete m_pImage;
    m_pImage = NULL;
}

    //Set the shaders
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    //Shader Sources
    // Put these into files and write a loader in the future
    // Note the added uniform!
    vertShader.load("../bin/matrix.vert");
    const char *vs = vertShader.getShader();

    fragShader.load("../bin/matrix.frag");
    const char *fs = fragShader.getShader();

    //compile the shaders
    GLint shader_status;

    // Vertex shader first
    glShaderSource(vertex_shader, 1, &vs, NULL);
    glCompileShader(vertex_shader);
    //check the compile status
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &shader_status);

    char buffer[512];

    if(!shader_status)
    {

        std::cerr << "[F] FAILED TO COMPILE VERTEX SHADER!" << std::endl;

        glGetShaderInfoLog(vertex_shader, 512, NULL, buffer); // inserts the error into the buffer

		std::cerr << buffer << std::endl; // prints out error

		
        return false;
    }

    // Now the Fragment shader
    glShaderSource(fragment_shader, 1, &fs, NULL);
    glCompileShader(fragment_shader);
    //check the compile status
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &shader_status);
    if(!shader_status)
    {
        std::cerr << "[F] FAILED TO COMPILE FRAGMENT SHADER!" << std::endl;

		glGetShaderInfoLog(fragment_shader, 512, NULL, buffer); // inserts the error into the buffer

		std::cerr << buffer << std::endl; // prints out error

        return false;
    }

    //Now we link the 2 shader objects into a program
    //This program is what is run on the GPU
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    //check if everything linked ok
    glGetProgramiv(program, GL_LINK_STATUS, &shader_status);
    if(!shader_status)
    {
        std::cerr << "[F] THE SHADER PROGRAM FAILED TO LINK" << std::endl;
        return false;
    }

    //Now we set the locations of the attributes and uniforms
    //this allows us to access them easily while rendering
    loc_position = glGetAttribLocation(program,
                    const_cast<const char*>("v_position"));
    if(loc_position == -1)
    {
        std::cerr << "[F] POSITION NOT FOUND" << std::endl;
        return false;
    }

    texAttrib = glGetAttribLocation(program,
                    const_cast<const char*>("v_tex"));
    if(texAttrib == -1)
    {
        std::cerr << "[F] V_TEX NOT FOUND" << std::endl;
        return false;
    }

    loc_mvpmat = glGetUniformLocation(program,
                    const_cast<const char*>("mvpMatrix"));
    if(loc_mvpmat == -1)
    {
        std::cerr << "[F] MVPMATRIX NOT FOUND" << std::endl;
        return false;
    }
    
    //--Init the view and projection matrices
    //  if you will be having a moving camera the view matrix will need to more dynamic
    //  ...Like you should update it before you render more dynamic 
    //  for this project having them static will be fine
    view = glm::lookAt( glm::vec3(0.0, 8.0, -16.0), //Eye Position
                        glm::vec3(0.0, 0.0, 0.0), //Focus point
                        glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

    projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                   float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                   0.01f, //Distance to the near plane, normally a small value like this
                                   100.0f); //Distance to the far plane, 

    //enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //and its done
    return true;
}

void cleanUp()
{
    // Clean up, Clean up
    glDeleteProgram(program);
    glDeleteBuffers(1, &vbo_geometry);
}

//returns the time delta
float getDT()
{
    float ret;
    t2 = std::chrono::high_resolution_clock::now();
    ret = std::chrono::duration_cast< std::chrono::duration<float> >(t2-t1).count();
    t1 = std::chrono::high_resolution_clock::now();
    return ret;
}

Vertex * loadOBJ( const char * objPath)
{
    Assimp::Importer importer;
    const aiScene * scene = importer.ReadFile( objPath, aiProcess_Triangulate);

    aiMesh * mesh = scene->mMeshes[0];

    numVertices = mesh->mNumFaces * 3;
    Vertex * geo = new Vertex[ numVertices ];

    for( unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        const aiFace & face = mesh->mFaces[i];

        for(unsigned int j = 0; j < 3; j++)
        {
            aiVector3D pos = mesh->mVertices[face.mIndices[j]];
            geo->position[0] = pos.x;
            geo->position[1] = pos.y;
            geo->position[2] = pos.z;

            aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[j]];
            geo->uv[0] = uv.x;
            geo->uv[1] = uv.y;
            geo++;
        }
    }
    geo -= mesh->mNumFaces * 3;

    return geo;
}

void loadSystemAttributes( const char fileName[32], vector<celestial>&  returnSystem)
{

    ifstream fin;
    fin.clear();
    fin.open(fileName);

    char celestialType[32];
    char attribute[32];
    int mostRecentPlanetIndex = 0;

    float conversionFactor = (M_PI/180.0);
    celestial temp;

    fin >> celestialType;
    int i = 0;
    while( fin.good() )
    {
        fin >> attribute >> temp.rotSpeed;
        fin >> attribute >> temp.revSpeed;
        fin >> attribute >> temp.axialTilt;
        fin >> attribute >> temp.radius;
        fin >> attribute >> temp.revRadius;
        fin >> attribute >> temp.revTilt;
        fin >> attribute >> temp.imgPath;

        cout << attribute << temp.rotSpeed << endl;
        cout << attribute << temp.revSpeed << endl;
        cout << attribute << temp.axialTilt << endl;
        cout << attribute << temp.radius << endl;
        cout << attribute << temp.revRadius << endl;
        cout << attribute << temp.revTilt << endl;
        cout << attribute << temp.imgPath << endl;
        
        temp.axialTilt *= conversionFactor;
        temp.revTilt *= conversionFactor;

        
        temp.model = glm::mat4(1.0f);
        temp.translation = glm::mat4(1.0f);
        temp.position = glm::vec3(0.0f);

        
 	      temp.parent = 0;
        if( strcmp(celestialType, "Star:") == 0 )
        {
            temp.radius /= SCALING_FACTOR;
           temp.revRadius = 0;
        }
        else if( strcmp(celestialType, "Planet:") == 0 )
        {
            temp.radius /= SCALING_FACTOR;
            temp.rotSpeed = temp.rotSpeed / EARTHS_DEG_PER_SEC * 0.05;
            temp.revSpeed = temp.revSpeed / EARTHS_DEG_PER_SEC * 0.05;
           temp.revRadius = 1 + i;
           mostRecentPlanetIndex = i;  
            
        }
        else if( strcmp(celestialType, "Rings:") == 0 )
        {
            temp.radius /= SCALING_FACTOR;
            temp.radius *= 3; 
            temp.rotSpeed = temp.rotSpeed / EARTHS_DEG_PER_SEC * 0.05;
            temp.revSpeed = temp.revSpeed / EARTHS_DEG_PER_SEC * 0.05;
           temp.parent = mostRecentPlanetIndex; 
            
        }
        else if( strcmp(celestialType, "Moon:") == 0 )
        {
            temp.radius  /= SCALING_FACTOR;
            temp.radius *= 3; 
            temp.rotSpeed = temp.rotSpeed / MOON_DEG_PER_SEC * 0.005;

            temp.revSpeed = temp.revSpeed / MOON_DEG_PER_SEC * 0.005;
            temp.revRadius /= SCALING_FACTOR;
            temp.revRadius /= 3;
            temp.parent = mostRecentPlanetIndex; 

        }

        returnSystem.push_back(temp);

        fin >> celestialType;
        numPlanets++;
        i += 1;
    }

   
}




