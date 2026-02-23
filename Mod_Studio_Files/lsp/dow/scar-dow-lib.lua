---@meta scar-dow-lib

-- Auto-generated from DoWDE .scar library files by scar-to-luadefs.py
-- 121 functions
-- Do not edit manually — regenerate with: python tools/scar-to-luadefs.py

function Balance_AddTimer() end

function Balance_ApplyTimeOut() end

function Balance_DumpStats() end

function Balance_GameOver() end

function Balance_OnInit() end

function Balance_PrintTime() end

---@param player any
function Balance_Reset(player) end

function Balance_Run() end

---@param iterations any
function Balance_SetIterations(iterations) end

---@param name any
function Balance_SetScriptName(name) end

---@param timeout any
function Balance_SetTimeOut(timeout) end

function Balance_TimeOut() end

--- Positions the camera view at the start of the first path so the game can fade up to a 'ready' shot. MUST be used first before any other CPath's are started. Should use CPath_CutToPath() to follow up.
---@param canimName string
---@param startKey string
---@param endKey string
function CPath_PresetCamera(canimName, startKey, endKey) end

--- Orders an entity to build a squad in it's build queue
--- The squadBlueprintToBuild is the name of the squad you want to build from the building.
---@param egroupname string
---@param squadBlueprintToBuild string
function Cmd_BuildSquad(egroupname, squadBlueprintToBuild) end

--- Checks the egroup for an entity that can cast the ability and casts it on the target.
---@param egroupname string
---@param abilityname string
---@param targetgroup string
---@return boolean
function Cmd_CastAbilityEGroupS(egroupname, abilityname, targetgroup) end

--- Checks the sgroup for an entity that can cast the ability and casts it on the target.
---@param sgroupname string
---@param abilityname string
---@param targetgroup string
---@return boolean
function Cmd_CastAbilitySGroupE(sgroupname, abilityname, targetgroup) end

--- Checks the egroup for all entities that can cast the ability and has those entities casts on themselves
---@param groupName string
---@param abilityName string
---@return boolean
function Cmd_EGroupCastAbilitySelf(groupName, abilityName) end

--- Relocates every entity in the group that can relocate to the closest free spot withing 'searchRadius' meters of marker 'markerName'
---@param groupName string
---@param markerName string
---@param searchRadius integer
function Cmd_EGroupRelocateMarker(groupName, markerName, searchRadius) end

--- Relocates every entity in the group that can relocate to the closest free spot within 'searchRadius' meters of 'position'.
---@param groupName string
---@param position Position
---@param searchRadius integer
function Cmd_EGroupRelocatePosition(groupName, position, searchRadius) end

--- Checks the egroup for an entity that can cast the ability and casts it at the marker.
---@param egroupname string
---@param abilityname string
---@param targetmarker string
---@return boolean
function Cmd_EntityCastAbilityMarker(egroupname, abilityname, targetmarker) end

--- Checks the egroup for an entity that can cast the ability and casts it on the target pos.
---@param egroupname string
---@param abilityname string
---@param targetpos Position
---@return boolean
function Cmd_EntityCastAbilityPos(egroupname, abilityname, targetpos) end

--- Send a spawn slot A command to a harvester unit
--- Spawn the unit linked to the spawnslot of the harvester unit
---@param sgroupname string
function Cmd_HarvestSpawnSlotA(sgroupname) end

--- Send a spawn slot A command to a harvester unit
--- Spawn the unit linked to the spawnslot of the harvester unit
---@param sgroupname string
function Cmd_HarvestSpawnSlotB(sgroupname) end

--- Send a spawn slot A command to a harvester unit
--- Spawn the unit linked to the spawnslot of the harvester unit
---@param sgroupname string
function Cmd_HarvestSpawnSlotC(sgroupname) end

--- Returns true if ALL or ANY squads in a group are currently reinforcing a leader.
--- Set all to true to check for ALL or set to false to check for ANY.
---@param sgroupname string
---@param all boolean
---@return boolean
function Cmd_IsReinforcingLeader(sgroupname, all) end

--- Moves squads in group to a location offset by some distance around 'position'.
--- The offset is a number from 0 to 7 and indicates a location around 'position' as follows:
--- 7   0   1
--- 6  pos  2
--- 5   4   3
---@param groupName string
---@param position Position
---@param offset integer
---@param distance number
function Cmd_MoveToPosOffset(groupName, position, offset, distance) end

