

public class BSTNode {

	/**
 	*  Parent of this node
 	*/
	private BSTNode parent;
	
	/**
 	*  Left child of this node
 	*/
	private BSTNode leftChild;
	
	/**
 	*  Right child of this node
 	*/
	private BSTNode rightChild;
	
	/**
 	*  Data contained in node
 	*/
	private NodeData data;
	
	/**
 	*  Constructor for BSTNode class. Initializes all member variables null
 	*/
	public BSTNode()
	{
		parent = null;
		leftChild = null;
		rightChild = null;
		data = null;
	}
	
	/**
 	*  Constructor for BSTNode class. Initializes all member variables to given values
 	*  @param newParent	Parent of new node
 	*  @param newLeftChild	Left child of new node
 	*  @param newRightChild	Right child of new node
 	*  @param newData	Data contained in new node
 	*/
	public BSTNode(BSTNode newParent, BSTNode newLeftChild, BSTNode newRightChild, NodeData newData)
	{
		parent = newParent;
		leftChild = newLeftChild;
		rightChild = newRightChild;
		data = newData;
	}

	/**
 	*  Getter method for a node's parent
 	*  @return Parent of node
 	*/
	public BSTNode getParent()
	{
		return parent;
	}
	
	/**
 	*  Getter method for a node's left child
 	*  @return Left child of node
 	*/
	public BSTNode getLeftChild()
	{
		return leftChild;
	}
	
	/**
 	*  Getter method for a node's right child
 	*  @return Right child of node
 	*/
	public BSTNode getRightChild()
	{
		return rightChild;
	}
	
	/**
 	*  Getter method for a node's data
 	*  @return Data stored in node
 	*/
	public NodeData getData()
	{
		return data;
	}
	
	/**
 	*  Setter method for parent of node
 	*  @param newParent New parent of node
 	*/
	public void setParent(BSTNode newParent)
	{
		parent = newParent;
	}
	
	/**
 	*  Setter method for left child of node
 	*  @param newLeftChild New left child of node
 	*/
	public void setLeftChild(BSTNode newLeftChild)
	{
		leftChild = newLeftChild;
	}
	
	/**
 	*  Setter method for right child of node
 	*  @param newRightChild New right child of node
 	*/
	public void setRightChild(BSTNode newRightChild)
	{
		rightChild = newRightChild;
	}
	
	/**
 	*  Setter method for data of node
 	*  @param newData	New data to store in node
 	*/
	public void setData(NodeData newData)
	{
		data = newData;
	}
	
	/**
 	*  Determines if a given node is a leaf
 	*  @param True if the node is a leaf, false otherwise
 	*/
	public boolean isLeaf()
	{
		return leftChild == null && rightChild == null;
	}
}






























