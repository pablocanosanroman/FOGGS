#include "Pacman.h"

#include <sstream>
#include <time.h>
#include <iostream>


Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.2f), _cPacmanFrameTime(250), _cMunchieFrameTime(500), _cCherryFrameTime(500), _cGhostFrameTime(250)
{
	//local variable
	srand(time(NULL));

	int i;

	for (i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i] = new Enemy();
		_munchies[i]->_munchieframeCount = rand() % 1;
		_munchies[i]->_munchieCurrentFrameTime = 0;
		_munchies[i]->_IsCollected = false;
		
	}

	//initialise ghost character
	for (int k = 0; k < GHOSTCOUNT-2; k++)
	{
		_ghosts[k] = new MovingEnemy();
		_ghosts[k]->direction = 0;
		_ghosts[k]->speed = 0.2f;
		_ghosts[k]->frame = 0;
		_ghosts[k]->CurrentFrameTimeGhost = 0;
		

	}

	_ghosts[3] = new MovingEnemy();
	_ghosts[3]->direction = 2;
	_ghosts[3]->speed = 0.2f;
	_ghosts[3]->frame = 0;
	_ghosts[3]->CurrentFrameTimeGhost = 0;
	
	
	_ghosts[2] = new MovingEnemy();
	_ghosts[2]->direction = 2;
	_ghosts[2]->speed = 0.2f;
	_ghosts[2]->frame = 0;
	_ghosts[2]->CurrentFrameTimeGhost = 0;
	

	//Starting Cherry
	_cherry = new Enemy();
	_cherry->_cherryCurrentFrameTime = 0;
	_cherry->_cherryframeCount = 0;
	_cherry->_IsCollected = false;

	//Starting Parameters of the game
	//Pacman
	_pacman = new Player();
	_pacman->dead = false;
	_pacman->_Direction = 0;
	_pacman->_CurrentFrameTime = 0;
	_pacman->_Frame = 0;
	_pacman->speedMultiplier = 1.0f;
	_pacman->lives = 3;

	//Start Menu
	_startmenu = true;

	//Game Over Menu
	_overmenu = false;

	//Win Menu
	_winmenu = false;
	
	//Pause Menu
	_paused = false;

	//Key to start
	_pKeyDownStart = false;

	//Starting pop sound
	_pop = new SoundEffect();
	_waka = new SoundEffect();
	_GameTheme = new SoundEffect();
	_cherrypop = new SoundEffect();


	//Initialise important Game aspects
	Audio::Initialise();
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	//destructor Pacman
	delete _pacman->_Texture;
	delete _pacman->_SourceRect;
	delete _pacman->_Position;
	delete _pacman;

	//destructor Munchie
	delete _munchies[0]->_munchieTexture;
	int nCount = 0;
	for (nCount = 0; nCount < MUNCHIECOUNT; nCount++)
	{
		delete _munchies[nCount]->_munchieTexture;
		delete _munchies[nCount]->_munchiePosition;
		delete _munchies[nCount]->_munchieSourceRect;
	}
	//array deleted
	delete[] _munchies;

	//destructor Cherry
	delete _cherry->_cherryTexture;
	delete _cherry->_cherrySourceRect;
	delete _cherry->_cherryPosition;
	delete _cherry;

	//destructor ghosts
	delete _ghosts[0]->texture;
	int nCount1 = 0;
	for (nCount1 = 0; nCount1 < GHOSTCOUNT; nCount1++)
	{
		delete _ghosts[nCount1]->texture;
		delete _ghosts[nCount1]->position;
		delete _ghosts[nCount1]->sourceRect;
	}
	delete[] _ghosts;

	//delete sound
	delete _pop;
	delete _waka;
	delete _GameTheme;
	delete _cherrypop;
}

