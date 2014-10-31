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

enum object {FLOOR, CUBE, CYL, SPHERE};

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
	Object(const Object & src);
	~Object();
	Object& operator=(const Object & src);

	bool loadObject(const char * path);
	bool loadTexture(const char * path);

    //getters
    Vertex * getGeometry() const;
    int getNumVertices() const;
    Magick::Blob * getBlob() const;
    Magick::Image * getImage() const;

    //setters
    void setVertexBuffer(GLuint vbo);
    void setTextureBuffer(GLuint vbo);
    void setCollisionShape();

    glm::mat4 model;
    GLuint vbo_geometry;
    GLuint vbo_texture;
	Vertex * geometry;
	int numVertices;
	Magick::Image * image;
	Magick::Blob * blob;
    bool movingDynamic;
    btCollisionShape * collisionShape;
    
};

//Global Variables
vector<Object> * gameObjects;
int w, h;
glm::mat4 view;
glm::mat4 projection;
GLuint program;
GLint loc_position;
GLint loc_texture;
GLint loc_mvpmat;

int sphereX =0;
int sphereZ =0; 
int cylX =0;
int cylZ =0;



    // BULLET initialize
    //build broadphase
    btBroadphaseInterface* broadphase = new btDbvtBroadphase();

    // set up the collision config, and dispatcher
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
    // actual physics solver
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

    // the world
    btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    // declare ground collisionshape and sphere and cylinder and cube
    btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
    btCollisionShape* frontShape = new btStaticPlaneShape(btVector3(0,0,1),1);
    /*
    btCollisionShape* backShape = new btStaticPlaneShape(btVector3(0,0,1),1);
    btCollisionShape* rightShape = new btStaticPlaneShape(btVector3(1,0,0),1);
    btCollisionShape* leftShape = new btStaticPlaneShape(btVector3(1,0,0),1);
    */
    btCollisionShape* fallShape = new btSphereShape(1);
    btCollisionShape* cylinderShape = new btCylinderShape(btVector3(1,1,0));
    btCollisionShape* cubeShape = new btBoxShape(btVector3(1,1,1));

    //declare ground motion state
    btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
    btRigidBody::btRigidBodyConstructionInfo
    groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
    btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
    //declare front wall
    /*  btDefaultMotionState* frontMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 10)));
    btRigidBody::btRigidBodyConstructionInfo
    frontRigidBodyCI(0, frontMotionState, frontShape, btVector3(0, 0, 0));
    btRigidBody* frontRigidBody = new btRigidBody(frontRigidBodyCI);
    //declare back wall
    btDefaultMotionState* backMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, -10)));
    btRigidBody::btRigidBodyConstructionInfo
    backRigidBodyCI(0, backMotionState, backShape, btVector3(0, 0, 0));
    btRigidBody* backRigidBody = new btRigidBody(backRigidBodyCI);
    //declare right wall
    btDefaultMotionState* rightMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(10, 0, 0)));
    btRigidBody::btRigidBodyConstructionInfo
    rightRigidBodyCI(0, rightMotionState, rightShape, btVector3(0, 0, 0));
    btRigidBody* rightRigidBody = new btRigidBody(rightRigidBodyCI);
    //declare left wall
    btDefaultMotionState* leftMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-10, 0, 0)));
    btRigidBody::btRigidBodyConstructionInfo
    leftRigidBodyCI(0, leftMotionState, leftShape, btVector3(0, 0, 0));
    btRigidBody* leftRigidBody = new btRigidBody(leftRigidBodyCI);
    */ //declare sphere motion state
    btDefaultMotionState* fallMotionState =
    new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));
    btScalar mass = 1;
    btVector3 fallInertia(0, 0, 0);
    btRigidBody* fallRigidBody; 
    //declare cylinder motion state
    btDefaultMotionState* cylinderMotionState =
    new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
    btRigidBody* cylinderRigidBody; 
    //declare cube motion state
    btDefaultMotionState* cubeMotionState =
    new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-3, 0, 0)));
    btRigidBody::btRigidBodyConstructionInfo
    cubeRigidBodyCI(0, cubeMotionState, cubeShape, btVector3(0, 0, 0));
    btRigidBody* cubeRigidBody = new btRigidBody(cubeRigidBodyCI);

//--GLUT Callbacks
void render();
void update();
void reshape(int n_w, int n_h);
void keyboard(unsigned char key, int x_pos, int y_pos);
void arrowKey( int key, int x_pos, int y_pos );
void mouse(int button, int state, int x_pos, int y_pos);
void menu(int id);

//Start and enders for the program
vector<Object> * initialize();
void cleanUp();

