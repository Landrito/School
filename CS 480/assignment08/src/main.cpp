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

#include <math.h>

#include "ShaderLoader.h" 

#include <btBulletDynamicsCommon.h>
using namespace std;

#define BIT(x) (1<<(x))
const short COL_NOTHING = 0; //<collide with invisibleWall
const short COL_PUCK = BIT(0); //<collide with puck
const short COL_PADDLE_ONE = BIT(1); //<collide with paddleone
const short COL_PADDLE_TWO= BIT(2); //<collide with paddletwo
const short COL_WALL = BIT(3);
const short COL_TABLE = BIT(4);
	

short wallCollidesWith = COL_PADDLE_ONE | COL_PADDLE_TWO | COL_TABLE ;
short puckCollidesWith = COL_PADDLE_ONE | COL_PADDLE_TWO | COL_TABLE;
short paddleOneCollidesWith = COL_PUCK | COL_WALL | COL_TABLE;
short paddleTwoCollidesWith = COL_PUCK | COL_WALL | COL_TABLE;
short tableCollidesWith = COL_PADDLE_ONE | COL_PADDLE_TWO | COL_PUCK | COL_WALL;

enum object {TABLE, PADDLEONE, PADDLETWO, PUCK};

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
	btTriangleMesh* trimesh;
};

//////////////////////////////////////////////Global Variables///////////////////////////////////////
vector<Object> * gameObjects;
int w, h;
glm::mat4 view;
glm::mat4 projection;
GLuint program;
GLint loc_position;
GLint loc_texture;
GLint loc_mvpmat;
bool pause = false;
int redScore = 0;
int blueScore = 0;
int scoreTimer = 0;
btTransform startingTransform;

int paddleOneX =0;
int paddleOneZ =0; 
int paddleTwoX =0;
int paddleTwoZ =0;

  /////////////////////////////////////////// BULLET initialize////////////////////////////////////
    //BUILD BROADPHASE
    btBroadphaseInterface* broadphase = new btDbvtBroadphase();

    // SET UP THE COLLISION CONFIGURATION AND DISPATCHER
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
    // ACTUAL PHYSICS SOLVER
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

    // DECLARE THE DYNAMIC WORLD
    btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    ////////////////////////////////////////////////////////////////////////////////////////////////////
 
btRigidBody* puckRigidBody; 
btRigidBody* paddleOneRigidBody;
btRigidBody* paddleTwoRigidBody; 
btRigidBody* tableRigidBody;
btRigidBody* invisibleWallRigidBody;



//--GLUT Callbacks
void render();
void update();
void reshape(int n_w, int n_h);
void keyboard(unsigned char key, int x_pos, int y_pos);
void arrowKey( int key, int x_pos, int y_pos );
void mouse(int button, int state, int x_pos, int y_pos);
void mouseMove(int x_pos, int y_pos);
void menu(int id);

//Start and enders for the program
vector<Object> * initialize();
void cleanUp();

//Time things
float getDT();
std::chrono::time_point<std::chrono::high_resolution_clock> t1,t2;




int main(int argc, char **argv)
{
   
    gameObjects = new vector<Object>;
    // Initialize glut
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(w, h);
    // Name and create the Window
    glutCreateWindow("AIR HOCKEY");

    // Create menu
    glutCreateMenu(menu);
    glutAddMenuEntry("Start", 1);
    glutAddMenuEntry("Stop", 2);
    glutAddMenuEntry("Pause", 3);
    glutAddMenuEntry("Harris-ify", 4);
    glutAddMenuEntry("Normal-ify", 5);
    glutAddMenuEntry("Exit", 6);
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
    glutMotionFunc(mouseMove);
    glutMouseFunc(mouse);
    glutSpecialFunc(arrowKey);
    // Initialize all of our resources(shaders, geometry)
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

    glUseProgram(0);

    string displayString = "Red Team: " + to_string(redScore) + "   Blue Team: " + to_string(blueScore);


    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);    // A
    glRasterPos2f(-0.5,0.9);     // B
    for( size_t i = 0; i < displayString.size(); ++i )
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, displayString[i]);
    }


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

bool tableRendered = false;