void Pacman::LoadContent()
{
	// Load Pacman
	_pacman->_Texture = new Texture2D();
	_pacman->_Texture->Load("Textures/Pacman.tga", false);
	_pacman->_Position = new Vector2(350.0f, 350.0f);
	_pacman->_SourceRect = new Rect(0.0f, 0.0f, 32, 32);

	// Load Munchies
	int i;
	
	Texture2D* munchieTex = new Texture2D();
	munchieTex->Load("Textures/Munchie-SpriteSheet.png", false);

	for (i = 0; i < MUNCHIECOUNT; i++)
	{
		
		_munchies[i]->_munchieTexture = munchieTex;
		_munchies[i]->_munchiePosition = new Vector2(100.0f, 450.0f);
		_munchies[i]->_munchieSourceRect = new Rect(0.0f, 0.0f, 12, 12);
		_munchies[i]->_munchiePosition = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		
	}

	//Load cherry
	_cherry->_cherryTexture = new Texture2D();
	_cherry->_cherryTexture->Load("Textures/Cherry-SpriteSheet.png", false);
	_cherry->_cherryPosition = new Vector2(100.0f, 450.0f);
	_cherry->_cherrySourceRect = new Rect(0.0f, 0.0f, 32, 32);
	_cherry->_cherryPosition = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));


	//Load Ghost character
	for (int k = 0; k < GHOSTCOUNT; k++)
	{
		_ghosts[k]->texture = new Texture2D();
		_ghosts[k]->texture->Load("Textures/RedGhost.png", false);
		_ghosts[k]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		_ghosts[k]->sourceRect = new Rect(0.0f, 0.0f, 36, 36);

	}
	

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);

	// Set Menu Paramters
	_menuBackground = new Texture2D();
	_menuBackground->Load("Textures/Transparency.png", false);
	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.2f, Graphics::GetViewportHeight() / 2.2f);

	//Set Start Menu Parameters
	_startmenuBackground = new Texture2D();
	_startmenuBackground->Load("Textures/Transparency.png", true);
	_startmenuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_startmenuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.6f, Graphics::GetViewportHeight() / 2.2f);

	//Set Game Over Menu Parameters
	_overmenuBackground = new Texture2D();
	_overmenuBackground->Load("Textures/Transparency.png", false);
	_overmenuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_overmenuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.2f, Graphics::GetViewportHeight() / 2.2f);

	//Set Win Menu Parameters
	_winmenuBackground = new Texture2D();
	_winmenuBackground->Load("Textures/Transparency.png", false);
	_winmenuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_winmenuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.2f, Graphics::GetViewportHeight() / 2.2f);
	

	//Set pop sound
	_GameTheme->Load("Sounds/GameTheme.wav");
	_GameTheme->SetLooping(true);
	_pop->Load("Sounds/pop.wav");
	_waka->Load("Sounds/Waka.wav");
	_waka->SetLooping(true);
	_cherrypop->Load("Sounds/cherrypop.wav");
	
	
}

//Input method

//Code to move Pacman
void Pacman::Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState)
{
	float pacmanSpeed = _cPacmanSpeed * elapsedTime * _pacman->speedMultiplier;

	if (state->IsKeyDown(Input::Keys::RIGHT))
	{
		_pacman->_Position->X += pacmanSpeed; //Moves Pacman across X axis
		_pacman->_Direction = 0;
		
	
	}
	else if (state->IsKeyDown(Input::Keys::LEFT))
	{
		_pacman->_Position->X += -pacmanSpeed;//Moves Pacman back the X axis
		_pacman->_Direction = 2;
		
	
	}
	else if (state->IsKeyDown(Input::Keys::UP))
	{
		_pacman->_Position->Y += -pacmanSpeed; //Moves Pacman up Y
		_pacman->_Direction = 3;
	
		
	}
	else if (state->IsKeyDown(Input::Keys::DOWN))
	{
		
		_pacman->_Position->Y += pacmanSpeed; //Moves Pacman down Y
		_pacman->_Direction = 1;
	
		
	}

	// Speed Multiplier
	if (state->IsKeyDown(Input::Keys::LEFTSHIFT))
	{
		//Apply multiplier
		_pacman->speedMultiplier = 2.0f;

		for (int k = 0; k < GHOSTCOUNT; k++)
		{
			_ghosts[k]->speed = 0.4f;
		}
	}
	else
	{
		//Reset multiplier
		_pacman->speedMultiplier = 1.0f;

		for (int k = 0; k < GHOSTCOUNT; k++)
		{
			_ghosts[k]->speed = 0.2f;
		}
	}
	//Code to move the cherry with the mouse
	mouseState->LeftButton;

	if (mouseState->LeftButton == Input::ButtonState::PRESSED)
	{
		_cherry->_cherryPosition->X = mouseState->X;
		_cherry->_cherryPosition->Y = mouseState->Y;
	}
	else if (state->IsKeyDown(Input::Keys::R))
	{
		_cherry->_cherryPosition = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		if (_cherry->_IsCollected)
		{
			_cherry->_cherryPosition->X = -50;
		}
	}
	
}

