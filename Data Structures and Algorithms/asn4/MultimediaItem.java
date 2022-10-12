


public class MultimediaItem {
	
	/**
 	*  Content of the MultimediaItem
 	*/
	private String content;
	
	/**
 	*  Type of media. 1 for text, 2 for audio
 	*  3 for image, 4 for HTML
 	*/
	private int type;
	
	/**
 	*  Constructor for MultimediaItem class. Assigns content and type
 	*  @param newContent	Content of the MultimediaItem
 	*  @param type	Type of media
 	*/
	public MultimediaItem(String newContent, int newType)
	{
		content = newContent;
		type = newType;
	}
	
	/**
 	*  Getter method for content of MultimediaItem
 	*  @return content
 	*/
	public String getContent()
	{
		return content;
	}
	
	/**
 	*  Getter method for MultimediaItem type
 	*  @return type
 	*/
	public int getType() 
	{
		return type;
	}


}











































