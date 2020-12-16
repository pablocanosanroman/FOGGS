#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

//This allow us to put multiple munchies, cpaital letters cause is a macro.
#define MUNCHIECOUNT 100

//This allow us to put multiples ghosts
#define GHOSTCOUNT 4

//This put another 4 ghosts (different color)
#define GHOSTCOUNT1 4

// Just need to include main header file
#include "S2D/S2D.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

//Data to represent Pacman
struct Player
{
	bool dead;
	Vector2* _Position;
	Rect* _SourceRect;
	Texture2D* _Texture;
	int _Direction;
	int _Frame;
	int _CurrentFrameTime;
	float speedMultiplier;
	int lives;
};

struct Enemy
{
	//Data of Munchie
	int _munchieframeCount;
	Vector2* _munchiePosition;
	Rect* _munchieSourceRect;
	Texture2D* _munchieTexture;
	int _munchieCurrentFrameTime;
	
	
	
	
	//Data of Cherry
	int _cherryframeCount;
	Vector2* _cherryPosition;
	Rect* _cherrySourceRect;
	Texture2D* _cherryTexture;
	int _cherryCurrentFrameTime;

	//Bool for both Munchie and Cherry
	bool _IsCollected;
	
};

struct MovingEnemy
{
	Vector2* position;
	Texture2D* texture;
	Rect* sourceRect;
	int direction;
	float speed;
	int frame;
	int CurrentFrameTimeGhost;

};


// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Pacman : public Game
{
private:
	
	bool _pKeyDownStart; //Key down for start menu

	bool _pKeyDownPause; //Key down for pause menu

	bool _pKeyDownRestart; //Key down for restart game

	bool _pKeyDownNextLevel; //Key down for next level

	
	//constant data for Game Variables
	const float _cPacmanSpeed;
	const int _cPacmanFrameTime;
	const int _cMunchieFrameTime;
	const int _cCherryFrameTime;
	const int _cGhostFrameTime;
	const int _cYellowGhostFrameTime;
	
	int munchieCollectedCount;
	

	//Data for Menu
	Texture2D* _menuBackground;
	Rect* _menuRectangle;
	Vector2* _menuStringPosition;
	bool _paused;

	//Data for Game Over Menu
	Texture2D* _overmenuBackground;
	Rect* _overmenuRectangle;
	Vector2* _overmenuStringPosition;
	bool _overmenu;

	//Data for the winning Menu
	Texture2D* _winmenuBackground;
	Rect* _winmenuRectangle;
	Vector2* _winmenuStringPosition;
	bool _restartWinMenu;

	//Data for start Menu
	Texture2D* _startmenuBackground;
	Rect* _startmenuRectangle;
	Vector2* _startmenuStringPosition;
	bool _startmenu;

	//Data to represent Pacman
	Player* _pacman;

	// Data to represent Munchie
	Enemy* _munchies[MUNCHIECOUNT];

	//Data to represent Ghost
	MovingEnemy* _redghosts[GHOSTCOUNT];

	//Data to represent yellow ghosts
	MovingEnemy* _yellowghosts[GHOSTCOUNT1];

	//Data to represent Cherry
	Enemy* _cherry;

	//sounds
	SoundEffect* _pop;
	SoundEffect* _waka;
	SoundEffect* _GameTheme;
	SoundEffect* _cherrypop;

	// Position for String
	Vector2* _stringPosition;

	//Input methods
	void Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState);

	//Check methods
	void CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey);
	void CheckRestart(Input::KeyboardState* state, Input::Keys restartKey);
	void CheckNextLevel(Input::KeyboardState* state, Input::Keys nextlevelKey);
	void CheckViewportCollision();
	void CheckGhostCollisions();
	void CheckYellowGhostCollisions();

	//Update methods
	void UpdatePacman(int elapsedTime);
	void UpdateMunchies(Enemy*, int elapsedTime);
	void UpdateGhost(MovingEnemy*, int elapsedTime);
	void UpdateGhost1(MovingEnemy*, int elapsedTime);
	void UpdateGhost2(MovingEnemy*, int elapsedTime);
	void UpdateYellowGhost(MovingEnemy*, int elapsedTime);
	void UpdateYellowGhost1(MovingEnemy*, int elapsedTime);
	void UpdateYellowGhost2(MovingEnemy*, int elapsedTime);
	void UpdateCherry(int elapsedTime);

	//Audio
	void GameThemeSound();

	
	


	

public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);

	/// <summary> Check the collision 
	bool virtual MunchieCollisionCheck(int pacmanX, int pacmanY, int pacmanwidth, int pacmanheight, int munchieX, int munchieY, int munchiewidth, int munchieheight);
	bool virtual CherryCollisionCheck(int pacmanX, int pacmanY, int pacmanwidth, int pacmanheight, int cherryX, int cherryY, int cherrywidth, int cherryheight);
};