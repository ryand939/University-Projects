
public class DictionaryException extends RuntimeException{


	public DictionaryException(String key)
	{
		super("No item in table with key " + key + ".");
	}
	
	public DictionaryException(Layout layout)
	{
		super("This Layout object already exists in the table.");
	}
}