//Check methods

void Pacman::CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey)
{
	if (state->IsKeyDown(Input::Keys::P) && !_pKeyDownStart &&!_pKeyDownRestart)
	{
		_pKeyDownStart = true;
		_pKeyDownRestart = true;
		_paused = !_paused;
		
		

	}
	if (state->IsKeyUp(Input::Keys::P))
	{
		_pKeyDownStart = false;
		_pKeyDownRestart = false;
		
	}
		
}

void Pacman::CheckRestart(Input::KeyboardState* state, Input::Keys restartKey)
{
	
	
		if (state->IsKeyDown(Input::Keys::Y) && !_pKeyDownPause && !_pKeyDownStart && _overmenu)
		{
			_pKeyDownStart = true;
			_pKeyDownPause = true;
			_overmenu = false;
			

			_pacman->dead = false;
			_pacman->_Position = new Vector2(350.0f, 350.0f);
			_pacman->lives = 3;

			_cherry->_cherryPosition = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));

			for (int i = 0; i < MUNCHIECOUNT; i++)
			{
				_munchies[i]->_munchiePosition = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
			}

			for (int k = 0; k < GHOSTCOUNT; k++)
			{
				_ghosts[k]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
			}


		}
		if (state->IsKeyUp(Input::Keys::Y))
		{
			_pKeyDownStart = false;
			_pKeyDownPause = false;
		}
	

}
//Code to make Pacman go to the other side of the map when he hits the wall
void Pacman::CheckViewportCollision()
{
	//Make Pacman go to the opposite wall if he hits a wall
	int windWidth = Graphics::GetViewportWidth() + 30;
	int windHeight = Graphics::GetViewportHeight() + 30;
	int PacmanSize = 32;

	if (_pacman->_Position->X + _pacman->_SourceRect->Width > windWidth) //1024 is game width
	{
		//Pacman hit right wall - reset his position
		_pacman->_Position->X = PacmanSize - _pacman->_SourceRect->Width;
	}
	if (_pacman->_Position->X + _pacman->_SourceRect->Width < PacmanSize)
	{
		//Pacman hit left wall - reset his position
		_pacman->_Position->X = windWidth - _pacman->_SourceRect->Width;
	}
	if (_pacman->_Position->Y + _pacman->_SourceRect->Width < PacmanSize)
	{
		//Pacman hit top wall - reset his position
		_pacman->_Position->Y = windHeight - _pacman->_SourceRect->Height;
	}
	if (_pacman->_Position->Y + _pacman->_SourceRect->Height > windHeight)
	{
		//Pacman hit down wall - reset his position
		_pacman->_Position->Y = PacmanSize - _pacman->_SourceRect->Height;
	}
}



//Update methods

//Make the animation of Pacman

void Pacman::UpdatePacman(int elapsedTime)
{
	_pacman->_SourceRect->X = _pacman->_SourceRect->Width * _pacman->_Frame;

	_pacman->_CurrentFrameTime += elapsedTime;

	if (_pacman->_CurrentFrameTime > _cPacmanFrameTime)
	{
		_pacman->_Frame++;

		if (_pacman->_Frame >= 2)
		{
			_pacman->_Frame = 0;
		}
		if (_paused)
		{
			_pacman->_Frame = 0;
		}

		_pacman->_CurrentFrameTime = 0;
	}
}

