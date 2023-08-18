/* PROGRAMMING FUNDAMENTAL'S PROJECT FOR FALL 2022 BS(CS)
 * You need to define the required function in the part instructed here below.
 * Avoid making any unnecessary changes, particularly the ones you don't understand.
 * The different pieces should be defined using an array - uncomment the following code once you are done creating the array.
 * TIP: Understand thoroughly before getting started with coding.
 * */

/* 
   Name: Muhammad Taha
   Student-ID: 22I-0870
   PF-Project
*/

//Header Files
#include<fstream>
using namespace std;
using namespace sf;

/////////////////////////////////////////////
///*** START CODING YOUR FUNTIONS HERE ***///
void onePiece(int& first, int& next, int& next2, int level)  //Function for the first piece so it isn't just a single block
{
	if (first == 0)   //Checks if this is the first piece
        {
        	int type;    //Declaring the variable to choose the type of block
        	if (level == 1)     //Checking the difficulty level
        	{
			type = rand() % 4;    //Choosing a random block from the first 4 if level is 1
		    	for (int generate = 0; generate < 4; generate++)
		    	{
		    		point_1[generate][0] = (BLOCKS[type][generate] % 2) + 4;    //Generating the point from the block array
		        	point_1[generate][1] = BLOCKS[type][generate] / 2;
		    	}
		    	next2 = rand() % 4;    //Generating the next two random blocks from the first 4 if the level is 1
		}
		else
		{
			type = rand() % 7;    //Choosing a random block from the first 7 if level is 7
		    	for (int generate = 0; generate < 4; generate++)
		    	{
		    		point_1[generate][0] = (BLOCKS[type][generate] % 2) + 4;    //Generating the point from the block array
		        	point_1[generate][1] = BLOCKS[type][generate] / 2;
		    	}
		    	next2 = rand() % 7;    //Generating the next random block from the first 4 if the level is 1
		}
        	first++;   //Making sure this function is only called once
        }	
}

bool gameOver(bool& gameover)    //Checks if the game is over
{
	for (int ceiling = 0; ceiling < N; ceiling++)
	{
		if (gameGrid[0][ceiling] != 0)
		{
			gameover = true;   //If block reaches the ceiling, game is over
			return 1;
		}
	}
	gameover = false;   //If block hasn't reached cieling, game isn't over
	return 0;
}

bool colorBomb()     //Generates the color bomb
{
	int bombchance = rand() % 50;     //Has a 1 in 50 chance of randomly making a bomb instead of a piece
	static bool last = false;    //To make sure two bombs don't spawn one after the other
	if (bombchance == 0 && last == false)
	{
		last = true;
		return 1;     //Creating bomb
	}
	last = false;
	return 0;    //Not creating bomb
}

bool anamolyUp(int level)    //Anamoly function that changes if level changes as the rows decrease
{
	for (int lessrow = 0; lessrow < 4; lessrow++)
	{
		if (point_1[lessrow][0] < 0 || point_1[lessrow][0] >= N || point_1[lessrow][1] >= M - level + 2)   //Decreases anomaly as level increases past level 2
		{
		    return false;
		}
		else if (gameGrid[point_1[lessrow][1]][point_1[lessrow][0]])
		{
			return false;
		}
	}
	return true;
}

void scoreSystem(int level, int& totalscore, int linecount)    //Function to calculate score depending on how many lines are destroyed at a time and level
{
	if (level == 1)		//Checks the level
	{
		//Adds score depending on how many lines are destroyed
		if (linecount == 1)
		{
			totalscore += 10;
		}
		if (linecount == 2)
		{
			totalscore += 30;
		}
		if (linecount == 3)
		{
			totalscore += 60;
		}
		if (linecount == 4)
		{
			totalscore += 100;
		}
	}
	else	//Multiplies score by 2 if level is not 1
	{
		if (linecount == 1)
		{
			totalscore += 20;
		}
		if (linecount == 2)
		{
			totalscore += 60;
		}
		if (linecount == 3)
		{
			totalscore += 120;
		}
		if (linecount == 4)
		{
			totalscore += 200;
		}
	}
}

