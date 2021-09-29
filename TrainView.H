/************************************************************************
     File:        TrainView.H
     Author:     
                  Michael Gleicher, gleicher@cs.wisc.edu
     Modifier
                  Yu-Chi Lai, yu-chi@cs.wisc.edu
     
     Comment:     
						The TrainView is the window that actually shows the 
						train. Its a
						GL display canvas (Fl_Gl_Window).  It is held within 
						a TrainWindow
						that is the outer window with all the widgets. 
						The TrainView needs 
						to be aware of the window - since it might need to 
						check the widgets to see how to draw
	  Note:        we need to have pointers to this, but maybe not know 
						about it (beware circular references)
     Platform:    Visio Studio.Net 2003/2005
*************************************************************************/

#pragma once

#include "RenderUtilities/BufferObject.h"
#include "RenderUtilities/Shader.h"
#include "RenderUtilities/Texture.h"
#include "RenderUtilities/WaterFrameBuffer.H"

// Preclarify for preventing the compiler error
class TrainWindow;
class CTrack;


//#######################################################################
// TODO
// You might change the TrainView in order to add different objects to
// be drawn, or adjust the lighting, or ...
//#######################################################################


// The TrainView also needs to handle its user interface events.
// since this is an FLTk Window, we need to include FlTk Headers
// the pragmas turn off the warnings from FlTk - our code should have no warnings,
// but their code doesn't meet that standard
#pragma warning(push)
#pragma warning(disable:4312)
#pragma warning(disable:4311)
#include <Fl/Fl_Gl_Window.h>
#pragma warning(pop)

#include <AL/alut.h>
//#include <AL/al.h>
//#include <AL/alc.h>
#include <vector>
#include <string>

// this uses the old ArcBall Code
#include "Utilities/ArcBallCam.H"

struct Drop
{
	Drop(glm::vec2 p, float t, float r, float k)
		:point(p), time(t), radius(r), keepTime(k)
	{
	}

	glm::vec2 point;
	float time;
	float radius;
	float keepTime;
};

class TrainView : public Fl_Gl_Window
{
	public:
		// note that we keep the "standard widget" constructor arguments
		TrainView(int x, int y, int w, int h, const char* l = 0);

		// overrides of important window things
		virtual int handle(int);
		virtual void draw();

		// all of the actual drawing happens in this routine
		// it has to be encapsulated, since we draw differently if
		// we're drawing shadows (no colors, for example)
		void drawStuff(bool doingShadows=false);

		void draw(glm::vec4 plane, bool reflection);

		// setup the projection - assuming that the projection stack has been
		// cleared for you
		void setProjection();

		// Reset the Arc ball control
		void resetArcball();

		// pick a point (for when the mouse goes down)
		void doPick();

		//set ubo
		void setUBO();

		void initSkyboxShader();

		unsigned int loadCubemap(std::vector<std::string> faces);
	
		void initTilesShader();

		void initWaterShader();

		void initSineWaveShader();

		void initHeightMapShader();

		void initPlaneShader();

		void drawSkyBox(bool reflection);

		void drawTiles(glm::vec4 plane, bool reflection);

		void drawSineWave(bool reflection);

		void drawHeightMapWave();

		void addDrop(float radius, float keepTime);

		void drawPlane();
	public:
		ArcBallCam		arcball;			// keep an ArcBall for the UI
		int				selectedCube;  // simple - just remember which cube is selected

		TrainWindow*	tw;				// The parent of this display window
		CTrack*			m_pTrack;		// The track of the entire scene

		Shader* shader		= nullptr;	
		Texture2D* texture	= nullptr;
		VAO* plane			= nullptr;
		UBO* commom_matrices= nullptr;

		Shader* skyboxShader = nullptr;
		Texture2D* skyBoxTexture = nullptr;

		Shader* tilesShader = nullptr;
		VAO* tiles			= nullptr;
		Texture2D* tilesTexture = nullptr;

		Shader* waterShader = nullptr;
		VAO* water			= nullptr;
		Texture2D* waterTexture = nullptr;

		Shader* sineWaveShader = nullptr;
		VAO* sineWave		= nullptr;
		Texture2D* sineWaveTexture = nullptr;

		Shader* heightMapShader = nullptr;
		VAO* heightMap		= nullptr;
		std::vector<Texture2D> heightMapTexture;	

		WaterFrameBuffers* waterFrameBuffers = nullptr;

		Texture2D* dudvTexture = nullptr;
		Texture2D* normalMap = nullptr;
		Texture2D* depthMap = nullptr;

		Shader* planeShader = nullptr;
		VAO* n_plane = nullptr;
		Texture2D* planeTexture = nullptr;

		//OpenAL
		glm::vec3 source_pos;
		glm::vec3 listener_pos;
		ALCdevice* device	= nullptr;
		ALCcontext* context = nullptr;
		ALuint source;
		ALuint buffer;

		unsigned int skyboxVAO;
		unsigned int skyboxVBO;
		unsigned int cubemapTexture;

		float				t_time = 0.0f;
		unsigned int		DIVIDE_LINE = 500;
		unsigned int		heightMapIndex = 0;
		float				moveFactor = 0.0f;
		float				WAVE_SPEED = 0.03f;
		glm::vec3			cameraPosition;
		glm::vec3			lightColor;
		glm::vec3			lightPosition;

		std::vector<Drop> allDrop;
		Shader* interactiveFrameShader = nullptr;
		unsigned int interactiveFrameBuffer;
		unsigned int interactiveTextureBuffer;
		unsigned int interactiveRenderBuffer;
		unsigned int interactiveQuadVAO;
		unsigned int interactiveQuadVBO;

	public:
		GLfloat* inverse(GLfloat* m);
};