//Animation for Munchie
void Pacman::UpdateMunchies(Enemy* pMunchie, int elapsedTime)
{
	
		pMunchie->_munchieCurrentFrameTime += elapsedTime;

		if (pMunchie->_munchieCurrentFrameTime > _cMunchieFrameTime)
		{
			pMunchie->_munchieframeCount++;

			if (pMunchie->_munchieframeCount >= 2)
			{
				pMunchie->_munchieframeCount = 0;

			}

			pMunchie->_munchieCurrentFrameTime = 0;
		}

		pMunchie->_munchieSourceRect->X = pMunchie->_munchieSourceRect->Width * pMunchie->_munchieframeCount;
	
}

//Animation of Cherry
void Pacman::UpdateCherry(int elapsedTime)
{
	_cherry->_cherryCurrentFrameTime += elapsedTime;

	if (_cherry->_cherryCurrentFrameTime > _cCherryFrameTime)
	{
		_cherry->_cherryframeCount++;

		if (_cherry->_cherryframeCount >= 2)
		{
			_cherry->_cherryframeCount = 0;
		}

		_cherry->_cherryCurrentFrameTime = 0;
	}

	_cherry->_cherrySourceRect->X = _cherry->_cherrySourceRect->Width * _cherry->_cherryframeCount;
}

//Animation for the ghosts

void Pacman::UpdateGhost1(MovingEnemy* ghost, int elapsedTime)
{
	ghost->sourceRect->X = ghost->sourceRect->Width * ghost->frame;

	ghost->CurrentFrameTimeGhost += elapsedTime;

	if (ghost->CurrentFrameTimeGhost > _cGhostFrameTime)
	{
		ghost->frame++;

		if (ghost->frame >= 2)
		{
			ghost->frame = 0;
		}
			
		if (_paused)
		{
			ghost->frame = 0;
		}

		ghost->CurrentFrameTimeGhost = 0;
	}
	

	if (ghost->direction == 2)
	{
		ghost->position->Y -= ghost->speed * elapsedTime;
		
	}
	else if (ghost->direction == 3)
	{
		ghost->position->Y += ghost->speed * elapsedTime;
		
	}
	else if (ghost->direction == 0)
	{
		ghost->position->X += ghost->speed * elapsedTime;
	
	}
	else if (ghost->direction == 1)
	{
		ghost->position->X -= ghost->speed * elapsedTime;
		
	}
	
	if (ghost->position->Y + ghost->sourceRect->Height >= Graphics::GetViewportHeight())
	{
		//are we bottom left, go right
		ghost->position->Y = Graphics::GetViewportHeight() - ghost->sourceRect->Height - 1;
		ghost->direction = 0;
	}
	if (ghost->position->X <= 0)
	{
		//are we top left, go down
		ghost->position->X = 1;
		ghost->direction = 3;
	}
	if (ghost->position->Y <= 0)
	{
		//are we top right, go left
		ghost->position->Y = 1;
		ghost->direction = 1;
	}
	if (ghost->position->X + ghost->sourceRect->Width >= Graphics::GetViewportWidth())
	{
		//are we bottom right, go up
		ghost->position->X = Graphics::GetViewportWidth() - ghost->sourceRect->Width - 1;
		ghost->direction = 2;
	}


}

void Pacman::UpdateGhost2(MovingEnemy* ghost, int elapsedTime)
{
	ghost->sourceRect->X = ghost->sourceRect->Width * ghost->frame;

	ghost->CurrentFrameTimeGhost += elapsedTime;

	if (ghost->CurrentFrameTimeGhost > _cGhostFrameTime)
	{
		ghost->frame++;

		if (ghost->frame >= 2)
		{
			ghost->frame = 0;
		}

		if (_paused)
		{
			ghost->frame = 0;
		}

		ghost->CurrentFrameTimeGhost = 0;
	}

	if (ghost->direction == 2)
	{
		ghost->position->Y -= ghost->speed * elapsedTime;
	}
	else if (ghost->direction == 3)
	{
		ghost->position->Y += ghost->speed * elapsedTime;
	}

	if (ghost->position->Y <= 0)
	{
		ghost->direction = 3;
	}
	else if (ghost->position->Y + ghost->sourceRect->Height >= Graphics::GetViewportHeight())
	{
		
		ghost->direction = 2;
	}

}