//Time things
float getDT();
std::chrono::time_point<std::chrono::high_resolution_clock> t1,t2;

btVector3 toBtVec3(Vertex vec);


int main(int argc, char **argv)
{
    gameObjects = new vector<Object>;
	//This object is only used to be able to be able to access the static members in the class
	Object bufferHolder;
    
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
    glutSpecialFunc(arrowKey);
    // Initialize all of our resources(shaders, geometry)
    //Initialize the objects
    gameObjects = initialize();
    if(gameObjects != NULL)
    {
        t1 = std::chrono::high_resolution_clock::now();
        glutMainLoop();
    }

    cleanUp();
    return 0;
}

void render()
{
    //clear the screen
    glClearColor(0.0, 0.0, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(unsigned int i = 0; i < gameObjects->size(); i++)
    {
        //premultiply the matrix for this example
        glm::mat4 mvp = projection * view * (*gameObjects)[i].model;

        //enable the shader program
        glUseProgram(program);

        //upload the matrix to the shader
        glUniformMatrix4fv(loc_mvpmat, 1, GL_FALSE, glm::value_ptr(mvp));

        //set up the Vertex Buffer Object so it can be drawn
        glEnableVertexAttribArray(loc_position);
        glEnableVertexAttribArray(loc_texture);
        glBindBuffer(GL_ARRAY_BUFFER, (*gameObjects)[i].vbo_geometry);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, (*gameObjects)[i].vbo_texture);

        //

        //set pointers into the vbo for each of the attributes(position and color)
        glVertexAttribPointer( loc_position,//location of attribute
                               3,//number of elements
                               GL_FLOAT,//type
                               GL_FALSE,//normalized?
                               sizeof(Vertex),//stride
                               0);//offset

        glVertexAttribPointer( loc_texture,
                               2,
                               GL_FLOAT,
                               GL_FALSE,
                               sizeof(Vertex),
                               (void*)offsetof(Vertex,uv));

        glDrawArrays(GL_TRIANGLES, 0, (*gameObjects)[i].numVertices);//mode, starting index, count

        //clean up
        glDisableVertexAttribArray(loc_position);
        glDisableVertexAttribArray(loc_texture);
    }
                           
    //swap the buffers
    glutSwapBuffers();
}

void update()
{
    float dt = getDT();// if you have anything moving, use dt.

    btTransform trans;
    btScalar m[16];
    
    fallRigidBody->applyCentralImpulse(btVector3(sphereX, 0, sphereZ));
    fallRigidBody->getMotionState()->getWorldTransform(trans);
    trans.getOpenGLMatrix(m);
    sphereX = 0;
    sphereZ = 0;
    (*gameObjects)[SPHERE].model = glm::make_mat4(m);

    cylinderRigidBody->applyCentralImpulse(btVector3(cylX, 0, cylZ));
    cylinderRigidBody->getMotionState()->getWorldTransform(trans);
    trans.getOpenGLMatrix(m);
    cylX = 0;
    cylZ = 0;
    (*gameObjects)[CYL].model = glm::make_mat4(m);

    groundRigidBody->getMotionState()->getWorldTransform(trans);
    trans.getOpenGLMatrix(m);
    (*gameObjects)[FLOOR].model = glm::make_mat4(m);

    cubeRigidBody->getMotionState()->getWorldTransform(trans);
    trans.getOpenGLMatrix(m);
    (*gameObjects)[CUBE].model = glm::make_mat4(m);

    dynamicsWorld->stepSimulation(dt,10);

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
    Object staticAccesser;
    projection = glm::perspective(45.0f, float(w)/float(h), 0.01f, 100.0f);
}
void keyboard(unsigned char key, int x_pos, int y_pos)
{
     // Handle keyboard input
    if(key == 27)//ESC
    {
        exit(0);
    }
    else if( key == 115 )// S
       {
     // move sphere back
        sphereZ--;
       }
    else if( key == 97 )// A
       {
        // move sphere left
          sphereX++;
       }

    else if( key == 119 ) // W
      {
    // move sphere forward
           sphereZ++;
      }
    else if( key == 100 ) // D
      {
       // Move sphere right
      sphereX--;
      }
}

void arrowKey( int key, int x_pos, int y_pos  )
{
    if( key == GLUT_KEY_RIGHT) // ->
      {
    // move cylinder right
          cylX--;
      }
    else if( key == GLUT_KEY_UP) // ->
      {
    // move cylinder back
          cylZ++;
      }
    else if( key == GLUT_KEY_DOWN) // ->
      {
    // move cylinder forward
          cylZ--;
      }
     else if( key == GLUT_KEY_LEFT ) // <-
      {
    // move cylinder left
          cylX++;
      }

}

