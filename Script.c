protected func Initialize()
{
    CreateObject(Rule_TeamAccount);
    CreateObject(Rule_BuyAtFlagpole);
    var rule = GetRelaunchRule();
    rule->SetFreeCrew(true);
    rule->SetBaseRespawn(true);
    rule->SetLastClonkRespawn(true);
    rule->SetInitialRelaunch(false);
    rule->SetAllowPlayerRestart(true);

    GUI_Controller->ShowWealth();

    var goal = CreateObject(Goal_ResourceExtraction);
    goal->SetResource("Gold");

    var goal2 = CreateObject(Goal_ResourceExtraction);
    goal2->SetResource("Ruby");

    InitEnvironment();

    return;
}

private func InitEnvironment()
{

    SetSkyParallax(0, 20, 20);

    // Set time of day to evening and create some clouds and celestials.
    Cloud->Place(15);
    Cloud->SetPrecipitation("Water", 100);
    Cloud->SetLightning(10);

    Grass->Place(LandscapeWidth() / 16);

    var time = CreateObject(Time);
    time->SetTime(60 * 12);
    time->SetCycleSpeed(20);

    // Sky
    SetSkyParallax(1, 20, 20, 0,0, nil, nil);

    // Some natural disasters.
    Earthquake->SetChance((SCENPAR_Difficulty - 1) * 20);
    Meteor->SetChance((SCENPAR_Difficulty - 1) * 20);
    
    // Spawn animals
	var x;
	var numberPlayers = GetPlayerCount();
	if (numberPlayers == 0)
		numberPlayers = 1;
		
	var mapHalfWidth = LandscapeWidth() / 2;
	
	
	// neutral
	var numWipfs = SCENPAR_MapSize * RandomX(2, 6);
	for (var i = 0; i < numWipfs; i++)
	{
		x = mapHalfWidth + RandomX(-mapHalfWidth, mapHalfWidth);
		CreateObject(Wipf, x, FindHeight(x) - 50);	
	}
	
	var numButterflies = SCENPAR_MapSize * 3;
	for (var i = 0; i < numButterflies; i++)
	{
		x = mapHalfWidth + RandomX(-mapHalfWidth, mapHalfWidth);
		CreateObject(Butterfly, x, FindHeight(x) - 50);	
	}
		
	var numFireflies = SCENPAR_MapSize * 5;
	for (var i = 0; i < numFireflies; i++)
	{
		x = mapHalfWidth + RandomX(-mapHalfWidth, mapHalfWidth);
		CreateObject(Firefly, x, FindHeight(x) - 50);	
	}
	
	
	// enemies
	var numZaphives = SCENPAR_MapSize;
	for (var i = 0; i < numZaphives; i++)
	{
		x = mapHalfWidth + RandomX(-mapHalfWidth, mapHalfWidth);
		CreateObject(Zaphive, x, FindHeight(x) - 50);	
	}
	
	var numPukas = SCENPAR_MapSize * 2 * numberPlayers;
	for (var i = 0; i < numPukas; i++)
	{
	
		x = mapHalfWidth + RandomX(-mapHalfWidth, mapHalfWidth);
		CreateObject(Puka, x, FindHeight(x) - 50);	
	}
	
	var numChippies = SCENPAR_MapSize * RandomX(3, 6);
	for (var i = 0; i < numChippies; i++)
	{
		x = mapHalfWidth + RandomX(-mapHalfWidth, mapHalfWidth);
		CreateObject(Chippie, x, FindHeight(x) - 50);	
	}
	
	var numMooqs = SCENPAR_MapSize * 2 * numberPlayers;
	for (var i = 0; i < numMooqs; i++)
	{
		x = mapHalfWidth + RandomX(-mapHalfWidth, mapHalfWidth);
		CreateObject(Mooq, x, FindHeight(x) - 50);
	}
	
	
	// spawn bandits
	var numBandits;
	if (SCENPAR_MapSize == 1)
	{
		if (numberPlayers > 1)
			numBandits = 1 + (numberPlayers / 2);
		else
			numBandits = 2;
			
	} else
	{
		numBandits = SCENPAR_MapSize + numberPlayers;
	}
	
	// all bandits in one place
	x = mapHalfWidth + RandomX(-mapHalfWidth, mapHalfWidth);
	for (var i = 0; i < numBandits; i++)
	{
		var bandit = CreateObjectAbove(Clonk, x, FindHeight(x) - 50);
		bandit.StaticSaveVar = "bandit";
		bandit->SetDir(DIR_Left);
		bandit->SetName("Bandit");
		bandit->SetAlternativeSkin("Guard");
	
		AI->AddAI(bandit);
		AI->SetHome(bandit);
		AI->SetGuardRange(bandit, bandit->GetX()-100, bandit->GetY()-100, 300, 110);
		
		// random weapons
		var enemyType = Random(2);
		if (enemyType == 0)
		{
			bandit->CreateContents(Sword);
			
		} else if (enemyType == 1)
		{
			bandit->CreateContents(Bow);
			bandit->CreateContents(Arrow, 3);	
		}
			
		bandit->DoEnergy(100 * SCENPAR_Difficulty);
		bandit->AddEnergyBar();
		x += 10;
	}
	
    return;
}

