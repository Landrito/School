putOnQ();
while( !solved && !Q.empty() )
{
	GetBoardOffQ();
	string2Board();
	if( canMoveForward() )
	{
		if( !haveWeBeenHere() )
		{
			InsertToQueue();
			InsertToCheckedBefore();
		}
	}
}