char checkBlockType()	//Function to check the type of the block
{
	if ((point_1[0][0] == point_1[2][0]) && (point_1[0][1] == point_1[1][1]))	//If block is O shaped
	{
		return 'O';
	}
	else if (((point_1[0][0] == point_1[3][0]) && (point_1[1][1] == point_1[2][1])) || ((point_1[0][1] == point_1[3][1]) && (point_1[1][0] == point_1[2][0])))	//If block is T shaped
	{
		return 'T';
	}
	else if (((point_1[1][0] == point_1[3][0]) && (point_1[0][1] == point_1[1][1])) || ((point_1[0][0] == point_1[1][0]) && (point_1[1][1] == point_1[3][1])))	//If block is L shaped
	{
		return 'L';
	}
	else if ((point_1[0][0] == 0) && (point_1[1][0] == 0) && (point_1[3][0] == 0) && (point_1[2][0] == 1) && (point_1[2][1] == point_1[3][1]))
	{
		return 'B';														//If block is J shaped and in the left corner
	}
	else if ((point_1[0][0] == point_1[1][0]) && (point_1[1][0] == point_1[3][0]) && (point_1[2][1] == point_1[3][1]))	//If block is J shaped
	{
		return 'J';
	}
	else if ((point_1[0][0] == point_1[1][0]) && (point_1[1][0] == point_1[3][0]))	//If block is I shaped
	{
		return 'I';
	}
	else if ((point_1[0][0] == point_1[2][0] && point_1[1][0] == point_1[3][0]))	//If block is Z shaped
	{
		return 'Z';
	}
	return 0;
}

//---Piece Starts to Fall When Game Starts---//
void fallingPiece(float& timer, float& delay, int& color, bool& press, bool& bomb, bool& bombstop, int& Irotate, int level, int& next, int& nextcolor, int& next2, int& nextcolor2)
{
	if (timer > delay)	//Checks if the timer is more than the delay
	{
        	for (int vertical = 0; vertical < 4; vertical++)
        	{
		    	point_2[vertical][0] = point_1[vertical][0];    //Assigning the temp point the same x-coordinates as the regular point
		    	point_2[vertical][1] = point_1[vertical][1];    //Assigning the temp point the same y-coordinates as the regular point
		    	point_1[vertical][1] += 1;                   //How much units downward
            	}
            	//Checks if the block reaches the ground and isn't a bomb
		if (!anamolyUp(level) || bombstop == true)
		{
			if (bombstop == false)	//If block isn't a bomb
			{
				for (int stop = 0; stop < 4; stop++)
				{	
					//This part is to fix a bug where certain blocks would fall through the floor at the leftmost column
					if (point_2[stop][0] == 0 && point_2[stop][1] >= M - 1 && checkBlockType() == 'I')
					{
						for (int bug = M - 4; bug < M; bug++)
						{
							gameGrid[bug][0] = color;    //Stores the block's color in the grid
						}
						break;
					}
					else if (checkBlockType() == 'B' && point_2[stop][1] >= M - 1)
					{
						for (int bug = M - 3; bug < M; bug++)
						{
							gameGrid[bug][0] = color;    //Stores the block's color in the grid
						}
						gameGrid[M - 3][1] = color;
						break;
					}
					else
					{
						gameGrid[point_2[stop][1]][point_2[stop][0]] = color;    //Stores the block's color in the grid
					}
				}
			}
			if (!colorBomb())	//If next block won't be a bomb
			{
				//-----------------------------Creates the new block----------------------------------------
				bomb = false;
				int type = next;	//Changes the type of block to the next one that is supposed to fall
				next = next2;
				color = nextcolor;	//Changes the color of block to the next one that is supposed to fall
				nextcolor = nextcolor2;
				for (int generate = 0; generate < 4; generate++)
				{
				    point_1[generate][0] = (BLOCKS[type][generate] % 2) + 4;   //Generating the point from the block array
				    point_1[generate][1] = (BLOCKS[type][generate] / 2);
				}
				if (level == 1)		//Checks level
				{
			    		next2 = rand() % 4;	//Randomly generates the next block to display from the first 4 if level is 1
			    		delay = 0.5;	//Delay is set to 0.5 when a new block is generated if level is 1
			    	}
			    	else
			    	{
			    		next2 = rand() % 7;	//Randomly generates the next block to display it from all 7 if level is not 1
			    		delay = 0.15;	//Delay is set to 0.15 when a new block is generated if level is not 1
			    	}
			    	nextcolor2 = 1 + rand() % 7;	//Randomly generates the next two block's color to display it
				press = false;		//So that when spacebar is pushed, delay is reduced to 0 until a new block is generated
				bombstop = false;	//Stores that the next block is not a bomb
				Irotate = 0;		//To reset the number of rotations of the I block when a new block is generated
			}
			else
			{
				//-------------------------------Creates the bomb--------------------------------------------
				color = 1 + rand() % 7;		//Randomly generates the bomb's color
				for (int generate = 0; generate < 4; generate++)
				{
				    point_1[generate][0] = color;	//Creating bomb as a single point
				    point_1[generate][1] = 1;
				}
				bomb = true;	//Flagging the bomb as true
			}
		}
		timer=0;	//Changing the timer to 0 as block moves downward
	}
}