--- Moves squads in group to a location offset by some distance around the targert sgroup.
--- The the target SGroup can only have one squad in it. The offset is a number from 0 to 7 and indicates a location around the target sgroup (oriented based on that group's orientation) as follows:
--- 7   0   1
--- 6  pos  2
--- 5   4   3
---@param groupName string
---@param targetGroupName string
---@param offset integer
---@param distance number
function Cmd_MoveToSGroupOffset(groupName, targetGroupName, offset, distance) end

--- Tell a squad to entrench.
--- The given unit should have an squad entrench extension.
---@param sgroupname string
function Cmd_SquadEntrench(sgroupname) end

--- Tell a squad to uproot.
--- The given unit should have an squad entrench extension.
---@param sgroupname string
function Cmd_SquadUproot(sgroupname) end

---@param T any
---@param msg any
---@param indent any
function Debug_PrintTable(T, msg, indent) end

--- Returns the sum of the current healths of all entities in the group.
---@param groupName string
---@return number
function EGroup_GetTotalHealthCurrent(groupName) end

--- Returns the sum of the maximum healths of all entities in the group.
---@param groupName string
---@return number
function EGroup_GetTotalHealthMax(groupName) end

--- Returns whether all or any of the entities in the group have the specified modifier applied to them.
--- If 'all' is true, all entities must have the modifier applied for the function to return true. If 'all' is false, at least one entity must have the modifier.
---@param groupName string
---@param modifierType string
---@param all boolean
---@return boolean
function EGroup_HasModifier(groupName, modifierType, all) end

--- Returns whether all or any entities in the group are relocating
--- If 'all' is true, all entities must be relocating for the function to return true. If 'all' is false, at least one entity must be relocating.
---@param groupName string
---@param all boolean
---@return boolean
function EGroup_IsRelocating(groupName, all) end

--- Returns whether all or any entities in the group are shrouded
--- If 'all' is true, all entities must be shrouded for the function to return true. If 'all' is false, at least one entity must be shrouded.
---@param groupName string
---@param all boolean
---@return boolean
function EGroup_IsShrouded(groupName, all) end

--- Respawns all entities in a group at a given position
---@param groupname String markerName string
function EGroup_ReSpawnMarker(groupname String markerName) end

--- Respawns all entities in a group at a given position
---@param groupname ScarPosition position string
function EGroup_ReSpawnPosition(groupname ScarPosition position) end

--- Takes the given egroupname and issues the 'Destroy' command to all entities. It should behave the same as if the player hit the delete key for each member.
--- To absolutely kill entities, this should be used instead of either EGroup_SetAvgHealth(egroupname, 0) or EGroup_DestroyAllEntities(egroupname) as both of these do not always accomplish the desired effect
---@param egroupname string
---@param boolean boolean
function EGroup_SelfDestroy(egroupname, boolean) end

--- Enables or disables the shroud ability for all entities in the group that have the ability
--- Shroud will be turned on if 'enable' is true and turned off when it is false
---@param groupName string
---@param enable boolean
function EGroup_Shroud(groupName, enable) end

--- Uncaptures any captured strategic point or relic in the group
---@param groupName string
function EGroup_Uncapture(groupName) end

--- Unreveals an area the size of a given markers proximity at that markers position.  YOU SHOULD ONLY CALL THIS IF YOU HAVE USED FOW_RevealMarker ON THAT MARKER PREVIOUSLY.
--- This function will unreveal the FOW for ALL alive players.
---@param markername string
function FOW_UnRevealMarker(markername) end

--- Returns a list of starting entities a race will currently be allocated.
--- This function will throw an exception if called outside of a Meta Map game, an unknown race is specified or an out of range index is given.
---@param startingEntityNameList table
function MetaMap_GetAttackingRaceStartingEntitiesList(startingEntityNameList) end

--- Returns a list of starting squads a race will currently be allocated.
--- This function will throw an exception if called outside of a Meta Map game, an unknown race is specified or an out of range index is given.
---@param raceName string
---@param startingSquadNameList table
function MetaMap_GetRaceStartingSquadsList(raceName, startingSquadNameList) end

---@param Modifier any
---@param entityid any
function ModifierUtil_IsEntityModifierApplied(Modifier, entityid) end

---@param Modifier any
---@param playerid any
function ModifierUtil_IsPlayerModifierApplied(Modifier, playerid) end

---@param Modifier any
---@param squadid any
function ModifierUtil_IsSquadModifierApplied(Modifier, squadid) end

--- Toggles the fx for the specified modifer
---@param modifier Modifier
function ModifierUtil_ToggleFX(modifier) end

--- Add a new objective using an objective table and associates it with a specific player
--- Example obj_table:
---@param obj_table table
---@param is_primary_obj boolean
---@param player PlayerID
function Objective_AddPlayer(obj_table, is_primary_obj, player) end

--- Associates a ping with an objective
--- The ping will be shown at the specified marker on the mini map in the objectives dialog when the objective it is associated with is selected. Use ping_type 'attack' for attack pings, otherwise default ping will be used.
---@param title_id integer
---@param marker_name string
---@param is_looping boolean
---@param ping_type string
---@return PingID
function Objective_PingMarker(title_id, marker_name, is_looping, ping_type) end

--- Associates a ping with an objective
--- The ping will be shown at the specified position on the mini map in the objectives dialog when the objective it is associated with is selected. Use ping_type 'attack' for attack pings, otherwise default ping will be used.
---@param title_id integer
---@param position Position
---@param is_looping boolean
---@param ping_type string
---@return PingID
function Objective_PingPosition(title_id, position, is_looping, ping_type) end

--- Add a new sub objective using an objective table and associates it with the specified parent objective
--- Please note the only one level of sub objectives is supported (i.e.  an objective cannot be added as a sub objective of an existing sub objective)
---@param obj_table table
---@param parent_id integer
function Objective_SubAdd(obj_table, parent_id) end

--- Sets the max squad cap for a player.
---@param playerId PlayerID
---@param cap integer
function Player_ForceSetMaxSquadCap(playerId, cap) end

--- Sets the max support cap for a player.
---@param playerId PlayerID
---@param cap integer
function Player_ForceSetMaxSupportCap(playerId, cap) end

--- Restrict a list of names.
--- list should contain an array of strings to restrict.
---@param playerID PlayerID
---@param list table
function Player_RestrictSquadList(playerID, list) end

--- Sets the Requisition, Power and Pop for the player. Pass in a 'nil' value instead of a Real amount to skip the associated Resource Type. Ignores income cap and resource sharing.
--- Leaving one Real amount off will automatically skip setting RT_Pop. Leaving two Real amounts off will skip RT_Power and RT_Pop.
---@param playerID PlayerID
---@param requisition number
---@param power number
---@param pop number
function Player_SetAllResources(playerID, requisition, power, pop) end

--- Unrestricts a list of names.
--- List should contain an array of research to restrict.
---@param playerID PlayerID
---@param list table
function Player_UnRestrictResearchList(playerID, list) end

--- Adds a set of tracks to the requested playlist channel, sets how the next track to be played will be chosen, ordered (true) or random (false), and sets silence intervals between tracks for the requested playlist channel.
---@param channel SoundChannel
---@param tracksTable table
---@param sequential boolean
---@param looping boolean
---@param silenceTable table
function Playlist_Manager(channel, tracksTable, sequential, looping, silenceTable) end

---@param race any
function PopulateListForRace(race) end

--- Checks to see if a Rule exists. If it does not exist, the rule will be added to be executed at every 'interval' seconds
---@param functionName function
---@param delay number
function Rule_Check_AddInterval(functionName, delay) end

--- Checks to see if a Rule exists. If it does not exist, the rule will be added to be executed  'calls' times, at every 'interval' seconds
---@param functionName function
---@param delay number
---@param iter integer
function Rule_Check_AddIntervalEx(functionName, delay, iter) end

--- Checks to see if a Rule exists. If it does not exist, the rule will be added to be executed once, after 'interval' seconds
---@param functionName function
---@param delay number
function Rule_Check_AddOneShot(functionName, delay) end

--- Checks to see if a Rule exists. If it does, it will be deleted. If it doesn't there will be no result.
---@param functionName function
function Rule_Check_Remove(functionName) end

--- Adds a leader at the given index to all squads in a group that can take a leader.
--- This function will bypass all cost and queue prereqs
---@param sgroupname string
---@param leaderIndex integer
function SGroup_AddLeadersAtIndex(sgroupname, leaderIndex) end

--- Pass in a string of a sgroup. If the group exists, any squads within that group will be destroyed. If the group does not exist or if the group is empty there will be no result.
--- The sgroupname is not deleted, it will still exist once the members are destroyed
---@param groupName string
function SGroup_Check_DestroyAllSquads(groupName) end

--- Counts the number of leaders in all squads in the specified group
---@param groupName string
---@return integer
function SGroup_CountLeaders(groupName) end

--- Returns the sum of the current healths of all entities in all squads in the group.
---@param groupName string
---@return number
function SGroup_GetTotalHealthCurrent(groupName) end

--- Returns the sum of the maximum healths of all entities in all squads in the group.
---@param groupName string
---@return number
function SGroup_GetTotalHealthMax(groupName) end

--- Returns true if all or any squads in the sgroup are in the hold of any of the buildings in the buildings egroup
--- It is not required that all (or even any) buildings in the buildings group have a hold
---@param groupName string
---@param buildingsGroupName string
---@param all boolean
---@return boolean
function SGroup_IsInHold(groupName, buildingsGroupName, all) end

--- Respawns all squads in a group at a given position
---@param groupname String markerName string
function SGroup_ReSpawnMarker(groupname String markerName) end

--- Respawns the group at a location and triggers the event file at the location of every entity belonging to every squad in the group
--- Example: SGroup_ReSpawnWithEffect("my_group", "mymarker", "data:Art/Events/Unit_Upgrade_Morale_FX/Reinforce_Trooper")
---@param groupName string
---@param markername string
---@param eventFilename string
function SGroup_ReSpawnMarkerWithEffect(groupName, markername, eventFilename) end

--- Respawns all squads in a group at a given position
---@param groupname ScarPosition position string
function SGroup_ReSpawnPosition(groupname ScarPosition position) end

--- Takes the given sgroupname and issues the 'Destroy' command to all squads. It should behave the same as if the player hit the delete key for each member.
--- To absolutely kill squads, this should be used instead of either SGroup_SetAvgHealth(sgroupname, 0) or SGroup_DestroyAllSquads(sgroupname) as both of these do not always accomplish the desired effect
---@param sgroupname string
---@param boolean boolean
function SGroup_SelfDestroy(sgroupname, boolean) end

---@param difficulty any
---@param player any
function SetHealthLevel(difficulty, player) end

--- Hide and remove a counter
---@param counterName string
function UI_HideCount(counterName) end

--- Hide and remove a ratio
---@param ratioName string
function UI_HideRatio(ratioName) end

--- Create a counter with a displayed value. Assign a player and the formatID (aka. Localized String value )
--- Example: UI_ShowCount( "ct_test", g_Player1, 861016, 5 )
---@param counterName string
---@param playerID PlayerID
---@param formatID integer
---@param initialValue number
function UI_ShowCount(counterName, playerID, formatID, initialValue) end

--- Create a counter with a displayed value. Assign a player and the formatID (aka. Localized String value )
--- Example: UI_ShowCount( "ct_test", g_Player1, 861016, 5 )
---@param counterName string
---@param playerID PlayerID
---@param formatID integer
---@param initialValue number
function UI_ShowCountDXP(counterName, playerID, formatID, initialValue) end

--- Update the value of a counter. Pass in the name and the new value for the counter to display
---@param counterName string
---@param countValue number
function UI_ShowCountUpdate(counterName, countValue) end

--- Update the value of a counter. Pass in the name  the new value for the counter to display and the formatID
---@param counterName string
---@param countValue number
---@param formatID integer
function UI_ShowCountUpdateDxp(counterName, countValue, formatID) end

--- DO NOT SCARDOC - PRIVATE RULE
function UI_ShowCountdown1SecRule() end

--- Create a ratio with displayed values. Assign a player and the formatID (aka. Localized String value )
--- Example: UI_ShowRatioDxp( "ratio_test", g_Player1, 38025, 4, 5 )
---@param ratioName string
---@param playerID PlayerID
---@param formatID integer
---@param value1 number
---@param value2 number
function UI_ShowRatio(ratioName, playerID, formatID, value1, value2) end

--- Update the values of a ratio. Pass in the name, the formatID (aka. Localized String value) and the new values for the ratio to display
---@param ratioName string
---@param formatID integer
---@param value1 number
---@param value2 number
function UI_ShowRatioUpdate(ratioName, formatID, value1, value2) end

--- Create and Ability Button
--- Must pass in a table of tga file references and localized string numbers.
---@param textureTable table
---@param tipTable table
---@param boolean boolean
---@return ButtonID
function Util_AbilityButton_Create(textureTable, tipTable, boolean) end

--- Create and Modal Ability Button
--- Must pass in a table of tga file references and localized string numbers.
---@param textureTable any
---@param tipTable table
---@param boolean1 boolean
---@return ButtonID
function Util_AbilityButton_CreateModal(textureTable, tipTable, boolean1) end

--- Gets units to attack the ground at a random position within the radius of a marker
--- Use this function to create controlled random ground attacks to make units appear more lifelike
---@param squadgroupname string
---@param markername string
function Util_AttackGroundMarkerRandomRadius(squadgroupname, markername) end

--- Gets units moving to a random position within the radius of a marker
--- Use this function to create controlled random movement to make units appear more lifelike
---@param squadgroupname string
---@param markername string
function Util_AttackMoveMarkerRandomRadius(squadgroupname, markername) end

--- Forces an update of all buttons in both the SGroup and EGroup button visibility managers
--- Calling this function should not be needed in most cases, and should be avoided because of performance overheads.
function Util_ButtonManagerUpdateAll() end

--- SGROUP - ADD BATTLEGROUP AT MARKER
---@param playerId any
---@param squadgroupName any
---@param armyType any
---@param makerName any
function Util_CreateBattleGroupAtMarker(playerId, squadgroupName, armyType, makerName) end

--- Create an Objective Beacon FX at a marker
---@param marker string
---@return integer
function Util_CreateBeaconMarker_Blue(marker) end

--- Create an Objective Beacon FX at a marker
---@param marker string
---@return integer
function Util_CreateBeaconMarker_Red(marker) end

--- Create an Objective Beacon FX at a position
---@param pos Position
---@return integer
function Util_CreateBeaconPosition_Blue(pos) end

--- Create an Objective Beacon FX at a position
---@param pos Position
---@return integer
function Util_CreateBeaconPosition_Red(pos) end

--- Non-disrupting version of CreatSquadsAndDropPodIn.  Creates a given number of squads with a loadout at a position and adds them to a squadgroup.  The squads are then dropped in a droppod that has no disruption.
--- Use loadout of zero to create a squad at its min loadout.  Squads can never exceed their max loadout.
---@param playerId PlayerID
---@param squadgroupName string
---@param squadBlueprint string
---@param pos Position
---@param numsquads integer
---@param loadout integer
---@return SGroupID
function Util_CreateSquadsAndDropPodIn2(playerId, squadgroupName, squadBlueprint, pos, numsquads, loadout) end

--- Drop-pod an existing squadgroup to a position on the map with no disruption
--- Works on spawned or depspawned group
---@param sgroupname string
---@param pos Position
function Util_DropPod2(sgroupname, pos) end

--- Add an Ability Button that you want on an EGROUP to the Visibility Manager
--- The function Util_EGroupAbilityButton_ManageVisible() must be added after this function is called for anything to happen.
---@param buttonID ButtonID
---@param egroupName string
function Util_EGroupAbilityButton_Add2Manager(buttonID, egroupName) end

function Util_EGroupAbilityButton_ManageVisible() end

--- Remove an Ability Button from the EGroup Visibility Manager
--- This will remove the button from the manager and hide it if it is visible.
---@param buttonID ButtonID
function Util_EGroupAbilityButton_Remove(buttonID) end

--- Plays the non looping piece of music once through, then, after delay, plays the looping piece of music, looping.
--- Use this function to setup detailed playlists in missions.
---@param musictable_non table
---@param musictable_loop table
---@param delay integer
function Util_Music_CreateSingleLoop(musictable_non, musictable_loop, delay) end

--- Toggles all modifier event FX for the player
--- Use this function to make NIS's look clean without wierd icons above units' heads
---@param titleid integer
function Util_NIS_ToggleModifierEvents(titleid) end

---@param playerId any
---@param tableName any
---@param mkr1 any
---@param mkr2 any
function Util_NearMkrGoMkr(playerId, tableName, mkr1, mkr2) end

--- Add an Ability Button that you want in the No Selection Visibility Manager
--- The function Util_NoSelectionAbilityButton_ManageVisible() must be added after this function is called for anything to happen.
---@param buttonID ButtonID
function Util_NoSelectionAbilityButton_Add2Manager(buttonID) end

--- Manages the visibility of Ability Buttons conditional on the user having NO selection made.
--- This function should be added once with Rule_Add() or Rule_AddDelay() in order to avoid lag between having no selection and the button appearing.
function Util_NoSelectionAbilityButton_ManageVisible() end

--- Remove an Ability Button from the No Selection Visibility Manager
--- This will remove the button from the manager and hide it if it is visible.
---@param buttonID ButtonID
function Util_NoSelectionAbilityButton_Remove(buttonID) end

--- Create a looping ping.
--- Works with the Util_Ping_Stop( ) to allow for the easy removal of looping pings without having to save pingID variables. Make up a pingname string and pass it in.
---@param pingname string
---@param pos string
function Util_Ping_LoopingPos(pingname, pos) end

--- Removes an existing Objective Beacon - colour doesn't matter (*snicker*)
---@param beaconID integer
function Util_RemoveBeaconMarker(beaconID) end

--- Add an Ability Button that you want on an SGROUP to the Visibility Manager
--- The function Util_SGroupAbilityButton_ManageVisible() must be added after this function is called for anything to happen.
---@param buttonID ButtonID
---@param sgroupName string
function Util_SGroupAbilityButton_Add2Manager(buttonID, sgroupName) end

function Util_SGroupAbilityButton_ManageVisible() end

--- Remove an Ability Button from the SGroup Visibility Manager
--- This will remove the button from the manager and hide it if it is visible.
---@param buttonID ButtonID
function Util_SGroupAbilityButton_Remove(buttonID) end

--- this function orders the squad to move to a location, but to take its time
---@param sgroup any
---@param finalMarker any
function Util_SGroupAmble(sgroup, finalMarker) end

--- Create and Scar Button
--- Must pass in a valid widget name and table of tga file references and localized string numbers.
---@param widgetName string
---@param textureTable any
---@param tipTable table
---@param boolean1 boolean
---@param boolean2 boolean
---@return ButtonID
function Util_ScarButton_Create(widgetName, textureTable, tipTable, boolean1, boolean2) end

--- Sets the blood level for every blood pit in the group
--- The level must be in the [0.0, 1.0] range
---@param pitGroupName string
---@param level number
function Util_SetBloodPitLevel(pitGroupName, level) end

--- Creates an objective and plays the objective created stinger to accompany it, and flashes the objective button.
--- Use this function to create an objective with the sound
---@param objectiveTable table
---@param primary boolean
---@param playerID integer
function Util_WXP_ObjectiveCreate(objectiveTable, primary, playerID) end

--- Creates a sub objective and plays the objective created stinger to accompany it, and flashes the objective button. Pass in the objective ID of the 'parent' objective
--- Use this function to create an objective with the sound
---@param objectiveTable table
---@param parent_id integer
function Util_WXP_ObjectiveSubCreate(objectiveTable, parent_id) end

--- adds the WCUtil_Rule_CheckTotalAnnihalte if it doesnt already exist
function WCUtil_AddCheckTotalAnnhilate() end

--- MUST be called every second
---@param TeamTable any
---@param SettingsTable any
function WCUtil_CheckStrategicPoints(TeamTable, SettingsTable) end

--- returns filled out team table
function WCUtil_InitTeamTable() end

--- checks all player players and kills any that are totally annihilated
function WCUtil_Rule_CheckTotalAnnihilate() end

--- Adds all squads belonging to all players into a group. Removes any from the group that are despawned and destroys whatever is left.
--- Should only be used when cleaning up, as this will destroy EVERY spawned squad in the entire game.
function WXP_KillAllPlayersSpawnedSquads() end

--- Adds all squads belonging to all players into a group. Removes any from the group that are despawned or in the indicated sgroup and destroys whatever is left.
--- Should only be used when cleaning up, as this will destroy EVERY spawned squad in the entire game... except the one that you ask it not to.
---@param sgroupname string
function WXP_KillAllPlayersSpawnedSquadsExcept(sgroupname) end

--- Turns off event cues, sets the viewable space to faded out, and presets the UI into letterbox mode
function WXP_OpeningNISPreset() end

--- Applies restrictions for space marines.  You should call this at the begining of your mission for Player 1.
---@param level integer
function WXP_Restrict(level) end

