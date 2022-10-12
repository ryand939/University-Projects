
public class LayoutNode {
	
	/**
 	*  Next node in the chain of LayoutNodes
 	*/
	private LayoutNode next;
	
	/**
 	*  Layout object belonging to this node
 	*/
	private Layout element;
	
	/**
 	*  Constructor for LayoutNode with no given Layout object
 	*/
	public LayoutNode()
	{
		next = null;
		element = null;
	}
	
	/**
 	*  Constructor for Layout node with provided Layout object
 	*  @param element	Layout object belonging to this new LayoutNode
 	*/
	public LayoutNode(Layout element)
	{
		next = null;
		this.element = element;
	}
	
	/**
 	*  Gets the next node in the LayoutNode chain
 	*  @return next	Next node
 	*/
	public LayoutNode getNext()
	{
		return next;
	}
	
	/**
 	*  Sets the next node in the LayoutNode chain
 	*  @param node	Node to link next
 	*/
	public void setNext(LayoutNode node)
	{
		next = node;
	}
	
	/**
 	*  Gets the Layout object belonging to this LayoutNode
 	*  @return element	Layout object belonging to this LayoutNode
 	*/
	public Layout getElement()
	{
		return element;
	}
	
	/**
 	*  Sets the layout object in this LayoutNode
 	*  @param element	Layout object to assign to this LayoutNode
 	*/
	public void setElement(Layout element)
	{
		this.element = element;
	}
}
