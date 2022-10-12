import java.util.ArrayList;

public class NodeData {

	/**
 	*  Key attribute for the data stored in a node
 	*/
	private String name;
	
	/**
 	*  List of MultimediaItems 
 	*/
	private ArrayList<MultimediaItem> media;
	
	/**
 	*  Constructor for NodeData class. Assigns name and creates and empty list of MultimediaItems
 	*  @param newName	Name of node
 	*/
	public NodeData(String newName)
	{
		name = newName;
		media = new ArrayList<MultimediaItem>();
	}
	
	/**
 	*  Adds a MultimediaItem to a node's list
 	*  @param add	MultimediaItem for node to store
 	*/
	public void add(MultimediaItem newItem)
	{
		media.add(newItem);
	}
	
	/**
 	*  Getter method for a node's name
 	*  @return name of node
 	*/
	public String getName()
	{
		return name;
	}
	
	/**
 	*  Getter method for a node's MultimediaItem list
 	*  @return list of MultimediaItems
 	*/
	public ArrayList<MultimediaItem> getMedia()
	{
		return media;
	}
	
	
	
	
	
	
	
	
	
	
	
}






































