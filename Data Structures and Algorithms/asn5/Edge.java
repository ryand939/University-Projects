


public class Edge {

	/**
 	*  First end point of edge
 	*/
	private Node firstEndpoint;
	
	/**
 	*  Second end point of edge
 	*/
	private Node secondEndpoint;
	
	/**
 	*  Identifier for type of edge between nodes
 	*/
	int type;
	
	/**
 	*  Constructor for Edge class. Creates an edge between two given nodes and defines type.
 	*  @param u	First end point of edge
 	*  @param v Second end point of edge 
 	*  @param edgeType	Identifier for type of edge between nodes
 	*/
	public Edge(Node u, Node v, int edgeType)
	{
		firstEndpoint = u;
		secondEndpoint = v;
		type = edgeType;
	}
	
	/**
 	*  Getter method for first end point
 	*  @return first end point of edge
 	*/
	public Node firstEndpoint()
	{
		return firstEndpoint;
	}
	
	/**
 	*  Getter method for second end point
 	*  @return second end point of edge
 	*/
	public Node secondEndpoint()
	{
		return secondEndpoint;
	}
	
	/**
 	*  Getter method for type of edge
 	*  @return integer type of edge
 	*/
	public int getType()
	{
		return type;
	}
	
	/**
 	*  Setter method for type of edge
 	*  @param newType	New integer value for edge type
 	*/
	public void setType(int newType)
	{
		type = newType;
	}
	
	/**
 	*  Determines if two edges are equal
 	*  @param otherEdge	Edge to compare
 	*/
	public boolean equals(Edge otherEdge)
	{
		// check if the otherEdge connects the same two nodes
		return ((firstEndpoint.equals(otherEdge.firstEndpoint) && secondEndpoint.equals(otherEdge.secondEndpoint)) ||
				firstEndpoint.equals(otherEdge.secondEndpoint) && secondEndpoint.equals(otherEdge.firstEndpoint));
	}
	
}















