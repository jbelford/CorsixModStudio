---@meta scar-dow

-- Auto-generated from scardoc.dat by scardoc-to-luadefs.py
-- 652 functions, 139 constants
-- Do not edit manually — regenerate with: python tools/scardoc-to-luadefs.py

-- Constants
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

---Breaks the relationship between an actor an its sgroup.  
---Speech played by this actor after this function call will no
--- longer look for the actor in the sgroup to play the talk
--- animation.  This function also removes the special sp_idle that
--- is applied to all actors.
---@param actor ActorTable
function Actor_Clear(actor) end

---Plays a speech event for a given actor. 
--- **Sounds will be taken from the data:sound/speech/nis/ folder. 
--- If you wish to play a sound from another folder you must use an
--- optional path (ie. 'speech/single_player/guardsman/morale')
---
--- If the speech file is not available, a placeholder time will be
--- calculated for the message using the number of words and the
--- message will be displayed for that length of time.
--- Actors:
---     ACT.Gabriel, ACT.Isador, ACT.Matiel, ACT.Bale, ACT.Sindri,
--- ACT.Orkamungus, ACT.Macha, ACT.Brom, ACT.Toth, ACT.ScoutMarine,
--- ACT.TacticalMarine, ACT.SluggaBoy, ACT.Guardsmen, ACT.Ranger,
--- ACT.Apothecary, ACT.Cultist, ACT.Nob, ACT.Grot, ACT.Corralis,
--- ACT.Servitor, ACT.Warlock, ACT.Dark_Reaper_Exarch,
--- ACT.EvilIsador
---
--- To play speech for a custom actor, pass in a table = {
--- blueprint = "blueprint_name", icon = "data:icon_path.tga" } for
--- the Actor.
---@param actor ActorTable
---@param locID integer
---@param optionalPath string
function Actor_PlaySpeech(actor, locID, optionalPath) end

---Assign an sgroup to a actor.  Will take the first matching
--- blueprint from the sgroup. 
---  Actor_SetFromSGroup( ACT.Gabriel, "sg_Gabriel" )
---@param actor ActorTable
---@param sgroupname string
function Actor_SetFromSGroup(actor, sgroupname) end

---Enables / disables the random infantry offset for all spawned
--- and depsawned units in an sgroup. 
---  You can use this function for the close up shots of your nis'
--- to ensure that the units are in    the same position that you
--- place them in the ME.  When you first disable the infantry
--- offset, the units will   snap to their original positions so it
--- might be a good idea to hide this behaviour from the user. 
--- When re-enabling    this offset, the units will stay in their
--- current positions until they move (they will not snap, so it
--- should be ok    to call this while the unit is in view)
---
---  NOTE: If you disable this: MAKE SURE YOU RE-ENABLE IT,
--- otherwise the units will not use random offsets ingame.
---@param sgroup SGroupID
---@param enable boolean
function Anim_EnableInfantryOffset(sgroup, enable) end

---Plays an animation from an entity. 
---This function can be called through a CTRL object in NISlets to
--- wait on the animation.
---@param egroup EGroupID
---@param animname string
function Anim_PlayEGroupAnim(egroup, animname) end

---Trigger an anim for all troopers in a squad that are capable of
--- playing the anim 
---@param sgroup SGroupID
---@param animname string
function Anim_PlaySGroupAnim(sgroup, animname) end

