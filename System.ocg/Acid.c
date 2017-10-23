#appendto Acid

public func IsFoundryProduct() { return true; }
public func GetProductionTime() { return 120; }

public func Construction(object creator)
{
	var res = _inherited(creator, ...);
	// If acid is created by the foundry we can safely assume it has been produced
	// and set the stack count to 100. The only exceptions are CreateContents script calls.
	if (creator && creator->~IsProducer())
		SetStackCount(200);
	return res;
}


local Components = {Oil = 100};
