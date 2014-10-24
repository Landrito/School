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

#include "ShaderLoader.h" 

#include <btBulletDynamicsCommon.h>
using namespace std;

enum object {BOARD, PUCK, PADDLE_ONE, PADDLE_TWO};

//This is the structure that will be
struct Vertex
{
    GLfloat position[3];
    GLfloat uv[2];
};

class Object
{	
public:
	Object();
	Object(const & Object src);
	~Object();
	Object& operator=(const & Object src);

	bool loadObject(const char * path);
	bool loadTexture(const char * path);

    //getters
    Vertex * getGeometry() const;
    int getNumVertices() const;
    Magick::Blob * getBlob() const;

    //setters
    void setVertexBuffer(GLuint vbo);
    void setTextureBuffer(GLuint vbo);

    glm::mat4 model;

    static glm::mat4 view;
    static glm::mat4 projection;
    static GLuint program;
    static GLint loc_position;
    static GLint loc_texture;
    static GLint loc_mvpmat;

private:
	Vertex * geometry;
	int numVertices;
	Magick::Image * image;
	Magick::Blob * blob;
    GLuint vbo_geometry;
    GLuint vbo_texture;
    glm::mat4 model;
};

//Global Variables
vector<Object> * gameObjects;
int w, h;

//--GLUT Callbacks
void render();
void update();
void reshape(int n_w, int n_h);
void keyboard(unsigned char key, int x_pos, int y_pos);
void mouse(int button, int state, int x_pos, int y_pos);
void menu(int id);

//Start and enders for the program
vector<Object> * initialize();
void initializeGlut();
void cleanUp();

//Time things
float getDT();
std::chrono::time_point<std::chrono::high_resolution_clock> t1,t2;

int main()
{
	//This object is only used to be able to be able to access the static members in the class
	Object bufferHolder;

	//Initialize the objects
	gameObjects = initialize();
    initializeGlut();

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

    cleanup();
    return 0;
}

void render()
{
    //clear the screen
    glClearColor(0.0, 0.0, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(int i = 0; i < gameObjects; i++)
    {
        //premultiply the matrix for this example
        glm::mat4 mvp = gameObjects[i].projection * gameObjects[i].view * gameObjects[i].model;

        //enable the shader program
        glUseProgram(gameObjects[i].program);

        //upload the matrix to the shader
        glUniformMatrix4fv(gameObjects[i].loc_mvpmat, 1, GL_FALSE, glm::value_ptr(mvp));

        //set up the Vertex Buffer Object so it can be drawn
        glEnableVertexAttribArray(gameObjects[i].loc_position);
        glEnableVertexAttribArray(gameObjects[i].loc_texture);
        glBindBuffer(GL_ARRAY_BUFFER, gameObjects[i].vbo_geometry);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gameObjects[i].text);

        //

        //set pointers into the vbo for each of the attributes(position and color)
        glVertexAttribPointer( gameObjects[i].loc_position,//location of attribute
                               3,//number of elements
                               GL_FLOAT,//type
                               GL_FALSE,//normalized?
                               sizeof(Vertex),//stride
                               0);//offset

        glVertexAttribPointer( gameObjects[i].texAttrib,
                               2,
                               GL_FLOAT,
                               GL_FALSE,
                               sizeof(Vertex),
                               (void*)offsetof(Vertex,uv));

        glDrawArrays(GL_TRIANGLES, 0, gameObjects[i].numVertices);//mode, starting index, count

        //clean up
        glDisableVertexAttribArray(gameObjects[i].loc_position);
        glDisableVertexAttribArray(gameObjects[i].loc_texture);
    }
                           
    //swap the buffers
    glutSwapBuffers();
}

void update()
{

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
}

void mouse(int button, int state, int x_pos, int y_pos) {}

void menu(int id)
{
    switch(id)
    {
        case 1:
          exit(); //start spinning case
          break;
    }
}

Object::Object() : model(1.0f), view(1.0f), projection(1.0f), program(0), geometry(NULL), numVertices(0), 
    image(NULL), blob(NULL), vbo_geometry(0), vbo_texture(0) {}

