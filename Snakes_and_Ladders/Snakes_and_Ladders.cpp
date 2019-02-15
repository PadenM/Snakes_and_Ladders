//	Paden McCown
//	Snakes_and_Ladders.cpp
//	A text based snakes and ladders maze that takes
//	a formatted text file as input to create the maze.


// Note:	File picker not implemented.
//			Different file names can be accepted for config by changing
//			"Maze.txt" on line 539 to desired name.

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <ctime>
#include <ctype.h>
#include <cstdlib>
using namespace std;

class Node {
public:
	Node(string newname);
	Node();
	void setNodeName(string newname);
	string getNodeName();
	void attachNewNode(Node *newNode, int direction);
	Node *getAttachedNode(int direction);
	void attachSnakeLadderNode(Node *newNode);
	Node *getSnakeLadderNode();
private:
	string name;
	Node *attachedNodes[4];
	Node *snakeOrLadderNodes;
};

class Player {
public:
	Player(string newName, Node* start);
	Player();
	void setPlayerName(string newName);
	string getPlayerName();
	void setPos(Node *newNode);
	Node *getPos();
	void setMoveCount(int newMoves);
	int getMoveCount();
	void addStep();
	int getSteps();
	string pathTaken();
	void addToPath(string room);
	void checkForSnakeLadder();
private:
	string name;
	Node *pos;
	int moves;
	int steps;
	string path;
};

class Maze {
public:
	Maze();
	void loadMaze(string fileName);
	void gameInstance(string textFile);
private:
	Node* gameMaze;
	vector<Player> playerVec;
	Node* startPoint;
	Node* endPoint;
};


//Node functions definitions

//Function:	Node constructor
//Inputs:		New name to assign to the name of the node
//Outputs:		
//Description:	Creates a new node a assigns newname to name
Node::Node(string newname)
{
	name = newname;
}

//Function:	Node default constructor
//Inputs:		
//Outputs:		
//Description:	Default constructor for the node objects
Node::Node()
{
	name = "notfound";
}

//Function:	setNodeName
//Inputs:		new name for node
//Outputs:		
//Description:	Setter function for names of Node objects
void Node::setNodeName(string newname)
{
	name = newname;
}

//Function:	getNodeName
//Inputs:		
//Outputs:	the name of the node
//Description:	getter function for names of Node objects
string Node::getNodeName()
{
	return name;
}

//Function:	attachNewNode
//Inputs:		new node to attach; direction : 0 = north, 1 = east, 2 = south, 3 = west
//Outputs:		
//Description:	attaches a new node to the current node in the specified direction
void Node::attachNewNode(Node *newNode, int direction)
{
	attachedNodes[direction] = newNode;
}

//Function:	getAttachedNode
//Inputs:		direction : 0 = north, 1 = east, 2 = south, 3 = west
//Outputs:	the specified attached node
//Description:	returns the specified attached node
Node * Node::getAttachedNode(int direction)
{
	return attachedNodes[direction];
}

//Function:	attachSnakeLadderNode
//Inputs:		new node pointer
//Outputs:		
//Description:	attaches the given node as a snake/ladder
void Node::attachSnakeLadderNode(Node * newNode)
{
	snakeOrLadderNodes = newNode;
}

//Function:	getSnakeLadderNode	
//Inputs:		
//Outputs:	the attached snake/ladder node	
//Description:	returns the attached snake/ladder node
Node * Node::getSnakeLadderNode()
{
	return snakeOrLadderNodes;
}

//Player functions definitions


//Function:	Player constructor
//Inputs:		new name, starting node
//Outputs:	
//Description:	Creates a new player with specified name and adds 
//			starting point to path history.
Player::Player(string newName, Node* start)
{
	name = newName;
	addToPath(start->getNodeName());
}

//Function:	Player default constructor
//Inputs:		
//Outputs:	
//Description:	Default constructor for Player
Player::Player()
{
	name = "noName";
}

//Function:	setPlayerName
//Inputs:		new name
//Outputs:	
//Description:	setter method for Player name
void Player::setPlayerName(string newName)
{
	name = newName;
	steps = 0;
}

//Function:	getPlayerName
//Inputs:		
//Outputs:	
//Description:	getter method for Player name
string Player::getPlayerName()
{
	return name;
}

//Function:	setPos
//Inputs:		new node
//Outputs:	
//Description:	Sets position of player to the passed node pointer
void Player::setPos(Node * newNode)
{
	pos = newNode;
}

