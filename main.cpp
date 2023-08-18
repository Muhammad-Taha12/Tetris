/* PROGRAMMING FUNDAMENTAL'S PROJECT FOR FALL 2022 BS(CS)
 * You don't need to change anything in the driver code/main function except the part(s) where instructed.
 * You are expected to define your functionalities in the "functionality.h" header and call them here in the instructed part(s).
 * The game is started with a small box, you need to create other possible in "pieces.h" file in form of array.
    E.g., Syntax for 2 pieces, each having four coordinates -> int Pieces[2][4];
 * Load the images into the textures objects from img directory.
 * Don't make any changes to the "utils.h" header file except for changing the coordinate of screen or global variables of the game.
 * SUGGESTION: If you want to make changes in "utils.h", do it once you complete rest of the functionalities.
 * TIP: You can study SFML for drawing scenes, sprites and window.
 * */

/* 
   Name: Muhammad Taha
   Student-ID: 22I-0870
   PF-Project
*/

//Header Files
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include "utils.h"
#include "pieces.h"
#include "functionality.h"
using namespace sf;

//Start of Main
int main()
{
	bool restart = false;  //Declaring the variable to restart the game
	do  //Start of do-while loop that restarts the game
	{
		srand(time(0));   //Randomizer
		RenderWindow window(VideoMode(320, 480), title);
    
		Texture obj1, obj2, obj3, obj4, obj5, obj6, obj7;
		/*
        		Load "frames.png","tiles.png" and "background.png" for Texture objects
    		*/
    
		obj1.loadFromFile("img/tiles.png");   //For the blocks
		obj2.loadFromFile("img/background1.png");   //The background
		obj3.loadFromFile("img/frame.png");   //The frame surrounding the field
		obj4.loadFromFile("img/transparent.png");  //For the block shadow
		obj5.loadFromFile("img/greyed.png");  //For the uninteractable rows that trigger after 5 minutes
		obj6.loadFromFile("img/Score.png");  //For the score
		obj7.loadFromFile("img/bombs.png");
    
		Sprite sprite(obj1), background(obj2), frame(obj3), shadow(obj4), grey(obj5), score(obj6), bombs(obj7);
		
		//Declaring Variables
		int delta_x=0, colorNum=1+rand()%7, first = 0, height, bombchance, Irotate = 0, level = 1, Rows = M - level + 1;
		int totalscore = 0, next = rand() % 4, next2, nextcolor = 1 + rand() % 7, nextcolor2 = 1 + rand() % 7, startlevel;
		int scorenumber[5] = {0}, nextpoint[4][2], nextpoint2[4][2];
		float timer=0, delay=0.5, totaltime = 0;
		bool rotate = false, press = false, bomb = false, bombstop = false, output = false, game = true, start = false, help = false, pause = false, gameover = false;
		string name;
		Clock clock;
		
		Music title;	//Declaring music variable
		title.openFromFile("img/title.wav");	//Setting Music to title music at the start
		title.play();
		title.setLoop(true);
				
		//Calling function to reset the gamestate back to the start when game is restarted
		restartGame(restart, Rows, totalscore, start, game, totaltime);
		
		//Start of Game
		while (window.isOpen() && game == true)
		{
			//Declaring the event
			Event e;
			
			//Checking if it is the start of the game
			while (!start)
			{

				obj2.loadFromFile("img/start.png");  //Using the title screen as the background
				window.draw(background);
				while (window.pollEvent(e))    //Event Listening
				{                  
		    			if (e.type == Event::Closed)   //Checking if the cross button is pressed
					{   
						window.close();
						restart = false;    //Closes the window and ends the program
						start = true;
					}                 
					if (e.type == Event::KeyPressed)   //Checking if a button is pressed
					{          
			    			if (e.key.code == Keyboard::A)
						{
							startlevel = 1;       //Starting from level 1 if A is pressed
							start = true;
						}
						if (e.key.code == Keyboard::B)   
						{				     
							startlevel = 2;       //Starting from level 2 if B is pressed
							start = true;
						}                   
						else if (e.key.code == Keyboard::H) 
						{
							help = true;          //Opening Help menu if H is pressed
						}
					}
				}
				
				while (help)   //Opening the Help menu
				{
					obj2.loadFromFile("img/help.png");   //Using the help menu as the background
					window.draw(background);
					window.display();
					while (window.pollEvent(e))     //Event Listening
					{                  
						if (e.type == Event::Closed)   //Checking if the cross button is pressed
						{   
							window.close();
							restart = false;    //Closes the window and ends the program
							help = false;
							start = true;
						}                     
				    		if (e.type == Event::KeyPressed)   //Checking if a button is pressed
				    		{
							if (e.key.code == Keyboard::Enter)
							{
								help = false;     //Gets rid of the help menu if Enter is pressed
							}
						}
				 	}                	
				}
				window.display();   //Displaying
				if (start == true)
				{
					title.openFromFile("img/tetris.wav");	//Changing music to the main theme when game starts
					title.play();
					title.setLoop(true);
				}
			}
			
			if (startlevel == 1)
			{
				obj2.loadFromFile("img/background1.png");   //Using the game background as the background
			}
			else
			{
				obj2.loadFromFile("img/background2.png");
			}
			
			//Checking if Down button is pressed, if the level is 1 and the piece is not a bomb
			if (press == false && level == 1 && !bomb)
			{
				delay = 0.5;    //Default delay which is slow
			}
			//If level is more than 1 or the piece is a bomb
			else if (press == false && level != 1 || bomb)
			{
				delay = 0.15;    //Delay is less, so blocks fall faster
			}
			
			float time = clock.getElapsedTime().asSeconds();
			clock.restart();
			timer+=time;
			totaltime += timer;
			
        		//---Event Listening Part---//
        		while (window.pollEvent(e))		//Event is occurring - until the game is in running state
        		{                    
				if (e.type == Event::Closed)		//If cross/close is clicked/pressed
				{   
					window.close();			//Opened window disposes
					restart = false;
				}                                   
				if (e.type == Event::KeyPressed && !bomb) 	//If any other key (not cross) is pressed and piece is not a bomb
				{             
					if (e.key.code == Keyboard::Space)   //If spacebar is pressed
					{
						delay = 0;        		//Piece moves downward immediately
 						press = true;
					}
					if (e.key.code == Keyboard::Up)            //Check if the other key pressed is UP key
					{
						rotate = true;                         //Rotation gets on
					}
					else if (e.key.code == Keyboard::Left)     //Check if the other key pressed is LEFT key
					{
						delta_x = -1;                          //Change in X-Axis - Negative
					}
					else if (e.key.code == Keyboard::Right)    //Check if the other key pressed is RIGHT key
					{
						delta_x = 1;                 	   //Change in X-Axis - Positive
					}
				}
			}
        		if (Keyboard::isKeyPressed(Keyboard::Down) && !bomb)   //Just another way to detect key presses without event listener
				delay=0.05;                                    //If DOWN key is pressed, delay in falling piece decreases from 0.5 or 0.15 to 0.05

			if (Keyboard::isKeyPressed(Keyboard::H))   //Checking if H key is pressed
			{
				pause = true;    //Pauses the game
			}
			
			//While the game is paused
			while (pause)
			{
				obj2.loadFromFile("img/pause.png");   //Using the pause screen as the background
				window.draw(background);
				window.display();
				while (window.pollEvent(e))    //Event listener
				{                  
					if (e.type == Event::Closed)		//If cross/close is clicked/pressed
					{   
						window.close();
						restart = false;             //Closes the window and the program
						pause = false;
					}                 
					if (e.type == Event::KeyPressed) 
					{
						if (e.key.code == Keyboard::Enter)   //Checking if Enter is pressed
						{
							pause = false;    //Unpauses the game
						}
						else if (e.key.code == Keyboard::H)   //Checking if H key is pressed
						{
							help = true;   //Opens the help menu
						}
						else if (e.key.code == Keyboard::N)    //Checking if N key is pressed
						{
							restart = true;
							pause = false;    //Starts a new game
							game = false;
						}
					}
				}
				while (help)   //Opening the Help menu
				{
					obj2.loadFromFile("img/help.png");   //Using the help menu as the background
					window.draw(background);
					window.display();
					while (window.pollEvent(e))     //Event Listening
					{                  
						if (e.type == Event::Closed)   //Checking if the cross button is pressed
						{   
							window.close();
							restart = false;    //Closes the window and ends the program
							help = false;
							pause = false;
						}                     
				    		if (e.type == Event::KeyPressed)   //Checking if a button is pressed
				    		{
							if (e.key.code == Keyboard::Enter)
							{
								help = false;     //Gets rid of the help menu if Enter is pressed
							}
						}
				 	}                	
				} 
			}
	
			///////////////////////////////////////////////
			///*** START CALLING YOUR FUNCTIONS HERE ***///
			onePiece(first, next, next2, level);    //The function that makes the first piece so it isn't just a single block
			fallingPiece(timer, delay, colorNum, press, bomb, bombstop, Irotate, level, next, nextcolor, next2, nextcolor2);  //Causes pieces to fall and also generates new pieces
			movePiece(delta_x);   //Moves the piece left and right
			removeLines(level, totalscore);    //Removes lines when they are filled
			rotateBlocks(rotate, Irotate, level);     //Rotates blocks when UP is pressed
			int height = blockShadow(bomb, Irotate, Rows);    //Calculates height for block shadow
			bombDetonation(colorNum, bomb, bombstop, Rows);    //Causes bomb to destroy 4 blocks if color doesn't match and all blocks when color matches
			gameOver(gameover);   //Checks if the game is over
			levelUp(level, totaltime, Rows, startlevel);    //Increases difficulty every 5 minutes
			scoreDisplay(totalscore, scorenumber);    //Displays the score
			nextPiece(next, next2, nextpoint, nextpoint2);    //Shows the upcoming piece
			//blockShadow(colorNum);
			///*** YOUR CALLING STATEMENTS END HERE ***///
			//////////////////////////////////////////////

			window.clear(Color::Black);
			window.draw(background);    //Drawing background
			
			//Displays the blocks on the grid
			for (int row = 0; row < Rows; row++)
			{
				for (int col = 0; col < N; col++)
				{
					if (gameGrid[row][col] == 0)
					{
					    	continue;
					}
					sprite.setTextureRect(IntRect(gameGrid[row][col]*18, 0, 18, 18));  //Sets texture from the image
					sprite.setPosition(col*18, row*18);   //Sets position
					sprite.move(28, 31); //offset
					window.draw(sprite);
				}
			}
			if (!bomb)
			{
				//Displays the falling blocks
				for (int piece = 0; piece < 4; piece++)
				{
					sprite.setTextureRect(IntRect(colorNum*18, 0, 18, 18));   //Sets texture from the image
					sprite.setPosition(point_1[piece][0]*18, point_1[piece][1]*18);   //Sets position
					sprite.move(28, 31);
					window.draw(sprite);
				}
        		}
			else
			{
				//Displays the falling bombs
				for (int piece = 0; piece < 4; piece++)
				{
					bombs.setTextureRect(IntRect(colorNum*18, 0, 18, 18));   //Sets texture from the image
					bombs.setPosition(point_1[piece][0]*18, point_1[piece][1]*18);   //Sets position
					bombs.move(28, 31);
					window.draw(bombs);
				}
			}
        		
        		if (startlevel != 1)
        		{
				//Displays the next block
				for (int nextblock = 0; nextblock < 4; nextblock++)
				{
					sprite.setTextureRect(IntRect(nextcolor*18, 0, 18, 18));    //Sets the texture from the same image as the blocks
					sprite.setPosition(nextpoint[nextblock][0]*18, nextpoint[nextblock][1]*18);    //Sets position
					
					//Offsetting according to which piece is next so all of them properly fit inside the next block display
					if (next == 0)
					{
					    	sprite.move(180,275);
					}
					else if (next == 1)
					{
					    	sprite.move(189,275);
					}
					else if (next >= 2 && next <= 6)
					{
					    	sprite.move(179,282);
					}
					window.draw(sprite);
				}
			}
			else
			{
				//Displays the next block
				for (int nextblock = 0; nextblock < 4; nextblock++)
				{
					sprite.setTextureRect(IntRect(nextcolor*18, 0, 18, 18));    //Sets the texture from the same image as the blocks
					sprite.setPosition(nextpoint[nextblock][0]*18, nextpoint[nextblock][1]*18);    //Sets position
					
					//Offsetting according to which piece is next so all of them properly fit inside the next block display
					if (next == 0)
					{
					    	sprite.move(180,366);
					}
					else if (next == 1)
					{
					    	sprite.move(189,366);
					}
					else if (next >= 2 && next <= 6)
					{
					    	sprite.move(179,373);
					}
					window.draw(sprite);
				}
				//Displays the next block after that
				for (int nextblock = 0; nextblock < 4; nextblock++)
				{
					sprite.setTextureRect(IntRect(nextcolor2*18, 0, 18, 18));    //Sets the texture from the same image as the blocks
					sprite.setPosition(nextpoint2[nextblock][0]*18, nextpoint2[nextblock][1]*18);    //Sets position
					
					//Offsetting according to which piece is next so all of them properly fit inside the next block display
					if (next2 == 0)
					{
					    	sprite.move(180,275);
					}
					else if (next2 == 1)
					{
					    	sprite.move(189,275);
					}
					else if (next2 >= 2 && next2 <= 6)
					{
					    	sprite.move(179,282);
					}
					window.draw(sprite);
				}
			}
			
			//Displays the uninteractive rows as they rise every 5 minutes
			for (int donerow = M - 1; donerow > M - level + 1; donerow--)
			{
				for (int donecol = 0; donecol < N; donecol++)
				{
					grey.setTextureRect(IntRect(gameGrid[donerow][donecol]*18, 0, 18, 18));
					grey.setPosition(donecol*18, donerow*18);
					grey.move(28,31); //offset
					window.draw(grey);
				}
			}
        		
        		//Displays the score
			for (int scoreloop = 0; scoreloop < 5; scoreloop++)
			{
				score.setTextureRect(IntRect(scorenumber[scoreloop]*14.5,0,15,18));
				score.setPosition(scoreloop*12,190);
				score.move(238,0);
				window.draw(score);
			}
			
			//Displays the blockshadow according to the height
			if (!bomb)
			{
				for (int shad = 0; shad < 4; shad++)
				{
					shadow.setTextureRect(IntRect(colorNum*18, 0, 18, 18));
					shadow.setPosition(point_1[shad][0]*18, (point_1[shad][1] + height)*18);
					shadow.move(28,31);
					window.draw(shadow);
				}
			}
	

			//---The Final on Which Everything is Drawn Over is Loaded---//
			window.draw(frame);
			
			if (gameover == true)
			{
				title.openFromFile("img/over.wav");	//Changing music to the game over theme when game is over
				title.play();
				title.setLoop(true);
			}
			
			//Checking if game is over to display the gameover screen
			while (gameover == true && game == true)
			{
				
				obj2.loadFromFile("img/GameOver.png");   //Displaying the gameover screen as the background
				window.draw(background);

				
				//Displays the score
				for (int scoreloop = 0; scoreloop < 5; scoreloop++)
				{
					score.setTextureRect(IntRect(scorenumber[scoreloop]*14.5,0,15,18));
					score.setPosition(scoreloop*12,240);
					score.move(130,0);
					window.draw(score);
				}
				window.display();
				
				//Event listener
				while (window.pollEvent(e))
				{ 
				    	if (e.type == Event::Closed)		//If cross/close is clicked/pressed
					{   
						window.close();
						restart = false;      //Closes the window and program
						gameover = false;
						game = false;
					}
					if (e.type == Event::TextEntered)   //Checks if text is entered
					{
			  		  	name += e.text.unicode;   //Adds the entered text to the string
			  		  	if (Keyboard::isKeyPressed(Keyboard::Enter))   //Checks if Enter is pressed
			  		  	{
			  		  		outputScore(totalscore, output, name);    //Outputs the score and name to a file
			  		  		
			  		  		while (!restart && game == true)    //Checks if game is being restarted
			  		  		{
			  		  			obj2.loadFromFile("img/Restart.png");    //Displaying the restart screen as the background
								window.draw(background);
								window.display();
								
								while (window.pollEvent(e))
								{                  
									if (e.type == Event::Closed)		//If cross/close is clicked/pressed
									{   
										window.close();
										restart = false;
										gameover = false;	//Closes the window and program
										game = false;
									}                
								    	if (e.type == Event::KeyPressed)  //Checks if key is pressed
								    	{
										if (e.key.code == Keyboard::R)   //If R is pressed
										{
											restart = true;
											gameover = false;   //Restarts the game
											game = false;
										}
										else if (e.key.code == Keyboard::Enter)    //If Enter is pressed
										{
											restart = false;    //Closes the game
											game = false;
										}
								       }
								 }
			  		  		}
			  		  	}
					}
			    	}

			}
				
			//---The Window that now Contains the Frame is Displayed---//
			window.display();
		}
	} while (restart == true);  //End of do-while loop that checks if game has been restarted
	return 0;
}  //End of Main
