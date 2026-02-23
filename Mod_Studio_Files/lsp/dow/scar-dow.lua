---@meta scar-dow

-- Auto-generated SCAR stubs for Dawn of War by scar-to-luadefs.py
-- Sources: function_list.htm + scardoc.dat
-- 922 functions, 139 constants
-- Do not edit manually — regenerate with: python tools/scar-to-luadefs.py
--
-- Import tiers:
--   Tier 1: available after import("ScarUtil.scar")
--   Tier 2: available after import("WXPScarUtil.scar")  (includes Tier 1)
--   Standalone: requires explicit import (noted per-section)
--   Engine-only: C++ built-ins with no .scar wrapper
--
-- All mod scripts should import("WXPScarUtil.scar") to get Tier 1 + Tier 2.

--------------------------------------------------------------------------------
-- Constants
--------------------------------------------------------------------------------

---@type any
AD_Advanced = nil

---@type any
AD_Easy = nil

---@type any
AD_Hard = nil

---@type any
AD_Insane = nil

---@type any
AD_Standard = nil

---@type any
CMD_BuildSquad = nil

---@type any
CMD_CancelProduction = nil

---@type any
CMD_DefaultAction = nil

---@type any
CMD_Destroy = nil

---@type any
CMD_RallyPoint = nil

---@type any
CMD_Stop = nil

---@type any
CT_Attacking = nil

---@type any
CT_Blocking = nil

---@type any
CT_BuildAddOns = nil

---@type any
CT_BuildBuildings = nil

---@type any
CT_BuildResearch = nil

---@type any
CT_BuildUnits = nil

---@type any
CT_Equal = nil

---@type any
CT_GreaterThanEqual = nil

---@type any
CT_Heavy = nil

---@type any
CT_LessThanEqual = nil

---@type any
CT_Light = nil

---@type any
CT_Negative = nil

---@type any
CT_None = nil

---@type any
CT_Resourcing = nil

---@type any
CT_SquadCap = nil

---@type any
CT_Stealth = nil

---@type any
CT_Support = nil

---@type any
CT_Tactics = nil

---@type any
DIFFICULTY_EASY = nil

---@type any
DIFFICULTY_HARD = nil

---@type any
DIFFICULTY_NORMAL = nil

---@type any
HTL_Max = nil

---@type any
HTL_Min = nil

---@type any
HTL_None = nil

---@type any
MAT_Entity = nil

---@type any
MAT_EntityType = nil

---@type any
MAT_Player = nil

---@type any
MAT_Squad = nil

---@type any
MAT_SquadType = nil

---@type any
MAT_WeaponType = nil

---@type any
MSTANCE_Assault = nil

---@type any
MSTANCE_Ranged = nil

---@type any
MSTANCE_Versatile = nil

---@type any
MUT_Addition = nil

---@type any
MUT_Enable = nil

---@type any
MUT_Multiplication = nil

---@type any
OS_Complete = nil

---@type any
OS_Failed = nil

---@type any
OS_Incomplete = nil

---@type any
PC_Ambient = nil

---@type any
PC_Music = nil

---@type any
PQ_AddOn = nil

---@type any
PQ_AddOns = nil

---@type any
PQ_Research = nil

---@type any
PQ_Squad = nil

---@type any
PQ_Squads = nil

---@type any
PROX_CENTER = nil

---@type any
PROX_LONGEST = nil

---@type any
PROX_SHORTEST = nil

---@type any
RS_Available = nil

---@type any
RS_Complete = nil

---@type any
RS_Locked = nil

---@type any
RS_Researching = nil

---@type any
RT_Leader = nil

---@type any
RT_Pop = nil

---@type any
RT_Power = nil

---@type any
RT_Requisition = nil

---@type any
RT_Trooper = nil

---@type any
RT_WeaponUpgrade = nil

---@type any
R_ALLY = nil

---@type any
R_ENEMY = nil

---@type any
R_NEUTRAL = nil

---@type any
R_UNDEFINED = nil

---@type any
SCMD_Ability = nil

---@type any
SCMD_Attach = nil

---@type any
SCMD_Attack = nil

---@type any
SCMD_AttackGround = nil

---@type any
SCMD_AttackMove = nil

---@type any
SCMD_BuildStructure = nil

---@type any
SCMD_CancelReinforcement = nil

---@type any
SCMD_Capture = nil

---@type any
SCMD_DefaultAction = nil

---@type any
SCMD_Destroy = nil

---@type any
SCMD_Detach = nil

---@type any
SCMD_Jump = nil

---@type any
SCMD_Load = nil

---@type any
SCMD_Melee = nil

---@type any
SCMD_Move = nil

---@type any
SCMD_Rampage = nil

---@type any
SCMD_ReinforceLeader = nil

---@type any
SCMD_ReinforceTrooper = nil

---@type any
SCMD_ReinforceTrooperOverwatch = nil

---@type any
SCMD_Repair = nil

---@type any
SCMD_SetInfiltrating = nil

---@type any
SCMD_Stance = nil

---@type any
SCMD_StanceMelee = nil

---@type any
SCMD_Stop = nil

---@type any
SCMD_Unload = nil

---@type any
SCMD_UnloadHere = nil

---@type any
SCMD_UpgradeTrooper = nil

---@type any
SCMD_UpgradeTrooperOverwatch = nil

---@type any
SPT_Relic = nil

---@type any
SPT_StrategicObjective = nil

---@type any
SPT_StrategicPoint = nil

---@type any
SQUADSTATEID_Capture = nil

---@type any
SQUADSTATEID_Combat = nil

---@type any
SQUADSTATEID_CombatStance = nil

---@type any
SQUADSTATEID_Construction = nil

---@type any
SQUADSTATEID_Idle = nil

---@type any
SQUADSTATEID_Melee = nil

---@type any
SQUADSTATEID_Move = nil

---@type any
SQUADSTATEID_Patrol = nil

---@type any
STANCE_Attack = nil

---@type any
STANCE_Burn = nil

---@type any
STANCE_CeaseFire = nil

---@type any
STANCE_Hold = nil

---@type any
STANCE_StandGround = nil

---@type any
STATEID_Attack = nil

---@type any
STATEID_Dead = nil

---@type any
STATEID_Idle = nil

---@type any
STATEID_Pause = nil

---@type any
STATEID_StrategicPointCapture = nil

---@type any
STATEID_StructureBuilding = nil

---@type any
TEAM_NEUTRAL = nil

---@type any
TV_SlideTargetBase = nil

---@type any
TV_SlideTargetRate = nil

---@type any
TV_SlideTargetThreshold = nil

---@type any
UC_Commander = nil

---@type any
UC_HeavyInfantryHigh = nil

---@type any
UC_HeavyInfantryMed = nil

---@type any
UC_LightInfantryHigh = nil

---@type any
UC_LightInfantryLow = nil

---@type any
UC_LightInfantryMe = nil

---@type any
UC_MonsterHigh = nil

---@type any
UC_MonsterMed = nil

---@type any
UC_VehicleHigh = nil

---@type any
UC_VehicleLow = nil

---@type any
UC_VehicleMed = nil

--------------------------------------------------------------------------------
-- Tier 1: Available via import("ScarUtil.scar")
--------------------------------------------------------------------------------

-- from actor.scar

--- Breaks the relationship between an actor an its sgroup.
---
--- Speech played by this actor after this function call will no longer look for the actor in the sgroup to play the talk animation.  This function also removes the special sp_idle that is applied to all actors.
---
--- *Source: Actor.scar*
---@param actor ActorTable
function Actor_Clear(actor) end

--- Plays a speech event for a given actor.
---
--- **Sounds will be taken from the data:sound/speech/nis/ folder.  If you wish to play a sound from another folder you must use an optional path (ie. 'speech/single_player/guardsman/morale')
---
---  If the speech file is not available, a placeholder time will be calculated for the message using the number of words and the message will be displayed for that length of time.
---  Actors:
---      ACT.Gabriel, ACT.Isador, ACT.Matiel, ACT.Bale, ACT.Sindri, ACT.Orkamungus, ACT.Macha, ACT.Brom, ACT.Toth, ACT.ScoutMarine, ACT.TacticalMarine, ACT.SluggaBoy, ACT.Guardsmen, ACT.Ranger, ACT.Apothecary, ACT.Cultist, ACT.Nob, ACT.Grot, ACT.Corralis, ACT.Servitor, ACT.Warlock, ACT.Dark_Reaper_Exarch, ACT.EvilIsador
---
---  To play speech for a custom actor, pass in a table = { blueprint = "blueprint_name", icon = "data:icon_path.tga" } for the Actor.
---
--- *Source: Actor.scar*
---@param actor ActorTable
---@param locID integer
---@param optionalPath string
function Actor_PlaySpeech(actor, locID, optionalPath) end

--- Assign an sgroup to a actor.  Will take the first matching blueprint from the sgroup.
---
--- Actor_SetFromSGroup( ACT.Gabriel, "sg_Gabriel" )
---
--- *Source: Actor.scar*
---@param actor ActorTable
---@param sgroupname string
function Actor_SetFromSGroup(actor, sgroupname) end

-- from camera.scar

--- Focus the camera on a named marker.
---
--- This function can be called through a CTRL object in NISlets.
---
--- *Source: Camera.scar*
---@param markername string
---@param secs number
function Camera_FocusOnTargetMarker(markername, secs) end

-- from command.scar

--- Attach the squad from sgroupnameAttachee to sgroupname.  Both SGroups must contain only one squad.
---
--- *Source: Command.scar*
---@param sgroupname string
---@param sgroupnameAttachee string
function Cmd_AttachSquads(sgroupname, sgroupnameAttachee) end

--- Order a squad group to attack another squad group.  This function should track the target squad groups position.
---
--- The source squads will not stop moving until they reach the targeted squad group.
---
--- *Source: Command.scar*
---@param sourcesquadgroup string
---@param targetentitygroup string
function Cmd_AttackEGroup(sourcesquadgroup, targetentitygroup) end

--- Order a squad group to ground attack a marker position.
---
--- *Source: Command.scar*
---@param sgroupname string
---@param markername string
function Cmd_AttackGroundMarker(sgroupname, markername) end

--- Order a squad group to ground attack a position.
---
--- *Source: Command.scar*
---@param sgroupname string
---@param targetposition Position
function Cmd_AttackGroundPos(sgroupname, targetposition) end

--- Order a squad group to ground move to a marker position.
---
--- *Source: Command.scar*
---@param sgroupname string
---@param markername string
function Cmd_AttackMoveMarker(sgroupname, markername) end

--- Order a squad group to attack move to a position.
---
--- *Source: Command.scar*
---@param sgroupname string
---@param targetposition Position
function Cmd_AttackMovePos(sgroupname, targetposition) end

--- Order a squad group to attack another squad group.  This function should track the target squad groups position.
---
--- The source squads will not stop moving until they reach the targeted squad group.
---
--- *Source: Command.scar*
---@param sourcesquadgroup string
---@param targetsquadgroup string
function Cmd_AttackSGroup(sourcesquadgroup, targetsquadgroup) end

--- Command an sgroup of units to capture an egroup of strategic points
---
--- *Source: Command.scar*
---@param sgroupname string
---@param egroupname string
function Cmd_Capture(sgroupname, egroupname) end

--- Checks the egroup for an entity that can cast the ability and casts it on the target.   Returns false if no items in the group can cast.
---
--- *Source: Command.scar*
---@param sgroupname string
---@param abilityname string
---@param targetgroup string
---@return boolean
function Cmd_CastAbilityEGroupS(sgroupname, abilityname, targetgroup) end

--- Checks the sgroup for an entity that can cast the ability and casts it on the target pos.   Returns false if no items in the group can cast.
---
--- *Source: Command.scar*
---@param sgroupname string
---@param abilityname string
---@param targetpos Position
---@return boolean
function Cmd_CastAbilityMarker(sgroupname, abilityname, targetpos) end

--- Checks the sgroup for an entity that can cast the ability and casts it on the target pos.   Returns false if no items in the group can cast.
---
--- *Source: Command.scar*
---@param sgroupname string
---@param abilityname string
---@param targetpos Position
---@return boolean
function Cmd_CastAbilityPos(sgroupname, abilityname, targetpos) end

--- Checks the sgroup for an entity that can cast the ability and casts it on the target pos.   Returns false if no items in the group can cast.
---
--- *Source: Command.scar*
---@param sgroupname string
---@param abilityname string
---@param targetgroup string
---@return boolean
function Cmd_CastAbilitySGroup(sgroupname, abilityname, targetgroup) end

--- Checks the sgroup for an entity that can cast the ability and casts it on the target.   Returns false if no items in the group can cast.
---
--- *Source: Command.scar*
---@param sgroupname string
---@param abilityname string
---@param targetgroup string
---@return boolean
function Cmd_CastAbilitySGroupE(sgroupname, abilityname, targetgroup) end

--- Checks the sgroup for an entity that can cast the ability and casts on itself  Returns false if no items in the group can cast.
---
--- *Source: Command.scar*
---@param sgroupname string
---@param abilityname string
---@return boolean
function Cmd_CastAbilitySelf(sgroupname, abilityname) end

--- Command an sgroup of builders to construct a building at a marker position
---
--- *Source: Command.scar*
---@param builderSGroup string
---@param buildingBlueprint string
---@param markername string
function Cmd_ConstructBlueprintMarker(builderSGroup, buildingBlueprint, markername) end

--- Command an sgroup of builders to construct a building an enity (for listening posts etc)
---
--- *Source: Command.scar*
---@param builderSGroup string
---@param buildingBlueprint string
---@param egroupName string
function Cmd_ConstructBlueprintOnEGroup(builderSGroup, buildingBlueprint, egroupName) end

--- Command an sgroup of builders to construct a building at a position
---
--- *Source: Command.scar*
---@param builderSGroup string
---@param buildingBlueprint string
---@param pos Position
function Cmd_ConstructBlueprintPos(builderSGroup, buildingBlueprint, pos) end

--- Command all buildings in an EGroup to unload their squads
---
--- *Source: Command.scar*
---@param buildinggroup string
function Cmd_EmptyBuilding(buildinggroup) end

--- Command all transports in an SGroup to move to position and unload
---
--- *Source: Command.scar*
---@param sgroupname string
---@param pos Position
function Cmd_EmptyTransport(sgroupname, pos) end

--- Command a SGroup to enter the first building in buildinggroup
---
--- *Source: Command.scar*
---@param sgroupname string
---@param buildinggroup string
function Cmd_EnterBuilding(sgroupname, buildinggroup) end

--- Command a SGroup to enter a transport in transportgroup
---
--- *Source: Command.scar*
---@param sgroupname string
---@param transportgroup string
function Cmd_EnterTransport(sgroupname, transportgroup) end

--- Enables/Disables infiltration for an sgroup
---
--- *Source: Command.scar*
---@param sgroupName string
---@param on boolean
function Cmd_Infiltrate(sgroupName, on) end

--- Returns true if ALL or ANY squads in a group are currently capturing something.
---
--- Set all to true to check for ALL or set to false to check for ANY
---
--- *Source: Command.scar*
---@param sgroupname string
---@param all boolean
---@return boolean
function Cmd_IsCapturing(sgroupname, all) end

--- Returns true if ALL or ANY squads in a group are currently constructing a building.
---
--- Set all to true to check for ALL or set to false to check for ANY
---
--- *Source: Command.scar*
---@param sgroupname string
---@param all boolean
---@return boolean
function Cmd_IsConstructingBuilding(sgroupname, all) end

--- Returns true if ALL or ANY entities in a group are currently producing squads
---
--- Set all to true to check for ALL or set to false to check for ANY
---
--- *Source: Command.scar*
---@param egroupname string
---@param all boolean
---@return boolean
function Cmd_IsProducingSquads(egroupname, all) end

--- Returns true if ALL or ANY squads in a group are currently calling for reinforcments.
---
--- Set all to true to check for ALL or set to false to check for ANY
---
--- *Source: Command.scar*
---@param sgroupname string
---@param all boolean
---@return boolean
function Cmd_IsReinforcing(sgroupname, all) end

--- Returns true if ALL or ANY squads in a group are currently reinforcing a leader.
---
--- Set all to true to check for ALL or set to false to check for ANY.
---
--- *Source: Command.scar*
---@param sgroupname string
---@param all boolean
---@return boolean
function Cmd_IsReinforcingLeader(sgroupname, all) end

--- Returns true if ALL or ANY squads in a group are currently upgrading.
---
--- Set all to true to check for ALL or set to false to check for ANY
---
--- *Source: Command.scar*
---@param sgroupname string
---@param all boolean
---@return boolean
function Cmd_IsUpgrading(sgroupname, all) end

--- Command an sgroup to jump to a given marker position.
---
--- Note: Jump restrictions such as range etc apply.
---
--- *Source: Command.scar*
---@param sgroupname string
---@param markername string
function Cmd_JumpToMarker(sgroupname, markername) end

--- Command an sgroup to jump to a given position.  The squad will jump to the closest possible position.
---
--- Note: Jump restrictions such as range etc apply.
---
--- *Source: Command.scar*
---@param sgroupname string
---@param position Position
function Cmd_JumpToPos(sgroupname, position) end

--- Moves a squad group to the closest marker in a list of marker names.
---
--- *Source: Command.scar*
---@param sgroupName string
---@param markerNameTable table
---@return MarkerID
function Cmd_MoveToClosestMarker(sgroupName, markerNameTable) end

--- Move a squad group to the center of an entity group.
---
--- The move command will be issued by the player owner of the first item in the group.  This function does nothing if the group is empty.
---
--- *Source: Command.scar*
---@param sgroupname string
---@param egroupname string
function Cmd_MoveToEGroup(sgroupname, egroupname) end

--- Move a squad group to a given markers position.
---
--- The move command will be issued by the player owner of the first item in the group.  This function does nothing if the group is empty.
---
--- *Source: Command.scar*
---@param sgroupname string
---@param markername string
function Cmd_MoveToMarker(sgroupname, markername) end

--- Move a squad group to another squad group.  This function should track the target squad groups position.
---
--- The source squads will not stop moving until they reach the targeted squad group.
---
--- *Source: Command.scar*
---@param sourcesquadgroup string
---@param targetsquadgroup string
function Cmd_MoveToSGroup(sourcesquadgroup, targetsquadgroup) end

--- Calls possesion on a squads in a group.  Requirments for possesion must be met
---
--- *Source: Command.scar*
---@param sgroupName string
function Cmd_Possesion(sgroupName) end

--- Order a squad group to ramapge to a marker pos/
---
--- *Source: Command.scar*
---@param sgroupname string
---@param markername string
function Cmd_RampageMarker(sgroupname, markername) end

--- Order a squad group to ramapge to a position.
---
--- *Source: Command.scar*
---@param sgroupname string
---@param targetposition Position
function Cmd_RampagePos(sgroupname, targetposition) end

--- Sends a reinforce leader command to all squads in a group.  count represents the number of commands to send.
---
--- Note: To create a squad bypassing the pre-reqs and costs use Util_CreateSquadsAtPositionEx.
---
--- *Source: Command.scar*
---@param squadgroupname string
---@param count integer
function Cmd_ReinforceLeader(squadgroupname, count) end

--- Sends a reinforce command to all squads in a group.  count represents the number of commands to send.
---
--- Note: To create a squad bypassing the pre-reqs and costs use Util_CreateSquadsAtPositionEx.
---
--- *Source: Command.scar*
---@param squadgroupname string
---@param count integer
function Cmd_ReinforceTrooper(squadgroupname, count) end

--- Order a squad group set their stance.  Use MSTANCE_Assault, MSTANCE_Ranged.
---
--- The order will be issued by the owner of the squad group.  This function does nothing if the squad group is empty.
---
--- *Source: Command.scar*
---@param squadgroupname string
---@param stance integer
function Cmd_SetMeleeStance(squadgroupname, stance) end

--- Order a squad group set their stance.  Use STANCE_Hold, STANCE_StandGround, STANCE_Burn, STANCE_CeaseFire, or STANCE_Attack.
---
--- The order will be issued by the owner of the squad group.  This function does nothing if the squad group is empty.
---
--- *Source: Command.scar*
---@param squadgroupname string
---@param stance integer
function Cmd_SetStance(squadgroupname, stance) end

--- Command an egroup to stop.  This is the same as issuing a stop from the taskbar
---
--- *Source: Command.scar*
---@param egroupname string
function Cmd_StopEntities(egroupname) end

--- Command an sgroup to stop.  This is the same as issuing a stop from the taskbar
---
--- *Source: Command.scar*
---@param sgroupname string
function Cmd_StopSquads(sgroupname) end

-- from cpuplayerutil.scar

--- Lock an EGroup from the cpuplayer
---
--- NOTE: Only use this for strategic points and buildings! It won't work for anything else
---
--- *Source: CpuPlayerUtil.scar*
---@param cpu_player PlayerID
---@param egroup string
function Cpu_LockEGroup(cpu_player, egroup) end

--- Lock an EGroup from all cpuplayers
---
--- NOTE: Only use this for strategic points and buildings! It won't work for anything else
---
--- *Source: CpuPlayerUtil.scar*
---@param egroup string
function Cpu_LockEGroupAcrossPlayers(egroup) end

--- Lock an SGroup from the cpuplayer
---
--- *Source: CpuPlayerUtil.scar*
---@param cpu_player PlayerID
---@param sgroup string
function Cpu_LockSGroup(cpu_player, sgroup) end

--- Lock an SGroup from all cpuplayers
---
--- *Source: CpuPlayerUtil.scar*
---@param sgroup string
function Cpu_LockSGroupAcrossPlayers(sgroup) end

--- Set a build order table for the cpu_player
---
--- An example build table:
---     local build_table =
---     {
---         {"space_marine_squad_servitor", 1, 5},
---         {"space_marine_squad_scout", 2, 3}
---     }
---  The first number in each entry is the starting value -- the lowest number gets built first
---  The second number is the increment that happens each time the unit is built
---  NOTE: Setting build demand table clobbers the class preferences
---
--- *Source: CpuPlayerUtil.scar*
---@param cpu_player PlayerID
---@param build_table table
function Cpu_SetBuildTable(cpu_player, build_table) end

--- Set limits to the buildings that an AI can build
---
--- An example buildings table:
---     local buildings_table =
---     {
---         {"space_marine_vehicle_building", 2},
---         {"space_marine_listening_post", 1}
---     }
---  The first argument is the building name, the second is the number of those buildings the AI can build
---
--- *Source: CpuPlayerUtil.scar*
---@param cpu_player PlayerID
---@param buildings_table table
function Cpu_SetBuildingLimitsTable(cpu_player, buildings_table) end

--- Set the difficulty without worrying if the Cpu is enabled or not
---
--- If you set the difficulty on a Cpu that's disabled, it'll crash. This should take care of that.  NOTE: You can always use the regular one, this is only if you don't know or may not know
---
--- *Source: CpuPlayerUtil.scar*
---@param cpu_player PlayerID
---@param difficulty_setting integer
function Cpu_SetDifficultyForDummies(cpu_player, difficulty_setting) end

--- Unlock an EGroup from the cpuplayer
---
--- NOTE: Only use this for strategic points and buildings! It won't work for anything else
---
--- *Source: CpuPlayerUtil.scar*
---@param cpu_player PlayerID
---@param egroup string
function Cpu_UnlockEGroup(cpu_player, egroup) end

--- Lock an EGroup from all cpuplayers
---
--- NOTE: Only use this for strategic points and buildings! It won't work for anything else
---
--- *Source: CpuPlayerUtil.scar*
---@param egroup string
function Cpu_UnlockEGroupAcrossPlayers(egroup) end

--- Unlock an SGroup from the cpuplayer
---
--- *Source: CpuPlayerUtil.scar*
---@param cpu_player PlayerID
---@param sgroup string
function Cpu_UnlockSGroup(cpu_player, sgroup) end

--- Lock an SGroup from all cpuplayers
---
--- *Source: CpuPlayerUtil.scar*
---@param sgroup string
function Cpu_UnlockSGroupAcrossPlayers(sgroup) end

-- from difficultylevel.scar

--- Returns the difficulty level for the mission (DIFFICULTY_EASY, DIFFICULTY_NORMAL or DIFFICULTY_HARD)
---
--- *Source: DifficultyLevel.scar*
---@return integer
function Difficulty_Get() end

--- Set the difficulty level for all players in a mission
---
--- This function should be called to set the difficulty level for all players in the mission.  Note that it WILL NOT modify attributes for the player themselves, just everyone else.   Values for difficulty are: 0 = easy, 1 = normal, 2 = hard
---
--- *Source: DifficultyLevel.scar*
---@param difficultlevel integer
function Difficulty_SetForAll(difficultlevel) end

--- Set the difficulty level for one player
---
--- This function should be called to set the difficulty level for a single player in a mission.  Values for difficulty are: 0 = easy, 1 = normal, 2 = hard
---
--- *Source: DifficultyLevel.scar*
---@param playerId PlayerID
---@param difficultlevel integer
function Difficulty_SetForPlayer(playerId, difficultlevel) end

-- from entity.scar

--- Returns true if entity is in cover.
---
--- *Source: Entity.scar*
---@param entityId EntityID
---@return boolean
function Entity_IsInCover(entityId) end

-- from groups.scar

--- Returns true if ALL or ANY entities in a group can see ALL or ANY entities in a given egroup.
---
--- *Source: Groups.scar*
---@param egroupname string
---@param targetegroupname string
---@param all boolean
---@return boolean
function EGroup_CanSeeEGroup(egroupname, targetegroupname, all) end

--- Returns true if ALL or ANY entities in a group can see ALL or ANY squads in a given sgroup.
---
--- *Source: Groups.scar*
---@param egroupname string
---@param targetsgroupname string
---@param all boolean
---@return boolean
function EGroup_CanSeeSGroup(egroupname, targetsgroupname, all) end

--- Returns true if ALL or ANY items in a group have the given active command
---
--- See Entity_GetActiveCommand for valid command types
---
--- *Source: Groups.scar*
---@param groupname string
---@param cmd integer
---@param all boolean
---@return boolean
function EGroup_CheckActiveCommand(groupname, cmd, all) end

--- Returns true if ALL or ANY items in a group have built the given addon
---
--- *Source: Groups.scar*
---@param groupname string
---@param addonname string
---@param all boolean
---@return boolean
function EGroup_ContainsAddOn(groupname, addonname, all) end

--- Find a entity group from name.  Creates a new one with given name if it doesnt exist.
---
--- *Source: Groups.scar*
---@param name string
---@return EGroupID
function EGroup_CreateIfNotFound(name) end

--- Despawn all spawned entities in a group.
---
--- *Source: Groups.scar*
---@param groupname string
function EGroup_DeSpawn(groupname) end

--- Destroys all spawned and despawned entities in a group.
---
--- Be careful not to confuse this with EGroup_Destroy which destroys the group and NOT the items it contains.  This function will destroy spawned and despawned items in a group
---
--- *Source: Groups.scar*
---@param groupname string
function EGroup_DestroyAllEntities(groupname) end

--- Gives addon to all members of the group
---
--- *Source: Groups.scar*
---@param groupname string
---@param addonname string
---@return boolean
function EGroup_ForceAddOn(groupname, addonname) end

--- Returns the average health of all units in a entity group as a weighted percent [0.0, 1.0].
---
--- Example: A group of entities with health 20/100 and 100/1000 would return 120/1100 ( approx 11 % )
---
--- *Source: Groups.scar*
---@param groupname string
---@return number
function EGroup_GetAvgHealth(groupname) end

--- Check invulnerablity state for ALL or ANY entity in an entity group.
---
--- Set all param to true to check for ALL or set to false to check for ANY.
---
--- *Source: Groups.scar*
---@param groupname string
---@param all boolean
---@return boolean
function EGroup_GetHealthInvulnerable(groupname, all) end

--- Return true if any entities in a group contain a strategic point with a structure on it
---
--- *Source: Groups.scar*
---@param groupname string
---@return boolean
function EGroup_HasStrategicPointWithStructure(groupname) end

--- Returns true if all or any strategic points in a group have been captured.
---
--- This function will ignore all entities that cannot be captured and will return false if no entities in the group can be captured.
---
--- *Source: Groups.scar*
---@param groupname string
---@param playerId PlayerID
---@param all boolean
---@return boolean
function EGroup_IsCapturedByPlayer(groupname, playerId, all) end

--- Returns true if a named entity group contains no spawned or despawned entities
---
--- *Source: Groups.scar*
---@param name string
---@return boolean
function EGroup_IsEmpty(name) end

--- Returns true if ALL or ANY entities are in cover.
---
--- *Source: Groups.scar*
---@param egroupname string
---@param all boolean
---@return boolean
function EGroup_IsInCover(egroupname, all) end

--- Return true if all items in a group are selected
---
--- *Source: Groups.scar*
---@param groupname string
---@return boolean
function EGroup_IsSelected(groupname) end

--- Returns true if ALL or ANY entities are under attack.
---
--- *Source: Groups.scar*
---@param egroupname string
---@param all boolean
---@return boolean
function EGroup_IsUnderAttack(egroupname, all) end

--- Returns true if ALL or ANY entities are under attack.
---
--- *Source: Groups.scar*
---@param egroupname string
---@param all boolean
---@param player PlayerID
---@return boolean
function EGroup_IsUnderAttackByPlayer(egroupname, all, player) end

--- Respawn all despawned entities in a group.
---
--- *Source: Groups.scar*
---@param groupname string
function EGroup_ReSpawn(groupname) end

--- Respawns all entities in a group at a given position
---
--- *Source: Groups.scar*
function EGroup_ReSpawnMarker() end

--- Respawns all entities in a group at a given position
---
--- *Source: Groups.scar*
function EGroup_ReSpawnPosition() end

--- Call Entity_ResetConstructionState for all spawned buildings
---
--- See Entity_ResetConstructionState for more info
---
--- *Source: Groups.scar*
---@param groupname string
function EGroup_ResetConstructionState(groupname) end

--- Resets the player owner to the world for all entities in the group
---
--- *Source: Groups.scar*
---@param groupname string
function EGroup_ResetPlayerOwner(groupname) end

--- Sets the health of each unit in an entity group to a given percent [0.0, 1.0].
---
--- *Source: Groups.scar*
---@param groupname string
---@param healthPercent number
---@return number
function EGroup_SetAvgHealth(groupname, healthPercent) end

--- Forces all buildings in progress to complete their construction state
---
--- *Source: Groups.scar*
---@param groupname string
function EGroup_SetConstructionComplete(groupname) end

--- Enable/Disable invulnerablity for an entire entity group.
---
--- IMPORTANT: You must disable invulnerability for everytime you enable it!
---
--- *Source: Groups.scar*
---@param groupname string
---@param enabled boolean
function EGroup_SetHealthInvulnerable(groupname, enabled) end

--- Changes the player owner for all spawned and despawned entities of an EGroup.
---
--- *Source: Groups.scar*
---@param groupname string
---@param owner PlayerID
function EGroup_SetPlayerOwner(groupname, owner) end

--- Adds a leader to all squads in a group that can take a leader.
---
--- This function will bypass all cost and queue prereqs
---
--- *Source: Groups.scar*
---@param sgroupname string
function SGroup_AddLeaders(sgroupname) end

--- Returns true if ALL or ANY squads in a group can see ALL or ANY squads in a target sgroup.
---
--- *Source: Groups.scar*
---@param sgroupname string
---@param targetegroupname string
---@param all boolean
---@return boolean
function SGroup_CanSeeSGroup(sgroupname, targetegroupname, all) end

--- Returns true if ALL or ANY items in a group have the given active command
---
--- See Squad_GetActiveCommand for valid command types
---
--- *Source: Groups.scar*
---@param groupname string
---@param cmd integer
---@param all boolean
---@return boolean
function SGroup_CheckActiveCommand(groupname, cmd, all) end