//Function:	getPos
//Inputs:		
//Outputs:	the player's position
//Description:	Returns the players position in the maze
Node * Player::getPos()
{
	return pos;
}

//Function:	setMoveCount
//Inputs:		int number of moves
//Outputs:	
//Description:	sets the player's current number of allowed moves
void Player::setMoveCount(int newMoves)
{
	moves = newMoves;
}

//Function:	getMoveCount
//Inputs:		
//Outputs:	the number of moves the player has at the moment
//Description:	Returns the current number of allowed moves the player has
int Player::getMoveCount()
{
	return moves;
}

//Function:	addStep
//Inputs:		
//Outputs:	
//Description:	adds one to the player's step count in the maze
void Player::addStep()
{
	steps++;
}

//Function:	getSteps
//Inputs:		
//Outputs:	the number of steps taken
//Description:	Returns the player's number of steps taken
int Player::getSteps()
{
	return steps;
}

//Function:	addToPath
//Inputs:		
//Outputs:	
//Description:	Adds to the player's path history
void Player::addToPath(string room)
{
	path = path + " " + room;
}

//Function:	pathTaken
//Inputs:		
//Outputs:	the player's path history
//Description:	Returns the player's path history
string Player::pathTaken()
{
	return path;
}

void Player::checkForSnakeLadder()
{
	if (getPos()->getSnakeLadderNode() != NULL) //If player finds snake or ladder, have them take it to the connected node.
	{

		cout << "\nYou found a snake or ladder that takes you from "
			<< getPos()->getNodeName() << " to " << getPos()->getSnakeLadderNode()->getNodeName();
		setPos(getPos()->getSnakeLadderNode());
		addToPath(getPos()->getNodeName());
		checkForSnakeLadder();	//Uses recursion to check each successive node moved to from a snake/ladder again to see
								// if it is also a snake/ladder
	}
}

//
//Maze function definitions
//

//Function:	Maze constructor
//Inputs:		
//Outputs:	
//Description:	Maze constructor
Maze::Maze()
{

}

//Function:	loadMaze
//Inputs:		fileName
//Outputs:		
//Description:	loads the maze from a configuration text file
void Maze::loadMaze(string fileName)
{
	fstream stream;
	string fileLine;
	int mazeSize;
	string startNode;
	string endNode;
	string temp;
	stream.open(fileName.c_str(), ios::in);
	stream >> mazeSize;
	stream >> startNode;
	stream >> endNode;
	getline(stream, fileLine);
	gameMaze = new Node[mazeSize];

	//Creating and naming nodes
	for (int i = 0; i < mazeSize; i++)
	{
		getline(stream, fileLine);
		stringstream ss(fileLine);
		getline(ss, fileLine, ' ');
		Node newNode(fileLine);
		gameMaze[i] = newNode;
	}
	stream.close();
	stream.open(fileName.c_str(), ios::in);
	stream >> temp;
	stream >> temp;
	stream >> temp;
	getline(stream, fileLine);
	cout << "\nLoading " << fileName << "...";
	for (int i = 0; i < mazeSize; i++)
	{
		int attachedNodeInt;
		getline(stream, fileLine);
		stringstream ss(fileLine);
		for (int j = -1; j < 5; j++)
		{
			getline(ss, fileLine, ' ');
			bool attached = false;
			if (fileLine != "*")
			{
				for (int k = 0; k < mazeSize; k++)
				{
					if (fileLine == gameMaze[k].getNodeName())
					{
						if (j == 4 && !attached)
						{
							gameMaze[i].attachSnakeLadderNode(&gameMaze[k]);
							attached = true;
							break;
						}
						else if (j < 4 && j > -1 && !attached)
						{
							gameMaze[i].attachNewNode(&gameMaze[k], j);
							attached = true;
							break;
						}
						else
						{
							break;
						}
					}
				}
			}//end k for loop
		}//end j for loop
	}//end i for loop
	stream.close();
	for (int i = 0; i < mazeSize; i++)
	{
		if (gameMaze[i].getNodeName() == startNode)
		{
			startPoint = &gameMaze[i];
			//cout << "\nStart point is " << startPoint->getNodeName();
		}
		if (gameMaze[i].getNodeName() == endNode)
		{
			endPoint = &gameMaze[i];
		}
	}
}

