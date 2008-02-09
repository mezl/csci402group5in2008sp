// manager.cc
// manager function should be with main() or office()
// manager funciton can't take any parameters

void Manager()
{
	// check each table for number of clerks
	// add 1 clerk if table is empty

	applicationTable.acquireLock();
	if (applicationTable.clerkCount() == 0))
	{
		applicationTable.addClerk();
	}
	applicationTable.releaseLock();

	pictureTable.acquireLock();
	if (pictureTable.clerkCount() == 0)
	{
		pictureTable.addClerk();
	}
	pictureTable.releaseLock();

	passportTable.acquireLock();
	if (passportTable.clerkCount() == 0)
	{
		passportTable.addClerk();
	}
	passportTable.releaseLock();
	
	cashierTable.acquireLock();
	if (cashierTable.clerkCount() == 0)
	{
		cashierTable.addClerk();
	}
	cashierTable.releaseLock();


	// check for number of customers in each line
	// add 1 clerk if # of customer in line is >3
	// must acquire the lock for both line and table before adding a clerk or checkin customer
	// must release all locks at completion.

	applicationline.regAcquire();
	applicationLine.preferAcquire();
	applicationTable.acquireLock();
	if (applicationLine.customerCount() > 3)
	{
		applicationTable.addClerk();
	}
	applicationLine.regRelease();
	applicationLine.preferRelease();
	applicaitonTable.releaseLock();

	pictureline.regAcquire();
	pictureLine.preferAcquire();
	pictureTable.acquireLock();
	if (pictureLine.customerCount() > 3)
	{
		pictureTable.addClerk();
	}
	pictureLine.regRelease();
	pictureLine.preferRelease();
	pictureTable.releaseLock();

	passportline.regAcquire();
	passportLine.preferAcquire();
	passportTable.acquireLock();
	if (passportLine.customerCount() > 3)
	{
		passportTable.addClerk();
	}
	passportLine.regRelease();
	passportLine.preferRelease();
	passportTable.releaseLock();

	cashierline.regAcquire();
	cashierLine.preferAcquire();
	cashierTable.acquireLock();
	if (cashierLine.customerCount() > 3)
	{
		cashierTable.addClerk();
	}
	cashierLine.regRelease();
	cashierLine.preferRelease();
	cashierTable.releaseLock();
}
