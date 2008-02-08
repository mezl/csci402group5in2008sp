//Base clerk function
Clerk::Clerk(Line *l,Table *t)
{
	line = l;
	table = t;//share with manager
	
}
Clerk::~Clerk(){
}

void Clerk::run()
{
	while(1)
	{
		//Do Lock
		line->Acquire();
		if(!line->IsPreferLineEmpty()) //if there is customer in prefer line
		{
			Customer *c = line->getNextPreferLineCustomer();
			handleCustomer(c);
		}
		else if(!line->IsRegLineEmpty())
		{
			Customer *c = line->getNextRegLineCustomer();
			handleCustomer(c);
		}else{
			//Go Sleep
			line->Release();			
			table->leave();
		}	
		line->Release();
	}
}
#endif


