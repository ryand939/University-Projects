
public class Board implements BoardADT{

	/**
 	*  2D char array to represent players on a game board
 	*/
	private char[][] theBoard;
	
	/**
 	*  Max number of empty positions
 	*/
	private int emptyPositions;
	
	/**
 	*  Max level of game tree to explore
 	*/
	private int maxLevels;
	
	/**
 	*  Constructor for Board class. Initializes variables relevant to the game
 	*  @param boardSize	Width and height of game board
 	*  @param emptyPositions	Minimum number of empty positions on the board
 	*  @param maxLevels	Maximum level of game tree to explore
 	*/
	public Board(int boardSize, int emptyPositions, int maxLevels)
	{
		theBoard = new char[boardSize][boardSize];
		this.emptyPositions = emptyPositions;
		this.maxLevels = maxLevels;
		
		
		// initialize all indices of char array to empty/e
		for(int i = 0; i < boardSize; i++)
			for(int x = 0; x < boardSize; x++)
				theBoard[i][x] = 'e';
	}
	
	/**
 	*  Initializes and returns a new dictionary
 	*  @return new dictionary of size 9901
 	*/
	public Dictionary makeDictionary()
	{
		return new Dictionary(9901);
	}

	/**
 	*  Saves specified tile in the game board
 	*/
	public void saveTile(int i, int j, char symbol)
	{
		theBoard[i][j] = symbol;
	}

	/**
 	*  Determines if the current state of the game is a draw
 	*  @param player	Determines if this player can make a move
 	*  @param minEmpty 	Minimum tiles that must remain empty
 	*  @return True if the game state is a draw, false otherwise
 	*/
	public boolean isDraw(char player, int minEmpty)
	{
		int numEmpty = 0;
		
		// get the number of empty tiles on the board
		for(int i = 0; i < theBoard.length; i++)
			for(int x = 0; x < theBoard.length; x++)
				if(theBoard[i][x] == 'e')
					numEmpty++;

		if((minEmpty == 0) && (numEmpty == 0))
			return true;
		else if((numEmpty == minEmpty) && !(playerHasAdjacents(player)))
			return true;
		else
			return false;
	}
	
	/**
 	*  Determines if there are empty tiles adjacent to a player
 	*  @param player	Determine if this player has an empty tile adjacent to them
 	*  @return True if the player has an empty tile adjacent to them
 	*/
	private boolean playerHasAdjacents(char player) 
	{
		// cases where we are checking the edge of the 2D array
		boolean canCheckUp, canCheckRight, canCheckDown, canCheckLeft;
		
		for(int x = 0; x < theBoard.length; x++)
			for(int y = 0; y < theBoard.length; y++)
			{
				if(theBoard[x][y] == 'e')
				{
					// evaluate the position of the current char and ensure we do not go out of bounds
					canCheckUp    = (y == 0) 				   ? false : true;
					canCheckDown  = (y == theBoard.length - 1) ? false : true;
					canCheckLeft  = (x == 0) 				   ? false : true;
					canCheckRight = (x == theBoard.length - 1) ? false : true;
					
					// if any of these conditions return true, there are still moves to be made for the player
					// meaning there is no draw just yet
					if(((canCheckRight && (theBoard[x + 1][y] == player)) || ((canCheckRight && canCheckUp)   && (theBoard[x + 1][y - 1] == player)) ||							
						(canCheckUp    && (theBoard[x][y - 1] == player)) || ((canCheckLeft && canCheckUp)    && (theBoard[x - 1][y - 1] == player)) ||						
						(canCheckDown  && (theBoard[x][y + 1] == player)) || ((canCheckDown && canCheckRight) && (theBoard[x + 1][y + 1] == player)) ||						
						(canCheckLeft  && (theBoard[x - 1][y] == player)) || ((canCheckDown && canCheckLeft)  && (theBoard[x - 1][y + 1] == player))))
						return true;
				}
			}
		return false;
	}

	/**
 	*  Determines if a player has won based off their positions in the game board
 	*  @param player	Player to determine victory status for
 	*  @return True if the player has won
 	*/
	public boolean winner(char player)
	{
		return checkHorizontalWinner(player) || checkVerticalWinner(player) || checkDiagonalWinner(player);
	}
	