Object::Object(const & Object src) : 
    model(src.model), 
    view(src.view), 
    projection(src.view), 
    numVertices(src.numVertices), 
    image(new Magick::Image( src.image ) ), 
    blob( new Magick::Blob( src.blob ) ) 
    vbo_geometry(src.vbo_geometry), 
    vbo_texture(src.vbo_texture)
{
	//copy each float in the vertex
	for(int i = 0; i < numVertices; i++)
	{
		for(int j = 0; j < 3; j++)
			this->geometry[i].position[j] = src->geometry[i].position[j];

		for(int k = 0; k < 2; k++)
			this->geometry[i].uv[k] = src->geometry[i].uv[k];
	}
}

Object::~Object()
{
	//delete all the things!
	delete [] geometry;
	delete image;
	delete blob;
}

Object& Object::operator=(const & Object src)
{
	//only copy stuff if it is not itself
	if(this != &src)
	{
		this->numVertices = src->numVertices;
		geometry = new Vertex[numVertices];
		blob = new Magick::Blob( (*src.blob) );
		image = new Magick::Image( (*src.image) );
        vbo

		for(int i = 0; i < numVertices; i++)
		{
			for(int j = 0; j < 3; j++)
				this->geometry[i].position[j] = src->geometry[i].position[j];

			for(int k = 0; k < 2; k++)
				this->geometry[i].uv[k] = src->geometry[i].uv[k];
		}
	}
}

bool Object::loadObject(const char * path)
{
	//Initialize the loader stuff
	Assimp::Importer importer;
    const aiScene * scene = importer.ReadFile( path, aiProcess_Triangulate);

    //only proceed if the file imported
    if(aiScene != NULL)
    {
    	//get the stuff from the scene
		aiMesh * mesh = scene->mMeshes[0];

		numVertices = mesh->mNumFaces * 3;
		geometry = new Vertex[ numVertices ];

		for( unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
		    const aiFace & face = mesh->mFaces[i];

		    for(unsigned int j = 0; j < 3; j++)
		    {
		        aiVector3D pos = mesh->mVertices[face.mIndices[j]];
		        geometry->position[0] = pos.x;
		        geometry->position[1] = pos.y;
		        geometry->position[2] = pos.z;

		        aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[j]];
		        geometry->uv[0] = uv.x;
		        geometry->uv[1] = uv.y;
		        geometry++;
		    }
		}
		geometry -= mesh->mNumFaces * 3;

		//return true cause you found the stuff!
		return true;
    }

    cout << "File " << path << " did note load correctly." << endl;
    //return false cause you didn't find the stuff
    return false;
}

bool Object::loadTexture(const char * path)
{
	//Try and get the model 
	try 
	{
	   	image = new Magick::Image(m_fileName);
	   	image->write(blob, "RGBA");
	}

	//catch and return false if it failed
	catch (Magick::Error& Error) 
	{
	   std::cout << "Error loading texture '" << m_fileName << "': " << Error.what() << std::endl;
	   return false;
	}
	return true;
}

Vertex * Object::getGeometry() const
{
	return geometry;
}

int Object::getNumVertices() const
{
	return numVertices;
}

Magick::Image * Object::getImage() const;
{
	return image;
}

Magick::Blob * Object::getBlob() const
{
	return blob;
}

