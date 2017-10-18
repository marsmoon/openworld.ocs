protected func Initialize()
{
    CreateObject(Rule_TeamAccount);
    CreateObject(Rule_BuyAtFlagpole);
    var rule = GetRelaunchRule();
    rule->SetFreeCrew(true);

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
