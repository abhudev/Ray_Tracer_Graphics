// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>

#include <vector>
#include <utility>
#include <string>
#include <cstring>
#include <iostream>
#include <cxxopts.hpp>

typedef std::pair<int, int> pii;

// Helper functions
bool read_objs(std::string path, 
			   std::vector<GLfloat*>& vert_vec, 
			   std::vector<GLfloat*>& clr_vec, 
			   std::vector<int>& vsize_vec, 
			   std::vector<int>& csize_vec, 
			   std::vector<int>& n_tri,
			   std::vector<pii>& idx);

void read_xyz(FILE* file, 
			  GLfloat& x, 
			  GLfloat &y, 
			  GLfloat &z);

void read_ray(FILE* file, 
			  int& I, 
			  int& J, 
			  GLfloat& p1x, 
			  GLfloat& p1y, 
			  GLfloat& p1z, 
			  GLfloat& p2x, 
			  GLfloat& p2y, 
			  GLfloat& p2z);

int main(int argc, char **argv)
{
	// :: Command line arguments ::
	cxxopts::Options options("Scene", "Interactive raytracing simulation");
	options.add_options()
        ("w, width", "Window width", cxxopts::value<int>()->default_value("500"))
        ("h, height", "Window height", cxxopts::value<int>()->default_value("500"))        
        ("i, scene", "Input scene (ob)", cxxopts::value<std::string>()->default_value("list_objs.ob"))
        ("x, xcord", "X ray point", cxxopts::value<int>())
        ("y, ycord", "Y ray point", cxxopts::value<int>())
        ;

	auto args = options.parse(argc, argv);

	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( args["w"].as<int>(), args["h"].as<int>(), "Tutorial 0 - Keyboard and Mouse", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
    glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	// Cull triangles which normal is not towards the camera
	// glEnable(GL_CULL_FACE);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader" );

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// Load the texture
	// GLuint Texture = loadDDS("uvtemplate.DDS");
	
	// Get a handle for our "myTextureSampler" uniform
	// GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");	

	enum obtype {OBJECT, RAY};

	// vert_vec - 

	std::vector<GLfloat*> vert_vec;	
	std::vector<GLfloat*> clr_vec;
	std::vector<int> vsize_vec;
	std::vector<int> csize_vec;
	std::vector<int> n_tri;
	std::vector<obtype> vec_obtype;
	std::vector<pii> idx;

	read_objs(args["i"].as<std::string>(), vert_vec, clr_vec, vsize_vec, csize_vec, n_tri, idx);

	int nobj = vert_vec.size();	

	GLuint vertexbuffer[nobj];
	GLuint colorbuffer[nobj];	

	printf("Number of objects: %d\n", nobj);

	for (int i = 0; i < nobj; ++i)
	{
		// Vertex buffer
		glGenBuffers(1, &vertexbuffer[i]);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[i]);
		glBufferData(GL_ARRAY_BUFFER, vsize_vec[i], vert_vec[i], GL_STATIC_DRAW);	

		// Color buffer
		glGenBuffers(1, &colorbuffer[i]);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer[i]);
		glBufferData(GL_ARRAY_BUFFER, csize_vec[i], clr_vec[i], GL_STATIC_DRAW);

		// Number of triangles
		// n_tri[i] = sizeof(vert_vec[i])/sizeof(vert_vec[i][0]);
		// n_tri[i] = 11;
		// printf("Object %d number of triangles = %d\n", i, n_tri[i]);
	}

	// :: Drawing loop ::	

	std::cout<<"Input query point:\n";
	int I, J;
	I = args["x"].as<int>();
	J = args["y"].as<int>();
	// std::cout<<"X: ";
	// std::cin>>I;
	// std::cout<<"Y: ";
	// std::cin>>J;	

		do{

			// Clear the screen
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Use our shader
			glUseProgram(programID);

			// Compute the MVP matrix from keyboard and mouse input
			computeMatricesFromInputs();
			glm::mat4 ProjectionMatrix = getProjectionMatrix();
			glm::mat4 ViewMatrix = getViewMatrix();
			glm::mat4 ModelMatrix = glm::mat4(1.0);
			glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

			// Send our transformation to the currently bound shader, 
			// in the "MVP" uniform
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

			// // Bind our texture in Texture Unit 0
			// glActiveTexture(GL_TEXTURE0);
			// glBindTexture(GL_TEXTURE_2D, Texture);
			// // Set our "myTextureSampler" sampler to use Texture Unit 0
			// glUniform1i(TextureID, 0);

			for (int i = 0; i < nobj; ++i)
			{
				// 1rst attribute buffer : vertices
				glEnableVertexAttribArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[i]);
				glVertexAttribPointer(
					0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
					3,                  // size
					GL_FLOAT,           // type
					GL_FALSE,           // normalized?
					0,                  // stride
					(void*)0            // array buffer offset
				);

				// 2nd attribute buffer : colors
				glEnableVertexAttribArray(1);
				glBindBuffer(GL_ARRAY_BUFFER, colorbuffer[i]);
				glVertexAttribPointer(
					1,                  // attribute. No particular reason for 1, but must match the layout in the shader.
					3,                  // size
					GL_FLOAT,           // type
					GL_FALSE,           // normalized?
					0,                  // stride
					(void*)0            // array buffer offset
				);

				// Draw the triangle ! or the ray!
				if(n_tri[i] > 0)
					glDrawArrays(GL_TRIANGLES, 0, n_tri[i]*3); // 12*3 indices starting at 0 -> 12 triangles
				else {
					if(idx[i].first == I && idx[i].second == J) {
						glLineWidth(5);
						glDrawArrays(GL_LINES, 0, 2);
					}
				}
					

				glDisableVertexAttribArray(0);
				glDisableVertexAttribArray(1);
			}

			// Swap buffers
			glfwSwapBuffers(window);
			glfwPollEvents();

		} // Check if the ESC key was pressed or the window was closed
		while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
			   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO and shader

	for (int i = 0; i < nobj; ++i)
	{
		glDeleteBuffers(1, &vertexbuffer[i]);
		glDeleteBuffers(1, &colorbuffer[i]);
		delete [] vert_vec[i];
		delete [] clr_vec[i];
	}
	
	glDeleteProgram(programID);
	// glDeleteTextures(1, &TextureID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