vector<Object> * initialize(GLuint & program, GLuint * vbo_geometry, GLuint * vbo_texture)
{
    
    //Initialize objects to put into the list;
    Object board, puck, paddleOne, paddleTwo;

    //If any of the load objects doesn't work return;
    if( !board.loadObject("board.obj") ||
    	!puck.loadObject("puck.obj") ||
    	!paddleOne.loadObject("paddle.obj") ||
    	!paddleTwo.loadObject("paddle.obj") )
    	return NULL;

    //If any of the load textures doesn't work return;
    if( !board.loadTexture("board.jpg") ||
    	!puck.loadTexture("puck.jpg") ||
    	!paddleOne.loadTexture("paddle.jpg") ||
    	!paddleTwo.loadTexture("paddle.jpg") )
    	return NULL;

    //push each object into the vector
    vector<Object> * objectVector;
    objectVector->push_back(BOARD);
    objectVector->push_back(PUCK);
    objectVector->push_back(PADDLE_ONE);
    objectVector->push_back(PADDLE_TWO);
    

    //for readibilities sake!
    int numObjects = objVector->size();

    //create the vbo arrays based on the numbers objects in the vector
    GLuint * newVertexBuffers = new GLuint[numObjects];
    GLuint * newTextureBuffers = new GLuint[numObjects];

    //generate buffers based on the number of objects
    glGenBuffers(numObjects, newVertexBuffers);
    glGenTextures(numObjects, newTextureBuffers);

    //loop through each object to bind it's textures
    for( int i = 0; i < numObjects; i++ )
    {
    	//Bind the geometry
    	glBindBuffer(GL_ARRAY_BUFFER, newTextureBuffers);
    	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), (*objectVector)[i].getGeometry(), GL_STATIC_DRAW);

        //Set the vbogeometry to the object
        objectVector[i]->setVertexBuffer(newVertexBuffers[i]);

    	//Bind the textures
		glActiveTexture(GL_TEXTURE0);
	    glBindTexture(GL_TEXTURE_2D, vbo_texture[i]);
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (*objectVector)[i].getImage()->columns(), 
	    	(*objectVector)[i].getImage()->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 
	    	(*objectVector)[i].getBlob()->data() );
	    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //set the vbotexture to the object 
        objectVector[i]->setTextureBuffer(newTextureBuffers[i]);   
	}

	//Initialize Loaders for shaders
    ShaderLoader vertShader, fragShader;

    //Set the shaders
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);


	//Note the added uniform!
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

    Object staticAccesser;

    //Now we set the locations of the attributes and uniforms
    //this allows us to access them easily while rendering
    staticAccesser.loc_position = glGetAttribLocation(program,
                    const_cast<const char*>("v_position"));
    if(loc_position == -1)
    {
        std::cerr << "[F] POSITION NOT FOUND" << std::endl;
        return false;
    }

    staticAccesser.loc_texture = glGetAttribLocation(program,
                    const_cast<const char*>("v_tex"));
    if(texAttrib == -1)
    {
        std::cerr << "[F] V_TEX NOT FOUND" << std::endl;
        return false;
    }

    staticAccess.loc_mvpmat = glGetUniformLocation(program,
                    const_cast<const char*>("mvpMatrix"));
    if(loc_mvpmat == -1)
    {
        std::cerr << "[F] MVPMATRIX NOT FOUND" << std::endl;
        return false;
    }
    
    staticAccesser.program = program;

    //--Init the view and projection matrices
    //  if you will be having a moving camera the view matrix will need to more dynamic
    //  ...Like you should update it before you render more dynamic 
    //  for this project having them static will be fine
    staticAccesser.view = glm::lookAt( glm::vec3(0.0, 8.0, -16.0), //Eye Position
                        glm::vec3(0.0, 0.0, 0.0), //Focus point
                        glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

    staticAccesser.projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                   float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                   0.01f, //Distance to the near plane, normally a small value like this
                                   100.0f); //Distance to the far plane, 

    //enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

void initializeGlut()
{
	// Initialize glut
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(w, h);
    // Name and create the Window
    glutCreateWindow("Solar System");

    // Create menu
    glutCreateMenu(menu);
    glutAddMenuEntry("Exit", 1);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void cleanUp()
{
    Object staticAccesser;
	glDeleteProgram(staticAccesser.program);
    glDeleteBuffers(gameObjects->size(), staticAccesser.vbo_geometry);
    glDeleteBuffers(gameObjects->size(), staticAccesser.vbo_texture);
}

void Object::setVertexBuffer(GLuint vbo) { vbo_geometry = vbo; }

void Object::setTextureBuffer(GLuint vbo) { vbo_texture = vbo; }

float getDT()
{
    float ret;
    t2 = std::chrono::high_resolution_clock::now();
    ret = std::chrono::duration_cast< std::chrono::duration<float> >(t2-t1).count();
    t1 = std::chrono::high_resolution_clock::now();
    return ret;
}
