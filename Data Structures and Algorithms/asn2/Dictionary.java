
public class Dictionary implements DictionaryADT{

	/**
 	*  Hash table to be represented by an array of layout nodes
 	*/
	private LayoutNode[] hashTable; 
	
	/**
 	*  Constructor for dictionary class, initializes all indexes of the hash table to null
 	*  @param size	Size of the hash table. Must be prime < 10,000
 	*/
	public Dictionary(int size)
	{
		hashTable = new LayoutNode[size];
		
		// ensure the empty indices are assigned null
		for(int x = 0; x < hashTable.length; x++)
			hashTable[x] = null;
	}

	/**
 	*  Puts a layout object into the hash table
 	*  @param data	Layout object to put in the hash table
 	*  @throws DictionaryException
 	*  @return 0 if the layout object was inserted without a collision, 1 if the layout object was inserted with a collision
 	*/
	public int put(Layout data) throws DictionaryException 
	{
		// create node for the layout data
		LayoutNode dataNode = new LayoutNode(data);
		
		// determine position for data item in the hash table
		int pos = hashFunction(data.getBoardLayout(), 35, hashTable.length);
		
		// no collision, simply assign
		if(hashTable[pos] == null)
		{
			hashTable[pos] = dataNode;
			return 0;
		}
		// collision, link dataNode to tail node, but check each existing node on the way
		else
		{
			LayoutNode currentNode = hashTable[pos];
			if(currentNode.getElement().getBoardLayout().equals(data.getBoardLayout()))
				throw new DictionaryException(data.getBoardLayout());
			
			
			while(currentNode.getNext() != null)
			{
				currentNode = currentNode.getNext();
				if(currentNode.getElement().getBoardLayout().equals(data.getBoardLayout()))
					throw new DictionaryException(data.getBoardLayout());
			}

			// next node must not be null, link dataNode to the tail node of this list (currentNode)
			currentNode.setNext(dataNode);
			return 1;
		}
		
		
	}

	/**
 	*  Removes a layout object from the hash table if it exists
 	*  @param boardLayout	String representation of board layout used as hash table key
 	*  @throws DictionaryException
 	*/
	public void remove(String boardLayout) throws DictionaryException 
	{
		int pos = hashFunction(boardLayout, 35, hashTable.length);
		LayoutNode currentNode = hashTable[pos];
		
		// if the element does not exist
		if(currentNode == null)
			throw new DictionaryException(boardLayout);
		// if the desired element is in first position
		else if (currentNode.getElement().getBoardLayout().equals(boardLayout))
		{
			hashTable[pos] = currentNode.getNext();
			return;
		}
		
		// otherwise, find the position of the node BEFORE the desired node
		while((currentNode.getNext() != null) && !(currentNode.getNext().getElement().getBoardLayout().equals(boardLayout)))
			currentNode = currentNode.getNext();
	
		// loop breaks when either we are in last position, or getnext is the desired node
		if(currentNode.getNext() == null) 
			throw new DictionaryException(boardLayout);
		else 
			currentNode.setNext(currentNode.getNext().getNext());
	
	}

	/**
 	*  Retrieves a score from a Layout object given it's key/boardLayout string
 	*  @param boardLayout	String representation of the board that operates as a key
 	*  @return Score associated to boardLayout, -1 if the boardLayout does not exist in the table
 	*/
	public int getScore(String boardLayout) 
	{
		int pos = hashFunction(boardLayout, 35, hashTable.length);
		LayoutNode currentNode = hashTable[pos];
		
		while((currentNode != null) && !(currentNode.getElement().getBoardLayout().equals(boardLayout)))
			currentNode = currentNode.getNext();
		
		if(currentNode == null)
			return -1;
		else
			return currentNode.getElement().getScore();
	}
	
	/**
 	*  Determines index of hash table based on hash function
 	*  @param key	Key for hash table index
 	*  @param x 	Constant multiplier
 	*  @param sizeTable	Size of the hash table
 	*  @return index	
 	*/
	private int hashFunction(String key, int x, int sizeTable)
	{
		int index = (int)key.charAt(0);
		
		for(int i = 1; i < key.length(); i++)
			index = ((index * x + (int)key.charAt(i)) % sizeTable); 
		
		return index;
	}

	
}
