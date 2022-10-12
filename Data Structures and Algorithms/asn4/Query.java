

import java.io.*;
import java.util.ArrayList;

public class Query {

public static void main(String[] args)
{
	// string used to hold user input
	String userInput;
	
	// string for reading new keys in a text document
	String newKey;

	// type of content determined by input
	int type;
	
	// init dictionary
	BSTOrderedDictionary dict = new BSTOrderedDictionary();
	
	// init tree root
	BSTNode root = dict.getRoot();
	
	// init sound player, picture viewer, and HTML viewer for program
	SoundPlayer sound = new SoundPlayer();
	PictureViewer pict = new PictureViewer();
	ShowHTML showWeb = new ShowHTML();
	try
	{
		// prepare bufferedReader for reading input file line by line
		FileInputStream fileInStream = new FileInputStream(args[0]);
		DataInputStream dataInStream = new DataInputStream(fileInStream);
		BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(dataInStream));
		// create all nodes and put them all on root
		// newKey will be assigned a new key for every loop
		while ((newKey = bufferedReader.readLine()) != null)   
		{
			// line or content related to key
			String newLine = bufferedReader.readLine();
			  
			// determine TYPE
			if(newLine.contains(".wav") || newLine.contains(".mid"))
				type = 2;
			else if(newLine.contains(".jpg") || newLine.contains(".gif"))
				type = 3;
			else if(newLine.contains(".html"))
				type = 4;
			else
				type = 1;
			dict.put(root, newKey.toLowerCase(), newLine, type);
			root = dict.getRoot();
		}
		dataInStream.close();    
	}
	catch (Exception e)
	{
		System.err.println("Error: " + e.getMessage());
	}
	
	// prepare string reader for user input
	StringReader keyboard = new StringReader();
	userInput = keyboard.read("Enter next command: ");

	while(!userInput.equals("end"))
	{
		try 
		{
			// determine what user wants base on if their command begins with keywords
			if(userInput.startsWith("get "))
			{
				String getWhat = userInput.substring(4);
				// determine if the dict has a node with key getWhat
				ArrayList<MultimediaItem> myList = dict.get(root, getWhat);
				if(myList == null)
				{
					System.out.println("The word " + getWhat + " is not in the ordered dictionary");
					String preceding = dict.predecessor(root, getWhat).getName().compareTo(getWhat) < 0 ? dict.predecessor(root, getWhat).getName() : ""; 
					String following = dict.successor(root, getWhat).getName().compareTo(getWhat) > 0 ? dict.successor(root, getWhat).getName() : ""; 
					System.out.println("Preceding word: " + preceding);
					System.out.println("Following word: " + following);
				}
				else 
				{

					for(int x = 0; x < myList.size(); x++)
					{
						if(myList.get(x).getType() == 1) // type text
						{
							System.out.println(myList.get(x).getContent());
						}
						else if(myList.get(x).getType() == 2) // type audio file
						{
							sound.play(myList.get(x).getContent());
						}
						else if(myList.get(x).getType() == 3) // type image
						{
							pict.show(myList.get(x).getContent());
						}
						else // type web page
						{
							showWeb.show(myList.get(x).getContent());
						}
					}	
				}

			}
			else if(userInput.startsWith("remove ")) // remove a node
			{
				String removeWhat = userInput.substring(7);
				dict.remove(root, removeWhat);
			}
			else if(userInput.startsWith("delete ")) // delete a node
			{
				// delete k t -> key-0, type-1
				String[] removeWhat = userInput.substring(7).split(" ");
				dict.remove(root, removeWhat[0], Integer.valueOf(removeWhat[1]));
			}
			else if(userInput.startsWith("add "))
			{
				// need to split carefully to allow for sentences
				// should split in order 0-key 1-content 2-type
				userInput = userInput.substring(4);
				String[] multipleInput = {userInput.substring(0, userInput.indexOf(" ")),
										  userInput.substring(userInput.indexOf(" ") + 1, userInput.length() - 2),
						                  Character.toString(userInput.charAt(userInput.length() - 1))};
						
				dict.put(root, multipleInput[0], multipleInput[1], Integer.valueOf(multipleInput[2]));
			}
			else if(userInput.startsWith("next "))
			{
				// find node p with attribute k or closest match
				// 0-key 1-d
				String[] multipleInput = userInput.substring(5).split(" ");
				int counter = 0;
				int upperLim = Integer.valueOf(multipleInput[1]);
				
				
				NodeData myNode = dict.successor(root, multipleInput[0]);
				
				// if the key exists, print it and increment both upper and lower range
				// counter != 0 means at least one item has been printed
				if(dict.get(root, multipleInput[0]) != null)
				{
					System.out.print(multipleInput[0] + " ");
					counter++;
					upperLim++;
				}
				
				// ensure the max loops are capped
				if(upperLim > dict.getNumInternalNodes())
					upperLim = dict.getNumInternalNodes();
				
				// while we are always printing out a larger name, and never looping past the length of tree size
				while((myNode != null) && (counter < upperLim) && (multipleInput[0].compareTo(myNode.getName()) < 0))
				{
					System.out.print(myNode.getName() + " ");
					myNode = dict.successor(root, myNode.getName());
					counter++;
				}
				
				// check if nothing was printed at all
				if(counter == 0)
					System.out.print("There are no keys larger than or equal to " + multipleInput[0]);
				System.out.println();
			}
			else if(userInput.startsWith("prev "))
			{
				// find node p with attribute k or closest match
				// 0-key 1-d
				String[] multipleInput = userInput.substring(5).split(" ");
				// find node with key k
				int counter = 0;
				int upperLim = Integer.valueOf(multipleInput[1]);

				// if the key exists, print it and increment both upper and lower range
				// counter != 0 means at least one item has been printed
				NodeData myNode = dict.predecessor(root, multipleInput[0]);
				if(dict.get(root, multipleInput[0]) != null)
				{
					System.out.print(multipleInput[0] + " ");
					counter++;
					upperLim++;
				}

				// ensure the max loops are capped
				if(upperLim > dict.getNumInternalNodes())
					upperLim = dict.getNumInternalNodes();

				// while myNode is not null, and we dont exceed the limit
				while((myNode != null) && (counter < upperLim) && (multipleInput[0].compareTo(myNode.getName()) > 0))
				{
					System.out.print(myNode.getName() + " ");
					myNode = dict.predecessor(root, myNode.getName());
					counter++;

				}
				
				// check if nothing was printed at all
				if(counter == 0)
					System.out.print("There are no keys less than or equal to " + multipleInput[0]);
				System.out.println();
			}
			else if(userInput.equalsIgnoreCase("first"))
			{
				if(root != null)
					System.out.println(dict.smallest(root).getName());
				else
					System.out.println("The ordered dictionary is empty");
			}
			else if(userInput.equalsIgnoreCase("last"))
			{
				if(root != null)
					System.out.println(dict.largest(root).getName());
				else
					System.out.println("The ordered dictionary is empty");
			}
			else if(userInput.equalsIgnoreCase("size"))
			{
				System.out.println(dict.getNumInternalNodes());
			}
			else 
			{
				System.out.println("Invalid command " + userInput);
			}
		}
		catch (Exception e) 
		{
		System.err.println("\nError: " + e.getMessage());
	    }
		
		// input for next loop, if input != "end"
		userInput = keyboard.read("Enter next command: ");
		

	
	}
	}
}