--- Check if all or any squads in a group contain (at least) the given number of weapon blueprints
---
--- *Source: Groups.scar*
---@param groupname string
---@param weaponname string
---@param count integer
---@param all boolean
---@return boolean
function SGroup_ContainsWeaponBlueprints(groupname, weaponname, count, all) end

--- Counts the number of leaders in all squads in the specified group
---
--- *Source: Groups.scar*
---@param groupName string
---@return integer
function SGroup_CountLeaders(groupName) end

--- Find a squad group from name.  Creates a new one with given name if it doesnt exist.
---
--- *Source: Groups.scar*
---@param name string
---@return SGroupID
function SGroup_CreateIfNotFound(name) end

--- Despawn all spawned squads in a group.
---
--- *Source: Groups.scar*
---@param groupname string
function SGroup_DeSpawn(groupname) end

--- Destroys all items in a group that are in proximity to a given marker.
---
--- *Source: Groups.scar*
---@param groupname string
---@param markername string
function SGroup_DestroyAllInMarker(groupname, markername) end

--- Destroys all spawned and despawned squads in a group.
---
--- Be careful not to confuse this with SGroup_Destroy which destroys the group and NOT the squads it contains.  This function will destroy spawned and despawned items in a group
---
--- *Source: Groups.scar*
---@param groupname string
function SGroup_DestroyAllSquads(groupname) end

--- Makes two SGroups face each other
---
--- *Source: Groups.scar*
---@param sgroupname1 string
---@param sgroupname2 string
function SGroup_FaceEachOther(sgroupname1, sgroupname2) end

--- Makes a SGroup face a marker.
---
--- *Source: Groups.scar*
---@param sgroupname string
---@param markername string
function SGroup_FaceMarker(sgroupname, markername) end

--- Makes the first SGroup face the second SGroup
---
--- *Source: Groups.scar*
---@param sgroupname1 string
---@param sgroupname2 string
function SGroup_FaceSGroup(sgroupname1, sgroupname2) end

--- Returns the average health of all units in a squad group as a weighted percent [0.0, 1.0].
---
--- Example: A group of squads with health 20/100 and 100/1000 would return 120/1100 ( approx 11 % )
---
--- *Source: Groups.scar*
---@param groupname string
---@return number
function SGroup_GetAvgHealth(groupname) end

--- Returns the average loadout of all squads in a group as a percent [0.0, 1.0].
---
--- Example: A group of squads with loadouts of 4/8 and 1/1 would return 0.75
---
--- *Source: Groups.scar*
---@param groupname string
---@return number
function SGroup_GetAvgLoadout(groupname) end

--- Returns the average morale of all units in a squad group as a weighted percent [0.0, 1.0].
---
--- Squads that are not affected by morale always have morale of 1.0.  These squads will count towards your   total average
---
--- *Source: Groups.scar*
---@param groupname string
---@return number
function SGroup_GetAvgMorale(groupname) end

--- Check invulnerablity state for ALL or ANY squads in a squad group.
---
--- Set all param to true to check for ALL or set to false to check for ANY.
---
--- *Source: Groups.scar*
---@param groupname string
---@param all boolean
---@return boolean
function SGroup_GetHealthInvulnerable(groupname, all) end

--- Check morale invulnerablity state for ALL or ANY squads in a squad group.
---
--- Set all param to true to check for ALL or set to false to check for ANY.
---
--- *Source: Groups.scar*
---@param groupname string
---@param all boolean
---@return boolean
function SGroup_GetMoraleInvulnerable(groupname, all) end

--- Returns true if the sgroup has ANY squads of type blueprint have received reinforcements
---
--- *Source: Groups.scar*
---@param sgroupname string
---@param squadblueprint string
---@return boolean
function SGroup_HasAnyReinforced(sgroupname, squadblueprint) end

--- Check if ALL or ANY squads in a sgroup have a leader
---
--- Set all param to true to check for ALL or set to false to check for ANY.
---
--- *Source: Groups.scar*
---@param groupname string
---@param all boolean
---@return boolean
function SGroup_HasLeader(groupname, all) end

--- Returns true if a named squad group is empty
---
--- *Source: Groups.scar*
---@param name string
---@return boolean
function SGroup_IsEmpty(name) end

--- Returns true if ALL or ANY squads are in cover.
---
--- *Source: Groups.scar*
---@param sgroupname string
---@param all boolean
---@return boolean
function SGroup_IsInCover(sgroupname, all) end

--- Returns true if ALL or ANY squads in a group are infiltrated
---
--- *Source: Groups.scar*
---@param sgroupname string
---@param all boolean
---@return boolean
function SGroup_IsInfiltrated(sgroupname, all) end

--- Returns true if ALL or ANY items in a group have broken morale
---
--- *Source: Groups.scar*
---@param groupname string
---@param all boolean
---@return boolean
function SGroup_IsMoraleBroken(groupname, all) end

--- Returns true if ANY entities in ANY squads of a group are on currently screen
---
--- IMPORTANT: This function does not check the FOW.  It will return true  even if the item is not revealed in FOW
---
--- *Source: Groups.scar*
---@param groupname string
---@return boolean
function SGroup_IsOnScreen(groupname) end

--- Return true if all items in a group are selected
---
--- *Source: Groups.scar*
---@param groupname string
---@return boolean
function SGroup_IsSelected(groupname) end

--- Returns true if ALL or ANY squads are under attack.
---
--- *Source: Groups.scar*
---@param sgroupname string
---@param all boolean
---@return boolean
function SGroup_IsUnderAttack(sgroupname, all) end

--- Returns true if ALL or ANY squads are under attack.
---
--- *Source: Groups.scar*
---@param sgroupname string
---@param all boolean
---@param player PlayerID
---@return boolean
function SGroup_IsUnderAttackByPlayer(sgroupname, all, player) end

--- Respawn all despawned squads in a group.
---
--- *Source: Groups.scar*
---@param groupname string
function SGroup_ReSpawn(groupname) end

--- Respawns all squads in a group at a given position
---
--- *Source: Groups.scar*
function SGroup_ReSpawnMarker() end

--- Respawns all squads in a group at a given position
---
--- *Source: Groups.scar*
function SGroup_ReSpawnPosition() end

--- Sets the health of each squad in a squad group to a given percent [0.0, 1.0].
---
--- *Source: Groups.scar*
---@param groupname string
---@param healthPercent number
---@return number
function SGroup_SetAvgHealth(groupname, healthPercent) end

--- Sets the moral of each squad in a squad group to a given percent [0.0, 1.0].
---
--- *Source: Groups.scar*
---@param groupname string
---@param moralePercent number
---@return number
function SGroup_SetAvgMorale(groupname, moralePercent) end

--- Enable/Disable invulnerablity for an entire squad group.
---
--- IMPORTANT: You must disable invulnerability for everytime you enable it!
---
--- *Source: Groups.scar*
---@param groupname string
---@param enabled boolean
function SGroup_SetHealthInvulnerable(groupname, enabled) end

--- Enable/Disable morale invulnerablity for an entire squad group.
---
--- IMPORTANT: You must disable invulnerability for everytime you enable it!
---
--- *Source: Groups.scar*
---@param groupname string
---@param enabled boolean
function SGroup_SetMoraleInvulnerable(groupname, enabled) end

--- Changes the player owner of spawned and despawned squads in an SGroup.
---
--- *Source: Groups.scar*
---@param groupname string
---@param owner PlayerID
function SGroup_SetPlayerOwner(groupname, owner) end

-- from modifiers.scar

--- Applys/Removes a modifier to an egroup
---
--- *Source: Modifiers.scar*
---@param groupid EGroupID
---@param modifier ScarModifier
---@param apply boolean
function ModifierUtil_ApplyToEGroup(groupid, modifier, apply) end

--- Applys/Removes a modifier to a squadgroup
---
--- *Source: Modifiers.scar*
---@param groupid SGroupID
---@param modifier ScarModifier
---@param apply boolean
function ModifierUtil_ApplyToSGroup(groupid, modifier, apply) end

--- Returns the applier table index for a given modifier.
---
--- Returns zero if applier for modifier is not found.
---
--- *Source: Modifiers.scar*
---@param modifier ScarModifier
---@param entityid EntityID
---@return integer
function ModifierUtil_GetEntityApplierIndex(modifier, entityid) end

--- Returns the applier table index for a given modifier.
---
--- Returns zero if applier for modifier is not found.
---
--- *Source: Modifiers.scar*
---@param modifier ScarModifier
---@param squadid SquadID
---@return integer
function ModifierUtil_GetSquadApplierIndex(modifier, squadid) end

--- Toggles the fx for the specified modifer
---
--- *Source: Modifiers.scar*
---@param modifier ScarModifier
function ModifierUtil_ToggleFX(modifier) end

-- from player.scar

--- Add resource to player, as opposed to just setting it.
---
--- *Source: Player.scar*
---@param playerId PlayerID
---@param resourceType integer
---@param value number
function Player_AddResource(playerId, resourceType, value) end

--- For the given player, get all of the squads gathered into a squadgroup of your naming.
---
--- Squads will be added to given squad group.  If the given squad group does not exist it will be created.
---
--- *Source: Player.scar*
---@param playerId PlayerID
---@param squadgroupName string
---@return SGroupID
function Player_AddSquadsToSGroup(playerId, squadgroupName) end

--- Returns true if ANY of a players squads are in proximity of a marker
---
--- *Source: Player.scar*
---@param playerID PlayerID
---@param markername string
---@return boolean
function Player_AreSquadsNearMarker(playerID, markername) end

--- Returns true if a player can see ALL or ANY items in an egroup
---
--- *Source: Player.scar*
---@param playerId PlayerID
---@param egroupname string
---@param all boolean
---@return boolean
function Player_CanSeeEGroup(playerId, egroupname, all) end

--- Returns true if a player can see ALL or ANY items in an sgroup
---
--- *Source: Player.scar*
---@param playerId PlayerID
---@param sgroupname string
---@param all boolean
---@return boolean
function Player_CanSeeSGroup(playerId, sgroupname, all) end

--- Sets the max squad cap for a player.
---
--- *Source: Player.scar*
---@param playerId PlayerID
---@param cap integer
function Player_ForceSetMaxSquadCap(playerId, cap) end

--- Sets the max support cap for a player.
---
--- *Source: Player.scar*
---@param playerId PlayerID
---@param cap integer
function Player_ForceSetMaxSupportCap(playerId, cap) end

--- Add all players entities that are in proximity to a marker into a squad group.  Group will be created if it doesnt already exist.
---
--- The proximity of a marker is set in the mission editor.
---
--- *Source: Player.scar*
---@param playerId PlayerID
---@param egroupName string
---@param markerName string
function Player_GetAllEntitiesNearMarker(playerId, egroupName, markerName) end

--- Add all players squads that are in proximity to a marker into a squad group.  Group will be created if it doesnt already exist.
---
--- The proximity of a marker is set in the mission editor.
---
--- *Source: Player.scar*
---@param playerId PlayerID
---@param squadgroupName string
---@param markerName string
function Player_GetAllSquadsNearMarker(playerId, squadgroupName, markerName) end

--- Returns the number of buidlings/sqauds with blueprints owned by a player
---
--- blueprint name can be a squad blueprint OR an entity blueprint
---
--- *Source: Player.scar*
---@param player PlayerID
---@param blueprintname string
---@return integer
function Player_GetBlueprintCount(player, blueprintname) end

--- Returns the total number of buildings owned by this player.
---
--- *Source: Player.scar*
---@param playerId PlayerID
---@return integer
function Player_GetBuildingsCount(playerId) end

--- Returns the total number of buildings owned by this player (with exclusions).
---
--- *Source: Player.scar*
---@param playerId PlayerID
---@param exceptions table
---@return integer
function Player_GetBuildingsCountExcept(playerId, exceptions) end

--- Returns the number of buildings owned by this player (inclusive).
---
--- *Source: Player.scar*
---@param playerId PlayerID
---@param building_names table
---@return integer
function Player_GetBuildingsCountOnly(playerId, building_names) end

--- Returns true if this player owns any of the indicated building type and they are fully constructed
---
--- *Source: Player.scar*
---@param playerId PlayerID
---@param building_blueprint Blueprint
---@return integer
function Player_HasBuildingType(playerId, building_blueprint) end

--- Returns true if this player owns any buildings. (with exclusions).
---
--- *Source: Player.scar*
---@param playerId PlayerID
---@param exceptions table
---@return integer
function Player_HasBuildingsExcept(playerId, exceptions) end

--- Returns true if this player owns any squads (with exclusions).
---
--- *Source: Player.scar*
---@param playerId PlayerID
---@param exceptions table
---@return integer
function Player_HasSquadsExcept(playerId, exceptions) end

--- Increases the ork pop cap using a modifier
---
--- *Source: Player.scar*
---@param playerId PlayerID
---@param maxcap integer
function Player_IncreasePopCap(playerId, maxcap) end

--- Returns true if the players are allied and false if they are not.
---
--- *Source: Player.scar*
---@param playerId1 PlayerID
---@param playerId2 PlayerID
---@return boolean
function Player_IsAllied(playerId1, playerId2) end

--- Returns true if a given player exculsivley owns all items in a group
---
--- *Source: Player.scar*
---@param playerID PlayerID
---@param egroupID EGroupID
---@return boolean
function Player_OwnsEGroup(playerID, egroupID) end

--- Returns true if a given player exculsivley owns all items in a group
---
--- *Source: Player.scar*
---@param playerID PlayerID
---@param sgroupID SGroupID
---@return boolean
function Player_OwnsSGroup(playerID, sgroupID) end

--- Restrict a list of names.
---
--- list should contain an array of strings to restrict.
---
--- *Source: Player.scar*
---@param id PlayerID
---@param list table
function Player_RestrictAddOnList(id, list) end

--- Restrict a list of names.
---
--- list should contain an array of strings to restrict.
---
--- *Source: Player.scar*
---@param id PlayerID
---@param list table
function Player_RestrictBuildingList(id, list) end

--- Restrict a list of names.
---
--- list should contain strings of research to restrict.
---
--- *Source: Player.scar*
---@param id PlayerID
---@param list table
function Player_RestrictResearchList(id, list) end

--- Restrict a list of names.
---
--- list should contain an array of strings to restrict.
---
--- *Source: Player.scar*
---@param id PlayerID
---@param list table
function Player_RestrictSquadList(id, list) end

--- Sets the current squad cap for the player.
---
--- Note that any modifiers that adjust the current squad cap will be applied on top of this.  Also note that the current squad cap cannot go higher than the max squad cap.
---
--- *Source: Player.scar*
---@param playerId PlayerID
---@param maxcap integer
function Player_SetSquadCap(playerId, maxcap) end

--- Sets the current support cap for a player.
---
--- Note that any modifiers that adjust the current support cap will be applied on top of this.  Also note, the current support cap cannot go higher than the max support cap.
---
--- *Source: Player.scar*
---@param playerId PlayerID
---@param cap integer
function Player_SetSupportCap(playerId, cap) end

--- Unrestricts a list of names.
---
--- List should contain an array of research to restrict.
---
--- *Source: Player.scar*
---@param id PlayerID
---@param list table
function Player_UnRestrictResearchList(id, list) end

--- Adds all the points owned by a given player into a given group.  use types SPT_StrategicPoint SPT_StrategicObjective or SPT_Relic
---
--- Pass in a playerID of nil to get all relics owned by the world
---
--- *Source: Player.scar*
---@param strategicpoint_type integer
---@param playerID PlayerID
---@param groupname string
function Util_GetStrategicPoints(strategicpoint_type, playerID, groupname) end

-- from production.scar

--- Adds a add-on item to a production list table.
---
--- *Source: Production.scar*
---@param addonname string
---@param list table
function Prod_AddAddOnToList(addonname, list) end

--- Adds a research item to a production list table.
---
--- *Source: Production.scar*
---@param researchname string
---@param list table
function Prod_AddResearchToList(researchname, list) end

--- Adds a squad item to a production list table.
---
--- *Source: Production.scar*
---@param blueprintname string
---@param list table
function Prod_AddSquadToList(blueprintname, list) end

--- Returns true if a production list contains the given add-on item
---
--- *Source: Production.scar*
---@param addonname string
---@param list table
---@return boolean
function Prod_DoesListContainAddOn(addonname, list) end

--- Returns true if a production list contains the given research item
---
--- *Source: Production.scar*
---@param researchname string
---@param list table
---@return boolean
function Prod_DoesListContainResearch(researchname, list) end

--- Returns true if a production list contains the given squad item
---
--- *Source: Production.scar*
---@param blueprintname string
---@param list table
---@return boolean
function Prod_DoesListContainSquad(blueprintname, list) end

--- Gets everything a given player currently has in production.
---
--- See Prod_GetProductionList for more info.
---
--- *Source: Production.scar*
---@param playerId PlayerID
---@param list table
function Prod_GetPlayerProductionList(playerId, list) end