protected func InitializePlayer(int plr)
{
    GivePlayerAllKnowledge(plr);

    var index = 0, crew;
    var x = LandscapeWidth() / 2 + plr * 32;
    Log(Format("X: %d", x));

    var base = CreateObject(Flagpole);
    base->SetPosition(x, FindHeight(x) - 30);
    base->SetOwner(plr);

    SetBaseMaterial(plr, Wood, 100);
    SetBaseProduction(plr, Wood, 10);
    SetBaseMaterial(plr, Metal, 100);
    SetBaseProduction(plr, Metal, 10);
    SetBaseMaterial(plr, Cloth, 10);
    SetBaseProduction(plr, Cloth, 5);
    SetBaseMaterial(plr, Clonk, 10);
    SetBaseProduction(plr, Clonk, 5);
    SetBaseMaterial(plr, Bread, 10);
    SetBaseProduction(plr, Bread, 5);
    SetBaseMaterial(plr, Seeds, 10);
    SetBaseProduction(plr, Seeds, 5);

    while (crew = GetCrew(plr, index))
    {
        if (index == 0) {
            crew->CreateContents(Hammer);
            crew->CreateContents(Axe);
        }
        crew->SetPosition(x, FindHeight(x) - 50);
        index++;
    }
}

func OnClonkCreation(crew)
{
    crew->CreateContents(Shovel);
    crew->SetMaxContentsCount(9);
}

// Gives the player specific knowledge as given in the knowledge array.
global func GivePlayerSpecificKnowledge(int plr, array knowledge)
{
    for (var plan in knowledge)
        SetPlrKnowledge(plr, plan);
    return;
}

// Gives the player specific knowledge as given in the knowledge array.
global func RemovePlayerSpecificKnowledge(int plr, array knowledge)
{
    for (var plan in knowledge)
        SetPlrKnowledge(plr, plan, true);
    return;
}

// Gives the player plans according to basic knowledge.
global func GivePlayerAllKnowledge(int plr)
{
    var knowledge = [
        // Basic structures for a settlement and production of tools and explosives.
        Flagpole, Basement, WindGenerator, SteamEngine, Compensator, Sawmill, Foundry, Elevator, ToolsWorkshop, ChemicalLab, Chest, WoodenBridge, WoodenCabin, StoneDoor,
        // Basic tools for mining and tree chopping and loam production.
        Shovel, Hammer, Axe, Pickaxe, Barrel, Bucket, Torch, Lantern,
        // The basic resources.
        Metal, Loam, GoldBar,
        // Some of the basic explosives.
        Dynamite, DynamiteBox,
        // Some basic vehicles which aid in the settlement construction.
        Lorry,
        // Stuff needed for pumping.
        Pump, Pipe,
        // Structures needed to process farming materials.
        Kitchen, Loom,
        // Basic tools for farming.
        Sickle,
        // Processed goods.
        Cloth, Flour, Bread,
        // Armory to construct the weapons.
        Armory,
        // Weapons and explosives.
        Bow, Arrow, FireArrow, BombArrow, Club, Sword, Javelin, Shield, Blunderbuss, LeadBullet, IronBomb, GrenadeLauncher, PowderKeg, Helmet, SmokeBomb,
        // Artillery vehicles.
        Catapult, Cannon,
        // Stuff to set up artillery.
        Armory, PowderKeg, Catapult, Cannon,
        // Inventors lab to construct the items.
        InventorsLab, Loom,
        // Advanced items in tools workshop and needed materials.
        Ropeladder, MetalBarrel, PowderKeg, WallKit, Cloth, DivingHelmet,
        // Advanced items in inventors lab.
        TeleGlove, WindBag, GrappleBow, Boompack, Balloon,
        // Shipyard to construct the vehicles.
        Shipyard, Loom,
        // Materials needed.
        Cloth,
        // Airship and plane.
        Airship, Airplane,
        ];

    for (var plan in knowledge)
        SetPlrKnowledge(plr, plan);
    return;
}