void update()
{
	if(!pause)
	{
		

	    float dt = getDT();// if you have anything moving, use dt.

	    btTransform trans;
	    btScalar m[16];
	    btVector3 v;

	    if(paddleOneX != 0 || paddleOneZ != 0){
	    	paddleOneRigidBody->applyCentralImpulse(btVector3(paddleOneX, 0, paddleOneZ));
	    	paddleOneX = 0;
	    	paddleOneZ = 0;
	    }
	    paddleOneRigidBody->getMotionState()->getWorldTransform(trans);
	    trans.getOpenGLMatrix(m);
	    (*gameObjects)[PADDLEONE].model = glm::make_mat4(m);


	    if(paddleTwoX != 0 || paddleTwoZ != 0){	
	    	paddleTwoRigidBody->applyCentralImpulse(btVector3(paddleTwoX, 0, paddleTwoZ));
	    	paddleTwoX = 0;
	    	paddleTwoZ = 0;
	    }
	    paddleTwoRigidBody->getMotionState()->getWorldTransform(trans);
	    trans.getOpenGLMatrix(m);
	    (*gameObjects)[PADDLETWO].model = glm::make_mat4(m);


	    if(!tableRendered){
	    	tableRendered = true;
	    	tableRigidBody->getMotionState()->getWorldTransform(trans);
	    	trans.getOpenGLMatrix(m);
	   		(*gameObjects)[TABLE].model = glm::make_mat4(m);
	    }
	   
	   puckRigidBody->getMotionState()->getWorldTransform(trans);
	    trans.getOpenGLMatrix(m);
	    glm::mat4 transformMatrix = glm::make_mat4(m);
	    glm::vec4 puckPosition = transformMatrix * glm::vec4(1.0f);

	    if(puckPosition.x < -12.1 && puckPosition.z < 2.5 && puckPosition.z > -2.5){
	    	puckRigidBody->setWorldTransform(startingTransform);
	    	puckRigidBody->setLinearVelocity(btVector3(0,0,0));
	    	if(scoreTimer == 0){
	    		redScore++;
	    		scoreTimer = 10;
	    	}
	    	
	    } else if(puckPosition.x > 8.7 && puckPosition.z < 2.5 && puckPosition.z > -2.5){
	    	puckRigidBody->setWorldTransform(startingTransform);
	    	puckRigidBody->setLinearVelocity(btVector3(0,0,0));
	    	if(scoreTimer == 0){
		    	blueScore++;
		    	scoreTimer = 10;
	    	}
	    } else if(puckPosition.z > 6 || puckPosition.z < -6) {
	    	puckRigidBody->setWorldTransform(startingTransform);
	    	puckRigidBody->setLinearVelocity(btVector3(0,0,0));
	    }

	    if(scoreTimer > 0){
		    scoreTimer--;
	    }


	    (*gameObjects)[PUCK].model = glm::make_mat4(m);

	    dynamicsWorld->stepSimulation(dt, 5);

	    glm::vec3 target_pos; // up will stay the same
	    target_pos.x = puckPosition.x / 9;
	    target_pos.y = puckPosition.y / 9;
	    target_pos.z = puckPosition.z / 9;

	   	view = glm::lookAt( glm::vec3(0.0, 12.0, -16.0),
		target_pos, 
		glm::vec3( 0.0, 1.0, 0.0));

	    // Update the state of the scene
	    glutPostRedisplay();//call the display callback
	}
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
     // move PADDLEONE back
        paddleOneZ--;
       }
    else if( key == 97 )// A
       {
        // move PADDLEONE left
          paddleOneX++;
       }

    else if( key == 119 ) // W
      {
    // move PADDLEONE forward
           paddleOneZ++;
      }
    else if( key == 100 ) // D
      {
       // Move PADDLEONE right
      	   paddleOneX--;
      }
}

void arrowKey( int key, int x_pos, int y_pos  )
{
    if( key == GLUT_KEY_RIGHT) // ->
      {
    // move PADDLETWO right
          paddleTwoX--;
      }
    else if( key == GLUT_KEY_UP) // ->
      {
    // move PADDLETWO back
          paddleTwoZ++;
      }
    else if( key == GLUT_KEY_DOWN) // ->
      {
    // move PADDLETWO forward
          paddleTwoZ--;
      }
     else if( key == GLUT_KEY_LEFT ) // <-
      {
    // move PADDLETWO left
          paddleTwoX++;
      }

}

void mouse(int button, int state, int x_pos, int y_pos) {}