--- Checks all entities to see if they're production-capable and if so, adds their queued items to a Production Table.
---
--- Use Prod_DoesListContainSquad etc to query the resulting production table.
---
---  Example:
---      local ProductionTable = { }
---      Prod_GetProductionList( "GRP_ANYENTITYGROUP", ProductionTable )
---      if(Prod_DoesListContainSquad("chaos_marine_squad", ProductionTable) then
---          -- There is a building in GRP_ANYENTITYGROUP that is currently producing a space marine Squad_CanSeeEntity
---      end
---
--- *Source: Production.scar*
---@param egroupname string
---@param list table
function Prod_GetProductionList(egroupname, list) end

--- Returns true the players henchmen are currently constructing a building with given blueprintname.
---
--- Note: If you find that you are calling this function for tons of different buildings, we should change this function to take a constuction list (similar to the way production lists are handled)
---
--- *Source: Production.scar*
---@param playerId PlayerID
---@param buildingname string
---@return boolean
function Prod_IsPlayerConstructing(playerId, buildingname) end

--- Returns true if the player is producing all or any items in a production table.  See Prod_IsProducing for more info.
---
--- *Source: Production.scar*
---@param playerId PlayerID
---@param list table
---@param all boolean
---@return boolean
function Prod_IsPlayerProducing(playerId, list, all) end

--- Returns true if buildings in a entity group are producing ALL or ANY items in a given production table.
---
--- Use Prod_AddSquadToList etc to add entries to the given production list.
---
---  Example:
---      local ProductionTable = { }
---      Prod_AddSquadToList( "chaos_squad_cultist", list )
---      Prod_AddSquadToList( "chaos_squad_cultist", list )
---      Prod_AddSquadToList( "chaos_squad_defiler", list )
---      if( Prod_IsProducing( "GRP_ANYENTITYGROUP", ProcutionTable, true ) then
---          -- Of all the buildings in GRP_ANYENTITYGROUP, they are producing 2 cultists and a defiler.
---      end
---
--- *Source: Production.scar*
---@param egroupname string
---@param list table
---@param all boolean
---@return boolean
function Prod_IsProducing(egroupname, list, all) end

-- from proximity.scar

--- Returns true if ALL entities in a group are in proximity to a marker.
---
--- The proximity of a marker can only be set in the mission editor.
---
--- *Source: Proximity.scar*
---@param groupname string
---@param markername string
---@return boolean
function Prox_AllEntitiesNearMarker(groupname, markername) end

--- Returns true if ALL squads in a group are in proximity to a marker.
---
--- The proximity of a marker can only be set in the mission editor.
---
--- *Source: Proximity.scar*
---@param groupname string
---@param markername string
---@return boolean
function Prox_AllSquadsNearMarker(groupname, markername) end

--- Returns true if ANY entity from a group are in proximity to a marker.
---
--- The proximity of a marker can only be set in the mission editor.
---
--- *Source: Proximity.scar*
---@param groupname string
---@param markername string
---@return boolean
function Prox_AnyEntityNearMarker(groupname, markername) end

--- Returns true if ANY squads from a group are in proximity to a marker.
---
--- The proximity of a marker can only be set in the mission editor.
---
--- *Source: Proximity.scar*
---@param groupname string
---@param markername string
---@return boolean
function Prox_AnySquadNearMarker(groupname, markername) end

--- Returns the distance between two entity groups. use checktype PROX_SHORTEST, PROX_LONGEST, or PROX_CENTER.
---
--- If check is PROX_SHORTEST this will return the shortest distance between the two groups.
---  If check is PROX_LONGEST this will return the longest distance between the two groups.
---  If check is PROX_CENTER this will return the distance between the two groups centers.
---
--- *Source: Proximity.scar*
---@param groupname1 string
---@param groupname2 string
---@param checktype ProxType
---@return number
function Prox_EGroupEGroup(groupname1, groupname2, checktype) end

--- Returns the distance between an entity group and a squad group.  use checktype PROX_SHORTEST, PROX_LONGEST, or PROX_CENTER.
---
--- If check is PROX_SHORTEST this will return the shortest distance between the two groups.
---  If check is PROX_LONGEST this will return the longest distance between the two groups.
---  If check is PROX_CENTER this will return the distance between the two groups centers.
---
--- *Source: Proximity.scar*
---@param egroupname string
---@param sgroupname string
---@param checktype ProxType
---@return number
function Prox_EGroupSGroup(egroupname, sgroupname, checktype) end

--- Checks if ALL or ANY entities are in proximity of a given entity group.
---
--- Set "all" to true to check that ALL entities are in proximity, or set "all" to false to check for ANY.
---
--- *Source: Proximity.scar*
---@param egroupname1 string
---@param egroupname2 string
---@param proximity number
---@param all boolean
---@return boolean
function Prox_EntitiesInProximityOfEntities(egroupname1, egroupname2, proximity, all) end

--- Returns the distance between a marker and an entity group.  use checktype PROX_SHORTEST, PROX_LONGEST, or PROX_CENTER.
---
--- If check is PROX_SHORTEST this will return the shortest distance between the two groups.
---  If check is PROX_LONGEST this will return the longest distance between the two groups.
---  If check is PROX_CENTER this will return the distance between the two groups centers.
---
--- *Source: Proximity.scar*
---@param markername string
---@param egroupname string
---@param checktype ProxType
---@return number
function Prox_MarkerEGroup(markername, egroupname, checktype) end

--- Returns the distance between a marker and a squad group.  use checktype PROX_SHORTEST, PROX_LONGEST, or PROX_CENTER.
---
--- If check is PROX_SHORTEST this will return the shortest distance between the two groups.
---  If check is PROX_LONGEST this will return the longest distance between the two groups.
---  If check is PROX_CENTER this will return the distance between the two groups centers.
---
--- *Source: Proximity.scar*
---@param markername string
---@param sgroupname string
---@param checktype ProxType
---@return number
function Prox_MarkerSGroup(markername, sgroupname, checktype) end

--- Checks if ALL or ANY players squads are in proximity of a given entity group.
---
--- Set "all" to true to check that ALL entities are in proximity, or set "all" to false to check for ANY.
---
--- *Source: Proximity.scar*
---@param playerId PlayerID
---@param egroupname string
---@param proximity number
---@param all boolean
---@return boolean
function Prox_PlayerEntitiesInProximityOfEntities(playerId, egroupname, proximity, all) end

--- Checks if ALL or ANY players entities are in proximity of a given squad group.
---
--- Set "all" to true to check that ALL entities are in proximity, or set "all" to false to check for ANY.
---
--- *Source: Proximity.scar*
---@param playerId1 PlayerID
---@param playerId2 PlayerID
---@param proximity number
---@param all boolean
---@return boolean
function Prox_PlayerEntitiesInProximityOfPlayerSquads(playerId1, playerId2, proximity, all) end

--- Checks if ALL or ANY players entities are in proximity of a given squad group.
---
--- Set "all" to true to check that ALL entities are in proximity, or set "all" to false to check for ANY.
---
--- *Source: Proximity.scar*
---@param playerId PlayerID
---@param sgroupname string
---@param proximity number
---@param all boolean
---@return boolean
function Prox_PlayerEntitiesInProximityOfSquads(playerId, sgroupname, proximity, all) end

--- Checks if ALL or ANY players squads are in proximity of a given entity group.
---
--- Set "all" to true to check that ALL squads are in proximity, or set "all" to false to check for ANY.
---
--- *Source: Proximity.scar*
---@param playerId PlayerID
---@param sgroupname string
---@param proximity number
---@param all boolean
---@return boolean
function Prox_PlayerSquadsInProximityOfEntities(playerId, sgroupname, proximity, all) end

--- Checks if ALL or ANY players squads are in proximity of a given players entities.
---
--- Set "all" to true to check that ALL squads are in proximity, or set "all" to false to check for ANY.
---
--- *Source: Proximity.scar*
---@param playerId1 PlayerID
---@param playerId2 PlayerID
---@param proximity number
---@param all boolean
---@return boolean
function Prox_PlayerSquadsInProximityOfPlayerEntities(playerId1, playerId2, proximity, all) end

--- Checks if ALL or ANY players squads are in proximity of a given players squads.
---
--- Set "all" to true to check that ALL squads are in proximity, or set "all" to false to check for ANY.
---
--- *Source: Proximity.scar*
---@param playerId1 PlayerID
---@param playerId2 PlayerID
---@param proximity number
---@param all boolean
---@return boolean
function Prox_PlayerSquadsInProximityOfPlayerSquads(playerId1, playerId2, proximity, all) end

--- Checks if ALL or ANY players squads are in proximity of a given squad group.
---
--- Set "all" to true to check that ALL squads are in proximity, or set "all" to false to check for ANY.
---
--- *Source: Proximity.scar*
---@param playerId PlayerID
---@param sgroupname string
---@param proximity number
---@param all boolean
---@return boolean
function Prox_PlayerSquadsInProximityOfSquads(playerId, sgroupname, proximity, all) end

--- Returns the distance between two squad groups. use checktype PROX_SHORTEST, PROX_LONGEST, or PROX_CENTER.
---
--- If check is PROX_SHORTEST this will return the shortest distance between the two groups.
---  If check is PROX_LONGEST this will return the longest distance between the two groups.
---  If check is PROX_CENTER this will return the distance between the two groups centers.
---
--- *Source: Proximity.scar*
---@param groupname1 string
---@param groupname2 string
---@param checktype ProxType
---@return number
function Prox_SGroupSGroup(groupname1, groupname2, checktype) end

--- Checks if ALL or ANY squads are in proximity of a given entity group.
---
--- Set "all" to true to check that ALL squads are in proximity, or set "all" to false to check for ANY.
---
--- *Source: Proximity.scar*
---@param sgroupname string
---@param egroupname string
---@param proximity number
---@param all boolean
---@return boolean
function Prox_SquadsInProximityOfEntities(sgroupname, egroupname, proximity, all) end

--- Checks if ALL or ANY squads are in proximity of a given squad group.
---
--- Set "all" to true to check that ALL squads are in proximity, or set "all" to false to check for ANY.
---
--- *Source: Proximity.scar*
---@param sgroupname1 string
---@param sgroupname2 string
---@param proximity number
---@param all boolean
---@return boolean
function Prox_SquadsInProximityOfSquads(sgroupname1, sgroupname2, proximity, all) end

-- from restrict.scar

--- Applies restrictions for space marines.  You should call this at the begining of your mission for Player 1.
---
--- *Source: Restrict.scar*
---@param playerId PlayerID
---@param level integer
function Restrict_SpaceMarines(playerId, level) end

--- Unrestricts a specific addon and gives an intelevent style dialog and an event cue with it.  Pass in "none" for event cue or dialog in order to not display the alerts
---
--- *Source: Restrict.scar*
---@param playerId PlayerID
---@param addon_blueprint Blueprint
---@param eventcuedescription_locId integer
---@param dialog_locId integer
function Unrestrict_AddOnWithAlerts(playerId, addon_blueprint, eventcuedescription_locId, dialog_locId) end

--- Unrestricts a specific building and gives an intelevent style dialog and an event cue with it.  Pass in "none" for event cue or dialog in order to not display the alerts
---
--- *Source: Restrict.scar*
---@param playerId PlayerID
---@param building_blueprint Blueprint
---@param eventcuedescription_locId integer
---@param dialog_locId integer
function Unrestrict_BuildingWithAlerts(playerId, building_blueprint, eventcuedescription_locId, dialog_locId) end

--- Unrestricts a specific research and gives an intelevent style dialog and an event cue with it.  Pass in "none" for event cue or dialog in order to not display the alerts
---
--- *Source: Restrict.scar*
---@param playerId PlayerID
---@param research_blueprint Blueprint
---@param eventcuedescription_locId integer
---@param dialog_locId integer
function Unrestrict_ResearchWithAlerts(playerId, research_blueprint, eventcuedescription_locId, dialog_locId) end

--- Unrestricts a specific squad and gives an intelevent style dialog and an event cue with it.  Pass in "none" for event cue or dialog in order to not display the alerts
---
--- *Source: Restrict.scar*
---@param playerId PlayerID
---@param squad_blueprint Blueprint
---@param eventcuedescription_locId integer
---@param dialog_locId integer
function Unrestrict_SquadWithAlerts(playerId, squad_blueprint, eventcuedescription_locId, dialog_locId) end

--- Unrestricts a specific weapon  and gives an intelevent style dialog and an event cue with it.  Pass in "none" for event cue or dialog in order to not display the alerts
---
--- *Source: Restrict.scar*
---@param playerId PlayerID
---@param weapon_blueprint Blueprint
---@param eventcuedescription_locId integer
---@param dialog_locId integer
function Unrestrict_WeaponWithAlerts(playerId, weapon_blueprint, eventcuedescription_locId, dialog_locId) end

-- from scarutil.scar

--- Reveals an area the size of a given markers proximity at that markers position for a given amount of time.  YOU SHOULD ONLY CALL THIS ONCE FOR EACH AREA.
---
--- This function will reveal the FOW for ALL alive players.
---
--- *Source: ScarUtil.scar*
---@param markername string
---@param durationSecs number
function FOW_RevealMarker(markername, durationSecs) end

--- Breaks the morale of the currently selected squad
---
--- *Source: ScarUtil.scar*
function Misc_BreakCurrentlySelectedSquad() end

--- Checks if any input event have occured in the last second
---
--- *Source: ScarUtil.scar*
function Misc_DetectAnyInput() end

--- Set the Force Commanders weapon to be Power Sword
---
--- This function should be used in any mission where the Force Commander shouldnt have a Daemon hamemr.
---
--- *Source: ScarUtil.scar*
---@param playerId PlayerID
function SetCommanderPowerSword(playerId) end

--- Set the Inquisitors weapon to be Power Sword
---
--- This function should be used in any mission where the Toth (Inquisitor) shouldnt have a Daemon hamemr.
---
--- *Source: ScarUtil.scar*
---@param playerId PlayerID
function SetInquisitorPowerSword(playerId) end

--- Creates an Autosave and prints text to screen to let player know its happening.
---
--- Use this function to create a save game in the single player game
---
--- *Source: ScarUtil.scar*
---@param localizedID string
function Util_Autosave(localizedID) end

--- If only one team is alive, players on that team win.  Returns true if one team left.
---
--- *Source: ScarUtil.scar*
---@param win_condition string
---@return boolean
function Util_CheckOneTeamLeft(win_condition) end

--- Creates horrors and summons them in
---
--- Use loadout of zero to create a squad at its min loadout.  Squads can never exceed their max loadout.
---
--- *Source: ScarUtil.scar*
---@param playerId PlayerID
---@param squadgroupName string
---@param pos Position
---@param numsquads integer
---@param loadout integer
---@return SGroupID
function Util_CreateHorrorsAndSummon(playerId, squadgroupName, pos, numsquads, loadout) end

--- Creates a given number of squads with a loadout at a position and adds them to a squadgroup.  The squads are then dropped in a droppod.
---
--- Use loadout of zero to create a squad at its min loadout.  Squads can never exceed their max loadout.
---
--- *Source: ScarUtil.scar*
---@param playerId PlayerID
---@param squadgroupName string
---@param squadBlueprint string
---@param pos Position
---@param numsquads integer
---@param loadout integer
---@return SGroupID
function Util_CreateSquadsAndDropPodIn(playerId, squadgroupName, squadBlueprint, pos, numsquads, loadout) end

--- Non-disrupting version of CreatSquadsAndDropPodIn.  Creates a given number of squads with a loadout at a position and adds them to a squadgroup.  The squads are then dropped in a droppod that has no disruption.
---
--- Use loadout of zero to create a squad at its min loadout.  Squads can never exceed their max loadout.
---
--- *Source: ScarUtil.scar*
---@param playerId PlayerID
---@param squadgroupName string
---@param squadBlueprint string
---@param pos Position
---@param numsquads integer
---@param loadout integer
---@return SGroupID
function Util_CreateSquadsAndDropPodIn2(playerId, squadgroupName, squadBlueprint, pos, numsquads, loadout) end

--- Creates a given number of squads at a marker position and adds them to a squadgroup.  The squadgroup will be created if it doesnt exist.
---
--- Squads will be created with min loadout.
---
--- *Source: ScarUtil.scar*
---@param playerId PlayerID
---@param squadgroupName string
---@param squadBlueprint string
---@param markername string
---@param numsquads integer
---@return SGroupID
function Util_CreateSquadsAtMarker(playerId, squadgroupName, squadBlueprint, markername, numsquads) end

--- Creates a given number of squads at a marker position with a loadout and adds them to a squadgroup.  The squadgroup will be created if it doesnt exist.
---
--- Use loadout of zero to create a squad at its min loadout.  Squads can never exceed their max loadout.
---
--- *Source: ScarUtil.scar*
---@param playerId PlayerID
---@param squadgroupName string
---@param squadBlueprint string
---@param markername string
---@param numsquads integer
---@param loadout integer
---@return SGroupID
function Util_CreateSquadsAtMarkerEx(playerId, squadgroupName, squadBlueprint, markername, numsquads, loadout) end

--- Creates a given number of squads at a marker position and adds them to a squadgroup.  The squad will spawn at a slightly random position in relation to the specified position.  The squadgroup will be created if it doesnt exist.
---
--- Squads will be created with min loadout.
---
--- *Source: ScarUtil.scar*
---@param playerId PlayerID
---@param squadgroupName string
---@param squadBlueprint string
---@param markername string
---@param numsquads integer
---@return SGroupID
function Util_CreateSquadsAtMarkerRandom(playerId, squadgroupName, squadBlueprint, markername, numsquads) end

--- Creates a given number of squads at a marker position with a loadout and adds them to a squadgroup.  The squad will spawn at a slightly random position in relation to the specified position.  The squadgroup will be created if it doesnt exist.
---
--- Use loadout of zero to create a squad at its min loadout.  Squads can never exceed their max loadout.
---
--- *Source: ScarUtil.scar*
---@param playerId PlayerID
---@param squadgroupName string
---@param squadBlueprint string
---@param markername string
---@param numsquads integer
---@param loadout integer
---@return SGroupID
function Util_CreateSquadsAtMarkerRandomEx(playerId, squadgroupName, squadBlueprint, markername, numsquads, loadout) end

--- Creates a given number of squads at a position and adds them to a squadgroup.  The squadgroup will be created if it doesnt exist.
---
--- Squads will be created with min loadout.
---
--- *Source: ScarUtil.scar*
---@param playerId PlayerID
---@param squadgroupName string
---@param squadBlueprint string
---@param pos Position
---@param numsquads integer
---@return SGroupID
function Util_CreateSquadsAtPosition(playerId, squadgroupName, squadBlueprint, pos, numsquads) end

--- Creates a given number of squads with a loadout at a position and adds them to a squadgroup.  The squadgroup will be created if it doesnt exist.
---
--- Use loadout of zero to create a squad at its min loadout.  Squads can never exceed their max loadout.
---
--- *Source: ScarUtil.scar*
---@param playerId PlayerID
---@param squadgroupName string
---@param squadBlueprint string
---@param pos Position
---@param numsquads integer
---@param loadout integer
---@return SGroupID
function Util_CreateSquadsAtPositionEx(playerId, squadgroupName, squadBlueprint, pos, numsquads, loadout) end

--- Creates a given number of squads at a position and adds them to a squadgroup.  The squad will spawn at a slightly random position in relation to the specified position.  The squadgroup will be created if it doesnt exist.
---
--- Squads will be created with min loadout.
---
--- *Source: ScarUtil.scar*
---@param playerId PlayerID
---@param squadgroupName string
---@param squadBlueprint string
---@param pos Position
---@param numsquads integer
---@return SGroupID
function Util_CreateSquadsAtPositionRandom(playerId, squadgroupName, squadBlueprint, pos, numsquads) end

--- Creates a given number of squads with a loadout at a position and adds them to a squadgroup.  The squad will spawn at a slightly random position in relation to the specified position.  The squadgroup will be created if it doesnt exist.
---
--- Use loadout of zero to create a squad at its min loadout.  Squads can never exceed their max loadout.
---
--- *Source: ScarUtil.scar*
---@param playerId PlayerID
---@param squadgroupName string
---@param squadBlueprint string
---@param pos Position
---@param numsquads integer
---@param loadout integer
---@return SGroupID
function Util_CreateSquadsAtPositionRandomEx(playerId, squadgroupName, squadBlueprint, pos, numsquads, loadout) end

--- Drop-pod an existing squadgroup to a position on the map
---
--- Works on spawned or depspawned group
---
--- *Source: ScarUtil.scar*
---@param sgroupname string
---@param pos Position
function Util_DropPod(sgroupname, pos) end

--- Drop-pod an existing squadgroup to a position on the map with no disruption
---
--- Works on spawned or depspawned group
---
--- *Source: ScarUtil.scar*
---@param sgroupname string
---@param pos Position
function Util_DropPod2(sgroupname, pos) end

--- Drop-pod an existing squadgroup to a marker on the map
---
--- Works on spawned or depspawned group
---
--- *Source: ScarUtil.scar*
---@param sgroupname string
---@param markername string
function Util_DropPodMarker(sgroupname, markername) end

--- Find all the entities with a given blueprint in sourcegroup and add them to destgroup.
---
--- destgroup will be created if it does not already exist.
---
--- *Source: ScarUtil.scar*
---@param sourcegroup string
---@param destgroup string
---@param bpname string
function Util_GetEntitiesByBP(sourcegroup, destgroup, bpname) end

--- Find all the squads with a given armour type in sourcegroup and add them to destgroup.
---
--- destgroup will be created if it does not already exist.  See Squad_HasArmourType for list of valid armour types.
---
--- *Source: ScarUtil.scar*
---@param sourcegroup string
---@param destgroup string
---@param armourtype string
function Util_GetSquadsByArmourType(sourcegroup, destgroup, armourtype) end

--- Find all the squads with a given blueprint in sourcegroup and add them to destgroup.
---
--- destgroup will be created if it does not already exist.
---
--- *Source: ScarUtil.scar*
---@param sourcegroup string
---@param destgroup string
---@param bpname string
function Util_GetSquadsByBP(sourcegroup, destgroup, bpname) end

--- Returns trailing numbers from a string, if it exists, nil otherwise. E.G. "marker23" would return 23.
---
--- *Source: ScarUtil.scar*
---@param val string
---@return number
function Util_GetTrailingNumber(val) end

--- Function takes multiple strings and returns a BlueprintTable ( used for SGroup_ContainsBlueprints )
---
--- Example:
---      local bpTable = Util_MakeBlueprintTable( "chaos_marine_squad", "chaos_marine_squad", "chaos_squad_bloodthirster" )
---      --Check if SGroup contains ALL items from the list (two chaos marines and a bloodthirster)
---      local result = SGroup_ContainsBlueprints( "sg_MyGroup", bpTable, true )
---
---      --Check if an SGroup contains ANY blueprints from the list     result = SGroup_ContainsBlueprints( "sg_MyGroup", bpTable, false )
---
---      If the first parameter is a table, it will append entries to the existing table
---
--- *Source: ScarUtil.scar*
---@param args any
---@return BlueprintTable
function Util_MakeBlueprintTable(args) end

--- Play an events file at a given markers location
---
--- You need to specify the path for the eventfile (e.g. "data:Art/Events/Guard/Abilities/Strafe_ordered_target")
---
--- *Source: ScarUtil.scar*
---@param markername string
---@param eventfile string
function Util_MarkerFX(markername, eventfile) end

--- Play the mission title fade.
---
--- *Source: ScarUtil.scar*
---@param loc_str LocString
function Util_MissionTitle(loc_str) end

--- Muffles / Unmuffles all background FX sounds for NIS's
---
--- *Source: ScarUtil.scar*
---@param mute boolean
function Util_MuffleNISSound(mute) end

--- Toggles all modifier event FX for the player
---
--- Use this function to make NIS's look clean without wierd icons above units' heads
---
--- *Source: ScarUtil.scar*
---@param titleid integer
function Util_NIS_ToggleModifierEvents(titleid) end

--- Toggles all modifier event FX for all players on the map
---
--- Use this function to make NIS's look clean without wierd icons above units' heads
---
--- *Source: ScarUtil.scar*
function Util_NIS_ToggleModifierEventsForAllPlayers() end

--- Marks the specified objective as completed
---
--- Use this function to complete an objective with the sound
---
--- *Source: ScarUtil.scar*
---@param titleid integer
function Util_ObjectiveComplete(titleid) end

--- Creates an objective and plays the objective created stinger to accompany it.
---
--- Use this function to create an objective with the sound
---
--- *Source: ScarUtil.scar*
---@param objectiveTable table
---@param primary boolean
function Util_ObjectiveCreate(objectiveTable, primary) end

--- Makes the specified objective as failed
---
--- Use this function to fail an objective with the sound
---
--- *Source: ScarUtil.scar*
---@param titleid integer
function Util_ObjectiveFail(titleid) end

--- Create a looping ping.
---
--- Works with the Util_Ping_Stop( ) to allow for the easy removal of looping pings without having to save pingID variables. Make up a pingname string and pass it in.
---
--- *Source: ScarUtil.scar*
---@param pingname string
---@param markername string
function Util_Ping_LoopingMkr(pingname, markername) end

--- Stops a looping ping
---
--- Works with the Util_Ping_LoopingMkr( ) to allow for the easy removal of looping pings without having to save pingID variables. Use the pingname to recall and stop a previously created looping ping
---
--- *Source: ScarUtil.scar*
---@param pingname string
function Util_Ping_Stop(pingname) end

--- Reset the contruction state for a building and order a builder group to start rebuilding the structure
---
--- The building group must contain one and only one structure
---
--- *Source: ScarUtil.scar*
---@param building_groupname string
---@param builder_groupname string
function Util_ResetAndRebuild(building_groupname, builder_groupname) end

--- Converts a 2D top down position to a 3D ScarPosition.
---
--- 3D ScarPositions have the x axis left to right, the z axis in to out, and the y axis down to up (y axis represents the height of the terrain).  Use this function to convert a top-down 2D position to a 3D world position.
---
---  Note: (0,0) is in the center of the map.
---
--- *Source: ScarUtil.scar*
---@param xpos number
---@param ypos number
---@return Position
function Util_ScarPos(xpos, ypos) end

--- Sets sounds properly for Intelevents.  set true for start of IE and false for end of IE
---
--- *Source: ScarUtil.scar*
---@param enter boolean
function Util_SetIE(enter) end

--- Play a Intel event.
---
--- This function should used instead of Event_Start because it handles priorities.  If an nislet is triggered during an intel event, the intel event will be cancelled, and if an intel event is triggered during a nislet, the intel event will be queued.
---
--- *Source: ScarUtil.scar*
---@param func function
function Util_StartIntel(func) end

--- Play a NISlet.
---
--- This function should used instead of Event_Start because it handles priorities.  If an nislet is triggered during an intel event, the intel event will be cancelled, and if an intel event is triggered during a nislet, the intel event will be queued.
---
--- *Source: ScarUtil.scar*
---@param func function
function Util_StartNIS(func) end

--- Summon an existing squadgroup to a position on the map
---
--- Works on spawned or depspawned group
---
--- *Source: ScarUtil.scar*
---@param sgroupname string
---@param pos Position
function Util_Summon(sgroupname, pos) end

--- Tracks a character and, if dead, drop pods a new one in.
---
--- Use this function to track whether the player has a force commander etc.  If ever that character dies, a replacement will drop pod in at the specified marker
---
--- *Source: ScarUtil.scar*
---@param playerid integer
---@param characterGroup string
---@param characterBlueprint string
---@param characterDropPodMarkerName string
function Util_TrackCharacterAndDropPodIn(playerid, characterGroup, characterBlueprint, characterDropPodMarkerName) end

-- from setup.scar

--- Initializes the all of the players settings in one function and returns the playerID.
---
--- In most cases you will call this function from OnGameSetup and store the returned playerId in  a global variable.  The player index should be in the range [0 , (numPlayers-1)].
---
---  Valid player races are the shortnames of the race attrib files.
---
---  Example:
---     function OnGameSetup( )
---         g_player1 = Setup_Player(0, "PlayerDisplayName", "ork_race", 1)
---      -- g_player2 and g_player3 will be on the same team (allies)
---      g_player2 = Setup_Player(1, "$20000", "space_marine_race", 2)
---      g_player3 = Setup_Player(2, "$20001", "eldar_race", 2)
---     end
---
--- *Source: Setup.scar*
---@param playerIndex integer
---@param playerName string
---@param playerRace string
---@param team integer
---@return PlayerID
function Setup_Player(playerIndex, playerName, playerRace, team) end

-- from timer.scar

--- Add the amount of time to the specified timer
---
--- *Source: Timer.scar*
---@param timerId integer
---@param period number
function Timer_Add(timerId, period) end

--- Display (in the console) the amount of time remaining in the specified timer.
---
--- *Source: Timer.scar*
---@param timerId integer
function Timer_Display(timerId) end

--- Stop the specified timer.
---
--- *Source: Timer.scar*
---@param timerId integer
function Timer_End(timerId) end

--- Get the remaining time for the specified timer.
---
--- *Source: Timer.scar*
---@param timerId integer
function Timer_GetRemaining(timerId) end

--- Pause the specified timer.
---
--- *Source: Timer.scar*
---@param timerId integer
function Timer_Pause(timerId) end

--- Resume the specified timer.
---
--- *Source: Timer.scar*
---@param timerId integer
function Timer_Resume(timerId) end

--- Start a timer with the specified id, taking the required period ( in seconds )
---
--- *Source: Timer.scar*
---@param timerId integer
---@param period number
function Timer_Start(timerId, period) end

-- from ui.scar

--- Add a new objective using an objective table.
---
--- Example obj_table:
---   obj_table = { title_id = 400009,    short_desc_id = 400010, help_tip_id = 400011 }
---
--- *Source: UI.scar*
---@param obj_table table
---@param is_primary_obj boolean
function Objective_Add(obj_table, is_primary_obj) end

--- Scar ping at a marker position. use ping_type 'attack' for attack pings otherwise default ping will be used
---
--- *Source: UI.scar*
---@param markername string
---@param is_looping boolean
---@param ping_type string
---@return PingID
function Ping_Marker(markername, is_looping, ping_type) end

--- Scar ping at a position.  Scar ping will show up on terrain and minimap.  use ping_type 'attack' for attack pings otherwise default ping will be used
---
--- *Source: UI.scar*
---@param pos Position
---@param is_looping boolean
---@param ping_type string
---@return PingID
function Ping_Position(pos, is_looping, ping_type) end

--- Use this to stop a looping ping
---
--- *Source: UI.scar*
---@param pingID PingID
function Ping_Stop(pingID) end

--- Prematurely hide/remove a countdown
---
--- *Source: UI.scar*
function UI_HideCountdown() end

--- Start a countdown displaying secsRemaining.  Timer will automagically countdown and hide once done.
---
--- Pass in a formatID with format 'Timer title: %1TIME%'
---
--- *Source: UI.scar*
---@param formatID integer
---@param secsRemaining number
function UI_ShowCountdown(formatID, secsRemaining) end

--------------------------------------------------------------------------------
-- Tier 2: Available via import("WXPScarUtil.scar")
--------------------------------------------------------------------------------

-- from wxpbutton.scar

--- Create and Ability Button
---
--- Must pass in a table of tga file references and localized string numbers.
---   Example:
---      local table1 = {"command_icons/stance_burn", "command_icons/stance_hold", "command_icons/stance_cease"}
---      "command_icons/stance_burn" = enabled icon, "command_icons/stance_hold" = disabled icon, "command_icons/stance_cease" = mouseover icon
---      local table2 = { "$830050", "$830051"}
---      "$830050" = title, "$830051" = description
---
---      g_buttonID1 = Util_SGroupAbilityButton_Create(table1, table2, false)
---
--- *Source: WXPButton.scar*
---@param textureTable table
---@param tooltipTable table
---@param enabled boolean
---@return ButtonID
function Util_AbilityButton_Create(textureTable, tooltipTable, enabled) end

--- Create and Modal Ability Button
---
--- Must pass in a table of tga file references and localized string numbers.
---   Example:
---      local table1 = {"command_icons/stance_burn", "command_icons/stance_hold", "command_icons/stance_cease"}
---      "command_icons/stance_burn" = enabled icon, "command_icons/stance_hold" = disabled icon, "command_icons/stance_cease" = mouseover icon
---      local table2 = { "$830050", "$830051"}
---      "$830050" = title, "$830051" = description
---
---      g_buttonID1 = Util_SGroupAbilityButton_Create(table1, table2, false)
---
--- *Source: WXPButton.scar*
---@param tooltipTable table
---@param enabled boolean
---@return ButtonID
function Util_AbilityButton_CreateModal(tooltipTable, enabled) end

--- Forces an update of all buttons in both the SGroup and EGroup button visibility managers
---
--- Calling this function should not be needed in most cases, and should be avoided because of performance overheads.
---
--- *Source: WXPButton.scar*
function Util_ButtonManagerUpdateAll() end

--- Add an Ability Button that you want on an EGROUP to the Visibility Manager
---
--- The function Util_EGroupAbilityButton_ManageVisible() must be added after this function is called for anything to happen.
---
--- *Source: WXPButton.scar*
---@param button ButtonID
---@param egroupname string
function Util_EGroupAbilityButton_Add2Manager(button, egroupname) end

--- Remove an Ability Button from the EGroup Visibility Manager
---
--- This will remove the button from the manager and hide it if it is visible.
---
--- *Source: WXPButton.scar*
---@param button ButtonID
function Util_EGroupAbilityButton_Remove(button) end

--- Add an Ability Button that you want in the No Selection Visibility Manager
---
--- The function Util_NoSelectionAbilityButton_ManageVisible() must be added after this function is called for anything to happen.
---
--- *Source: WXPButton.scar*
---@param button ButtonID
function Util_NoSelectionAbilityButton_Add2Manager(button) end

--- Manages the visibility of Ability Buttons conditional on the user having NO selection made.
---
--- This function should be added once with Rule_Add() or Rule_AddDelay() in order to avoid lag between having no selection and the button appearing.
---
--- *Source: WXPButton.scar*
function Util_NoSelectionAbilityButton_ManageVisible() end

--- Remove an Ability Button from the No Selection Visibility Manager
---
--- This will remove the button from the manager and hide it if it is visible.
---
--- *Source: WXPButton.scar*
---@param button ButtonID
function Util_NoSelectionAbilityButton_Remove(button) end

--- Add an Ability Button that you want on an SGROUP to the Visibility Manager
---
--- The function Util_SGroupAbilityButton_ManageVisible() must be added after this function is called for anything to happen.
---
--- *Source: WXPButton.scar*
---@param button ButtonID
---@param sgroupname string
function Util_SGroupAbilityButton_Add2Manager(button, sgroupname) end

--- Remove an Ability Button from the SGroup Visibility Manager
---
--- This will remove the button from the manager and hide it if it is visible.
---
--- *Source: WXPButton.scar*
---@param button ButtonID
function Util_SGroupAbilityButton_Remove(button) end

--- Create and Scar Button
---
--- Must pass in a valid widget name and table of tga file references and localized string numbers.
---   Example:
---      local table1 = {"command_icons/stance_burn", "command_icons/stance_hold", "command_icons/stance_cease"}
---      local table2 = { "$830050", "$830051"}
---
---      current acceptible widget names include : "btn_ScarUI"
---
---      g_buttonID1 = Util_ScarButton_Create("btn_ScarUI", table1, table2, true, false)
---
--- *Source: WXPButton.scar*
---@param widgetName string
---@param tooltipTable table
---@param visible boolean
---@param enabled boolean
---@return ButtonID
function Util_ScarButton_Create(widgetName, tooltipTable, visible, enabled) end

-- from wxpcommand.scar

--- The squadBlueprintToBuild is the name of the squad you want to build from the building.
---
--- *Source: WXPCommand.scar*
---@param egroupName string
---@param squadBlueprintToBuild string
function Cmd_BuildSquad(egroupName, squadBlueprintToBuild) end

--- Checks the egroup for all entities that can cast the ability and has those entities casts on themselves
---
--- *Source: WXPCommand.scar*
---@param groupName string
---@param abilityName string
---@return boolean
function Cmd_EGroupCastAbilitySelf(groupName, abilityName) end

--- Relocates every entity in the group that can relocate to the closest free spot withing 'searchRadius' meters of marker 'markerName'
---
--- *Source: WXPCommand.scar*
---@param groupName string
---@param markerName string
---@param searchRadius integer
function Cmd_EGroupRelocateMarker(groupName, markerName, searchRadius) end

--- Relocates every entity in the group that can relocate to the closest free spot within 'searchRadius' meters of 'position'.
---
--- *Source: WXPCommand.scar*
---@param groupName string
---@param position Position
---@param searchRadius integer
function Cmd_EGroupRelocatePosition(groupName, position, searchRadius) end

--- Checks the egroup for an entity that can cast the ability and casts it at the marker.   Returns false if no items in the group can cast.
---
--- *Source: WXPCommand.scar*
---@param egroupname string
---@param abilityname string
---@param targetmarker string
---@return boolean
function Cmd_EntityCastAbilityMarker(egroupname, abilityname, targetmarker) end

--- Checks the egroup for an entity that can cast the ability and casts it on the target pos.   Returns false if no items in the group can cast.
---
--- *Source: WXPCommand.scar*
---@param egroupname string
---@param abilityname string
---@param targetpos Position
---@return boolean
function Cmd_EntityCastAbilityPos(egroupname, abilityname, targetpos) end

--- Send a spawn slot A command to a harvester unit
---
--- Spawn the unit linked to the spawnslot of the harvester unit
---
--- *Source: WXPCommand.scar*
---@param sgroupname string
function Cmd_HarvestSpawnSlotA(sgroupname) end

--- Send a spawn slot A command to a harvester unit
---
--- Spawn the unit linked to the spawnslot of the harvester unit
---
--- *Source: WXPCommand.scar*
---@param sgroupname string
function Cmd_HarvestSpawnSlotB(sgroupname) end

--- Send a spawn slot A command to a harvester unit
---
--- Spawn the unit linked to the spawnslot of the harvester unit
---
--- *Source: WXPCommand.scar*
---@param sgroupname string
function Cmd_HarvestSpawnSlotC(sgroupname) end

--- The offset is a number from 0 to 7 and indicates a location around 'position' as follows:
--- 7   0   1
--- 6  pos  2
--- 5   4   3
---
--- *Source: WXPCommand.scar*
---@param groupName string
---@param position Position
---@param offset integer
---@param distance number
function Cmd_MoveToPosOffset(groupName, position, offset, distance) end

--- The the target SGroup can only have one squad in it. The offset is a number from 0 to 7 and indicates a location around the target sgroup (oriented based on that group's orientation) as follows:
--- 7   0   1
--- 6  pos  2
--- 5   4   3
---
--- *Source: WXPCommand.scar*
---@param groupName string
---@param targetGroupName string
---@param offset integer
---@param distance number
function Cmd_MoveToSGroupOffset(groupName, targetGroupName, offset, distance) end

--- Tell a squad to entrench.
---
--- The given unit should have an squad entrench extension.
---
--- *Source: WXPCommand.scar*
---@param sgroupname string
function Cmd_SquadEntrench(sgroupname) end

--- Tell a squad to uproot.
---
--- The given unit should have an squad entrench extension.
---
--- *Source: WXPCommand.scar*
---@param sgroupname string
function Cmd_SquadUproot(sgroupname) end

-- from wxpgroups.scar

--- Returns the sum of the current healths of all entities in the group.
---
--- *Source: WXPGroups.scar*
---@param groupName string
---@return number
function EGroup_GetTotalHealthCurrent(groupName) end

--- Returns the sum of the maximum healths of all entities in the group.
---
--- *Source: WXPGroups.scar*
---@param groupName string
---@return number
function EGroup_GetTotalHealthMax(groupName) end

--- Returns whether all or any of the entities in the group have the specified modifier applied to them.
---
--- If 'all' is true, all entities must have the modifier applied for the function to return true. If 'all' is false, at least one entity must have the modifier.
---
--- *Source: WXPGroups.scar*
---@param groupName string
---@param modifierType string
---@param all boolean
---@return boolean
function EGroup_HasModifier(groupName, modifierType, all) end

--- Returns whether all or any entities in the group are relocating
---
--- If 'all' is true, all entities must be relocating for the function to return true. If 'all' is false, at least one entity must be relocating.
---
--- *Source: WXPGroups.scar*
---@param groupName string
---@param all boolean
---@return boolean
function EGroup_IsRelocating(groupName, all) end

--- Uncaptures any captured strategic point or relic in the group
---
--- *Source: WXPGroups.scar*
---@param groupName string
function EGroup_Uncapture(groupName) end

--- Adds a leader at the given index to all squads in a group that can take a leader.
---
--- This function will bypass all cost and queue prereqs
---
--- *Source: WXPGroups.scar*
---@param sgroupname string
---@param leaderIndex integer
function SGroup_AddLeadersAtIndex(sgroupname, leaderIndex) end

--- Returns the sum of the current healths of all entities in all squads in the group.
---
--- *Source: WXPGroups.scar*
---@param groupName string
---@return number
function SGroup_GetTotalHealthCurrent(groupName) end

--- Returns the sum of the maximum healths of all entities in all squads in the group.
---
--- *Source: WXPGroups.scar*
---@param groupName string
---@return number
function SGroup_GetTotalHealthMax(groupName) end

--- Returns true if all or any squads in the sgroup are in the hold of any of the buildings in the buildings egroup
---
--- It is not required that all (or even any) buildings in the buildings group have a hold
---
--- *Source: WXPGroups.scar*
---@param groupName string
---@param buildingsGroupName string
---@param all boolean
---@return boolean
function SGroup_IsInHold(groupName, buildingsGroupName, all) end

--- Respawns the group at a location and triggers the event file at the location of every entity belonging to every squad in the group
---
--- Example: SGroup_ReSpawnWithEffect("my_group", "mymarker", "data:Art/Events/Unit_Upgrade_Morale_FX/Reinforce_Trooper")
---
--- *Source: WXPGroups.scar*
---@param groupName string
---@param markername string
---@param eventFilename string
function SGroup_ReSpawnMarkerWithEffect(groupName, markername, eventFilename) end

-- from wxpmetamap.scar

--- Returns a list of starting entities a race will currently be allocated.
---
--- This function will throw an exception if called outside of a Meta Map game, an unknown race is specified or an out of range index is given.
---
--- *Source: WXPMetaMap.scar*
---@param startingEntityNameList table
function MetaMap_GetAttackingRaceStartingEntitiesList(startingEntityNameList) end

--- Returns a list of starting squads a race will currently be allocated.
---
--- This function will throw an exception if called outside of a Meta Map game, an unknown race is specified or an out of range index is given.
---
--- *Source: WXPMetaMap.scar*
---@param raceName string
---@param startingSquadNameList table
function MetaMap_GetRaceStartingSquadsList(raceName, startingSquadNameList) end

-- from wxprestrict.scar

--- Applies restrictions for space marines.  You should call this at the begining of your mission for Player 1.
---
--- *Source: WXPRestrict.scar*
---@param level integer
function WXP_Restrict(level) end

-- from wxpscarutil.scar

--- Positions the camera view at the start of the first path so the game can fade up to a 'ready' shot. MUST be used first before any other CPath's are started. Should use CPath_CutToPath() to follow up.
---
--- *Source: WXPScarUtil.scar*
---@param canimName string
---@param startKey string
---@param endKey string
function CPath_PresetCamera(canimName, startKey, endKey) end

--- Returns whether all or any entities in the group are shrouded
---
--- If 'all' is true, all entities must be shrouded for the function to return true. If 'all' is false, at least one entity must be shrouded.
---
--- *Source: WXPScarUtil.scar*
---@param groupName string
---@param all boolean
---@return boolean
function EGroup_IsShrouded(groupName, all) end

--- Takes the given egroupname and issues the 'Destroy' command to all entities. It should behave the same as if the player hit the delete key for each member.
---
--- To absolutely kill entities, this should be used instead of either EGroup_SetAvgHealth(egroupname, 0) or EGroup_DestroyAllEntities(egroupname) as both of these do not always accomplish the desired effect
---
--- *Source: WXPScarUtil.scar*
---@param egroupname string
---@param spawnedOnly boolean
function EGroup_SelfDestroy(egroupname, spawnedOnly) end

--- Enables or disables the shroud ability for all entities in the group that have the ability
---
--- Shroud will be turned on if 'enable' is true and turned off when it is false
---
--- *Source: WXPScarUtil.scar*
---@param groupName string
---@param enable boolean
function EGroup_Shroud(groupName, enable) end

--- Unreveals an area the size of a given markers proximity at that markers position.  YOU SHOULD ONLY CALL THIS IF YOU HAVE USED FOW_RevealMarker ON THAT MARKER PREVIOUSLY.
---
--- This function will unreveal the FOW for ALL alive players.
---
--- *Source: WXPScarUtil.scar*
---@param markername string
function FOW_UnRevealMarker(markername) end

--- Sets the Requisition, Power and Pop for the player. Pass in a 'nil' value instead of a Real amount to skip the associated Resource Type. Ignores income cap and resource sharing.
---
--- Leaving one Real amount off will automatically skip setting RT_Pop. Leaving two Real amounts off will skip RT_Power and RT_Pop.
---
--- *Source: WXPScarUtil.scar*
---@param player PlayerID
---@param req number
---@param power number
---@param pop number
function Player_SetAllResources(player, req, power, pop) end

--- Adds a set of tracks to the requested playlist channel, sets how the next track to be played will be chosen, ordered (true) or random (false), and sets silence intervals between tracks for the requested playlist channel.
---
--- Example:
---      Valid sound channels are either PC_Music or PC_Ambient
---      This utility function should be called once per channel
---
---      local tracksTable = {"music_track1", "music_track2"}
---      Must use valid music or ambient track names from the \Warhammer\DataGeneric\W40k\Sound\Music or \Warhammer\DataGeneric\WXP\Sound\Music directory
---
---      local silenceTable = {5, 20}
---      This example would set a random time of as little as 5 seconds but no more than 20 seconds between each track.
---
---      The function call could look something like:
---      t_music = {"music_theme_chaos_01", "music_ork_theme", "music_coming_of_chaos" }
---      Playlist_Manager( PC_Music, t_music, true , {20, 40})
---
--- *Source: WXPScarUtil.scar*
---@param channel SoundChannel
---@param tracksTable table
---@param sequential boolean
---@param looping boolean
---@param silenceTable table
function Playlist_Manager(channel, tracksTable, sequential, looping, silenceTable) end

--- Checks to see if a Rule exists. If it does not exist, the rule will be added to be executed at every 'interval' seconds
---
--- *Source: WXPScarUtil.scar*
---@param f function
---@param interval number
function Rule_Check_AddInterval(f, interval) end

--- Checks to see if a Rule exists. If it does not exist, the rule will be added to be executed  'calls' times, at every 'interval' seconds
---
--- *Source: WXPScarUtil.scar*
---@param f function
---@param interval number
---@param calls integer
function Rule_Check_AddIntervalEx(f, interval, calls) end

--- Checks to see if a Rule exists. If it does not exist, the rule will be added to be executed once, after 'interval' seconds
---
--- *Source: WXPScarUtil.scar*
---@param f function
---@param interval number
function Rule_Check_AddOneShot(f, interval) end

--- Checks to see if a Rule exists. If it does, it will be deleted. If it doesn't there will be no result.
---
--- *Source: WXPScarUtil.scar*
---@param f function
function Rule_Check_Remove(f) end

--- Pass in a string of a sgroup. If the group exists, any squads within that group will be destroyed. If the group does not exist or if the group is empty there will be no result.
---
--- The sgroupname is not deleted, it will still exist once the members are destroyed
---
--- *Source: WXPScarUtil.scar*
---@param squadgroupName string
function SGroup_Check_DestroyAllSquads(squadgroupName) end

--- Takes the given sgroupname and issues the 'Destroy' command to all squads. It should behave the same as if the player hit the delete key for each member.
---
--- To absolutely kill squads, this should be used instead of either SGroup_SetAvgHealth(sgroupname, 0) or SGroup_DestroyAllSquads(sgroupname) as both of these do not always accomplish the desired effect
---
--- *Source: WXPScarUtil.scar*
---@param sgroupname string
---@param spawnedOnly boolean
function SGroup_SelfDestroy(sgroupname, spawnedOnly) end

--- Hide and remove a counter
---
--- *Source: WXPScarUtil.scar*
---@param counterName string
function UI_HideCount(counterName) end

--- Hide and remove a ratio
---
--- *Source: WXPScarUtil.scar*
---@param ratioName string
function UI_HideRatio(ratioName) end

--- Create a counter with a displayed value. Assign a player and the formatID (aka. Localized String value )
---
--- Example: UI_ShowCount( "ct_test", g_Player1, 861016, 5 )
---
--- *Source: WXPScarUtil.scar*
---@param counterName string
---@param player PlayerID
---@param formatID integer
---@param initialValue number
function UI_ShowCount(counterName, player, formatID, initialValue) end

--- Create a counter with a displayed value. Assign a player and the formatID (aka. Localized String value )
---
--- Example: UI_ShowCount( "ct_test", g_Player1, 861016, 5 )
---
--- *Source: WXPScarUtil.scar*
---@param counterName string
---@param player PlayerID
---@param formatID integer
---@param initialValue number
function UI_ShowCountDXP(counterName, player, formatID, initialValue) end

--- Update the value of a counter. Pass in the name and the new value for the counter to display
---
--- *Source: WXPScarUtil.scar*
---@param counterName string
---@param countValue number
function UI_ShowCountUpdate(counterName, countValue) end

--- Update the value of a counter. Pass in the name  the new value for the counter to display and the formatID
---
--- *Source: WXPScarUtil.scar*
---@param counterName string
---@param countValue number
function UI_ShowCountUpdateDxp(counterName, countValue) end

--- Create a ratio with displayed values. Assign a player and the formatID (aka. Localized String value )
---
--- Example: UI_ShowRatioDxp( "ratio_test", g_Player1, 38025, 4, 5 )
---
--- *Source: WXPScarUtil.scar*
---@param counterName string
---@param player PlayerID
---@param formatID integer
---@param value1 number
---@param value2 number
function UI_ShowRatio(counterName, player, formatID, value1, value2) end

--- Update the values of a ratio. Pass in the name, the formatID (aka. Localized String value) and the new values for the ratio to display
---
--- *Source: WXPScarUtil.scar*
---@param ratioName string
---@param formatID integer
---@param value1 number
---@param value2 number
function UI_ShowRatioUpdate(ratioName, formatID, value1, value2) end

--- Gets units to attack the ground at a random position within the radius of a marker
---
--- Use this function to create controlled random ground attacks to make units appear more lifelike
---
--- *Source: WXPScarUtil.scar*
---@param sgroupname string
---@param markername string
function Util_AttackGroundMarkerRandomRadius(sgroupname, markername) end

--- Gets units moving to a random position within the radius of a marker
---
--- Use this function to create controlled random movement to make units appear more lifelike
---
--- *Source: WXPScarUtil.scar*
---@param sgroupname string
---@param markername string
function Util_AttackMoveMarkerRandomRadius(sgroupname, markername) end

--- Create an Objective Beacon FX at a marker
---
--- *Source: WXPScarUtil.scar*
---@param markerName string
---@return integer
function Util_CreateBeaconMarker_Blue(markerName) end

--- Create an Objective Beacon FX at a marker
---
--- *Source: WXPScarUtil.scar*
---@param markerName string
---@return integer
function Util_CreateBeaconMarker_Red(markerName) end

--- Create an Objective Beacon FX at a position
---
--- *Source: WXPScarUtil.scar*
---@param position Position
---@return integer
function Util_CreateBeaconPosition_Blue(position) end

--- Create an Objective Beacon FX at a position
---
--- *Source: WXPScarUtil.scar*
---@param position Position
---@return integer
function Util_CreateBeaconPosition_Red(position) end

--- Plays the non looping piece of music once through, then, after delay, plays the looping piece of music, looping.
---
--- Use this function to setup detailed playlists in missions.
---
--- *Source: WXPScarUtil.scar*
---@param non_loopingTracks table
---@param loopingTracks table
---@param delay integer
function Util_Music_CreateSingleLoop(non_loopingTracks, loopingTracks, delay) end

--- Create a looping ping.
---
--- Works with the Util_Ping_Stop( ) to allow for the easy removal of looping pings without having to save pingID variables. Make up a pingname string and pass it in.
---
--- *Source: WXPScarUtil.scar*
---@param pingname string
---@param markername string
function Util_Ping_LoopingPos(pingname, markername) end

--- Removes an existing Objective Beacon - colour doesn't matter (*snicker*)
---
--- *Source: WXPScarUtil.scar*
---@param beaconID integer
function Util_RemoveBeaconMarker(beaconID) end

--- Sets the blood level for every blood pit in the group
---
--- The level must be in the [0.0, 1.0] range
---
--- *Source: WXPScarUtil.scar*
---@param bloodpitGroupName string
---@param level number
function Util_SetBloodPitLevel(bloodpitGroupName, level) end

--- Creates an objective and plays the objective created stinger to accompany it, and flashes the objective button.
---
--- Use this function to create an objective with the sound
---
--- *Source: WXPScarUtil.scar*
---@param objectiveTable table
---@param primary boolean
---@param playerID integer
function Util_WXP_ObjectiveCreate(objectiveTable, primary, playerID) end

--- Creates a sub objective and plays the objective created stinger to accompany it, and flashes the objective button. Pass in the objective ID of the 'parent' objective
---
--- Use this function to create an objective with the sound
---
--- *Source: WXPScarUtil.scar*
---@param objectiveTable table
---@param parentObjectiveID integer
function Util_WXP_ObjectiveSubCreate(objectiveTable, parentObjectiveID) end

--- Adds all squads belonging to all players into a group. Removes any from the group that are despawned and destroys whatever is left.
---
--- Should only be used when cleaning up, as this will destroy EVERY spawned squad in the entire game.
---
--- *Source: WXPScarUtil.scar*
function WXP_KillAllPlayersSpawnedSquads() end

--- Adds all squads belonging to all players into a group. Removes any from the group that are despawned or in the indicated sgroup and destroys whatever is left.
---
--- Should only be used when cleaning up, as this will destroy EVERY spawned squad in the entire game... except the one that you ask it not to.
---
--- *Source: WXPScarUtil.scar*
---@param squadgroupName string
function WXP_KillAllPlayersSpawnedSquadsExcept(squadgroupName) end

--- Turns off event cues, sets the viewable space to faded out, and presets the UI into letterbox mode
---
--- *Source: WXPScarUtil.scar*
function WXP_OpeningNISPreset() end

-- from wxpui.scar

--- Add a new objective using an objective table and associates it with a specific player
---
--- Example obj_table:
---   obj_table = { title_id = 400009,    short_desc_id = 400010, help_tip_id = 400011 }
---
--- *Source: WXPUI.scar*
---@param obj_table table
---@param is_primary_obj boolean
---@param player PlayerID
function Objective_AddPlayer(obj_table, is_primary_obj, player) end

--- Associates a ping with an objective
---
--- The ping will be shown at the specified marker on the mini map in the objectives dialog when the objective it is associated with is selected. Use ping_type 'attack' for attack pings, otherwise default ping will be used.
---
--- *Source: WXPUI.scar*
---@param title_id integer
---@param marker_name string
---@param is_looping boolean
---@param ping_type string
---@return PingID
function Objective_PingMarker(title_id, marker_name, is_looping, ping_type) end

--- Associates a ping with an objective
---
--- The ping will be shown at the specified position on the mini map in the objectives dialog when the objective it is associated with is selected. Use ping_type 'attack' for attack pings, otherwise default ping will be used.
---
--- *Source: WXPUI.scar*
---@param title_id integer
---@param position Position
---@param is_looping boolean
---@param ping_type string
---@return PingID
function Objective_PingPosition(title_id, position, is_looping, ping_type) end

--- Add a new sub objective using an objective table and associates it with the specified parent objective
---
--- Please note the only one level of sub objectives is supported (i.e.  an objective cannot be added as a sub objective of an existing sub objective)
---
---   Example obj_table:
---  obj_table = { title_id = 400009, short_desc_id = 400010, help_tip_id = 400011 }
---
--- *Source: WXPUI.scar*
---@param obj_table table
---@param parent_id integer
function Objective_SubAdd(obj_table, parent_id) end

--------------------------------------------------------------------------------
-- Standalone: Requires explicit import()
--------------------------------------------------------------------------------

-- from balancetool.scar — requires import("BalanceTool.scar")

--- Creates the maximum number of squads of this loadout given the resources at a marker position and adds them to a squadgroup.  The squadgroup will be created if it doesnt exist.
---
--- Loadout will be clipped to unitMin and unitMax from the squad blueprint.
---
--- *Source: BalanceTool.scar*
---@param playerId PlayerID
---@param squadgroupName string
---@param squadBlueprint string
---@param load_out_table table
---@param markername string
---@param requisition_amount number
---@param power_amount number
---@return SGroupID
function Balance_CreateSquadsAtMarkerByResource(playerId, squadgroupName, squadBlueprint, load_out_table, markername, requisition_amount, power_amount) end

--- Set the Lua Function used to initialize each Balance Tool iteration.
---
--- The function is used to set up the forces that will battle
---
--- *Source: BalanceTool.scar*
---@param func function
function Balance_SetInit(func) end

-- from camerashake.scar — requires import("CameraShake.scar")

--- Trigger a crazy big camera shake
---
--- *Source: CameraShake.scar*
---@param epicentre Position
---@param duration number
function CameraShake_Large(epicentre, duration) end

--- Trigger a regular camera shake
---
--- *Source: CameraShake.scar*
---@param epicentre Position
---@param duration number
function CameraShake_Medium(epicentre, duration) end

--- Trigger a small camera shake
---
--- *Source: CameraShake.scar*
---@param epicentre Position
---@param duration number
function CameraShake_Small(epicentre, duration) end

--------------------------------------------------------------------------------
-- Engine-only: C++ built-ins (no .scar wrapper)
--------------------------------------------------------------------------------

-- from LuaAnimator.cpp

--- Enables / disables the random infantry offset for all spawned and depsawned units in an sgroup.
---
--- You can use this function for the close up shots of your nis' to ensure that the units are in    the same position that you place them in the ME.  When you first disable the infantry offset, the units will   snap to their original positions so it might be a good idea to hide this behaviour from the user.  When re-enabling    this offset, the units will stay in their current positions until they move (they will not snap, so it should be ok    to call this while the unit is in view)
---
---   NOTE: If you disable this: MAKE SURE YOU RE-ENABLE IT, otherwise the units will not use random offsets ingame.
---
--- *Source: LuaAnimator.cpp*
---@param sgroup SGroupID
---@param enable boolean
function Anim_EnableInfantryOffset(sgroup, enable) end

---
--- *Source: LuaAnimator.cpp*
---@param egroup EGroupID
---@param animname string
function Anim_PlayEGroupAnim(egroup, animname) end

--- Plays an animation from an entity.
---
--- This function can be called through a CTRL object in NISlets to wait on the animation.
---
--- *Source: LuaAnimator.cpp*
---@param entity EntityID
---@param animname string
function Anim_PlayEntityAnim(entity, animname) end

---
--- *Source: LuaAnimator.cpp*
---@param sgroup SGroupID
---@param animname string
function Anim_PlaySGroupAnim(sgroup, animname) end

--- Trigger an anim for all troopers in a squad that are capable of playing the anim
---
--- *Source: LuaAnimator.cpp*
---@param squad SquadID
---@param animname string
function Anim_PlaySquadAnim(squad, animname) end

--- Find the first entity in the attacking squad group and attack the first entity in the victim squad group capable of playing the anim.
---
--- Example: Anim_PlaySyncAnim( 'sg_force_commander', 'sg_blood_thirster', 'SyncKill_7', 'SyncDeath_7'
---   Also, you will want to make sure the two squads are close to each other since the sync kill animation is going to 'pop' the units into place.
---
--- *Source: LuaAnimator.cpp*
---@param attacker SGroupID
---@param victim SGroupID
---@param killanim string
---@param deathanim string
---@param distance number
function Anim_PlaySyncAnim(attacker, victim, killanim, deathanim, distance) end

---
--- *Source: LuaAnimator.cpp*
---@param egroup EGroupID
---@param animname string
function Anim_StopEGroupAnim(egroup, animname) end

--- Force an animation that was started with Anim_PlayEntityAnim to stop if it is still running
---
--- *Source: LuaAnimator.cpp*
---@param entity EntityID
---@param animname string
function Anim_StopEntityAnim(entity, animname) end

---
--- *Source: LuaAnimator.cpp*
---@param sgroup SGroupID
---@param animname string
function Anim_StopSGroupAnim(sgroup, animname) end

--- Force all animations that were started with Anim_PlaySquadAnim to stop if they are still running
---
--- *Source: LuaAnimator.cpp*
---@param squad SquadID
---@param animname string
function Anim_StopSquadAnim(squad, animname) end

-- from LuaBlueprint.cpp

--- Returns true if an entity blueprint exists with the given name.
---
--- *Source: LuaBlueprint.cpp*
---@param name string
---@return boolean
function EBP_Exists(name) end

--- Returns true if a squad blueprint exists with the given name.
---
--- *Source: LuaBlueprint.cpp*
---@param name string
---@return boolean
function SBP_Exists(name) end

-- from LuaCamera.cpp

--- Activate the specified camera within the entity.
---
--- You can layer this on other Camera_ActivateEntityCam calls.
---   camera name - name of the camera in the model
---  shake amount - amount of shake to allow (0-1.0)
---  IMPORTANT: Blend time will have unpredictable results if blending to a moving entity or the camera is animated within the blend time.  Use with EXTREME caution!
---
--- *Source: LuaCamera.cpp*
---@param pEntity EntityID
---@param cameraName string
---@param shakeAmount number
---@return boolean
function Camera_ActivateEntityCam(pEntity, cameraName, shakeAmount) end

--- Resets the camera to default view
---
--- *Source: LuaCamera.cpp*
function Camera_DefaultView() end

--- Disable any playing entity cameras started with Camera_ActivateEntityCam
---
--- blend time - time to blend to default camera
---
--- *Source: LuaCamera.cpp*
---@param blendTime number
---@return boolean
function Camera_DisableEntityCam(blendTime) end

--- Print current camera settings.
---
--- *Source: LuaCamera.cpp*
function Camera_Dump() end

--- Focus the camera on a target enitity group.
---
--- This function MUST be called through a CTRL object in NISlets if secs is greater then zero.
---
--- *Source: LuaCamera.cpp*
---@param egroupID EGroupID
---@param secs number
function Camera_FocusOnTargetEGroup(egroupID, secs) end

--- Focus the camera on a target position.
---
--- This function MUST be called through a CTRL object in NISlets if secs is greater then zero.
---   IMPORTANT: This function does not lock user input.  If this function is called through a CTRL Camera_SetInputEnabled(false) should almost certainly be called!
---
--- *Source: LuaCamera.cpp*
---@param targetpos Position
---@param secs number
function Camera_FocusOnTargetPos(targetpos, secs) end

--- Focus the camera on a target squad group.
---
--- This function MUST be called through a CTRL object in NISlets if secs is greater then zero.
---
--- *Source: LuaCamera.cpp*
---@param sgroupID SGroupID
---@param secs number
function Camera_FocusOnTargetSGroup(sgroupID, secs) end

--- Returns the enabled/disabled state of the camera input.
---
--- *Source: LuaCamera.cpp*
---@return boolean
function Camera_GetInputEnabled() end

--- Returns true if an entity is on screen
---
--- IMPORTANT: This function does not check the FOW.  It will return true   even if the item is not revealed in FOW
---
--- *Source: LuaCamera.cpp*
---@param entity EntityID
---@return boolean
function Camera_IsEntityOnScreen(entity) end

--- Returns true if the camera is focused on and set to follow an entity (apostrophe command)
---
--- *Source: LuaCamera.cpp*
---@return boolean
function Camera_IsInFollowMode() end

--- Returns true if any entities in the squad are on screen
---
--- IMPORTANT: This function does not check the FOW.  It will return true   even if the item is not revealed in FOW
---
--- *Source: LuaCamera.cpp*
---@param squad SquadID
---@return boolean
function Camera_IsSquadOnScreen(squad) end

--- Activate the specified camera within the entity.
---
--- You can layer this on other Camera_SGActivateEntityCam calls.
---   camera name - name of the camera in the model
---  shake amount - amount of shake to allow (0-1.0)
---  IMPORTANT: Blend time will have unpredictable results if blending to a moving entity or the camera is animated within the blend time.  Use with EXTREME caution!
---
--- *Source: LuaCamera.cpp*
---@param pSGroup SGroupID
---@param cameraName string
---@param shakeAmount number
function Camera_SGActivateEntityCam(pSGroup, cameraName, shakeAmount) end

--- Disable any playing entity cameras started with Camera_SGActivateEntityCam
---
--- blend time - time to blend to default camera
---
--- *Source: LuaCamera.cpp*
---@param blendTime number
function Camera_SGDisableEntityCam(blendTime) end

--- Set the camera's declination immediately
---
--- Specify declination as a value between 0-360 degrees  IMPORTANT: This function has the potential to affect the camera's declination as well.
---
--- *Source: LuaCamera.cpp*
---@param declination number
function Camera_SetDeclination(declination) end

--- Set the camera's zoom distance (in metres) immediately
---
--- IMPORTANT: This function has the potential to affect the camera's declination as well.
---
--- *Source: LuaCamera.cpp*
---@param distance number
function Camera_SetDistance(distance) end

--- Enables / Disables camera input.
---
--- Disable the camera input to block all camera commands from the game.
---
--- *Source: LuaCamera.cpp*
---@param enabled boolean
function Camera_SetInputEnabled(enabled) end

--- Set the camera's orbit immediately
---
--- Specify orbit as a value between 0-360 degrees  IMPORTANT: This function has the potential to affect the camera's declination as well.
---
--- *Source: LuaCamera.cpp*
---@param orbit number
function Camera_SetOrbit(orbit) end

--- Set the camera's target position immediately
---
--- IMPORTANT: This function has the potential to affect the camera's declination as well.
---
--- *Source: LuaCamera.cpp*
---@param target Position
function Camera_SetTarget(target) end

--- Shake that hot camera!
---
--- epicentre - origin of camera shake.  as you move away from this origin, the shake is less intense.
---   timeSecs - duration of the shake
---  intesity - intesity of the shake.
---  radius - the extents of the shake from the epicentre
---  falloff - the amount of shake that the camera loses as it gets further from the epicentre
---
---  NOTE: You should not call this function directly from your mission scripts.  You should use the functions in CameraShake.scar
---
--- *Source: LuaCamera.cpp*
---@param epicentre Position
---@param timeSecs number
---@param intensity number
---@param radius number
---@param falloff number
function Camera_Shake(epicentre, timeSecs, intensity, radius, falloff) end

-- from LuaCommand.cpp

--- Construct a building at a location
---
--- *Source: LuaCommand.cpp*
---@param player PlayerID
---@param builders SGroupID
---@param ebpname string
---@param pos Position
function Command_ConstructBuilding(player, builders, ebpname, pos) end

--- Send a entity command to a entity group(CMD_DefaultAction, CMD_Stop, CMD_Destroy, CMD_BuildSquad, CMD_CancelProduction, CMD_RallyPoint)
---
--- Entity commands are mostly used for buildings etc.  If you need to issue commands to units, use the Squad_Command function.
---
--- *Source: LuaCommand.cpp*
---@param player PlayerID
---@param egroup EGroupID
---@param entityCommand integer
function Command_Entity(player, egroup, entityCommand) end

--- Tries to have the entity cast the ability on a position.
---
--- Returns false if the entity cannot cast the ability.
---
--- *Source: LuaCommand.cpp*
---@param player PlayerID
---@param caster EntityID
---@param abilityName string
---@param pos Position
---@return boolean
function Command_EntityAbilityPos(player, caster, abilityName, pos) end

--- Tries to have the entity cast the ability on an SGroup
---
--- Returns false if the entity cannot cast the ability.
---
--- *Source: LuaCommand.cpp*
---@param player PlayerID
---@param caster EntityID
---@param abilityName string
---@param target SGroupID
---@return boolean
function Command_EntityAbilitySGroup(player, caster, abilityName, target) end

--- Tries to have the entity cast the ability on itself.
---
--- Returns false if the entity cannot do the ability.
---
--- *Source: LuaCommand.cpp*
---@param player PlayerID
---@param caster EntityID
---@param abilityName string
---@return boolean
function Command_EntityAbilitySelf(player, caster, abilityName) end

--- Command the entity to build a certain squad blueprint.
---
--- Entity commands are mostly used for buildings etc. Used when ordering to build something from a building
---
--- *Source: LuaCommand.cpp*
---@param player PlayerID
---@param egroup EGroupID
function Command_EntityBuild(player, egroup) end

--- Send a entity-based command to an entity group.
---
--- Use this function to issue orders that require a entity to an entity group (eg. order a building to attack another building)
---  See Command_Entity for a list of all the possible entityCommands.
---
--- *Source: LuaCommand.cpp*
---@param player PlayerID
---@param egroup EGroupID
---@param entityCommand integer
---@param target EGroupID
function Command_EntityEntity(player, egroup, entityCommand, target) end

--- Send a position command to an entity group.
---
--- Use this function to issue orders that require a position to an entity group (eg. set a rally point for a building)
---  See Command_Entity for a list of all the possible entityCommands.
---
--- *Source: LuaCommand.cpp*
---@param player PlayerID
---@param egroup EGroupID
---@param entityCommand integer
---@param target Position
function Command_EntityPos(player, egroup, entityCommand, target) end

--- Relocates a building to the closest free spot to 'position'.
---
--- A free spot will be searched for only within 'searchRadius' meters of 'position'.
---
--- *Source: LuaCommand.cpp*
---@param player PlayerID
---@param building EntityID
---@param position Position
---@param searchRadius integer
function Command_EntityRelocate(player, building, position, searchRadius) end

--- Send a squad-based command to an entity group.
---
--- Use this function to issue orders that require a squad to an entity group (eg. order a building to attack a squad)
---  See Command_Entity for a list of all the possible entityCommands.
---
--- *Source: LuaCommand.cpp*
---@param player PlayerID
---@param egroup EGroupID
---@param entityCommand integer
---@param target SGroupID
function Command_EntitySquad(player, egroup, entityCommand, target) end

--- Returns true if the given ssquad command has been issued in the last num of seconds.
---
--- See Command_Entity for possible cmd types
---
--- *Source: LuaCommand.cpp*
---@param cmd integer
---@param secs number
---@return boolean
function Command_HasBeenIssued(cmd, secs) end

--- Returns true if the given ssquad command has been issued in the last num of seconds.
---
--- See Command_Squad for possible squadCmd types
---
--- *Source: LuaCommand.cpp*
---@param squadCmd integer
---@param secs number
---@return boolean
function Command_HasBeenIssuedSquad(squadCmd, secs) end

--- Send a squad command to a squad group
---
--- SCMD_DefaultAction, SCMD_Move, SCMD_Stop, SCMD_Destroy,  SCMD_BuildStructure, SCMD_Capture, SCMD_Attack, SCMD_ReinforceTrooper, SCMD_ReinforceLeader,  SCMD_UpgradeTrooper, SCMD_CancelReinforcement, SCMD_AttackGround, SCMD_Melee, SCMD_Stance,  SCMD_StanceMelee, SCMD_AttackMove, SCMD_Jump, SCMD_Ability, SCMD_Attach, SCMD_Detach, SCMD_ReinforceTrooperOverwatch, SCMD_UpgradeTrooperOverwatch, SCMD_SetInfiltrating, SCMD_Repair, SCMD_Load, SCMD_Unload, SCMD_Rampage, SCMD_UnloadHere SCMD_DeployWeapon, SCMD_UndeployWeapon
---
--- *Source: LuaCommand.cpp*
---@param player PlayerID
---@param sgroup SGroupID
---@param squadCommand integer
function Command_Squad(player, sgroup, squadCommand) end

--- Tries to cast ability on a target sgroup.  Returns false if nothing can cast the ability
---
--- *Source: LuaCommand.cpp*
---@param player PlayerID
---@param caster SquadID
---@param abilityname string
---@param target EGroupID
---@return boolean
function Command_SquadAbilityEGroup(player, caster, abilityname, target) end

--- Casts an ability at a location.  Returns false if nothing can cast the ability
---
--- *Source: LuaCommand.cpp*
---@param player PlayerID
---@param caster SquadID
---@param abilityname string
---@param pos Position
---@return boolean
function Command_SquadAbilityPos(player, caster, abilityname, pos) end

--- Tries to cast ability on a target sgroup.  Returns false if nothing can cast the ability
---
--- *Source: LuaCommand.cpp*
---@param player PlayerID
---@param caster SquadID
---@param abilityname string
---@param target SGroupID
---@return boolean
function Command_SquadAbilitySGroup(player, caster, abilityname, target) end

--- Tries to cast ability on self.  Returns false if nothing can cast the ability
---
--- *Source: LuaCommand.cpp*
---@param player PlayerID
---@param caster SquadID
---@param abilityname string
---@return boolean
function Command_SquadAbilitySelf(player, caster, abilityname) end

--- Send an entity command to a squad group.
---
--- Use this function to isssue an entity-based command to a squad group. (eg. order a squad to attack a building)
---  See Command_Squad for a list of all the possible squadCommands
---
--- *Source: LuaCommand.cpp*
---@param player PlayerID
---@param sgroup SGroupID
---@param squadCommand integer
---@param target EGroupID
function Command_SquadEntity(player, sgroup, squadCommand, target) end

--- Send a squad command to a squad group with custom data
---
--- *Source: LuaCommand.cpp*
---@param player PlayerID
---@param sgroup SGroupID
---@param squadCommand integer
---@param cmdparam integer
---@param cmdflags integer
function Command_SquadExt(player, sgroup, squadCommand, cmdparam, cmdflags) end

--- Send a position command to a squad group.
---
--- Use this function to issue orders that require a position to a squad group (eg. order a squad to move to position, or attack position)
---  See Command_Squad for a list of all the possible squadCommands.
---
--- *Source: LuaCommand.cpp*
---@param player PlayerID
---@param sgroup SGroupID
---@param squadCommand integer
---@param target Position
function Command_SquadPos(player, sgroup, squadCommand, target) end

--- Send an squad-based command to a squad group.
---
--- Use this function to isssue a squad-based command to a squad group. (eg. order a squad to attack another squad)
---  See Command_Squad for a list of all the possible squadCommands
---
--- *Source: LuaCommand.cpp*
---@param player PlayerID
---@param sgroup SGroupID
---@param squadCommand integer
---@param target SGroupID
function Command_SquadSquad(player, sgroup, squadCommand, target) end

-- from LuaCpuInterface.cpp

--- Assigns a cpu player to a marker to gather forces with a priority
---
--- The preference works in an additive way. Thus, if the preferences are:
---  Marker1:    10
---  Marker2:    20
---  Marker3:    5
---  Then the probability that the next unit built gathers at marker3 is 5/(10+20+5) = ~14% chance
---
--- *Source: LuaCpuInterface.cpp*
---@param player PlayerID
---@param marker MarkerID
---@param preference number
function Cpu_AssignWaypoint(player, marker, preference) end

--- Removes all the building limits set by Cpu_SetBuildingLimit
---
--- The building_name corresponds to the file name
---  (e.g. "space_marine_vehicle_building" and not "machine_cult")
---
--- *Source: LuaCpuInterface.cpp*
---@param player PlayerID
function Cpu_ClearBuildingLimits(player) end

--- Gets the AI to try to take back the closest VO
---
--- *Source: LuaCpuInterface.cpp*
---@param player PlayerID
function Cpu_CounterVictoryObj(player) end

--- Run this string in the AI of player
---
--- *Source: LuaCpuInterface.cpp*
---@param player PlayerID
---@param s string
function Cpu_DoString(player, s) end

--- Enables/Disables a cpuplayer
---
--- *Source: LuaCpuInterface.cpp*
---@param player PlayerID
---@param enable boolean
function Cpu_Enable(player, enable) end

--- Enables/Disables all cpuplayers
---
--- *Source: LuaCpuInterface.cpp*
---@param enable boolean
function Cpu_EnableAll(enable) end

--- Enables/disables a component of the AI
---
--- Components:
---     CT_Attacking
---     CT_Resourcing
---     CT_BuildUnits
---     CT_BuildBuildings
---     CT_BuildResearch
---     CT_BuildAddOns    CT_Tactics
---
--- *Source: LuaCpuInterface.cpp*
---@param player PlayerID
---@param enable boolean
---@param ct integer
function Cpu_EnableComponent(player, enable, ct) end

--- Force the AI to attack
---
--- *Source: LuaCpuInterface.cpp*
---@param player PlayerID
---@param attack PlayerID
function Cpu_ForceAttack(player, attack) end

--- Get the AI difficulty setting of this player
---
--- AD_Easy
---  AD_Standard
---  AD_Hard
---  AD_Advanced
---  AD_Insane
---
--- *Source: LuaCpuInterface.cpp*
---@param player PlayerID
---@return integer
function Cpu_GetDifficulty(player) end

--- Returns true if player is a cpu player
---
--- *Source: LuaCpuInterface.cpp*
---@param player PlayerID
---@return boolean
function Cpu_IsCpuPlayer(player) end

--- Returns true if we are currently debugging player
---
--- *Source: LuaCpuInterface.cpp*
---@param player PlayerID
---@return boolean
function Cpu_IsDebugging(player) end

--- Returns true if player is a CpuPlayer and is enabled
---
--- *Source: LuaCpuInterface.cpp*
---@param player PlayerID
---@return boolean
function Cpu_IsEnabled(player) end

--- Lock an entity so the cpuplayer can't touch it
---
--- Lockable entities: buildings and strategic points. If you would like to lock a squad, use Cpu_LockSquad
---
--- *Source: LuaCpuInterface.cpp*
---@param player PlayerID
---@param id integer
function Cpu_LockEntity(player, id) end

--- Lock a squad so the cpuplayer can't touch it
---
--- Locking a squad forces the cpuplayer to let go of control of the squad     so designers can do what they like with them without fear of the AI conflicting
---
--- *Source: LuaCpuInterface.cpp*
---@param player PlayerID
---@param id integer
function Cpu_LockSquad(player, id) end

--- Reset the demand table
---
--- *Source: LuaCpuInterface.cpp*
---@param player PlayerID
function Cpu_ResetBuildTable(player) end

--- Reset the preferences for unit classes
---
--- *Source: LuaCpuInterface.cpp*
---@param player PlayerID
function Cpu_ResetClassPreferences(player) end

--- Set an entry in the demand table (Use SetBuildTable instead of this function)
---
--- *Source: LuaCpuInterface.cpp*
---@param player PlayerID
---@param name string
---@param start number
---@param increment number
function Cpu_SetBuildDemand(player, name, start, increment) end

--- Puts a limit on the number of this type of building that can be built
---
--- The building_name corresponds to the file name
---  (e.g. "space_marine_vehicle_building" and not "machine_cult")
---
--- *Source: LuaCpuInterface.cpp*
---@param player PlayerID
---@param building_name string
---@param limit integer
function Cpu_SetBuildingLimit(player, building_name, limit) end

--- Set the preference for a particular unit class
---
--- UnitClasses:
---      UC_LightInfantryLow
---      UC_LightInfantryMed
---      UC_LightInfantryHigh
---      UC_HeavyInfantryMed
---      UC_HeavyInfantryHigh
---      UC_VehicleLow
---      UC_VehicleMed
---      UC_VehicleHigh
---      UC_MonsterMed
---      UC_MonsterHigh
---      UC_Commander
---
---  The preference works in an additive way. Thus, if the preferences are:
---  UC_LightInfantry:    10
---  UC_HeavyInfantry:    20
---  UC_Vehicle:            5
---  Then the probability that a vehicle is built next is 5/(10+20+5) = ~14% chance
---  NOTE: Setting class preferences clobbers the build demand table
---
--- *Source: LuaCpuInterface.cpp*
---@param player PlayerID
---@param unit_class integer
---@param preference number
function Cpu_SetClassPreference(player, unit_class, preference) end

--- Set the AI difficulty setting of this player
---
--- AD_Easy
---  AD_Standard
---  AD_Hard
---  AD_Advanced
---  AD_Insane
---
--- *Source: LuaCpuInterface.cpp*
---@param player PlayerID
---@param difficulty integer
function Cpu_SetDifficulty(player, difficulty) end

--- If enabled, cpu player talks smack
---
--- *Source: LuaCpuInterface.cpp*
---@param player PlayerID
---@param enable boolean
function Cpu_SetLippy(player, enable) end

--- Set the AI to have a maximum army of a percentage of the enemy army
---
--- [0.0,1.0]
---
--- *Source: LuaCpuInterface.cpp*
---@param player PlayerID
---@param percent number
function Cpu_SetMaxPercentage(player, percent) end

--- Unlock all locked resources so the cpuplayer has control of them
---
--- *Source: LuaCpuInterface.cpp*
---@param player PlayerID
function Cpu_UnlockAll(player) end

--- Unlock an entity so the cpuplayer has control of it
---
--- *Source: LuaCpuInterface.cpp*
---@param player PlayerID
---@param id integer
function Cpu_UnlockEntity(player, id) end

--- Unlock a squad so the cpuplayer has control of it
---
--- *Source: LuaCpuInterface.cpp*
---@param player PlayerID
---@param id integer
function Cpu_UnlockSquad(player, id) end

-- from LuaEntity.cpp

--- Returns true if a target entity is in the entities sight radius and is NOT infiltrated
---
--- *Source: LuaEntity.cpp*
---@param entity EntityID
---@param target EntityID
---@return boolean
function Entity_CanSeeEntity(entity, target) end

--- Returns true if a target squad is in the squads sight radius and is NOT infiltrated.
---
--- *Source: LuaEntity.cpp*
---@param entity EntityID
---@param target SquadID
---@return boolean
function Entity_CanSeeSquad(entity, target) end

--- Returns true if addon has been built for the given entity
---
--- *Source: LuaEntity.cpp*
---@param entity EntityID
---@param addonname string
---@return boolean
function Entity_ContainsAddOn(entity, addonname) end

--- Creates an entity at a given position and assigns it to a given player.  'blueprint' is a string value containing the name of the entity blueprint.
---
--- *Source: LuaEntity.cpp*
---@param blueprint string
---@param player PlayerID
---@param pos Position
---@return EntityID
function Entity_Create(blueprint, player, pos) end

--- Creates a building for the specified player at the given marker at some percentage into construction
---
--- When placing the building, this function will ignore placement failures due to the control radius, FOW blocking or ground cover blocking. The function returns true if it manages to create the building and false otherwise (logs error on failure).
---
--- *Source: LuaEntity.cpp*
---@param player PlayerID
---@param egroupName string
---@param blueprint string
---@param markerName string
---@param constructionPercent number
---@return boolean
function Entity_CreateBuildingMarker(player, egroupName, blueprint, markerName, constructionPercent) end

--- Creates a building for the specified player at the given position at some percentage into construction
---
--- When placing the building, this function will ignore placement failures due to the control radius, FOW blocking or ground cover blocking. The function returns true if it manages to create the building and false otherwise (logs error on failure).
---
--- *Source: LuaEntity.cpp*
---@param player PlayerID
---@param egroupName string
---@param blueprint string
---@param position Position
---@param constructionPercent number
---@return boolean
function Entity_CreateBuildingPosition(player, egroupName, blueprint, position, constructionPercent) end

--- Creates a building for the specified player at the given position at some percentage into construction
---
--- When placing the building, this function will ignore placement failures due to the control radius, FOW blocking or ground cover blocking. The function returns true if it manages to create the building and false otherwise (logs error on failure).
---
--- *Source: LuaEntity.cpp*
---@param player PlayerID
---@param egroupName string
---@param blueprint string
---@param position Position
---@param constructionPercent number
---@return boolean
function Entity_CreateBuildingPositionForce(player, egroupName, blueprint, position, constructionPercent) end

--- DeSpawn the entity at its current position
---
--- *Source: LuaEntity.cpp*
---@param entity EntityID
function Entity_DeSpawn(entity) end

--- Remove an entity from the world and destroy it.
---
--- *Source: LuaEntity.cpp*
---@param entity EntityID
function Entity_Destroy(entity) end

--- Instantly builds an addon on a building, ignoring building requirments.
---
--- *Source: LuaEntity.cpp*
---@param building EntityID
---@param addonname string
function Entity_ForceAddOn(building, addonname) end

--- Get an entity from a mission editor ID.
---
--- *Source: LuaEntity.cpp*
---@param id integer
---@return EntityID
function Entity_FromWorldID(id) end

--- Returns the active entity command.  Some valid entity commands: STATEID_Attack, STATEID_StructureBuilding, STATEID_StrategicPointCapture, STATEID_Dead, STATEID_GetBackUp, STATEID_Idle, STATEID_Pause, STATEID_DeployWeapon
---
--- *Source: LuaEntity.cpp*
---@param entity EntityID
---@return integer
function Entity_GetActiveCommand(entity) end

--- Returns the ID of the entity blueprint
---
--- *Source: LuaEntity.cpp*
---@param entity EntityID
---@return integer
function Entity_GetBlueprintID(entity) end

--- Returns the ID of the blueprint from blueprint file name
---
--- *Source: LuaEntity.cpp*
---@param blueprint string
---@return integer
function Entity_GetBlueprintIDFromName(blueprint) end

--- Returns the name of the entity blueprint (from the attribute editor)
---
--- *Source: LuaEntity.cpp*
---@param entity EntityID
---@return string
function Entity_GetBlueprintName(entity) end

--- Returns the construction progress (with range [0.0, 1.0] for a given entity.  Returns 0.0 if the entity is not a building.
---
--- *Source: LuaEntity.cpp*
---@param entity EntityID
---@return number
function Entity_GetBuildingProgress(entity) end

--- Returns the type of cover an entity is in.
---
--- Returns CT_None if entity does not have cover extension Returns one of the possible cover types: CT_None, CT_Light, CT_Heavy, CT_Negative, CT_Blocking, CT_Stealth
---
--- *Source: LuaEntity.cpp*
---@param entity EntityID
---@return integer
function Entity_GetCoverType(entity) end

--- Returns the entities unique id in the world
---
--- *Source: LuaEntity.cpp*
---@param entity EntityID
---@return integer
function Entity_GetGameID(entity) end

--- Returns the health of an entity.
---
--- Health will be zero for entities with no health extension.
---
--- *Source: LuaEntity.cpp*
---@param entity EntityID
---@return number
function Entity_GetHealth(entity) end

--- Returns true if an entity is invulnerable.
---
--- *Source: LuaEntity.cpp*
---@param entity EntityID
---@return boolean
function Entity_GetHealthInvulnerable(entity) end

--- Returns the max health of an entity.
---
--- Max health will be zero for entities with no health extension.
---
--- *Source: LuaEntity.cpp*
---@param entity EntityID
---@return number
function Entity_GetHealthMax(entity) end

--- Returns the Player owner of the given entity. Entity MUST NOT be owned by the world.
---
--- *Source: LuaEntity.cpp*
---@param entity EntityID
---@return PlayerID
function Entity_GetPlayerOwner(entity) end

--- Returns the position of the entity.  The position is currently a lua table with three entries (x, y, z)
---
--- *Source: LuaEntity.cpp*
---@param entity EntityID
---@return Position
function Entity_GetPosition(entity) end

--- Returns the name of the production item with index in range of [0,n-1]
---
--- There are currently 3 types of production items.  PQ_Squads, PQ_AddOns, and PQ_Research.  Any of these items can in a production queue.
---
---  The results of this function will vary depending on the production items type.  For PQ_Squads, the squad blueprint name will be returned, for PQ_AddOns the AddOn name will be returned, and for PQ_Research the research name will be returned.
---
---  Use Entity_GetProductionQueueItemType to get the type of a given production queue item if you are interested in such things.
---
--- *Source: LuaEntity.cpp*
---@param entity EntityID
---@param index integer
---@return string
function Entity_GetProductionQueueItemName(entity, index) end

--- Returns the production type (PQ_AddOn, PQ_Squad or PQ_Research) for a production queue item with index.
---
--- *Source: LuaEntity.cpp*
---@param entity EntityID
---@param index integer
---@return integer
function Entity_GetProductionQueueItemType(entity, index) end

--- Returns the number of items in the entities production queue.
---
--- It is an error to call this function on an entity that does not have a production queue.
---   Use Entity_HasProductionQueue to check that the entity has a queue.
---
--- *Source: LuaEntity.cpp*
---@param entity EntityID
---@return integer
function Entity_GetProductionQueueSize(entity) end

--- Returns the Squad of the given entity.
---
--- *Source: LuaEntity.cpp*
---@param entity EntityID
---@return SquadID
function Entity_GetSquad(entity) end

--- Returns true if the entity is owned by the world
---
--- *Source: LuaEntity.cpp*
---@param entity EntityID
---@return boolean
function Entity_GetWorldOwner(entity) end

--- Returns true if an entity has a production queue.
---
--- *Source: LuaEntity.cpp*
---@param entity EntityID
---@return boolean
function Entity_HasProductionQueue(entity) end

--- Returns true if the entity belongs to a squad.
---
--- *Source: LuaEntity.cpp*
---@param entity EntityID
---@return boolean
function Entity_HasSquad(entity) end

--- Returns whether the specified ability is active for the entity.
---
--- *Source: LuaEntity.cpp*
---@param entity EntityID
---@param abilityName string
---@return boolean
function Entity_IsAbilityActive(entity, abilityName) end

--- Returns true if the given entity is a constructable building
---
--- *Source: LuaEntity.cpp*
---@param e EntityID
---@return boolean
function Entity_IsBuilding(e) end

--- Returns true if a strategic point (an entity) is 100% captured by a given player.
---
--- Check that the entity is a strategic point using Entity_IsStrategic point before calling this function.
---
--- *Source: LuaEntity.cpp*
---@param entity EntityID
---@param player PlayerID
---@return boolean
function Entity_IsCapturedByPlayer(entity, player) end

---
--- *Source: LuaEntity.cpp*
---@param entity EntityID
---@return boolean
function Entity_IsRelocating(entity) end

--- Returns true if an entity is a strategic point (eg. it can be captured)
---
--- *Source: LuaEntity.cpp*
---@param entity EntityID
---@return boolean
function Entity_IsStrategicPoint(entity) end

--- Returns true if the entity was attacked in the last second.
---
--- *Source: LuaEntity.cpp*
---@param entity EntityID
---@return boolean
function Entity_IsUnderAttack(entity) end

--- Returns true if the entity was attacked by the given player over the last second.
---
--- *Source: LuaEntity.cpp*
---@param entity EntityID
---@param player PlayerID
---@return boolean
function Entity_IsUnderAttackByPlayer(entity, player) end

--- Resets the construction state for a given building.    You will not be able to build stuff from the building until construction is completed
---
--- *Source: LuaEntity.cpp*
---@param e EntityID
function Entity_ResetConstructionState(e) end

--- Offsets entity from squad middle (specified distance is max)
---
--- *Source: LuaEntity.cpp*
---@param entity EntityID
---@param fMaxRadius number
function Entity_ScatterFromSquad(entity, fMaxRadius) end

--- Sets the construction progress for the entity.
---
--- After this function is called, the entity will be in the construction state. The construction progress needs to be in the [0.0, 1.0] range. This function will fail if the entity is not a building.
---
--- *Source: LuaEntity.cpp*
---@param entity EntityID
---@param progress number
function Entity_SetBuildingProgress(entity, progress) end

--- Forces a building in progress to complete construction
---
--- *Source: LuaEntity.cpp*
---@param e EntityID
function Entity_SetConstructionComplete(e) end

--- Set entity to render at fixed height
---
--- *Source: LuaEntity.cpp*
---@param squad SquadID
---@param onOff boolean
---@param val number
function Entity_SetFixedY(squad, onOff, val) end

--- Set the health of an entity.  healthPercent must be in the range [0.0, 1.0].
---
--- *Source: LuaEntity.cpp*
---@param entity EntityID
---@param healthPercent number
function Entity_SetHealth(entity, healthPercent) end

--- Make an entity invulnerable to physical damage.
---
--- IMPORTANT: You must disable invulnerability for everytime you enable it!
---
--- *Source: LuaEntity.cpp*
---@param entity EntityID
---@param enabled boolean
function Entity_SetHealthInvulnerable(entity, enabled) end

--- Changes the owner of the given squad.
---
--- *Source: LuaEntity.cpp*
---@param entity EntityID
---@param owner PlayerID
function Entity_SetPlayerOwner(entity, owner) end

--- Sets the position of the entity.  The position is currently a lua table with three entries (x, y, z)
---
--- *Source: LuaEntity.cpp*
---@param entity EntityID
---@param pos Position
function Entity_SetPosition(entity, pos) end

--- Sets the owner of the entity to the world
---
--- *Source: LuaEntity.cpp*
---@param entity EntityID
function Entity_SetWorldOwner(entity) end

--- Spawn the entity at its current position
---
--- *Source: LuaEntity.cpp*
---@param entity EntityID
function Entity_Spawn(entity) end

--- Returns true the given strategic point has a structure on it
---
--- Throws an error if entity is not a strategic point
---
--- *Source: LuaEntity.cpp*
---@param e EntityID
---@return boolean
function Entity_StrategicPointHasStructure(e) end

-- from LuaFOW.cpp

--- Resets the revealed FOW areas and destroys all ghosted structures.
---
--- *Source: LuaFOW.cpp*
function FOW_Reset() end

--- Reveal FOW for all players
---
--- *Source: LuaFOW.cpp*
function FOW_RevealAll() end

--- Reveals a circular area for all alive players over a given duration. YOU SHOULD ONLY CALL THIS ONCE FOR EACH AREA.
---
--- Pass in a duration of -1 for indefinite duration ( until unreveal is called )
---
--- *Source: LuaFOW.cpp*
---@param x number
---@param z number
---@param radius number
---@param durationSecs number
function FOW_RevealArea(x, z, radius, durationSecs) end

--- Reveals an entity groups line of sight(LOS) for all alive players over a given duration. YOU SHOULD ONLY CALL THIS ONCE FOR EACH AREA.
---
--- Pass in a duration of -1 for indefinite duration ( until the entities are dead )
---
--- *Source: LuaFOW.cpp*
---@param group EGroupID
---@param durationSecs number
function FOW_RevealEGroup(group, durationSecs) end

--- Reveals an entities line of sight (LOS) for all alive players over a given duration. YOU SHOULD ONLY CALL THIS ONCE FOR EACH AREA.
---
--- Pass in a duration of -1 for indefinite duration ( until the entity is dead )
---
--- *Source: LuaFOW.cpp*
---@param entity EntityID
---@param durationSecs number
function FOW_RevealEntity(entity, durationSecs) end

--- Reveals a squad groups line of sight(LOS) for all alive players over a given duration. YOU SHOULD ONLY CALL THIS ONCE FOR EACH AREA.
---
--- Pass in a duration of -1 for indefinite duration ( until the squads are dead )
---
--- *Source: LuaFOW.cpp*
---@param group SGroupID
---@param durationSecs number
function FOW_RevealSGroup(group, durationSecs) end

--- Reveals a squads line if sight(LOS) for all alive players over a given duration. YOU SHOULD ONLY CALL THIS ONCE FOR EACH AREA.
---
--- Pass in a duration of -1 for indefinite duration ( until the entity is dead )
---
--- *Source: LuaFOW.cpp*
---@param squad SquadID
---@param durationSecs number
function FOW_RevealSquad(squad, durationSecs) end

--- Adds all items in a group to a given players FOW.
---
--- Items line of sight will be visible until it is destroyed or UnTag is called.
---
--- *Source: LuaFOW.cpp*
---@param player PlayerID
---@param group EGroupID
function FOW_TagEGroup(player, group) end

--- Adds all items in a group to a given players FOW.
---
--- Items line of sight will be visible until it is destroyed or UnTag is called.
---
--- *Source: LuaFOW.cpp*
---@param player PlayerID
---@param group SGroupID
function FOW_TagSGroup(player, group) end

--- Use to undo a FOW_RevealAll
---
--- *Source: LuaFOW.cpp*
function FOW_UnRevealAll() end

--- UnReveals a circular area for all alive players. YOU SHOULD ONLY CALL THIS ONCE FOR EACH AREA.
---
--- *Source: LuaFOW.cpp*
---@param x number
---@param z number
---@param radius number
function FOW_UnRevealArea(x, z, radius) end

--- Removes items from a players FOW that were added using FOW_TagEGroup
---
--- *Source: LuaFOW.cpp*
---@param player PlayerID
---@param group EGroupID
function FOW_UnTagEGroup(player, group) end

--- Removes items from a players FOW that were added using FOW_TagSGroup
---
--- *Source: LuaFOW.cpp*
---@param player PlayerID
---@param group SGroupID
function FOW_UnTagSGroup(player, group) end

-- from LuaGameMisc.cpp

--- Forces teamcolor of a entity to switch colors when assigned to the indicated player
---
--- Second parameter set the auto team color switch to be enabled for this player..  By default teamcolors do not switch when an entity changes players Misc_ForceAutoSwitchTeamColor(playerID, enable )
---
--- *Source: LuaGameMisc.cpp*
---@param pPlayer PlayerID
---@param enabled boolean
function Misc_ForceAutoSwitchTeamColor(pPlayer, enabled) end

--- Returns the player ID of the local player. Returns -1 if no local player.
---
--- You can never assume that there will be a local player!  There is no local player in recorded games and ai vs ai games.
---
--- *Source: LuaGameMisc.cpp*
---@return integer
function Misc_GetLocalPlayerID() end

--- Forces a player teamcolor to the specified set.
---
--- Second parameter corresponds to team specific id i.e. if real filename is ork_team1.rtx this parameter is team1.  Default multiplayer colors are default_1 ..  Misc_PlayerTeamColor(playerID, "filenameid")
---
--- *Source: LuaGameMisc.cpp*
---@param pPlayer PlayerID
function Misc_PlayerTeamColor(pPlayer) end

--- Retrieve a custom value in current player profile.  Returns an empty string if value does not exist
---
--- *Source: LuaGameMisc.cpp*
---@param varName string
---@return string
function PlayerProfile_GetVar(varName) end

--- Save a custom value in current player profile.
---
--- *Source: LuaGameMisc.cpp*
---@param varName string
---@param val string
function PlayerProfile_SetVar(varName, val) end

--- Do not pre-cache the specified player's EBPs in single player missions
---
--- This function will validate all blueprint names.
---  For example, you can turn off pre-caching for g_Player1 (assuming this player is setup) by:
---  g_Player1NoPrecacheTable =
---  {
---      "chaos_greater_sacrificial_circle"
---      "chaos_thermo_plasma_generator"
---      "chaos_raptor"
---      "chaos_raptor_aspiring_champion"
---      "chaos_bloodthirster"
---      "chaos_rhino"
---      "chaos_obliterator"
---      "chaos_daemon_prince"
---  }
---
---  Player_DoNotPrecache( g_Player1, g_Player1NoPrecacheTable )
---
--- *Source: LuaGameMisc.cpp*
---@param pPlayer PlayerID
---@param ebps table
function Player_DoNotPrecache(pPlayer, ebps) end

--- Returns the local player index to be used with World_GetPlayerAt.
---
--- *Source: LuaGameMisc.cpp*
---@return integer
function Tut_GetLocalPlayerIndex() end

-- from LuaGameSetup.cpp

--- Set the UI name of a given player.  Name can be a localized string "$10020" or a normal string "PlayerName".
---
--- *Source: LuaGameSetup.cpp*
---@param player PlayerID
---@param name LocString
function Setup_SetPlayerName(player, name) end

--- Set the race for a given player.  Use World_GetRaceIndex() to get the race index from the ME name.
---
--- *Source: LuaGameSetup.cpp*
---@param player PlayerID
function Setup_SetPlayerRace(player) end

-- from LuaGroup.cpp

--- Adds an entity to the end of a group if the group doesnt already have it.
---
--- *Source: LuaGroup.cpp*
---@param group EGroupID
---@param entity EntityID
function EGroup_Add(group, entity) end

--- Appends the entities in one group to another group.
---
--- All entities from 'grouptoadd' will be added to 'group'.
---  If 'group' already contains an entity from 'grouptoadd' it will not be added.
---  This function does not clear the contents of 'grouptoadd'.
---  Example: Add group2 (0, 10, 11, 22) to group1 (1, 11, 20) --&gt; group1 would now be (1, 11, 20, 0, 10, 22)
---
--- *Source: LuaGroup.cpp*
---@param group EGroupID
---@param grouptoadd EGroupID
function EGroup_AddGroup(group, grouptoadd) end

--- Removes all entities from a group
---
--- *Source: LuaGroup.cpp*
---@param egroup EGroupID
function EGroup_Clear(egroup) end

--- Returns true if the contents of the two groups are equal. Order of the entities does not matter.
---
--- *Source: LuaGroup.cpp*
---@param group1 EGroupID
---@param group2 EGroupID
---@return boolean
function EGroup_Compare(group1, group2) end

--- Check if a group contains ALL or ANY of the items in a blueprint table.
---
--- See SGroup_ContainsBlueprints for more info.
---
--- *Source: LuaGroup.cpp*
---@param egroup EGroupID
---@param blueprintTable table
---@param all boolean
---@return boolean
function EGroup_ContainsBlueprints(egroup, blueprintTable, all) end

--- Returns true if an sgroup contains a given squad
---
--- *Source: LuaGroup.cpp*
---@param egroup EGroupID
---@param entity EntityID
---@return boolean
function EGroup_ContainsEntity(egroup, entity) end

--- Returns the total number of spawned and despawned entities in a group.
---
--- *Source: LuaGroup.cpp*
---@param egroup EGroupID
---@return integer
function EGroup_Count(egroup) end

--- Returns the number of despawned entities in a group.
---
--- *Source: LuaGroup.cpp*
---@param egroup EGroupID
---@return integer
function EGroup_CountDeSpawned(egroup) end

--- Returns the number of spawned entities in a group.
---
--- *Source: LuaGroup.cpp*
---@param egroup EGroupID
---@return integer
function EGroup_CountSpawned(egroup) end

--- Returns a new entity group with the given name.
---
--- Entity groups are used for buildings and objects such as rocks and trees.
---  If you need to issue orders to a group vehicles or units you must use a SquadGroup.
---
--- *Source: LuaGroup.cpp*
---@param name string
---@return EGroupID
function EGroup_Create(name) end

--- Creates an egroup (if one doesn't exist) and adds to it entities that have a specified blueprint that are part of a different group.
---
--- If the group being created already exists, it will be appended to. Entities will not be removed from the from group.
---
--- *Source: LuaGroup.cpp*
---@param newGroupName string
---@param fromGroupName string
---@param blueprint string
function EGroup_CreateFromEGroup(newGroupName, fromGroupName, blueprint) end

--- Manually destroy a group that you dont need anymore.
---
--- *Source: LuaGroup.cpp*
---@param egroup EGroupID
function EGroup_Destroy(egroup) end

--- Returns true if the entity group with the given name exists
---
--- *Source: LuaGroup.cpp*
---@param name string
---@return boolean
function EGroup_Exists(name) end

--- Call a lua function for each item in a group. Function will recieve (groupid, itemindex, itemid) and should return true to break or false to continue.
---
--- function Rule_Test( )
---      local DespawnEntity = function( egroupid, itemindex, entityID )
---          Entity_Despawn( entityID )
---      end
---
---      EGroup_ForEach( EGroup_FromName("eg_PlayersHQ"), DespawnEntity )
---  end
---  Note: This function iterates over SPAWNED ENTITIES ONLY.
---
--- *Source: LuaGroup.cpp*
---@param egroup EGroupID
---@param f function
---@return boolean
function EGroup_ForEach(egroup, f) end

--- Call a lua function for each item in a group. Function will recieve (groupid, itemindex, itemid) and should return a bool.
---
--- This is used for checking if ALL or ANY items in a group match a given predicate. (eg. Are all items in a group choas marines)
---  Note: This function iterates over SPAWNED ENTITIES ONLY.
---
--- *Source: LuaGroup.cpp*
---@param egroup EGroupID
---@param all boolean
---@param f function
---@return boolean
function EGroup_ForEachAllOrAny(egroup, all, f) end

--- Same as EGroup_ForEachAllOrAny except you have a choice to iterate over spawned entities, despawned entities, or both.
---
--- *Source: LuaGroup.cpp*
---@param egroup EGroupID
---@param all boolean
---@param f function
---@param spawned boolean
---@param despawned boolean
---@return boolean
function EGroup_ForEachAllOrAnyEx(egroup, all, f, spawned, despawned) end

--- Same as EGroup_ForEach except you have a choice to iterate over spawned entities, despawned entities, or both.
---
--- *Source: LuaGroup.cpp*
---@param egroup EGroupID
---@param f function
---@param spawned boolean
---@param despawned boolean
---@return boolean
function EGroup_ForEachEx(egroup, f, spawned, despawned) end

--- Same as EGroup_ForEach except you have a choice to iterate over spawned entities, despawned entities, or both and passes the position to the scar function.
---
--- *Source: LuaGroup.cpp*
---@param egroup EGroupID
---@param f function
---@param position Position
---@param spawned boolean
---@param despawned boolean
---@return boolean
function EGroup_ForEachExPosition(egroup, f, position, spawned, despawned) end

--- Find an entity group with a given name.
---
--- *Source: LuaGroup.cpp*
---@param name string
---@return EGroupID
function EGroup_FromName(name) end

--- Returns the despawned entity at the given index.
---
--- Use EGroup_GetSpawnedEntityAt if you want the spawned items in the group
---  Use index 1 to get the first entity in the group.
---  It is an error if index &gt; EGroup_GetCountDeSpawned()
---
--- *Source: LuaGroup.cpp*
---@param group EGroupID
---@param index integer
---@return EntityID
function EGroup_GetDeSpawnedEntityAt(group, index) end

--- Returns the name of a given entity group.
---
--- *Source: LuaGroup.cpp*
---@param egroup EGroupID
---@return string
function EGroup_GetName(egroup) end

--- Returns the center position of an entity group.
---
--- *Source: LuaGroup.cpp*
---@param group EGroupID
---@return Position
function EGroup_GetPosition(group) end

--- Returns the spawned entity at the given index.
---
--- Use EGroup_GetDeSpawnedEntityAt if you want the despawned items in the group
---  Use index 1 to get the first entity in the group.
---  It is an error if index &gt; EGroup_GetCountSpawned()
---
--- *Source: LuaGroup.cpp*
---@param group EGroupID
---@param index integer
---@return EntityID
function EGroup_GetSpawnedEntityAt(group, index) end

--- Performs a group intersection.
---
--- Only entities that are in both groups will be added to 'group'.
---  The contents of 'grouptointersect' will not be changed.
---  Example: group = (1,2,3,4,5,6)  grouptointersect = (2,4,6,8,10)   --&gt; group will now equal (2,4,6)
---
--- *Source: LuaGroup.cpp*
---@param group EGroupID
---@param grouptointersect EGroupID
function EGroup_Intersection(group, grouptointersect) end

--- Removes an entity from a group.
---
--- *Source: LuaGroup.cpp*
---@param group EGroupID
---@param entity EntityID
function EGroup_Remove(group, entity) end

--- Removes the entities in one group from another group.
---
--- If 'group' does not contain an entity from 'groupToRemove', that entity will be skipped.
---   Example: Remove group2 (0, 10, 11, 22) from group1 (0, 10, 11, 12, 13, 14) --&gt; group1 would now be (12, 13, 14)
---
--- *Source: LuaGroup.cpp*
---@param group EGroupID
function EGroup_RemoveGroup(group) end

--- Adds an squadron to the end of a group if the group doesnt already have it.
---
--- *Source: LuaGroup.cpp*
---@param group SGroupID
---@param squadron SquadID
function SGroup_Add(group, squadron) end

--- Same as EGroup_AddGroup.  Note: You cannot mix squad groups and entity groups.
---
--- *Source: LuaGroup.cpp*
---@param group SGroupID
---@param grouptoadd SGroupID
function SGroup_AddGroup(group, grouptoadd) end

--- Removes all entities from a group.
---
--- *Source: LuaGroup.cpp*
---@param sgroup SGroupID
function SGroup_Clear(sgroup) end

--- Returns true if the contents of the two groups are equal. Order of the entities does not matter.
---
--- *Source: LuaGroup.cpp*
---@param group1 SGroupID
---@param group2 SGroupID
---@return boolean
function SGroup_Compare(group1, group2) end

--- Check if a group contains ALL or ANY of the items in a blueprint table.
---
--- This function will validate all blueprint names.
---  Set all to true to check for all blueprints in the blueprintTable or set to false to check for any.
---  A blueprint table containing two marines and a bloodthirster looks like so:
---  BlueprintTable = {
---      { name = "chaos_marine_squad", count = 2 }
---      { name = "chaos_squad_bloodthirster", count = 1 }
---  }
---
---  You could also call Util_MakeBlueprintTable:
---  BlueprintTable = Util_MakeBlueprintTable( "chaos_marine_squad", "chaos_marine_squad", "chaos_squad_bloodthirster")
---
---  This function will take care of setting up the table for you.
---
--- *Source: LuaGroup.cpp*
---@param sgroup SGroupID
---@param blueprintTable table
---@param all boolean
---@return boolean
function SGroup_ContainsBlueprints(sgroup, blueprintTable, all) end

--- Returns true if an sgroup contains a given squad
---
--- *Source: LuaGroup.cpp*
---@param sgroup SGroupID
---@param squad SquadID
---@return boolean
function SGroup_ContainsSquad(sgroup, squad) end

--- Returns the total number of spawned and despawned squads in a group.
---
--- *Source: LuaGroup.cpp*
---@param sgroup SGroupID
---@return integer
function SGroup_Count(sgroup) end

--- Returns the number of despawned squads in a group.
---
--- *Source: LuaGroup.cpp*
---@param sgroup SGroupID
---@return integer
function SGroup_CountDeSpawned(sgroup) end

--- Returns the number of spawned squads in a group.
---
--- *Source: LuaGroup.cpp*
---@param sgroup SGroupID
---@return integer
function SGroup_CountSpawned(sgroup) end

--- Returns a new squadron group with the given name.
---
--- Squad groups are used for all units and vehicles.  You can issue orders such as move and attack to an entire squad group.  If you want a group for buildings or objects such as trees, use an EntityGroup instead.
---   Note that you are allowed to have unamed squadgroups or squadgroups with the samename.  Be careful not to create  duplicates of your squad group when you dont intend to.  To be safe, you can check if the SquadGroup you want to create  exists using SGroup_Exists( )
---
--- *Source: LuaGroup.cpp*
---@param name string
---@return SGroupID
function SGroup_Create(name) end

--- Manually destroy a group that you dont need anymore.
---
--- *Source: LuaGroup.cpp*
---@param egroup SGroupID
function SGroup_Destroy(egroup) end

--- Returns true if the squad group with the given name exists
---
--- *Source: LuaGroup.cpp*
---@param name string
---@return boolean
function SGroup_Exists(name) end

--- Call a lua function for each item in a group. Function will recieve (groupid, itemindex, itemid) and should return true to break or false to continue.
---
--- function Rule_Test( )
---      local DespawnSquad = function( sgroupid, itemindex, squadID )
---          Squad_Despawn( squadID )
---      end
---
---      SGroup_ForEach( SGroup_FromName("sg_Squads"), DespawnSquad )
---  end
---
---  This functions iterates over spawned squads only.
---
--- *Source: LuaGroup.cpp*
---@param sgroup SGroupID
---@param f function
---@return boolean
function SGroup_ForEach(sgroup, f) end

--- Call a lua function for each item in a group. Function will recieve (groupid, itemindex, itemid) and should return a bool.
---
--- This is used for checking if ALL or ANY items in a group match a given predicate. (eg. Are all items in a group choas marines)
---  This functions iterates over spawned squads only.
---
--- *Source: LuaGroup.cpp*
---@param sgroup SGroupID
---@param all boolean
---@param f function
---@return boolean
function SGroup_ForEachAllOrAny(sgroup, all, f) end

--- Same as SGroup_ForEachAllOrAny except you have a choice to iterate over spawned squads, despawned squads, or both.
---
--- *Source: LuaGroup.cpp*
---@param sgroup SGroupID
---@param all boolean
---@param f function
---@param spawned boolean
---@param despawned boolean
---@return boolean
function SGroup_ForEachAllOrAnyEx(sgroup, all, f, spawned, despawned) end

--- Same as SGroup_ForEach except you have a choice to iterate over spawned squads, despawned squads, or both.
---
--- *Source: LuaGroup.cpp*
---@param sgroup SGroupID
---@param f function
---@param spawned boolean
---@param despawned boolean
---@return boolean
function SGroup_ForEachEx(sgroup, f, spawned, despawned) end

--- Same as SGroup_ForEach except you have a choice to iterate over spawned squads, despawned squads, or both and passes the position to the scar function.
---
--- *Source: LuaGroup.cpp*
---@param sgroup SGroupID
---@param f function
---@param position Position
---@param spawned boolean
---@param despawned boolean
---@return boolean
function SGroup_ForEachExPosition(sgroup, f, position, spawned, despawned) end

--- Find an squadron group with a given name.
---
--- *Source: LuaGroup.cpp*
---@param name string
---@return SGroupID
function SGroup_FromName(name) end

--- Returns the despawned squad at a certain position in the group.
---
--- This is a one-based index (use index 1 to get the first squad in the group.)    It is an error if index &gt; SGroup_CountDeSpawned().
---
--- *Source: LuaGroup.cpp*
---@param group SGroupID
---@param index integer
---@return SquadID
function SGroup_GetDeSpawnedSquadAt(group, index) end

--- Returns the name of a given squad group.
---
--- *Source: LuaGroup.cpp*
---@param sgroup SGroupID
---@return string
function SGroup_GetName(sgroup) end

--- Returns the center position of a squad group.
---
--- Despawned squads are ignored.
---
--- *Source: LuaGroup.cpp*
---@param group SGroupID
---@return Position
function SGroup_GetPosition(group) end

--- Returns the spawned squad at a certain position in the group.
---
--- This is a one-based index (use index 1 to get the first squad in the group.)    It is an error if index &gt; SGroup_CountSpawned().
---
--- *Source: LuaGroup.cpp*
---@param group SGroupID
---@param index integer
---@return SquadID
function SGroup_GetSpawnedSquadAt(group, index) end

--- Same as EGroup_Intersection. Note: You cannot mix squad groups and entity groups.
---
--- *Source: LuaGroup.cpp*
---@param group SGroupID
---@param grouptointersect SGroupID
function SGroup_Intersection(group, grouptointersect) end

--- Removes an squadron from a group.
---
--- *Source: LuaGroup.cpp*
---@param group SGroupID
---@param squadron SquadID
function SGroup_Remove(group, squadron) end

-- from LuaLocalizer.cpp

---
--- *Source: LuaLocalizer.cpp*
---@param number integer
---@return LocString
function Loc_ConvertNumber(number) end

---
--- *Source: LuaLocalizer.cpp*
---@param formatID integer
---@param arg1 LocString
---@return LocString
function Loc_FormatText1(formatID, arg1) end

---
--- *Source: LuaLocalizer.cpp*
---@param formatID integer
---@param arg1 LocString
---@param arg2 LocString
---@return LocString
function Loc_FormatText2(formatID, arg1, arg2) end

---
--- *Source: LuaLocalizer.cpp*
---@param formatID integer
---@param arg1 LocString
---@param arg2 LocString
---@param arg3 LocString
---@return LocString
function Loc_FormatText3(formatID, arg1, arg2, arg3) end

---
--- *Source: LuaLocalizer.cpp*
---@param formatID integer
---@param arg1 LocString
---@param arg2 LocString
---@param arg3 LocString
---@param arg4 LocString
---@return LocString
function Loc_FormatText4(formatID, arg1, arg2, arg3, arg4) end

---
--- *Source: LuaLocalizer.cpp*
---@param formatID integer
---@param secs integer
---@return LocString
function Loc_FormatTime(formatID, secs) end

-- from LuaMarker.cpp

--- Returns true if a generic number attribute exists for the marker type.
---
--- *Source: LuaMarker.cpp*
---@param marker MarkerID
---@param attrname string
---@return boolean
function Marker_DoesNumberAttributeExist(marker, attrname) end

--- Returns true if a generic string attribute exists for the marker type.
---
--- *Source: LuaMarker.cpp*
---@param marker MarkerID
---@param attrname string
---@return boolean
function Marker_DoesStringAttributeExist(marker, attrname) end

--- Returns true if marker exists.
---
--- *Source: LuaMarker.cpp*
---@param name string
---@param type string
---@return boolean
function Marker_Exists(name, type) end

--- Returns a ScarMarker given its name and type from the Mission Editor.
---
--- The type is defined in the markers list of the mission editor when placing markers.
---   You will need the result of this function to call any functions that start with Marker_
---
--- *Source: LuaMarker.cpp*
---@param name string
---@param type string
---@return MarkerID
function Marker_FromName(name, type) end

--- Returns the name of a given marker.  This value gets set in the Mission Editor.
---
--- *Source: LuaMarker.cpp*
---@param marker MarkerID
---@return string
function Marker_GetName(marker) end

--- Returns a generic number attribute defined in a marker.
---
--- Different marker types have different attributes. Note: All marker types have Name and Proximity.  Do not use this function to retrieve these values, use Marker_GetName and Marker_GetProximity instead.
---
--- *Source: LuaMarker.cpp*
---@param marker MarkerID
---@param attrname string
---@return number
function Marker_GetNumberAttribute(marker, attrname) end

--- Returns the position of a given marker.
---
--- *Source: LuaMarker.cpp*
---@param marker MarkerID
---@return Position
function Marker_GetPosition(marker) end

--- Returns the proximity of a given marker.  This value gets set in the Mission Editor.
---
--- *Source: LuaMarker.cpp*
---@param marker MarkerID
---@return number
function Marker_GetProximity(marker) end

--- Returns a generic string attribute defined in a marker.
---
--- Different marker types can have different attributes.
---   Note: All marker types have Name and Proximity.  Do not use this function to retrieve these values, use Marker_GetName and Marker_GetProximity instead.
---   See markertypes.lua for a listing of all the marker types.
---
--- *Source: LuaMarker.cpp*
---@param marker MarkerID
---@param attrname string
---@return string
function Marker_GetStringAttribute(marker, attrname) end

--- Returns the typename of a given marker.  This is the typename from the Mission Editor (name displayed when placing markers)
---
--- *Source: LuaMarker.cpp*
---@param marker MarkerID
---@return string
function Marker_GetType(marker) end

--- Returns true if the given position is in the markers proximity radius.
---
--- *Source: LuaMarker.cpp*
---@param marker MarkerID
---@param pos Position
---@return boolean
function Marker_InProximity(marker, pos) end

-- from LuaMetaMap.cpp

--- Returns the race index for attacker in the current game.
---
--- This function will throw an exception if called outside of a Meta Map game.
---
--- *Source: LuaMetaMap.cpp*
---@return integer
function MetaMap_GetAttackerRaceIndex() end

--- Returns the race name for the attacker in the current game.
---
--- This function will throw an exception if called outside of a Meta Map game.
---
--- *Source: LuaMetaMap.cpp*
---@return string
function MetaMap_GetAttackerRaceName() end

--- Returns the number of starting entities a race will currently be allocated.
---
--- Race starting entities are given through the Forward Base mechanism
---   This function will throw an exception if called outside of a Meta Map game or an unknown race is specified.
---
--- *Source: LuaMetaMap.cpp*
---@return integer
function MetaMap_GetAttackingRaceStartingEntitiesCount() end

--- Returns the indexed starting entity for the attacking race.
---
--- Use the function MetaMap_GetRaceStartingEntitiesCount to determine the number of starting squads a race will be allocated.
---   This function will throw an exception if called outside of a Meta Map game, an unknown race is specified or an out of range index is given.
---
--- *Source: LuaMetaMap.cpp*
---@param index integer
---@return string
function MetaMap_GetAttackingRaceStartingEntity(index) end

--- Returns the territory index for the attacker in the current game.
---
--- This function will throw an exception if called outside of a Meta Map game.
---
--- *Source: LuaMetaMap.cpp*
---@return integer
function MetaMap_GetAttackingTerritoryIndex() end

--- Returns the race index for the defender in the current game.
---
--- This function will throw an exception if called outside of a Meta Map game.
---
--- *Source: LuaMetaMap.cpp*
---@return integer
function MetaMap_GetDefenderRaceIndex() end

--- Returns the race name for the defender in the current game.
---
--- This function will throw an exception if called outside of a Meta Map game.
---
--- *Source: LuaMetaMap.cpp*
---@return string
function MetaMap_GetDefenderRaceName() end

--- Returns the territory index for the defender in the current game.
---
--- This function will throw an exception if called outside of a Meta Map game.
---
--- *Source: LuaMetaMap.cpp*
---@return integer
function MetaMap_GetDefendingTerritoryIndex() end

--- Returns the name of the meta map player's color scheme.
---
--- This function will throw an exception if called outside of a Meta Map game.
---
--- *Source: LuaMetaMap.cpp*
---@return string
function MetaMap_GetPlayerNColorScheme() end

--- Returns the name of the meta map player's faction.
---
--- This function will throw an exception if called outside of a Meta Map game.
---
--- *Source: LuaMetaMap.cpp*
---@param playerNum integer
---@return string
function MetaMap_GetPlayerNFactionName(playerNum) end

--- Returns the index of the meta map playerN's race.
---
--- This function will throw an exception if called outside of a Meta Map game.
---
--- *Source: LuaMetaMap.cpp*
---@param playerNum integer
---@return integer
function MetaMap_GetPlayerNRaceIndex(playerNum) end

--- Returns the name of the meta map playerN's race.
---
--- This function will throw an exception if called outside of a Meta Map game.
---
--- *Source: LuaMetaMap.cpp*
---@param playerNum integer
---@return string
function MetaMap_GetPlayerNRaceName(playerNum) end

--- Returns the index of the meta map player's race.
---
--- This function will throw an exception if called outside of a Meta Map game.
---
--- *Source: LuaMetaMap.cpp*
---@return integer
function MetaMap_GetPlayerRaceIndex() end

--- Returns the name of the meta map player's race.
---
--- This function will throw an exception if called outside of a Meta Map game.
---
--- *Source: LuaMetaMap.cpp*
---@return string
function MetaMap_GetPlayerRaceName() end

--- Returns the indexed starting squad for a race.
---
--- Use the function MetaMap_GetRaceStartingSquadsCount to determine the number of starting squads a race will be allocated.
---   This function will throw an exception if called outside of a Meta Map game, an unknown race is specified or an out of range index is given.
---
--- *Source: LuaMetaMap.cpp*
---@param raceName string
---@param index integer
---@return string
function MetaMap_GetRaceStartingSquadName(raceName, index) end

--- Returns the number of starting squads a race will currently be allocated.
---
--- Race starting squads are given through several possible mechanisms, including the owned territories for   the race (and thus changes as a race gains or loses territories).
---  This function will throw an exception if called outside of a Meta Map game or an unknown race is specified.
---
--- *Source: LuaMetaMap.cpp*
---@param raceName string
---@return integer
function MetaMap_GetRaceStartingSquadsCount(raceName) end

--- Returns the military strength of the given territory.
---
--- This function will throw an exception if called outside of a Meta Map game.
---
--- *Source: LuaMetaMap.cpp*
---@param _territoryIndex integer
---@return integer
function MetaMap_GetTerritoryMilitaryStrength(_territoryIndex) end

--- Returns the number of AI opponents in the given territory.
---
--- This function will throw an exception if called outside of a Meta Map game.
---
--- *Source: LuaMetaMap.cpp*
---@param territoryIndex integer
---@return integer
function MetaMap_GetTerritoryOpponentCount(territoryIndex) end

--- Returns the difficulty of a given opponent in a given territory.
---
--- Opponent indices are 0 based.  The return value will be one of: AD_Easy, AD_Standard, AD_Hard, AD_Advanced or AD_Insane.
---  This function will throw an exception if called outside of a Meta Map game.
---
--- *Source: LuaMetaMap.cpp*
---@param territoryIndex integer
---@param opponentIndex integer
---@return integer
function MetaMap_GetTerritoryOpponentDifficulty(territoryIndex, opponentIndex) end

--- Returns the race index for the owner of the given territory.
---
--- This function will throw an exception if called outside of a Meta Map game.
---
--- *Source: LuaMetaMap.cpp*
---@param _territoryIndex integer
---@return integer
function MetaMap_GetTerritoryOwnerRaceIndex(_territoryIndex) end

--- Returns true if there is a valid attacking territory index, false if not.
---
--- This function will throw an exception if called outside of a Meta Map game.
---
--- *Source: LuaMetaMap.cpp*
---@return boolean
function MetaMap_IsAttackingTerritoryIndexValid() end

--- Returns whether or not this is a coop game
---
--- This function will throw an exception if called outside of a Meta Map game.
---
--- *Source: LuaMetaMap.cpp*
---@return boolean
function MetaMap_IsCoop() end

--- Returns true if the given territory is currently a stronghold, false otherwise.
---
--- This function will throw an exception if called outside of a Meta Map game.
---
--- *Source: LuaMetaMap.cpp*
---@param _territoryIndex integer
---@return boolean
function MetaMap_IsTerritoryStronghold(_territoryIndex) end

--- Updates the wargear research for the given player.
---
--- This function will throw an exception if called outside of a Meta Map game.
---
--- *Source: LuaMetaMap.cpp*
function MetaMap_UpdatePlayerWargear() end

-- from LuaModifier.cpp

--- Applies an entity modifier to an entity.
---
--- *Source: LuaModifier.cpp*
---@param modifier ScarModifier
---@param entity EntityID
---@return integer
function Modifier_ApplyToEntity(modifier, entity) end

--- Applies a player modifier to a player.
---
--- *Source: LuaModifier.cpp*
---@param modifier ScarModifier
---@param player PlayerID
---@return integer
function Modifier_ApplyToPlayer(modifier, player) end

--- Applies a squad modifier to a squad.
---
--- *Source: LuaModifier.cpp*
---@param modifier ScarModifier
---@param squad SquadID
---@return integer
function Modifier_ApplyToSquad(modifier, squad) end

--- Returns a modifier that you can apply to stuff.
---
--- applicationType : MAT_EntityType, MAT_Entity, MAT_Player, MAT_Squad, MAT_SquadType, or MAT_WeaponType.
---  modtype : Name of modifier from the modifiers table in the attribute editor.
---  usageType : MUT_Addition, MUT_Multiplication or MUT_Enable.
---  exclusive :
---  value : The value for the modifier.
---  extname : The name of the entity/squad/weapon blueprint for when the applicationType is MAT_EntityType, MAT_SquadType or MAT_WeaponType.
---
--- *Source: LuaModifier.cpp*
---@param applicationType integer
---@param modtype string
---@param usageType integer
---@param exclusive boolean
---@param value number
---@param extname string
---@return ScarModifier
function Modifier_Create(applicationType, modtype, usageType, exclusive, value, extname) end

--- Todo.
---
--- *Source: LuaModifier.cpp*
---@param modifier ScarModifier
function Modifier_Destroy(modifier) end

--- Enable / Disable a particular modifier FX for all items in a group
---
--- Pass iModifier_EGroup_FX_Enablen the egroup, along with the name of the FX (Entity) Modifier, and whether to disable (0) or enable (1) it
---
--- *Source: LuaModifier.cpp*
---@param egroup EGroupID
---@param modifierName string
---@param bEnable boolean
function Modifier_EGroup_FX_Enable(egroup, modifierName, bEnable) end

--- Checks for the existance of a modifier.
---
--- *Source: LuaModifier.cpp*
---@param modifierID integer
---@return boolean
function Modifier_Exists(modifierID) end

--- Enable / Disable a particular modifier FX for a single entity
---
--- Pass iModifier_EGroup_FX_Enablen the egroup, along with the name of the FX (Entity) Modifier, and whether to disable (0) or enable (1) it
---
--- *Source: LuaModifier.cpp*
---@param entity EntityID
---@param modifierName string
---@param bEnable boolean
function Modifier_FX_Enable(entity, modifierName, bEnable) end

--- Returns whether an entity has the specified modifier
---
--- *Source: LuaModifier.cpp*
---@param entity EntityID
---@param modifierType string
---@return boolean
function Modifier_IsAppliedToEntity(entity, modifierType) end

--- Enabled or disable a particular modifier FX for all entities owned by a player
---
--- *Source: LuaModifier.cpp*
---@param player PlayerID
---@param modifierName string
---@param enable boolean
function Modifier_Player_FX_Enable(player, modifierName, enable) end

--- Remove an applied modifier.
---
--- You will need the modID returned from Modifier_ApplyToPlayer etc
---
--- *Source: LuaModifier.cpp*
---@param modID integer
function Modifier_Remove(modID) end

--- Enable / Disable a particular modifier FX for all items in a group
---
--- Pass in the sgroup, along with the name of the FX (Entity) Modifier, and whether to disable (0) or enable (1) it
---
--- *Source: LuaModifier.cpp*
---@param sgroup SGroupID
---@param modifierName string
---@param bEnable boolean
function Modifier_SGroup_FX_Enable(sgroup, modifierName, bEnable) end

--- Toggle a particular modifier FX for all items in a group
---
--- Pass in the sgroup, along with the name of the FX (Entity) Modifier to toggle
---
--- *Source: LuaModifier.cpp*
---@param sgroup SGroupID
---@param modifierName string
function Modifier_SGroup_FX_Toggle(sgroup, modifierName) end

-- from LuaNISletAnim.cpp

--- Blends out of the current camera path with the supplied blendOut duration (in seconds).
---
--- In order to call this, you must have called CPath_Start() or one of the CPath_Cut functions
---
---   This function can be called through a CTRL and waited on.
---
--- *Source: LuaNISletAnim.cpp*
---@param blendOut number
function CPath_Blendout(blendOut) end

--- Cuts to a new section of a started path, OR to a section of a different path.
---
--- This function can be called through a CTRL and waited on.
---
--- *Source: LuaNISletAnim.cpp*
---@param cpath string
---@param fromkeyframe string
---@param tokeyframe string
function CPath_CutToPath(cpath, fromkeyframe, tokeyframe) end

--- Instantly cut to a point on a path.
---
--- Since this function is instant, it makes no sense to call it through a CTRL
---
--- *Source: LuaNISletAnim.cpp*
---@param cpath string
---@param keyframe string
function CPath_CutToPoint(cpath, keyframe) end

--- Returns true if a camera is playing OR waiting to be blended out
---
--- *Source: LuaNISletAnim.cpp*
---@return boolean
function CPath_IsPlaying() end

--- Starts a camera path immediately.
---
--- Only one camera path can be started at a time.  Once a camera path is started, it is ok to cut to a different path or to play sections from the same path.
---
---   Once the frame specified by tokeyframe has been reached, the camera will pause and hold on that position until you cut to another section or it blends out using the blendout value.
---
---  The keyframe strings represent the names of the keyframes as specified in the ME.
---
---  This function can be called through a CTRL and waited on.
---
--- *Source: LuaNISletAnim.cpp*
---@param cpath string
---@param fromkeyframe string
---@param tokeyframe string
function CPath_Start(cpath, fromkeyframe, tokeyframe) end

--- Removes the attached entity from the path
---
--- *Source: LuaNISletAnim.cpp*
---@param eanimname string
function EPath_Finish(eanimname) end

--- Moves/Rotates an entity group along a path. Entity remains at end of path until EPath_Finish is called.
---
--- Works the same as SPath_Start
---
--- *Source: LuaNISletAnim.cpp*
---@param eanimname string
---@param fromkeyframe string
---@param tokeyframe string
---@param egroup EGroupID
function EPath_Start(eanimname, fromkeyframe, tokeyframe, egroup) end

--- Moves/Rotates an entity group along a path.
---
--- Works the same as SPath_StartEx
---
--- *Source: LuaNISletAnim.cpp*
---@param eanimname string
---@param fromkeyframe string
---@param tokeyframe string
---@param egroup EGroupID
---@param speed number
---@param autoFinish boolean
function EPath_StartEx(eanimname, fromkeyframe, tokeyframe, egroup, speed, autoFinish) end

--- Returns true if a fade is currently playing.
---
--- *Source: LuaNISletAnim.cpp*
---@return boolean
function Fade_IsFading() end

--- Starts a black screen fade.  Set in to true to fade in and set to false to fade out.
---
--- This function can be called through a CTRL object and waited on.  Will not fade out if the screen is already faded.
---
--- *Source: LuaNISletAnim.cpp*
---@param timeSecs number
---@param in boolean
function Fade_Start(timeSecs, in) end

--- Starts a color screen fade. Screen will fade to passed in color. Set in to true to fade in and set to false to fade out.
---
--- This function can be called through a CTRL object and waited on.  Will not fade out if the screen is already faded.
---
--- *Source: LuaNISletAnim.cpp*
---@param timeSecs number
---@param color Colour
---@param in boolean
function Fade_StartColor(timeSecs, color, in) end

--- Wait for a path to reach a certain keyframe.  Can be used on any type of path anim.
---
--- *Source: LuaNISletAnim.cpp*
---@param pathname string
---@param keyframe string
function Path_KeyframeWait(pathname, keyframe) end

--- Call through a CTRL and use a wait to wait until a squad is a certain size.
---
--- Use countType: CT_Equal, CT_LessThanEqual, CT_GreaterThanEqual.  Note: Function must be called through a CTRL.
---
--- *Source: LuaNISletAnim.cpp*
---@param sgroup SGroupID
---@param count integer
---@param countType integer
function SGroup_WaitForCount(sgroup, count, countType) end

--- Removes the attached squads from the path and puts them back into pathfinding.
---
--- *Source: LuaNISletAnim.cpp*
---@param sanimname string
function SPath_Finish(sanimname) end

--- Moves a squad along a path.
---
--- The squad will move along the terrain regardless of the height of the path. The squad will also be removed from pathfinding while traversing the path.
---
---   This function can be called through a CTRL object and waited on.
---
--- *Source: LuaNISletAnim.cpp*
---@param sanimname string
---@param fromkeyframe string
---@param tokeyframe string
---@param sgroup SGroupID
function SPath_Start(sanimname, fromkeyframe, tokeyframe, sgroup) end

--- Starts a squad path with a speed override.
---
--- This function works just like SPath_Start.  The speed override use this speed for all points. Set speed to zero to use default.
---
---   Setting autoFinsih to true will automagically call SPath_Finish at the end of the path.
---
--- *Source: LuaNISletAnim.cpp*
---@param sanimname string
---@param fromkeyframe string
---@param tokeyframe string
---@param sgroup SGroupID
---@param speed number
---@param autoFinish boolean
function SPath_StartEx(sanimname, fromkeyframe, tokeyframe, sgroup, speed, autoFinish) end

-- from LuaPlayer.cpp

--- Returns an EntityGroup containing all the players entities.
---
--- This function returns a 'global' entity group with the name '__Player%dEntities', where %d is the player ID.  This means that you should never need to destroy it./n However, if you do destroy it, it will be recreated the next time this function is called.
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@return string
function *Player_GetEntities(player) end

--- Returns a SquadGroup containing all the players units.
---
--- This function returns a 'global' squad group with the name '__Player%dSquads', where %d is the player ID.  This means that you should never need to destroy it./n However, if you do destroy it, it will be recreated the next time this function is called.
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@return string
function *Player_GetSquads(player) end

--- Returns true if a player can see a given entity
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@param entity EntityID
---@return boolean
function Player_CanSeeEntity(player, entity) end

--- Returns true if a player can see a given position.
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@param pos Position
---@return boolean
function Player_CanSeePosition(player, pos) end

--- Returns true if a player can see ALL or ANY troopers in a given squad
---
--- Set all to true to check that ALL troopers are visible or set to false to check for ANY.
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@param squad SquadID
---@param all boolean
---@return boolean
function Player_CanSeeSquad(player, squad, all) end

--- DeSpawn a players buildings and/or squads depending on the booleans you pass in
---
--- *Source: LuaPlayer.cpp*
---@param pPlayer PlayerID
---@param bDespawnBuildings boolean
---@param bDespawnSquads boolean
function Player_DespawnAll(pPlayer, bDespawnBuildings, bDespawnSquads) end

--- Destroy a players buildings, squads and disown their SPs depending on the booleans you pass in
---
--- *Source: LuaPlayer.cpp*
---@param pPlayer PlayerID
---@param bDestroyBuildings boolean
---@param bDestroySquads boolean
---@param bDisownSPs boolean
function Player_DestroyAll(pPlayer, bDestroyBuildings, bDestroySquads, bDisownSPs) end

--- Returns a player given a player id from the ME.  NOTE: this is a temp function, and will be replaced with Player_FromName
---
--- *Source: LuaPlayer.cpp*
---@param id integer
---@return PlayerID
function Player_FromId(id) end

--- Get a player using their name from the Mission Editor.  NOT DONE YET
---
--- *Source: LuaPlayer.cpp*
---@param name string
---@return PlayerID
function Player_FromName(name) end

--- Returns the current number of active squads the player has.
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@return integer
function Player_GetActiveSquadCount(player) end

--- Add all players entities that are in proximity to a position into an entity group.  Group will be created if it doesnt already exist.
---
--- *Source: LuaPlayer.cpp*
---@param p PlayerID
---@param egroupName string
---@param pos Position
---@param proximity number
function Player_GetAllEntitiesNearPos(p, egroupName, pos, proximity) end

--- Add all players squads that are in proximity to a point into a squad group.  Group will be created if it doesnt already exist.
---
--- *Source: LuaPlayer.cpp*
---@param p PlayerID
---@param sgroupName string
---@param pos Position
---@param proximity number
function Player_GetAllSquadsNearPos(p, sgroupName, pos, proximity) end

--- Returns the current number of spawned buildings the player has.
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@return integer
function Player_GetBuildingSpawnedCount(player) end

--- Use capType CT_SquadCap to get current squad cap or CT_Support to get current support cap.
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@param capType integer
---@return integer
function Player_GetCurrentSquadCap(player, capType) end

--- Returns the players UI name.
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@return LocString
function Player_GetDisplayName(player) end

--- Returns an EntityGroup containing all the players entities.
---
--- This function returns a 'global' entity group with the name '__Player%dEntities', where %d is the player ID.  This means that you should never need to destroy it./n However, if you do destroy it, it will be recreated the next time this function is called.
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@return EGroupID
function Player_GetEntities(player) end

--- Returns the number of entities a player currently owns
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@return integer
function Player_GetEntityCount(player) end

--- Returns the name of an entity a player currently owns
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@param index integer
---@return string
function Player_GetEntityName(player, index) end

--- Returns the id of the player
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@return integer
function Player_GetID(player) end

--- Use capType CT_SquadCap to get max squad cap or CT_Support to get max support cap.
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@param capType integer
---@return integer
function Player_GetMaxSquadCap(player, capType) end

--- Returns the number of strategic objectives this player owns
---
--- *Source: LuaPlayer.cpp*
---@param p PlayerID
---@return integer
function Player_GetNumStrategicObjectives(p) end

--- Returns the number of strategic points with blueprint strategic_point_flag
---
--- *Source: LuaPlayer.cpp*
---@param p PlayerID
---@return integer
function Player_GetNumStrategicPoints(p) end

--- Returns the race index for the given player.
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@return integer
function Player_GetRace(player) end

--- Returns the ME name of the race for a given player.
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@return string
function Player_GetRaceName(player) end

--- Returns the relationship between 2 players.  Possible relationships are R_ENEMY, R_ALLY, R_NEUTRAL, R_UNDEFINED.
---
--- *Source: LuaPlayer.cpp*
---@param player1 PlayerID
---@param player2 PlayerID
---@return integer
function Player_GetRelationship(player1, player2) end

--- Returns RS_Locked,RS_Available,RS_Researching or RS_Complete
---
--- RS_Locked - Research is restricted or not yet available.
---  RS_Researching - Research is queued up in one of the players buildings.
---  RS_Complete - Research has been granted or completed.
---  RS_Available - Research is available to build in one or more of the players buildings
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@param researchName string
---@return integer
function Player_GetResearchState(player, researchName) end

--- Returns the amount of resources a given player has.  Possible resource types are RT_Requisition, RT_Power, RT_Pop
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@param resourceType integer
---@return number
function Player_GetResource(player, resourceType) end

--- Returns the number of squads a player currently owns
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@return integer
function Player_GetSquadCount(player) end

--- Returns a SquadGroup containing all the players units.
---
--- This function returns a 'global' squad group with the name '__Player%dSquads', where %d is the player ID.  This means that you should never need to destroy it./n However, if you do destroy it, it will be recreated the next time this function is called.
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@return SGroupID
function Player_GetSquads(player) end

--- Returns the starting position for a given player
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@return Position
function Player_GetStartPosition(player) end

--- Add all of a players strategic points to a egroup
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@param egroup EGroupID
function Player_GetStrategicPoints(player, egroup) end

--- Get the team a player is on
---
--- *Source: LuaPlayer.cpp*
---@param p PlayerID
---@return integer
function Player_GetTeam(p) end

--- Returns the current number of units the player has.
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@return integer
function Player_GetUnitCount(player) end

--- Returns the current number of spawned units the player has.
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@return integer
function Player_GetUnitSpawnedCount(player) end

--- Grants and applies a research item to a given player
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@param researchName string
function Player_GrantResearch(player, researchName) end

--- Returns true if player is still alive and false if player is dead.  Will error if playerIdx is an invalid index.
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@return boolean
function Player_IsAlive(player) end

--- A player is totally annihilated when they have no 'killable' squads or entities.
---
--- A killable squad or entity is any squad or entity with health (captured strategic points do not count)
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@return boolean
function Player_IsAnnihilated(player) end

--- Kill a player and reveals their FOW.  Will error if playerIdx is an invalid index.
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
function Player_Kill(player) end

--- Restricts an ability for a player.
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@param abilityName string
function Player_RestrictAbility(player, abilityName) end

--- Restrict an addon for a player
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@param name string
function Player_RestrictAddOn(player, name) end

--- Restricts a building for a player.
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@param name string
function Player_RestrictBuilding(player, name) end

--- Restricts an ability for an entity blueprint for a player.
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@param abilityName string
---@param blueprint string
function Player_RestrictEntityAbility(player, abilityName, blueprint) end

--- Restricts a research item for a player
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@param name string
function Player_RestrictResearch(player, name) end

--- Restricts a squad for a player.
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@param name string
function Player_RestrictSquad(player, name) end

--- Restricts an ability for a squad blueprint for a player.
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@param abilityName string
---@param blueprint string
function Player_RestrictSquadAbility(player, abilityName, blueprint) end

--- Restrict a weapon for a player.
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@param name string
function Player_RestrictWeapon(player, name) end

--- Sets the maximum population resource for the player.
---
--- This value is absolute - any modifiers are capped to this value.
---
--- *Source: LuaPlayer.cpp*
---@param pPlayer PlayerID
---@param maxCap integer
function Player_SetMaxPopulationRes(pPlayer, maxCap) end

--- Sets the maximum squad cap for the player.
---
--- This value is absolute - any modifiers are capped to this value.
---
--- *Source: LuaPlayer.cpp*
---@param pPlayer PlayerID
---@param maxCap integer
function Player_SetMaxSquadCap(pPlayer, maxCap) end

--- Sets the maximum support cap for the player.
---
--- This value is absolute - any modifiers are capped to this value.
---
--- *Source: LuaPlayer.cpp*
---@param pPlayer PlayerID
---@param maxCap integer
function Player_SetMaxSupportCap(pPlayer, maxCap) end

--- Set the resource amount for a given player.  Ignores income cap and resource sharing.
---
--- Possible resource types are RT_Requisition, RT_Power, RT_Pop.
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@param resourceType integer
---@param amt number
function Player_SetResource(player, resourceType, amt) end

--- Put a player in a team. Use TEAM_NEUTRAL as the team_id to set the player as neutral
---
--- *Source: LuaPlayer.cpp*
---@param p PlayerID
---@param team_id integer
function Player_SetTeam(p, team_id) end

--- Unrestricts an ability for the player.
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@param abilityName string
function Player_UnRestrictAbility(player, abilityName) end

--- Unrestricts an addon for a player
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@param name string
function Player_UnRestrictAddOn(player, name) end

--- Unrestricts a building for a player.
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@param name string
function Player_UnRestrictBuilding(player, name) end

--- Unrestricts an ability for an entity blueprint for the player.
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@param abilityName string
---@param blueprint string
function Player_UnRestrictEntityAbility(player, abilityName, blueprint) end

--- Unrestricts a research item for a player
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@param name string
function Player_UnRestrictResearch(player, name) end

--- Unrestricts a squad for a player.
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@param name string
function Player_UnRestrictSquad(player, name) end

--- Unrestricts an ability for a squad blueprint for the player.
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@param abilityName string
---@param blueprint string
function Player_UnRestrictSquadAbility(player, abilityName, blueprint) end

--- Unrestricts a weapon for a player.
---
--- *Source: LuaPlayer.cpp*
---@param player PlayerID
---@param name string
function Player_UnRestrictWeapon(player, name) end

-- from LuaRuleSystem.cpp

--- Add a rule to be executed every frame
---
--- *Source: LuaRuleSystem.cpp*
---@param f function
function Rule_Add(f) end

--- Add a rule to be executed every frame, executed for the first time after 'delay' seconds
---
--- *Source: LuaRuleSystem.cpp*
---@param f function
---@param delay number
function Rule_AddDelay(f, delay) end

--- Add a rule to be executed at every 'interval' seconds
---
--- *Source: LuaRuleSystem.cpp*
---@param f function
---@param interval number
function Rule_AddInterval(f, interval) end

--- Add a rule to be executed at every 'interval' seconds, executed for the first time after 'delay' seconds
---
--- *Source: LuaRuleSystem.cpp*
---@param f function
---@param interval number
---@param delay number
function Rule_AddIntervalDelay(f, interval, delay) end

--- Add a rule to be executed 'calls' times, at every 'interval' seconds
---
--- *Source: LuaRuleSystem.cpp*
---@param f function
---@param interval number
---@param calls integer
function Rule_AddIntervalEx(f, interval, calls) end

--- Add a rule to be executed 'calls' times, at every 'interval' seconds, executed for the first time after 'delay' seconds
---
--- *Source: LuaRuleSystem.cpp*
---@param f function
---@param interval number
---@param calls integer
---@param delay number
function Rule_AddIntervalExDelay(f, interval, calls, delay) end

--- Add a rule to be executed once, after 'delay' seconds
---
--- *Source: LuaRuleSystem.cpp*
---@param f function
---@param delay number
function Rule_AddOneShot(f, delay) end

--- Test if a rule is currently active
---
--- *Source: LuaRuleSystem.cpp*
---@param f function
---@return boolean
function Rule_Exists(f) end

--- Remove a currently active rule
---
--- *Source: LuaRuleSystem.cpp*
---@param f function
function Rule_Remove(f) end

--- Kills all rules.
---
--- *Source: LuaRuleSystem.cpp*
function Rule_RemoveAll() end

--- Removes all rules other than the listed exceptions.
---
--- *Source: LuaRuleSystem.cpp*
---@param exceptions table
function Rule_RemoveAllExcept(exceptions) end

-- from LuaSimDynamics.cpp

--- Works like Squad_FacePosition.  All Squads will face the same direction, with the squad the closest to the center determining the direction.
---
--- This function works on spawned squads only.
---
--- *Source: LuaSimDynamics.cpp*
---@param sgroup SGroupID
---@param pos Position
function SGroup_FacePosition(sgroup, pos) end

--- Orders each squad in the group to face a random direction.
---
--- *Source: LuaSimDynamics.cpp*
---@param sgroup SGroupID
function SGroup_FaceRandom(sgroup) end

--- Set the rotation of all troopers in a squad to face the position.
---
--- The leader of the squad will face the position and all other troops will use the same rotation as their leader.
---
---   This function works on spawned squads only.
---
--- *Source: LuaSimDynamics.cpp*
---@param squad SquadID
---@param pos Position
function Squad_FacePosition(squad, pos) end

--- Get 2 squads to face each other.
---
--- The leaders of the squad will face each other, and all troopers in the squad will use the same rotation as their leader.
---
---   This function works on spawned squads only.
---
--- *Source: LuaSimDynamics.cpp*
---@param squad1 SquadID
---@param squad2 SquadID
function Squad_FaceSquad(squad1, squad2) end

-- from LuaSound.cpp

--- Plays a sound from the Data:Sound folder.  Use the path relative to Sound folder without the file extension.
---
--- Returns a soundID you can use to stop the sound etc.
---   Example:
---
---      local id = Sound_Play( "Speech/SpaceMarine/AddOnComplete" )
---      Sound_Stop( id )
---
--- *Source: LuaSound.cpp*
---@param name string
---@return integer
function Sound_Play(name) end

--- Play specified stinger right now.
---
--- While a stinger is playing, the music track is muted but keeps playing
---
--- *Source: LuaSound.cpp*
---@param track string
function Sound_PlayStinger(track) end

--- Add track to the requested playlist channel.
---
--- Valid channel values: PC_Music or PC_Ambient
---   Valid tracks are the sound files from the Data:Sound/Music folder, without the file extension.
---
--- *Source: LuaSound.cpp*
---@param channel SoundChannel
---@param track string
function Sound_PlaylistAddTrack(channel, track) end

--- Removes all tracks from the requested playlist channel.
---
--- Valid channel values: PC_Music or PC_Ambient
---
--- *Source: LuaSound.cpp*
---@param channel SoundChannel
function Sound_PlaylistClear(channel) end

--- Mutes playlist channel down to specified volume.
---
--- Valid channel values: PC_Music or PC_Ambient
---
--- *Source: LuaSound.cpp*
---@param channel SoundChannel
function Sound_PlaylistMute(channel) end

--- Play specified track right now.
---
--- Valid channel values: PC_Music or PC_Ambient
---
--- *Source: LuaSound.cpp*
---@param channel SoundChannel
---@param track string
function Sound_PlaylistPlayNow(channel, track) end

--- Set silence intervals between tracks for the requested playlist channel.  the actual interval time will be randomly chosen between min and max, where min is greater of equal to 0, and min is less than of equal to max
---
--- Valid channel values: PC_Music or PC_Ambient
---
--- *Source: LuaSound.cpp*
---@param channel SoundChannel
---@param min number
---@param max number
function Sound_PlaylistSetSilence(channel, min, max) end

--- Set how the next track to be played will be chosen, ordered (true) or random (false)
---
--- Valid channel values: PC_Music or PC_Ambient
---
--- *Source: LuaSound.cpp*
---@param channel SoundChannel
---@param sequential boolean
function Sound_PlaylistSetorder(channel, sequential) end

--- Starts playing requested playlist channel.
---
--- Valid channel values: PC_Music or PC_Ambient
---
--- *Source: LuaSound.cpp*
---@param channel SoundChannel
function Sound_PlaylistStart(channel) end

--- Stops playing requested playlist channel.
---
--- Valid channel values: PC_Music or PC_Ambient
---
--- *Source: LuaSound.cpp*
---@param channel SoundChannel
function Sound_PlaylistStop(channel) end

--- Restores muted playlist channel.
---
--- Valid channel values: PC_Music or PC_Ambient
---
--- *Source: LuaSound.cpp*
---@param channel SoundChannel
function Sound_PlaylistUnMute(channel) end

--- Changes the volume of a sound that was started using Sound_Play.
---
--- Use the soundID returned from Sound_Play.  Volume should have range [0.0, 1.0] and fadeTime should be greater or equal to zero.
---
--- *Source: LuaSound.cpp*
---@param soundID integer
---@param volume number
---@param fadeTime number
function Sound_SetSoundVolume(soundID, volume, fadeTime) end

--- Sets a multiply modifier for the ambient sound volume
---
--- The modifiers must be in the [0.0, 1.0] range
---
--- *Source: LuaSound.cpp*
---@param multiplier number
function Sound_SetVolumeModifierAmbient(multiplier) end

--- Sets a multiply modifier for the music volume
---
--- The modifiers must be in the [0.0, 1.0] range
---
--- *Source: LuaSound.cpp*
---@param multiplier number
function Sound_SetVolumeModifierMusic(multiplier) end

--- Sets a multiply modifier for the sfx volume
---
--- The modifiers must be in the [0.0, 1.0] range
---
--- *Source: LuaSound.cpp*
---@param multiplier number
function Sound_SetVolumeModifierSfx(multiplier) end

--- Sets a multiply modifier for the voice volume
---
--- The modifiers must be in the [0.0, 1.0] range
---
--- *Source: LuaSound.cpp*
---@param multiplier number
function Sound_SetVolumeModifierVoice(multiplier) end

--- Stops a sound that was started using Sound_Play.
---
--- Use the soundID returned from Sound_Play.
---
--- *Source: LuaSound.cpp*
---@param soundID integer
function Sound_Stop(soundID) end

--- Resets the volume modifiers
---
--- *Source: LuaSound.cpp*
function Sound_VolumeModifiersReset() end

-- from LuaSquad.cpp

--- The SquadID for the given unique squad id.  Return nil if id is invalid.
---
--- NOTE: This function is for internal use only and shouldn't be called in Scar code.
---
--- *Source: LuaSquad.cpp*
---@param id integer
---@return SquadID
function *Squad_GetSquadFromGameID(id) end

--- Returns true if any of the victims were attacked by any of the attackers in the last one second.
---
--- *Source: LuaSquad.cpp*
---@param sg SGroupID
---@return boolean
function SGroup_IsVehicle(sg) end

--- Returns true if any of the victims were attacked by any of the attackers in the last one second.
---
--- *Source: LuaSquad.cpp*
---@param victims SGroupID
---@param attackers SGroupID
---@return boolean
function SGroup_WasAttacked(victims, attackers) end

--- Add a leader to a squad.  Returns true on success and false on error.
---
--- This function will return false for squads that do no have reinforce extension.
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@return boolean
function Squad_AddLeader(squad) end

--- Add a leader to a squad using the given index in the squad's leader extension info.  Returns true on success and false on error.
---
--- This function will return false for squads that do no have reinforce extension.  Only values from 1 to 4 are valid for the index.
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@param index integer
---@return boolean
function Squad_AddLeaderAtIndex(squad, index) end

--- Poopy poops
---
--- Stinky Tooter
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@param type integer
---@param pos Position
---@return boolean
function Squad_BombingRun(squad, type, pos) end

--- Returns true if a given entity is in the squads sight radius and is NOT infiltrated.
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@param entity EntityID
---@return boolean
function Squad_CanSeeEntity(squad, entity) end

--- Returns true if a target squad is in the squads sight radius and is NOT infiltrated.
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@param target SquadID
---@return boolean
function Squad_CanSeeSquad(squad, target) end

--- Returns true if this squadron can upgrade with given unit.
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@param weaponname string
---@return boolean
function Squad_CanUpgradeWeapon(squad, weaponname) end

--- Returns the number of units currently in a squad (spawned AND despawned!!)
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@return integer
function Squad_Count(squad) end

--- Counts the number of leaders in the squad
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@return integer
function Squad_CountLeaders(squad) end

--- Create a squad and assign it to a player.
---
--- This will create a squad of size 'loadoutCount' and of unit type 'trooper_base' (from squad blueprint)
---  Note: if loadoutCount is zero, the min loadout will be used.  loadoutCount will be clipped to the max loadout if it ever exceeds it.
---
--- *Source: LuaSquad.cpp*
---@param squadblueprint string
---@param player PlayerID
---@param pos Position
---@param loadoutCount integer
---@return SquadID
function Squad_Create(squadblueprint, player, pos, loadoutCount) end

--- Poopy poops
---
--- Stinky Tooter
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
function Squad_DanceOfDeath(squad) end

--- Despawn the entire squad at its current position.
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
function Squad_DeSpawn(squad) end

--- Poopy poops
---
--- Stinky Tooter
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
function Squad_Deceive(squad) end

--- Deep strike a group of DESPAWNED squads to a given location.
---
--- This function will do nothing if the squad is in SquadStateLoad (loading a transport)
---
---   ebpname is the name of the building with the deepstrike_ext (ie space_marine_orbital_relay)
---
---  you should never assume that this function will deepstrike your units because there may not be enough room!
---
--- *Source: LuaSquad.cpp*
---@param sgroup SGroupID
---@param ebpname string
---@param pos Position
---@return boolean
function Squad_DeepStrikeToPos(sgroup, ebpname, pos) end

--- Remove an squad from the world and destroy it.
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
function Squad_Destroy(squad) end

--- Detaches an attached squad and returns the new detached squad.
---
--- To call this function the squad must have an attach extension and an attached squad.
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@return SquadID
function Squad_Detach(squad) end

--- Returns true if a squad with the given game id exists in the world
---
--- To get equivalent functionality, put your squad into a squad group and check if that squad group is empty.
---
--- *Source: LuaSquad.cpp*
---@param squadGameID integer
---@return boolean
function Squad_Exists(squadGameID) end

--- Forces all weapons on all hardpoints to default for all members of the squad
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
function Squad_ForceResetWeapons(squad) end

--- Force a given number of troops in a squad to upgrade their weapons regardless of requirments.
---
--- Returns the number of troopers succesfully upgraded.  Pass in zero to upgrade all troops.
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@param weaponname string
---@param nUpgrades integer
---@return integer
function Squad_ForceUpgradeWeapons(squad, weaponname, nUpgrades) end

--- Returns the active squad command.  Valid squad commands are SQUADSTATEID_Idle, SQUADSTATEID_Move, SQUADSTATEID_Construction, SQUADSTATEID_Capture, SQUADSTATEID_Combat, SQUADSTATEID_Melee, SQUADSTATEID_CombatStance, SQUADSTATEID_Patrol
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@return integer
function Squad_GetActiveCommand(squad) end

--- Returns the name of the attached squad blueprint (from the attribute editor)
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@return string
function Squad_GetAttachedBlueprintName(squad) end

--- Returns a string containing the base unit name for the current squadron
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@return string
function Squad_GetBaseUnitName(squad) end

--- Returns the name of the squad blueprint (from the attribute editor)
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@return string
function Squad_GetBlueprintName(squad) end

--- Find out what type of cover a given trooper in a squad is in.
---
--- Trooper index is in the range [0..n-1]
---   Returns CT_None if trooper does not have cover extension Returns one of the possible cover types: CT_None, CT_Light, CT_Heavy, CT_Negative, CT_Blocking, CT_Stealth
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@param trooperIndex integer
---@return integer
function Squad_GetCoverType(squad, trooperIndex) end

--- Gets the name of the formation for the given squad
---
--- If the squad does not have formation, it returns the default formation BLOCK
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@return string
function Squad_GetFormation(squad) end

--- Returns an integer containing the unqiue squad ID for this squad.
---
--- NOTE: This function is for internal use only and shouldn't be called in Scar code.
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@return integer
function Squad_GetGameID(squad) end

--- Returns the current health of a squad.
---
--- The current health of the squad is the total health of each entity in that squad.
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@return number
function Squad_GetHealth(squad) end

--- Returns true if any entities in a squad are invulnerable.
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@return boolean
function Squad_GetHealthInvulnerable(squad) end

--- Returns the max health of the squad.
---
--- The max health of a squad is the max health of each entity in that squad.  This means that the max health of a squad may change when entities are added or removed from the squad.
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@return number
function Squad_GetHealthMax(squad) end

--- Returns the entity blueprint name for the leader of a squad.
---
--- If a unit is attached to the squad, this will always be the leader otherwise the sergeant, then the a trooper.
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@return string
function Squad_GetLeaderBP(squad) end

--- Returns the max number of units allowed in the squadron (from squad_loadout_ext)
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@return integer
function Squad_GetMax(squad) end

--- Returns the minimum number of units in the squadron (from squad_loadout_ext)
---
--- *Source: LuaSquad.cpp*
---@param squad_blueprint string
---@return integer
function Squad_GetMaxFromName(squad_blueprint) end

--- Returns the minimum number of units in the squadron (from squad_loadout_ext)
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@return integer
function Squad_GetMin(squad) end

--- Returns the minimum number of units in the squadron (from squad_loadout_ext)
---
--- *Source: LuaSquad.cpp*
---@param squad_blueprint string
---@return integer
function Squad_GetMinFromName(squad_blueprint) end

--- Returns the current morale of a squad as a percent [0.0, 1.0]
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@return number
function Squad_GetMorale(squad) end

--- Returns true if a squad is invulnerable to morale damage.
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@return boolean
function Squad_GetMoraleInvulnerable(squad) end

--- Returns the Player owner of the given squad. Squad MUST NOT be owned by the world.
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@return PlayerID
function Squad_GetPlayerOwner(squad) end

--- Returns the average position of the spawned units in the squad. The position is currently a lua table with three entries (x, y, z)
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@return Position
function Squad_GetPosition(squad) end

--- Returns the average position of the despawned AND spawned units in the squad.
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@return Position
function Squad_GetPositionDeSpawned(squad) end

--- Returns the movement speed of the LEADER in km per hour
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@return number
function Squad_GetSpeed(squad) end

--- Returns the max number of upgrades this squadron can get (from squad_loadout_ext)
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@return integer
function Squad_GetUpgradeMax(squad) end

--- Returns true if the given squad is owned by the world
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@return boolean
function Squad_GetWorldOwner(squad) end

--- Poopy poops
---
--- Stinky Tooter
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@param pos Position
function Squad_GrandIllusion(squad, pos) end

--- Checks if a squads base trooper blueprint has the given armour type.
---
--- armourTypeName param must be a valid armour type (type_armour) from the Attribute Editor.
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@param armourTypeName string
---@return boolean
function Squad_HasArmourType(squad, armourTypeName) end

--- Returns true if a squad has been reinforced with a leader.
---
--- NOTE: Does not check reinforce queue for queued leaders.
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@return boolean
function Squad_HasLeader(squad) end

--- Returns whether or not the squad has morale.
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@return boolean
function Squad_HasMorale(squad) end

--- Returns true if a squad has been attached.
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@return boolean
function Squad_IsAttached(squad) end

--- Returns true if ALL or ANY troopers in a squad are in cover
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@param all boolean
---@return boolean
function Squad_IsInCover(squad, all) end

--- Returns true if the squad is in a hold of any building in the buildings group
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@param buildingsGroup EGroupID
---@return boolean
function Squad_IsInHold(squad, buildingsGroup) end

--- Returns true if a squad has infiltrate enabled
---
--- This function will always return false for squads that do not have a squad_infiltration_ext
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@return boolean
function Squad_IsInfiltrated(squad) end

--- Returns true if morale is broken for a squad
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@return boolean
function Squad_IsMoraleBroken(squad) end

--- Returns true if the squad is currently reinforcing.  This function will return false if the squad does not have a reinforce ext.
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@return boolean
function Squad_IsReinforcing(squad) end

--- Returns true if the squad is currentyly reinforcing a leader.
---
--- This function will return false if the squad does not have a reinforce extension.
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@return boolean
function Squad_IsReinforcingLeader(squad) end

--- Returns true if any trooper in the squad was attacked in the last second.
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@return boolean
function Squad_IsUnderAttack(squad) end

--- Returns true if any trooper in the squad was attacked by the given player in the last second
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@param player PlayerID
---@return boolean
function Squad_IsUnderAttackByPlayer(squad, player) end

--- Returns true if the squad is currently upgrading.  This function will return false if the squad does not have a reinforce ext.
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@return boolean
function Squad_IsUpgrading(squad) end

--- Go through the squad and destroy all entities with matching blueprints
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@param entitybp string
function Squad_KillEntityWithBP(squad, entitybp) end

--- Poopy poops
---
--- Stinky Tooter
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@param pos Position
function Squad_Misdirect(squad, pos) end

--- Set the health of all troopers in a squad.  Health must be in range [0.0, 1.0]
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@param healthPercent number
function Squad_SetHealth(squad, healthPercent) end

--- Make a squad invulnerable to physical damage.
---
--- IMPORTANT: You must disable invulnerability for everytime you enable it!
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@param enabled boolean
function Squad_SetHealthInvulnerable(squad, enabled) end

--- Set the morale of all troopers in a squad.  Morale must be in range [0.0, 1.0]
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@param moralePercent number
function Squad_SetMorale(squad, moralePercent) end

--- Makes a squad invulnerable to morale damage.
---
--- IMPORTANT: You must disable morale invulnerability for everytime you enable it!
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@param enabled boolean
function Squad_SetMoraleInvulnerable(squad, enabled) end

--- Changes the owner of the given squad.
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@param owner PlayerID
function Squad_SetPlayerOwner(squad, owner) end

--- Sets the owner to the world for all entities in the squad.
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
function Squad_SetWorldOwner(squad) end

--- Poopy poops
---
--- Stinky Tooter
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
function Squad_SoulDestruction(squad) end

--- Poopy poops
---
--- Stinky Tooter
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
function Squad_SoulHarvest(squad) end

--- Spawn the entire squad at a given position
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@param pos Position
function Squad_Spawn(squad, pos) end

--- Turns off the rendering system for moving the squad around to make movement seem more appropriate for a vehicle.
---
--- Turns off the rendering system for moving the squad around to make movement seem more appropriate for a vehicle.
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
function Squad_TurnOffVehiclePhysics(squad) end

--- Find a upgradable trooper in a squad and upgrade its weapon.  Can only upgrade to weapons which have all requirements met.
---
--- Use Squad_ForceUpgradeWeapons to force all troops in a squad to upgrade their weapons regardless of requirments.
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@param weaponname string
---@return boolean
function Squad_UpgradeWeapon(squad, weaponname) end

--- Returns the current number of weapons in a squad.
---
--- *Source: LuaSquad.cpp*
---@param squad SquadID
---@param weaponname string
---@return integer
function Squad_WeaponCount(squad, weaponname) end

-- from LuaStatLogging.cpp

--- Amount of resources spent by player to acquire their army.  Possible resource types are RT_Requisition, RT_Power and RT_Pop.
---
--- *Source: LuaStatLogging.cpp*
---@param playerID PlayerID
---@param resourceType integer
---@return number
function Stats_ActiveArmyCost(playerID, resourceType) end

--- Amount of resources of the given type that the player with the given ID has gathered.  Possible resource types are RT_Requisition, RT_Power and RT_Pop.
---
--- *Source: LuaStatLogging.cpp*
---@param playerID integer
---@param resourceType integer
---@return integer
function Stats_PlayerResGathered(playerID, resourceType) end

--- Resource score gained by the specified player. Possible resource types are RT_Requisition, RT_Power and RT_Pop.
---
--- *Source: LuaStatLogging.cpp*
---@param playerID integer
---@param resourceType integer
---@return integer
function Stats_PlayerResScore(playerID, resourceType) end

--- Amount of resources of the given type that the player with the given ID has spent.  Possible resource types are RT_Requisition, RT_Power and RT_Pop.
---
--- *Source: LuaStatLogging.cpp*
---@param playerID integer
---@param resourceType integer
---@return integer
function Stats_PlayerResSpent(playerID, resourceType) end

--- Amount of resources of the given type that the player with the given ID has tributed.  Possible resource types are RT_Requisition, RT_Power and RT_Pop.
---
--- *Source: LuaStatLogging.cpp*
---@param playerID integer
---@param resourceType integer
---@return integer
function Stats_PlayerResTributed(playerID, resourceType) end

--- True if player has won.
---
--- *Source: LuaStatLogging.cpp*
---@param playerID integer
---@return boolean
function Stats_PlayerWon(playerID) end

-- from LuaUI.cpp

--- Creates a SCAR button represented by the widget with the passed in name and enabled/disabled based on the flag
---
--- Will fail if a scar button with the passed in widget has already been created
---
--- *Source: LuaUI.cpp*
---@param widgetName string
---@param enabled boolean
---@return ButtonID
function Button_Add(widgetName, enabled) end

--- Creates a SCAR button
---
--- The button will be represented by the widget with the passed in name and visible/hidden and enabled/diabled based on the passed in flags. This function will fail if a scar button associated with the passed in widget already exists.
---
--- *Source: LuaUI.cpp*
---@param widgetName string
---@param visible boolean
---@param enabled boolean
---@param modal boolean
---@return ButtonID
function Button_Add(widgetName, visible, enabled, modal) end

--- Creates a SCAR ability button showing the specified ability and enabled/disabled based on the flag
---
--- Will fail if specified ability does not exists or scar button the ability has already been created
---
--- *Source: LuaUI.cpp*
---@param abilityName string
---@param enabled boolean
---@return ButtonID
function Button_AddAbility(abilityName, enabled) end

--- Creates a SCAR ability button
---
--- The button will be visible/hidden and enabled/disabled based on the passed in flags
---
--- *Source: LuaUI.cpp*
---@param visible boolean
---@param enabled boolean
---@param modal boolean
---@return ButtonID
function Button_AddAbility(visible, enabled, modal) end

--- Checks whether the identified SCAR button currently exists
---
--- Will fail if the identified button does not exist
---
--- *Source: LuaUI.cpp*
---@param buttonID ButtonID
---@return boolean
function Button_Exists(buttonID) end

--- Checks whether the identified SCAR button is currently enabled
---
--- Will fail if the identified button does not exist
---
--- *Source: LuaUI.cpp*
---@param buttonID ButtonID
---@return boolean
function Button_GetEnabled(buttonID) end

--- Checks whether the identified SCAR button has been pressed
---
--- Will fail if the identified button does not exist
---
--- *Source: LuaUI.cpp*
---@param buttonID ButtonID
---@return boolean
function Button_GetPressed(buttonID) end

--- Adds the entity that was clicked to the passed in EGroup
---
--- Will return true if an entity was clicked on and false otherwise. Will fail if the identified button does not exist, if the button wasn't pressed, or if the button isn't modal.
---
--- *Source: LuaUI.cpp*
---@param buttonID ButtonID
---@param egroup EGroupID
---@return boolean
function Button_GetPressedEntity(buttonID, egroup) end

--- Return the position that was clicked
---
--- Will fail if the identified button does not exist, if the button wasn't pressed, or if the button isn't modal
---
--- *Source: LuaUI.cpp*
---@param buttonID ButtonID
---@return Position
function Button_GetPressedPosition(buttonID) end

--- Adds the squad that was clicked to the passed in SGroup
---
--- Will return true if a squad was clicked on and false otherwise. Will fail if the identified button does not exist, if the button wasn't pressed, or if the button isn't modal
---
--- *Source: LuaUI.cpp*
---@param buttonID ButtonID
---@param sgroup SGroupID
---@return boolean
function Button_GetPressedSquad(buttonID, sgroup) end

--- Checks whether the identified SCAR button is currently visible
---
--- Will fail if the identified button does not exist
---
--- *Source: LuaUI.cpp*
---@param buttonID ButtonID
---@return boolean
function Button_GetVisible(buttonID) end

--- Removes the identified SCAR button
---
--- Will fail if the identified button does not exist
---
--- *Source: LuaUI.cpp*
---@param buttonID ButtonID
function Button_Remove(buttonID) end

--- Enables or disables the identified SCAR button
---
--- Will fail if the identified button does not exist
---
--- *Source: LuaUI.cpp*
---@param buttonID ButtonID
---@param enable boolean
function Button_SetEnabled(buttonID, enable) end

--- Allows the button owner to express a user interface location preference.
---
--- The user interface may not follow this request - for example the prefered location may be taken.
---
--- *Source: LuaUI.cpp*
---@param buttonID ButtonID
---@param hint integer
function Button_SetIndexHint(buttonID, hint) end

--- Sets the tooltip title and description for the identified button
---
--- Will fail if the identified button does not exist
---
--- *Source: LuaUI.cpp*
---@param buttonID ButtonID
---@param instruction LocString
function Button_SetModalInstruction(buttonID, instruction) end

--- Sets the enabled, disabled and hover textures for the identified SCAR button
---
--- Will fail if the identified button does not exist
---
--- *Source: LuaUI.cpp*
---@param buttonID ButtonID
---@param enabled string
---@param disabled string
---@param hover string
function Button_SetTextures(buttonID, enabled, disabled, hover) end

--- Sets all for the identified SCAR button from the icon of the specified squad blueprint
---
--- Will fail if the identified button does not exist
---
--- *Source: LuaUI.cpp*
---@param buttonID ButtonID
---@param squadblueprint string
function Button_SetTexturesFromSquadBlueprint(buttonID, squadblueprint) end

--- Sets the tooltip title and description for the identified button
---
--- Will fail if the identified button does not exist
---
--- *Source: LuaUI.cpp*
---@param buttonID ButtonID
---@param title LocString
---@param description LocString
function Button_SetTooltip(buttonID, title, description) end

--- Sets the tooltip title and description for the identified button from the squadblueprint
---
--- Will fail if the identified button does not exist
---
--- *Source: LuaUI.cpp*
---@param buttonID ButtonID
---@param squadblueprint string
function Button_SetTooltipFromSquadBlueprint(buttonID, squadblueprint) end

--- Hides or shows the identified SCAR button
---
--- Will fail if the identified button does not exist
---
--- *Source: LuaUI.cpp*
---@param buttonID ButtonID
---@param visible boolean
function Button_SetVisible(buttonID, visible) end

--- Spawn a custom event cue item from the data:art/ui/ingame/event_cue_icons/custom/ folder and the data:sound/ folder
---
--- ie. EventCue_DoEvent( 'icon_image', 'icon_sound', '$20400', 'You can also just use text if you dont care for localization' )
---
--- *Source: LuaUI.cpp*
---@param icon string
---@param sound string
---@param title LocString
---@param desc LocString
function EventCue_DoEvent(icon, sound, title, desc) end

--- Toggles event cue ui and all vocalizations (selection, ordering, game events...)
---
--- *Source: LuaUI.cpp*
---@param enabled boolean
function EventCue_Enable(enabled) end

--- Enable/Disable flashing on construction buttons for builder units.
---
--- Pass in the ebp name for the building button you want to flash
---
--- *Source: LuaUI.cpp*
---@param ebpname string
---@param enable boolean
---@param stopFlashingWhenClicked boolean
function Flash_ConstructionButton(ebpname, enable, stopFlashingWhenClicked) end

--- Flash other binding. See extended description in scardoc.
---
--- bindingname, bindingparams
---  move_modal, not 0 - used
---  attackmove_modal, 0 - not used
---  attackground_modal, 0 - not used
---  jump_modal, 0 - not used
---  rampage_modal, 0 - not used
---  unload_modal, 0 - not used
---  attach_modal, 0 - not used
---  player_resource, 0 - power resource, 1 - requistion resource, 2 - ork resource
---  focus_on_next, 0 - next builder button, 1 - next military button, 2 - next research button
---  dlg_objectives, 0 - not used
---  build_structures_menu, 0 - not used
---  command_infiltrate, 0 - not used
---
--- *Source: LuaUI.cpp*
---@param bindingname string
---@param bindingparam integer
---@param enable boolean
---@param stopFlashingWhenClicked boolean
function Flash_OtherButton(bindingname, bindingparam, enable, stopFlashingWhenClicked) end

--- Enable/Disable flash production buttons itemtype PQ_Research, PQ_Squad, or PQ_AddOn
---
--- PQ_Research - itemname is the name of the research blueprint
---  PQ_AddOn - itemname is the name of the addon blueprint
---  PQ_Squad - itemname is the name of the squad blueprint
---
--- *Source: LuaUI.cpp*
---@param itemtype integer
---@param itemname string
---@param enable boolean
---@param stopFlashingWhenClicked boolean
function Flash_ProductionButton(itemtype, itemname, enable, stopFlashingWhenClicked) end

--- Enable/Disable flashing of reinforcement buttons itemtype RT_Trooper, RT_Leader, RT_WeaponUpgrade
---
--- weaponname is only used when itemtype is RT_WeaponUpgrade
---
--- *Source: LuaUI.cpp*
---@param itemtype integer
---@param squadbpname string
---@param weaponname string
---@param enable boolean
---@param stopFlashingWhenClicked boolean
function Flash_ReinforcementButton(itemtype, squadbpname, weaponname, enable, stopFlashingWhenClicked) end

--- Returns the display text for a hotkey from the hotkeys file
---
--- *Source: LuaUI.cpp*
---@param hotkeyname string
---@return LocString
function Hotkey_GetText(hotkeyname) end

--- Returns true if an objective exists
---
--- *Source: LuaUI.cpp*
---@param title_id integer
---@return boolean
function Objective_Exists(title_id) end

--- Returns OS_Incomplete, OS_Complete or OS_Failed
---
--- *Source: LuaUI.cpp*
---@param title_id integer
---@return integer
function Objective_GetState(title_id) end

--- Removes the identified ping from the identified objective
---
--- *Source: LuaUI.cpp*
---@param title_id integer
---@param ping_id integer
function Objective_PingRemove(title_id, ping_id) end

--- Resets a player's objectives colors to the defaults
---
--- *Source: LuaUI.cpp*
---@param player PlayerID
function Objective_ResetPlayerColor(player) end

--- Sets the default primary objective
---
--- *Source: LuaUI.cpp*
---@param obj_id integer
function Objective_SetDefaultPrimary(obj_id) end

--- Sets the default secondary objective
---
--- *Source: LuaUI.cpp*
---@param obj_id integer
function Objective_SetDefaultSecondary(obj_id) end

--- Sets a player's objectives colors
---
--- *Source: LuaUI.cpp*
---@param player PlayerID
---@param normal Colour
---@param failed Colour
function Objective_SetPlayerColor(player, normal, failed) end

--- Set an objective to OS_Complete or OS_Failed
---
--- You cannot set an objectives state to OS_Incomplete.  When an ojective is completed or failed, it will trigger a ui event icon.
---
--- *Source: LuaUI.cpp*
---@param title_id integer
---@param state integer
function Objective_SetState(title_id, state) end

--- Opens up the objectives menu for the player
---
--- *Source: LuaUI.cpp*
function Objective_ShowScreen() end

--- Opens up the objectives menu for the player
---
--- *Source: LuaUI.cpp*
function Objective_ShowScreenSecondary() end

--- Enable strategic point, relic, or objective ui
---
--- *Source: LuaUI.cpp*
---@param strat_ebp_name string
---@param enable boolean
function Taskbar_EnableStrategicUI(strat_ebp_name, enable) end

--- Returns HTL_None, HTL_Min, or HTL_Max
---
--- *Source: LuaUI.cpp*
---@return integer
function Taskbar_GetHelpTextLevel() end

--- Returns true if the HQ has been focused on using the Home key in the last amount of time
---
--- *Source: LuaUI.cpp*
---@param secs number
---@return boolean
function Taskbar_HasFocusedOnHQ(secs) end

--- Clear the time and hide ui
---
--- *Source: LuaUI.cpp*
---@param strat_ebp_name string
function Taskbar_HideStrategicUITimer(strat_ebp_name) end

--- Returns true if the taskbar is minimized and false if it is in its normal state
---
--- *Source: LuaUI.cpp*
---@return boolean
function Taskbar_IsMinimized() end

--- Set tooltip level to HTL_None, HTL_Min, or HTL_Max
---
--- *Source: LuaUI.cpp*
---@param level integer
function Taskbar_SetHelpTextLevel(level) end

--- Set the timer beside a strategic UI
---
--- *Source: LuaUI.cpp*
---@param strat_ebp_name string
---@param player PlayerID
---@param secsRemaining integer
function Taskbar_SetStrategicUITimer(strat_ebp_name, player, secsRemaining) end

--- Activate scar submenu on the taskbar
---
--- *Source: LuaUI.cpp*
---@param show boolean
function Taskbar_ShowMenu(show) end

--- Displays a breif UI warning in the critical alert message area.
---
--- PLEASE be very careful how you use this as it can cause localization issues.  For example, if you want to display the message 'PlayerName1 is going to win!', you CANNOT use concatanation in lua to create the string, you should use Loc_FormatText instead.  Ask for help if you are unsure.
---
--- *Source: LuaUI.cpp*
---@param locText LocString
function UIWarning_Show(locText) end

--- Create win/lose warning.  You can use the name to reference the warning later.
---
--- String name - The name of the win warning.  Each win warning needs a unique name so you have a way of changing its text at a later time.
---  PlayerID player - The player that this win warning applies to.  This isnt used right now, but its good to have in case we want to make the text the same color as the player or something.
---  String icon - The name of the icon.  Again, this is not used right now, but it may be in the future.
---  LocString wWinConditionName - This is a localized string containing the name of the win condition that this win warning applies to. This text is used for the title of the win warnings help tip.
---  LocString wHelpTip - This is a localized string containing the help tip description.
---
--- *Source: LuaUI.cpp*
---@param name string
---@param player PlayerID
---@param icon string
---@param wWinConditionName LocString
---@param wHelpTip LocString
function WinWarning_Add(name, player, icon, wWinConditionName, wHelpTip) end

--- Returns true if the named win warning exists.
---
--- *Source: LuaUI.cpp*
---@param name string
---@return boolean
function WinWarning_Exists(name) end

--- Removes an added win/lose warning
---
--- *Source: LuaUI.cpp*
---@param name string
function WinWarning_Remove(name) end

--- Set the text for a win warning.
---
--- Designers: PLEASE be careful how you use this.  Use Loc_FormatText where nesc. for localization.
---
--- *Source: LuaUI.cpp*
---@param name string
---@param locText LocString
function WinWarning_SetText(name, locText) end

-- from LuaW40k.cpp

--- Returns true if the camera has recieved commands in the last number of seconds.
---
--- *Source: LuaW40k.cpp*
---@param secs number
---@return boolean
function Camera_HasPanned(secs) end

--- Returns true if the camera has recieved commands in the last number of seconds.
---
--- *Source: LuaW40k.cpp*
---@param secs number
---@return boolean
function Camera_HasReset(secs) end

--- Returns true if the camera has recieved commands in the last number of seconds.
---
--- *Source: LuaW40k.cpp*
---@param secs number
---@return boolean
function Camera_HasRotated(secs) end

--- Returns true if the camera has recieved commands in the last number of seconds.
---
--- *Source: LuaW40k.cpp*
---@param secs number
---@return boolean
function Camera_HasZoomed(secs) end

--- Returns true if the app has had any keyboard input in the last second
---
--- *Source: LuaW40k.cpp*
---@return boolean
function Misc_DetectKeyboardInput() end

--- Returns true if the app has had any mouse input in the last second
---
--- *Source: LuaW40k.cpp*
---@return boolean
function Misc_DetectMouseInput() end

--- Returns the world position of the mouse on the terrain
---
--- *Source: LuaW40k.cpp*
---@return Position
function Misc_GetMouseOnTerrain() end

--- Returns the entity under the mouse (if any)
---
--- *Source: LuaW40k.cpp*
---@return EntityID
function Misc_GetMouseOverEntity() end

--- Clears a given egroup and adds the current selection to the group
---
--- *Source: LuaW40k.cpp*
---@param group EGroupID
function Misc_GetSelectedEntities(group) end

--- Clears a given egroup and adds the current selection to the group
---
--- *Source: LuaW40k.cpp*
---@param group SGroupID
function Misc_GetSelectedSquads(group) end

--- Returns true if an entity is currently selected
---
--- *Source: LuaW40k.cpp*
---@param entity EntityID
---@return boolean
function Misc_IsEntitySelected(entity) end

--- Returns true if the mouse is over an entity
---
--- *Source: LuaW40k.cpp*
---@return boolean
function Misc_IsMouseOverEntity() end

--- Returns true if a squad is currently selected
---
--- *Source: LuaW40k.cpp*
---@param squad SquadID
---@return boolean
function Misc_IsSquadSelected(squad) end

--- Assign a group number [0,9] to the first entity of an egroup.
---
--- *Source: LuaW40k.cpp*
---@param egroup EGroupID
---@param nGroupNumber integer
function W40k_AssignHotkeyEGroup(egroup, nGroupNumber) end

--- Assign a group number [0,9] to a squad group
---
--- *Source: LuaW40k.cpp*
---@param sgroup SGroupID
---@param nGroupNumber integer
function W40k_AssignHotkeySGroup(sgroup, nGroupNumber) end

--- Autosaves the current mission to the player:campaign folder.
---
--- Must provide a UI name
---
--- *Source: LuaW40k.cpp*
---@param savename LocString
function W40k_Autosave(savename) end

--- Precache a range of speech files.
---
--- This is useful for preloading sounds at the begining of the mission so they arent loaded during the NISlets.
---
--- *Source: LuaW40k.cpp*
---@param rangeBegin integer
---@param rangeEnd integer
function W40k_CacheSpeech(rangeBegin, rangeEnd) end

--- Sets the next mission for the current campaign
---
--- Can only be used with campaign games. Will fail if and empty mission is specified or the specified mission does not exist in the current campaign.
---
--- *Source: LuaW40k.cpp*
---@param missionFilename string
function W40k_CampaignNextMission(missionFilename) end

--- Enables/Disables all input EXCEPT for ESC and F10.
---
--- *Source: LuaW40k.cpp*
---@param enabled boolean
function W40k_EnableInput(enabled) end

--- Enableds/disables all selection ui
---
--- *Source: LuaW40k.cpp*
---@param enable boolean
function W40k_EnableSelectionUI(enable) end

--- Removes a message added by W40k_ShowSystemMessage.
---
--- *Source: LuaW40k.cpp*
---@param msg LocString
function W40k_HideSystemMessage(msg) end

--- Fade a background image in then out.  You cannot WAIT on this function.
---
--- texture filename is relative to the data:art/ui/ingame/image_fades/ folder
---
--- *Source: LuaW40k.cpp*
---@param texture string
---@param in number
---@param lifetime number
---@param out number
function W40k_ImageFade(texture, in, lifetime, out) end

--- Returns true if the ui is in letterbox mode
---
--- *Source: LuaW40k.cpp*
---@return boolean
function W40k_IsLetterboxed() end

--- Enters/Exits letterbox mode in amount of time specified by timeSecs.
---
--- This function can be called through a CTRL object in NISlets.
---
--- *Source: LuaW40k.cpp*
---@param on boolean
---@param timeSecs number
function W40k_Letterbox(on, timeSecs) end

--- Marks the rectangular area between the two markers as a no build zone.
---
--- 'markerFrom' is the lower left corner of the rectangular zone, 'markerTo' is the upper right (i.e. the from marker needs to be to the left of and below the to marker). The zone will also not be created if parts (or all) of the area over which it is being created already is no build (we don't track who gave no build, therefore we wouldn't know if it's ok to get rid of it come removal time).
---
--- *Source: LuaW40k.cpp*
---@param markerFrom string
---@param markerTo string
---@return integer
function W40k_NoBuildZoneMarker(markerFrom, markerTo) end

--- Marks the rectangular area between the two points as a no build zone.
---
--- 'positionFrom' is the lower left corner of the rectangular zone, 'positionTo' is the upper right (i.e. the from position needs to be to the left of and below the to position). The zone will also not be created if parts (or all) of the area over which it is being created already is no build (we don't track who gave no build, therefore we wouldn't know if it's ok to get rid of it come removal time).
---
--- *Source: LuaW40k.cpp*
---@param positionFrom Position
---@param positionTo Position
---@return integer
function W40k_NoBuildZonePosition(positionFrom, positionTo) end

--- Removes a no build zone.
---
--- *Source: LuaW40k.cpp*
---@param zoneID integer
function W40k_NoBuildZoneRemove(zoneID) end

--- Starts a movie.  Use the shortname for moviename (ie mymovie) with no extension.
---
--- This function call be called through a CTRL object and waited on
---
--- *Source: LuaW40k.cpp*
---@param moviename string
function W40k_PlayMovie(moviename) end

--- Select the first spawned entity in an egroup. Clears current selection.
---
--- *Source: LuaW40k.cpp*
---@param group EGroupID
function W40k_SelectEGroup(group) end

--- Select all spawned squads in an sgroup.  Clears current selection.
---
--- *Source: LuaW40k.cpp*
---@param group SGroupID
function W40k_SelectSGroup(group) end

--- Cycle focus through current selection.
---
--- This is the same as using the aposophre key to focus on an entity.  It is known that this feature does not work well on vehicles.  I would recommend not using it for such.
---
--- *Source: LuaW40k.cpp*
function W40k_SelectionFocus() end

--- Shows a system message in the area where Game Paused text appears/
---
--- Call W40k_HideSystemMessage to remove the message.
---
--- *Source: LuaW40k.cpp*
---@param msg LocString
function W40k_ShowSystemMessage(msg) end

--- - Switch to the specified player
---
--- Set enablePrevAI to enable the AI for the abandoned player, or false to leave it off.
---
--- *Source: LuaW40k.cpp*
---@param pPlayer PlayerID
---@param enablePrevAI boolean
function W40k_SwitchToPlayer(pPlayer, enablePrevAI) end

--- Unlocks user/camera input during letterbox. This should be used for debug only
---
--- *Source: LuaW40k.cpp*
function W40k_UnlockInputOnLetterBox() end

--- Clears all persistent bodies from the world
---
--- *Source: LuaW40k.cpp*
function World_ClearPersistentBodies() end

--- Triggers an event file at the given location.
---
--- The event file must be prepended with "data:Art/Events" and the directory it's in (if not in the Events directory). The file extension can be omitted.
---   Example: World_FXEvent("data:Art/Events/Unit_Upgrade_Morale_FX/Reinforce_Trooper", World_Pos(0,0,0))
---
--- *Source: LuaW40k.cpp*
---@param filename string
---@param pos Position
function World_FXEvent(filename, pos) end

--- Destroy/Stop an event that was started with World_FXEvent
---
--- Pass in the result of World_FXEvent
---
--- *Source: LuaW40k.cpp*
---@param event integer
function World_FXEventDestroy(event) end

--- Triggers an event file at the given location (relative to Art/Events without extension folder)
---
--- Example:
---
--- World_FXEvent("movedest" World_Pos(0,0,0))
---       Plays the MoveFX event at location(0,0,0)
---
--- *Source: LuaW40k.cpp*
---@param filename string
---@param pos Position
---@return integer
function World_FXEventLooping(filename, pos) end

--- Triggers an event file at the location of every entity belonging to every squad in the group
---
--- The event file must be prepended with "data:Art/Events" and the directory it's in (if not in the Events directory). The file extension can be omitted.
---   Example: World_FXEventSquad("my_group", "data:Art/Events/Unit_Upgrade_Morale_FX/Reinforce_Trooper")
---
--- *Source: LuaW40k.cpp*
---@param filename string
---@param sgroup SGroupID
function World_FXEventSquad(filename, sgroup) end

-- from LuaWorld.cpp

--- Clears all the usable bodies from the world.
---
--- *Source: LuaWorld.cpp*
function World_ClearUsableBodies() end

---
--- *Source: LuaWorld.cpp*
---@param r integer
---@param g integer
---@param b integer
---@return Colour
function World_Color(r, g, b) end

--- Get the distance between a squad group and a point.
---
--- Pass in true for 3rd parameter to get the closest point and pass in false to get the furthest point.  The closest/furthest point is calculated on a per entity basis.  So the closest distance would be that of the closest entity.
---  Also note, it is an error to call this function with an empty group.
---
--- *Source: LuaWorld.cpp*
---@param egroup EGroupID
---@param p1 Position
---@param closest boolean
---@return number
function World_DistanceEGroupToPoint(egroup, p1, closest) end

--- Get the distance between two points.
---
--- *Source: LuaWorld.cpp*
---@param p1 Position
---@param p2 Position
---@return number
function World_DistancePointToPoint(p1, p2) end

--- Get the distance between a squad group and a point.
---
--- Pass in true for 3rd parameter to get the closest point and pass in false to get the furthest point.  The closest/furthest point is calculated on a per squadron basis.  So the closest distance would be that of the closest squadron.
---  It is an error to call this function with an empty group.  Check the group size with SquadGroup_Count( ) first.
---
--- *Source: LuaWorld.cpp*
---@param sgroup SGroupID
---@param p1 Position
---@param closest boolean
---@return number
function World_DistanceSGroupToPoint(sgroup, p1, closest) end

--- Determines if the specified race uses requisition.
---
--- Given a race index, returns true if the race uses requisition, and false otherwise (e.g., Necron use power).
---
--- *Source: LuaWorld.cpp*
---@param race_index integer
---@return boolean
function World_DoesRaceUseRequisition(race_index) end

--- Enables or disables the fog of war between these two players
---
--- *Source: LuaWorld.cpp*
---@param p0 PlayerID
---@param p1 PlayerID
---@param enable boolean
function World_EnablePlayerToPlayerFOW(p0, p1, enable) end

--- Return the total game time in seconds.
---
--- *Source: LuaWorld.cpp*
---@return number
function World_GetGameTime() end

--- Returns the number of strategic objectives on this map
---
--- *Source: LuaWorld.cpp*
---@return integer
function World_GetNumStrategicObjectives() end

--- Returns the number of strategic points with blueprint strategic_point_flag on this map
---
--- *Source: LuaWorld.cpp*
---@return integer
function World_GetNumStrategicPoints() end

--- - Returns the player at a given zero based index.
---
--- *Source: LuaWorld.cpp*
---@param index integer
---@return PlayerID
function World_GetPlayerAt(index) end

--- Return the total number of players in the world
---
--- *Source: LuaWorld.cpp*
---@return integer
function World_GetPlayerCount() end

--- - Returns the player index given the Player*
---
--- *Source: LuaWorld.cpp*
---@param player PlayerID
---@return integer
function World_GetPlayerIndex(player) end

--- Returns the number of types of buildings a race can build
---
--- *Source: LuaWorld.cpp*
---@param race_index integer
---@return integer
function World_GetPossibleBuildingsCount(race_index) end

--- Returns the name of a chosen building for a race
---
--- *Source: LuaWorld.cpp*
---@param race_index integer
---@param building_index integer
---@return string
function World_GetPossibleBuildingsName(race_index, building_index) end

--- Returns the number of types of squads a race can build
---
--- *Source: LuaWorld.cpp*
---@param race_index integer
---@return integer
function World_GetPossibleSquadsCount(race_index) end

--- Returns the blueprint name of a chosen squad for a race
---
--- *Source: LuaWorld.cpp*
---@param race_index integer
---@param squad_index integer
---@return string
function World_GetPossibleSquadsName(race_index, squad_index) end

--- Returns the race index of a race, given its name (from the ME).
---
--- Valid race names for W40K are "space_marine_race", "chaos_marine_race", "ork_race", "guard_race", or "eldar_race".
---
--- *Source: LuaWorld.cpp*
---@param racename string
---@return integer
function World_GetRaceIndex(racename) end

--- Returns a random integer with range [min, max]
---
--- It is recomended you use this instead of luas math.random function
---
--- *Source: LuaWorld.cpp*
---@param min integer
---@param max integer
---@return integer
function World_GetRand(min, max) end

--- - Given any position in the world, this function will return position safe for spawning a given entity
---
--- *Source: LuaWorld.cpp*
---@param around Position
---@param entity EntityID
---@return Position
function World_GetSpawnablePosition(around, entity) end

--- Adds all of the world-owned strategic points to the given egroup
---
--- *Source: LuaWorld.cpp*
---@param egroup EGroupID
function World_GetStrategicPoints(egroup) end

--- - to document
---
--- *Source: LuaWorld.cpp*
---@return boolean
function World_IsGameOver() end

--- Resurrects all resurrectable units that are inside the given area
---
--- *Source: LuaWorld.cpp*
---@param position Position
---@param radius number
function World_MassResurrection(position, radius) end

--- Returns true if the squad is owned by the world
---
--- *Source: LuaWorld.cpp*
---@param entity EntityID
---@return boolean
function World_OwnsEntity(entity) end

--- Returns true if the squad is owned by the world
---
--- *Source: LuaWorld.cpp*
---@param squad SquadID
---@return boolean
function World_OwnsSquad(squad) end

--- Returns true if two world positions are in proximity to each other
---
--- *Source: LuaWorld.cpp*
---@param p1 Position
---@param p2 Position
---@param prox number
---@return boolean
function World_PointPointProx(p1, p2, prox) end

--- Creates a new Position object.
---
--- A position object is basically a table with an x, y, and z attribute.  You can directly access the individual components in the same way that you would access a field in a table.
---
---   Example:
---    --* lua script snip
---    local pos = World_Pos(2, 0, 0)
---    pos.x = pos.x + 1
---    print pos.x -- this will print 3
---    --* lua script snip
---
--- *Source: LuaWorld.cpp*
---@param x number
---@param y number
---@param z number
---@return Position
function World_Pos(x, y, z) end

--- Moves all squads around the passed in marker outside the area defined by the marker's proximity.
---
--- Any squad inside the area will attempt to move to the nearest location outside the area. If that's not possible, the squad will move to a random location (but still clear the area).
---
--- *Source: LuaWorld.cpp*
---@param markerName string
function World_ScatterSquadsMarker(markerName) end

--- Moves all squads (exception those in the 'except' sgroup) around the passed in marker outside the area defined by the marker's proximity.
---
--- Any squad inside the area will attempt to move to the nearest location outside the area. If that's not possible, the squad will move to a random location (but still clear the area).
---
--- *Source: LuaWorld.cpp*
---@param markerName string
---@param except SGroupID
function World_ScatterSquadsMarkerExcept(markerName, except) end

--- Moves all squads inside the area defined by the passed in position and radius outside that area.
---
--- Any squad inside the area will attempt to move to the nearest location outside the area. If that's not possible, the squad will move to a random location (but still clear the area).
---
--- *Source: LuaWorld.cpp*
---@param position Position
---@param radius number
function World_ScatterSquadsPosition(position, radius) end

--- Moves all squads (exception those in the 'except' sgroup) inside the area defined by the passed in position and radius outside that area.
---
--- Any squad inside the area will attempt to move to the nearest location outside the area. If that's not possible, the squad will move to a random location (but still clear the area).
---
--- *Source: LuaWorld.cpp*
---@param position Position
---@param radius number
---@param except SGroupID
function World_ScatterSquadsPositionExcept(position, radius, except) end

--- - Puts the entities into dead state if they are a reanimatable using the anim appropriate for death before the mission start.
---
--- *Source: LuaWorld.cpp*
---@param squad_group SGroupID
function World_SetDeadMissionStart(squad_group) end

--- - to document
---
--- *Source: LuaWorld.cpp*
function World_SetGameOver() end

--- Trigger a lose from a wincondition. Use empty string for SP.  Sets the player and all the members allied to player to a win state, with a reason for winning.    Sets the player to a lose state, with a reason for losing, and kills the player
---
--- *Source: LuaWorld.cpp*
---@param player PlayerID
---@param winconditionName string
function World_SetPlayerLose(player, winconditionName) end

--- - Trigger a win from a wincondition. Use empty string for SP.  Sets all other players to a lose state but does not kill all of their units.  Call World_SetPlayerLost to kill all units.
---
--- *Source: LuaWorld.cpp*
---@param player PlayerID
---@param winconditionName string
function World_SetTeamWin(player, winconditionName) end

-- from Scar.cpp

--- Pauses for a given amount of time. This function MUST be called from a CTRL object in NISlet events only!
---
--- See Event_Start for more information on NISlet events.
---
--- *Source: Scar.cpp*
---@param seconds number
function Event_Delay(seconds) end

--- Returns true if any event is currently running
---
--- *Source: Scar.cpp*
---@return boolean
function Event_IsAnyRunning() end

--- Returns true if a given event is running
---
--- *Source: Scar.cpp*
---@param f function
---@return boolean
function Event_IsRunning(f) end

--- Completes execution of the event immediatley (all calls to Wait() are ignored)
---
--- *Source: Scar.cpp*
function Event_Skip() end

--- Starts event.  Event will not start until all rules are evaluated for this frame!
---
--- The only time you should need this function is to start playing a NISLet.  If your NISLet is named NISLet01 you would call this function like so:
---
---      Event_Start( NISLets.NISLet01, 0 )
---
---  Events are run until a call to Wait() is hit.  When the event reaches Wait() it will not continue it execution until all controllers it is waiting for are done.
---  To add a controller to the block simply call the controller through the CTRL object.  For example, to call the Delay controller you should call CTRL.Delay(5). This will make the succesive call to Wait delay for 5 seconds.
---
---  Example: (NISLets are located in the .nis file for your mission)
---  NISLets = {
---      MyNISLet = function ( )
---
---          print("Yo")
---          CTRL.Event_Delay(3)
---          print("This will show up right after Yo, it will not wait for 3 seconds")
---          CTRL.Event_Delay(2)
---          Event_Wait() -- execution will now delay for 5 seconds (waiting for controllers)
---
---          print("Yo, 5 seconds later")
---
---      end
---
---  To trigger the above NISLet from a Rule or your mission script you would call Event_Start( NISLets.MyNISLet, 0 )
---
---  If you start an event that has higher priority then the current event playing, the current event will be cancelled.
---
---  If you start an event that has lower or equal priority then the current event playing, the event will be queued and it will play when the current event is complete.
---
---  Zero is the highest priority and Zero+N is lower priority.
---
--- *Source: Scar.cpp*
---@param f function
---@param priority integer
function Event_Start(f, priority) end

--- Returns the time, in seconds, since the last NIS or Intel Event finished playing
---
--- *Source: Scar.cpp*
---@return number
function Event_TimeSinceLast() end

--- Register an init function with the scar system.
---
--- This init function will be called when scar is started up.
---  **Important: Make sure you do not register two functions with the same name; the init function names should be unique.
---
--- *Source: Scar.cpp*
---@param f function
function Scar_AddInit(f) end

--- Register a load function with the scar system.
---
--- This load function will be called when scar is started up while loading a savegame.
---  **Important: Make sure you do not register two functions with the same name; the load function names should be unique.
---
--- *Source: Scar.cpp*
---@param function function
function Scar_AddLoad(function) end

--- Returns true if an init function exists
---
--- *Source: Scar.cpp*
---@param f function
---@return boolean
function Scar_InitExists(f) end

--- Returns true if a load function exists
---
--- *Source: Scar.cpp*
---@param function function
---@return boolean
function Scar_LoadExists(function) end

--- Unregister an init function that was registered from Scar_AddInit
---
--- *Source: Scar.cpp*
---@param f function
function Scar_RemoveInit(f) end

--- Unregister a load function that was registered from Scar_AddLoad
---
--- *Source: Scar.cpp*
---@param function function
function Scar_RemoveLoad(function) end

-- from StatLogging.cpp

--- Number of commander units the player with the given ID has killed
---
--- *Source: StatLogging.cpp*
---@param playerID integer
---@return integer
function Stats_PlayerCommanderUnitsKilled(playerID) end

--- Number of structures the player with the given ID has lost
---
--- *Source: StatLogging.cpp*
---@param playerID integer
---@return integer
function Stats_PlayerStructuresLost(playerID) end

--- Number of structures the player with the given ID has razed
---
--- *Source: StatLogging.cpp*
---@param playerID integer
---@return integer
function Stats_PlayerStructuresRazed(playerID) end

--- Number of units the player with the given ID has killed
---
--- *Source: StatLogging.cpp*
---@param playerID integer
---@return integer
function Stats_PlayerUnitsKilled(playerID) end

--- Number of units the player with the given ID has lost
---
--- *Source: StatLogging.cpp*
---@param playerID integer
---@return integer
function Stats_PlayerUnitsLost(playerID) end

-- from luaBlueprintManager.cpp

--- Dump animator statistics to a file
---
--- *Source: luaBlueprintManager.cpp*
function blueprint_dump() end

-- from scardoc.dat

--- Get the current target position for the camera. 
---
--- *Source: scardoc.dat*
---@return Position
function Camera_GetTargetPos() end

--- Get values: TV_SlideTargetRate, TV_SlideTargetBase,
---  TV_SlideTargetThreshold 
---
--- *Source: scardoc.dat*
---@param tuningValue integer
---@return number
function Camera_GetTuningValue(tuningValue) end

--- Get the current zoom distance for the camera. 
---
--- *Source: scardoc.dat*
---@return number
function Camera_GetZoomDist() end

--- Reload camere tuning values from Data:Camera.lua. 
---
--- *Source: scardoc.dat*
function Camera_Reload() end

--- Set values: TV_SlideTargetRate, TV_SlideTargetBase,
---  TV_SlideTargetThreshold 
---
--- *Source: scardoc.dat*
---@param tuningValue integer
---@param value number
function Camera_SetTuningValue(tuningValue, value) end

--- Set the current zoom distance for the camera. 
---
--- *Source: scardoc.dat*
---@param distance number
function Camera_SetZoomDist(distance) end

--- Updates the peristent EGroup that contains all entities for this
---  player 
---
--- *Source: scardoc.dat*
---@param player PlayerID
---@return EGroupID
function Player_UpdateAllEntities(player) end

--- Updates the peristent SGroup that contains all squads for this
---  player 
---
--- *Source: scardoc.dat*
---@param player PlayerID
---@return SGroupID
function Player_UpdateAllSquads(player) end

--- Quits the app immediately 
---
--- *Source: scardoc.dat*
function W40k_QuitApp() end

--- Loads data:scar/fileName and executes it
---
--- *Source: scardoc.dat*
---@param fileName string
function import(fileName) end

--- Receives any number of arguments and prints their values as strings
---
--- *Source: scardoc.dat*
---@param s1 any
---@param s2 any
---@param s3 any
---@param ... any
function print(s1, s2, s3, ...) end