void Pacman::UpdateGhost(MovingEnemy* ghost, int elapsedTime)
{
	ghost->sourceRect->X = ghost->sourceRect->Width * ghost->frame;

	ghost->CurrentFrameTimeGhost += elapsedTime;

	if (ghost->CurrentFrameTimeGhost > _cGhostFrameTime)
	{
		ghost->frame++;

		if (ghost->frame >= 2)
		{
			ghost->frame = 0;
		}

		if (_paused)
		{
			ghost->frame = 0;
		}

		ghost->CurrentFrameTimeGhost = 0;
	}

	if (ghost->direction == 0) //Moves Right
	{
		ghost->position->X += ghost->speed * elapsedTime;
	}
	else if (ghost->direction == 1) //Moves Left
	{
		ghost->position->X -= ghost->speed * elapsedTime;

	}

	if (ghost->position->X + ghost->sourceRect->Width >= Graphics::GetViewportWidth()) //Hits Right edge
	{
		ghost->direction = 1;
	}
	else if (ghost->position->X <= 0)
	{
		ghost->direction = 0;
	}
	
}


void Pacman::Update(int elapsedTime)
{
	//Make the animation of Pacman

	//Animation of Munchie
	//This change the sprite of the munchie
	/*_munchieSourceRect->Y = _munchieSourceRect->Height * _MunchieframeCount;*/
		
	// This change the sprite of the pacman
	_pacman->_SourceRect->Y = _pacman->_SourceRect->Height * _pacman->_Direction;

	for (int k = 0; k < GHOSTCOUNT; k++)
	{
		_ghosts[k]->sourceRect->Y = _ghosts[k]->sourceRect->Height * _ghosts[k]->direction;
	}

	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	//Gets the current state of the mouse
	Input::MouseState* mouseState = Input::Mouse::GetState();

	//State when the game is not paused

	if (_startmenu)
	{
		if (keyboardState->IsKeyDown(Input::Keys::SPACE))
		{
			_startmenu = false;
			GameThemeSound();
			Audio::Play(_waka);
			_waka->SetLooping(true);
			_waka->SetGain(0.1f);
			

			if (!Audio::Play(_GameTheme))
			{
				cout << "Not working" << endl;
			}
		}
	}
	else
	{
		
		CheckPaused(keyboardState, Input::Keys::P);


		if (!_paused && !_overmenu && !_winmenu)
		{
			

			//Code to move Pacman
			Input(elapsedTime, keyboardState, mouseState);

			

			//Code to make Pacman go to the other side of the map when he hits the wall
			CheckViewportCollision();

			//Check collision with munchie
			for (int i = 0; i < MUNCHIECOUNT; i++)
			{
				MunchieCollisionCheck(_pacman->_Position->X, _pacman->_Position->Y, _pacman->_SourceRect->Width, _pacman->_SourceRect->Height, _munchies[i]->_munchiePosition->X, _munchies[i]->_munchiePosition->Y,
					_munchies[i]->_munchieSourceRect->Width, _munchies[i]->_munchieSourceRect->Height);
			}
			
			//Check collision with cherry
			CherryCollisionCheck(_pacman->_Position->X, _pacman->_Position->Y, _pacman->_SourceRect->Width, _pacman->_SourceRect->Height, _cherry->_cherryPosition->X, _cherry->_cherryPosition->Y,
				_cherry->_cherrySourceRect->Width, _cherry->_cherrySourceRect->Height);

			CheckGhostCollisions();

			//Make the animation of Pacman
			UpdatePacman(elapsedTime);

			//Animation for Munchie
			for (int i = 0; i < MUNCHIECOUNT; i++)
			{
				UpdateMunchies(_munchies[i], elapsedTime);
			}

			//Animation of Cherry
			UpdateCherry(elapsedTime);

			for (int k = 0; k < GHOSTCOUNT-1; k++)
			{
				UpdateGhost(_ghosts[k], elapsedTime);
			}

			UpdateGhost1(_ghosts[3], elapsedTime);
			UpdateGhost2(_ghosts[2], elapsedTime);
		}

		
		
			CheckRestart(keyboardState, Input::Keys::Y);
		
		
	}

	

}

