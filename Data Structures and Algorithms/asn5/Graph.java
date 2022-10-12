import java.util.ArrayList;

public class Graph implements GraphADT {

	/**
 	*  Adjacency matrix representation of graph
 	*/
	private Edge[][] graph;
	
	/**
 	*  List of nodes for graph
 	*/
	private ArrayList<Node> nodes;

	
	/**
 	*  Constructor for Graph class. 
 	*  @param n	Number of nodes for graph
 	*/
	public Graph(int n)
	{
		// create arraylist of the provided length
		nodes = new ArrayList<Node>(n);
		
		graph = new Edge[n][n];
		
		// add nodes to the new list
		for(int x = 0; x < n; x++)
			nodes.add(new Node(x));
		
		// graph is empty by default, so all indexes in 2D array are false
		for(int x = 0; x < n; x++)
			for(int y = 0; y < n; y++)
				graph[x][y] = null;
	}
	
	/**
 	*  Determine if a given node exists in the node list
 	*  @param node	Node to search for
 	*  @return True if the given node is contained in the node list
 	*/
	private boolean nodeExists(Node node)
	{
		for(int x = 0; x < nodes.size(); x++)
			if(nodes.get(x).equals(node))
				return true;
		return false;
	}
	
	/**
 	*  Determine if a given edge exists
 	*  @param nodeu	First node of edge
 	*  @param nodev	Second node of edge
 	*  @return True if the given edge exists in the graph
 	*/
	private boolean edgeExists(Node nodeu, Node nodev)
	{
		return ((graph[nodeu.getName()][nodev.getName()] != null) && (graph[nodev.getName()][nodeu.getName()] != null));
	}
	
	/**
 	*  Update the adjacency matrix
 	*  @param newEdge	Edge to modify
 	*/
	private void updateAdjMatrix(Edge newEdge)
	{
		graph[newEdge.firstEndpoint().getName()][newEdge.secondEndpoint().getName()] = newEdge;
		graph[newEdge.secondEndpoint().getName()][newEdge.firstEndpoint().getName()] = newEdge;
	}
	
	/**
 	*  Insert an edge into the graph
 	*  @param nodeu	First end point of edge
 	*  @param nodev	Second end point of edge
 	*  @param type	Type of edge
 	*  @throws GraphException
 	*/
	public void insertEdge(Node nodeu, Node nodev, int type) throws GraphException 
	{
		// search list to ensure the nodes exist
		if(!(nodeExists(nodeu) && nodeExists(nodev)))
			throw new GraphException("Failed to insert edge: One or both of the provided nodes does not exist.");
		else if(edgeExists(nodeu, nodev))
			throw new GraphException("Failed to insert edge: Edge already exists in the graph.");

		// now we know these nodes exist, and there is no existing link between them.
		Edge newEdge = new Edge(nodeu, nodev, type);
		
		// update this edge in the adjacency matrix representing our graph
		updateAdjMatrix(newEdge);
	}


	/**
 	*  Find and return a node given its integer name
 	*  @param u	Integer name of node
 	*  @return Node with integer name
 	*  @throws GraphException
 	*/
	public Node getNode(int u) throws GraphException 
	{

		for(int x = 0; x < nodes.size(); x++)
			if(nodes.get(x).getName() == u)
				return nodes.get(x);

		// if the program is at this point the node must not exist, throw GraphException
		throw new GraphException("Failed get node: No node exists with name " + u);
	}

	
	/**
 	*  Determine if a node belongs to a particular edge
 	*  @param edge	Edge to check
 	*  @param node	Node belonging to edge
 	*  @return True if node belongs to edge
 	*/
	private boolean edgeContains(Edge edge, Node node)
	{
		if(edge == null)
			return false;
		else
			return (edge.firstEndpoint().equals(node) || edge.secondEndpoint().equals(node));
	}
	
	
	/**
 	*  Gets the edges which a particular node belongs to
 	*  @param u	Node to check for incident edges
 	*  @return ArrayList of incident edges
 	*  @throws GraphException
 	*/
	public ArrayList<Edge> incidentEdges(Node u) throws GraphException 
	{
		// is u a legitimate node in the graph?
		if(!nodeExists(u))
			throw new GraphException("Failed to find incident edges on node: Given node does not exist.");
		
		// create an empty list of edges
		ArrayList<Edge> edgeList = new ArrayList<Edge>();
		
		// get the name of the node to search at that level 
		int level = u.getName();
		
		// scan the 2D array looking for edges which contain node u
		for(int x = 0; x < nodes.size(); x++)
		{
			if(edgeContains(graph[level][x], u))
				edgeList.add(graph[level][x]);
		}
		
		if(edgeList.isEmpty())
			return null;
		else
			return edgeList;
	}

	
	/**
 	*  Gets the edge between two nodes if it exists
 	*  @param u	First end point of edge
 	*  @param v	Second end point of edge
 	*  @return Edge between the two given nodes
 	*  @throws GraphException
 	*/
	public Edge getEdge(Node u, Node v) throws GraphException 
	{
		if(edgeExists(u, v))
		{
			return graph[u.getName()][v.getName()];
		}
		else
			throw new GraphException("Failed to get edge: No edge exists between the given nodes.");
	}

	
	/**
 	*  Determines if two nodes are adjacent
 	*  @param u	First node
 	*  @param v	Second node
 	*  @return True if the two nodes are adjacent
 	*  @throws GraphException
 	*/
	public boolean areAdjacent(Node u, Node v) throws GraphException 
	{
		if(!(nodeExists(u) && nodeExists(v)))
			throw new GraphException("Failed to determine if the given nodes are adjacent: One or both of the provided nodes does not exist.");
		
		if(edgeExists(u, v))
			return true;
		else 
			return false;

	}
	
	
	
}






