void movePiece(int& xshift)   //Function to move block horizontally
{
	if (xshift != 0)	//Checks if right or left key is pressed
	{
		for (int move = 0; move < 4; move++)
		{
			point_2[move][0] = point_1[move][0];	//Creates a temporary point to restore to if block is moved outside the grid
			point_1[move][0] += xshift;	//Moves the block left or right
		}
		if (!anamoly())		//If block leaves the grid
		{
			for (int wall = 0; wall < 4; wall++)
			{
				point_1[wall][0] = point_2[wall][0];	//Restores block to original location
			}
		}
		xshift = 0;	//Changing it to 0 until key is pressed again
	}
}

void removeLines(int level, int& totalscore)	//Function to remove the lines
{	
	//Declaring Variables
	int counter = 0, linecount = 0, down = -1;
	
	for (int line = 0; line < M; line++)
	{
		counter = 0;	//Counts number of colums
		for (int length = 0; length < 10; length++)
		{
			if (gameGrid[line][length])
			{
				counter++;	//If column has a block, counter goes up
			}
		}
		if (counter == N)	//If counter is N, it means that all the columns are filled
		{
			for (int down = line - 1; down >= 0; down--)
			{
				for (int length = 0; length < 10; length++)
				{
					gameGrid[down + 1][length] = gameGrid[down][length];	//Moves line downwards
				}
			}
			linecount++;	//Counts number of lines removed for the scoring system
		}
	}
	if (linecount != 0)
	{
		scoreSystem(level, totalscore, linecount);    //Adding score to the totalscore if a line is cleared
	}
}

bool left()	//Function to check left wall when rotating
{
	for (int wall = 0; wall < 4; wall++)
	{
		if (point_1[wall][0] < 0)
		{
			return false;	//Checks if block bypasses the left boundary
		}
	}
	return true;
}

bool right()	//Function to check right wall when rotating
{
	for (int wall = 0; wall < 4; wall++)
	{
		if (point_1[wall][0] >= N)
		{
			return false;	//Checks if block bypasses the left boundary
		}
	}
	return true;
}

void rotateBlocks(bool& turn, int& Irotate, int level)	//Function to rotate blocks
{	
	if (turn)	//Checks if the button to rotate is pressed
	{
		int xcenter = point_1[1][0], ycenter = point_1[1][1], rotate;	//Finding the x and y coordinates of the center of the block
		if (checkBlockType() == 'T' || checkBlockType() == 'L')		//Checking if block is T or L because they have a different center of rotation than the rest of the blocks
		{
			xcenter = point_1[2][0], ycenter = point_1[2][1];	//Finding the x and y coordinates of the center of the T and L blocks
		}
		if (checkBlockType() != 'O')	//Checking if block type is O because it doesn't rotate
		{
			for (int axis = 0; axis < 4; axis++)
			{	
				point_2[axis][0] = point_1[axis][0];	//Temporarily storing the blocks coordinates in a different variable
				point_2[axis][1] = point_1[axis][1];
				
				rotate = point_2[axis][1] - ycenter;	//Finding how much the x-coordinate of the block will rotate
				point_1[axis][0] = xcenter - rotate;	//Rotating the x-coordinate according to its center and rotation
				
				rotate = point_2[axis][0] - xcenter;	//Finding how much the y-coordinate of the block will rotate
				point_1[axis][1] = rotate + ycenter;	//Rotating the y-coordinate according to its center and rotation
			}
			while (!left())		//Checking if block goes through the left wall of the grid
			{
				for (int wall = 0; wall < 4; wall++)
				{
					point_1[wall][0] += 1;		//Moving the block right if it does
				}
			}
			while (!right())	//Checking if block goes through the right wall of the grid
			{
				for (int wall = 0; wall < 4; wall++)
				{
					point_1[wall][0] -= 1;		//Moving the block left if it does
				}
			}
		}
		if (!anamolyUp(level))		//Checking if the block goes through other blocks when rotating
		{
			for (int wall = 0; wall < 4; wall++)
			{
				point_1[wall][0] = point_2[wall][0];		//Resetting the block's position if it goes through the other blocks
				point_1[wall][1] = point_2[wall][1];
			}
		}
		if (checkBlockType() == 'I')	//Checking if the block being rotated is I shaped
		{
			Irotate++;	//Increasing the number of rotations of the I block
		}
		turn = false;	   //Making the rotate false until the UP key is pressed again
	}
}