void read_xyz(FILE* file, 
			  GLfloat& x, 
			  GLfloat &y, 
			  GLfloat &z) {
	
	fscanf(file, "%f", &x);
	fscanf(file, "%f", &y);
	fscanf(file, "%f", &z);		
}

void read_ray(FILE* file, 
			  int& I, 
			  int& J, 
			  GLfloat& p1x, 
			  GLfloat& p1y, 
			  GLfloat& p1z, 
			  GLfloat& p2x, 
			  GLfloat& p2y, 
			  GLfloat& p2z) {
	
	fscanf(file, "%d", &I);
	fscanf(file, "%d", &J);
	fscanf(file, "%f", &p1x);  
	fscanf(file, "%f", &p1y);  
	fscanf(file, "%f", &p1z);
	fscanf(file, "%f", &p2x);
	fscanf(file, "%f", &p2y);
	fscanf(file, "%f", &p2z);				  

}

bool read_objs(std::string path, 
			   std::vector<GLfloat*>& vert_vec, 
			   std::vector<GLfloat*>& clr_vec, 
			   std::vector<int>& vsize_vec, 
			   std::vector<int>& csize_vec, 
			   std::vector<int>& n_tri,
			   std::vector<pii>& idx)
{
	const char* in_path = path.c_str();
	printf("Loading object file %s...\n", in_path);
	FILE * file = fopen(in_path, "r");
	if( file == NULL ){
		printf("Impossible to open the file\n");		
		return false;
	}


	enum set_states {VERT, CLR, RAY};

	set_states state = RAY;

	std::vector<GLfloat> in_v;
	std::vector<GLfloat> in_c;

	// Keep track of vert_vec index
	int v_ind = 0;

	// printf("Start!!\n");
	while(1){
		char lineHeader[128];
		GLfloat x, y, z;
		int I, J;
		GLfloat p1x, p1y, p1z, p2x, p2y, p2z;
		
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.		

		if(state == RAY) {
			if( strcmp(lineHeader, "v") == 0 ) {
				state = VERT;
				read_xyz(file, x, y, z);
				in_v.push_back(x);
				in_v.push_back(y);
				in_v.push_back(z);
				continue;
			}
			else if( strcmp(lineHeader, "c") == 0 ) {
				printf("Ray can't be immediately followed by color!\n");
				exit(1);
			}

			read_ray(file, I, J, p1x, p1y, p1z, p2x, p2y, p2z);
			idx.push_back(pii(I,J));
			vert_vec.push_back(new GLfloat[6]);
			clr_vec.push_back(new GLfloat[6]);
			vert_vec[v_ind][0] = p1x; vert_vec[v_ind][1] = p1y; vert_vec[v_ind][2] = p1z;
			vert_vec[v_ind][3] = p2x; vert_vec[v_ind][4] = p2y; vert_vec[v_ind][5] = p2z;
			clr_vec[v_ind][0] = 1; clr_vec[v_ind][1] = 0; clr_vec[v_ind][2] = 0;
			clr_vec[v_ind][3] = 1; clr_vec[v_ind][4] = 0; clr_vec[v_ind][5] = 0;

			n_tri.push_back(0);
			vsize_vec.push_back(sizeof(vert_vec[v_ind][0]) * 6);
			csize_vec.push_back(sizeof(clr_vec[v_ind][0]) * 6);
			v_ind++;

		}

		else if(state == VERT) {
			if( strcmp(lineHeader, "c") == 0 ) {
				state = CLR;
				read_xyz(file, x, y, z);
				// printf("c %.3f %.3f %.3f\n", x, y, z);
				in_c.push_back(x);
				in_c.push_back(y);
				in_c.push_back(z);				
				continue;
			}

			else if( strcmp(lineHeader, "ray") == 0 ) {
				printf("Vertex immediately followed by ray - not allowed!\n");
				exit(1);
			}

			read_xyz(file, x, y, z);

			// printf("v %.1f %.1f %.1f\n", x, y, z);
			in_v.push_back(x);
			in_v.push_back(y);
			in_v.push_back(z);

		}

		else if(state == CLR){
			if( strcmp(lineHeader, "v") == 0 ) {
				state = VERT;				
				// Create new dynamic array
				if(in_v.size() != in_c.size()){
					printf("Vertex and object size do not match!\n");
					exit(1);
				}
				int nv = in_v.size();
				if(nv % 9 != 0) {
					printf("Triangles not specified properly\n");
					exit(1);
				}

				idx.push_back(pii(-10,-10));
				vert_vec.push_back(new GLfloat[nv]);
				clr_vec.push_back(new GLfloat[nv]);
				for (int i = 0; i < nv; ++i) {
					vert_vec[v_ind][i] = in_v[i];
					clr_vec[v_ind][i] = in_c[i];
				}

				// Set sizes
				n_tri.push_back(nv/9);
				vsize_vec.push_back(sizeof(vert_vec[v_ind][0]) * nv);
				csize_vec.push_back(sizeof(clr_vec[v_ind][0]) * nv);				

				in_v.clear();
				in_c.clear();
				v_ind++;

				read_xyz(file, x, y, z);
				// printf("v %.1f %.1f %.1f\n", x, y, z);
				in_v.push_back(x);
				in_v.push_back(y);
				in_v.push_back(z);
				continue;
			}
			else if( strcmp(lineHeader, "ray") == 0 ) {
				printf("All rays mut come in BEGINNING of the file!\n");
				exit(1);
			}

			read_xyz(file, x, y, z);

			// printf("c %.3f %.3f %.3f\n", x, y, z);			
			in_c.push_back(x);
			in_c.push_back(y);
			in_c.push_back(z);			
		}

	}
	fclose(file);	
}