---Find the first entity in the attacking squad group and attack
--- the first entity in the victim squad group capable of playing
--- the anim. 
---Example: Anim_PlaySyncAnim( 'sg_force_commander',
--- 'sg_blood_thirster', 'SyncKill_7', 'SyncDeath_7'
--- Also, you will want to make sure the two squads are close to
--- each other since the sync kill animation is going to 'pop' the
--- units into place.
---@param attacker SGroupID
---@param victim SGroupID
---@param killanim string
---@param deathanim string
---@param distance number
function Anim_PlaySyncAnim(attacker, victim, killanim, deathanim, distance) end

---Force an animation that was started with Anim_PlayEntityAnim to
--- stop if it is still running	 
---@param egroup EGroupID
---@param animname string
function Anim_StopEGroupAnim(egroup, animname) end

---Force all animations that were started with Anim_PlaySquadAnim
--- to stop if they are still running	 
---@param sgroup SGroupID
---@param animname string
function Anim_StopSGroupAnim(sgroup, animname) end

---Creates the maximum number of squads of this loadout given the
--- resources at a marker position and adds them to a squadgroup. 
--- The squadgroup will be created if it doesnt exist. 
---Loadout will be clipped to unitMin and unitMax from the squad
--- blueprint.
---@param playerId PlayerID
---@param squadgroupName string
---@param squadBlueprint string
---@param load_out_table table
---@param markername string
---@param requisition_amount number
---@param power_amount number
---@return SGroupID
function Balance_CreateSquadsAtMarkerByResource(playerId, squadgroupName, squadBlueprint, load_out_table, markername, requisition_amount, power_amount) end

---Set the Lua Function used to initialize each Balance Tool
--- iteration.  
---The function is used to set up the forces that will battle
---@param func function
function Balance_SetInit(func) end

---Blends out of the current camera path using time from the ME. 
---In order to call this, you must have called CPath_Start()
---
--- This function can be called through a CTRL and waited on.
function CPath_Blendout() end

---Cuts to a new section of a started path, OR to a section of a
--- different path. 
---The blendin parameter is only valid when cutting to a different
--- camera path.   When blendin is true, the camera will blend-in
--- from its current position to the keyframe at fromkeyframe using
--- the blend-in time from the ME.
---
--- This function can be called through a CTRL and waited on.
---@param cpath string
---@param fromkeyframe string
---@param tokeyframe string
---@param blendin boolean
function CPath_CutToPath(cpath, fromkeyframe, tokeyframe, blendin) end

---Instantly cut to a point on a path. 
---Since this function is instant, it makes no sense to call it
--- through a CTRL
---@param cpath string
---@param keyframe string
function CPath_CutToPoint(cpath, keyframe) end

---Returns true if a camera is playing OR waiting to be blended out
--- 
---@return boolean
function CPath_IsPlaying() end

---Starts a camera path using the blend-in time from the ME. 
---Only one camera path can be started at a time.  Once a camera
--- path is started, it is ok to cut to a different path or to play
--- sections from the same path.
---
--- Once the frame specified by tokeyframe has been reached, the
--- camera will pause and hold on that position until
--- CPath_Blendout is called or until you cut to another section.
---
--- The keyframe strings represent the names of the keyframes as
--- specified in the ME.
---
--- This function can be called through a CTRL and waited on.
---@param cpath string
---@param fromkeyframe string
---@param tokeyframe string
function CPath_Start(cpath, fromkeyframe, tokeyframe) end

---Trigger a crazy big camera shake 
---@param epicentre Position
---@param duration number
function CameraShake_Large(epicentre, duration) end

---Trigger a regular camera shake 
---@param epicentre Position
---@param duration number
function CameraShake_Medium(epicentre, duration) end

---Trigger a small camera shake 
---@param epicentre Position
---@param duration number
function CameraShake_Small(epicentre, duration) end

---Resets the camera to default view 
function Camera_DefaultView() end

---Print current camera settings. 
function Camera_Dump() end

---Focus the camera on a target enitity group. 
---This function MUST be called through a CTRL object in NISlets if
--- secs is greater then zero.
---
---@param egroupID EGroupID
---@param secs number
function Camera_FocusOnTargetEGroup(egroupID, secs) end

---Focus the camera on a named marker. 
---This function can be called through a CTRL object in NISlets.
---@param markername string
---@param secs number
function Camera_FocusOnTargetMarker(markername, secs) end

---Focus the camera on a target position. 
---This function MUST be called through a CTRL object in NISlets if
--- secs is greater then zero.
--- IMPORTANT: This function does not lock user input.  If this
--- function is called through a CTRL Camera_SetInputEnabled(false)
--- should almost certainly be called!
---@param targetpos Position
---@param secs number
function Camera_FocusOnTargetPos(targetpos, secs) end

---Focus the camera on a target squad group. 
---This function MUST be called through a CTRL object in NISlets if
--- secs is greater then zero.
---
---@param sgroupID SGroupID
---@param secs number
function Camera_FocusOnTargetSGroup(sgroupID, secs) end

---Returns the enabled/disabled state of the camera input. 
---@return boolean
function Camera_GetInputEnabled() end

---Get the current target position for the camera. 
---@return Position
function Camera_GetTargetPos() end

---Get values: TV_SlideTargetRate, TV_SlideTargetBase,
--- TV_SlideTargetThreshold 
---@param tuningValue integer
---@return number
function Camera_GetTuningValue(tuningValue) end

---Get the current zoom distance for the camera. 
---@return number
function Camera_GetZoomDist() end

---Returns true if the camera has recieved commands in the last
--- number of seconds. 
---@param secs number
---@return boolean
function Camera_HasPanned(secs) end

---Returns true if the camera has recieved commands in the last
--- number of seconds. 
---@param secs number
---@return boolean
function Camera_HasReset(secs) end

---Returns true if the camera has recieved commands in the last
--- number of seconds. 
---@param secs number
---@return boolean
function Camera_HasRotated(secs) end

---Returns true if the camera has recieved commands in the last
--- number of seconds. 
---@param secs number
---@return boolean
function Camera_HasZoomed(secs) end

---Returns true if an entity is on screen 
---IMPORTANT: This function does not check the FOW.  It will return
--- true  even if the item is not revealed in FOW
---@param entity EntityID
---@return boolean
function Camera_IsEntityOnScreen(entity) end

---Returns true if the camera is focused on and set to follow an
--- entity (apostrophe command) 
---@return boolean
function Camera_IsInFollowMode() end

---Returns true if any entities in the squad are on screen 
---IMPORTANT: This function does not check the FOW.  It will return
--- true  even if the item is not revealed in FOW
---@param squad SquadID
---@return boolean
function Camera_IsSquadOnScreen(squad) end

---Reload camere tuning values from Data:Camera.lua. 
function Camera_Reload() end

---Enables / Disables camera input. 
---Disable the camera input to block all camera commands from the
--- game.
---@param enabled boolean
function Camera_SetInputEnabled(enabled) end

---Set values: TV_SlideTargetRate, TV_SlideTargetBase,
--- TV_SlideTargetThreshold 
---@param tuningValue integer
---@param value number
function Camera_SetTuningValue(tuningValue, value) end

---Set the current zoom distance for the camera. 
---@param distance number
function Camera_SetZoomDist(distance) end

---Shake that hot camera! 
---epicentre - origin of camera shake.  as you move away from this
--- origin, the shake is less intense.
--- timeSecs - duration of the shake
--- intesity - intesity of the shake.
--- radius - the extents of the shake from the epicentre
--- falloff - the amount of shake that the camera loses as it gets
--- further from the epicentre
---
--- NOTE: You should not call this function directly from your
--- mission scripts.  You should use the functions in
--- CameraShake.scar
---@param epicentre Position
---@param timeSecs number
---@param intensity number
---@param radius number
---@param falloff number
function Camera_Shake(epicentre, timeSecs, intensity, radius, falloff) end

---Attach the squad from sgroupnameAttachee to sgroupname.  Both
--- SGroups must contain only one squad. 
---@param sgroupname string
---@param sgroupnameAttachee string
function Cmd_AttachSquads(sgroupname, sgroupnameAttachee) end

---Order a squad group to attack another squad group.  This
--- function should track the target squad groups position.  
---The source squads will not stop moving until they reach the
--- targeted squad group.
---@param sourcesquadgroup string
---@param targetentitygroup string
function Cmd_AttackEGroup(sourcesquadgroup, targetentitygroup) end

---Order a squad group to ground attack a marker position.  
---@param sgroupname string
---@param markername string
function Cmd_AttackGroundMarker(sgroupname, markername) end

---Order a squad group to ground attack a position.  
---@param sgroupname string
---@param targetposition Position
function Cmd_AttackGroundPos(sgroupname, targetposition) end

---Order a squad group to ground move to a marker position.  
---@param sgroupname string
---@param markername string
function Cmd_AttackMoveMarker(sgroupname, markername) end

---Order a squad group to attack move to a position.  
---@param sgroupname string
---@param targetposition Position
function Cmd_AttackMovePos(sgroupname, targetposition) end

---Order a squad group to attack another squad group.  This
--- function should track the target squad groups position.  
---The source squads will not stop moving until they reach the
--- targeted squad group.
---@param sourcesquadgroup string
---@param targetsquadgroup string
function Cmd_AttackSGroup(sourcesquadgroup, targetsquadgroup) end

---Command an sgroup of units to capture an egroup of strategic
--- points 
---@param sgroupname string
---@param egroupname string
function Cmd_Capture(sgroupname, egroupname) end

---Checks the sgroup for an entity that can cast the ability and
--- casts it on the target pos.  Returns false if no items in the
--- group can cast. 
---@param sgroupname string
---@param abilityname string
---@param targetpos Position
---@return boolean
function Cmd_CastAbilityMarker(sgroupname, abilityname, targetpos) end

---Checks the sgroup for an entity that can cast the ability and
--- casts it on the target pos.  Returns false if no items in the
--- group can cast. 
---@param sgroupname string
---@param abilityname string
---@param targetpos Position
---@return boolean
function Cmd_CastAbilityPos(sgroupname, abilityname, targetpos) end

---Checks the sgroup for an entity that can cast the ability and
--- casts it on the target pos.  Returns false if no items in the
--- group can cast. 
---@param sgroupname string
---@param abilityname string
---@param targetgroup string
---@return boolean
function Cmd_CastAbilitySGroup(sgroupname, abilityname, targetgroup) end

---Checks the sgroup for an entity that can cast the ability and
--- casts on itself Returns false if no items in the group can
--- cast. 
---@param sgroupname string
---@param abilityname string
---@return boolean
function Cmd_CastAbilitySelf(sgroupname, abilityname) end

---Command an sgroup of builders to construct a building at a
--- marker position 
---@param builderSGroup string
---@param buildingBlueprint string
---@param markername string
function Cmd_ConstructBlueprintMarker(builderSGroup, buildingBlueprint, markername) end

---Command an sgroup of builders to construct a building an enity
--- (for listening posts etc) 
---@param builderSGroup string
---@param buildingBlueprint string
---@param egroupName string
function Cmd_ConstructBlueprintOnEGroup(builderSGroup, buildingBlueprint, egroupName) end

---Command an sgroup of builders to construct a building at a
--- position 
---@param builderSGroup string
---@param buildingBlueprint string
---@param pos Position
function Cmd_ConstructBlueprintPos(builderSGroup, buildingBlueprint, pos) end

---Command all buildings in an EGroup to unload their squads 
---@param buildinggroup string
function Cmd_EmptyBuilding(buildinggroup) end

---Command all transports in an SGroup to move to position and
--- unload 
---@param sgroupname string
---@param pos Position
function Cmd_EmptyTransport(sgroupname, pos) end

---Command a SGroup to enter the first building in buildinggroup 
---@param sgroupname string
---@param buildinggroup string
function Cmd_EnterBuilding(sgroupname, buildinggroup) end

---Command a SGroup to enter a transport in transportgroup 
---@param sgroupname string
---@param transportgroup string
function Cmd_EnterTransport(sgroupname, transportgroup) end

---Enables/Disables infiltration for an sgroup 
---@param sgroupName string
---@param on boolean
function Cmd_Infiltrate(sgroupName, on) end

---Returns true if ALL or ANY squads in a group are currently
--- capturing something. 
---Set all to true to check for ALL or set to false to check for
--- ANY
---@param sgroupname string
---@param all boolean
---@return boolean
function Cmd_IsCapturing(sgroupname, all) end

---Returns true if ALL or ANY squads in a group are currently
--- constructing a building. 
---Set all to true to check for ALL or set to false to check for
--- ANY
---@param sgroupname string
---@param all boolean
---@return boolean
function Cmd_IsConstructingBuilding(sgroupname, all) end

---Returns true if ALL or ANY entities in a group are currently
--- producing squads 
---Set all to true to check for ALL or set to false to check for
--- ANY
---@param egroupname string
---@param all boolean
---@return boolean
function Cmd_IsProducingSquads(egroupname, all) end

---Returns true if ALL or ANY squads in a group are currently
--- calling for reinforcments. 
---Set all to true to check for ALL or set to false to check for
--- ANY
---@param sgroupname string
---@param all boolean
---@return boolean
function Cmd_IsReinforcing(sgroupname, all) end

---Returns true if ALL or ANY squads in a group are currently
--- upgrading. 
---Set all to true to check for ALL or set to false to check for
--- ANY
---@param sgroupname string
---@param all boolean
---@return boolean
function Cmd_IsUpgrading(sgroupname, all) end

---Command an sgroup to jump to a given marker position.  
---Note: Jump restrictions such as range etc apply.
---@param sgroupname string
---@param markername string
function Cmd_JumpToMarker(sgroupname, markername) end

---Command an sgroup to jump to a given position.  The squad will
--- jump to the closest possible position. 
---Note: Jump restrictions such as range etc apply.
---@param sgroupname string
---@param position Position
function Cmd_JumpToPos(sgroupname, position) end

---Moves a squad group to the closest marker in a list of marker
--- names. 
---@param sgroupName string
---@param markerNameTable StringTable
---@return markerID
function Cmd_MoveToClosestMarker(sgroupName, markerNameTable) end

---Move a squad group to the center of an entity group. 
---The move command will be issued by the player owner of the first
--- item in the group.  This function does nothing if the group is
--- empty.
---@param sgroupname string
---@param egroupname string
function Cmd_MoveToEGroup(sgroupname, egroupname) end

---Move a squad group to a given markers position. 
---The move command will be issued by the player owner of the first
--- item in the group.  This function does nothing if the group is
--- empty.
---@param sgroupname string
---@param markername string
function Cmd_MoveToMarker(sgroupname, markername) end

---Move a squad group to another squad group.  This function should
--- track the target squad groups position.  
---The source squads will not stop moving until they reach the
--- targeted squad group.
---@param sourcesquadgroup string
---@param targetsquadgroup string
function Cmd_MoveToSGroup(sourcesquadgroup, targetsquadgroup) end

---Calls possesion on a squads in a group.  Requirments for
--- possesion must be met 
---@param sgroupName string
function Cmd_Possesion(sgroupName) end

---Order a squad group to ramapge to a marker pos/ 
---@param sgroupname string
---@param targetposition Position
function Cmd_RampageMarker(sgroupname, targetposition) end

---Order a squad group to ramapge to a position.  
---@param sgroupname string
---@param targetposition Position
function Cmd_RampagePos(sgroupname, targetposition) end

---Sends a reinforce leader command to all squads in a group. 
--- count represents the number of commands to send. 
---Note: To create a squad bypassing the pre-reqs and costs use
--- Util_CreateSquadsAtPositionEx.
---@param squadgroupname string
---@param count integer
function Cmd_ReinforceLeader(squadgroupname, count) end

---Sends a reinforce command to all squads in a group.  count
--- represents the number of commands to send. 
---Note: To create a squad bypassing the pre-reqs and costs use
--- Util_CreateSquadsAtPositionEx.
---@param squadgroupname string
---@param count integer
function Cmd_ReinforceTrooper(squadgroupname, count) end

---Order a squad group set their stance.  Use MSTANCE_Assault,
--- MSTANCE_Ranged, MSTANCE_Versatile. 
---The order will be issued by the owner of the squad group.  This
--- function does nothing if the squad group is empty.
---@param squadgroupname string
---@param stance integer
function Cmd_SetMeleeStance(squadgroupname, stance) end

---Order a squad group set their stance.  Use STANCE_Hold,
--- STANCE_StandGround, STANCE_Burn, STANCE_CeaseFire, or
--- STANCE_Attack. 
---The order will be issued by the owner of the squad group.  This
--- function does nothing if the squad group is empty.
---@param squadgroupname string
---@param stance integer
function Cmd_SetStance(squadgroupname, stance) end

---Command an egroup to stop.  This is the same as issuing a stop
--- from the taskbar 
---@param egroupname string
function Cmd_StopEntities(egroupname) end

---Command an sgroup to stop.  This is the same as issuing a stop
--- from the taskbar 
---@param sgroupname string
function Cmd_StopSquads(sgroupname) end

---Construct a building at a location 
---@param player PlayerID
---@param builders SGroupID
---@param ebpname string
---@param pos Position
function Command_ConstructBuilding(player, builders, ebpname, pos) end

--- Send a entity command to a entity group(CMD_DefaultAction,
--- CMD_Stop, CMD_Destroy, CMD_BuildSquad, CMD_CancelProduction,
--- CMD_RallyPoint) 
--- Entity commands are mostly used for buildings etc.  If you need
--- to issue commands to units, use the Squad_Command function.
---@param player PlayerID
---@param egroup EGroupID
---@param entityCommand integer
function Command_Entity(player, egroup, entityCommand) end

---Send a entity-based command to an entity group. 
--- Use this function to issue orders that require a entity to an
--- entity group (eg. order a building to attack another building)
--- See Command_Entity for a list of all the possible
--- entityCommands.
---@param player PlayerID
---@param egroup EGroupID
---@param entityCommand integer
---@param target EGroupID
function Command_EntityEntity(player, egroup, entityCommand, target) end

---Send a position command to an entity group. 
--- Use this function to issue orders that require a position to an
--- entity group (eg. set a rally point for a building)
--- See Command_Entity for a list of all the possible
--- entityCommands.
---@param player PlayerID
---@param egroup EGroupID
---@param entityCommand integer
---@param target Position
function Command_EntityPos(player, egroup, entityCommand, target) end

---Send a squad-based command to an entity group. 
--- Use this function to issue orders that require a squad to an
--- entity group (eg. order a building to attack a squad)
--- See Command_Entity for a list of all the possible
--- entityCommands.
---@param player PlayerID
---@param egroup EGroupID
---@param entityCommand integer
---@param target SGroupID
function Command_EntitySquad(player, egroup, entityCommand, target) end

---Returns true if the given ssquad command has been issued in the
--- last num of seconds. 
---See Command_Entity for possible cmd types
---@param cmd integer
---@param secs number
---@return boolean
function Command_HasBeenIssued(cmd, secs) end

---Returns true if the given ssquad command has been issued in the
--- last num of seconds. 
---See Command_Squad for possible squadCmd types
---@param squadCmd integer
---@param secs number
---@return boolean
function Command_HasBeenIssuedSquad(squadCmd, secs) end

--- Send a squad command to a squad group 
--- SCMD_DefaultAction, SCMD_Move, SCMD_Stop, SCMD_Destroy, 
--- SCMD_BuildStructure, SCMD_Capture, SCMD_Attack,
--- SCMD_ReinforceTrooper, SCMD_ReinforceLeader,  SCMD_UpgradeTroop
---er, SCMD_CancelReinforcement, SCMD_AttackGround, SCMD_Melee,
--- SCMD_Stance,  SCMD_StanceMelee, SCMD_AttackMove, SCMD_Jump,
--- SCMD_Ability, SCMD_Attach, SCMD_Detach, SCMD_ReinforceTrooperOv
---erwatch, SCMD_UpgradeTrooperOverwatch, SCMD_SetInfiltrating,
--- SCMD_Repair, SCMD_Load, SCMD_Unload, SCMD_Rampage,
--- SCMD_UnloadHere
---@param player PlayerID
---@param sgroup SGroupID
---@param squadCommand integer
function Command_Squad(player, sgroup, squadCommand) end

---Casts an ability at a location.  Returns false if nothing can
--- cast the ability 
---@param player PlayerID
---@param caster SquadID
---@param abilityname string
---@param pos Position
---@return boolean
function Command_SquadAbilityPos(player, caster, abilityname, pos) end

---Tries to cast ability on a target sgroup.  Returns false if
--- nothing can cast the ability 
---@param player PlayerID
---@param caster SquadID
---@param abilityname string
---@param target SGroupID
---@return boolean
function Command_SquadAbilitySGroup(player, caster, abilityname, target) end

---Tries to cast ability on self.  Returns false if nothing can
--- cast the ability 
---@param player PlayerID
---@param caster SquadID
---@param abilityname string
---@return boolean
function Command_SquadAbilitySelf(player, caster, abilityname) end

---Send an entity command to a squad group. 
--- Use this function to isssue an entity-based command to a squad
--- group. (eg. order a squad to attack a building)
--- See Command_Squad for a list of all the possible squadCommands
---@param player PlayerID
---@param sgroup SGroupID
---@param squadCommand integer
---@param target EGroupID
function Command_SquadEntity(player, sgroup, squadCommand, target) end

--- Send a squad command to a squad group with custom data 
---@param player PlayerID
---@param sgroup SGroupID
---@param squadCommand integer
---@param cmdparam integer
---@param cmdflags integer
function Command_SquadExt(player, sgroup, squadCommand, cmdparam, cmdflags) end

---Send a position command to a squad group. 
--- Use this function to issue orders that require a position to a
--- squad group (eg. order a squad to move to position, or attack
--- position)
--- See Command_Squad for a list of all the possible squadCommands.
---@param player PlayerID
---@param sgroup SGroupID
---@param squadCommand integer
---@param target Position
function Command_SquadPos(player, sgroup, squadCommand, target) end

---Send an squad-based command to a squad group. 
--- Use this function to isssue a squad-based command to a squad
--- group. (eg. order a squad to attack another squad)
--- See Command_Squad for a list of all the possible squadCommands
---@param player PlayerID
---@param sgroup SGroupID
---@param squadCommand integer
---@param target SGroupID
function Command_SquadSquad(player, sgroup, squadCommand, target) end

---Assigns a cpu player to a marker to gather forces with a
--- priority 
--- The preference works in an additive way. Thus, if the
--- preferences are:
--- Marker1:	10
--- Marker2:	20
--- Marker3:	5
--- Then the probability that the next unit built gathers at
--- marker3 is 5/(10+20+5) = ~14% chance
---
---@param player PlayerID
---@param marker MarkerID
---@param preference number
function Cpu_AssignWaypoint(player, marker, preference) end

---Removes all the building limits set by Cpu_SetBuildingLimit 
--- The building_name corresponds to the file name
--- (e.g. "space_marine_vehicle_building" and not "machine_cult")
---@param player PlayerID
function Cpu_ClearBuildingLimits(player) end

---Gets the AI to try to take back the closest VO 
---
---@param player PlayerID
function Cpu_CounterVictoryObj(player) end

---Run this string in the AI of player 
---@param player PlayerID
---@param s string
function Cpu_DoString(player, s) end

---Enables/Disables a cpuplayer 
---@param player PlayerID
---@param enable boolean
function Cpu_Enable(player, enable) end

---Enables/Disables all cpuplayers 
---@param enable boolean
function Cpu_EnableAll(enable) end

---Enables/disables a component of the AI 
--- Components:
---	CT_Attacking
---	CT_Resourcing
---	CT_BuildUnits
---	CT_BuildBuildings
---	CT_BuildResearch
---	CT_BuildAddOns	CT_Tactics
---@param player PlayerID
---@param enable boolean
---@param ct integer
function Cpu_EnableComponent(player, enable, ct) end

---Force the AI to attack 
---
---@param player PlayerID
---@param attack PlayerID
function Cpu_ForceAttack(player, attack) end

---Get the AI difficulty setting of this player 
--- AD_Easy
--- AD_Standard
--- AD_Hard
--- AD_Advanced
--- AD_Insane
---
---@param player PlayerID
---@return integer
function Cpu_GetDifficulty(player) end

---Returns true if player is a cpu player 
---@param player PlayerID
---@return boolean
function Cpu_IsCpuPlayer(player) end

---Returns true if we are currently debugging player 
---@param player PlayerID
---@return boolean
function Cpu_IsDebugging(player) end

---Returns true if player is a CpuPlayer and is enabled 
---@param player PlayerID
---@return boolean
function Cpu_IsEnabled(player) end

---Lock an EGroup from the cpuplayer 
--- NOTE: Only use this for strategic points and buildings! It
--- won't work for anything else
---@param cpu_player PlayerID
---@param egroup EGroup
function Cpu_LockEGroup(cpu_player, egroup) end

---Lock an EGroup from all cpuplayers 
--- NOTE: Only use this for strategic points and buildings! It
--- won't work for anything else
---@param egroup EGroup
function Cpu_LockEGroupAcrossPlayers(egroup) end

---Lock an entity so the cpuplayer can't touch it 
---	Lockable entities: buildings and strategic points. If you would
--- like to lock a squad, use Cpu_LockSquad 
---@param player PlayerID
---@param id integer
function Cpu_LockEntity(player, id) end

---Lock an SGroup from the cpuplayer 
---@param cpu_player PlayerID
---@param sgroup SGroup
function Cpu_LockSGroup(cpu_player, sgroup) end

---Lock an SGroup from all cpuplayers 
---@param sgroup SGroup
function Cpu_LockSGroupAcrossPlayers(sgroup) end

---Lock a squad so the cpuplayer can't touch it 
---	Locking a squad forces the cpuplayer to let go of control of
--- the squad 	so designers can do what they like with them without
--- fear of the AI conflicting
---@param player PlayerID
---@param id integer
function Cpu_LockSquad(player, id) end

---Reset the demand table 
---@param player PlayerID
function Cpu_ResetBuildTable(player) end

---Reset the preferences for unit classes 
---@param player PlayerID
function Cpu_ResetClassPreferences(player) end

---Set an entry in the demand table (Use SetBuildTable instead of
--- this function) 
---@param player PlayerID
---@param name string
---@param start number
---@param increment number
function Cpu_SetBuildDemand(player, name, start, increment) end

---Set a build order table for the cpu_player 
--- An example build table:
---	local build_table =
---	{
---		{"space_marine_squad_servitor", 1, 5},
---		{"space_marine_squad_scout", 2, 3}
---	}
--- The first number in each entry is the starting value -- the
--- lowest number gets built first
--- The second number is the increment that happens each time the
--- unit is built
--- NOTE: Setting build demand table clobbers the class preferences
---@param cpu_player PlayerID
---@param build_table table
function Cpu_SetBuildTable(cpu_player, build_table) end

---Puts a limit on the number of this type of building that can be
--- built 
--- The building_name corresponds to the file name
--- (e.g. "space_marine_vehicle_building" and not "machine_cult")
---@param player PlayerID
---@param building_name string
---@param limit integer
function Cpu_SetBuildingLimit(player, building_name, limit) end

---Set limits to the buildings that an AI can build 
--- An example buildings table:
---	local buildings_table =
---	{
---		{"space_marine_vehicle_building", 2},
---		{"space_marine_listening_post", 1}
---	}
--- The first argument is the building name, the second is the
--- number of those buildings the AI can build
---@param cpu_player PlayerID
---@param buildings_table table
function Cpu_SetBuildingLimitsTable(cpu_player, buildings_table) end

---Set the preference for a particular unit class 
--- UnitClasses: 
---     UC_LightInfantryLow 
---     UC_LightInfantryMed 
---     UC_LightInfantryHigh 
---     UC_HeavyInfantryMed 
---     UC_HeavyInfantryHigh 
---     UC_VehicleLow 
---     UC_VehicleMed 
---     UC_VehicleHigh 
---     UC_MonsterMed 
---     UC_MonsterHigh 
---     UC_Commander 
---
--- The preference works in an additive way. Thus, if the
--- preferences are:
--- UC_LightInfantry:	10
--- UC_HeavyInfantry:	20
--- UC_Vehicle:			5
--- Then the probability that a vehicle is built next is
--- 5/(10+20+5) = ~14% chance
--- NOTE: Setting class preferences clobbers the build demand table
---@param player PlayerID
---@param unit_class integer
---@param preference number
function Cpu_SetClassPreference(player, unit_class, preference) end

---Set the AI difficulty setting of this player 
--- AD_Easy
--- AD_Standard
--- AD_Hard
--- AD_Advanced
--- AD_Insane
---
---@param player PlayerID
---@param difficulty integer
function Cpu_SetDifficulty(player, difficulty) end

---Set the difficulty without worrying if the Cpu is enabled or not
--- 
---If you set the difficulty on a Cpu that's disabled, it'll crash.
--- This should take care of that. NOTE: You can always use the
--- regular one, this is only if you don't know or may not know
---@param cpu_player PlayerID
---@param difficulty_setting integer
function Cpu_SetDifficultyForDummies(cpu_player, difficulty_setting) end

---If enabled, cpu player talks smack 
---@param player PlayerID
---@param enable boolean
function Cpu_SetLippy(player, enable) end

---Set the AI to have a maximum army of a percentage of the enemy
--- army 
---[0.0,1.0]
---@param player PlayerID
---@param percent number
function Cpu_SetMaxPercentage(player, percent) end

---Unlock all locked resources so the cpuplayer has control of them
--- 
---@param player PlayerID
function Cpu_UnlockAll(player) end

---Unlock an EGroup from the cpuplayer 
--- NOTE: Only use this for strategic points and buildings! It
--- won't work for anything else
---@param cpu_player PlayerID
---@param egroup EGroup
function Cpu_UnlockEGroup(cpu_player, egroup) end

---Lock an EGroup from all cpuplayers 
--- NOTE: Only use this for strategic points and buildings! It
--- won't work for anything else
---@param egroup EGroup
function Cpu_UnlockEGroupAcrossPlayers(egroup) end

---Unlock an entity so the cpuplayer has control of it 
---@param player PlayerID
---@param id integer
function Cpu_UnlockEntity(player, id) end

---Unlock an SGroup from the cpuplayer 
---@param cpu_player PlayerID
---@param sgroup SGroup
function Cpu_UnlockSGroup(cpu_player, sgroup) end

---Lock an SGroup from all cpuplayers 
---@param sgroup SGroup
function Cpu_UnlockSGroupAcrossPlayers(sgroup) end

---Unlock a squad so the cpuplayer has control of it 
---@param player PlayerID
---@param id integer
function Cpu_UnlockSquad(player, id) end

---Returns the difficulty level for the mission (DIFFICULTY_EASY,
--- DIFFICULTY_NORMAL or DIFFICULTY_HARD) 
---@return integer
function Difficulty_Get() end

---Set the difficulty level for all players in a mission 
---This function should be called to set the difficulty level for
--- all players in the mission.  Note that it WILL NOT modify
--- attributes for the player themselves, just everyone else.  
--- Values for difficulty are: 0 = easy, 1 = normal, 2 = hard
---@param difficultlevel integer
function Difficulty_SetForAll(difficultlevel) end

---Set the difficulty level for one player 
---This function should be called to set the difficulty level for a
--- single player in a mission.  Values for difficulty are: 0 =
--- easy, 1 = normal, 2 = hard
---@param playerId PlayerID
---@param difficultlevel integer
function Difficulty_SetForPlayer(playerId, difficultlevel) end

---Returns true if an entity blueprint exists with the given name. 
---@param name string
---@return boolean
function EBP_Exists(name) end

---Adds an entity to the end of a group if the group doesnt already
--- have it. 
---@param group EGroupID
---@param entity EntityID
function EGroup_Add(group, entity) end

---Appends the entities in one group to another group.   
--- All entities from 'grouptoadd' will be added to 'group'.
--- If 'group' already contains an entity from 'grouptoadd' it will
--- not be added.
--- This function does not clear the contents of 'grouptoadd'.
--- Example: Add group2 (0, 10, 11, 22) to group1 (1, 11, 20) --
--- group1 would now be (1, 11, 20, 0, 10, 22)
---
---@param group EGroupID
---@param grouptoadd EGroupID
function EGroup_AddGroup(group, grouptoadd) end

---Returns true if ALL or ANY entities in a group can see ALL or
--- ANY entities in a given egroup. 
---@param egroupname string
---@param targetegroupname string
---@param all boolean
---@return boolean
function EGroup_CanSeeEGroup(egroupname, targetegroupname, all) end

---Returns true if ALL or ANY entities in a group can see ALL or
--- ANY squads in a given sgroup. 
---@param egroupname string
---@param targetsgroupname string
---@param all boolean
---@return boolean
function EGroup_CanSeeSGroup(egroupname, targetsgroupname, all) end

---Returns true if ALL or ANY items in a group have the given
--- active command 
---See Entity_GetActiveCommand for valid command types
---@param groupname string
---@param cmd integer
---@param all boolean
---@return boolean
function EGroup_CheckActiveCommand(groupname, cmd, all) end

---Removes all entities from a group 
---@param egroup EGroupID
function EGroup_Clear(egroup) end

---Returns true if the contents of the two groups are equal. Order
--- of the entities does not matter. 
---@param group1 EGroupID
---@param group2 EGroupID
---@return boolean
function EGroup_Compare(group1, group2) end

---Returns true if ALL or ANY items in a group have built the given
--- addon 
---@param groupname string
---@param addonname string
---@param all boolean
---@return boolean
function EGroup_ContainsAddOn(groupname, addonname, all) end

---Check if a group contains ALL or ANY of the items in a blueprint
--- table. 
---See SGroup_ContainsBlueprints for more info.
---@param egroup EGroupID
---@param blueprintTable LuaConfig::RefTable
---@param all boolean
---@return boolean
function EGroup_ContainsBlueprints(egroup, blueprintTable, all) end

---Returns true if an sgroup contains a given squad 
---@param egroup EGroupID
---@param entity EntityID
---@return boolean
function EGroup_ContainsEntity(egroup, entity) end

---Returns the total number of spawned and despawned entities in a
--- group. 
---@param egroup EGroupID
---@return integer
function EGroup_Count(egroup) end

---Returns the number of despawned entities in a group. 
---@param egroup EGroupID
---@return integer
function EGroup_CountDeSpawned(egroup) end

---Returns the number of spawned entities in a group. 
---@param egroup EGroupID
---@return integer
function EGroup_CountSpawned(egroup) end

---Returns a new entity group with the given name. 
--- Entity groups are used for buildings and objects such as rocks
--- and trees.
--- If you need to issue orders to a group vehicles or units you
--- must use a SquadGroup.
---@param name string
---@return EGroupID
function EGroup_Create(name) end

---Find a entity group from name.  Creates a new one with given
--- name if it doesnt exist. 
---@param name string
---@return EGroupID
function EGroup_CreateIfNotFound(name) end

---Despawn all spawned entities in a group. 
---@param groupname string
function EGroup_DeSpawn(groupname) end

---Manually destroy a group that you dont need anymore. 
---@param egroup EGroupID
function EGroup_Destroy(egroup) end

---Destroys all spawned and despawned entities in a group. 
--- Be careful not to confuse this with EGroup_Destroy which
--- destroys the group and NOT the items it contains.  This
--- function will destroy spawned and despawned items in a group
---@param groupname string
function EGroup_DestroyAllEntities(groupname) end

---Returns true if the entity group with the given name exists 
---@param name string
---@return boolean
function EGroup_Exists(name) end

---Call a lua function for each item in a group. Function will
--- recieve (groupid, itemindex, itemid) and should return true to
--- break or false to continue. 
--- function Rule_Test( )
---     local DespawnEntity = function( egroupid, itemindex,
--- entityID )
---         Entity_Despawn( entityID )
---     end
---
---     EGroup_ForEach( EGroup_FromName("eg_PlayersHQ"),
--- DespawnEntity )
--- end
--- Note: This function iterates over SPAWNED ENTITIES ONLY.
---@param egroup EGroupID
---@param f LuaBinding::StackVarFunction
---@return boolean
function EGroup_ForEach(egroup, f) end

---Call a lua function for each item in a group. Function will
--- recieve (groupid, itemindex, itemid) and should return a bool.
--- 
--- This is used for checking if ALL or ANY items in a group match
--- a given predicate. (eg. Are all items in a group choas
--- marines)
--- Note: This function iterates over SPAWNED ENTITIES ONLY.
---@param egroup EGroupID
---@param all boolean
---@param f LuaBinding::StackVarFunction
---@return boolean
function EGroup_ForEachAllOrAny(egroup, all, f) end

---Same as EGroup_ForEachAllOrAny except you have a choice to
--- iterate over spawned entities, despawned entities, or both. 
---@param egroup EGroupID
---@param all boolean
---@param f LuaBinding::StackVarFunction
---@param spawned boolean
---@param despawned boolean
---@return boolean
function EGroup_ForEachAllOrAnyEx(egroup, all, f, spawned, despawned) end

---Same as EGroup_ForEach except you have a choice to iterate over
--- spawned entities, despawned entities, or both. 
---@param egroup EGroupID
---@param f LuaBinding::StackVarFunction
---@param spawned boolean
---@param despawned boolean
---@return boolean
function EGroup_ForEachEx(egroup, f, spawned, despawned) end

---Gives addon to all members of the group 
---@param groupname string
---@param addonname string
---@return boolean
function EGroup_ForceAddOn(groupname, addonname) end

---Find an entity group with a given name. 
---@param name string
---@return EGroupID
function EGroup_FromName(name) end

---Returns the average health of all units in a entity group as a
--- weighted percent [0.0, 1.0]. 
---Example: A group of entities with health 20/100 and 100/1000
--- would return 120/1100 ( approx 11 % )
---@param groupname string
---@return number
function EGroup_GetAvgHealth(groupname) end

---Returns the despawned entity at the given index. 
--- Use EGroup_GetSpawnedEntityAt if you want the spawned items in
--- the group
--- Use index 1 to get the first entity in the group.
--- It is an error if index  EGroup_GetCountDeSpawned()
---
---@param group EGroupID
---@param index integer
---@return EntityID
function EGroup_GetDeSpawnedEntityAt(group, index) end

---Check invulnerablity state for ALL or ANY entity in an entity
--- group. 
--- Set all param to true to check for ALL or set to false to check
--- for ANY.
---@param groupname string
---@param all boolean
---@return boolean
function EGroup_GetHealthInvulnerable(groupname, all) end

---Returns the name of a given entity group. 
---@param egroup EGroupID
---@return string
function EGroup_GetName(egroup) end

---Returns the center position of an entity group. 
---@param group EGroupID
---@return Position
function EGroup_GetPosition(group) end

---Returns the spawned entity at the given index. 
--- Use EGroup_GetDeSpawnedEntityAt if you want the despawned items
--- in the group
--- Use index 1 to get the first entity in the group.
--- It is an error if index  EGroup_GetCountSpawned()
---
---@param group EGroupID
---@param index integer
---@return EntityID
function EGroup_GetSpawnedEntityAt(group, index) end

---Return true if any entities in a group contain a strategic point
--- with a structure on it 
---@param groupname string
---@return boolean
function EGroup_HasStrategicPointWithStructure(groupname) end

---Performs a group intersection.  
--- Only entities that are in both groups will be added to
--- 'group'.
--- The contents of 'grouptointersect' will not be changed.
--- Example: group = (1,2,3,4,5,6)  grouptointersect = (2,4,6,8,10)
---   -- group will now equal (2,4,6)
---@param group EGroupID
---@param grouptointersect EGroupID
function EGroup_Intersection(group, grouptointersect) end

---Returns true if all or any strategic points in a group have been
--- captured. 
---This function will ignore all entities that cannot be captured
--- and will return false if no entities in the group can be
--- captured.
---@param groupname string
---@param playerId PlayerID
---@param all boolean
---@return boolean
function EGroup_IsCapturedByPlayer(groupname, playerId, all) end

---Returns true if a named entity group contains no spawned or
--- despawned entities 
---@param name string
---@return boolean
function EGroup_IsEmpty(name) end

---Returns true if ALL or ANY entities are in cover. 
---@param egroupname string
---@param all boolean
---@return boolean
function EGroup_IsInCover(egroupname, all) end

---Return true if all items in a group are selected 
---@param groupname string
---@return boolean
function EGroup_IsSelected(groupname) end

---Returns true if ALL or ANY entities are under attack. 
---@param egroupname string
---@param all boolean
---@return boolean
function EGroup_IsUnderAttack(egroupname, all) end

---Returns true if ALL or ANY entities are under attack. 
---@param egroupname string
---@param all boolean
---@param player PlayerID
---@return boolean
function EGroup_IsUnderAttackByPlayer(egroupname, all, player) end

---Respawn all despawned entities in a group. 
---@param groupname string
function EGroup_ReSpawn(groupname) end

---Removes an entity from a group. 
---@param group EGroupID
---@param entity EntityID
function EGroup_Remove(group, entity) end

---Call Entity_ResetConstructionState for all spawned buildings 
---See Entity_ResetConstructionState for more info
---@param groupname string
function EGroup_ResetConstructionState(groupname) end

---Resets the player owner to the world for all entities in the
--- group 
---@param groupname string
function EGroup_ResetPlayerOwner(groupname) end

---Sets the health of each unit in an entity group to a given
--- percent [0.0, 1.0]. 
---@param groupname string
---@param healthPercent number
---@return number
function EGroup_SetAvgHealth(groupname, healthPercent) end

---Forces all buildings in progress to complete their construction
--- state 
---@param groupname string
function EGroup_SetConstructionComplete(groupname) end

---Enable/Disable invulnerablity for an entire entity group. 
---IMPORTANT: You must disable invulnerability for everytime you
--- enable it!
---@param groupname string
---@param enabled boolean
function EGroup_SetHealthInvulnerable(groupname, enabled) end

---Changes the player owner for all spawned and despawned entities
--- of an EGroup. 
---@param groupname string
---@param owner PlayerID
function EGroup_SetPlayerOwner(groupname, owner) end

---Removes the attached entity from the path 
---@param eanimname string
function EPath_Finish(eanimname) end

---Moves/Rotates an entity group along a path. Entity remains at
--- end of path until EPath_Finish is called. 
---Works the same as SPath_Start
---@param eanimname string
---@param fromkeyframe string
---@param tokeyframe string
---@param egroup EGroupID
function EPath_Start(eanimname, fromkeyframe, tokeyframe, egroup) end

---Moves/Rotates an entity group along a path. 
---Works the same as SPath_StartEx
---@param eanimname string
---@param fromkeyframe string
---@param tokeyframe string
---@param egroup EGroupID
---@param speed number
---@param autoFinish boolean
function EPath_StartEx(eanimname, fromkeyframe, tokeyframe, egroup, speed, autoFinish) end

---Returns true if a target entity is in the entities sight radius
--- and is NOT infiltrated 
---@param entity EntityID
---@param target EntityID
---@return boolean
function Entity_CanSeeEntity(entity, target) end

---Returns true if a target squad is in the squads sight radius and
--- is NOT infiltrated. 
---@param entity EntityID
---@param target SquadID
---@return boolean
function Entity_CanSeeSquad(entity, target) end

---Returns true if addon has been built for the given entity 
---@param entity EntityID
---@param addonname string
---@return boolean
function Entity_ContainsAddOn(entity, addonname) end

---Creates an entity at a given position and assigns it to a given
--- player.  'blueprint' is a string value containing the name of
--- the entity blueprint. 
---@param blueprint string
---@param player PlayerID
---@param pos Position
---@return EntityID
function Entity_Create(blueprint, player, pos) end

---DeSpawn the entity at its current position 
---@param entity EntityID
function Entity_DeSpawn(entity) end

---Remove an entity from the world and destroy it. 
---@param entity EntityID
function Entity_Destroy(entity) end

---Instantly builds an addon on a building, ignoring building
--- requirments. 
---@param building EntityID
---@param addonname string
function Entity_ForceAddOn(building, addonname) end

---Get an entity from a mission editor ID. 
---@param id integer
---@return EntityID
function Entity_FromWorldID(id) end

---Returns the active entity command.  Some valid entity commands:
--- STATEID_Attack, STATEID_StructureBuilding, STATEID_StrategicPoi
---ntCapture, STATEID_Dead, STATEID_Idle, STATEID_Pause 
---@param entity EntityID
---@return integer
function Entity_GetActiveCommand(entity) end

---Returns the ID of the entity blueprint 
---@param entity EntityID
---@return integer
function Entity_GetBlueprintID(entity) end

---Returns the ID of the blueprint from blueprint file name 
---@param blueprint string
---@return integer
function Entity_GetBlueprintIDFromName(blueprint) end

---Returns the name of the entity blueprint (from the attribute
--- editor) 
---@param entity EntityID
---@return string
function Entity_GetBlueprintName(entity) end

---Returns the construction progress (with range [0.0, 1.0] for a
--- given entity.  Returns 0.0 if the entity is not a building. 
---@param entity EntityID
---@return number
function Entity_GetBuildingProgress(entity) end

---Returns the type of cover an entity is in. 
--- Returns CT_None if entity does not have cover extension Returns
--- one of the possible cover types: CT_None, CT_Light, CT_Heavy,
--- CT_Negative, CT_Blocking, CT_Stealth
---@param entity EntityID
---@return integer
function Entity_GetCoverType(entity) end

---Returns the entities unique id in the world 
---@param entity EntityID
---@return integer
function Entity_GetGameID(entity) end

---Returns the health of an entity. 
---Health will be zero for entities with no health extension.
---@param entity EntityID
---@return number
function Entity_GetHealth(entity) end

---Returns true if an entity is invulnerable. 
---@param entity EntityID
---@return boolean
function Entity_GetHealthInvulnerable(entity) end

---Returns the max health of an entity. 
---Max health will be zero for entities with no health extension.
---@param entity EntityID
---@return number
function Entity_GetHealthMax(entity) end

---Returns the Player owner of the given entity. Entity MUST NOT be
--- owned by the world. 
---@param entity EntityID
---@return PlayerID
function Entity_GetPlayerOwner(entity) end

---Returns the position of the entity.  The position is currently a
--- lua table with three entries (x, y, z) 
---@param entity EntityID
---@return Position
function Entity_GetPosition(entity) end

---Returns the name of the production item with index in range of
--- [0,n-1] 
--- There are currently 3 types of production items.  PQ_Squads,
--- PQ_AddOns, and PQ_Research.  Any of these items can in a
--- production queue.
---
--- The results of this function will vary depending on the
--- production items type.  For PQ_Squads, the squad blueprint name
--- will be returned, for PQ_AddOns the AddOn name will be
--- returned, and for PQ_Research the research name will be
--- returned.
---
--- Use Entity_GetProductionQueueItemType to get the type of a
--- given production queue item if you are interested in such
--- things.
---@param entity EntityID
---@param index integer
---@return string
function Entity_GetProductionQueueItemName(entity, index) end

---Returns the production type (PQ_AddOn, PQ_Squad or PQ_Research)
--- for a production queue item with index. 
---@param entity EntityID
---@param index integer
---@return integer
function Entity_GetProductionQueueItemType(entity, index) end

---Returns the number of items in the entities production queue. 
---It is an error to call this function on an entity that does not
--- have a production queue.
--- Use Entity_HasProductionQueue to check that the entity has a
--- queue.
---@param entity EntityID
---@return integer
function Entity_GetProductionQueueSize(entity) end

---Returns true if the entity is owned by the world 
---@param entity EntityID
---@return boolean
function Entity_GetWorldOwner(entity) end

---Returns true if an entity has a production queue. 
---@param entity EntityID
---@return boolean
function Entity_HasProductionQueue(entity) end

---Returns true if the given entity is a building 
---@param e EntityID
---@return boolean
function Entity_IsBuilding(e) end

---Returns true if a strategic point (an entity) is 100% captured
--- by a given player. 
---Check that the entity is a strategic point using
--- Entity_IsStrategic point before calling this function.
---@param entity EntityID
---@param player PlayerID
---@return boolean
function Entity_IsCapturedByPlayer(entity, player) end

---Returns true if entity is in cover. 
---@param entityId EntityID
---@return boolean
function Entity_IsInCover(entityId) end

---Returns true if an entity is a strategic point (eg. it can be
--- captured) 
---@param entity EntityID
---@return boolean
function Entity_IsStrategicPoint(entity) end

---Returns true if the entity was attacked in the last second. 
---@param entity EntityID
---@return boolean
function Entity_IsUnderAttack(entity) end

---Returns true if the entity was attacked by the given player over
--- the last second. 
---@param entity EntityID
---@param player PlayerID
---@return boolean
function Entity_IsUnderAttackByPlayer(entity, player) end

---Resets the construction state for a given building.   You will
--- not be able to build stuff from the building until construction
--- is completed 
---@param e EntityID
function Entity_ResetConstructionState(e) end

---Offsets entity from squad middle (specified distance is max) 
---@param entity EntityID
---@param fMaxRadius number
function Entity_ScatterFromSquad(entity, fMaxRadius) end

---Forces a building in progress to complete construction 
---@param e EntityID
function Entity_SetConstructionComplete(e) end

---Set the health of an entity.  healthPercent must be in the range
--- [0.0, 1.0]. 
---@param entity EntityID
---@param healthPercent number
function Entity_SetHealth(entity, healthPercent) end

---Make an entity invulnerable to physical damage. 
---IMPORTANT: You must disable invulnerability for everytime you
--- enable it!
---@param entity EntityID
---@param enabled boolean
function Entity_SetHealthInvulnerable(entity, enabled) end

---Changes the owner of the given squad. 
---@param entity EntityID
---@param owner PlayerID
function Entity_SetPlayerOwner(entity, owner) end

---Sets the position of the entity.  The position is currently a
--- lua table with three entries (x, y, z) 
---@param entity EntityID
---@param pos Position
function Entity_SetPosition(entity, pos) end

---Sets the owner of the entity to the world 
---@param entity EntityID
function Entity_SetWorldOwner(entity) end

---Spawn the entity at its current position 
---@param entity EntityID
function Entity_Spawn(entity) end

---Returns true the given strategic point has a structure on it 
---Throws an error if entity is not a strategic point
---@param e EntityID
---@return boolean
function Entity_StrategicPointHasStructure(e) end

---Spawn a custom event cue item from the data:art/ui/ingame/event_
---cue_icons/custom/ folder and the data:sound/ folder 
---ie. EventCue_DoEvent( 'icon_image', 'icon_sound', '$20400', 'You
--- can also just use text if you dont care for localization' )
---@param icon string
---@param sound string
---@param title LocString
---@param desc LocString
function EventCue_DoEvent(icon, sound, title, desc) end

---Toggles event cue ui and all vocalizations (selection, ordering,
--- game events...) 
---@param enabled boolean
function EventCue_Enable(enabled) end

---Pauses for a given amount of time. This function MUST be called
--- from a CTRL object in NISlet events only! 
---See Event_Start for more information on NISlet events.
---@param seconds number
function Event_Delay(seconds) end

---Returns true if any event is currently running 
---@return boolean
function Event_IsAnyRunning() end

---Returns true if a given event is running 
---@param f function
---@return boolean
function Event_IsRunning(f) end

---Completes execution of the event immediatley (all calls to
--- Wait() are ignored) 
function Event_Skip() end

---Starts event.  Event will not start until all rules are
--- evaluated for this frame! 
--- The only time you should need this function is to start playing
--- a NISLet.  If your NISLet is named NISLet01 you would call this
--- function like so:
---
---     Event_Start( NISLets.NISLet01, 0 )
---
--- Events are run until a call to Wait() is hit.  When the event
--- reaches Wait() it will not continue it execution until all
--- controllers it is waiting for are done.
--- To add a controller to the block simply call the controller
--- through the CTRL object.  For example, to call the Delay
--- controller you should call CTRL.Delay(5). This will make the
--- succesive call to Wait delay for 5 seconds.
---
--- Example: (NISLets are located in the .nis file for your
--- mission)
--- NISLets = {
---     MyNISLet = function ( )
---
---         print("Yo")
---         CTRL.Event_Delay(3)
---         print("This will show up right after Yo, it will not
--- wait for 3 seconds")
---         CTRL.Event_Delay(2)
---         Event_Wait() -- execution will now delay for 5 seconds
--- (waiting for controllers)
---
---         print("Yo, 5 seconds later")
---
---     end
---
--- To trigger the above NISLet from a Rule or your mission script
--- you would call Event_Start( NISLets.MyNISLet, 0 )
---
--- If you start an event that has higher priority then the current
--- event playing, the current event will be cancelled.
---
--- If you start an event that has lower or equal priority then the
--- current event playing, the event will be queued and it will
--- play when the current event is complete.
---
--- Zero is the highest priority and Zero+N is lower priority.
---
---
---@param f function
---@param priority integer
function Event_Start(f, priority) end

---Resets the revealed FOW areas and destroys all ghosted
--- structures. 
function FOW_Reset() end

---Reveal FOW for all players 
function FOW_RevealAll() end

---Reveals a circular area for all alive players over a given
--- duration. YOU SHOULD ONLY CALL THIS ONCE FOR EACH AREA. 
---Pass in a duration of -1 for indefinite duration ( until
--- unreveal is called )
---@param x number
---@param z number
---@param radius number
---@param durationSecs number
function FOW_RevealArea(x, z, radius, durationSecs) end

---Reveals an entity groups line of sight(LOS) for all alive
--- players over a given duration. YOU SHOULD ONLY CALL THIS ONCE
--- FOR EACH AREA. 
---Pass in a duration of -1 for indefinite duration ( until the
--- entities are dead )
---@param group EGroupID
---@param durationSecs number
function FOW_RevealEGroup(group, durationSecs) end

---Reveals an entities line of sight (LOS) for all alive players
--- over a given duration. YOU SHOULD ONLY CALL THIS ONCE FOR EACH
--- AREA. 
---Pass in a duration of -1 for indefinite duration ( until the
--- entity is dead )
---@param entity EntityID
---@param durationSecs number
function FOW_RevealEntity(entity, durationSecs) end

---Reveals an area the size of a given markers proximity at that
--- markers position for a given amount of time.  YOU SHOULD ONLY
--- CALL THIS ONCE FOR EACH AREA. 
---This function will reveal the FOW for ALL alive players.
---@param markername string
---@param durationSecs number
function FOW_RevealMarker(markername, durationSecs) end

---Reveals a squad groups line of sight(LOS) for all alive players
--- over a given duration. YOU SHOULD ONLY CALL THIS ONCE FOR EACH
--- AREA. 
---Pass in a duration of -1 for indefinite duration ( until the
--- squads are dead )
---@param group SGroupID
---@param durationSecs number
function FOW_RevealSGroup(group, durationSecs) end

---Reveals a squads line if sight(LOS) for all alive players over a
--- given duration. YOU SHOULD ONLY CALL THIS ONCE FOR EACH AREA. 
---Pass in a duration of -1 for indefinite duration ( until the
--- entity is dead )
---@param squad SquadID
---@param durationSecs number
function FOW_RevealSquad(squad, durationSecs) end

---Adds all items in a group to a given players FOW. 
---Items line of sight will be visible until it is destroyed or
--- UnTag is called.
---@param player PlayerID
---@param group EGroupID
function FOW_TagEGroup(player, group) end

---Adds all items in a group to a given players FOW. 
---Items line of sight will be visible until it is destroyed or
--- UnTag is called.
---@param player PlayerID
---@param group SGroupID
function FOW_TagSGroup(player, group) end

---Use to undo a FOW_RevealAll 
function FOW_UnRevealAll() end

---UnReveals a circular area for all alive players. YOU SHOULD ONLY
--- CALL THIS ONCE FOR EACH AREA. 
---@param x number
---@param z number
---@param radius number
function FOW_UnRevealArea(x, z, radius) end

---Removes items from a players FOW that were added using
--- FOW_TagEGroup 
---@param player PlayerID
---@param group EGroupID
function FOW_UnTagEGroup(player, group) end

---Removes items from a players FOW that were added using
--- FOW_TagSGroup 
---@param player PlayerID
---@param group SGroupID
function FOW_UnTagSGroup(player, group) end

---Returns true if a fade is currently playing. 
---@return boolean
function Fade_IsFading() end

---Starts a screen fade.  Set in to true to fade in and set to
--- false to fade out. 
---This function can be called through a CTRL object and waited on.
---  Will not fade out if the screen is already faded.
---@param timeSecs number
---@param in boolean
function Fade_Start(timeSecs, in) end

---Enable/Disable flashing on construction buttons for builder
--- units. 
---Pass in the ebp name for the building button you want to flash
---@param ebpname string
---@param enable boolean
---@param stopFlashingWhenClicked boolean
function Flash_ConstructionButton(ebpname, enable, stopFlashingWhenClicked) end

---Flash other binding. See extended description in scardoc. 
--- bindingname, bindingparams
--- move_modal, not 0 - used
--- attackmove_modal, 0 - not used
--- attackground_modal, 0 - not used
--- jump_modal, 0 - not used
--- rampage_modal, 0 - not used
--- unload_modal, 0 - not used
--- attach_modal, 0 - not used
--- player_resource, 0 - power resource, 1 - requistion resource, 2
--- - ork resource
--- focus_on_next, 0 - next builder button, 1 - next military
--- button, 2 - next research button
--- dlg_objectives, 0 - not used
--- build_structures_menu, 0 - not used
--- command_infiltrate, 0 - not used
---
---@param bindingname string
---@param bindingparam integer
---@param enable boolean
---@param stopFlashingWhenClicked boolean
function Flash_OtherButton(bindingname, bindingparam, enable, stopFlashingWhenClicked) end

---Enable/Disable flash production buttons itemtype PQ_Research,
--- PQ_Squad, or PQ_AddOn 
--- PQ_Research - itemname is the name of the research blueprint
--- PQ_AddOn - itemname is the name of the addon blueprint
--- PQ_Squad - itemname is the name of the squad blueprint
---
---@param itemtype integer
---@param itemname string
---@param enable boolean
---@param stopFlashingWhenClicked boolean
function Flash_ProductionButton(itemtype, itemname, enable, stopFlashingWhenClicked) end

---Enable/Disable flashing of reinforcement buttons itemtype
--- RT_Trooper, RT_Leader, RT_WeaponUpgrade 
---weaponname is only used when itemtype is RT_WeaponUpgrade
---@param itemtype integer
---@param squadbpname string
---@param weaponname string
---@param enable boolean
---@param stopFlashingWhenClicked boolean
function Flash_ReinforcementButton(itemtype, squadbpname, weaponname, enable, stopFlashingWhenClicked) end

---Returns the display text for a hotkey from the hotkeys file 
---@param hotkeyname string
---@return LocString
function Hotkey_GetText(hotkeyname) end

---Applies an entity modifier to an entity. 
---@param number integer
---@return LocString
function Loc_ConvertNumber(number) end

---Returns true if a generic number attribute exists for the marker
--- type. 
---@param marker MarkerID
---@param attrname string
---@return boolean
function Marker_DoesNumberAttributeExist(marker, attrname) end

---Returns true if a generic string attribute exists for the marker
--- type. 
---@param marker MarkerID
---@param attrname string
---@return boolean
function Marker_DoesStringAttributeExist(marker, attrname) end

---Returns true if marker exists. 
---@param name string
---@param type string
---@return boolean
function Marker_Exists(name, type) end

---Returns a ScarMarker given its name and type from the Mission
--- Editor. 
---  The type is defined in the markers list of the mission editor
--- when placing markers.
---  You will need the result of this function to call any
--- functions that start with Marker_
---@param name string
---@param type string
---@return MarkerID
function Marker_FromName(name, type) end

---Returns the name of a given marker.  This value gets set in the
--- Mission Editor. 
---@param marker MarkerID
---@return string
function Marker_GetName(marker) end

---Returns a generic number attribute defined in a marker. 
---  Different marker types have different attributes. Note: All
--- marker types have Name and Proximity.  Do not use this function
--- to retrieve these values, use Marker_GetName and
--- Marker_GetProximity instead.
---@param marker MarkerID
---@param attrname string
---@return number
function Marker_GetNumberAttribute(marker, attrname) end

---Returns the position of a given marker. 
---@param marker MarkerID
---@return Position
function Marker_GetPosition(marker) end

---Returns the proximity of a given marker.  This value gets set in
--- the Mission Editor. 
---@param marker MarkerID
---@return number
function Marker_GetProximity(marker) end

---Returns a generic string attribute defined in a marker. 
---  Different marker types can have different attributes.
---  Note: All marker types have Name and Proximity.  Do not use
--- this function to retrieve these values, use Marker_GetName and
--- Marker_GetProximity instead.
---  See markertypes.lua for a listing of all the marker types.
---@param marker MarkerID
---@param attrname string
---@return string
function Marker_GetStringAttribute(marker, attrname) end

---Returns the typename of a given marker.  This is the typename
--- from the Mission Editor (name displayed when placing markers) 
---@param marker MarkerID
---@return string
function Marker_GetType(marker) end

---Returns true if the given position is in the markers proximity
--- radius. 
---@param marker MarkerID
---@param pos Position
---@return boolean
function Marker_InProximity(marker, pos) end

---Breaks the morale of the currently selected squad 
function Misc_BreakCurrentlySelectedSquad() end

---Checks if any input event have occured in the last second 
function Misc_DetectAnyInput() end

---Applys/Removes a modifier to an egroup 
---@param groupid EGroupID
---@param modifier Modifier
---@param apply boolean
function ModifierUtil_ApplyToEGroup(groupid, modifier, apply) end

---Applys/Removes a modifier to a squadgroup 
---@param groupid SGroupID
---@param modifier Modifier
---@param apply boolean
function ModifierUtil_ApplyToSGroup(groupid, modifier, apply) end

---Returns the applier table index for a given modifier. 
---Returns zero if applier for modifier is not found.
---@param modifier Modifier
---@param playerid PlayerID
---@return integer
function ModifierUtil_GetEntityApplierIndex(modifier, playerid) end

---Returns the applier table index for a given modifier. 
---Returns zero if applier for modifier is not found.
---@param modifier Modifier
---@param entityid EntityID
---@return integer
function ModifierUtil_GetEntityApplierIndex(modifier, entityid) end

---Returns the applier table index for a given modifier. 
---Returns zero if applier for modifier is not found.
---@param modifier Modifier
---@param squadid SquadID
---@return integer
function ModifierUtil_GetSquadApplierIndex(modifier, squadid) end

---Applies a player modifier to a player. 
---@param modifier ScarModifier
---@param player PlayerID
---@return integer
function Modifier_ApplyToPlayer(modifier, player) end

---Applies a squad modifier to a squad. 
---@param modifier ScarModifier
---@param squad SquadID
---@return integer
function Modifier_ApplyToSquad(modifier, squad) end

---Returns a modifier that you can apply to stuff. 
--- applicationType : MAT_EntityType, MAT_Entity, MAT_Player,
--- MAT_Squad, MAT_SquadType, or MAT_WeaponType.
--- modtype : Name of modifier from the modifiers table in the
--- attribute editor.
--- usageType : MUT_Addition, MUT_Multiplication or MUT_Enable.
--- exclusive : 
--- value : The value for the modifier.
--- extname : The name of the entity/squad/weapon blueprint for
--- when the applicationType is MAT_EntityType, MAT_SquadType or
--- MAT_WeaponType.
---@param applicationType integer
---@param modtype string
---@param usageType integer
---@param exclusive boolean
---@param value number
---@param extname string
---@return ScarModifier
function Modifier_Create(applicationType, modtype, usageType, exclusive, value, extname) end

---Todo. 
---@param modifier ScarModifier
function Modifier_Destroy(modifier) end

---Enable / Disable a particular modifier FX for all items in a
--- group 
---Pass iModifier_EGroup_FX_Enablen the egroup, along with the name
--- of the FX (Entity) Modifier, and whether to disable (0) or
--- enable (1) it
---@param egroup EGroupID
---@param modifierName string
---@param bEnable boolean
function Modifier_EGroup_FX_Enable(egroup, modifierName, bEnable) end

---Enable / Disable a particular modifier FX for a single entity 
---Pass iModifier_EGroup_FX_Enablen the egroup, along with the name
--- of the FX (Entity) Modifier, and whether to disable (0) or
--- enable (1) it
---@param entity EntityID
---@param modifierName string
---@param bEnable boolean
function Modifier_FX_Enable(entity, modifierName, bEnable) end

---Remove an applied modifier. 
---You will need the modID returned from Modifier_ApplyToPlayer etc
---@param modID integer
function Modifier_Remove(modID) end

---Enable / Disable a particular modifier FX for all items in a
--- group 
---Pass in the egroup, along with the name of the FX (Entity)
--- Modifier, and whether to disable (0) or enable (1) it
---@param sgroup SGroupID
---@param modifierName string
---@param bEnable boolean
function Modifier_SGroup_FX_Enable(sgroup, modifierName, bEnable) end

---Add a new objective using an objective table. 
---Example obj_table:
--- obj_table = { title_id = 400009,	short_desc_id = 400010,
--- help_tip_id = 400011 }
---@param obj_table LuaTable
---@param is_primary_obj boolean
function Objective_Add(obj_table, is_primary_obj) end

---Returns true if an objective exists 
---@param title_id integer
---@return boolean
function Objective_Exists(title_id) end

---Returns OS_Incomplete, OS_Complete or OS_Failed 
---@param title_id integer
---@return integer
function Objective_GetState(title_id) end

---Set an objective to OS_Complete or OS_Failed 
---You cannot set an objectives state to OS_Incomplete.  When an
--- ojective is completed or failed, it will trigger a ui event
--- icon.
---@param title_id integer
---@param state integer
function Objective_SetState(title_id, state) end

---Opens up the objectives menu for the player 
function Objective_ShowScreen() end

---Wait for a path to reach a certain keyframe.  Can be used on any
--- type of path anim. 
---@param pathname string
---@param keyframe string
function Path_KeyframeWait(pathname, keyframe) end

---Scar ping at a marker position. use ping_type 'attack' for
--- attack pings otherwise default ping will be used 
---@param markername string
---@param is_looping boolean
---@param ping_type string
---@return PingID
function Ping_Marker(markername, is_looping, ping_type) end

---Scar ping at a position.  Scar ping will show up on terrain and
--- minimap.  use ping_type 'attack' for attack pings otherwise
--- default ping will be used 
---@param pos Position
---@param is_looping boolean
---@param ping_type string
---@return PingID
function Ping_Position(pos, is_looping, ping_type) end

---Use this to stop a looping ping 
---@param pingID PingID
function Ping_Stop(pingID) end

---Retrieve a custom value in current player profile.  Returns an
--- empty string if value does not exist 
---@param varName string
---@return string
function PlayerProfile_GetVar(varName) end

---Save a custom value in current player profile. 
---@param varName string
---@param val string
function PlayerProfile_SetVar(varName, val) end

---Add resource to player, as opposed to just setting it. 
---@param playerId PlayerID
---@param resourceType integer
---@param value number
function Player_AddResource(playerId, resourceType, value) end

---For the given player, get all of the squads gathered into a
--- squadgroup of your naming.   
---Squads will be added to given squad group.  If the given squad
--- group does not exist it will be created.
---@param playerId PlayerID
---@param squadgroupName string
---@return SGroupID
function Player_AddSquadsToSGroup(playerId, squadgroupName) end

---Returns true if ANY of a players squads are in proximity of a
--- marker 
---@param playerID PlayerID
---@param markername string
---@return boolean
function Player_AreSquadsNearMarker(playerID, markername) end

---Returns true if a player can see ALL or ANY items in an egroup 
---@param playerId PlayerID
---@param egroupname string
---@param all boolean
---@return boolean
function Player_CanSeeEGroup(playerId, egroupname, all) end

---Returns true if a player can see a given entity 
---@param player PlayerID
---@param entity EntityID
---@return boolean
function Player_CanSeeEntity(player, entity) end

---Returns true if a player can see a given position. 
---@param player PlayerID
---@param pos Position
---@return boolean
function Player_CanSeePosition(player, pos) end

---Returns true if a player can see ALL or ANY items in an sgroup 
---@param playerId PlayerID
---@param sgroupname string
---@param all boolean
---@return boolean
function Player_CanSeeSGroup(playerId, sgroupname, all) end

---Returns true if a player can see ALL or ANY troopers in a given
--- squad 
---Set all to true to check that ALL troopers are visible or set to
--- false to check for ANY.
---@param player PlayerID
---@param squad SquadID
---@param all boolean
---@return boolean
function Player_CanSeeSquad(player, squad, all) end

---Returns a player given a player id from the ME.  NOTE: this is a
--- temp function, and will be replaced with Player_FromName 
---@param id integer
---@return PlayerID
function Player_FromId(id) end

---Get a player using their name from the Mission Editor.  NOT DONE
--- YET 
---@param name string
---@return PlayerID
function Player_FromName(name) end

---Add all players entities that are in proximity to a marker into
--- a squad group.  Group will be created if it doesnt already
--- exist. 
---The proximity of a marker is set in the mission editor.
---@param playerId PlayerID
---@param egroupName string
---@param markerName string
function Player_GetAllEntitiesNearMarker(playerId, egroupName, markerName) end

---Add all players entities that are in proximity to a position
--- into an entity group.  Group will be created if it doesnt
--- already exist. 
---
---@param p PlayerID
---@param egroupName string
---@param pos Position
---@param proximity number
function Player_GetAllEntitiesNearPos(p, egroupName, pos, proximity) end

---Add all players squads that are in proximity to a marker into a
--- squad group.  Group will be created if it doesnt already exist.
--- 
---The proximity of a marker is set in the mission editor.
---@param playerId PlayerID
---@param squadgroupName string
---@param markerName string
function Player_GetAllSquadsNearMarker(playerId, squadgroupName, markerName) end

---Add all players squads that are in proximity to a point into a
--- squad group.  Group will be created if it doesnt already exist.
--- 
---
---@param p PlayerID
---@param sgroupName string
---@param pos Position
---@param proximity number
function Player_GetAllSquadsNearPos(p, sgroupName, pos, proximity) end

---Returns the number of buidlings/sqauds with blueprints owned by
--- a player 
---blueprint name can be a squad blueprint OR an entity blueprint
---@param player PlayerID
---@param blueprintname string
---@return integer
function Player_GetBlueprintCount(player, blueprintname) end

---Returns the total number of buildings owned by this player. 
---@param playerId PlayerID
---@return integer
function Player_GetBuildingsCount(playerId) end

---Returns the total number of buildings owned by this player (with
--- exclusions). 
---@param playerId PlayerID
---@param exceptions table
---@return integer
function Player_GetBuildingsCountExcept(playerId, exceptions) end

---Returns the number of buildings owned by this player
--- (inclusive). 
---@param playerId PlayerID
---@param building_names table
---@return integer
function Player_GetBuildingsCountOnly(playerId, building_names) end

---Use capType CT_SquadCap to get current squad cap or CT_Support
--- to get current support cap. 
---@param player PlayerID
---@param capType integer
---@return integer
function Player_GetCurrentSquadCap(player, capType) end

---Returns the players UI name. 
---@param player PlayerID
---@return LocString
function Player_GetDisplayName(player) end

---Returns an EntityGroup containing all the players entities. 
--- This function returns a 'global' entity group with the name
--- '__Player%dEntities', where %d is the player ID.  This means
--- that you should never need to destroy it./n However, if you do
--- destroy it, it will be recreated the next time this function is
--- called.
---@param player PlayerID
---@return EGroupID
function Player_GetEntities(player) end

---Returns the number of entities a player currently owns 
---@param player PlayerID
---@return integer
function Player_GetEntityCount(player) end

---Returns the name of an entity a player currently owns 
---@param player PlayerID
---@param index integer
---@return string
function Player_GetEntityName(player, index) end

---Returns the id of the player 
---@param player PlayerID
---@return integer
function Player_GetID(player) end

---Use capType CT_SquadCap to get max squad cap or CT_Support to
--- get max support cap. 
---@param player PlayerID
---@param capType integer
---@return integer
function Player_GetMaxSquadCap(player, capType) end

---Returns the number of strategic objectives this player owns 
---@param p PlayerID
---@return integer
function Player_GetNumStrategicObjectives(p) end

---Returns the number of strategic points with blueprint
--- strategic_point_flag 
---@param p PlayerID
---@return integer
function Player_GetNumStrategicPoints(p) end

---Returns the race index for the given player. 
---@param player PlayerID
---@return integer
function Player_GetRace(player) end

---Returns the ME name of the race for a given player. 
---@param player PlayerID
---@return string
function Player_GetRaceName(player) end

---Returns the relationship between 2 players.  Possible
--- relationships are R_ENEMY, R_ALLY, R_NEUTRAL, R_UNDEFINED. 
---@param player1 PlayerID
---@param player2 PlayerID
---@return integer
function Player_GetRelationship(player1, player2) end

---Returns RS_Locked,RS_Available,RS_Researching or RS_Complete 
--- RS_Locked - Research is restricted or not yet available.
--- RS_Researching - Research is queued up in one of the players
--- buildings.
--- RS_Complete - Research has been granted or completed.
--- RS_Available - Research is available to build in one or more of
--- the players buildings
---@param player PlayerID
---@param researchName string
---@return integer
function Player_GetResearchState(player, researchName) end

---Returns the amount of resources a given player has.  Possible
--- resource types are RT_Requisition, RT_Power, RT_Pop 
---@param player PlayerID
---@param resourceType integer
---@return number
function Player_GetResource(player, resourceType) end

---Returns the number of squads a player currently owns 
---@param player PlayerID
---@return integer
function Player_GetSquadCount(player) end

---Returns a SquadGroup containing all the players units. 
--- This function returns a 'global' squad group with the name
--- '__Player%dSquads', where %d is the player ID.  This means that
--- you should never need to destroy it./n However, if you do
--- destroy it, it will be recreated the next time this function is
--- called.
---@param player PlayerID
---@return SGroupID
function Player_GetSquads(player) end

---Returns the starting position for a given player 
---@param player PlayerID
---@return Position
function Player_GetStartPosition(player) end

---Add all of a players strategic points to a egroup 
---@param player PlayerID
---@param egroup EGroupID
function Player_GetStrategicPoints(player, egroup) end

---Get the team a player is on 
---@param p PlayerID
---@return integer
function Player_GetTeam(p) end

---Returns the current number of units the player has. 
---@param player PlayerID
---@return integer
function Player_GetUnitCount(player) end

---Grants and applies a research item to a given player 
---@param player PlayerID
---@param researchName string
function Player_GrantResearch(player, researchName) end

---Returns true if this player owns any of the indicated building
--- type and they are fully constructed 
---@param playerId PlayerID
---@param building_blueprint Blueprint
---@return integer
function Player_HasBuildingType(playerId, building_blueprint) end

---Returns true if this player owns any buildings. (with
--- exclusions). 
---@param playerId PlayerID
---@param exceptions table
---@return integer
function Player_HasBuildingsExcept(playerId, exceptions) end

---Returns true if this player owns any squads (with exclusions). 
---@param playerId PlayerID
---@param exceptions table
---@return integer
function Player_HasSquadsExcept(playerId, exceptions) end

---Increases the ork pop cap using a modifier 
---@param playerId PlayerID
---@param maxcap integer
function Player_IncreasePopCap(playerId, maxcap) end

---Returns true if player is still alive and false if player is
--- dead.  Will error if playerIdx is an invalid index. 
---@param player PlayerID
---@return boolean
function Player_IsAlive(player) end

---Returns true if the players are allied and false if they are
--- not. 
---@param playerId1 PlayerID
---@param playerId2 PlayerID
---@return boolean
function Player_IsAllied(playerId1, playerId2) end

---A player is totally annihilated when they have no 'killable'
--- squads or entities. 
---A killable squad or entity is any squad or entity with health
--- (captured strategic points do not count)
---@param player PlayerID
---@return boolean
function Player_IsAnnihilated(player) end

---Kill a player and reveals their FOW.  Will error if playerIdx is
--- an invalid index. 
---@param player PlayerID
function Player_Kill(player) end

---Returns true if a given player exculsivley owns all items in a
--- group 
---@param playerID PlayerID
---@param egroupID EGroupID
---@return boolean
function Player_OwnsEGroup(playerID, egroupID) end

---Returns true if a given player exculsivley owns all items in a
--- group 
---@param playerID PlayerID
---@param sgroupID SGroupID
---@return boolean
function Player_OwnsSGroup(playerID, sgroupID) end

---Restrict an addon for a player 
---@param player PlayerID
---@param name string
function Player_RestrictAddOn(player, name) end

---Restrict a list of names. 
---list should contain an array of strings to restrict.
---@param id PlayerID
---@param list StringTable
function Player_RestrictAddOnList(id, list) end

---Restricts a building for a player. 
---@param player PlayerID
---@param name string
function Player_RestrictBuilding(player, name) end

---Restrict a list of names. 
---list should contain an array of strings to restrict.
---@param id PlayerID
---@param list StringTable
function Player_RestrictBuildingList(id, list) end

---Restricts a research item for a player 
---@param player PlayerID
---@param name string
function Player_RestrictResearch(player, name) end

---Restrict a list of names. 
---list should contain an array of strings to restrict.
---@param id PlayerID
---@param list StringTable
function Player_RestrictResearchList(id, list) end

---Restricts a squad for a player. 
---@param player PlayerID
---@param name string
function Player_RestrictSquad(player, name) end

---Restrict a weapon for a player. 
---@param player PlayerID
---@param name string
function Player_RestrictWeapon(player, name) end

---Sets the maximum squad cap for the player. 
---Note that any modifiers that adjust the max squad cap will be
--- applied on top of this.
---@param playerId PlayerID
---@param maxcap integer
function Player_SetMaxSquadCap(playerId, maxcap) end

---Sets the maximum support cap for the player. 
---Note that any modifiers that adjust the max support cap will be
--- applied on top of this.
---@param playerId PlayerID
---@param maxcap integer
function Player_SetMaxSupportCap(playerId, maxcap) end

---Set the resource amount for a given player.  Ignores income cap
--- and resource sharing. 
---Possible resource types are RT_Requisition, RT_Power, RT_Pop.
---
---@param player PlayerID
---@param resourceType integer
---@param amt number
function Player_SetResource(player, resourceType, amt) end

---Sets the curretn squad cap for the player. 
---Note that any modifiers that adjust the current squad cap will
--- be applied on top of this.  Also note that the current squad
--- cap cannot go higher than the max squad cap.
---@param playerId PlayerID
---@param maxcap integer
function Player_SetSquadCap(playerId, maxcap) end

---Sets the current support cap for a player. 
---Note that any modifiers that adjust the current support cap will
--- be applied on top of this.  Also note, the current support cap
--- cannot go higher than the max support cap.
---@param playerId PlayerID
---@param cap integer
function Player_SetSupportCap(playerId, cap) end

---Put a player in a team. Use TEAM_NEUTRAL as the team_id to set
--- the player as neutral 
---@param p PlayerID
---@param team_id integer
function Player_SetTeam(p, team_id) end

---Unrestricts an addon for a player 
---@param player PlayerID
---@param name string
function Player_UnRestrictAddOn(player, name) end

---Unrestricts a building for a player. 
---@param player PlayerID
---@param name string
function Player_UnRestrictBuilding(player, name) end

---Unrestricts a research item for a player 
---@param player PlayerID
---@param name string
function Player_UnRestrictResearch(player, name) end

---Unrestricts a squad for a player. 
---@param player PlayerID
---@param name string
function Player_UnRestrictSquad(player, name) end

---Unrestricts a weapon for a player. 
---@param player PlayerID
---@param name string
function Player_UnRestrictWeapon(player, name) end

---Updates the peristent EGroup that contains all entities for this
--- player 
---@param player PlayerID
---@return EGroupID
function Player_UpdateAllEntities(player) end

---Updates the peristent SGroup that contains all squads for this
--- player 
---@param player PlayerID
---@return SGroupID
function Player_UpdateAllSquads(player) end

---Adds a add-on item to a production list table. 
---@param addonname string
---@param list LuaTable
function Prod_AddAddOnToList(addonname, list) end

---Adds a research item to a production list table. 
---@param researchname string
---@param list LuaTable
function Prod_AddResearchToList(researchname, list) end

---Adds a squad item to a production list table. 
---@param blueprintname string
---@param list LuaTable
function Prod_AddSquadToList(blueprintname, list) end

---Returns true if a production list contains the given add-on item
--- 
---@param addonname string
---@param list LuaTable
---@return boolean
function Prod_DoesListContainAddOn(addonname, list) end

---Returns true if a production list contains the given research
--- item 
---@param researchname string
---@param list LuaTable
---@return boolean
function Prod_DoesListContainResearch(researchname, list) end

---Returns true if a production list contains the given squad item 
---@param blueprintname string
---@param list LuaTable
---@return boolean
function Prod_DoesListContainSquad(blueprintname, list) end

---Gets everything a given player currently has in production. 
---See Prod_GetProductionList for more info.
---@param playerId PlayerID
---@param list LuaTable
function Prod_GetPlayerProductionList(playerId, list) end

---Checks all entities to see if they're production-capable and if
--- so, adds their queued items to a Production Table. 
--- Use Prod_DoesListContainSquad etc to query the resulting
--- production table.
---
--- Example:
---     local ProductionTable = { }
---     Prod_GetProductionList( "GRP_ANYENTITYGROUP",
--- ProductionTable )
---     if(Prod_DoesListContainSquad("chaos_marine_squad",
--- ProductionTable) then
---         -- There is a building in GRP_ANYENTITYGROUP that is
--- currently producing a space marine Squad_CanSeeEntity
---     end
---@param egroupname string
---@param list LuaTable
function Prod_GetProductionList(egroupname, list) end

---Returns true the players henchmen are currently constructing a
--- building with given blueprintname. 
--- Note: If you find that you are calling this function for tons
--- of different buildings, we should change this function to take
--- a constuction list (similar to the way production lists are
--- handled)
---@param playerId PlaierID
---@param buildingname string
---@return boolean
function Prod_IsPlayerConstructing(playerId, buildingname) end

---Returns true if the player is producing all or any items in a
--- production table.  See Prod_IsProducing for more info. 
---@param playerId PlayerID
---@param list LuaTable
---@param all boolean
---@return boolean
function Prod_IsPlayerProducing(playerId, list, all) end

---Returns true if buildings in a entity group are producing ALL or
--- ANY items in a given production table. 
--- Use Prod_AddSquadToList etc to add entries to the given
--- production list.
---
--- Example:
---     local ProductionTable = { }
---     Prod_AddSquadToList( "chaos_squad_cultist", list )
---     Prod_AddSquadToList( "chaos_squad_cultist", list )
---     Prod_AddSquadToList( "chaos_squad_defiler", list )
---     if( Prod_IsProducing( "GRP_ANYENTITYGROUP", ProcutionTable,
--- true ) then
---         -- Of all the buildings in GRP_ANYENTITYGROUP, they are
--- producing 2 cultists and a defiler.
---     end
---@param egroupname string
---@param list LuaTable
---@param all boolean
---@return boolean
function Prod_IsProducing(egroupname, list, all) end

---Returns true if ALL entities in a group are in proximity to a
--- marker. 
--- The proximity of a marker can only be set in the mission
--- editor.
---@param groupname string
---@param markername string
---@return boolean
function Prox_AllEntitiesNearMarker(groupname, markername) end

---Returns true if ALL squads in a group are in proximity to a
--- marker. 
--- The proximity of a marker can only be set in the mission
--- editor.
---@param groupname string
---@param markername string
---@return boolean
function Prox_AllSquadsNearMarker(groupname, markername) end

---Returns true if ANY entity from a group are in proximity to a
--- marker. 
--- The proximity of a marker can only be set in the mission
--- editor.
---@param groupname string
---@param markername string
---@return boolean
function Prox_AnyEntityNearMarker(groupname, markername) end

---Returns true if ANY squads from a group are in proximity to a
--- marker. 
--- The proximity of a marker can only be set in the mission
--- editor.
---@param groupname string
---@param markername string
---@return boolean
function Prox_AnySquadNearMarker(groupname, markername) end

---Returns the distance between two entity groups. use checktype
--- PROX_SHORTEST, PROX_LONGEST, or PROX_CENTER. 
--- If check is PROX_SHORTEST this will return the shortest
--- distance between the two groups.
--- If check is PROX_LONGEST this will return the longest distance
--- between the two groups.
--- If check is PROX_CENTER this will return the distance between
--- the two groups centers.
---
---@param groupname1 string
---@param groupname2 string
---@param checktype ProxType
---@return number
function Prox_EGroupEGroup(groupname1, groupname2, checktype) end

---Returns the distance between an entity group and a squad group. 
--- use checktype PROX_SHORTEST, PROX_LONGEST, or PROX_CENTER. 
--- If check is PROX_SHORTEST this will return the shortest
--- distance between the two groups.
--- If check is PROX_LONGEST this will return the longest distance
--- between the two groups.
--- If check is PROX_CENTER this will return the distance between
--- the two groups centers.
---
---@param egroupname string
---@param sgroupname string
---@param checktype ProxType
---@return number
function Prox_EGroupSGroup(egroupname, sgroupname, checktype) end

---Checks if ALL or ANY entities are in proximity of a given entity
--- group. 
---Set "all" to true to check that ALL entities are in proximity,
--- or set "all" to false to check for ANY.
---@param egroupname1 string
---@param egroupname2 string
---@param proximity number
---@param all boolean
---@return boolean
function Prox_EntitiesInProximityOfEntities(egroupname1, egroupname2, proximity, all) end

---Returns the distance between a marker and an entity group.  use
--- checktype PROX_SHORTEST, PROX_LONGEST, or PROX_CENTER. 
--- If check is PROX_SHORTEST this will return the shortest
--- distance between the two groups.
--- If check is PROX_LONGEST this will return the longest distance
--- between the two groups.
--- If check is PROX_CENTER this will return the distance between
--- the two groups centers.
---
---@param markername string
---@param egroupname string
---@param checktype ProxType
---@return number
function Prox_MarkerEGroup(markername, egroupname, checktype) end

---Returns the distance between a marker and a squad group.  use
--- checktype PROX_SHORTEST, PROX_LONGEST, or PROX_CENTER. 
--- If check is PROX_SHORTEST this will return the shortest
--- distance between the two groups.
--- If check is PROX_LONGEST this will return the longest distance
--- between the two groups.
--- If check is PROX_CENTER this will return the distance between
--- the two groups centers.
---
---@param markername string
---@param sgroupname string
---@param checktype ProxType
---@return number
function Prox_MarkerSGroup(markername, sgroupname, checktype) end

---Checks if ALL or ANY players squads are in proximity of a given
--- entity group. 
---Set "all" to true to check that ALL entities are in proximity,
--- or set "all" to false to check for ANY.
---@param playerId PlayerID
---@param egroupname string
---@param proximity number
---@param all boolean
---@return boolean
function Prox_PlayerEntitiesInProximityOfEntities(playerId, egroupname, proximity, all) end

---Checks if ALL or ANY players entities are in proximity of a
--- given squad group. 
---Set "all" to true to check that ALL entities are in proximity,
--- or set "all" to false to check for ANY.
---@param playerId1 PlayerID
---@param playerId2 PlayerID
---@param proximity number
---@param all boolean
---@return boolean
function Prox_PlayerEntitiesInProximityOfPlayerSquads(playerId1, playerId2, proximity, all) end

---Checks if ALL or ANY players entities are in proximity of a
--- given squad group. 
---Set "all" to true to check that ALL entities are in proximity,
--- or set "all" to false to check for ANY.
---@param playerId PlayerID
---@param sgroupname string
---@param proximity number
---@param all boolean
---@return boolean
function Prox_PlayerEntitiesInProximityOfSquads(playerId, sgroupname, proximity, all) end

---Checks if ALL or ANY players squads are in proximity of a given
--- entity group. 
---Set "all" to true to check that ALL squads are in proximity, or
--- set "all" to false to check for ANY.
---@param playerId PlayerID
---@param sgroupname string
---@param proximity number
---@param all boolean
---@return boolean
function Prox_PlayerSquadsInProximityOfEntities(playerId, sgroupname, proximity, all) end

---Checks if ALL or ANY players squads are in proximity of a given
--- players entities. 
---Set "all" to true to check that ALL squads are in proximity, or
--- set "all" to false to check for ANY.
---@param playerId1 PlayerID
---@param playerId2 PlayerID
---@param proximity number
---@param all boolean
---@return boolean
function Prox_PlayerSquadsInProximityOfPlayerEntities(playerId1, playerId2, proximity, all) end

---Checks if ALL or ANY players squads are in proximity of a given
--- players squads. 
---Set "all" to true to check that ALL squads are in proximity, or
--- set "all" to false to check for ANY.
---@param playerId1 PlayerID
---@param playerId2 PlayerID
---@param proximity number
---@param all boolean
---@return boolean
function Prox_PlayerSquadsInProximityOfPlayerSquads(playerId1, playerId2, proximity, all) end

---Checks if ALL or ANY players squads are in proximity of a given
--- squad group. 
---Set "all" to true to check that ALL squads are in proximity, or
--- set "all" to false to check for ANY.
---@param playerId PlayerID
---@param sgroupname string
---@param proximity number
---@param all boolean
---@return boolean
function Prox_PlayerSquadsInProximityOfSquads(playerId, sgroupname, proximity, all) end

---Returns the distance between two squad groups. use checktype
--- PROX_SHORTEST, PROX_LONGEST, or PROX_CENTER. 
--- If check is PROX_SHORTEST this will return the shortest
--- distance between the two groups.
--- If check is PROX_LONGEST this will return the longest distance
--- between the two groups.
--- If check is PROX_CENTER this will return the distance between
--- the two groups centers.
---
---@param groupname1 string
---@param groupname2 string
---@param checktype ProxType
---@return number
function Prox_SGroupSGroup(groupname1, groupname2, checktype) end

---Checks if ALL or ANY squads are in proximity of a given entity
--- group. 
---Set "all" to true to check that ALL squads are in proximity, or
--- set "all" to false to check for ANY.
---@param sgroupname string
---@param egroupname string
---@param proximity number
---@param all boolean
---@return boolean
function Prox_SquadsInProximityOfEntities(sgroupname, egroupname, proximity, all) end

---Checks if ALL or ANY squads are in proximity of a given squad
--- group. 
---Set "all" to true to check that ALL squads are in proximity, or
--- set "all" to false to check for ANY.
---@param sgroupname1 string
---@param sgroupname2 string
---@param proximity number
---@param all boolean
---@return boolean
function Prox_SquadsInProximityOfSquads(sgroupname1, sgroupname2, proximity, all) end

---Applies restrictions for space marines.  You should call this at
--- the begining of your mission for Player 1. 
---@param playerId PlayerID
---@param level integer
function Restrict_SpaceMarines(playerId, level) end

---Add a rule to be executed every frame 
---@param f function
function Rule_Add(f) end

---Add a rule to be executed at every 'interval' seconds 
---@param f function
---@param interval number
function Rule_AddInterval(f, interval) end

---Add a rule to be executed 'calls' times, at every 'interval'
--- seconds 
---@param f function
---@param interval number
---@param calls integer
function Rule_AddIntervalEx(f, interval, calls) end

---Add a rule to be executed once, after 'interval' seconds 
---@param f function
---@param interval number
function Rule_AddOneShot(f, interval) end

---Test if a rule is currently active 
---@param f function
---@return boolean
function Rule_Exists(f) end

---Remove a currently active rule 
---@param f function
function Rule_Remove(f) end

---Kills all rules. 
function Rule_RemoveAll() end

---Returns true if a squad blueprint exists with the given name. 
---@param name string
---@return boolean
function SBP_Exists(name) end

---Adds an squadron to the end of a group if the group doesnt
--- already have it. 
---@param group SGroupID
---@param squadron SquadID
function SGroup_Add(group, squadron) end

---Same as EGroup_AddGroup.  Note: You cannot mix squad groups and
--- entity groups. 
---@param group SGroupID
---@param grouptoadd SGroupID
function SGroup_AddGroup(group, grouptoadd) end

---Adds a leader to all squads in a group that can take a leader. 
---This function will bypass all cost and queue prereqs
---@param sgroupname string
function SGroup_AddLeaders(sgroupname) end

---Returns true if ALL or ANY squads in a group can see ALL or ANY
--- squads in a target sgroup. 
---@param sgroupname string
---@param targetegroupname string
---@param all boolean
---@return boolean
function SGroup_CanSeeSGroup(sgroupname, targetegroupname, all) end

---Returns true if ALL or ANY items in a group have the given
--- active command 
---See Squad_GetActiveCommand for valid command types
---@param groupname string
---@param cmd integer
---@param all boolean
---@return boolean
function SGroup_CheckActiveCommand(groupname, cmd, all) end

---Removes all entities from a group. 
---@param sgroup SGroupID
function SGroup_Clear(sgroup) end

---Returns true if the contents of the two groups are equal. Order
--- of the entities does not matter. 
---@param group1 SGroupID
---@param group2 SGroupID
---@return boolean
function SGroup_Compare(group1, group2) end

---Check if a group contains ALL or ANY of the items in a blueprint
--- table. 
--- This function will validate all blueprint names.
--- Set all to true to check for all blueprints in the
--- blueprintTable or set to false to check for any.
--- A blueprint table containing two marines and a bloodthirster
--- looks like so:
--- BlueprintTable = {
---     { name = "chaos_marine_squad", count = 2 }
---     { name = "chaos_squad_bloodthirster", count = 1 }
--- }
---
--- You could also call Util_MakeBlueprintTable:
--- BlueprintTable = Util_MakeBlueprintTable( "chaos_marine_squad",
--- "chaos_marine_squad", "chaos_squad_bloodthirster")
---
--- This function will take care of setting up the table for you.
---@param sgroup SGroupID
---@param blueprintTable LuaConfig::RefTable
---@param all boolean
---@return boolean
function SGroup_ContainsBlueprints(sgroup, blueprintTable, all) end

---Returns true if an sgroup contains a given squad 
---@param sgroup SGroupID
---@param squad SquadID
---@return boolean
function SGroup_ContainsSquad(sgroup, squad) end

---Check if all or any squads in a group contain (at least) the
--- given number of weapon blueprints 
---@param groupname string
---@param weaponname string
---@param count integer
---@param all boolean
---@return boolean
function SGroup_ContainsWeaponBlueprints(groupname, weaponname, count, all) end

---Returns the total number of spawned and despawned squads in a
--- group. 
---@param sgroup SGroupID
---@return integer
function SGroup_Count(sgroup) end

---Returns the number of despawned squads in a group. 
---@param sgroup SGroupID
---@return integer
function SGroup_CountDeSpawned(sgroup) end

---Returns the number of spawned squads in a group. 
---@param sgroup SGroupID
---@return integer
function SGroup_CountSpawned(sgroup) end

---Returns a new squadron group with the given name.   
---  Squad groups are used for all units and vehicles.  You can
--- issue orders such as move and attack to an entire squad group. 
--- If you want a group for buildings or objects such as trees, use
--- an EntityGroup instead.
---  Note that you are allowed to have unamed squadgroups or
--- squadgroups with the samename.  Be careful not to create 
--- duplicates of your squad group when you dont intend to.  To be
--- safe, you can check if the SquadGroup you want to create 
--- exists using SGroup_Exists( )
---@param name string
---@return SGroupID
function SGroup_Create(name) end

---Find a squad group from name.  Creates a new one with given name
--- if it doesnt exist. 
---@param name string
---@return SGroupID
function SGroup_CreateIfNotFound(name) end

---Despawn all spawned squads in a group. 
---@param groupname string
function SGroup_DeSpawn(groupname) end

---Manually destroy a group that you dont need anymore. 
---@param egroup SGroupID
function SGroup_Destroy(egroup) end

---Destroys all items in a group that are in proximity to a given
--- marker. 
---@param groupname string
---@param markername string
function SGroup_DestroyAllInMarker(groupname, markername) end

---Destroys all spawned and despawned squads in a group. 
--- Be careful not to confuse this with SGroup_Destroy which
--- destroys the group and NOT the squads it contains.  This
--- function will destroy spawned and despawned items in a group
---@param groupname string
function SGroup_DestroyAllSquads(groupname) end

---Returns true if the squad group with the given name exists 
---@param name string
---@return boolean
function SGroup_Exists(name) end

---Makes two SGroups face each other 
---@param sgroupname1 string
---@param sgroupname2 string
function SGroup_FaceEachOther(sgroupname1, sgroupname2) end

---Makes a SGroup face a marker. 
---@param sgroupname string
---@param markername string
function SGroup_FaceMarker(sgroupname, markername) end

---Works like Squad_FacePosition.  All Squads will face the same
--- direction, with the squad the closest to the center determining
--- the direction. 
---This function works on spawned squads only.
---@param sgroup SGroupID
---@param pos Position
function SGroup_FacePosition(sgroup, pos) end

---Makes the first SGroup face the second SGroup 
---@param sgroupname1 string
---@param sgroupname2 string
function SGroup_FaceSGroup(sgroupname1, sgroupname2) end

---Call a lua function for each item in a group. Function will
--- recieve (groupid, itemindex, itemid) and should return true to
--- break or false to continue. 
--- function Rule_Test( )
---     local DespawnSquad = function( sgroupid, itemindex, squadID
--- )
---         Squad_Despawn( squadID )
---     end
---
---     SGroup_ForEach( SGroup_FromName("sg_Squads"), DespawnSquad
--- )
--- end
---
--- This functions iterates over spawned squads only.
---@param sgroup SGroupID
---@param f LuaBinding::StackVarFunction
---@return boolean
function SGroup_ForEach(sgroup, f) end

---Call a lua function for each item in a group. Function will
--- recieve (groupid, itemindex, itemid) and should return a bool.
--- 
--- This is used for checking if ALL or ANY items in a group match
--- a given predicate. (eg. Are all items in a group choas
--- marines)
--- This functions iterates over spawned squads only.
---@param sgroup SGroupID
---@param all boolean
---@param f LuaBinding::StackVarFunction
---@return boolean
function SGroup_ForEachAllOrAny(sgroup, all, f) end

---Same as SGroup_ForEachAllOrAny except you have a choice to
--- iterate over spawned squads, despawned squads, or both. 
---@param sgroup SGroupID
---@param all boolean
---@param f LuaBinding::StackVarFunction
---@param spawned boolean
---@param despawned boolean
---@return boolean
function SGroup_ForEachAllOrAnyEx(sgroup, all, f, spawned, despawned) end

---Same as SGroup_ForEach except you have a choice to iterate over
--- spawned squads, despawned squads, or both. 
---@param sgroup SGroupID
---@param f LuaBinding::StackVarFunction
---@param spawned boolean
---@param despawned boolean
---@return boolean
function SGroup_ForEachEx(sgroup, f, spawned, despawned) end

---Find an squadron group with a given name. 
---@param name string
---@return SGroupID
function SGroup_FromName(name) end

---Returns the average health of all units in a squad group as a
--- weighted percent [0.0, 1.0]. 
---Example: A group of squads with health 20/100 and 100/1000 would
--- return 120/1100 ( approx 11 % )
---@param groupname string
---@return number
function SGroup_GetAvgHealth(groupname) end

---Returns the average loadout of all squads in a group as a
--- percent [0.0, 1.0]. 
---Example: A group of squads with loadouts of 4/8 and 1/1 would
--- return 0.75
---@param groupname string
---@return number
function SGroup_GetAvgLoadout(groupname) end

---Returns the average morale of all units in a squad group as a
--- weighted percent [0.0, 1.0]. 
---Squads that are not affected by morale always have morale of
--- 1.0.  These squads will count towards your  total average
---@param groupname string
---@return number
function SGroup_GetAvgMorale(groupname) end

---Returns the despawned squad at a certain position in the group.
--- 
---  This is a one-based index (use index 1 to get the first squad
--- in the group.)    It is an error if index  SGroup_GetDeSpawnedC
---ount().
---@param group SGroupID
---@param index integer
---@return SquadID
function SGroup_GetDeSpawnedSquadAt(group, index) end

---Check invulnerablity state for ALL or ANY squads in a squad
--- group. 
--- Set all param to true to check for ALL or set to false to check
--- for ANY.
---@param groupname string
---@param all boolean
---@return boolean
function SGroup_GetHealthInvulnerable(groupname, all) end

---Check morale invulnerablity state for ALL or ANY squads in a
--- squad group. 
--- Set all param to true to check for ALL or set to false to check
--- for ANY.
---@param groupname string
---@param all boolean
---@return boolean
function SGroup_GetMoraleInvulnerable(groupname, all) end

---Returns the name of a given squad group. 
---@param sgroup SGroupID
---@return string
function SGroup_GetName(sgroup) end

---Returns the center position of a squad group. 
---Despawned squads are ignored.
---@param group SGroupID
---@return Position
function SGroup_GetPosition(group) end

---Returns the spawned squad at a certain position in the group. 
---  This is a one-based index (use index 1 to get the first squad
--- in the group.)    It is an error if index  SGroup_GetSpawnedCou
---nt().
---@param group SGroupID
---@param index integer
---@return SquadID
function SGroup_GetSpawnedSquadAt(group, index) end

---Returns true if the sgroup has ANY squads of type blueprint have
--- received reinforcements  
---@param sgroupname string
---@param squadblueprint string
---@return boolean
function SGroup_HasAnyReinforced(sgroupname, squadblueprint) end

---Check if ALL or ANY squads in a sgroup have a leader 
--- Set all param to true to check for ALL or set to false to check
--- for ANY.
---@param groupname string
---@param all boolean
---@return boolean
function SGroup_HasLeader(groupname, all) end

---Same as EGroup_Intersection. Note: You cannot mix squad groups
--- and entity groups. 
---@param group SGroupID
---@param grouptointersect SGroupID
function SGroup_Intersection(group, grouptointersect) end

---Returns true if a named squad group is empty 
---@param name string
---@return boolean
function SGroup_IsEmpty(name) end

---Returns true if ALL or ANY squads are in cover. 
---@param sgroupname string
---@param all boolean
---@return boolean
function SGroup_IsInCover(sgroupname, all) end

---Returns true if ALL or ANY squads in a group are infiltrated 
---@param sgroupname string
---@param all boolean
---@return boolean
function SGroup_IsInfiltrated(sgroupname, all) end

---Returns true if ALL or ANY items in a group have broken morale 
---@param groupname string
---@param all boolean
---@return boolean
function SGroup_IsMoraleBroken(groupname, all) end

---Returns true if ANY entities in ANY squads of a group are on
--- currently screen 
---IMPORTANT: This function does not check the FOW.  It will return
--- true  even if the item is not revealed in FOW
---@param groupname string
---@return boolean
function SGroup_IsOnScreen(groupname) end

---Return true if all items in a group are selected 
---@param groupname string
---@return boolean
function SGroup_IsSelected(groupname) end

---Returns true if ALL or ANY squads are under attack. 
---@param sgroupname string
---@param all boolean
---@return boolean
function SGroup_IsUnderAttack(sgroupname, all) end

---Returns true if ALL or ANY squads are under attack. 
---@param sgroupname string
---@param all boolean
---@param player PlayerID
---@return boolean
function SGroup_IsUnderAttackByPlayer(sgroupname, all, player) end

---Respawn all despawned squads in a group. 
---@param groupname string
function SGroup_ReSpawn(groupname) end

---Removes an squadron from a group. 
---@param group SGroupID
---@param squadron SquadID
function SGroup_Remove(group, squadron) end

---Sets the health of each squad in a squad group to a given
--- percent [0.0, 1.0]. 
---@param groupname string
---@param healthPercent number
---@return number
function SGroup_SetAvgHealth(groupname, healthPercent) end

---Sets the moral of each squad in a squad group to a given percent
--- [0.0, 1.0]. 
---@param groupname string
---@param moralePercent number
---@return number
function SGroup_SetAvgMorale(groupname, moralePercent) end

---Enable/Disable invulnerablity for an entire squad group. 
---IMPORTANT: You must disable invulnerability for everytime you
--- enable it!
---@param groupname string
---@param enabled boolean
function SGroup_SetHealthInvulnerable(groupname, enabled) end

---Enable/Disable morale invulnerablity for an entire squad group.
--- 
---IMPORTANT: You must disable invulnerability for everytime you
--- enable it!
---@param groupname string
---@param enabled boolean
function SGroup_SetMoraleInvulnerable(groupname, enabled) end

---Changes the player owner of spawned and despawned squads in an
--- SGroup. 
---@param groupname string
---@param owner PlayerID
function SGroup_SetPlayerOwner(groupname, owner) end

---Call through a CTRL and use a wait to wait until a squad is a
--- certain size. 
---Use countType: CT_Equal, CT_LessThanEqual, CT_GreaterThanEqual. 
--- Note: Function must be called through a CTRL.
---@param sgroup SGroupID
---@param count integer
---@param countType integer
function SGroup_WaitForCount(sgroup, count, countType) end

---Returns true if any of the victims were attacked by any of the
--- attackers in the last one second. 
---@param victims SGroupID
---@param attackers SGroupID
---@return boolean
function SGroup_WasAttacked(victims, attackers) end

---Removes the attached squads from the path and puts them back
--- into pathfinding. 
---@param sanimname string
function SPath_Finish(sanimname) end

---Moves a squad along a path. 
---The squad will move along the terrain regardless of the height
--- of the path. The squad will also be removed from pathfinding
--- while traversing the path.
---
--- This function can be called through a CTRL object and waited
--- on.
---@param sanimname string
---@param fromkeyframe string
---@param tokeyframe string
---@param sgroup SGroupID
function SPath_Start(sanimname, fromkeyframe, tokeyframe, sgroup) end

---Starts a squad path with a speed override. 
---This function works just like SPath_Start.  The speed override
--- use this speed for all points. Set speed to zero to use
--- default.
---
--- Setting autoFinsih to true will automagically call SPath_Finish
--- at the end of the path.
---@param sanimname string
---@param fromkeyframe string
---@param tokeyframe string
---@param sgroup SGroupID
---@param speed number
---@param autoFinish boolean
function SPath_StartEx(sanimname, fromkeyframe, tokeyframe, sgroup, speed, autoFinish) end

---Register an init function with the scar system. 
--- This init function will be called when scar is started up.
--- **Important: Make sure you do not register two functions with
--- the same name; the init function names should be unique.
---@param f function
function Scar_AddInit(f) end

---Returns true if an init function exists 
---@param f function
---@return boolean
function Scar_InitExists(f) end

---Unregister an init function that was registered from
--- Scar_AddInit 
---@param f function
function Scar_RemoveInit(f) end

---Set the Force Commanders weapon to be Power Sword 
---This function should be used in any mission where the Force
--- Commander shouldnt have a Daemon hamemr.
---@param playerId PlayerID
function SetCommanderPowerSword(playerId) end

---Set the Inquisitors weapon to be Power Sword 
---This function should be used in any mission where the Toth
--- (Inquisitor) shouldnt have a Daemon hamemr.
---@param playerId PlayerID
function SetInquisitorPowerSword(playerId) end

---Initializes the all of the players settings in one function and
--- returns the playerID. 
--- In most cases you will call this function from OnGameSetup and
--- store the returned playerId in  a global variable.  The player
--- index should be in the range [0 , (numPlayers-1)].
---
--- Valid player races are the shortnames of the race attrib
--- files.
---
--- Example:
---	function OnGameSetup( )
---	    g_player1 = Setup_Player(0, "PlayerDisplayName",
--- "ork_race", 1)
---     -- g_player2 and g_player3 will be on the same team
--- (allies)
---     g_player2 = Setup_Player(1, "$20000", "space_marine_race",
--- 2)
---     g_player3 = Setup_Player(2, "$20001", "eldar_race", 2)
---	end
---@param playerIndex integer
---@param playerName string
---@param playerRace string
---@param team integer
---@return PlayerID
function Setup_Player(playerIndex, playerName, playerRace, team) end

---Set the UI name of a given player.  Name can be a localized
--- string "$10020" or a normal string "PlayerName". 
---@param player PlayerID
---@param name LocString
function Setup_SetPlayerName(player, name) end

---Set the race for a given player.  Use World_GetRaceIndex() to
--- get the race index from the ME name. 
---@param player PlayerID
---@param char const
function Setup_SetPlayerRace(player, char) end

---Plays a sound from the Data:Sound folder.  Use the path relative
--- to Sound folder without the file extension. 
---Returns a soundID you can use to stop the sound etc.
--- Example:
---
---     local id = Sound_Play( "Speech/SpaceMarine/AddOnComplete"
--- )
---     Sound_Stop( id )
---@param name string
---@return integer
function Sound_Play(name) end

---Play specified stinger right now.  
---While a stinger is playing, the music track is muted but keeps
--- playing
---@param track string
function Sound_PlayStinger(track) end

---Add track to the requested playlist channel. 
---Valid channel values: PC_Music or PC_Ambient 
--- Valid tracks are the sound files from the Data:Sound/Music
--- folder, without the file extension.
---@param channel SoundChannel
---@param track string
function Sound_PlaylistAddTrack(channel, track) end

---Removes all tracks from the requested playlist channel. 
---Valid channel values: PC_Music or PC_Ambient
---@param channel SoundChannel
function Sound_PlaylistClear(channel) end

---Mutes playlist channel down to specified volume. 
---Valid channel values: PC_Music or PC_Ambient
---@param channel SoundChannel
function Sound_PlaylistMute(channel) end

---Play specified track right now. 
---Valid channel values: PC_Music or PC_Ambient
---@param channel SoundChannel
---@param track string
function Sound_PlaylistPlayNow(channel, track) end

---Set silence intervals between tracks for the requested playlist
--- channel. the actual interval time will be randomly chosen
--- between min and max, where min is greater of equal to 0, and
--- min is less than of equal to max 
---Valid channel values: PC_Music or PC_Ambient
---@param channel SoundChannel
---@param min number
---@param max number
function Sound_PlaylistSetSilence(channel, min, max) end

---Set how the next track to be played will be chosen, ordered
--- (true) or random (false) 
---Valid channel values: PC_Music or PC_Ambient
---@param channel SoundChannel
---@param sequential boolean
function Sound_PlaylistSetorder(channel, sequential) end

---Starts playing requested playlist channel. 
---Valid channel values: PC_Music or PC_Ambient
---@param channel SoundChannel
function Sound_PlaylistStart(channel) end

---Stops playing requested playlist channel. 
---Valid channel values: PC_Music or PC_Ambient
---@param channel SoundChannel
function Sound_PlaylistStop(channel) end

---Restores muted playlist channel. 
---Valid channel values: PC_Music or PC_Ambient
---@param channel SoundChannel
function Sound_PlaylistUnMute(channel) end

---Changes the volume of a sound that was started using Sound_Play.
---   
---Use the soundID returned from Sound_Play.  Volume should have
--- range [0.0, 1.0] and fadeTime should be greater or equal to
--- zero.
---@param soundID integer
---@param volume number
---@param fadeTime number
function Sound_SetSoundVolume(soundID, volume, fadeTime) end

---Stops a sound that was started using Sound_Play.   
---Use the soundID returned from Sound_Play.
---@param soundID integer
function Sound_Stop(soundID) end

---Add a leader to a squad.  Returns true on success and false on
--- error. 
---This function will return false for squads that do no have
--- reinforce extension.
---@param squad SquadID
---@return boolean
function Squad_AddLeader(squad) end

---Returns true if a given entity is in the squads sight radius and
--- is NOT infiltrated. 
---@param squad SquadID
---@param entity EntityID
---@return boolean
function Squad_CanSeeEntity(squad, entity) end

---Returns true if a target squad is in the squads sight radius and
--- is NOT infiltrated. 
---@param squad SquadID
---@param target SquadID
---@return boolean
function Squad_CanSeeSquad(squad, target) end

---Returns true if this squadron can upgrade with given unit. 
---@param squad SquadID
---@param weaponname string
---@return boolean
function Squad_CanUpgradeWeapon(squad, weaponname) end

---Returns the number of units currently in a squad (spawned AND
--- despawned!!) 
---@param squad SquadID
---@return integer
function Squad_Count(squad) end

---Create a squad and assign it to a player.  
--- This will create a squad of size 'loadoutCount' and of unit
--- type 'trooper_base' (from squad blueprint)
--- Note: if loadoutCount is zero, the min loadout will be used. 
--- loadoutCount will be clipped to the max loadout if it ever
--- exceeds it.
---@param squadblueprint string
---@param player PlayerID
---@param pos Position
---@param loadoutCount integer
---@return SquadID
function Squad_Create(squadblueprint, player, pos, loadoutCount) end

---Despawn the entire squad at its current position. 
---@param squad SquadID
function Squad_DeSpawn(squad) end

---Deep strike a group of DESPAWNED squads to a given location. 
---This function will do nothing if the squad is in SquadStateLoad
--- (loading a transport)
---
--- ebpname is the name of the building with the deepstrike_ext (ie
--- space_marine_orbital_relay)
---
--- you should never assume that this function will deepstrike your
--- units because there may not be enough room!
---@param sgroup SGroupID
---@param ebpname string
---@param pos Position
---@return boolean
function Squad_DeepStrikeToPos(sgroup, ebpname, pos) end

---Remove an squad from the world and destroy it. 
---@param squad SquadID
function Squad_Destroy(squad) end

---Detaches an attached squad and returns the new detached squad. 
---To call this function the squad must have an attach extension
--- and an attached squad.
---@param squad SquadID
---@return SquadID
function Squad_Detach(squad) end

---Returns true if a squad with the given game id exists in the
--- world 
---@param squadGameID integer
---@return boolean
function Squad_Exists(squadGameID) end

---Set the rotation of all troopers in a squad to face the
--- position. 
---The leader of the squad will face the position and all other
--- troops will use the same rotation as their leader.
---
--- This function works on spawned squads only.
---@param squad SquadID
---@param pos Position
function Squad_FacePosition(squad, pos) end

---Get 2 squads to face each other. 
---The leaders of the squad will face each other, and all troopers
--- in the squad will use the same rotation as their leader.
---
--- This function works on spawned squads only.
---@param squad1 SquadID
---@param squad2 SquadID
function Squad_FaceSquad(squad1, squad2) end

---Forces all weapons on all hardpoints to default for all members
--- of the squad 
---@param squad SquadID
function Squad_ForceResetWeapons(squad) end

---Force a given number of troops in a squad to upgrade their
--- weapons regardless of requirments. 
---Returns the number of troopers succesfully upgraded.  Pass in
--- zero to upgrade all troops.
---@param squad SquadID
---@param weaponname string
---@param nUpgrades integer
---@return integer
function Squad_ForceUpgradeWeapons(squad, weaponname, nUpgrades) end

---Returns the active squad command.  Valid squad commands are
--- SQUADSTATEID_Idle, SQUADSTATEID_Move, SQUADSTATEID_Construction
---, SQUADSTATEID_Capture, SQUADSTATEID_Combat, SQUADSTATEID_Melee,
--- SQUADSTATEID_CombatStance, SQUADSTATEID_Patrol 
---@param squad SquadID
---@return integer
function Squad_GetActiveCommand(squad) end

---Returns the name of the attached squad blueprint (from the
--- attribute editor) 
---@param squad SquadID
---@return string
function Squad_GetAttachedBlueprintName(squad) end

---Returns a string containing the base unit name for the current
--- squadron 
---@param squad SquadID
---@return string
function Squad_GetBaseUnitName(squad) end

---Returns the name of the squad blueprint (from the attribute
--- editor) 
---@param squad SquadID
---@return string
function Squad_GetBlueprintName(squad) end

---Find out what type of cover a given trooper in a squad is in. 
---Trooper index is in the range [0..n-1]
--- Returns CT_None if trooper does not have cover extension
--- Returns one of the possible cover types: CT_None, CT_Light,
--- CT_Heavy, CT_Negative, CT_Blocking, CT_Stealth
---@param squad SquadID
---@param trooperIndex integer
---@return integer
function Squad_GetCoverType(squad, trooperIndex) end

---Gets the name of the formation for the given squad 
---If the squad does not have formation, it returns the default
--- formation BLOCK
---@param squad SquadID
---@return string
function Squad_GetFormation(squad) end

---Returns an integer containing the unqiue squad ID for this
--- squad. 
---@param squad SquadID
---@return integer
function Squad_GetGameID(squad) end

---Returns the current health of a squad. 
---The current health of the squad is the total health of each
--- entity in that squad.
---@param squad SquadID
---@return number
function Squad_GetHealth(squad) end

---Returns true if any entities in a squad are invulnerable. 
---@param squad SquadID
---@return boolean
function Squad_GetHealthInvulnerable(squad) end

---Returns the max health of the squad. 
--- The max health of a squad is the max health of each entity in
--- that squad.  This means that the max health of a squad may
--- change when entities are added or removed from the squad.
---
---
---@param squad SquadID
---@return number
function Squad_GetHealthMax(squad) end

---Returns the entity blueprint name for the leader of a squad. 
---If a unit is attached to the squad, this will always be the
--- leader otherwise the sergeant, then the a trooper.
---@param squad SquadID
---@return string
function Squad_GetLeaderBP(squad) end

---Returns the max number of units allowed in the squadron (from
--- squad_loadout_ext) 
---@param squad SquadID
---@return integer
function Squad_GetMax(squad) end

---Returns the minimum number of units in the squadron (from
--- squad_loadout_ext) 
---@param squad_blueprint string
---@return integer
function Squad_GetMaxFromName(squad_blueprint) end

---Returns the minimum number of units in the squadron (from
--- squad_loadout_ext) 
---@param squad SquadID
---@return integer
function Squad_GetMin(squad) end

---Returns the minimum number of units in the squadron (from
--- squad_loadout_ext) 
---@param squad_blueprint string
---@return integer
function Squad_GetMinFromName(squad_blueprint) end

---Returns the current morale of a squad as a percent [0.0, 1.0] 
---@param squad SquadID
---@return number
function Squad_GetMorale(squad) end

---Returns true if a squad is invulnerable to morale damage. 
---@param squad SquadID
---@return boolean
function Squad_GetMoraleInvulnerable(squad) end

---Returns the Player owner of the given squad. Squad MUST NOT be
--- owned by the world. 
---@param squad SquadID
---@return PlayerID
function Squad_GetPlayerOwner(squad) end

---Returns the average position of the spawned units in the squad.
--- The position is currently a lua table with three entries (x, y,
--- z) 
---@param squad SquadID
---@return Position
function Squad_GetPosition(squad) end

---Returns the average position of the despawned AND spawned units
--- in the squad. 
---@param squad SquadID
---@return Position
function Squad_GetPositionDeSpawned(squad) end

---Returns the movement speed of the LEADER in km per hour 
---@param squad SquadID
---@return number
function Squad_GetSpeed(squad) end

---Returns the max number of upgrades this squadron can get (from
--- squad_loadout_ext) 
---@param squad SquadID
---@return integer
function Squad_GetUpgradeMax(squad) end

---Returns true if the given squad is owned by the world 
---@param squad SquadID
---@return boolean
function Squad_GetWorldOwner(squad) end

---Checks if a squads base trooper blueprint has the given armour
--- type. 
--- armourTypeName param must be a valid armour type (type_armour)
--- from the Attribute Editor.
--- ArmourTypes: "tp_armour", "tp_builder", "tp_building",
--- "tp_infantry", "tp_infantry", "tp_infantry_heavy",
--- "tp_commander", "tp_monster", "tp_vehicle"
---@param squad SquadID
---@param armourTypeName string
---@return boolean
function Squad_HasArmourType(squad, armourTypeName) end

---Returns true if a squad has been reinforced with a leader. 
---NOTE: Does not check reinforce queue for queued leaders.
---@param squad SquadID
---@return boolean
function Squad_HasLeader(squad) end

---Returns true if a squad has been attached. 
---@param squad SquadID
---@return boolean
function Squad_IsAttached(squad) end

---Returns true if ALL or ANY troopers in a squad are in cover 
---@param squad SquadID
---@param all boolean
---@return boolean
function Squad_IsInCover(squad, all) end

---Returns true if a squad has infiltrate enabled 
---This function will always return false for squads that do not
--- have a squad_infiltration_ext
---@param squad SquadID
---@return boolean
function Squad_IsInfiltrated(squad) end

---Returns true if morale is broken for a squad 
---@param squad SquadID
---@return boolean
function Squad_IsMoraleBroken(squad) end

---Returns true if the squad is currently reinforcing.  This
--- function will return false if the squad does not have a
--- reinforce ext. 
---@param squad SquadID
---@return boolean
function Squad_IsReinforcing(squad) end

---Returns true if any trooper in the squad was attacked in the
--- last second. 
---@param squad SquadID
---@return boolean
function Squad_IsUnderAttack(squad) end

---Returns true if any trooper in the squad was attacked by the
--- given player in the last second 
---@param squad SquadID
---@param player PlayerID
---@return boolean
function Squad_IsUnderAttackByPlayer(squad, player) end

---Returns true if the squad is currently upgrading.  This function
--- will return false if the squad does not have a reinforce ext. 
---@param squad SquadID
---@return boolean
function Squad_IsUpgrading(squad) end

---Go through the squad and destroy all entities with matching
--- blueprints 
---@param squad SquadID
---@param entitybp string
function Squad_KillEntityWithBP(squad, entitybp) end

---Set the health of all troopers in a squad.  Health must be in
--- range [0.0, 1.0] 
---@param squad SquadID
---@param healthPercent number
function Squad_SetHealth(squad, healthPercent) end

---Make a squad invulnerable to physical damage. 
---IMPORTANT: You must disable invulnerability for everytime you
--- enable it!
---@param squad SquadID
---@param enabled boolean
function Squad_SetHealthInvulnerable(squad, enabled) end

---Set the morale of all troopers in a squad.  Morale must be in
--- range [0.0, 1.0] 
---@param squad SquadID
---@param moralePercent number
function Squad_SetMorale(squad, moralePercent) end

---Makes a squad invulnerable to morale damage. 
---IMPORTANT: You must disable morale invulnerability for everytime
--- you enable it!
---@param squad SquadID
---@param enabled boolean
function Squad_SetMoraleInvulnerable(squad, enabled) end

---Changes the owner of the given squad. 
---@param squad SquadID
---@param owner PlayerID
function Squad_SetPlayerOwner(squad, owner) end

---Sets the owner to the world for all entities in the squad. 
---@param squad SquadID
function Squad_SetWorldOwner(squad) end

---Spawn the entire squad at a given position 
---@param squad SquadID
---@param pos Position
function Squad_Spawn(squad, pos) end

---Find a upgradable trooper in a squad and upgrade its weapon. 
--- Can only upgrade to weapons which have all requirements met. 
---Use Squad_ForceUpgradeWeapons to force all troops in a squad to
--- upgrade their weapons regardless of requirments.
---@param squad SquadID
---@param weaponname string
---@return boolean
function Squad_UpgradeWeapon(squad, weaponname) end

---Returns the current number of weapons in a squad. 
---@param squad SquadID
---@param weaponname string
---@return integer
function Squad_WeaponCount(squad, weaponname) end

---Enable strategic point, relic, or objective ui 
---@param strat_ebp_name string
---@param enable boolean
function Taskbar_EnableStrategicUI(strat_ebp_name, enable) end

---Returns HTL_None, HTL_Min, or HTL_Max 
---@return integer
function Taskbar_GetHelpTextLevel() end

---Returns true if the HQ has been focused on using the Home key in
--- the last amount of time 
---@param secs number
---@return boolean
function Taskbar_HasFocusedOnHQ(secs) end

---Clear the time and hide ui 
---@param strat_ebp_name string
function Taskbar_HideStrategicUITimer(strat_ebp_name) end

---Returns true if the taskbar is minimized and false if it is in
--- its normal state 
---@return boolean
function Taskbar_IsMinimized() end

---Set tooltip level to HTL_None, HTL_Min, or HTL_Max 
---@param level integer
function Taskbar_SetHelpTextLevel(level) end

---Set the timer beside a strategic UI 
---@param strat_ebp_name string
---@param player PlayerID
---@param secsRemaining integer
function Taskbar_SetStrategicUITimer(strat_ebp_name, player, secsRemaining) end

---Add the amount of time to the specified timer 
---@param timerId integer
---@param period number
function Timer_Add(timerId, period) end

---Display (in the console) the amount of time remaining in the
--- specified timer. 
---@param timerId integer
function Timer_Display(timerId) end

---Stop the specified timer. 
---@param timerId integer
function Timer_End(timerId) end

---Get the remaining time for the specified timer. 
---@param timerId integer
function Timer_GetRemaining(timerId) end

---Pause the specified timer. 
---@param timerId integer
function Timer_Pause(timerId) end

---Resume the specified timer. 
---@param timerId integer
function Timer_Resume(timerId) end

---Start a timer with the specified id, taking the required period
--- ( in seconds ) 
---@param timerId integer
---@param period number
function Timer_Start(timerId, period) end

---Displays a breif UI warning in the critical alert message area.
--- 
---PLEASE be very careful how you use this as it can cause
--- localization issues.  For example, if you want to display the
--- message 'PlayerName1 is going to win!', you CANNOT use
--- concatanation in lua to create the string, you should use
--- Loc_FormatText instead.  Ask for help if you are unsure.
---@param locText LocString
function UIWarning_Show(locText) end

---Prematurely hide/remove a countdown 
function UI_HideCountdown() end

---Start a countdown displaying secsRemaining.  Timer will
--- automagically countdown and hide once done. 
---Pass in a formatID with format 'Timer title: %1TIME%'
---@param formatID integer
---@param secsRemaining number
function UI_ShowCountdown(formatID, secsRemaining) end

---Unrestricts a specific addon and gives an intelevent style
--- dialog and an event cue with it.  Pass in "none" for event cue
--- or dialog in order to not display the alerts 
---@param playerId PlayerID
---@param addon_blueprint blueprint
---@param eventcuedescription_locId locID
---@param dialog_locId locID
function Unrestrict_AddOnWithAlerts(playerId, addon_blueprint, eventcuedescription_locId, dialog_locId) end

---Unrestricts a specific building and gives an intelevent style
--- dialog and an event cue with it.  Pass in "none" for event cue
--- or dialog in order to not display the alerts 
---@param playerId PlayerID
---@param building_blueprint blueprint
---@param eventcuedescription_locId locID
---@param dialog_locId locID
function Unrestrict_BuildingWithAlerts(playerId, building_blueprint, eventcuedescription_locId, dialog_locId) end

---Unrestricts a specific research and gives an intelevent style
--- dialog and an event cue with it.  Pass in "none" for event cue
--- or dialog in order to not display the alerts 
---@param playerId PlayerID
---@param research_blueprint blueprint
---@param eventcuedescription_locId locID
---@param dialog_locId locID
function Unrestrict_ResearchWithAlerts(playerId, research_blueprint, eventcuedescription_locId, dialog_locId) end

---Unrestricts a specific squad and gives an intelevent style
--- dialog and an event cue with it.  Pass in "none" for event cue
--- or dialog in order to not display the alerts 
---@param playerId PlayerID
---@param squad_blueprint blueprint
---@param eventcuedescription_locId locID
---@param dialog_locId locID
function Unrestrict_SquadWithAlerts(playerId, squad_blueprint, eventcuedescription_locId, dialog_locId) end

---Unrestricts a specific weapon  and gives an intelevent style
--- dialog and an event cue with it.  Pass in "none" for event cue
--- or dialog in order to not display the alerts 
---@param playerId PlayerID
---@param weapon_blueprint blueprint
---@param eventcuedescription_locId locID
---@param dialog_locId locID
function Unrestrict_WeaponWithAlerts(playerId, weapon_blueprint, eventcuedescription_locId, dialog_locId) end

---Creates an Autosave and prints text to screen to let player know
--- its happening. 
---Use this function to create a save game in the single player
--- game
---@param localizedID string
function Util_Autosave(localizedID) end

---If only one team is alive, players on that team win.  Returns
--- true if one team left. 
---@param win_condition string
---@return boolean
function Util_CheckOneTeamLeft(win_condition) end

---Creates horrors and summons them in 
---Use loadout of zero to create a squad at its min loadout. 
--- Squads can never exceed their max loadout.
---@param playerId PlayerID
---@param squadgroupName string
---@param pos Position
---@param numsquads integer
---@param loadout integer
---@return SGroupID
function Util_CreateHorrorsAndSummon(playerId, squadgroupName, pos, numsquads, loadout) end

---Creates a given number of squads with a loadout at a position
--- and adds them to a squadgroup.  The squads are then dropped in
--- a droppod. 
---Use loadout of zero to create a squad at its min loadout. 
--- Squads can never exceed their max loadout.
---@param playerId PlayerID
---@param squadgroupName string
---@param squadBlueprint string
---@param pos Position
---@param numsquads integer
---@param loadout integer
---@return SGroupID
function Util_CreateSquadsAndDropPodIn(playerId, squadgroupName, squadBlueprint, pos, numsquads, loadout) end

---Creates a given number of squads at a marker position and adds
--- them to a squadgroup.  The squadgroup will be created if it
--- doesnt exist. 
---Squads will be created with min loadout.
---@param playerId PlayerID
---@param squadgroupName string
---@param squadBlueprint string
---@param markername string
---@param numsquads integer
---@return SGroupID
function Util_CreateSquadsAtMarker(playerId, squadgroupName, squadBlueprint, markername, numsquads) end

---Creates a given number of squads at a marker position with a
--- loadout and adds them to a squadgroup.  The squadgroup will be
--- created if it doesnt exist. 
---Use loadout of zero to create a squad at its min loadout. 
--- Squads can never exceed their max loadout.
---@param playerId PlayerID
---@param squadgroupName string
---@param squadBlueprint string
---@param markername string
---@param numsquads integer
---@param loadout integer
---@return SGroupID
function Util_CreateSquadsAtMarkerEx(playerId, squadgroupName, squadBlueprint, markername, numsquads, loadout) end

---Creates a given number of squads at a marker position and adds
--- them to a squadgroup.  The squad will spawn at a slightly
--- random position in relation to the specified position.  The
--- squadgroup will be created if it doesnt exist. 
---Squads will be created with min loadout.
---@param playerId PlayerID
---@param squadgroupName string
---@param squadBlueprint string
---@param markername string
---@param numsquads integer
---@return SGroupID
function Util_CreateSquadsAtMarkerRandom(playerId, squadgroupName, squadBlueprint, markername, numsquads) end

---Creates a given number of squads at a marker position with a
--- loadout and adds them to a squadgroup.  The squad will spawn at
--- a slightly random position in relation to the specified
--- position.  The squadgroup will be created if it doesnt exist. 
---Use loadout of zero to create a squad at its min loadout. 
--- Squads can never exceed their max loadout.
---@param playerId PlayerID
---@param squadgroupName string
---@param squadBlueprint string
---@param markername string
---@param numsquads integer
---@param loadout integer
---@return SGroupID
function Util_CreateSquadsAtMarkerRandomEx(playerId, squadgroupName, squadBlueprint, markername, numsquads, loadout) end

---Creates a given number of squads at a position and adds them to
--- a squadgroup.  The squadgroup will be created if it doesnt
--- exist. 
---Squads will be created with min loadout.
---@param playerId PlayerID
---@param squadgroupName string
---@param squadBlueprint string
---@param pos Position
---@param numsquads integer
---@return SGroupID
function Util_CreateSquadsAtPosition(playerId, squadgroupName, squadBlueprint, pos, numsquads) end

---Creates a given number of squads with a loadout at a position
--- and adds them to a squadgroup.  The squadgroup will be created
--- if it doesnt exist. 
---Use loadout of zero to create a squad at its min loadout. 
--- Squads can never exceed their max loadout.
---@param playerId PlayerID
---@param squadgroupName string
---@param squadBlueprint string
---@param pos Position
---@param numsquads integer
---@param loadout integer
---@return SGroupID
function Util_CreateSquadsAtPositionEx(playerId, squadgroupName, squadBlueprint, pos, numsquads, loadout) end

---Creates a given number of squads at a position and adds them to
--- a squadgroup.  The squad will spawn at a slightly random
--- position in relation to the specified position.  The squadgroup
--- will be created if it doesnt exist. 
---Squads will be created with min loadout.
---@param playerId PlayerID
---@param squadgroupName string
---@param squadBlueprint string
---@param pos Position
---@param numsquads integer
---@return SGroupID
function Util_CreateSquadsAtPositionRandom(playerId, squadgroupName, squadBlueprint, pos, numsquads) end

---Creates a given number of squads with a loadout at a position
--- and adds them to a squadgroup.  The squad will spawn at a
--- slightly random position in relation to the specified position.
---  The squadgroup will be created if it doesnt exist. 
---Use loadout of zero to create a squad at its min loadout. 
--- Squads can never exceed their max loadout.
---@param playerId PlayerID
---@param squadgroupName string
---@param squadBlueprint string
---@param pos Position
---@param numsquads integer
---@param loadout integer
---@return SGroupID
function Util_CreateSquadsAtPositionRandomEx(playerId, squadgroupName, squadBlueprint, pos, numsquads, loadout) end

---Drop-pod an existing squadgroup to a position on the map 
---Works on spawned or depspawned group
---@param sgroupname string
---@param pos Position
function Util_DropPod(sgroupname, pos) end

---Drop-pod an existing squadgroup to a marker on the map 
---Works on spawned or depspawned group
---@param sgroupname string
---@param markername string
function Util_DropPodMarker(sgroupname, markername) end

---Find all the entities with a given blueprint in sourcegroup and
--- add them to destgroup. 
---destgroup will be created if it does not already exist. 
---@param sourcegroup string
---@param destgroup string
---@param bpname string
function Util_GetEntitiesByBP(sourcegroup, destgroup, bpname) end

---Find all the squads with a given armour type in sourcegroup and
--- add them to destgroup. 
---destgroup will be created if it does not already exist.  See
--- Squad_HasArmourType for list of valid armour types.
---@param sourcegroup string
---@param destgroup string
---@param armourtype string
function Util_GetSquadsByArmourType(sourcegroup, destgroup, armourtype) end

---Find all the squads with a given blueprint in sourcegroup and
--- add them to destgroup. 
---destgroup will be created if it does not already exist. 
---@param sourcegroup string
---@param destgroup string
---@param bpname string
function Util_GetSquadsByBP(sourcegroup, destgroup, bpname) end

---Adds all the points owned by a given player into a given group. 
--- use types SPT_StrategicPoint SPT_StrategicObjective or
--- SPT_Relic 
---Pass in a playerID of nil to get all relics owned by the world
---@param strategicpoint_type integer
---@param playerID PlayerID
---@param groupname string
function Util_GetStrategicPoints(strategicpoint_type, playerID, groupname) end

---Returns trailing numbers from a string, if it exists, nil
--- otherwise. E.G. "marker23" would return 23. 
---@param val string
---@return number
function Util_GetTrailingNumber(val) end

---Function takes multiple strings and returns a BlueprintTable (
--- used for SGroup_ContainsBlueprints ) 
--- Example:
---     local bpTable = Util_MakeBlueprintTable( "chaos_marine_squa
---d", "chaos_marine_squad", "chaos_squad_bloodthirster" )
---     --Check if SGroup contains ALL items from the list (two
--- chaos marines and a bloodthirster)
---     local result = SGroup_ContainsBlueprints( "sg_MyGroup",
--- bpTable, true )
---
---     --Check if an SGroup contains ANY blueprints from the list 
---    result = SGroup_ContainsBlueprints( "sg_MyGroup", bpTable,
--- false )
---
---     If the first parameter is a table, it will append entries
--- to the existing table
---@param args Args
---@return BlueprintTable
function Util_MakeBlueprintTable(args) end

---Play an events file at a given markers location 
---@param markername string
---@param eventfile string
---@return void
function Util_MarkerFX(markername, eventfile) end

---Play the mission title fade. 
---@param loc_str LocString
function Util_MissionTitle(loc_str) end

---Muffles / Unmuffles all background FX sounds for NIS's 
---@param mute boolean
function Util_MuffleNISSound(mute) end

---Marks the specified objective as completed 
---Use this function to complete an objective with the sound
---@param titleid integer
function Util_ObjectiveComplete(titleid) end

---Creates an objective and plays the objective created stinger to
--- accompany it. 
---Use this function to create an objective with the sound
---@param objectiveTable LuaTable
---@param primary boolean
function Util_ObjectiveCreate(objectiveTable, primary) end

---Makes the specified objective as failed 
---Use this function to fail an objective with the sound
---@param titleid integer
function Util_ObjectiveFail(titleid) end

---Create a looping ping. 
---Works with the Util_Ping_Stop( ) to allow for the easy removal
--- of looping pings without having to save pingID variables. Make
--- up a pingname string and pass it in.
---@param pingname string
---@param markername string
function Util_Ping_LoopingMkr(pingname, markername) end

---Stops a looping ping 
---Works with the Util_Ping_LoopingMkr( ) to allow for the easy
--- removal of looping pings without having to save pingID
--- variables. Use the pingname to recall and stop a previously
--- created looping ping
---@param pingname string
function Util_Ping_Stop(pingname) end

---Reset the contruction state for a building and order a builder
--- group to start rebuilding the structure 
---The building group must contain one and only one structure
---@param building_groupname string
---@param builder_groupname string
function Util_ResetAndRebuild(building_groupname, builder_groupname) end

---Converts a 2D top down position to a 3D ScarPosition. 
--- 3D ScarPositions have the x axis left to right, the z axis in
--- to out, and the y axis down to up (y axis represents the height
--- of the terrain).  Use this function to convert a top-down 2D
--- position to a 3D world position.
---
--- Note: (0,0) is in the center of the map.
---@param xpos number
---@param ypos number
---@return Position
function Util_ScarPos(xpos, ypos) end

---Sets sounds properly for Intelevents.  set true for start of IE
--- and false for end of IE 
---@param enter boolean
function Util_SetIE(enter) end

---Play a Intel event. 
---This function should used instead of Event_Start because it
--- handles priorities.  If an nislet is triggered during an intel
--- event, the intel event will be cancelled, and if an intel event
--- is triggered during a nislet, the intel event will be queued.
---@param func function
function Util_StartIntel(func) end

---Play a NISlet. 
---This function should used instead of Event_Start because it
--- handles priorities.  If an nislet is triggered during an intel
--- event, the intel event will be cancelled, and if an intel event
--- is triggered during a nislet, the intel event will be queued.
---@param func function
function Util_StartNIS(func) end

---Summon an existing squadgroup to a position on the map 
---Works on spawned or depspawned group
---@param sgroupname string
---@param pos Position
function Util_Summon(sgroupname, pos) end

---Tracks a character and, if dead, drop pods a new one in. 
---Use this function to track whether the player has a force
--- commander etc.  If ever that character dies, a replacement will
--- drop pod in at the specified marker
---@param playerid Integrer
---@param characterGroup string
---@param characterBlueprint string
---@param characterDropPodMarkerName string
function Util_TrackCharacterAndDropPodIn(playerid, characterGroup, characterBlueprint, characterDropPodMarkerName) end

---Assign a group number [0,9] to the first entity of an egroup. 
---@param egroup EGroupID
---@param nGroupNumber integer
function W40k_AssignHotkeyEGroup(egroup, nGroupNumber) end

---Assign a group number [0,9] to a squad group 
---@param sgroup SGroupID
---@param nGroupNumber integer
function W40k_AssignHotkeySGroup(sgroup, nGroupNumber) end

---Autosaves the current mission to the player:campaign folder. 
---Must provide a UI name
---@param savename LocString
function W40k_Autosave(savename) end

---Precache a range of speech files. 
---This is useful for preloading sounds at the begining of the
--- mission so they arent loaded during the NISlets.
---@param rangeBegin integer
---@param rangeEnd integer
function W40k_CacheSpeech(rangeBegin, rangeEnd) end

---Enables/Disables all input EXCEPT for ESC and F10. 
---@param enabled boolean
function W40k_EnableInput(enabled) end

---Enableds/disables all selection ui 
---@param enable boolean
function W40k_EnableSelectionUI(enable) end

---Removes a message added by W40k_ShowSystemMessage. 
---@param msg LocString
function W40k_HideSystemMessage(msg) end

---Fade a background image in then out.  You cannot WAIT on this
--- function. 
---texture filename is relative to the data:art/ui/ingame/image_fad
---es/ folder
---@param texture string
---@param in number
---@param lifetime number
---@param out number
function W40k_ImageFade(texture, in, lifetime, out) end

---Returns true if the ui is in letterbox mode 
---@return boolean
function W40k_IsLetterboxed() end

---Enters/Exits letterbox mode in amount of time specified by
--- timeSecs. 
---This function can be called through a CTRL object in NISlets.
---@param on boolean
---@param timeSecs number
function W40k_Letterbox(on, timeSecs) end

---Starts a movie.  Use the shortname for moviename (ie mymovie)
--- with no extension. 
---This function call be called through a CTRL object and waited on
---@param moviename string
function W40k_PlayMovie(moviename) end

---Quits the app immediately 
function W40k_QuitApp() end

---Select the first spawned entity in an egroup. Clears current
--- selection. 
---@param group EGroupID
function W40k_SelectEGroup(group) end

---Select all spawned squads in an sgroup.  Clears current
--- selection. 
---@param group SGroupID
function W40k_SelectSGroup(group) end

---Cycle focus through current selection. 
---This is the same as using the aposophre key to focus on an
--- entity. It is known that this feature does not work well on
--- vehicles.  I would recommend not using it for such.
function W40k_SelectionFocus() end

---Shows a system message in the area where Game Paused text
--- appears/ 
---Call W40k_HideSystemMessage to remove the message.
---@param msg LocString
function W40k_ShowSystemMessage(msg) end

---Unlocks user/camera input during letterbox. This should be used
--- for debug only 
function W40k_UnlockInputOnLetterBox() end

---Create win/lose warning.  You can use the name to reference the
--- warning later. 
--- String name - The name of the win warning.  Each win warning
--- needs a unique name so you have a way of changing its text at a
--- later time.
--- PlayerID player - The player that this win warning applies to. 
--- This isnt used right now, but its good to have in case we want
--- to make the text the same color as the player or something.
--- String icon - The name of the icon.  Again, this is not used
--- right now, but it may be in the future.
--- LocString wWinConditionName - This is a localized string
--- containing the name of the win condition that this win warning
--- applies to. This text is used for the title of the win warnings
--- help tip.
--- LocString wHelpTip - This is a localized string containing the
--- help tip description.
---
---@param name string
---@param player PlayerID
---@param icon string
---@param wWinConditionName LocString
---@param wHelpTip LocString
function WinWarning_Add(name, player, icon, wWinConditionName, wHelpTip) end

---Returns true if the named win warning exists. 
---@param name string
---@return boolean
function WinWarning_Exists(name) end

---Removes an added win/lose warning 
---@param name string
function WinWarning_Remove(name) end

---Set the text for a win warning. 
---Designers: PLEASE be careful how you use this.  Use
--- Loc_FormatText where nesc. for localization.
---@param name string
---@param locText LocString
function WinWarning_SetText(name, locText) end

---Get the distance between a squad group and a point. 
--- Pass in true for 3rd parameter to get the closest point and
--- pass in false to get the furthest point.  The closest/furthest
--- point is calculated on a per entity basis.  So the closest
--- distance would be that of the closest entity.
--- Also note, it is an error to call this function with an empty
--- group.
---
---@param egroup EGroupID
---@param p1 Position
---@param closest boolean
---@return number
function World_DistanceEGroupToPoint(egroup, p1, closest) end

---Get the distance between two points. 
---@param p1 Position
---@param p2 Position
---@return number
function World_DistancePointToPoint(p1, p2) end

---Get the distance between a squad group and a point. 
--- Pass in true for 3rd parameter to get the closest point and
--- pass in false to get the furthest point.  The closest/furthest
--- point is calculated on a per squadron basis.  So the closest
--- distance would be that of the closest squadron.
--- It is an error to call this function with an empty group. 
--- Check the group size with SquadGroup_Count( ) first.
---@param sgroup SGroupID
---@param p1 Position
---@param closest boolean
---@return number
function World_DistanceSGroupToPoint(sgroup, p1, closest) end

---Enables or disables the fog of war between these two players 
---@param p0 PlayerID
---@param p1 PlayerID
---@param enable boolean
function World_EnablePlayerToPlayerFOW(p0, p1, enable) end

---Triggers an event file at the given location (relative to
--- Art/Events without extension folder) 
---Example: 
---
---World_FXEvent("movedest" World_Pos(0,0,0))
---     Plays the MoveFX event at location(0,0,0)
---@param filename string
---@param pos Position
function World_FXEvent(filename, pos) end

---Destroy/Stop an event that was started with World_FXEvent 
---Pass in the result of World_FXEvent
---@param event integer
function World_FXEventDestroy(event) end

---Triggers an event file at the given location (relative to
--- Art/Events without extension folder) 
---Example: 
---
---World_FXEvent("movedest" World_Pos(0,0,0))
---     Plays the MoveFX event at location(0,0,0)
---@param filename string
---@param pos Position
---@return integer
function World_FXEventLooping(filename, pos) end

---Return the total game time in seconds. 
---@return number
function World_GetGameTime() end

---Returns the number of strategic objectives on this map 
---@return integer
function World_GetNumStrategicObjectives() end

---Returns the number of strategic points with blueprint
--- strategic_point_flag on this map 
---@return integer
function World_GetNumStrategicPoints() end

---- Returns the player at a given zero based index. 
---@param index integer
---@return PlayerID
function World_GetPlayerAt(index) end

---Return the total number of players in the world 
---@return integer
function World_GetPlayerCount() end

---- Returns the player index given the Player* 
---@param player PlayerID
---@return integer
function World_GetPlayerIndex(player) end

---Returns the number of types of buildings a race can build 
---@param race_index integer
---@return integer
function World_GetPossibleBuildingsCount(race_index) end

---Returns the name of a chosen building for a race 
---@param race_index integer
---@param building_index integer
---@return string
function World_GetPossibleBuildingsName(race_index, building_index) end

---Returns the number of types of squads a race can build 
---@param race_index integer
---@return integer
function World_GetPossibleSquadsCount(race_index) end

---Returns the blueprint name of a chosen squad for a race 
---@param race_index integer
---@param squad_index integer
---@return string
function World_GetPossibleSquadsName(race_index, squad_index) end

---Returns the race index of a race, given its name (from the ME).
--- 
---Valid race names for W40K are "space_marine_race",
--- "chaos_marine_race", "ork_race", "guard_race", or "eldar_race".
---@param racename string
---@return integer
function World_GetRaceIndex(racename) end

---Returns a random integer with range [min, max] 
---It is recomended you use this instead of luas math.random
--- function
---@param min integer
---@param max integer
---@return integer
function World_GetRand(min, max) end

---- Given any position in the world, this function will return
--- position safe for spawning a given entity 
---@param around Position
---@param entity EntityID
---@return Position
function World_GetSpawnablePosition(around, entity) end

---Adds all of the world-owned strategic points to the given egroup
--- 
---@param egroup EGroupID
function World_GetStrategicPoints(egroup) end

---- to document 
---@return boolean
function World_IsGameOver() end

---Returns true if the squad is owned by the world 
---@param entity EntityID
---@return boolean
function World_OwnsEntity(entity) end

---Returns true if the squad is owned by the world 
---@param squad SquadID
---@return boolean
function World_OwnsSquad(squad) end

---Returns true if two world positions are in proximity to each
--- other 
---@param p1 Position
---@param p2 Position
---@param prox number
---@return boolean
function World_PointPointProx(p1, p2, prox) end

---Creates a new Position object. 
---  A position object is basically a table with an x, y, and z
--- attribute.  You can directly access the individual components
--- in the same way that you would access a field in a table.
---
---  Example:
---   --* lua script snip
---   local pos = World_Pos(2, 0, 0)
---   pos.x = pos.x + 1
---   print pos.x -- this will print 3
---   --* lua script snip
---
---@param x number
---@param y number
---@param z number
---@return Position
function World_Pos(x, y, z) end

---- to document 
function World_SetGameOver() end

---Trigger a lose from a wincondition. Use empty string for SP. 
--- Sets the player and all the members allied to player to a win
--- state, with a reason for winning.   Sets the player to a lose
--- state, with a reason for losing, and kills the player 
---@param player PlayerID
---@param winconditionName string
function World_SetPlayerLose(player, winconditionName) end

---- Trigger a win from a wincondition. Use empty string for SP. 
--- Sets all other players to a lose state but does not kill all of
--- their units.  Call World_SetPlayerLost to kill all units. 
---@param player PlayerID
---@param winconditionName string
function World_SetTeamWin(player, winconditionName) end

---Dump animator statistics to a file 
function blueprint_dump() end

---Loads data:scar/fileName and executes it
---@param fileName string
function import(fileName) end

---Receives any number of arguments and prints their values as strings
---@param s1 any
---@param s2 any
---@param s3 any
---@param ... any
function print(s1, s2, s3, ...) end

