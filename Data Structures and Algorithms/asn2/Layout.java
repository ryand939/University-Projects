
public class Layout {

	/**
 	*  String representation of board layout
 	*/
	private String boardLayout;
	
	/**
 	*  Score associated to each layout object
 	*/
	private int score;
	
	/**
 	*  Constructor for Layout object, initializes member variables
 	*  @param boardLayout	String representation of board layout
 	*  @param score	Score associated with layout object
 	*/
	public Layout(String boardLayout, int score)
	{
		this.boardLayout = boardLayout;
		this.score = score;
	}
	
	/**
 	*  Get string representation of board layout
 	*  @return boardLayout	String representation of board layout
 	*/
	public String getBoardLayout()
	{
		return boardLayout;
	}
	
	/**
 	*  Get score of layout
 	*  @return score	Score associated to layout object
 	*/
	public int getScore()
	{
		return score;
	}
	
}