void mouse(int button, int state, int x_pos, int y_pos) {}

void menu(int id)
{
    switch(id)
    {
        case 1:
          exit(0); //start spinning case
          break;
    }
}

Object::Object() : model(1.0f), vbo_geometry(0), vbo_texture(0), geometry(NULL), numVertices(0), 
    image(new Magick::Image), blob(new Magick::Blob) {}

Object::Object(const Object & src) : 
    model(src.model), 
    vbo_geometry(src.vbo_geometry), 
    vbo_texture(src.vbo_texture),
    numVertices(src.numVertices), 
    image(new Magick::Image( *(src.image) ) ), 
    blob( new Magick::Blob( *(src.blob) ) )
    
{
    geometry = new Vertex[numVertices];
	//copy each float in the vertex
	for(int i = 0; i < numVertices; i++)
	{
		for(int j = 0; j < 3; j++)
			this->geometry[i].position[j] = src.geometry[i].position[j];

		for(int k = 0; k < 2; k++)
			this->geometry[i].uv[k] = src.geometry[i].uv[k];
	}
}

Object::~Object()
{
	//delete all the things!
	delete [] geometry;
	delete image;
	delete blob;
}

Object& Object::operator=(const Object & src)
{
	//only copy stuff if it is not itself
	if(this != &src)
	{
		this->numVertices = src.numVertices;
		this->geometry = new Vertex[numVertices];
		this->blob = new Magick::Blob( *(src.blob) );
		this->image = new Magick::Image( *(src.image) );
        this->vbo_texture = src.vbo_texture;
        this->vbo_geometry = src.vbo_geometry;

		for(int i = 0; i < numVertices; i++)
		{
			for(int j = 0; j < 3; j++)
				this->geometry[i].position[j] = src.geometry[i].position[j];

			for(int k = 0; k < 2; k++)
				this->geometry[i].uv[k] = src.geometry[i].uv[k];
		}
	}
    return * this;
}