int previous_x_pos = 0;
int previous_y_pos = 0;

void mouseMove(int x_pos, int y_pos)
{
	int xDel = x_pos - previous_x_pos;
	int yDel = y_pos - previous_y_pos;
	if( abs(xDel) > abs(yDel) ){
		paddleTwoX = -xDel / abs(xDel);
	} else {
		paddleTwoZ = -yDel / abs(yDel);
	}
	previous_x_pos = x_pos;
	previous_y_pos = y_pos;
}

void menu(int id)
{
    switch(id)
    {	
    	case 1:
    		pause = false;
    		break;
    	case 2:
    		pause = true;
    		break;
    	case 3:
    		pause = !pause;
    		break;
    	case 4:
    		(*gameObjects)[TABLE].loadTexture("harris.jpg");

	    	//Bind the textures
			glActiveTexture(GL_TEXTURE0);
		    glBindTexture(GL_TEXTURE_2D, (*gameObjects)[TABLE].vbo_texture);
		    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (*gameObjects)[TABLE].getImage()->columns(), 
		    	(*gameObjects)[TABLE].getImage()->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 
		    	(*gameObjects)[TABLE].getBlob()->data() );
		    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    		break;
    	case 5:
    		(*gameObjects)[TABLE].loadTexture("Grey.jpg");

	    	//Bind the textures
			glActiveTexture(GL_TEXTURE0);
		    glBindTexture(GL_TEXTURE_2D, (*gameObjects)[TABLE].vbo_texture);
		    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (*gameObjects)[TABLE].getImage()->columns(), 
		    	(*gameObjects)[TABLE].getImage()->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 
		    	(*gameObjects)[TABLE].getBlob()->data() );
		    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    		break;
        case 6:
          exit(0); //start spinning case
          break;
    }
}

Object::Object() : model(1.0f), vbo_geometry(0), vbo_texture(0), geometry(NULL), numVertices(0), 
    image(new Magick::Image), blob(new Magick::Blob), trimesh(NULL){}

Object::Object(const Object & src) : 
    model(src.model), 
    vbo_geometry(src.vbo_geometry), 
    vbo_texture(src.vbo_texture),
    numVertices(src.numVertices),
    image(new Magick::Image( *(src.image) ) ), 
    blob( new Magick::Blob( *(src.blob) ) ),
    trimesh(new btTriangleMesh(*(src.trimesh)))
    
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
		this->trimesh = new btTriangleMesh(*(src.trimesh) );
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
	trimesh = new btTriangleMesh();

        for(unsigned int i = 0; i < scene->mNumMeshes; i++)
        {
            mesh[i] = scene->mMeshes[i];
            for(unsigned int j = 0; j < mesh[i]->mNumFaces; j++)
            {
                numVertices += mesh[i]->mFaces[j].mNumIndices;  
            } 
        }

        geometry = new Vertex[ numVertices ];
	btVector3 * array = new btVector3[3];

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
		    
		    //trimesh
		    array[j]=btVector3(pos.x, pos.y,pos.z);

                    aiVector3D uv = mesh[h]->mTextureCoords[0][ face.mIndices[j] ];

                    geometry->uv[0] = uv.x;
                    geometry->uv[1] = uv.y;
                    geometry++;
                }
		trimesh->addTriangle(array[0],array[1],array[2]);
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
   

//Initialize objects to put into the list;
    Object table, paddleOne, paddleTwo, puck;

//If any of the load objects doesn't work return;
    if( !table.loadObject("objects/simple_table.obj") ||
    	!puck.loadObject("objects/big_puck.obj") ||
    	!paddleOne.loadObject("objects/paddle.obj") ||
    	!paddleTwo.loadObject("objects/paddle.obj") )
    	return NULL;

    //If any of the load textures doesn't work return;
    if( !table.loadTexture("Grey.jpg") ||
    	!puck.loadTexture("White.jpg") ||
    	!paddleOne.loadTexture("Maroon.jpg") ||
    	!paddleTwo.loadTexture("Green.jpg") )
    	return NULL;
