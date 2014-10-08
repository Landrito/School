#define GLM_FORCE_RADIANS

#include <GL/glew.h> // glew must be included before the main gl libs
#include <GL/glut.h> // doing otherwise causes compiler shouting
#include <iostream>
#include <chrono>
#include <vector>

#include <Magick++.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

#include "ShaderLoader.h" //Gets the shader files

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
GLuint text;
int numVertices;

Magick::Blob m_blob;


//uniform locations
GLint loc_mvpmat;// Location of the modelviewprojection matrix in the shader

//attribute locations
GLint loc_position;
GLint texAttrib;

//transform matrices
glm::mat4 model;//obj->world each object should have its own model matrix
glm::mat4 view;//world->eye
glm::mat4 projection;//eye->clip
glm::mat4 mvp;//premultiplied modelviewprojection

//spin/rotation flags
int spinCube = 0;
int rotationDirection = 1;
float rotationSpeed = 1;

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
    if( argc != 3 )
    {
      //If not shout at them and leave
        printf("This progam needs two filenames. \n");
        exit(1);
    }

    // Initialize glut
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(w, h);
    // Name and create the Window
    glutCreateWindow("Matrix Example");

    // Create menu
    glutCreateMenu(menu);
    glutAddMenuEntry("Start Cube Spin", 1);
    glutAddMenuEntry("Stop Cube Spin", 2);
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

    //premultiply the matrix for this example
    mvp = projection * view * model;

    //enable the shader program
    glUseProgram(program);

    //upload the matrix to the shader
    glUniformMatrix4fv(loc_mvpmat, 1, GL_FALSE, glm::value_ptr(mvp));

    //set up the Vertex Buffer Object so it can be drawn
    glEnableVertexAttribArray(loc_position);
    glEnableVertexAttribArray(texAttrib);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_geometry);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, text);

    //

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
                           
    //swap the buffers
    glutSwapBuffers();
}

void update()
{
    //total time
    static float angle = 0.0;
    static float spin = 0.0;
    float dt = getDT();// if you have anything moving, use dt.

    

    angle += dt * M_PI/2 * rotationDirection * rotationSpeed; //move through 90 degrees a second
    spin += dt * M_PI * spinCube;
   
    model = glm::translate( glm::mat4(1.0f), glm::vec3(4.0 * sin(angle), 0.0, 4.0 * cos(angle)));
    model *= glm::rotate( glm::mat4(1.0f), spin, glm::vec3(0.0, 1.0, 0.0) );
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
    if(key == 65 || key == 97)//'A'
    {
        rotationDirection *= -1; //Change the direction the cube is rotating
    }
    if(key == 83 || key == 115)//'S'
    {
        rotationSpeed += .5;
    }
    if(key == 68 || key == 100)//'D'
    {
        rotationSpeed -= .5;
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



    // Initialize basic geometry and shaders for this example

    //this defines a cube, this is why a model loader is nice
    //you can also do this with a draw elements and indices, try to get that working
    Vertex * geometry = loadOBJ(objPath);
    Magick::Image * m_pImage = new Magick::Image(imgPath);
    m_pImage->write(&m_blob, "RGBA");
    


    // Create a Vertex Buffer object to store this vertex info on the GPU
    glGenBuffers(1, &vbo_geometry);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_geometry);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), geometry, GL_STATIC_DRAW);

    //--Geometry done

    //Create a Texture Buffer Object
    glGenTextures(1, &text);
    glBindTexture(GL_TEXTURE_2D, text);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_pImage->columns(), m_pImage->rows(), 0, 
    				GL_RGB, GL_UNSIGNED_BYTE, m_blob.data() );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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
