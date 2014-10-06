void list::insertInOrder(char insertChar)
{
	node * tempCursor = head;
	node * previous;
	if(isEmpty()) 
		//just insert if the list is empty
	{
		head = new node(insertChar, null);
	}
	else if(insertChar < tempCursor->data) 
		//if the character is less than the first insert it before.
	{
			head = new node(insertChar, head->next);
	}
	else 
		//otherwise, traverse until it is less 
		//than a cursor and point it to that cursor
	{
		while(tempCursor != NULL && insertChar > tempCursor->data)
		{
			previous = tempCursor;
			tempCursor = tempCursor->next;
		}
		previous->next = new node(insertChar, previous->next);
	}
}

void list::sort()
{
	list newList;
	while(!isEmpty())
	{
		list.insertInOrder(remove(head));
	}
	head = list.head;
}

