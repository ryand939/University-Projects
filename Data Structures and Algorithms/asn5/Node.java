
public class Node {

	/**
 	*  Name attribute of node
 	*/
	private int name;
	
	/**
 	*  Boolean to represent whether or not a node has been visited
 	*/
	private boolean marked;
	
	
	/**
 	*  Constructor for Node class. Assigns the node's name.
 	*  @param nodeName	Integer for node's name
 	*/
	public Node(int nodeName)
	{
		marked = false;
		name = nodeName;
	}
	
	/**
 	*  Setter method for mark
 	*  @param mark	Whether or not a node has been marked
 	*/
	public void setMark(boolean mark)
	{
		marked = mark;	
	}
	
	/**
 	*  Getter method for mark
 	*  @return True if node is marked
 	*/
	public boolean getMark()
	{
		return marked;
	}
	
	
	/**
 	*  Getter method for name attribute of node
 	*  @return name of node
 	*/
	public int getName()
	{
		return name;
	}
	
	/**
 	*  Determines if two nodes are equal
 	*  @param otherNode	Node to compare
 	*/
	public boolean equals(Node otherNode)
	{
		return (name == otherNode.name);
	}
}