bool Object::loadObject(const char * path)
{
	numVertices = 0;
    Assimp::Importer importer;
    const aiScene * scene = importer.ReadFile( path, aiProcess_Triangulate);

    if(scene != NULL)
    {

        aiMesh ** mesh = new aiMesh * [scene->mNumMeshes];

        for(unsigned int i = 0; i < scene->mNumMeshes; i++)
        {
            mesh[i] = scene->mMeshes[i];
            for(unsigned int j = 0; j < mesh[i]->mNumFaces; j++)
            {
                numVertices += mesh[i]->mFaces[j].mNumIndices;  
            } 
        }

        geometry = new Vertex[ numVertices ];

        for(unsigned int h = 0; h < scene->mNumMeshes; h++)
        {

            for( unsigned int i = 0; i < mesh[h]->mNumFaces; i++)
            {

                const aiFace & face = mesh[h]->mFaces[i];
                for(unsigned int j = 0; j < 3; j++)
                {

                    aiVector3D pos = mesh[h]->mVertices[ face.mIndices[j] ];
                    
                    geometry->position[0] = pos.x;
                    geometry->position[1] = pos.y;
                    geometry->position[2] = pos.z;

                    aiVector3D uv = mesh[h]->mTextureCoords[0][ face.mIndices[j] ];

                    geometry->uv[0] = uv.x;
                    geometry->uv[1] = uv.y;
                    geometry++;
                }
            }
        }

        geometry -= numVertices;

        printf("%s has %d vertices\n", path, numVertices / 3);
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
	   	image = new Magick::Image(path);
	   	image->write(blob, "RGBA");
	}

	//catch and return false if it failed
	catch (Magick::Error& Error) 
	{
	   std::cout << "Error loading texture '" << path << "': " << Error.what() << std::endl;
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

Magick::Image * Object::getImage() const
{
	return image;
}

Magick::Blob * Object::getBlob() const
{
	return blob;
}

vector<Object> * initialize()
{
    
    //initialize dynamics world
    dynamicsWorld->setGravity(btVector3(0,-9.81, 0));
    // put ground rigid body in to dynamics world
    dynamicsWorld->addRigidBody(groundRigidBody);
    
    //calculate inertia and mass put sphere rigid body in to dynamics world
    fallShape->calculateLocalInertia(mass, fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
    fallRigidBody = new btRigidBody(fallRigidBodyCI);    
    dynamicsWorld->addRigidBody(fallRigidBody);
    //cylinder
    cylinderShape->calculateLocalInertia(mass, fallInertia);
    btRigidBody::btRigidBodyConstructionInfo cylinderRigidBodyCI(mass, cylinderMotionState, cylinderShape, fallInertia);
    cylinderRigidBody = new btRigidBody( cylinderRigidBodyCI);
    dynamicsWorld->addRigidBody(cylinderRigidBody);
    //cube
    dynamicsWorld->addRigidBody(cubeRigidBody);

    //Initialize objects to put into the list;
    Object board, puck, paddleOne, paddleTwo;




    //If any of the load objects doesn't work return;
    if( !board.loadObject("objects/tablev1.obj") ||
    	!puck.loadObject("objects/puck.obj") ||
    	!paddleOne.loadObject("objects/paddle.obj") ||
    	!paddleTwo.loadObject("objects/paddle.obj") )
    	return NULL;

    //If any of the load textures doesn't work return;
    if( !board.loadTexture("Grey.jpg") ||
    	!puck.loadTexture("texture_earth_clouds.jpg") ||
    	!paddleOne.loadTexture("texture_earth_clouds.jpg") ||
    	!paddleTwo.loadTexture("texture_earth_clouds.jpg") )
    	return NULL;

    //set if the object is a dynamic moving object
    board.movingDynamic = false;
    puck.movingDynamic = true;
    paddleOne.movingDynamic = true;
    paddleTwo.movingDynamic = true;

    //set the collision shapes
    board.setCollisionShape();
    puck.setCollisionShape();
    paddleOne.collisionShape = new btCylinderShape( btVector3(1,1,0) );
    paddleTwo.collisionShape = new btCylinderShape( btVector3(1,1,0) );

    //push each object into the vector
    vector<Object> * objectVector = new vector<Object>;
    
    objectVector->push_back(board);
    objectVector->push_back(puck);
    objectVector->push_back(paddleOne);
    objectVector->push_back(paddleTwo);
    

    //for readibilities sake!
    int numObjects = objectVector->size();

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
    	glBindBuffer(GL_ARRAY_BUFFER, newTextureBuffers[i]);
    	glBufferData(GL_ARRAY_BUFFER, (*objectVector)[i].numVertices * sizeof(Vertex), (*objectVector)[i].getGeometry(), GL_STATIC_DRAW);

        //Set the vbogeometry to the object
        (*objectVector)[i].setVertexBuffer(newVertexBuffers[i]);

    	//Bind the textures
		glActiveTexture(GL_TEXTURE0);
	    glBindTexture(GL_TEXTURE_2D, newTextureBuffers[i]);
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (*objectVector)[i].getImage()->columns(), 
	    	(*objectVector)[i].getImage()->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 
	    	(*objectVector)[i].getBlob()->data() );
	    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //set the vbotexture to the object 
        (*objectVector)[i].setTextureBuffer(newTextureBuffers[i]);   
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

		
        return NULL;
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

        return NULL;
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
        return NULL;
    }

    Object staticAccesser;

    //Now we set the locations of the attributes and uniforms
    //this allows us to access them easily while rendering
    loc_position = glGetAttribLocation(program,
                    const_cast<const char*>("v_position"));
    if(loc_position == -1)
    {
        std::cerr << "[F] POSITION NOT FOUND" << std::endl;
        return NULL;
    }

    loc_texture = glGetAttribLocation(program,
                    const_cast<const char*>("v_tex"));
    if(loc_texture == -1)
    {
        std::cerr << "[F] V_TEX NOT FOUND" << std::endl;
        return NULL;
    }

    loc_mvpmat = glGetUniformLocation(program,
                    const_cast<const char*>("mvpMatrix"));
    if(loc_mvpmat == -1)
    {
        std::cerr << "[F] MVPMATRIX NOT FOUND" << std::endl;
        return NULL;
    }
    
    program = program;

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

    return objectVector;
}

void cleanUp()
{
    Object staticAccesser;
	glDeleteProgram(program);
    //glDeleteBuffers(gameObjects->size(), vbo_geometry);
    //glDeleteBuffers(gameObjects->size(), vbo_texture);
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

void Object::setCollisionShape()
{
    btTriangleMesh* trimesh = new btTriangleMesh();
    for (int i=0;i<numVertices / 3 ; i = i + 3)
    { 
        trimesh->addTriangle(toBtVec3(geometry[i] ), toBtVec3(geometry[i+1] ), toBtVec3(geometry[i+2]) );
    }

    if(movingDynamic)
        collisionShape = new btConvexTriangleMeshShape(trimesh);

    else
    {
        bool useQuantization = true;
        collisionShape = new btBvhTriangleMeshShape(trimesh,useQuantization);
    }
}

btVector3 toBtVec3( Vertex vec)
{
    return btVector3(vec.position[0], vec.position[1], vec.position[2]);
}