//Function for the collision with munchies
bool Pacman::MunchieCollisionCheck(int pacmanX, int pacmanY, int pacmanwidth, int pacmanheight, int munchieX, int munchieY, int munchiewidth, int munchieheight)
{
	int pacmanleft = pacmanX;
	int munchieleft = munchieX;
	int pacmanright = pacmanX + pacmanwidth;
	int munchieright = munchieX + munchiewidth;
	int pacmantop = pacmanY;
	int munchietop = munchieY;
	int pacmanbottom = pacmanY + pacmanheight;
	int munchiebottom = munchieY + munchieheight;
	

	if (pacmanbottom < munchiebottom)
	{
		return false;
	}
	else if (pacmantop > munchiebottom)
	{
		return false;
	}
	else if (pacmanright < munchieleft)
	{
		return false;
	}
	else if (pacmanleft > munchieleft)
	{
		return false;
	}
	else
	{
		return true;
	}

}

//Function for the collision with cherry
bool Pacman::CherryCollisionCheck(int pacmanX, int pacmanY, int pacmanwidth, int pacmanheight, int cherryX, int cherryY, int cherrywidth, int cherryheight)
{
	int pacmanleft = pacmanX;
	int cherryleft = cherryX;
	int pacmanright = pacmanX + pacmanwidth;
	int cherryright = cherryX + cherrywidth;
	int pacmantop = pacmanY;
	int cherrytop = cherryY;
	int pacmanbottom = pacmanY + pacmanheight;
	int cherrybottom = cherryY + cherryheight;


	if (pacmanbottom < cherrybottom)
	{
		return false;
	}
	else if (pacmantop > cherrybottom)
	{
		return false;
	}
	else if (pacmanright < cherryleft)
	{
		return false;
	}
	else if (pacmanleft > cherryleft)
	{
		return false;
	}
	else
	{
		return true;
	}

}

void Pacman::CheckGhostCollisions()
{
	//Local Variables
	int i = 0;
	int pacmanbottom = _pacman->_Position->Y + _pacman->_SourceRect->Height;
	int ghostbottom = 0;
	int pacmanleft = _pacman->_Position->X;
	int ghostleft = 0;
	int pacmanright = _pacman->_Position->X + _pacman->_SourceRect->Width;
	int ghostright = 0;
	int pacmantop = _pacman->_Position->Y;
	int ghosttop = 0;

	for (i = 0; i < GHOSTCOUNT; i++)
	{
		//Populate variables with Ghost data
		ghostbottom = _ghosts[i]->position->Y + _ghosts[i]->sourceRect->Height;
		ghostleft = _ghosts[i]->position->X;
		ghostright = _ghosts[i]->position->X + _ghosts[i]->sourceRect->Width;
		ghosttop = _ghosts[i]->position->Y;

		if ((pacmanbottom > ghosttop) && (pacmantop < ghostbottom) && (pacmanright > ghostleft)
			&& (pacmanleft < ghostright))
		{
			_pacman->dead = true;
			_pacman->lives--;
			_pacman->_Position = new Vector2(350.0f, 350.0f);
			
			if (!_pacman->lives == 0)
			{
				_pacman->dead = false;
			}
			else
			{
				_pacman->dead = true;
			}
			i = GHOSTCOUNT;

		}
	}
}

//Audio for game theme
void Pacman::GameThemeSound()
{
	if (!_paused && !_overmenu && !_winmenu)
	{
		Audio::Play(_GameTheme);
		_GameTheme->SetLooping(true);
		_GameTheme->SetGain(0.3f);
		
	}
}