int blockShadow(bool bomb, int Irotate, int Rows)	//Function to find the height of the block shadow
{
	if (!bomb)	//Checking if block isn't bomb because bomb shouldn't have shadow
	{
		//Declaring variables
		int height, min = 20, check = 0;
		do
		{
			for (int x = 0; x < 4; x++)
			{
				height = Rows - point_1[x][1] - 1;	//Subtracting height depending on the grid
				for (int y = check; y <= Rows; y++)
				{
					if (gameGrid[y][point_1[x][0]])		//Checks if grid has a block
					{
						height -= Rows - y;	 //Decreases height depending on how many blocks there are
						if (min >= height)
						{
							min = height;	//Finds the least possible height of the shadow
							if (min < 0)	//Checks if the height is negative
							{
								check = y + 1;		//Recalculates the height so the shadow isn't above the block
								min = 20;
							}
						}
						break;
					}
				}

			}
		} while (min < 0);	//Loop runs until the minimum is not negative
		
		//This is to fix a glitch that was occurring with the shadow on the left most column of the grid
		check = 0;
		for (int y = 0; y <= Rows; y++)
		{
			if (point_1[0][0] == 0 && gameGrid[y][0] == 0)
			{
				check++;	//Checks if the grid has a block at the leftmost column
			}
		}
		if (check > Rows && checkBlockType() == 'I')	//Checks if the block type is I and there are no blocks in the leftmost column
		{
			min -= point_1[0][1] + 4;	//Decrements the height of the shadow to adjust it properly
			if (Irotate % 2 == 1 && Irotate <= 4)
			{
				min += 3;	//Increases the height of the I block if it is straight so it doesn't fall through the floor
			}
		}
		else if (check > Rows && checkBlockType() == 'L' && point_1[1][0] != point_1[2][0])	//Checks if the block type is L and there are no blocks in the leftmost column
		{
			min -= 1;	//Decreases height so it isn't floating above the surface
		}
		else if (check > Rows && checkBlockType() == 'Z')	//Checks if the block type is Z and there are no blocks in the leftmost column
		{
			min -= 1;	//Decreases height so it isn't floating above the surface
		}
		else if (check > Rows && checkBlockType() == 'T' && point_1[1][0] != point_1[2][0])	//Checks if the block type is T and there are no blocks in the leftmost column
		{
			min -= 1;	//Decreases height so it isn't floating above the surface
		}
		else if (check > Rows && checkBlockType() == 'B')
		{
			min -= 2;
		}
		return min;	//Returns the minimum height calculated
	}
	return 0;
}

void bombDetonation(int color, bool& bomb, bool& bombstop, int Rows)	//Function that makes the bomb destroy blocks
{
	if (bomb && !bombstop)		//Checks if it is a bomb
	{
		bool full = false, half = false, dud = false;		//Declaring type of bomb
		//Declaring other variables
		int x, y, bombcounter = 0;
		for (int width = 0; width < 4; width++)
		{
			if (gameGrid[point_1[width][1] + 1][point_1[width][0]] == color && point_1[width][1] < Rows - 1)	//Checks if bomb lands on a block of the same color
			{
				full = true;	//Makes a full bomb
				break;
			}
			else if (gameGrid[point_1[width][1] + 1][point_1[width][0]] != 0 && point_1[width][1] < Rows - 1)	//Checks if bomb lands on a block of a different color
			{
				half = true;	//Makes a half bomb
				x = point_1[width][0], y = point_1[width][1];	//Calculates the x and y coordinates of the bomb when it lands
				break;
			}
			else if (gameGrid[point_1[width][1] + 1][point_1[width][0]] == 0 && point_1[width][1] == Rows - 2)	//Checks if bomb doesn't land on a block
			{
				dud = true;	//Makes the bomb dissapear and do nothing
				break;
			}
		}
		if (full)	//If it is a full bomb
		{
			bombstop = true;
			for (int blow = 0; blow < Rows; blow++)
			{
				for (int up = 0; up < N; up++)
				{
					if (gameGrid[blow][up] != 0)
					{
						gameGrid[blow][up] = 0;		//Destroys all blocks in the grid
					}
				}
			}
		}
		else if (half)	//If it is a half bomb
		{
			bombstop = true;
			//Creating new variables
			int hcounter = 0, tempx = x;
			while (bombcounter < 5)		//To make sure 4 blocks around the bomb are destroyed
			{
				//These if statements are to destroy the 4 closest blocks around the bomb by changing the x and y coordinates arount the bomb
				if (bombcounter == 0)
				{
					gameGrid[y][x] = 0;
					bombcounter++;
					x--;
				}
				if (gameGrid[y][x] != 0 && gameGrid[y][x] < Rows && gameGrid[y][x] < N)
				{
					gameGrid[y][x] = 0;
					bombcounter++;
				}
				else if (gameGrid[y+1][x] != 0 && gameGrid[y+1][x] < Rows && gameGrid[y+1][x] < N)
				{
					gameGrid[y+1][x] = 0;
					bombcounter++;
				}
				else if (hcounter < 3)
				{
					x++;
					hcounter++;
				}
				else if (hcounter >= 3 && hcounter < 8)
				{
					x--;
					hcounter++;
				}
				else
				{
					x = tempx;
					y++;
					hcounter = 0;
				}
			}
		}
		else if (dud)	//If bomb didn't land on a block
		{
			bombstop = true;
		}
	}
}