bool useQuantization = true;
    //declare collisionShapes
    btCollisionShape* tableShape = new btBvhTriangleMeshShape(table.trimesh,useQuantization);
    btCollisionShape* puckShape = new btConvexTriangleMeshShape(puck.trimesh);
    btCollisionShape* paddleOneShape = new btConvexTriangleMeshShape(paddleOne.trimesh);
    btCollisionShape* paddleTwoShape = new btConvexTriangleMeshShape(paddleTwo.trimesh);
    btCollisionShape* invisibleWallShape = new btBoxShape(btVector3(0.1,10,10));
    
    //set motion states
	//table
    	 btDefaultMotionState* tableMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
        //puck
	btDefaultMotionState* puckMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-2.8, 0.5, -1)));
	//paddleOne
	btDefaultMotionState* paddleOneMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(3, 0, 0)));
	//paddleTwo
	btDefaultMotionState* paddleTwoMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-3, 0, 0)));
	//Invisible wall
	btDefaultMotionState* invisibleWallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));

    //construct rigid bodies
    //table
    btRigidBody::btRigidBodyConstructionInfo tableRigidBodyCI(0, tableMotionState, tableShape,btVector3(0, 0, 0));
    tableRigidBodyCI.m_restitution = 0.999f;
	//puck
	btRigidBody::btRigidBodyConstructionInfo puckRigidBodyCI(0.05f, puckMotionState, puckShape,btVector3(0, 0, 0));
	puckRigidBodyCI.m_restitution = 0.999f;
	//paddleOne
	btRigidBody::btRigidBodyConstructionInfo paddleOneRigidBodyCI(2, paddleOneMotionState, paddleOneShape,btVector3(0, 0, 0));
	paddleOneRigidBodyCI.m_restitution = 0.999f;
	//paddleTwo
	btRigidBody::btRigidBodyConstructionInfo paddleTwoRigidBodyCI(2, paddleTwoMotionState, paddleTwoShape,btVector3(0, 0, 0));
	paddleTwoRigidBodyCI.m_restitution = 0.999f;
	//Invisible wall
	btRigidBody::btRigidBodyConstructionInfo invisibleWallRigidBodyCI(0, invisibleWallMotionState, invisibleWallShape,btVector3(0, 0, 0));
	invisibleWallRigidBodyCI.m_restitution = 1.0f;

    //set rigid bodies
	//table
	tableRigidBody = new btRigidBody(tableRigidBodyCI);
	//puck
	puckRigidBody = new btRigidBody(puckRigidBodyCI);
	//paddleOne
	paddleOneRigidBody = new btRigidBody(paddleOneRigidBodyCI);
	//paddleTwo
	paddleTwoRigidBody = new btRigidBody(paddleTwoRigidBodyCI);
	//invisible wall
	invisibleWallRigidBody = new btRigidBody(invisibleWallRigidBodyCI);
	
	paddleOneRigidBody->setLinearFactor(btVector3(1,0,1));
	paddleTwoRigidBody->setLinearFactor(btVector3(1,0,1));
	puckRigidBody->setLinearFactor(btVector3(1,0,1));

	paddleOneRigidBody->setActivationState(DISABLE_DEACTIVATION);
	paddleTwoRigidBody->setActivationState(DISABLE_DEACTIVATION);

    //place in dynamic world
	dynamicsWorld->addRigidBody(tableRigidBody, COL_TABLE, tableCollidesWith);
    dynamicsWorld->addRigidBody(puckRigidBody, COL_PUCK, puckCollidesWith);
	dynamicsWorld->addRigidBody(paddleOneRigidBody, COL_PADDLE_ONE, paddleOneCollidesWith);
	dynamicsWorld->addRigidBody(paddleTwoRigidBody, COL_PADDLE_TWO, paddleTwoCollidesWith);
	dynamicsWorld->addRigidBody(invisibleWallRigidBody, COL_WALL, wallCollidesWith);

    
    //push each object into the vector
    vector<Object> * objectVector = new vector<Object>;
    
    objectVector->push_back(table);
    objectVector->push_back(paddleOne);
    objectVector->push_back(paddleTwo);
    objectVector->push_back(puck);


	
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
    view = glm::lookAt( glm::vec3(0.0, 12.0, -16.0), //Eye Position
                        glm::vec3(0.0, 0.0, 0.0), //Focus point
                        glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

    projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                   float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                   0.01f, //Distance to the near plane, normally a small value like this
                                   100.0f); //Distance to the far plane, 

    //enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    puckRigidBody->getMotionState()->getWorldTransform(startingTransform);

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

