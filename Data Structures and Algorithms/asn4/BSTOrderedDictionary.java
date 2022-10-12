import java.util.ArrayList;

public class BSTOrderedDictionary implements BSTOrderedDictionaryADT{
	
	/**
 	*  Root of tree
 	*/
	private BSTNode root;
	
	/**
 	*  Number of internal nodes containing data
 	*/
	private int numInternalNodes;
	
	/**
 	*  Constructor for BSTOrderedDictionary class. Initialize member variables to default.
 	*/
	public BSTOrderedDictionary()
	{
		BSTNode root = new BSTNode();
		numInternalNodes = 0;
	}

	/**
 	*  Getter method for root
 	*  @return Root of tree
 	*/
	public BSTNode getRoot() 
	{
		return root;
	}

	/**
 	*  Getter method for number of internal nodes
 	*  @return Number of internal nodes
 	*/
	public int getNumInternalNodes() 
	{
		return numInternalNodes;
	}

	/**
 	*  Getter method for ArrayList of node with key
 	*  @return ArrayList object of node with key
 	*/
	public ArrayList<MultimediaItem> get(BSTNode r, String key) 
	{
		BSTNode node = nodeSearch(r, key);

		if(node == null) 
			return null;
		else 
			return node.getData().getMedia();
	}

	/**
 	*  Removes node containing key
 	*  @param r	Root of tree
 	*  @param key	Key contained in node to delete
 	*  @throws DictionaryException
 	*/
	public void remove(BSTNode r, String key) throws DictionaryException 
	{
		// node to remove
		BSTNode nodeForRemoval = nodeSearch(r, key);
		
		if(nodeForRemoval == null)
			throw new DictionaryException("No record in the ordered dictionary has key  " + key + ".\n");
		
		if(nodeForRemoval != null) // assume the node exists
		{
			BSTNode parent = nodeForRemoval.getParent();
			// case where one child node is null
			if((nodeForRemoval.getLeftChild() == null || nodeForRemoval.getRightChild() == null) && !(nodeForRemoval.isLeaf()))
			{
				BSTNode replacementNode = nodeForRemoval.getLeftChild() == null ? nodeForRemoval.getRightChild() : nodeForRemoval.getLeftChild();
				
				// node for removal is left child
				if(parent.getLeftChild().equals(nodeForRemoval))
				{
					parent.setLeftChild(replacementNode);
					replacementNode.setParent(parent);
					numInternalNodes--;
				}
				// node for removal is a right child
				else
				{
					parent.setRightChild(replacementNode);
					replacementNode.setParent(parent);
					numInternalNodes--;
				}
		
			}
			// both children are null
			else if(nodeForRemoval.isLeaf())
			{
				// node for removal is left child
				if(!(parent.getLeftChild() == null) && parent.getLeftChild().equals(nodeForRemoval))
				{
					parent.setLeftChild(null);
					numInternalNodes--;
				}
				// node for removal is a right child
				else
				{
					parent.setRightChild(null);
					numInternalNodes--;
				}
			}
			// case where both children are valid nodes
			else
			{
				NodeData s = smallest(nodeForRemoval.getRightChild());
				remove(r, s.getName());
				nodeForRemoval.setData(s);
			}
			
		}
	}
	/**
 	*  Removes all MultimediaItem objects of type from node's ArrayList with key. Removes node if empty.
 	*  @param r	Root of tree
 	*  @param key	Key contained in node
 	*  @param type	Type of MultimediaItem to remove
 	*  @throws DictionaryException
 	*/
	public void remove(BSTNode r, String key, int type) throws DictionaryException 
	{
		// node with key
		BSTNode nodeForRemoval = nodeSearch(r, key);
		if(nodeForRemoval == null)
			throw new DictionaryException("No record in the ordered dictionary has key " + key + ".\n");
		
		// check every item in the list
		for(int x = 0; x < nodeForRemoval.getData().getMedia().size(); x++)
		{
			if(nodeForRemoval.getData().getMedia().get(x).getType() == type)
			{
				nodeForRemoval.getData().getMedia().remove(x);
				// must account for indexes shifting left
				x--; 
			}
		}
		//after loop, check if the arraylist is empty. If it is empty, remove the node
		if(nodeForRemoval.getData().getMedia().isEmpty())
			remove(r, key);
		
	}

	/**
 	*  Determines successor of node in a tree, or the closest thing to a successor
 	*  @param r	Root of tree
 	*  @param key	Key contained in node
 	*/
	public NodeData successor(BSTNode r, String key) 
	{
		BSTNode myNode = nodeSearch(r, key);
		if(r.isLeaf() || (myNode != null && myNode.getData().equals(largest(r))))
			return null;
		else
		{
			// find successor of this node
			if(myNode == null)
			{
				myNode = closestSearch(r, key);
				if(myNode.getData().getName().compareTo(key) > 0)
					return myNode.getData();
			}
			if(myNode.getRightChild() != null)
				return smallest(myNode.getRightChild());
			else
			{
				BSTNode parent = myNode.getParent();
				while((parent != root) && (myNode == parent.getRightChild()))
				{
					myNode = parent;
					parent = parent.getParent();
				}
				if(myNode == root)
				{
					return myNode.getData();
				}
				else
				{
					return parent.getData();
				}
			}
				
		}
		
		
	}