	/**
 	*  Determines if a player has a full vertical row of their tiles
 	*  @param player	Player to check
 	*  @return True if the player has a full vertical row of their tiles
 	*/
	private boolean checkVerticalWinner(char player) {
		int count;
		for(int y = 0; y < theBoard.length; y++)
		{
			count = 0;
			for(int x = 0; x < theBoard.length; x++)
				if(theBoard[x][y] == player)
					count++;
			
			if(count == theBoard.length)
				return true;
		}
		return false;
	}

	/**
 	*  Determines if a player has a full horizontal row of their tiles
 	*  @param player	Player to check
 	*  @return True if the player has a full horizontal row of their tiles
 	*/
	private boolean checkHorizontalWinner(char player) {
		int count;
		for(int x = 0; x < theBoard.length; x++)
		{
			count = 0;
			for(int y = 0; y < theBoard.length; y++)
				if(theBoard[x][y] == player)
					count++;
			
			if(count == theBoard.length)
				return true;
		}
		return false;
	}

	/**
 	*  Determines if a player has filled a diagonal on the board with their tiles
 	*  @param player	Player to check
 	*  @return True if the player has filled a diagonal on the board with their tiles
 	*/
	private boolean checkDiagonalWinner(char player)
	{
		int count = 0;
		// check top left to bottom right
		for(int d = 0; d < theBoard.length; d++)
			if(theBoard[d][d] == player)
				count++;
		
		if(count == theBoard.length)
			return true;
		
		// check top right to bottom left
		count = 0;
		int x = theBoard.length - 1;
		int y = 0;
		while((x >= 0) && (y < theBoard.length))
		{
			if(theBoard[x][y] == player)
				count++;
			x--;
			y++;
		}
		
		if(count == theBoard.length)
			return true;
		
		return false;
	}

	/**
 	*  Evaluates the current state of the game
 	*  @param player	Player to check
 	*  @param minEmpty	Minimum number of empty tiles
 	*  @return	0 if a human has won, 3 if a computer has won, 2 if the game is a draw, 1 if the game is incomplete
 	*/
	public int evaluate(char player, int minEmpty)
	{
		if(winner('h'))
			return 0;
		else if(winner('c'))
			return 3;
		else if(isDraw(player, minEmpty))
			return 2;
		else
			return 1;
	}

	/**
 	*  Determines if the board layout has been stored already
 	*  @param table	Dictionary to check
 	*  @return	-1 if the item does not exist in the table, otherwise return its associated score
 	*/
	public int repeatedLayout(Dictionary table)
	{
		// first represent board as string
		String s = "";
		for(int column = 0; column < theBoard.length; column++)
			for(int row = 0; row < theBoard.length; row++)
				s += theBoard[column][row];
		
		// check if there is a data item in the dictionary with key s
		return table.getScore(s);
	} 

	/**
 	*  Stores the layout in the dictionary
 	*  @param table	Dictionary to store layout
 	*  @param score	Associated score with layout
 	*/
	public void storeLayout(Dictionary table, int score)
	{
		// first represent board as string
		String s = "";
		for(int column = 0; column < theBoard.length; column++)
			for(int row = 0; row < theBoard.length; row++)
				s += theBoard[column][row];
		
		Layout uniqueLayout = new Layout(s, score);
		table.put(uniqueLayout);
	} 

	/**
 	*  Determines if a position in the board is empty
 	*  @param row	Row number of specified tile
 	*  @param column	column number of specified tile
 	*  @return True if the position is an empty tile
 	*/
	public boolean positionIsEmpty(int row, int column)
	{
		return theBoard[row][column] == 'e';
	}

	/**
 	*  Determines if a position in the board is a computer tile
 	*  @param row	Row number of specified tile
 	*  @param column	column number of specified tile
 	*  @return True if the position is a computer tile
 	*/
	public boolean isComputerTile(int row, int column)
	{
		return theBoard[row][column] == 'c';
	}

	/**
 	*  Determines if a position in the board is a human tile
 	*  @param row	Row number of specified tile
 	*  @param column	column number of specified tile
 	*  @return True if the position is a human tile
 	*/
	public boolean isHumanTile(int row, int column)
	{
		return theBoard[row][column] == 'h';
	}

}