//Function:	gameInstance	
//Inputs:		textFile containing maze config data
//Outputs:	
//Description:	Contains the maze traversal and current players in game instance
void Maze::gameInstance(string textFile)
{
	if (!std::ifstream(textFile.c_str()))
	{
		cout << "\nConfiguration file not found.\nMake sure Maze.txt is is the same directory as this exe.";
		std::getchar();
	}
	else
	{
		loadMaze(textFile);
		cout << "\n====================================================="
			<< "\n|    Welcome to the Snakes and Ladders Maze Game    |"
			<< "\n=====================================================\n";
		bool keepGoing = true;
		int playerCount = 0;
		while (keepGoing)
		{
			cout << "\nEnter the number of players: ";
			cin >> playerCount;
			if (!cin)
			{
				cin.clear();
				cin.ignore(500, '\n');
				cout << "\nInvalid input. Please enter a number.";
			}
			else
			{
				string tempName;
				for (int i = 0; i < playerCount; i++)
				{
					cout << "\nEnter the name for player #" << i + 1 << ": ";
					cin >> tempName;
					Player newPlayer(tempName, startPoint);
					newPlayer.setPos(startPoint);
					playerVec.push_back(newPlayer);
				}
				keepGoing = false;
			}
		}

		keepGoing = true;
		//This will loop until a player reaches the end of the maze.
		while (keepGoing)
		{
			string temp;
			//This will loop until each player has had a turn
			for (int i = 0; i < playerCount; i++)
			{
				if (!keepGoing)
				{
					break;
				}
				cout << "\n\n========================="
					 << "\n|\t" << playerVec[i].getPlayerName() << "'s turn\t|"
					 << "\n========================="
					 << "\n\nPress enter to roll the dice";
				cin.ignore();
				temp = cin.get();
				srand(time(0));
				int roll = rand() % 6 + 1;
				cout << "\n" << playerVec[i].getPlayerName() << " rolled a " << roll
					 << "\n" << playerVec[i].getPlayerName() << " can move " << roll << " times";
				playerVec[i].setMoveCount(roll);
				//This will loop until the current players moves are all used
				for (int j = 0; j < playerVec[i].getMoveCount(); j++)
				{
					if (!keepGoing)
					{
						break;
					}
					cout << "\n" << playerVec[i].getPlayerName() << " is currently in Room "
						<< playerVec[i].getPos()->getNodeName() << " . You can go ";
					//Loops to print all of the available directions to travel next
					bool first = true;
					for (int k = 0; k < 4; k++)
					{
						if (!first && playerVec[i].getPos()->getAttachedNode(k) != NULL)
						{
							cout << "or ";
						}
						if (playerVec[i].getPos()->getAttachedNode(k) != NULL)
						{
							first = false;
							if (k == 0)
								cout << "North ";
							else if (k == 1)
								cout << "East ";
							else if (k == 2)
								cout << "South ";
							else if (k == 3)
								cout << "West ";
						}
					}

					string userString;
					bool badChoice = true;
					while (badChoice)
					{
						cout << "\n... Which way will you go? ";
						cin >> userString;
						char userChoice = tolower(userString.at(0));
						int directionInt = -1;
						if (userChoice == 'n')
						{
							directionInt = 0;
							badChoice = false;
						}
						else if (userChoice == 'e')
						{
							directionInt = 1;
							badChoice = false;
						}
						else if (userChoice == 's')
						{
							directionInt = 2;
							badChoice = false;
						}
						else if (userChoice == 'w')
						{
							directionInt = 3;
							badChoice = false;
						}

						if (directionInt == -1 || playerVec[i].getPos()->getAttachedNode(directionInt) == NULL)
						{
							badChoice = true;
						}
						if (!badChoice)
						{
							playerVec[i].setPos(playerVec[i].getPos()->getAttachedNode(directionInt)); // Move player to chosen node.
							playerVec[i].addStep();
							playerVec[i].addToPath(playerVec[i].getPos()->getNodeName());
							playerVec[i].checkForSnakeLadder();
							badChoice = false;
							if (playerVec[i].getPos()->getNodeName() == endPoint->getNodeName())
							{
								keepGoing = false;
								cout << "\nCongratulations! " << playerVec[i].getPlayerName() << " has reached the end!";
								cout << "\nIt took " << playerVec[i].getSteps() << " steps.";
								cout << "\nYou took this path: " << playerVec[i].pathTaken() << "\n";
							}
						}
						else
						{
							cout << "\nThat is not a valid option.";
						}
					}//end while
				}
			}
		}
	}
}


//Function:	main function
//Inputs:		
//Outputs:	0
//Description:	
int main()
{
	Maze SnakeLadder;
	SnakeLadder.gameInstance("Maze.txt");
	return 0;
}

