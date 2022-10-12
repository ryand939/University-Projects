
import java.io.*;
import java.util.*;

public class Solver {
	
	/**
 	*  Graph representation of labyrinth
 	*/
	private Graph graph;
	
	/**
 	*  Size of board
 	*/
	private int size;
	
	/**
 	*  Length of labyrinth
 	*/
	private int length;
	
	/**
 	*  Width of labyrinth
 	*/
	private int width;
	
	/**
 	*  Number of blast bombs
 	*/
	private int numBlastBomb;
	
	/**
 	*  Number of melt bombs
 	*/
	private int numMeltBomb;
	
	/**
 	*  Entrance node of labyrinth
 	*/
	private Node entrance;
	
	/**
 	*  Exit node of labyrinth
 	*/
	private Node exit;

	
	/**
 	*  Constructor for Solver class. Creates a graph based on labyrinth file
 	*  @param inputFile	Name of file containing labyrinth data
 	*  @throws LabyrinthException
 	*/
	public Solver(String inputFile) throws LabyrinthException
	{
		// new line for reader
		String newLine;
		
		// 2D map representation of labyrinth for easy reading
		char[][] labMap;
		
		// number of characters to read vertical
		int limitVertical;
		
		// number of characters to read horizontal
		int limitHorizontal;
		
		try
		{
			// prepare bufferedReader for reading input file line by line
			FileInputStream fileInStream = new FileInputStream(inputFile);
			DataInputStream dataInStream = new DataInputStream(fileInStream);
			BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(dataInStream));

			// first five arguments are labyrinth info
			size = Integer.parseInt(bufferedReader.readLine()); 
			width = Integer.parseInt(bufferedReader.readLine()); 
			length = Integer.parseInt(bufferedReader.readLine()); 
			numBlastBomb = Integer.parseInt(bufferedReader.readLine()); 
			numMeltBomb = Integer.parseInt(bufferedReader.readLine()); 
			
			// first line of labyrinth
			newLine = bufferedReader.readLine();
			
			// establish character limits given the file input
			limitVertical = (length + (length - 1));
			limitHorizontal = (width + (width - 1));
			labMap = new char[limitVertical][limitHorizontal];
			
			
			// create the 2D labyrinth map
			for(int x = 0; newLine != null; x++)   
			{
				for(int y = 0; y < limitHorizontal; y++)
					labMap[x][y] = newLine.charAt(y);
				
				newLine = bufferedReader.readLine();
			}
			dataInStream.close(); 
			
			// the nodes are named starting at integer 0
			int nodeCount = 0;
			
			// we need length*width # of nodes in our labyrinth
			graph = new Graph(length * width);
			for(int x = 0; x < limitVertical; x += 2)   
			{
				for(int y = 0; y < limitHorizontal; y += 2)
				{
					// labMap[x][y] will either be a o e or x
					// link edge to right if possible
					if(y != (limitHorizontal - 1) && labMap[x][y + 1] != '*')
						graph.insertEdge(graph.getNode(nodeCount), graph.getNode(nodeCount + 1), edgeToType(labMap[x][y + 1]));
					
					// link edge below if possible
					if(x != (limitVertical - 1) && labMap[x + 1][y] != '*')
						graph.insertEdge(graph.getNode(nodeCount), graph.getNode(nodeCount + width), edgeToType(labMap[x + 1][y]));
					
					// character is labyrinth entrance
					if(labMap[x][y] == 'e')
						entrance = graph.getNode(nodeCount);
					// character is labyrinth exit
					else if(labMap[x][y] == 'x')
						exit = graph.getNode(nodeCount);
					
					nodeCount++;
				}
				
			}

		}
		catch(FileNotFoundException e){
			throw new LabyrinthException("Labyrinth file does not exist.");
		}
		catch(Exception e){
			System.err.println(e.getMessage());
		}
		

	}
	
	/**
 	*  Getter method for graph object
 	*  @return graph representation of labyrinth
 	*  @throws LabyrinthException
 	*/
	Graph getGraph() throws LabyrinthException
	{
		if(graph != null)
			return graph;
		else
			throw new LabyrinthException("Error getting graph: Graph is undefined.");
	}
	
	/**
 	*  Gets a character and returns the integer type of edge
 	*  @param edgeChar Character to convert
 	*  @return integer type of edge
 	*/
	private int edgeToType(char edgeChar)
	{
		if(edgeChar == 'M' || edgeChar == 'm')
			return 4;
		else if(edgeChar == 'R' || edgeChar == 'r')
			return 3;
		else if(edgeChar == 'B' || edgeChar == 'b')
			return 2;
		else if(edgeChar == '-' || edgeChar == '|')
			return 1;
		else
			return -1;
	}
	
	/**
 	*  Gets an iterator containing a node path to the labyrinth exit or null if there is no exit.
 	*  @return Path of nodes to exit represented by iterator
 	*  @throws GraphException
 	*/
	public Iterator<Node> solve() throws GraphException
	{
		Stack<Node> nodePath = new Stack<Node>();
		nodePath.push(entrance);
		nodePath = DFS(nodePath);
		
		if(nodePath == null)
			return null;
		else
			return nodePath.iterator();
	}
	
	/**
 	*  Depth first search. Recursively searches each node in a graph to find an exit
 	*  @return Path of nodes to exit represented by a stack
 	*  @throws GraphException
 	*/
	public Stack<Node> DFS(Stack<Node> nodePath) throws GraphException
	{
		// get the current node based off the top of stack (peek)
		Node currNode = nodePath.peek();
		
		// mark the current node
		currNode.setMark(true);
		
		// determine if this node has edges to potentially explore
		ArrayList<Edge> pathOptions = graph.incidentEdges(currNode);
		
		// currNode is exit, done search	
		if(currNode.equals(exit))
			return nodePath;
		// backtrack conditions
		else if(!hasOptions(currNode, pathOptions))
			return backtrack(nodePath);
		
		// recursive part, branch off into each adjacent node
		for(int x = pathOptions.size() - 1; x >= 0 && !nodePath.peek().equals(exit); x--)
		{
			// skip the node if we've visited and marked it
			if(getAdjacentNode(pathOptions.get(x), currNode).getMark() == true)
				continue;
			
			// check if edge is special type, and we have supplies to explore
			if((pathOptions.get(x).type == 2) && (numBlastBomb >= 1))
			{
				numBlastBomb--;
				nodePath.push(getAdjacentNode(pathOptions.get(x), currNode));
				nodePath = DFS(nodePath);
			}
			else if((pathOptions.get(x).type == 3) && (numBlastBomb >= 2))
			{
				numBlastBomb-=2;
				nodePath.push(getAdjacentNode(pathOptions.get(x), currNode));
				nodePath = DFS(nodePath);
			}
			else if((pathOptions.get(x).type == 4) && (numMeltBomb >= 1))
			{
				numMeltBomb--;
				nodePath.push(getAdjacentNode(pathOptions.get(x), currNode));
				nodePath = DFS(nodePath);
			}
			else if(pathOptions.get(x).type == 1)
			{
				nodePath.push(getAdjacentNode(pathOptions.get(x), currNode));
				nodePath = DFS(nodePath);
			}
		}
		
		// check if we are done
		if(nodePath.peek().equals(exit))
			return nodePath;
		// ran out of edges, and still on entrance. No solution so rtn null
		else if(currNode.equals(entrance))
			return null;
		// case where loop returned after searching the last possible node, backtrack 
		else
			return backtrack(nodePath);
		
	}
	
	/**
 	*  Determines whether a move can be made off a given node, or it is a dead end
 	*  @param node	Node to check in graph
 	*  @param pathOptions	Incident edges from the given node
 	*  @return True if there are valid move(s) to be made
 	*/
	private boolean hasOptions(Node node, ArrayList<Edge> pathOptions)
	{
		for(int x = 0; x < pathOptions.size(); x++)
		{
			if((((pathOptions.get(x).type == 2) && (numBlastBomb >= 1)) || 
				((pathOptions.get(x).type == 3) && (numBlastBomb >= 2) ||
				((pathOptions.get(x).type == 4) && (numMeltBomb >= 1)) ||
				((pathOptions.get(x).type == 1)))) && (getAdjacentNode(pathOptions.get(x), node).getMark() == false))
			{
				return true;
			}
		}
		return false;
		
		
	}
	
	/**
 	*  Method to undo/backtrack a move. Refunds any bombs used 
 	*  @param nodePath	Current path of nodes to backtrack last move
 	*  @return a stack that has been backtracked once
 	*  @throws GraphException
 	*/
	private Stack<Node> backtrack(Stack<Node> nodePath) throws GraphException // prev node will be the currentnode at dead end
	{
		// previous node removed from the top of the stack
		Node prevNode = nodePath.pop();
		
		// unmark node we backtrack from
		prevNode.setMark(false);

		// current node is now the next in stack
		Node currNode = nodePath.peek();
		
		// determine the edge we backtracked over
		Edge lastEdge = graph.getEdge(currNode, prevNode);
		
		if(lastEdge.type == 2)
			numBlastBomb++;
		else if(lastEdge.type == 3)
			numBlastBomb += 2;
		else if(lastEdge.type == 4)
			numMeltBomb++;
		return nodePath;
	}
	
	/**
 	*  Gets the adjacent node in an edge
 	*  @param edge	Connection between two nodes
 	*  @param node	One end point node of edge
 	*  @return the node adjacent to the given node
 	*/
	private Node getAdjacentNode(Edge edge, Node node)
	{
		if(edge.firstEndpoint().equals(node))
			return edge.secondEndpoint();
		else 
			return edge.firstEndpoint();
	}
}