void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Pacman X: " << _pacman->_Position->X << " Y: " << _pacman->_Position->Y << " Lives:" << _pacman->lives;

	SpriteBatch::BeginDraw(); // Starts Drawing


	// Draw start Menu
	if (_startmenu)
	{
		std::stringstream startmenuStream;
		startmenuStream << "                  PACMAN\n\nPress Spacebar to start the game";

		SpriteBatch::Draw(_startmenuBackground, _startmenuRectangle, nullptr);
		SpriteBatch::DrawString(startmenuStream.str().c_str(), _startmenuStringPosition, Color::Red);

	}
	else
	{
		

		if (!_pacman->dead)
		{
			SpriteBatch::Draw(_pacman->_Texture, _pacman->_Position, _pacman->_SourceRect); // Draws Pacman

		}
		else
		{
			if (_pacman->lives == 0)
			{
				_overmenu = true;
			}
			
			
		}
		

		int i;
		for (i = 0; i < MUNCHIECOUNT; i++)
		{
			//Draw Munchie
			/*SpriteBatch::Draw(_munchieTexture, _munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);*/
			SpriteBatch::Draw(_munchies[i]->_munchieTexture, _munchies[i]->_munchiePosition, _munchies[i]->_munchieSourceRect);
			
			if (MunchieCollisionCheck(_pacman->_Position->X, _pacman->_Position->Y, _pacman->_SourceRect->Width, _pacman->_SourceRect->Height, _munchies[i]->_munchiePosition->X, _munchies[i]->_munchiePosition->Y,
				_munchies[i]->_munchieSourceRect->Width, _munchies[i]->_munchieSourceRect->Height))
			{
				_munchies[i]->_IsCollected = true;
				

				if (_munchies[i]->_IsCollected)
				{
					_munchies[i]->_munchiePosition->X = -50;
					Audio::Play(_pop);
					_pop->SetGain(0.2f);
					
				}
				
				
			}

			
			

		}

		SpriteBatch::Draw(_cherry->_cherryTexture, _cherry->_cherryPosition, _cherry->_cherrySourceRect);

		if (CherryCollisionCheck(_pacman->_Position->X, _pacman->_Position->Y, _pacman->_SourceRect->Width, _pacman->_SourceRect->Height, _cherry->_cherryPosition->X, _cherry->_cherryPosition->Y,
			_cherry->_cherrySourceRect->Width, _cherry->_cherrySourceRect->Height))
		{
			_cherry->_IsCollected = true;


			if (_cherry->_IsCollected)
			{
				_cherry->_cherryPosition->X = -50;
				Audio::Play(_cherrypop);
				_cherrypop->SetGain(1.0f);
				
			}
			
		}

		if (_munchies[MUNCHIECOUNT]->_IsCollected == true && _cherry->_IsCollected == true)
		{
			_winmenu == true;
		}
		
		
		
		for (int k = 0; k < GHOSTCOUNT; k++)
		{
			SpriteBatch::Draw(_ghosts[k]->texture, _ghosts[k]->position, _ghosts[k]->sourceRect);
		}

		//Draw the coordinates of Pacman

		SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
		// Draws Pause menu String
		
		if (_paused)
		{
			std::stringstream menuStream;
			menuStream << "PAUSED!";

			SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
			SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);

			
		}

		//Draws Game Over menu String
		if (_overmenu)
		{
			
			std::stringstream overmenuStream;
			overmenuStream << "      GAME OVER!\nPress Y to play again";

			SpriteBatch::Draw(_overmenuBackground, _overmenuRectangle, nullptr);
			SpriteBatch::DrawString(overmenuStream.str().c_str(), _overmenuStringPosition, Color::Red);

		
		}

		//Draws Win Menu String
		if (_winmenu)
		{
			std::stringstream winmenuStream;
			winmenuStream << "CONGRATULATIONS! YOU WON!\nPress Y to play again";

			SpriteBatch::Draw(_winmenuBackground, _winmenuRectangle, nullptr);
			SpriteBatch::DrawString(winmenuStream.str().c_str(), _winmenuStringPosition, Color::Green);
		}
	}

	
	SpriteBatch::EndDraw(); // Ends Drawing
	
}