	/**
 	*  Determines predecessor of node in a tree, or the closest thing to a predecessor
 	*  @param r	Root of tree
 	*  @param key	Key contained in node
 	*/
	public NodeData predecessor(BSTNode r, String key) 
	{
		BSTNode myNode = nodeSearch(r, key);
		
		if(r.isLeaf() || (myNode != null && myNode.getData().equals(smallest(r))))
			return null;
		else
		{
			// find predecessor of this node
			if(myNode == null)
			{
				myNode = closestSearch(r, key);
				// if its the closest smallest value less than key, return it
				if(myNode.getData().getName().compareTo(key) < 0)
					return myNode.getData();
			}
			
			if(myNode.getLeftChild() != null)
				return largest(myNode.getLeftChild());
			else
			{
				BSTNode parent = myNode.getParent();
				while((parent != root) && (myNode == parent.getLeftChild()))
				{
					myNode = parent;
					parent = parent.getParent();
				}
				
				if(myNode == root)
					return myNode.getData();
				
				else
					return parent.getData();
			}
				
		}
	}

	/**
 	*  Determines the smallest node in a tree
 	*  @param r	Root of tree
 	*/
	public NodeData smallest(BSTNode r) 
	{
		if(r.isLeaf())
			return r.getData();
		
		if(r.getLeftChild() != null)
			return smallest(r.getLeftChild());
		else
			return r.getData();
	}

	/**
 	*  Determines the largest node in a tree
 	*  @param r	Root of tree
 	*/
	public NodeData largest(BSTNode r) 
	{
		if(r.isLeaf())
			return r.getData();
		
		if(r.getRightChild() != null)
			return largest(r.getRightChild());
		else
			return r.getData();
	}
	
	/**
 	*  Searches for a node with key in tree
 	*  @param r	Root of tree
 	*  @param key	Key contained in node
 	*/
	private BSTNode nodeSearch(BSTNode r, String key)
	{
		if(r == null)
			return null;
		else if(r.getData().getName().compareTo(key) == 0)
			return r;
		else if(r.getData().getName().compareTo(key) > 0)
			return nodeSearch(r.getLeftChild(), key);
		else //(r.getData().getName().(key) > 0)
			return nodeSearch(r.getRightChild(), key);
	}
	
	/**
 	*  Closest node to a node with key that is not in the tree
 	*  @param r	Root of tree
 	*  @param key	Key contained in node
 	*/
	private BSTNode closestSearch(BSTNode r, String key)
	{
		if(r.getData().getName().compareTo(key) == 0)
			return r;
		else if(r.getData().getName().compareTo(key) > 0)
		{
			if(r.getLeftChild() != null)
				return closestSearch(r.getLeftChild(), key);
			else
				return r;
		}
		else //(r.getData().getName().(key) > 0)
		{
			if(r.getRightChild() != null)
				return closestSearch(r.getRightChild(), key);
			else
				return r;
		}

	}
	
	/**
 	*  Puts new node in the tree
 	*  @param r	Root of tree
 	*  @param name	Key/name contained in node
 	*  @param content	Content of the MultimediaItem
 	*  @param type	Type of MultimediaItem
 	*/
	public void put(BSTNode r, String name, String content, int type)
	{
		// prepare the potential new node 
		BSTNode newNode = new BSTNode();
		NodeData data = new NodeData(name);
		data.add(new MultimediaItem(content, type));
		newNode.setData(data);
		
		// case where you put node on an empty tree
		if(r == null) 
		{
			root = newNode;
			numInternalNodes++;
		}
		
		// case where the node exists
		else if(r.getData().getName().compareTo(name) == 0) 
			r.getData().add(new MultimediaItem(content, type));
		
		// case where r is greater than name, go left
		else if(r.getData().getName().compareTo(name) > 0) 
		{
			// found position to add
			if(r.getLeftChild() == null) 
			{
				r.setLeftChild(newNode);
				newNode.setParent(r);
				numInternalNodes++;
			}
			
			// left child not null, investigate further
			else 
				put(r.getLeftChild(), name, content, type);
			
		}
		// only remaining possibility is go right
		else 
		{
			// found position to add
			if(r.getRightChild() == null) 
			{
				r.setRightChild(newNode);
				newNode.setParent(r);
				numInternalNodes++;
			}
			
			// right child not null, investigate further
			else 
				put(r.getRightChild(), name, content, type);
		}
		
		
	}
}










