void levelUp(int& level, float totaltime, int& Rows, int startlevel)	//Increasing the difficulty level
{
	int simpletime = totaltime / 100;	//The overall time is around 1/100th of a second, so dividing it by 100 to calculate the time every second
	simpletime /= 300;	//Dividing by 300 to calculate 5 minutes, which is 300 seconds
	if (startlevel == 1)	//Checking the level that was chosen at the start
	{
		level = simpletime + 1;		//Increasing the level after every 5 minutes
	}
	else
	{
		level = simpletime + 2;		//Increasing the level further after every 5 minutes if starting level was higher
	}
	if (level > 2)		//Once levels are past 2, every new level, the number of playable rows will decrease from the bottom
	{
		Rows = M - level + 2;		//Making bottom rows un-interactable
	}
}

void scoreDisplay(int totalscore, int scorenumber[])	//Displaying the score
{
	int scoreCounter = 0, temp;
	while (scoreCounter  < 5)
	{
		scorenumber[scoreCounter] = totalscore % 10;	//Storing the overall score in an array to multiply with the display sprite to show the number accordingly
		totalscore /= 10;	//Then dividing total score to find the next digit
		scoreCounter++;		//Calculates upto 5 digits
	}
	for (int reverse = 0; reverse < 2; reverse++)	//Reversing the score array because they are stored in reverse order
	{
		temp = scorenumber[reverse];
		scorenumber[reverse] = scorenumber[4 - reverse];
		scorenumber[4 - reverse] = temp;
	}

}

void outputScore(int totalscore, bool& output, string name)	//Outputting the score to a file
{
	if (output == false)	//Checks if it needs to output
	{
		ofstream highscore;	//Declaring name of file
		highscore.open("High Score.txt", ios::app);	//Opening file
		highscore << "Name: ";		//Storing the name from string onto the file
		highscore << name << endl;
		highscore << "Score: ";		//Storing the score from game onto the file
		highscore << totalscore << endl;
		highscore.close();	//Closing the file
		output = true;
	}
}

void nextPiece(int next, int next2, int nextpoint[4][2], int nextpoint2[4][2])	//Finds the next two pieces to display
{
	for (int generate = 0; generate < 4; generate++)
	{
		nextpoint[generate][0] = (BLOCKS[next][generate] % 2) + 4;	//Generating the next point from the block array to display at the right
		nextpoint[generate][1] = (BLOCKS[next][generate] / 2);
	}
	for (int generate = 0; generate < 4; generate++)
	{
		nextpoint2[generate][0] = (BLOCKS[next2][generate] % 2) + 4;	//Generating the next point after that from the block array to display at the right
		nextpoint2[generate][1] = (BLOCKS[next2][generate] / 2);
	}
}

void restartGame(bool& restart, int Rows, int& totalscore, bool& start, bool& game, float& totaltime)	//Restarting the game
{
	if (restart)	//Checks if the game is supposed to restart
	{
		for (int blow = 0; blow < Rows; blow++)
		{
			for (int up = 0; up < N; up++)
			{
				if (gameGrid[blow][up] != 0)
				{
					gameGrid[blow][up] = 0;		//Destroys all the blocks in the grid to start over
				}
			}
		}
		totalscore = 0;		//Changes total score back to 0
		totaltime = 0;		//Changes time elapsed back to 0
		restart = false;
		start = false;		//Takes you to start of the game
		game = true;
	}
}

///*** YOUR FUNCTIONS END HERE ***///
/////////////////////////////////////
