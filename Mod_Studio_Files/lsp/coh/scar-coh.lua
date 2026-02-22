---@meta scar-coh

-- Auto-generated from scardoc_coh.dat by scardoc-to-luadefs.py
-- 1386 functions, 0 constants
-- Do not edit manually — regenerate with: python tools/scardoc-to-luadefs.py

--- Clears all the resources for each budget and recalculate their values
--- RETURNS: Void
---
---@param pPlayer PlayerID
function AI_ClearAllBudgetTotals(pPlayer) end

--- Clears all the budget weights that have been previously added
--- RETURNS: Void
---
---@param pPlayer PlayerID
function AI_ClearAllBudgetWeight(pPlayer) end

--- Clear the given budget's account of all of its resources.
--- RETURNS: Void
---
---@param pPlayer PlayerID
---@param budgetname string
function AI_ClearBudgetTotal(pPlayer, budgetname) end

--- Clear the build demand table
--- RETURNS: Void
---
---@param pPlayer PlayerID
function AI_ClearBuildDemand(pPlayer) end

--- Clear all class preferences that have been set up both class and anti-class
--- RETURNS: Void
---
---@param pPlayer PlayerID
function AI_ClearClassPreference(pPlayer) end

--- This clears the importance override on this military point
--- RETURNS: Void
---
---@param pPlayer PlayerID
---@param pEntity EntityID
function AI_ClearImportance(pPlayer, pEntity) end

--- Clears all PBG limits that were previously set
--- RETURNS: Void
---
---@param pPlayer PlayerID
function AI_ClearProductionLimits(pPlayer) end

--- Clears the attack request for this player. This player will now attack any enemies. 
--- RETURNS: Void
---
---@param pPlayer PlayerID
function AI_ClearTargetPlayer(pPlayer) end

--- Run this string in the AI of player
--- RETURNS: Void
---
---@param pPlayer PlayerID
---@param s string
function AI_DoString(pPlayer, s) end

--- Enables/Disables an AI player
--- RETURNS: Void
---
---@param pPlayer PlayerID
---@param enable boolean
function AI_Enable(pPlayer, enable) end

--- Enables/Disables all AI players
--- RETURNS: Void
---
---@param enable boolean
function AI_EnableAll(enable) end

--- Enables/disables a component of the AI
--- RETURNS: Void
---
---@param pPlayer PlayerID
---@param enable boolean
---@param ct integer
function AI_EnableComponent(pPlayer, enable, ct) end

--- Tells the AI to take all of its units and forces them to attack the given player immediately
--- RETURNS: Void
---
---@param pPlayer PlayerID
---@param pPlayerTarget PlayerID
function AI_ForceAttack(pPlayer, pPlayerTarget) end

--- create the tables of units to spawn
---
---Located in FILE: \scar\screensaver.scar LINE: 1411
function AI_InitializeSpawnTable() end

--- Returns true if player is an AI player
--- RETURNS: Boolean
---
---@param pPlayer PlayerID
function AI_IsAIPlayer(pPlayer) end

--- Returns true if player is a AIPlayer and is enabled
--- RETURNS: Boolean
---
---@param pPlayer PlayerID
function AI_IsEnabled(pPlayer) end

--- Locks the entity and disables its tactics (if any) and the AI will no longer use this object
--- RETURNS: Void
---
---@param pPlayer PlayerID
---@param pEntity EntityID
function AI_LockEntity(pPlayer, pEntity) end

--- Lock an SGroup from the ai_player
--- RETURNS: Void
---
---@param ai_player PlayerID
---@param sgroup SGroup
function AI_LockSGroup(ai_player, sgroup) end

--- Locks the squad and disables its tactics (if any) and the AI will no longer use this object
--- RETURNS: Void
---
---@param pPlayer PlayerID
---@param pSquad SquadID
function AI_LockSquad(pPlayer, pSquad) end

--- Offset the anticlass preference to increase the demand for certain unit anticlasses
--- RETURNS: Void
---
---@param pPlayer PlayerID
---@param className string
---@param increment number
function AI_SetAntiClassPreference(pPlayer, className, increment) end

--- Sets the given budget to the given weight
--- RETURNS: Void
---
---@param pPlayer PlayerID
---@param budgetName string
---@param weight number
function AI_SetBudgetWeight(pPlayer, budgetName, weight) end

--- Sets the demand for this squadasd
--- RETURNS: Void
---
---@param pPlayer PlayerID
---@param pbgID integer
---@param start number
---@param increment number
function AI_SetBuildDemand(pPlayer, pbgID, start, increment) end

--- Sets the demand for this squadasd
--- RETURNS: Void
---
---@param pPlayer PlayerID
---@param pbg PropertyBagGroup
---@param start number
---@param increment number
function AI_SetBuildDemandPBG(pPlayer, pbg, start, increment) end

--- Set a build order table for the ai_player
--- RETURNS: Void
---
---@param ai_player PlayerID
---@param build_table table
function AI_SetBuildTable(ai_player, build_table) end

--- Offset the class preference to increase the demand for certain unit class
--- RETURNS: Void
---
---@param pPlayer PlayerID
---@param className string
---@param increment number
function AI_SetClassPreference(pPlayer, className, increment) end

--- Set the difficulty level of this AI player
--- RETURNS: Void
---
---@param pPlayer PlayerID
---@param difficultyLevel integer
function AI_SetDifficulty(pPlayer, difficultyLevel) end

--- This overrides the default importance of the given military point
--- RETURNS: Void
---
---@param pPlayer PlayerID
---@param pEntity EntityID
---@param importance number
function AI_SetImportance(pPlayer, pEntity, importance) end

--- Set limits to the production items that an AI can build
--- RETURNS: Void
---
---@param ai_player PlayerID
---@param limits_table table
function AI_SetProductionLimitsTable(ai_player, limits_table) end

--- Sets a limit to a PBG type (buildings, squads, upgrades, etc)
--- RETURNS: Void
---
---@param pPlayer PlayerID
---@param pbgtype integer
---@param pbgID integer
---@param limit integer
function AI_SetProduductionLimits(pPlayer, pbgtype, pbgID, limit) end

--- Tells the AI to attack only the given player.
--- RETURNS: Void
---
---@param pPlayer PlayerID
---@param playerID integer
function AI_SetTargetPlayer(pPlayer, playerID) end

--- Set the victory point percentage - how much should we prefer victory points over resource points? Percentage should be between 0.0 and 1.0. 1.0 means 100 percent. 0.5 means choose equally between victory points and other kinds of points.
--- RETURNS: Void
---
---@param pPlayer PlayerID
---@param percentage number
function AI_SetVictoryPointPercentage(pPlayer, percentage) end

---
---Located in FILE: \scar\screensaver.scar LINE: 1322
function AI_Setup() end

--- 
--- RETURNS: Void
---
---@param pPlayer PlayerID
function AI_UnlockAll(pPlayer) end

--- Unlocks this entity so that AI can use it again
--- RETURNS: Void
---
---@param pPlayer PlayerID
---@param pEntity EntityID
function AI_UnlockEntity(pPlayer, pEntity) end

--- Unlock an SGroup from the ai_player
--- RETURNS: Void
---
---@param ai_player PlayerID
---@param sgroup SGroup
function AI_UnlockSGroup(ai_player, sgroup) end

--- Unlocks the given squad so the AI can use it again
--- RETURNS: Void
---
---@param pPlayer PlayerID
---@param pSquad SquadID
function AI_UnlockSquad(pPlayer, pSquad) end

--- Re-updates the AI in regards to all the static objects in the world (if SCAR creates new strategic points dynamically this will need to be called)
--- RETURNS: Void
---
---@param pPlayer PlayerID
function AI_UpdateStatics(pPlayer) end

--- Clear ties between an actor and any units
---
---@param actor ActorTable
function Actor_Clear(actor) end

---
---Located in FILE: \scar\actor.scar LINE: 10
function Actor_Init() end

--- Plays a speech event for a given actor WITH a portrait and subtitle
--- RETURNS: Void
---
---@param actor ActorTable
---@param locID[ integer
---@param continueButton boolean
---@param stickySubtitle boolean
---@param blockInput] boolean
function Actor_PlaySpeech(actor, locID[, continueButton, stickySubtitle, blockInput]) end

---
---Located in FILE: \scar\actor.scar LINE: 58
---@param portrait any
---@param actor any
---@param locID any
---@param continueButton any
---@param stickySubtitle any
---@param blockInput any
function Actor_PlaySpeechInternal(portrait, actor, locID, continueButton, stickySubtitle, blockInput) end

--- Plays a speech event for a given actor WITHOUT a portrait or subtitle. See Actor_PlaySpeech for more details
--- RETURNS: Void
---
---@param actor ActorTable
---@param locID[ integer
---@param continueButton boolean
---@param stickySubtitle boolean
---@param blockInput] boolean
function Actor_PlaySpeechWithoutPortrait(actor, locID[, continueButton, stickySubtitle, blockInput]) end

--- Tie an entire sgroup to an actor, so audio comes from a squad member
--- RETURNS: Void
---
---@param actor ActorTable
---@param sgroup SGroupID
function Actor_SetFromSGroup(actor, sgroup) end

--- Tie a single squad to an actor, so audio comes from a squad member
--- RETURNS: Void
---
---@param actor ActorTable
---@param squad SquadID
function Actor_SetFromSquad(actor, squad) end

--- Plays an animation from an entity.
--- RETURNS: Void
---
---@param entity EntityID
---@param animname string
function Anim_PlayEntityAnim(entity, animname) end

--- Sets an sgroup to charge a position when the player steps into the trigger zone
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param triggerarea[ MarkerID/Position/EGroupID
---@param triggerrange] number
function AutoCharge_AddSGroup(sgroup, triggerarea[, triggerrange]) end

---
---Located in FILE: \scar\designerlib.scar LINE: 631
function AutoCharge_Manager() end

--- Stops monitoring all squads from the auto-charge functions
--- RETURNS: Void
---
function AutoCharge_RemoveAll() end

--- Removes a squad from being monitored by the auto-charge functions
--- RETURNS: Void
---
---@param sgroup SGroupID
function AutoCharge_RemoveSGroup(sgroup) end

--- Toggles all cinematic related settings. true = go to cinematic, false = go back to normal
--- RETURNS: Void
---
---@param in/out boolean
---@param seconds number
function AutoCinematic(in/out, seconds) end

---
---Located in FILE: \scar\designerlib.scar LINE: 250
---@param sgroup any
---@param originpos any
function AutoReinforce_AddSGroup(sgroup, originpos) end

---
---Located in FILE: \scar\designerlib.scar LINE: 326
function AutoReinforce_Manager() end

--- Stops monitoring all squads from the auto-reinforce functions
--- RETURNS: Void
---
function AutoReinforce_RemoveAll() end

--- Removes an SGroup from the auto-reinforce functions
--- RETURNS: Void
---
---@param sgroup SGroupID
function AutoReinforce_RemoveSGroup(sgroup) end

--- Sets an sgroup to retreat to the given destination or building once pinned for a certain duration, or reduced to a third of it's original size
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param destination[ MarkerID/Position/EGroupID
---@param threshold] number
function AutoRetreat_AddSGroup(sgroup, destination[, threshold]) end

---
---Located in FILE: \scar\designerlib.scar LINE: 474
function AutoRetreat_Manager() end

--- Stops monitoring all squads from the auto-retreat functions
--- RETURNS: Void
---
function AutoRetreat_RemoveAll() end

--- Removes a squad from being monitored by the auto-retreat functions
--- RETURNS: Void
---
---@param sgroup SGroupID
function AutoRetreat_RemoveSGroup(sgroup) end

---
---Located in FILE: \scar\designerlib.scar LINE: 524
---@param me any
function AutoRetreat_Retreat(me) end

--- Adds a territory to the automanager. You specify the territory by passing in an EGroup containing the flag.
--- RETURNS: Void
---
---@param egroup[ EGroupID
---@param egroup2 EGroupID
---@param egroup3...] EGroupID
function AutoTerritory_AddEGroup(egroup[, egroup2, egroup3...]) end

---
---Located in FILE: \scar\designerlib.scar LINE: 896
function AutoTerritory_Manager() end

--- Removes all territories from the automanager and shuts the system down.
--- RETURNS: Void
---
function AutoTerritory_RemoveAll() end

--- Removes a territory from the automanager. You specify the territory by passing in an EGroup containing the flag.
--- RETURNS: Void
---
---@param egroup[ EGroupID
---@param egroup2 EGroupID
---@param egroup3...] EGroupID
function AutoTerritory_RemoveEGroup(egroup[, egroup2, egroup3...]) end

---
---Located in FILE: \scar\battle_simulator.scar LINE: 196
function BATTLE_IsDone() end

--- start a new battle
---
---Located in FILE: \scar\battle_simulator.scar LINE: 71
function BATTLE_Loop() end

---
---Located in FILE: \scar\battle_simulator.scar LINE: 151
function BATTLE_Over() end

--- e.g. "BattleScripts/dobattle.lua"
---
--- the time the current battle started at
--- save the current battle number
---
---Located in FILE: \scar\battle_simulator.scar LINE: 31
---@param battescript any
function BATTLE_Start(battescript) end

--- Creates the maximum number of squads of this loadout given the resources at a marker position and adds them to a squadgroup.  The squadgroup will be created if it doesnt exist.
--- RETURNS: SGroupID
---
---@param playerId PlayerID
---@param squadgroupName string
---@param squadBlueprint string
---@param load_out_table table
---@param markername string
---@param requisition_amount number
---@param power_amount number
function Balance_CreateSquadsAtMarkerByResource(playerId, squadgroupName, squadBlueprint, load_out_table, markername, requisition_amount, power_amount) end

--- Set the Lua Function used to initialize each Balance Tool iteration. 
--- RETURNS: Void
---
---@param func function
function Balance_SetInit(func) end

--- write all battle simulation result
--- RETURNS: Void
---
---@param Void any
function BattleSim_DumpResults(Void) end

--- start battle simulation screen. 
--- RETURNS: Void
---
---@param Void any
function BattleSim_StartBattleSim(Void) end

--- record one sentence of battle sim result
--- RETURNS: Void
---
---@param char *s const
function BattleSim_WriteOneResult(char *s) end

--- Procedurally rotates the camera around a target. Camera input will be disabled. distance is in meters, declination is in degrees, speed is in degrees/sec.
--- RETURNS: Void
---
---@param target Position
---@param distance number
---@param declination number
---@param speed number
function Camera_AutoRotate(target, distance, declination, speed) end

--- Clamps the camera's target position to a marker
--- RETURNS: Void
---
---@param marker MarkerID
function Camera_ClampToMarker(marker) end

--- Print current camera settings.
--- RETURNS: Void
---
---@param void any
function Camera_Dump(void) end

--- Camera would move to the specified position. bPan is a boolean flag that pans the camera if set to true
--- RETURNS: Void
---
---@param pos Position
---@param bPan boolean
function Camera_FocusOnPosition(pos, bPan) end

---
---Located in FILE: \scar\camera.scar LINE: 11
---@param marker any
---@param secs any
function Camera_FocusOnTargetMarker(marker, secs) end

---
---Located in FILE: \scar\camera.scar LINE: 15
---@param marker any
function Camera_FocusOnTargetMarkerIfClose(marker) end

--- Focus the Camera on the specificed position
--- RETURNS: Void
---
---@param targetpos Position
function Camera_FocusOnTargetPos(targetpos) end

--- Set the camera to follow the first squad in an SGroup.
--- RETURNS: Void
---
---@param sgroup SGroupID
function Camera_FollowSGroup(sgroup) end

--- Camera would follow current selection
--- RETURNS: Void
---
---@param Void any
function Camera_FollowSelection(Void) end

--- Camera would follow specified squad
--- RETURNS: Void
---
---@param pSquad SquadID
function Camera_FollowSquad(pSquad) end

--- Get the current camera declination (tilt)
--- RETURNS: Real
---
---@param Void any
function Camera_GetDeclination(Void) end

--- Get the current camera orbit (rotation)
--- RETURNS: Real
---
---@param Void any
function Camera_GetOrbit(Void) end

--- Get the current target position for the camera.
--- RETURNS: Math::Vector3f
---
---@param Void any
function Camera_GetTargetPos(Void) end

--- Get values: TV_SlideTargetRate, TV_SlideTargetBase, TV_SlideTargetThreshold
--- RETURNS: Real
---
---@param tuningValue integer
function Camera_GetTuningValue(tuningValue) end

--- Get the current zoom distance for the camera.
--- RETURNS: Real
---
---@param Void any
function Camera_GetZoomDist(Void) end

--- Returns whether the camera distance, orbit and declination are close to default values
--- RETURNS: Boolean
---
---@param Void any
function Camera_IsDefault(Void) end

--- Returns the enabled/disabled state of the camera input.
--- RETURNS: Boolean
---
---@param Void any
function Camera_IsInputEnabled(Void) end

--- Move the camera to an EGroup's location. The optional pan flag should be true if you want the camera to glide to the new location. You can specify a pan rate to use (use SLOW_CAMERA_PANNING for a default slower rate)
--- RETURNS: Void
---
---@param egroup[ EGroupID
---@param pan boolean
---@param panRate] number
function Camera_MoveToEGroup(egroup[, pan, panRate]) end

--- Move the camera to desired location. The optional pan flag should be true if you want the camera to glide to the new location. You can specify a pan rate to use (use SLOW_CAMERA_PANNING for a default slower rate)
--- RETURNS: Void
---
---@param position[ MarkerID/Pos
---@param pan boolean
---@param panRate] number
function Camera_MoveToPosition(position[, pan, panRate]) end

--- Slightly refocus the camera to rest on a position if it's close by.
--- RETURNS: Void
---
---@param position MarkerID/Pos
function Camera_MoveToPositionIfClose(position) end

--- Move the camera to an SGroup's location. The optional pan flag should be true if you want the camera to glide to the new location. You can specify a pan rate to use (use SLOW_CAMERA_PANNING for a default slower rate)
--- RETURNS: Void
---
---@param sgroup[ SGroupID
---@param pan boolean
---@param panRate] number
function Camera_MoveToSGroup(sgroup[, pan, panRate]) end

--- Reload all cameras' tuning values
--- RETURNS: Void
---
---@param Void any
function Camera_Reload(Void) end

--- Reset camera position to default home position
--- RETURNS: Void
---
---@param Void any
function Camera_ResetToDefault(Void) end

--- Set the current camera declination (tilt)
--- RETURNS: Void
---
---@param dec number
function Camera_SetDeclination(dec) end

--- Helper function to set the default camera parameters
--- RETURNS: Void
---
---@param height number
---@param declination number
---@param angle number
function Camera_SetDefault(height, declination, angle) end

--- Enables / Disables camera input.
--- RETURNS: Void
---
---@param enabled boolean
function Camera_SetInputEnabled(enabled) end

--- Set the current camera orbit (rotation)
--- RETURNS: Void
---
---@param orbit number
function Camera_SetOrbit(orbit) end

--- Set values: TV_SlideTargetRate, TV_SlideTargetBase, TV_SlideTargetThreshold
--- RETURNS: Void
---
---@param tuningValue integer
---@param value number
function Camera_SetTuningValue(tuningValue, value) end

--- Set the current zoom distance for the camera.
--- RETURNS: Void
---
---@param distance number
function Camera_SetZoomDist(distance) end

--- Stops the camera from procedurally rotating. Input is restored.
--- RETURNS: Void
---
---@param Void any
function Camera_StopAutoRotating(Void) end

--- Frees up the camera (so it's not clamped to a marker anymore)
--- RETURNS: Void
---
---@param Void any
function Camera_Unclamp(Void) end

--- Stops an SGroup from auto-targetting, until one of their members is explicity given an attack order or Ceasefire_RemoveSGroup() is called (at which point they all start firing again)
--- RETURNS: Void
---
---@param sgroup[ SGroupID
---@param function] function
function Ceasefire_AddSGroup(sgroup[, function]) end

--- internal function that manages the ceasefires
---
---Located in FILE: \scar\designerlib.scar LINE: 745
function Ceasefire_Manager() end

--- Removes the ceasefire effect from an sgroup. This may already have been removed by issuing an attack order directly to the group.
--- RETURNS: Void
---
---@param sgroup SGroupID
function Ceasefire_RemoveSGroup(sgroup) end

--- Order a squad group to abandon their current team weapon if they have it and they could (tuning value in attribute editor)
--- RETURNS: 
---
---@param sgroupid SGroupID
---@param queued? boolean
function Cmd_AbandonTeamWeapon(sgroupid, queued) end

--- Attach the squad from sgroupnameAttachee to sgroupname.  Both SGroups must contain only one squad.
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param sgroupAttachee SGroupID
function Cmd_AttachSquads(sgroup, sgroupAttachee) end

--- Order a squad group to attack another squad group.  This function should track the target squad groups position. 
--- RETURNS: Void
---
---@param sourcesgroup SGroupID
---@param targetegroup EGroupID
---@param queued? boolean
function Cmd_AttackEGroup(sourcesgroup, targetegroup, queued) end

--- Order a squad group to attack another squad group.  This function should track the target entity groups position. 
--- RETURNS: Void
---
---@param sourcesgroup SGroupID
---@param targetegroup EGroupID
---@param plan string
---@param queued? boolean
function Cmd_AttackEGroupWithPlan(sourcesgroup, targetegroup, plan, queued) end

--- Order a squad group to ground move to an egroup's position. 
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param targetegroup EGroupID
function Cmd_AttackMoveEGroup(sgroup, targetegroup) end

--- Order a squad group to ground move to an egroup's position only after other queued commands are done. 
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param targetegroup EGroupID
function Cmd_AttackMoveEGroupQueued(sgroup, targetegroup) end

--- Order a squad group to ground move to a marker position. 
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param marker MarkerID
function Cmd_AttackMoveMarker(sgroup, marker) end

--- Order a squad group to ground move to a marker position only after other queued commands are done. 
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param marker MarkerID
function Cmd_AttackMoveMarkerQueued(sgroup, marker) end

--- Order a squad group to attack move to a position. 
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param targetposition Position
function Cmd_AttackMovePos(sgroup, targetposition) end

--- Order a squad group to attack move to a position only after other queued commands are done.
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param targetposition Position
function Cmd_AttackMovePosQueued(sgroup, targetposition) end

--- Order a squad group to attack move with custom plan to a position only after other queued commands are done.
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param targetposition Position
---@param plan string
function Cmd_AttackMovePosQueuedWithPlan(sgroup, targetposition, plan) end

--- Order a squad group to attack move with custom plan to a position. 
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param targetposition Position
---@param plan string
function Cmd_AttackMovePosWithPlan(sgroup, targetposition, plan) end

--- Order a squad group to ground move to a sgroup's position. 
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param targetsgroup SGroupID
function Cmd_AttackMoveSGroup(sgroup, targetsgroup) end

--- Order a squad group to ground move to a sgroup's position only after other queued commands are done. 
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param targetsgroup SGroupID
function Cmd_AttackMoveSGroupQueued(sgroup, targetsgroup) end

--- Command attacker sgroup to attack move to strategic point target; when it is capturable, the sgroup would capture it
--- RETURNS: Void
---
---@param attacker SGroupID
---@param target EGroupID
---@param !Boolean!queued? any
function Cmd_AttackMoveThenCapture(attacker, target, !Boolean!queued) end

--- Order a squad group to attack move to cover that's close to a position. 
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param targetposition Position
---@param searchRadius number
function Cmd_AttackMoveToCover(sgroup, targetposition, searchRadius) end

--- Order a squad group to attack move to cover that's close to a position, only after other queued commands are done. 
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param targetposition Position
---@param searchRadius number
function Cmd_AttackMoveToCoverQueued(sgroup, targetposition, searchRadius) end

--- Order a squad group to attack another squad group.  This function should track the target squad groups position. 
--- RETURNS: Void
---
---@param sourcesgroup SGroupID
---@param targetsgroup SGroupID
---@param queued? boolean
function Cmd_AttackSGroup(sourcesgroup, targetsgroup, queued) end

--- Order a squad group to attack another squad group.  This function should track the target squad groups position. 
--- RETURNS: Void
---
---@param sourcesgroup SGroupID
---@param targetsgroup SGroupID
---@param plan string
---@param queued? boolean
function Cmd_AttackSGroupWithPlan(sourcesgroup, targetsgroup, plan, queued) end

--- Order a squad group to capture team weapon entity group.
--- RETURNS: 
---
---@param sgroupid SGroupID
---@param targetid EGroupID
---@param queued? boolean
function Cmd_CaptureTeamWeapon(sgroupid, targetid, queued) end

--- Order a squad group to dig in the first position and facing the second position
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param targetposition Position
---@param facingposition Position
function Cmd_DigIn(sgroup, targetposition, facingposition) end

--- Order a squad group to dig out if they are already dug in
--- RETURNS: Void
---
---@param sgroup SGroupID
function Cmd_DigOut(sgroup) end

--- Order a squad group to execute default reaction squad AI plan
--- RETURNS: Void
---
---@param sgroupid SGroupID
---@param planfile string
---@param !Boolean!queued? any
function Cmd_DoCustomPlan(sgroupid, planfile, !Boolean!queued) end

--- Order a squad group to execute default reaction squad AI plan towards a position
--- RETURNS: Void
---
---@param sgroupid SGroupID
---@param planfile string
---@param pos Pos
---@param !Boolean!queued? any
function Cmd_DoCustomPlanTowardsPos(sgroupid, planfile, pos, !Boolean!queued) end

--- Order a squad group to execute default reaction squad AI plan
--- RETURNS: Void
---
---@param sgroupid SGroupID
---@param !Boolean!queued? any
function Cmd_DoDefaultPlan(sgroupid, !Boolean!queued) end

--- Order a squad group to execute default reaction squad AI plan towards a position
--- RETURNS: Void
---
---@param sgroupid SGroupID
---@param pos Pos
---@param !Boolean!queued? any
function Cmd_DoDefaultPlanTowardsPos(sgroupid, pos, !Boolean!queued) end

--- apply critical hit to the entities
--- RETURNS: Void
---
---@param playerid PlayerID
---@param sgroupid SGroupID
---@param criticalid CriticalID
---@param damageid DamageID
function Cmd_EGroupCriticalHit(playerid, sgroupid, criticalid, damageid) end

--- Sends an upgrade command to all entities in a group.  upgradeid should be the number returned by Util_GetUpgradeID( upgradename )
--- RETURNS: Void
---
---@param egroup EGroupID
---@param upgradeid[ UpgradeID
---@param count] integer
function Cmd_EntityUpgrade(egroup, upgradeid[, count]) end

--- Order a squad group to load at a random entity of the egroup
--- RETURNS: EntityID id of the destination entity
---
---@param fromsgroupid SGroupID
---@param toegroupid EGroupID
---@param overload [Bool
---@param queued] boolean
function Cmd_Garrison(fromsgroupid, toegroupid, overload, queued]) end

--- Order a squad group to dig in instantly at the current position and facing
--- RETURNS: Void
---
---@param sgroup SGroupID
function Cmd_InstantDigIn(sgroup) end

--- Order a squad group to dig out instantly
--- RETURNS: Void
---
---@param sgroup SGroupID
function Cmd_InstantDigOut(sgroup) end

--- Sends an instant upgrade command to all entities in a group.  upgradeid should be the number returned by Util_GetUpgradeID( upgradename )
--- RETURNS: Void
---
---@param egroup EGroupID
---@param upgradeid[ UpgradeID
---@param count] integer
function Cmd_InstantEntityUpgrade(egroup, upgradeid[, count]) end

--- Order a squad group to garrison instantly at an random entity (mostly buildings) of the entity group
--- RETURNS: EntityID id of the destination entity
---
---@param fromsgroupid SGroupID
---@param toegroupid EGroupID
---@param overload [Bool
---@param queued] boolean
function Cmd_InstantGarrison(fromsgroupid, toegroupid, overload, queued]) end

--- Order a squad group to load instantly at an random squad of the second squad group
--- RETURNS: SquadID id of the destination squad
---
---@param fromsgroupid SGroupID
---@param tosgroupid SGroupID
---@param overload [Bool
---@param queued] boolean
function Cmd_InstantLoad(fromsgroupid, tosgroupid, overload, queued]) end

--- Sends an instant upgrade command to the player.  upgradeid should be the number returned by Util_GetUpgradeID( upgradename )
--- RETURNS: Void
---
---@param playerid PlayerID
---@param upgradeid[ UpgradeID
---@param count] integer
function Cmd_InstantPlayerUpgrade(playerid, upgradeid[, count]) end

--- Sends a instant reinforce command to all squads in a group.  count represents the number of commands to send.
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param count integer
function Cmd_InstantReinforceUnit(sgroup, count) end

--- Sends a instant reinforce command to all squads in a group.  count represents the number of commands to send. spawnlocation is where the reinforced unit will spawn. You can optionally find a hidden position by specifying a checktype (CHECK_OFFCAMERA, CHECK_IN_FOW or CHECK_BOTH), and what to do if a hidden position can't be found (SPAWN_ATMARKER, SPAWN_ATSQUAD, or DO_NOTHING) - SPAWN_ATMARKER is the default.
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param count integer
---@param spawnlocation[ MarkerID/Pos
---@param checktype[ integer
---@param failtype]] integer
function Cmd_InstantReinforceUnitPos(sgroup, count, spawnlocation[, checktype[, failtype]]) end

--- Order a squad group to instant setup their team weapon
--- RETURNS: 
---
---@param sgroupid SGroupID
---@param queued? boolean
function Cmd_InstantSetupTeamWeapon(sgroupid, queued) end

--- Sends an instant upgrade command to all squads in a group.  upgradeid should be the number returned by Util_GetUpgradeID( upgradename )
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param upgradeid[ UpgradeID
---@param count] integer
function Cmd_InstantSquadUpgrade(sgroup, upgradeid[, count]) end

--- Returns true if ALL or ANY squads in a group are currently capturing something.
--- RETURNS: Boolean
---
---@param sgroup SGroupID
---@param all boolean
function Cmd_IsCapturing(sgroup, all) end

--- Returns true if ALL or ANY squads in a group are currently constructing a building.
--- RETURNS: Boolean
---
---@param sgroup SGroup
---@param all boolean
function Cmd_IsConstructingBuilding(sgroup, all) end

--- Returns true if ALL or ANY entities in a group are currently producing squads
--- RETURNS: Boolean
---
---@param egroup EGroupID
---@param all boolean
function Cmd_IsProducingSquads(egroup, all) end

--- Returns true if ALL or ANY squads in a group are currently calling for reinforcments.
--- RETURNS: Boolean
---
---@param sgroup SGroupID
---@param all boolean
function Cmd_IsReinforcing(sgroup, all) end

--- Returns true if ALL or ANY squads in a group are currently upgrading. You can pass in nil for upgradeid if it doesn't matter what is being upgraded.
--- RETURNS: Boolean
---
---@param sgroup SGroupID
---@param upgradeid UpgradeID
---@param all boolean
function Cmd_IsUpgrading(sgroup, upgradeid, all) end

--- Order a squad group to load at an random squad of the second squad group
--- RETURNS: SquadID id of the destination squad
---
---@param fromsgroupid SGroupID
---@param tosgroupid SGroupID
---@param overload [Bool
---@param queued] boolean
function Cmd_Load(fromsgroupid, tosgroupid, overload, queued]) end

--- Move a squad group out of a position to a certain radius
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param position Pos
---@param radius integer
---@param queued? boolean
function Cmd_MoveAwayFromPos(sgroup, position, radius, queued) end

--- Moves a squad group to the closest marker in a list/table of MarkerIDs.
--- RETURNS: markerID
---
---@param sgroup SGroupID
---@param markertable table
function Cmd_MoveToClosestMarker(sgroup, markertable) end

--- Move a squad group to cover that's close a given position. If cover cannot be found, it just moves to the target position.
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param position Pos
---@param searchRadius[ number
---@param queued] boolean
function Cmd_MoveToCover(sgroup, position, searchRadius[, queued]) end

--- Move a squad group to the center of an entity group.
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param egroup EGroupID
---@param queued? boolean
function Cmd_MoveToEGroup(sgroup, egroup, queued) end

--- Move a squad group to a given markers position.
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param marker MarkerID
---@param queued? boolean
function Cmd_MoveToMarker(sgroup, marker, queued) end

--- Move a squad group to a given markers position only after other previously queued command is done
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param marker MarkerID
function Cmd_MoveToMarkerQueued(sgroup, marker) end

--- Move a squad group to a given position. The sgroup can be deleted when in proximity of a marker if you pass in the marker as the 4th argument.
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param position Pos
---@param queued [Boolean
---@param deleteWhenNearMarker] MarkerID
function Cmd_MoveToPos(sgroup, position, queued, deleteWhenNearMarker]) end

--- 
--- RETURNS: Void
---
---@param source SGroupID
---@param position Position
---@param offset integer
---@param distance[ number
---@param queued] boolean
function Cmd_MoveToPosOffset(source, position, offset, distance[, queued]) end

--- Move a squad group to another squad group.  This function should track the target squad groups position. 
--- RETURNS: Void
---
---@param sourcesgroup SGroupID
---@param targetsgroup SGroupID
---@param queued? boolean
function Cmd_MoveToSGroup(sourcesgroup, targetsgroup, queued) end

--- 
--- RETURNS: Void
---
---@param source SGroupID
---@param target SGroupID
---@param offset integer
---@param distance[ number
---@param queued] boolean
function Cmd_MoveToSGroupOffset(source, target, offset, distance[, queued]) end

--- Command player to do ability onto itself.
--- RETURNS: Void
---
---@param playerid PlayerID
---@param abilityid AbilityID
---@param skipCostPrereq? boolean
function Cmd_PlayerAbility(playerid, abilityid, skipCostPrereq) end

--- Command player to do ability on a marker position.
--- RETURNS: Void
---
---@param playerid PlayerID
---@param abilityid AbilityID
---@param marker MarkerID
---@param skipCostPrereq? boolean
function Cmd_PlayerAbilityMarker(playerid, abilityid, marker, skipCostPrereq) end

--- Command player to do ability on a position.
--- RETURNS: Void
---
---@param playerid PlayerID
---@param abilityid AbilityID
---@param position Position
---@param skipCostPrereq? boolean
function Cmd_PlayerAbilityPos(playerid, abilityid, position, skipCostPrereq) end

--- Command player to do ability on a position with direction
--- RETURNS: Void
---
---@param playerid PlayerID
---@param abilityid AbilityID
---@param position Position
---@param direction Position
---@param skipCostPrereq? boolean
function Cmd_PlayerAbilityPosDir(playerid, abilityid, position, direction, skipCostPrereq) end

--- Sends an upgrade command to the player. upgradeid should be the number returned by Util_GetUpgradeID( upgradename )
--- RETURNS: Void
---
---@param playerid PlayerID
---@param upgradeid[ UpgradeID
---@param count] integer
function Cmd_PlayerUpgrade(playerid, upgradeid[, count]) end

--- Sends a reinforce command to all squads in a group.  count represents the number of commands to send.
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param count integer
function Cmd_ReinforceUnit(sgroup, count) end

--- Sends a reinforce command to all squads in a group.  count represents the number of commands to send. spawnlocation is where the reinforced unit will spawn. You can optionally find a hidden position by specifying a checktype (CHECK_OFFCAMERA, CHECK_IN_FOW or CHECK_BOTH), and what to do if a hidden position can't be found (SPAWN_ATMARKER, SPAWN_ATSQUAD, or DO_NOTHING) - SPAWN_ATMARKER is the default.
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param count integer
---@param spawnlocation[ MarkerID/Pos
---@param checktype[ integer
---@param failtype]] integer
function Cmd_ReinforceUnitPos(sgroup, count, spawnlocation[, checktype[, failtype]]) end

--- Order a squad group to retreat to the nearest building
--- RETURNS: Void
---
---@param sgroup SGroupID
function Cmd_Retreat(sgroup) end

--- Order a squad group to retreat to the nearest building
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param destination Position
function Cmd_RetreatToLocation(sgroup, destination) end

--- Order a squad group to revert occupied building
--- RETURNS: 
---
---@param sgroupid SGroupID
---@param targetid EGroupID
---@param queued? boolean
function Cmd_RevertOccupiedBuilding(sgroupid, targetid, queued) end

--- apply critical hit to the entities
--- RETURNS: Void
---
---@param playerid PlayerID
---@param sgroupid SGroupID
---@param criticalid CriticalID
---@param damageid DamageID
function Cmd_SGroupCriticalHit(playerid, sgroupid, criticalid, damageid) end

--- apply critical hit a single entity
--- RETURNS: Void
---
---@param playerid PlayerID
---@param entityid EntityID
---@param criticalid CriticalID
---@param damageid DamageID
function Cmd_SingleCriticalHit(playerid, entityid, criticalid, damageid) end

--- Sends an ability command without additional target to all squads in a group.  abilityid should be the number returned by Util_GetAbilityID( abilityname )
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param abilityid AbilityID
---@param skipCostPrereq? boolean
function Cmd_SquadAbility(sgroup, abilityid, skipCostPrereq) end

--- Command squad to do ability on an entity.
--- RETURNS: Void
---
---@param sgroupid SGroupID
---@param abilityid AbilityID
---@param to_egroupid EGroupID
---@param skipCostPrereq? boolean
function Cmd_SquadAbilityEntity(sgroupid, abilityid, to_egroupid, skipCostPrereq) end

--- Command squad to do ability on a position.
--- RETURNS: Void
---
---@param sgroupid SGroupID
---@param abilityid AbilityID
---@param position Position
---@param skipCostPrereq? boolean
function Cmd_SquadAbilityPos(sgroupid, abilityid, position, skipCostPrereq) end

--- Command squad to do ability on a squad.
--- RETURNS: Void
---
---@param sgroupid SGroupID
---@param abilityid AbilityID
---@param to_sgroupid SGroupID
---@param skipCostPrereq? boolean
function Cmd_SquadAbilitySquad(sgroupid, abilityid, to_sgroupid, skipCostPrereq) end

--- Sends an camouflage stance command to all squads in a group.  stanceid should be the number returned by Util_GetCamouflageStanceID( stancename )
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param stanceid CamouflageStanceID
function Cmd_SquadCamouflageStance(sgroup, stanceid) end

--- Send a command to the squad to follow a path. Can wait at each waypoint. The sgroup can be deleted when in proximity of a marker if you pass in the marker as the 7th argument.
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param pathName string
---@param bFromClosest boolean
---@param bLoop boolean
---@param bAttackMove boolean
---@param pauseTime[ number
---@param deleteWhenNearMarker] MarkerID
function Cmd_SquadPath(sgroup, pathName, bFromClosest, bLoop, bAttackMove, pauseTime[, deleteWhenNearMarker]) end

--- Causes a squad to patrol a marker attacking any enemies that come within its radius. If used on circular markers, the radius must be at least 5. To stop the squad from patrolling the marker, use Cmd_StopSquads.
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param marker MarkerID
function Cmd_SquadPatrolMarker(sgroup, marker) end

--- Order all squads in the SGroup to unload from vehicle or building that they are currently in
--- RETURNS: 
---
---@param sgroupid SGroupID
---@param destination MarkerID/Pos
---@param queued? boolean
function Cmd_SquadUnload(sgroupid, destination, queued) end

--- Sends an upgrade command to all squads in a group.  upgradeid should be the number returned by Util_GetUpgradeID( upgradename )
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param upgradeid[ UpgradeID
---@param count] integer
function Cmd_SquadUpgrade(sgroup, upgradeid[, count]) end

--- Order a squad group to attack another squad group without moving.  This function should track the target squad groups position. 
--- RETURNS: Void
---
---@param sourcesgroup SGroupID
---@param targetegroup EGroupID
---@param queued? boolean
function Cmd_StationaryAttackEGroup(sourcesgroup, targetegroup, queued) end

--- Order a squad group to attack a position without moving.  This function should track the target squad groups position. 
--- RETURNS: Void
---
---@param sourcesgroup SGroupID
---@param target MarkerID/Pos
---@param queued? boolean
function Cmd_StationaryAttackPosition(sourcesgroup, target, queued) end

--- Order a squad group to attack another squad group without moving.  This function should track the target squad groups position. 
--- RETURNS: Void
---
---@param sourcesgroup SGroupID
---@param targetsgroup SGroupID
---@param queued? boolean
function Cmd_StationaryAttackSGroup(sourcesgroup, targetsgroup, queued) end

--- Command an egroup to stop.  This is the same as issuing a stop from the taskbar
--- RETURNS: Void
---
---@param egroup EGroupID
function Cmd_StopEntities(egroup) end

--- Command an sgroup to stop.  This is the same as issuing a stop from the taskbar
--- RETURNS: Void
---
---@param sgroup SGroupID
function Cmd_StopSquads(sgroup) end

--- Pass in a group to command to 'stop'. Pass in booleans for capturing and building
--- RETURNS: Void
---
---@param sgroup1[ SGroupID
---@param stopCapture boolean
---@param stopBuild] boolean
function Cmd_StopSquadsExcept(sgroup1[, stopCapture, stopBuild]) end

--- Order all squads from the building to ungarrison into destinated location
--- RETURNS: 
---
---@param egroupid EGroupID
---@param destination Position
function Cmd_Ungarrison(egroupid, destination) end

--- Order first squad in the sgroup from the building to ungarrison into destinated location
--- RETURNS: 
---
---@param egroupid EGroupID
---@param destination Position
---@param sgroupid SGroupID
function Cmd_UngarrisonSquad(egroupid, destination, sgroupid) end

--- Order all squads from the vehicle in the SGroup to unload when the vehicle reaches the destination
--- RETURNS: 
---
---@param sgroupid SGroupID
---@param destination MarkerID/Pos
function Cmd_Unload(sgroupid, destination) end

---  Send a entity command to a entity group(CMD_DefaultAction, CMD_Stop, CMD_Destroy, CMD_BuildSquad, CMD_CancelProduction, CMD_RallyPoint, CMD_AttackForced)
--- RETURNS: Void
---
---@param player PlayerID
---@param egroup EGroupID
---@param entityCommand integer
function Command_Entity(player, egroup, entityCommand) end

--- Send a entity-based command to an entity group.
--- RETURNS: Void
---
---@param player PlayerID
---@param egroup EGroupID
---@param entityCommand integer
---@param target EGroupID
function Command_EntityEntity(player, egroup, entityCommand, target) end

---  Send a squad command to a squad group with custom data
--- RETURNS: Void
---
---@param player PlayerID
---@param egroup EGroupID
---@param entityCommand integer
---@param cmdparam integer
---@param queued boolean
function Command_EntityExt(player, egroup, entityCommand, cmdparam, queued) end

--- Send a position command to an entity group.
--- RETURNS: Void
---
---@param player PlayerID
---@param egroup EGroupID
---@param entityCommand integer
---@param target Position
function Command_EntityPos(player, egroup, entityCommand, target) end

--- Send a dual target (position and squad) command to an entity group.
--- RETURNS: Void
---
---@param player PlayerID
---@param egroup EGroupID
---@param entityCommand integer
---@param target Position
---@param sgroup SGroupID
function Command_EntityPosSquad(player, egroup, entityCommand, target, sgroup) end

--- Send a squad-based command to an entity group.
--- RETURNS: Void
---
---@param player PlayerID
---@param egroup EGroupID
---@param entityCommand integer
---@param target SGroupID
function Command_EntitySquad(player, egroup, entityCommand, target) end

---  Send a player command to a player
--- RETURNS: Void
---
---@param player PlayerID
---@param dest PlayerID
---@param playerCommand integer
function Command_Player(player, dest, playerCommand) end

--- Send a player ability command (PCMD_Ability) to a player
--- RETURNS: Void
---
---@param player PlayerID
---@param dest PlayerID
---@param abilityBagID integer
---@param skipCostPrereq boolean
function Command_PlayerAbility(player, dest, abilityBagID, skipCostPrereq) end

---  Send a player command to itself to act upon single with custom parameter and index data (PCMD_CriticalHit)
--- RETURNS: Void
---
---@param player PlayerID
---@param entity EntityID
---@param playerCommand integer
---@param criticalBagID integer
---@param queued boolean
---@param damageBagID integer
function Command_PlayerEntityCriticalHit(player, entity, playerCommand, criticalBagID, queued, damageBagID) end

---  Send a player command to a player with a custom flag
--- RETURNS: Void
---
---@param player PlayerID
---@param dest PlayerID
---@param playerCommand integer
---@param cmdparam integer
---@param queued boolean
function Command_PlayerExt(player, dest, playerCommand, cmdparam, queued) end

--- Send a position command to a player.
--- RETURNS: Void
---
---@param player PlayerID
---@param dest PlayerID
---@param playerCommand integer
---@param pos Position
function Command_PlayerPos(player, dest, playerCommand, pos) end

--- Send a positional ability command (PCMD_Ability) to a player
--- RETURNS: Void
---
---@param player PlayerID
---@param dest PlayerID
---@param pos Position
---@param abilityBagID integer
---@param skipCostPrereq boolean
function Command_PlayerPosAbility(player, dest, pos, abilityBagID, skipCostPrereq) end

--- Send a positional/directional ability command (PCMD_Ability) to a player
--- RETURNS: Void
---
---@param player PlayerID
---@param dest PlayerID
---@param pos Position
---@param dir Position
---@param abilityBagID integer
---@param skipCostPrereq boolean
function Command_PlayerPosDirAbility(player, dest, pos, dir, abilityBagID, skipCostPrereq) end

--- Send a position command to a player with extra info
--- RETURNS: Void
---
---@param player PlayerID
---@param dest PlayerID
---@param playerCommand integer
---@param pos Position
---@param cmdparam integer
---@param queued boolean
function Command_PlayerPosExt(player, dest, playerCommand, pos, cmdparam, queued) end

---  Send a player command to itself to order squads in the sgroup to construct the building at specific position and facing
--- RETURNS: Void
---
---@param player PlayerID
---@param sgroup SGroupID
---@param structureBlueprintID integer
---@param position Position
---@param facing Position
---@param queued boolean
function Command_PlayerSquadConstructBuilding(player, sgroup, structureBlueprintID, position, facing, queued) end

---  Send a player command to itself to order squads in the sgroup to construct fences from posStart to posEnd
--- RETURNS: Void
---
---@param player PlayerID
---@param sgroup SGroupID
---@param structureBlueprintID integer
---@param posStart Position
---@param posEnd Position
---@param queued boolean
function Command_PlayerSquadConstructFence(player, sgroup, structureBlueprintID, posStart, posEnd, queued) end

---  Send a player command to itself to order squads in the sgroup to construct a field ranging from posStart to posEnd
--- RETURNS: Void
---
---@param player PlayerID
---@param sgroup SGroupID
---@param structureBlueprintID integer
---@param posStart Position
---@param posEnd Position
---@param queued boolean
function Command_PlayerSquadConstructField(player, sgroup, structureBlueprintID, posStart, posEnd, queued) end

---  Send a player command to itself to act upon all entities in the sgroup with custom parameter and index data (PCMD_CriticalHit)
--- RETURNS: Void
---
---@param player PlayerID
---@param sgroup SGroupID
---@param playerCommand integer
---@param criticalBagID integer
---@param queued boolean
---@param damageBagID integer
function Command_PlayerSquadCriticalHit(player, sgroup, playerCommand, criticalBagID, queued, damageBagID) end

---  Send a squad command to a squad group
--- RETURNS: Void
---
---@param player PlayerID
---@param sgroup SGroupID
---@param squadCommand integer
---@param queued boolean
function Command_Squad(player, sgroup, squadCommand, queued) end

--- Send a ability command (SCMD_Ability) to a squad
--- RETURNS: Void
---
---@param player PlayerID
---@param sgroup SGroupID
---@param abilityBagID integer
---@param skipCostPrereq boolean
function Command_SquadAbility(player, sgroup, abilityBagID, skipCostPrereq) end

--- Send a position ATTACK MOVE command to a squad group with custom data
--- RETURNS: Void
---
---@param player PlayerID
---@param sgroup SGroupID
---@param squadCommand integer
---@param target Position
---@param planName string
---@param queued boolean
function Command_SquadAttackMovePos(player, sgroup, squadCommand, target, planName, queued) end

--- Send a custom squad AI plan command to the squad to execute the plan specified towards the position
--- RETURNS: Void
---
---@param player PlayerID
---@param sgroup SGroupID
---@param planFile string
---@param queued boolean
function Command_SquadDoCustomPlan(player, sgroup, planFile, queued) end

--- Send a custom squad AI plan command to the squad to execute the plan specified towards the position
--- RETURNS: Void
---
---@param player PlayerID
---@param sgroup SGroupID
---@param pos Position
---@param planFile string
---@param queued boolean
function Command_SquadDoCustomPlanTarget(player, sgroup, pos, planFile, queued) end

--- Send an entity command to a squad group.
--- RETURNS: Void
---
---@param player PlayerID
---@param sgroup SGroupID
---@param squadCommand integer
---@param target EGroupID
---@param queued boolean
function Command_SquadEntity(player, sgroup, squadCommand, target, queued) end

--- Send a entity ability command (SCMD_Ability) to a squad
--- RETURNS: Void
---
---@param player PlayerID
---@param sgroup SGroupID
---@param target EGroupID
---@param abilityBagID integer
---@param skipCostPrereq boolean
function Command_SquadEntityAbility(player, sgroup, target, abilityBagID, skipCostPrereq) end

--- Send an entity command ATTACK to a squad group.
--- RETURNS: Void
---
---@param player PlayerID
---@param sgroup SGroupID
---@param target EGroupID
---@param bCheckFOW boolean
---@param bStationary boolean
---@param planName string
---@param queued boolean
function Command_SquadEntityAttack(player, sgroup, target, bCheckFOW, bStationary, planName, queued) end

--- Send a entity command to a squad group with custom data
--- RETURNS: Void
---
---@param player PlayerID
---@param sgroup SGroupID
---@param squadCommand integer
---@param target EGroupID
---@param cmdparam integer
---@param queued boolean
function Command_SquadEntityExt(player, sgroup, squadCommand, target, cmdparam, queued) end

--- Send special squad command to a squad group with squad load parameters
--- RETURNS: Void
---
---@param player PlayerID
---@param sgroup SGroupID
---@param squadCommand integer
---@param target EGroupID
---@param bOverLoad boolean
---@param bQueued boolean
function Command_SquadEntityLoad(player, sgroup, squadCommand, target, bOverLoad, bQueued) end

---  Send a squad command to a squad group with custom data
--- RETURNS: Void
---
---@param player PlayerID
---@param sgroup SGroupID
---@param squadCommand integer
---@param cmdparam integer
---@param queued boolean
function Command_SquadExt(player, sgroup, squadCommand, cmdparam, queued) end

--- Send a position command to a squad group.
--- RETURNS: Void
---
---@param player PlayerID
---@param sgroup SGroupID
---@param squadCommand integer
---@param target Position
---@param queued boolean
function Command_SquadPos(player, sgroup, squadCommand, target, queued) end

--- Send a positional ability command (SCMD_Ability) to a squad
--- RETURNS: Void
---
---@param player PlayerID
---@param sgroup SGroupID
---@param pos Position
---@param abilityBagID integer
---@param skipCostPrereq boolean
function Command_SquadPosAbility(player, sgroup, pos, abilityBagID, skipCostPrereq) end

--- Send a position command to a squad group with custom data
--- RETURNS: Void
---
---@param player PlayerID
---@param sgroup SGroupID
---@param squadCommand integer
---@param target Position
---@param cmdparam integer
---@param queued boolean
function Command_SquadPosExt(player, sgroup, squadCommand, target, cmdparam, queued) end

--- Send a command to a squad group with 2 positional input
--- RETURNS: Void
---
---@param player PlayerID
---@param sgroup SGroupID
---@param squadCommand integer
---@param target Position
---@param target2 Position
function Command_SquadPosPos(player, sgroup, squadCommand, target, target2) end

--- Send an position command ATTACK to a squad group.
--- RETURNS: Void
---
---@param player PlayerID
---@param sgroup SGroupID
---@param target Position
---@param bCheckFOW boolean
---@param bStationary boolean
---@param planName string
---@param queued boolean
function Command_SquadPositionAttack(player, sgroup, target, bCheckFOW, bStationary, planName, queued) end

--- Send an squad-based command to a squad group.
--- RETURNS: Void
---
---@param player PlayerID
---@param sgroup SGroupID
---@param squadCommand integer
---@param target SGroupID
---@param queued boolean
function Command_SquadSquad(player, sgroup, squadCommand, target, queued) end

--- Send a squad ability command (SCMD_Ability) to a squad
--- RETURNS: Void
---
---@param player PlayerID
---@param sgroup SGroupID
---@param target SGroupID
---@param abilityBagID integer
---@param skipCostPrereq boolean
function Command_SquadSquadAbility(player, sgroup, target, abilityBagID, skipCostPrereq) end

--- Send an squad-based command to a squad group.
--- RETURNS: Void
---
---@param player PlayerID
---@param sgroup SGroupID
---@param target SGroupID
---@param bCheckFOW boolean
---@param bStationary boolean
---@param planName string
---@param queued boolean
function Command_SquadSquadAttack(player, sgroup, target, bCheckFOW, bStationary, planName, queued) end

--- Send a squad command to a squad group with custom data
--- RETURNS: Void
---
---@param player PlayerID
---@param sgroup SGroupID
---@param squadCommand integer
---@param target SGroupID
---@param cmdparam integer
---@param queued boolean
function Command_SquadSquadExt(player, sgroup, squadCommand, target, cmdparam, queued) end

--- Send special squad command to a squad group with squad load parameters
--- RETURNS: Void
---
---@param player PlayerID
---@param sgroup SGroupID
---@param squadCommand integer
---@param target SGroupID
---@param bOverLoad boolean
---@param bQueued boolean
function Command_SquadSquadLoad(player, sgroup, squadCommand, target, bOverLoad, bQueued) end

---create one battle result string
---
---Located in FILE: \scar\battle_simulator.scar LINE: 118
function CreateResults() end

---
---Located in FILE: \scar\debug.scar LINE: 1
---@param T any
---@param msg any
---@param indent any
function Debug_PrintTable(T, msg, indent) end

---
---Located in FILE: \scar\designerlib.scar LINE: 14
function DesignerLib_Init() end

---
---Located in FILE: \scar\underattack.scar LINE: 96
---@param sgroup any
---@param boolean any
function DoICareIfImUnderAttack(sgroup, boolean) end

---
---Located in FILE: \scar\underattack.scar LINE: 104
---@param tablename any
function DoTankAction(tablename) end

--- Returns true if an entity blueprint exists with the given name.
--- RETURNS: Boolean
---
---@param name string
function EBP_Exists(name) end

--- Adds an entity to the end of a group if the group doesnt already have it.
--- RETURNS: Void
---
---@param group EGroupID
---@param entity EntityID
function EGroup_Add(group, entity) end

--- Appends the entities in one group to another group.  
--- RETURNS: Void
---
---@param group EGroupID
---@param grouptoadd EGroupID
function EGroup_AddEGroup(group, grouptoadd) end

--- Returns true if ALL or ANY entities in a group can see ALL or ANY entities in a given egroup.
--- RETURNS: Boolean
---
---@param egroup EGroupID
---@param targetegroup EGroupID
---@param all boolean
function EGroup_CanSeeEGroup(egroup, targetegroup, all) end

--- Returns true if ALL or ANY entities in a group can see ALL or ANY squads in a given sgroup.
--- RETURNS: Boolean
---
---@param egroup EGroupID
---@param targetsgroup SGroupID
---@param all boolean
function EGroup_CanSeeSGroup(egroup, targetsgroup, all) end

--- Removes all entities from a group
--- RETURNS: Void
---
---@param egroup EGroupID
function EGroup_Clear(egroup) end

--- Returns true if the contents of the two groups are equal. Order of the entities does not matter.
--- RETURNS: Boolean
---
---@param group1 EGroupID
---@param group2 EGroupID
function EGroup_Compare(group1, group2) end

--- Check if a group contains ALL or ANY of the items in a blueprint table.
--- RETURNS: Boolean
---
---@param egroup EGroupID
---@param blueprints BlueprintTable
---@param all boolean
function EGroup_ContainsBlueprints(egroup, blueprints, all) end

--- Returns true if EGroup1 contains ANY or ALL of EGroup2
--- RETURNS: Boolean
---
---@param egroup1 EGroupID
---@param egroup2 EGroupID
---@param all boolean
function EGroup_ContainsEGroup(egroup1, egroup2, all) end

--- Returns true if EGroup contains a particular EntityID
--- RETURNS: Boolean
---
---@param egroup EGroupID
---@param entity EntityID
function EGroup_ContainsEntity(egroup, entity) end

--- Returns the total number of spawned and despawned entities in a group.
--- RETURNS: Integer
---
---@param egroup EGroupID
function EGroup_Count(egroup) end

--- Get the number of alive entities (both spawned and despawned)
--- RETURNS: int
---
---@param numAlive INT
function EGroup_CountAlive(numAlive) end

--- Returns the number of despawned entities in a group.
--- RETURNS: Integer
---
---@param egroup EGroupID
function EGroup_CountDeSpawned(egroup) end

--- Returns the number of spawned entities in a group.
--- RETURNS: Integer
---
---@param egroup EGroupID
function EGroup_CountSpawned(egroup) end

--- Returns a new entity group with the given name.
--- RETURNS: EGroupID
---
---@param name string
function EGroup_Create(name) end

--- Find a entity group from name.  Creates a new one with given name if it doesnt exist.
--- RETURNS: EGroupID
---
---@param egroupname string
function EGroup_CreateIfNotFound(egroupname) end

--- Create and display kicker message on the each entity in the egroup to the player
--- RETURNS: Void
---
---@param group EGroupID
---@param textid LocString
function EGroup_CreateKickerMessage(group, textid) end

--- Returns a table of egroups NOT in the world builder
--- RETURNS: LuaTable
---
---@param format string
---@param size integer
function EGroup_CreateTable(format, size) end

--- Despawn all spawned entities in a group.
--- RETURNS: Void
---
---@param egroup EGroupID
function EGroup_DeSpawn(egroup) end

--- Manually destroy a group that you dont need anymore.
--- RETURNS: Void
---
---@param egroup EGroupID
function EGroup_Destroy(egroup) end

--- Destroys all spawned and despawned entities in a group.
--- RETURNS: Void
---
---@param egroup EGroupID
function EGroup_DestroyAllEntities(egroup) end

--- Duplicates an EGroup
--- RETURNS: Void
---
---@param egroupid1 EGroupID
---@param egroupid2 EGroupID
function EGroup_Duplicate(egroupid1, egroupid2) end

--- Returns true if the entity group with the given name exists
--- RETURNS: Boolean
---
---@param name string
function EGroup_Exists(name) end

--- Filters an EGroup by blueprint.
--- RETURNS: Void
---
---@param egroup EGroupID
---@param blueprint String/ID/Table
---@param filtertype integer
function EGroup_Filter(egroup, blueprint, filtertype) end

--- Filters an EGroup by construction status.
--- RETURNS: Void
---
---@param egroup EGroupID
---@param filtertype integer
function EGroup_FilterUnderConstruction(egroup, filtertype) end

--- Call a lua function for each item in a group. Function will recieve (groupid, itemindex, itemid) and should return true to break or false to continue.
--- RETURNS: Boolean
---
---@param egroup EGroupID
---@param f LuaBinding::StackVarFunction
function EGroup_ForEach(egroup, f) end

--- Call a lua function for each item in a group. Function will recieve (groupid, itemindex, itemid) and should return a bool.
--- RETURNS: Boolean
---
---@param egroup EGroupID
---@param all boolean
---@param f LuaBinding::StackVarFunction
function EGroup_ForEachAllOrAny(egroup, all, f) end

--- Same as EGroup_ForEachAllOrAny except you have a choice to iterate over spawned entities, despawned entities, or both.
--- RETURNS: Boolean
---
---@param egroup EGroupID
---@param all boolean
---@param f LuaBinding::StackVarFunction
---@param spawned boolean
---@param despawned boolean
function EGroup_ForEachAllOrAnyEx(egroup, all, f, spawned, despawned) end

--- Same as EGroup_ForEach except you have a choice to iterate over spawned entities, despawned entities, or both.
--- RETURNS: Boolean
---
---@param egroup EGroupID
---@param f LuaBinding::StackVarFunction
---@param spawned boolean
---@param despawned boolean
function EGroup_ForEachEx(egroup, f, spawned, despawned) end

--- Find an entity group with a given name.
--- RETURNS: EGroupID
---
---@param name string
function EGroup_FromName(name) end

--- Returns the average health of all units in a entity group as a weighted percent [0.0, 1.0].
--- RETURNS: Real
---
---@param egroup EGroupID
function EGroup_GetAvgHealth(egroup) end

--- Returns the despawned entity at the given index.
--- RETURNS: EntityID
---
---@param group EGroupID
---@param index integer
function EGroup_GetDeSpawnedEntityAt(group, index) end

--- Check invulnerablity state for ALL or ANY entity in an entity group.
--- RETURNS: Boolean
---
---@param egroup EGroupID
---@param all boolean
function EGroup_GetInvulnerable(egroup, all) end

--- Returns the name of a given entity group.
--- RETURNS: String
---
---@param egroup EGroupID
function EGroup_GetName(egroup) end

--- Returns the number of combat slots in total of a certain hold-able type 
--- RETURNS: Integer
---
---@param egroup EGroupID
---@param holdTypeName string
function EGroup_GetNumCombatSlots(egroup, holdTypeName) end

--- Returns the center position of an entity group.
--- RETURNS: Position
---
---@param group EGroupID
function EGroup_GetPosition(group) end

--- Get a random spawned entity from egroup
--- RETURNS: EntityID
---
---@param egroupid EGroup
function EGroup_GetRandomSpawnedEntity(egroupid) end

--- Builds a table of EGroupIDs that are named in a sequence. i.e. a name of "eg_building" will find groups "eg_building1", "eg_building2" and so on, up until it looks for a group that isn't there.
--- RETURNS: Table
---
---@param name string
function EGroup_GetSequence(name) end

--- Returns the spawned entity at the given index.
--- RETURNS: EntityID
---
---@param group EGroupID
---@param index integer
function EGroup_GetSpawnedEntityAt(group, index) end

--- Returns the distance from the centre of the group of the entity that furthest out. 
--- RETURNS: Real
---
---@param egroup EGroupID
function EGroup_GetSpread(egroup) end

--- Returns an sgroup containing all squads held by any entities in an egroup
--- RETURNS: Void
---
---@param egroup EGroupID
---@param sgroupRecipient SGroupID
function EGroup_GetSquadsHeld(egroup, sgroupRecipient) end

--- Returns a table of egroups from the world builder
--- RETURNS: LuaTable
---
---@param format string
function EGroup_GetWBTable(format) end

--- Returns whether ANY or ALL entities in an EGroup have the specified upgrade
--- RETURNS: Boolean
---
---@param egroup EGroupID
---@param upgrade UpgradeID
---@param all boolean
function EGroup_HasUpgrade(egroup, upgrade, all) end

--- Hide or show all entities in an EGroup
--- RETURNS: Void
---
---@param egroup EGroupID
---@param hide boolean
function EGroup_Hide(egroup, hide) end

--- Change the ownership of a Strategic Point
--- RETURNS: Void
---
---@param egroup EGroupID
---@param player PlayerID
function EGroup_InstantCaptureStrategicPoint(egroup, player) end

--- Performs a group intersection. 
--- RETURNS: Void
---
---@param group EGroupID
---@param grouptointersect EGroupID
function EGroup_Intersection(group, grouptointersect) end

--- Returns true if all or any strategic points in a group have been captured. Use ANY or ALL.
--- RETURNS: Boolean
---
---@param egroup EGroupID
---@param playerId PlayerID
---@param all boolean
function EGroup_IsCapturedByPlayer(egroup, playerId, all) end

--- Returns true if ALL or ANY entities are attacking within the time
--- RETURNS: Boolean
---
---@param egroup EGroupID
---@param all boolean
---@param time number
function EGroup_IsDoingAttack(egroup, all, time) end

--- Returns true if a named entity group contains no spawned or despawned entities
--- RETURNS: Boolean
---
---@param egroup EGroupID
function EGroup_IsEmpty(egroup) end

--- Returns whether any entity in an EGroup has a hold on anything
--- RETURNS: Void
---
---@param egroup EGroupID
function EGroup_IsHoldingAny(egroup) end

--- Returns true if ALL or ANY entities are in cover.
--- RETURNS: Boolean
---
---@param egroup EGroupID
---@param all boolean
function EGroup_IsInCover(egroup, all) end

--- Returns true if ANY or ALL entities in an EGroup are moving.
--- RETURNS: Boolean
---
---@param egroupid EGroupID
---@param all boolean
function EGroup_IsMoving(egroupid, all) end

--- Returns true if ANY or ALL (use those keywords) of the enities in the group are present onscreen. You can pass in a percentage of the screen to check, so 0.8 would be a centered rectangle occupying 80% of the screen.
--- RETURNS: Bool
---
---@param player PlayerID
---@param group EGroupID
---@param all[ boolean
---@param percent] number
function EGroup_IsOnScreen(player, group, all[, percent]) end

--- Returns true if ALL or ANY entities are under attack within the time
--- RETURNS: Boolean
---
---@param egroup EGroupID
---@param all boolean
---@param time number
function EGroup_IsUnderAttack(egroup, all, time) end

--- Check if the entities are attacked by the player
--- RETURNS: Bool
---
---@param group EGroupID
---@param attackerplayer PlayerID
---@param duration number
function EGroup_IsUnderAttackByPlayer(group, attackerplayer, duration) end

--- Kill all entities in an EGroup
--- RETURNS: Void
---
---@param egroup EGroupID
function EGroup_Kill(egroup) end

--- Calls a function when any entity in an EGroup gets destroyed by the player clicking the "Detonate me" button
--- RETURNS: Void
---
---@param id EGroupID
---@param function function
function EGroup_NotifyOnPlayerDemolition(id, function) end

--- Respawn all despawned entities in a group.
--- RETURNS: Void
---
---@param egroup EGroupID
function EGroup_ReSpawn(egroup) end

--- Removes an entity from a group.
--- RETURNS: Void
---
---@param group EGroupID
---@param entity EntityID
function EGroup_Remove(group, entity) end

--- Trigger animation action for an EGroup. Please only use this for simple animations
--- RETURNS: Void
---
---@param egroup EGroupID
---@param actionName string
function EGroup_SetAnimatorAction(egroup, actionName) end

--- Set animation event for an EGroup. Please only use this for simple animations
--- RETURNS: Void
---
---@param egroup EGroupID
---@param eventName string
function EGroup_SetAnimatorEvent(egroup, eventName) end

--- Set animation state of a state machine for an EGroup. Please only use this for simple animations
--- RETURNS: Void
---
---@param egroup EGroupID
---@param stateMachineName string
---@param stateName string
function EGroup_SetAnimatorState(egroup, stateMachineName, stateName) end

--- Set animation variable value for an EGroup. Please only use this for simple animations
--- RETURNS: Void
---
---@param egroup EGroupID
---@param variableName string
---@param value number
function EGroup_SetAnimatorVariable(egroup, variableName, value) end

--- Sets whether a weapon to auto-target things or not
--- RETURNS: Void
---
---@param group EGroupID
---@param hardpoint string
---@param enable boolean
function EGroup_SetAutoTargetting(group, hardpoint, enable) end

--- Sets the health of each unit in an entity group to a given percent [0.0, 1.0].
--- RETURNS: Real
---
---@param egroup EGroupID
---@param healthPercent number
function EGroup_SetAvgHealth(egroup, healthPercent) end

--- Set the minimum health for this entity
--- RETURNS: Void
---
---@param egroup EGroupID
---@param minhealth float
function EGroup_SetHealthMinCap(egroup, minhealth) end

--- Enable/Disable invulnerablity for an entire entity group. Use true and false for simple on/off, or use a number between 0.0 and 1.0 for more precise control on how much damage an entity can take before it takes no more.
--- RETURNS: Void
---
---@param egroup EGroupID
---@param enabled[ boolean
---@param reset_time] number
function EGroup_SetInvulnerable(egroup, enabled[, reset_time]) end

--- Changes the player owner for all spawned and despawned entities of an EGroup.
--- RETURNS: Void
---
---@param egroup EGroup
---@param owner PlayerID
function EGroup_SetPlayerOwner(egroup, owner) end

--- Set player selectable state of entities in the egroup
--- RETURNS: Void
---
---@param egroup EGroupID
---@param selectable boolean
function EGroup_SetSelectable(egroup, selectable) end

--- Creates an entity group containing a single entity
--- RETURNS: EGroupID
---
---@param egroup EGroupID
---@param entity entityID
function EGroup_Single(egroup, entity) end

--- Displays info about the panel that is intersected by the passed in ray
--- RETURNS: Void
---
---@param pEntity EntityID
---@param cam Math::Vector3f
---@param terrain Math::Vector3f
function Entity_BuildingPanelInfo(pEntity, cam, terrain) end

--- Check if the entity can load squad or not
--- RETURNS: Boolean
---
---@param entity EntityID
---@param squad SquadID
function Entity_CanLoadSquad(entity, squad) end

--- Returns true if a target entity is in the entities sight radius
--- RETURNS: Boolean
---
---@param entity EntityID
---@param target EntityID
function Entity_CanSeeEntity(entity, target) end

--- Returns true if a target squad is in the squads sight radius
--- RETURNS: Boolean
---
---@param entity EntityID
---@param target SquadID
function Entity_CanSeeSquad(entity, target) end

--- Clears any previous posture suggestions made to an entity
--- RETURNS: Void
---
---@param entity EntityID
function Entity_ClearPostureSuggestion(entity) end

--- Creates an entity at a given position and assigns it to a given player.  'blueprint' is a string value containing the name of the entity blueprint. This function does not spawn the entity so you will need to call Entity_Spawn to see this entity
--- RETURNS: EntityID
---
---@param blueprintID integer
---@param player PlayerID
---@param pos Position
function Entity_Create(blueprintID, player, pos) end

--- Creates an entity at a given position and assigns it to a given player.  'blueprint' is a string value containing the name of the entity blueprint. This function spawns the entity so there is no need to call Entity_Spawn
--- RETURNS: EntityID
---
---@param blueprintID integer
---@param pos Position
function Entity_CreateENV(blueprintID, pos) end

--- DeSpawn the entity at its current position
--- RETURNS: Void
---
---@param entity EntityID
function Entity_DeSpawn(entity) end

--- Remove an entity from the world and destroy it.
--- RETURNS: Void
---
---@param entity EntityID
function Entity_Destroy(entity) end

--- Disables the death of the given entity building, only works for panel based destructible buldings
--- RETURNS: Boolean
---
---@param pEntity EntityID
---@param bDisableDeath boolean
function Entity_DisableBuildingDeath(pEntity, bDisableDeath) end

--- Damages this entity but only if its a destructible building
--- RETURNS: Void
---
---@param pEntity EntityID
---@param cam Math::Vector3f
---@param terrain Math::Vector3f
---@param dmgType integer
---@param radius number
function Entity_DoBuildingDamageRay(pEntity, cam, terrain, dmgType, radius) end

--- Force constructs this entity but only if its a building
--- RETURNS: Void
---
---@param e EntityID
function Entity_ForceConstruct(e) end

--- Get an entity from a mission editor ID.
--- RETURNS: EntityID
---
---@param id integer
function Entity_FromWorldID(id) end

--- Returns the active entity command.  Some valid entity commands: STATEID_StructureBuilding, STATEID_Dead, STATEID_Idle, STATEID_Pause
--- RETURNS: Integer
---
---@param entity EntityID
function Entity_GetActiveCommand(entity) end

--- Returns the id of the entity blueprint
--- RETURNS: Integer
---
---@param entity EntityID
function Entity_GetBlueprintID(entity) end

--- Returns the name of the entity blueprint (from the attribute editor)
--- RETURNS: String
---
---@param entity EntityID
function Entity_GetBlueprintName(entity) end

--- Returns the construction progress (with range [0.0, 1.0] for a given entity.  Returns 0.0 if the entity is not a building.
--- RETURNS: Real
---
---@param pEntity EntityID
function Entity_GetBuildingProgress(pEntity) end

--- Get cover safety value from the where the entity is standing. The safety value is number from -.5 to .5.
--- RETURNS: Real
---
---@param entity EntityID
function Entity_GetCoverValue(entity) end

--- Returns the entities unique id in the world
--- RETURNS: Integer
---
---@param entity EntityID
function Entity_GetGameID(entity) end

--- Returns the heading of the entity.  The heading is currently a lua table with three entries (x, y, z)
--- RETURNS: Position
---
---@param entity EntityID
function Entity_GetHeading(entity) end

--- Returns the health of an entity.
--- RETURNS: Real
---
---@param entity EntityID
function Entity_GetHealth(entity) end

--- Returns the max health of an entity.
--- RETURNS: Real
---
---@param entity EntityID
function Entity_GetHealthMax(entity) end

--- Returns the percentage health, taking into account destructible buildings
--- RETURNS: Real
---
---@param entity EntityID
function Entity_GetHealthPercentage(entity) end

--- Check if the entity is invulnerable or not
--- RETURNS: Bool
---
---@param squad SquadID
function Entity_GetInvulnerable(squad) end

--- Returns the invulnerable point in terms of percentage
--- RETURNS: Real
---
---@param entity EntityID
function Entity_GetInvulnerableMinCap(entity) end

--- Returns the number of combat slots in total of a certain hold-able type
--- RETURNS: Integer
---
---@param entity EntityID
---@param holdTypeName string
function Entity_GetNumCombatSlots(entity, holdTypeName) end

--- Returns the Player owner of the given entity. Entity MUST NOT be owned by the world.
--- RETURNS: PlayerID
---
---@param entity EntityID
function Entity_GetPlayerOwner(entity) end

--- Returns the position of the entity.  The position is currently a lua table with three entries (x, y, z)
--- RETURNS: Position
---
---@param entity EntityID
function Entity_GetPosition(entity) end

--- Returns the name of the production item with index in range of [0,n-1]
--- RETURNS: String
---
---@param entity EntityID
---@param index integer
function Entity_GetProductionQueueItemName(entity, index) end

--- Returns the production type (PITEM_Upgrade, PITEM_Spawn, PITEM_SquadUpgrade, PITEM_SquadReinforce, PITEM_PlayerUpgrade) for a production queue item with index.
--- RETURNS: Integer
---
---@param entity EntityID
---@param index integer
function Entity_GetProductionQueueItemType(entity, index) end

--- Returns the number of items in the entities production queue.
--- RETURNS: Integer
---
---@param entity EntityID
function Entity_GetProductionQueueSize(entity) end

--- Returns the inner sight radius for this entity
--- RETURNS: Real
---
---@param entity EntityID
function Entity_GetSightInnerHeight(entity) end

--- Returns the inner sight radius for this entity
--- RETURNS: Real
---
---@param entity EntityID
function Entity_GetSightInnerRadius(entity) end

--- Returns the outer sight radius for this entity
--- RETURNS: Real
---
---@param entity EntityID
function Entity_GetSightOuterHeight(entity) end

--- Returns the outer sight radius for this entity
--- RETURNS: Real
---
---@param entity EntityID
function Entity_GetSightOuterRadius(entity) end

--- Returns the Squad for the passed Entity. (May be NULL)
--- RETURNS: SquadID
---
---@param pEntity EntityID
function Entity_GetSquad(pEntity) end

--- Adds squads held by an entity to an SGroup
--- RETURNS: Boolean
---
---@param pEntity EntityID
---@param sgroup SGroupID
function Entity_GetSquadsHeld(pEntity, sgroup) end

--- Gets the total number of panels in a building (returns 0 for anything but panel based destructible buldings)
--- RETURNS: Integer
---
---@param pEntity EntityID
function Entity_GetTotalPanelCount(pEntity) end

--- Gets the current number of undestroyed panels in a building (returns 0 for anything but panel based destructible buldings)
--- RETURNS: Integer
---
---@param pEntity EntityID
function Entity_GetUndestroyedPanelCount(pEntity) end

--- Return true if the entity has the given criticalID applied to it
--- RETURNS: Boolean
---
---@param pEntity EntityID
---@param criticalBagID integer
function Entity_HasCritical(pEntity, criticalBagID) end

--- Returns true if an entity has a production queue.
--- RETURNS: Boolean
---
---@param entity EntityID
function Entity_HasProductionQueue(entity) end

--- Return true if the entity has purchased the specified upgrade.
--- RETURNS: Boolean
---
---@param pEntity EntityID
---@param pbgID integer
function Entity_HasUpgrade(pEntity, pbgID) end

--- Strategic point will be captured instantly by the team of the supplied player
--- RETURNS: Void
---
---@param entity EntityID
---@param player PlayerID
function Entity_InstantCaptureStrategicPoint(entity, player) end

--- Returns true if entity is still alive
--- RETURNS: Integer
---
---@param pEntity EntityID
function Entity_IsAlive(pEntity) end

--- Returns true if the entity is attacking within the time
--- RETURNS: Boolean
---
---@param entity EntityID
---@param time number
function Entity_IsAttacking(entity, time) end

--- Returns true if the given entity is a building
--- RETURNS: Boolean
---
---@param e EntityID
function Entity_IsBuilding(e) end

--- Returns true if the given blueprint is a building
--- RETURNS: Integer
---
---@param blueprintID integer
function Entity_IsEBPBuilding(blueprintID) end

--- Returns true if the given blueprint is objcover
--- RETURNS: Boolean
---
---@param blueprintID integer
function Entity_IsEBPObjCover(blueprintID) end

--- Check if the entity has a hold on anything
--- RETURNS: Boolean
---
---@param entity EntityID
function Entity_IsHoldingAny(entity) end

--- Returns true if entity is in cover.
--- RETURNS: Boolean
---
---@param entityId EntityID
function Entity_IsInCover(entityId) end

--- Returns whether an entity is moving.
--- RETURNS: Boolean
---
---@param pEntity EntityID
function Entity_IsMoving(pEntity) end

--- Determines if this entity is of the given type. Types are defined in type_ext/unit_type_list
--- RETURNS: Boolean
---
---@param entity EntityID
---@param type string
function Entity_IsOfType(entity, type) end

--- Returns true if the entity is part of a squad
--- RETURNS: Boolean
---
---@param pEntity EntityID
function Entity_IsPartOfSquad(pEntity) end

--- Returns true if the entity is a starting position
--- RETURNS: Boolean
---
---@param entity EntityID
function Entity_IsStartingPosition(entity) end

--- Returns true if the entity is a strategic point.
--- RETURNS: Boolean
---
---@param entity EntityID
function Entity_IsStrategicPoint(entity) end

--- Returns true if strategic point is captured by the team of the player provided.
--- RETURNS: Boolean
---
---@param entity EntityID
---@param player PlayerID
function Entity_IsStrategicPointCapturedBy(entity, player) end

--- Return true if the entity is a team weapon
--- RETURNS: Boolean
---
---@param entity EntityID
function Entity_IsSyncWeapon(entity) end

--- Returns true if the entity is under attack. 
--- RETURNS: Boolean
---
---@param entity EntityID
---@param time number
function Entity_IsUnderAttack(entity, time) end

--- Returns true if the entity is under attack by a certain player
--- RETURNS: Boolean
---
---@param entity EntityID
---@param pAttackerOwner PlayerID
---@param time number
function Entity_IsUnderAttackByPlayer(entity, pAttackerOwner, time) end

--- Check if an entity with the given ID can be found in the world
--- RETURNS: Boolean
---
---@param id integer
function Entity_IsValid(id) end

--- Returns whether an entity is a vehicle
--- RETURNS: Boolean
---
---@param pEntity EntityID
function Entity_IsVehicle(pEntity) end

--- Returns true if entityID is a victory point
--- RETURNS: Boolean
---
---@param *pEntity Entity
function Entity_IsVictoryPoint(*pEntity) end

--- Kill the entity.  Sets health to 0, and triggers death effects.
--- RETURNS: Void
---
---@param entity EntityID
function Entity_Kill(entity) end

--- Calls a function when an entity gets destroyed by the player clicking the "Detonate me" button next to an entity.
--- RETURNS: Void
---
---@param entity Entity
---@param function function
function Entity_NotifyOnPlayerDemolition(entity, function) end

--- Trigger animation action for an entity. Please only use this for simple animations
--- RETURNS: Void
---
---@param pEntity EntityID
---@param actionName string
function Entity_SetAnimatorAction(pEntity, actionName) end

--- Set animation action parameter for an entity. Please only use this for simple animations
--- RETURNS: Void
---
---@param pEntity EntityID
---@param actionParameterName string
---@param actionParameterValue string
function Entity_SetAnimatorActionParameter(pEntity, actionParameterName, actionParameterValue) end

--- Set animation event for an entity. Please only use this for simple animations
--- RETURNS: Void
---
---@param pEntity EntityID
---@param eventName string
function Entity_SetAnimatorEvent(pEntity, eventName) end

--- Set animation state of a state machine for an entity. Please only use this for simple animations
--- RETURNS: Void
---
---@param pEntity EntityID
---@param stateMachineName string
---@param stateName string
function Entity_SetAnimatorState(pEntity, stateMachineName, stateName) end

--- Set animation variable value for an entity. Please only use this for simple animations
--- RETURNS: Void
---
---@param pEntity EntityID
---@param variableName string
---@param value number
function Entity_SetAnimatorVariable(pEntity, variableName, value) end

--- Sets the heading of the entity.  The position is currently a lua table with three entries (x, y, z)
--- RETURNS: Void
---
---@param entity EntityID
---@param pos Position
function Entity_SetHeading(entity, pos) end

--- Set the health of an entity.  healthPercent must be in the range [0.0, 1.0].
--- RETURNS: Void
---
---@param entity EntityID
---@param healthPercent number
function Entity_SetHealth(entity, healthPercent) end

--- Set invulnerability on the entity. Reset time is in seconds. If it it set, the invulnerability will expire after this time.
--- RETURNS: Void
---
---@param entity EntityID
---@param enable boolean
---@param reset_time number
function Entity_SetInvulnerable(entity, enable, reset_time) end

--- Make an entity invulnerable to physical damage when health is below the minimum health percentage
--- RETURNS: Void
---
---@param entity EntityID
---@param minHealthPercentage number
---@param resetTime number
function Entity_SetInvulnerableMinCap(entity, minHealthPercentage, resetTime) end

--- Changes the owner of the given squad.
--- RETURNS: Void
---
---@param entity EntityID
---@param owner PlayerID
function Entity_SetPlayerOwner(entity, owner) end

--- Sets the position of the entity.  The position is currently a lua table with three entries (x, y, z)
--- RETURNS: Void
---
---@param entity EntityID
---@param pos Position
function Entity_SetPosition(entity, pos) end

--- Sets whether or not a projectile can explode. 
--- RETURNS: Void
---
---@param projectile EntityID
---@param canExplode boolean
function Entity_SetProjectileCanExplode(projectile, canExplode) end

--- Sets a strategic point to neutral (not owned by any team)
--- RETURNS: Void
---
---@param entity EntityID
function Entity_SetStrategicPointNeutral(entity) end

--- Spawn the entity at its current position
--- RETURNS: Void
---
---@param entity EntityID
function Entity_Spawn(entity) end

--- Suggests a posture to an entity, lasting the passed duration
--- RETURNS: Void
---
---@param entity EntityID
---@param posture unsigned
---@param duration number
function Entity_SuggestPosture(entity, posture, duration) end

--- Tags the entity to be used for debugging
--- RETURNS: Void
---
---@param entity EntityID
function Entity_TagDebug(entity) end

--- Hides or shows an entity visually.
--- RETURNS: Void
---
---@param pEntity EntityID
---@param bHide boolean
function Entity_VisHide(pEntity, bHide) end

--- Creates an Event Cue message which automatically sends the camera to a specified point when clicked on. 
--- RETURNS: event cue ID
---
---@param style CueStyleID
---@param title LocString
---@param description LocString
---@param cameratarget[ Marker/Pos/EGroup/SGroup
---@param hintpointtext LocString
---@param function function
---@param lifetime number
---@param dismissOnClick] boolean
function EventCue_Create(style, title, description, cameratarget[, hintpointtext, function, lifetime, dismissOnClick]) end

--- internal function to clean up hintpoints after 5 seconds
---
---Located in FILE: \scar\ui.scar LINE: 392
function EventCue_InternalHintPointManager() end

--- internal function that's activated when the event cue item is clicked on
---
---Located in FILE: \scar\ui.scar LINE: 347
---@param id any
function EventCue_InternalManager(id) end

--- Pauses for a given amount of time. This function MUST be called from a CTRL object in NISlet events only!
--- RETURNS: Void
---
---@param seconds number
function Event_Delay(seconds) end

--- Returns true if any event is currently running
--- RETURNS: Boolean
---
---@param Void any
function Event_IsAnyRunning(Void) end

--- Returns true if a given event is running
--- RETURNS: Boolean
---
---@param f function
function Event_IsRunning(f) end

--- Completes execution of the event immediatley (all calls to Wait() are ignored)
--- RETURNS: Void
---
---@param Void any
function Event_Skip(Void) end

--- Starts event.  Event will not start until all rules are evaluated for this frame!
--- RETURNS: Void
---
---@param eventFunction function
---@param priority integer
function Event_Start(eventFunction, priority) end

--- Enables or disables the FOW, including out of bound areas and all entities on the map
--- RETURNS: Void
---
---@param enable boolean
function FOW_Enable(enable) end

--- Reveal FOW for all players
--- RETURNS: Void
---
---@param Void any
function FOW_RevealAll(Void) end

--- Reveals a circular area for all alive players over a given duration. YOU SHOULD ONLY CALL THIS ONCE FOR EACH AREA.
--- RETURNS: Void
---
---@param x number
---@param z number
---@param radius number
---@param durationSecs number
function FOW_RevealArea(x, z, radius, durationSecs) end

--- Reveals an entity groups line of sight(LOS) for all alive players over a given duration. YOU SHOULD ONLY CALL THIS ONCE FOR EACH AREA.
--- RETURNS: Void
---
---@param group EGroupID
---@param durationSecs number
function FOW_RevealEGroup(group, durationSecs) end

--- Reveals an entity groups in FOW for all alive players over a given duration.
--- RETURNS: Void
---
---@param group EGroupID
---@param durationSecs number
function FOW_RevealEGroupOnly(group, durationSecs) end

--- Reveals an entities line of sight (LOS) for all alive players over a given duration. YOU SHOULD ONLY CALL THIS ONCE FOR EACH AREA.
--- RETURNS: Void
---
---@param entity EntityID
---@param durationSecs number
function FOW_RevealEntity(entity, durationSecs) end

--- Reveals an area the size of a given markers proximity at that markers position for a given amount of time. Pass in a duration of 1 for indefinite duration. YOU SHOULD ONLY CALL THIS ONCE FOR EACH AREA. 
--- RETURNS: Void
---
---@param marker MarkerID
---@param duration number
function FOW_RevealMarker(marker, duration) end

--- Reveals a squad groups line of sight(LOS) for all alive players over a given duration. YOU SHOULD ONLY CALL THIS ONCE FOR EACH AREA.
--- RETURNS: Void
---
---@param group SGroupID
---@param durationSecs number
function FOW_RevealSGroup(group, durationSecs) end

--- Reveals a squad groups in fow for all alive players over a given duration. YOU SHOULD ONLY CALL THIS ONCE FOR EACH AREA.
--- RETURNS: Void
---
---@param group SGroupID
---@param durationSecs number
function FOW_RevealSGroupOnly(group, durationSecs) end

--- Reveals a squads line if sight(LOS) for all alive players over a given duration. YOU SHOULD ONLY CALL THIS ONCE FOR EACH AREA.
--- RETURNS: Void
---
---@param squad SquadID
---@param durationSecs number
function FOW_RevealSquad(squad, durationSecs) end

--- Adds all items in a group to a given players FOW.
--- RETURNS: Void
---
---@param player PlayerID
---@param group SGroupID
function FOW_TagSGroup(player, group) end

--- Use to undo a FOW_RevealAll
--- RETURNS: Void
---
---@param Void any
function FOW_UnRevealAll(Void) end

--- UnReveals a circular area for all alive players. YOU SHOULD ONLY CALL THIS ONCE FOR EACH AREA.
--- RETURNS: Void
---
---@param x number
---@param z number
---@param radius number
function FOW_UnRevealArea(x, z, radius) end

--- Unreveals an area the size of a given markers proximity at that markers position. YOU SHOULD ONLY CALL THIS ONCE FOR EACH AREA. 
--- RETURNS: Void
---
---@param marker MarkerID
function FOW_UnRevealMarker(marker) end

--- Removes items from a players FOW that were added using FOW_TagSGroup
--- RETURNS: Void
---
---@param player PlayerID
---@param group SGroupID
function FOW_UnTagSGroup(player, group) end

--- Add a fallback location to the manager.
--- RETURNS: Void
---
---@param player PlayerID
---@param marker string
---@param markerlist table
function FallBack_AddLocation(player, marker, markerlist) end

---
---Located in FILE: \scar\fallback.scar LINE: 103
function FallBack_Init() end

---
---Located in FILE: \scar\fallback.scar LINE: 189
function FallBack_Print() end

--- Remove a destination from all fall backs for a player.
--- RETURNS: Void
---
---@param player PlayerID
---@param destination string
function FallBack_RemoveDestination(player, destination) end

--- Remove a fallback location from the manager.
--- RETURNS: Void
---
---@param player PlayerID
---@param marker string
function FallBack_RemoveLocation(player, marker) end

--- Restores various aspects of the single player game after loading a mission from a save game
--- RETURNS: Void
---
function Game_DefaultGameRestore() end

--- Enables/Disables all input EXCEPT for ESC and F10.
--- RETURNS: Void
---
---@param enabled boolean
function Game_EnableInput(enabled) end

--- DEPRECATED
--- RETURNS: Void
---
---@param Void any
function Game_EndCurrentSpeech(Void) end

--- Ends the single player game (win/lose). You can optionally specify a message that will show up in the gameover dialog box
--- RETURNS: Void
---
---@param win[ boolean
---@param message LocString
---@param nis] boolean
function Game_EndSP(win[, message, nis]) end

--- Fades the screen to black - FADE_OUT to fade to black, FADE_IN to fade back in
--- RETURNS: Void
---
---@param direction boolean
---@param length number
function Game_FadeToBlack(direction, length) end

--- Returns the aspect ratio used when in cinematic mode.  width / height
--- RETURNS: Real
---
---@param Void any
function Game_GetCinematicAspectRatio(Void) end

--- Get the local player. (should only be used for UI purpose)
--- RETURNS: PlayerID
---
---@param Void any
function Game_GetLocalPlayer(Void) end

--- Returns UIM_Normal, UIM_Fullscreen, or UIM_Cinematic
--- RETURNS: Integer
---
---@param Void any
function Game_GetMode(Void) end

--- Returns current single player difficulty. Values are GD_EASY, GD_NORMAL, GD_HARD, GD_EXPERT.
--- RETURNS: Integer
---
---@param Void any
function Game_GetSPDifficulty(Void) end

--- Determine if there is a valid local player. (UI only -- nondeterminstic)
--- RETURNS: Boolean
---
---@param Void any
function Game_HasLocalPlayer(Void) end

--- Removes a message added by Game_ShowSystemMessage.
--- RETURNS: Void
---
---@param msg LocString
function Game_HideSystemMessage(msg) end

--- Returns true if the ui is in letterbox mode
--- RETURNS: Boolean
---
---@param Void any
function Game_IsLetterboxed(Void) end

--- Use to test whether the game is running in RTM mode or not. Using -rtm from the command line will cause this function to also return true in non-RTM builds.
--- RETURNS: Boolean
---
---@param Void any
function Game_IsRTM(Void) end

--- Enters/Exits letterbox mode in amount of time specified by timeSecs.
--- RETURNS: Void
---
---@param on boolean
---@param timeSecs number
function Game_Letterbox(on, timeSecs) end

--- Replaces atmosphere with the one in the specified file
--- RETURNS: Void
---
---@param filename string
function Game_LoadAtmosphere(filename) end

--- DEPRECATED
--- RETURNS: Void
---
---@param icon string
---@param locID integer
---@param disableIconSubtitle boolean
---@param continueButton boolean
---@param lockCursorToContinueButton boolean
---@param stickySubtitle boolean
---@param additionalPath string
function Game_PlaySpeech(icon, locID, disableIconSubtitle, continueButton, lockCursorToContinueButton, stickySubtitle, additionalPath) end

--- DEPRECATED
--- RETURNS: Void
---
---@param squad SquadID
---@param icon string
---@param locID integer
---@param disableIconSubtitle boolean
---@param continueButton boolean
---@param lockCursorToContinueButton boolean
---@param stickySubtitle boolean
---@param additionalPath string
function Game_PlaySquadSpeech(squad, icon, locID, disableIconSubtitle, continueButton, lockCursorToContinueButton, stickySubtitle, additionalPath) end

--- Quits the app immediately
--- RETURNS: Void
---
---@param Void any
function Game_QuitApp(Void) end

--- Fades the screen to a given RGBA colour over a number of seconds
--- RETURNS: Void
---
---@param r number
---@param g number
---@param b number
---@param a number
---@param timeSecs number
function Game_ScreenFade(r, g, b, a, timeSecs) end

--- Sets the aspect ratio used when in cinematic mode.  Expects width / height.
--- RETURNS: Void
---
---@param ratio number
function Game_SetCinematicAspectRatio(ratio) end

--- Set the game screen mode
--- RETURNS: Void
---
---@param mode integer
function Game_SetMode(mode) end

--- A scale value for selection circle alpha
--- RETURNS: Void
---
---@param alphaScale number
function Game_SetSelectionAlphaScale(alphaScale) end

--- Brings up the pause menu. The game does not get paused until the end of the current sim tick, so anything that comes after Game_ShowPauseMenu in your function will still get executed, as well as any rules set to run during that frame.
--- RETURNS: Void
---
---@param Void any
function Game_ShowPauseMenu(Void) end

--- Shows a system message in the area where Game Paused text appears/
--- RETURNS: Void
---
---@param msg LocString
function Game_ShowSystemMessage(msg) end

--- Skips all events. Can either delete or skip queued events.
--- RETURNS: Void
---
---@param bDeleteQueued boolean
function Game_SkipAllEvents(bDeleteQueued) end

--- Skips the currently playing event (and stops current sound)
--- RETURNS: Void
---
---@param Void any
function Game_SkipEvent(Void) end

--- Fade in and out two lines of subtext.
--- RETURNS: Void
---
---@param line1 LocString
---@param line2 LocString
---@param in number
---@param lifetime number
---@param out number
function Game_SubTextFade(line1, line2, in, lifetime, out) end

--- Unlocks user/camera input during letterbox. This should be used for debug only
--- RETURNS: Void
---
---@param Void any
function Game_UnlockInputOnLetterBox(Void) end

--- Disable the spotting of enemy entities that may become ghosts in the FoW
--- RETURNS: Void
---
---@param Void any
function Ghost_DisableSpotting(Void) end

--- Enable the spotting of enemy entities that may become ghosts in the FoW
--- RETURNS: Void
---
---@param Void any
function Ghost_EnableSpotting(Void) end

--- Adds a Hint Point that will only appear on Mouseover of the target.
--- RETURNS: Void
---
---@param hintText LocString
---@param hintTarget Marker/Egroup/Sgroup
---@param targetRadius number
---@param looping boolean
function HintMouseover_Add(hintText, hintTarget, targetRadius, looping) end

--- internal function
---
---Located in FILE: \scar\ui.scar LINE: 184
function HintMouseover_Manager() end

--- Removes a Mouseover Hint Point from the managing function.
--- RETURNS: Void
---
---@param hintText LocString
---@param hintTarget Marker/Egroup/Sgroup
function HintMouseover_Remove(hintText, hintTarget) end

--- Creates a hintpoint attached to a Marker, EGroup, SGroup or position
--- RETURNS: HintPointID
---
---@param where StackVar
---@param bVisible boolean
---@param hintText LocString
function HintPoint_Add(where, bVisible, hintText) end

--- Adds a taskbar hintpoint to an ability button
--- RETURNS: Integer
---
---@param abilityBagID integer
---@param hint LocString
---@param removeOnClick boolean
function HintPoint_AddToAbilityButton(abilityBagID, hint, removeOnClick) end

--- Adds a taskbar hintpoint to a command button
--- RETURNS: Integer
---
---@param command integer
---@param hint LocString
---@param removeOnClick boolean
function HintPoint_AddToCommandButton(command, hint, removeOnClick) end

--- Adds a hintpoint to an upgrade on the company commander branch
--- RETURNS: Integer
---
---@param upgradeID integer
---@param hint LocString
function HintPoint_AddToCommanderBranchUpgrade(upgradeID, hint) end

--- Adds a hintpoint to an upgrade on the company commander branch
--- RETURNS: Integer
---
---@param upgradeID integer
---@param hint LocString
function HintPoint_AddToCommanderTreeUpgrade(upgradeID, hint) end

--- Adds a taskbar hintpoint to a construction button
--- RETURNS: Integer
---
---@param entityBlueprintID integer
---@param hint LocString
---@param removeOnClick boolean
function HintPoint_AddToConstructionButton(entityBlueprintID, hint, removeOnClick) end

--- Adds a taskbar hintpoint to a construction menu
--- RETURNS: Integer
---
---@param menuName string
---@param hint LocString
---@param removeOnClick boolean
function HintPoint_AddToConstructionMenu(menuName, hint, removeOnClick) end

--- Adds a hintpoint to an event cue that was created from SCAR
--- RETURNS: Integer
---
---@param eventCueID integer
---@param hint LocString
---@param removeOnClick boolean
function HintPoint_AddToEventCue(eventCueID, hint, removeOnClick) end

--- Adds a taskbar hintpoint to an objective icon
--- RETURNS: Integer
---
---@param objectiveID integer
---@param hint LocString
---@param removeOnClick boolean
function HintPoint_AddToObjectiveIcon(objectiveID, hint, removeOnClick) end

--- Adds a taskbar hintpoint to a production button
--- RETURNS: Integer
---
---@param type integer
---@param pbgID integer
---@param hint LocString
---@param removeOnClick boolean
function HintPoint_AddToProductionButton(type, pbgID, hint, removeOnClick) end

--- Adds a taskbar hintpoint to an item in the production queue
--- RETURNS: Integer
---
---@param index integer
---@param hint LocString
---@param removeOnClick boolean
function HintPoint_AddToProductionQueueItem(index, hint, removeOnClick) end

--- Adds a taskbar hintpoint to a squad's badge in the selection area, using the squad as the lookup
--- RETURNS: Integer
---
---@param squad SquadID
---@param hint LocString
---@param removeOnClick boolean
function HintPoint_AddToSquadBadge(squad, hint, removeOnClick) end

--- Adds a taskbar hintpoint to a squad's badge in the selection area, using the display index (from 0 to 9) as the lookup
--- RETURNS: Integer
---
---@param displayIndex integer
---@param hint LocString
---@param removeOnClick boolean
function HintPoint_AddToSquadBadgeByIndex(displayIndex, hint, removeOnClick) end

--- Adds a taskbar hintpoint to a squad control group (hotkey)
--- RETURNS: Integer
---
---@param controlGroupIndex integer
---@param hint LocString
---@param removeOnClick boolean
function HintPoint_AddToSquadControlGroup(controlGroupIndex, hint, removeOnClick) end

--- Adds a hintpoint to the tactical map, in world coordinates.
--- RETURNS: Integer
---
---@param position Position
---@param hint LocString
function HintPoint_AddToTacticalMap(position, hint) end

--- Adds a taskbar hintpoint to any binding, provided its name
--- RETURNS: Integer
---
---@param bindingName string
---@param hint LocString
---@param removeOnClick boolean
function HintPoint_AddToTaskbarBinding(bindingName, hint, removeOnClick) end

--- Removes a hintpoint.
--- RETURNS: Void
---
---@param HintPointID integer
function HintPoint_Remove(HintPointID) end

--- Removes all hintpoints.
--- RETURNS: Void
---
---@param Void any
function HintPoint_RemoveAll(Void) end

--- Sets a hintpoint's visibility. Currently, FOW is not accounted for.
--- RETURNS: Void
---
---@param HintPointID integer
---@param bVisible boolean
function HintPoint_SetVisible(HintPointID, bVisible) end

--- Creates a infopoint attached to a Marker, EGroup or SGroup.
--- RETURNS: HintPointID
---
---@param player PlayerID
---@param where StackVar
---@param range number
---@param bVisible boolean
---@param callback Luafunction
function InfoPoint_Add(player, where, range, bVisible, callback) end

--- Removes an infopoint.
--- RETURNS: Void
---
---@param HintPointID integer
function InfoPoint_Remove(HintPointID) end

--- Removes all infopoints.
--- RETURNS: Void
---
---@param Void any
function InfoPoint_RemoveAll(Void) end

--- Sets an infopoint's visiblity through FOW.
--- RETURNS: Void
---
---@param InfoPointID integer
---@param bVisible boolean
function InfoPoint_SetVisible(InfoPointID, bVisible) end

--------------------------------------------------------------------------
--- also describe the kind of challenges the player will face
--- give a brief description of win conditions and loose conditions if any 
--- Describe what the player is charged to do
--------------------------------------------------------------------------
--- [[ Objective Name ]]
--------------------------------------------------------------------------
---
---Located in FILE: \scar\missionname.scar LINE: 220
function Initialize_OBJ_ObjectiveName() end

--------------------------------------------------------------------------
--- they are healed.
--- near they will follow him. The objective is complete when
--- pinned from the opening NIS barrage. When the commander gets 
--- The player must send a commander out to rescue some riflemen
--------------------------------------------------------------------------
--- [[ Rescue the Wounded ]]
--------------------------------------------------------------------------
---
---Located in FILE: \scar\outfit_style_objectives.scar LINE: 23
function Initialize_OBJ_RescueWounded() end

--- DEV ONLY: Converts ansi text to localized text.
--- RETURNS: LocString
---
---@param text string
function LOC(text) end

--- Returns a localized string containing the number.
--- RETURNS: LocString
---
---@param number integer
function Loc_ConvertNumber(number) end

--- Empty string.
--- RETURNS: LocString
---
---@param Void any
function Loc_Empty(Void) end

--- Returns a formatted localized string.
--- RETURNS: LocString
---
---@param FormatID integer
function Loc_FormatText(FormatID) end

--- Returns a formatted time string. can omit hours and leading zeroes (for example, 4:57 instead of 00:04:57)
--- RETURNS: LocString
---
---@param secs integer
---@param show_hours boolean
---@param leading_zeroes boolean
function Loc_FormatTime(secs, show_hours, leading_zeroes) end

---------------------------------------------------------------------------------
---------------------------------------------------------------------------------
---
--- (c) 2006 Relic Entertainment
--- 
---
--- Defines variables to be used for important Lua defined contsants
--- LUA CONSTANTS
---
---------------------------------------------------------------------------------
---
---Located in FILE: \scar\luaconsts.scar LINE: 12
function LuaConsts_Init() end

--- this is a precautionary action - deg
--- Scar commands used before initialization can cause crashes
--- delaying the variables that rely on Scar commands
---
---Located in FILE: \scar\luaconsts.scar LINE: 308
function LuaConsts_OnLoad() end

---
---Located in FILE: \scar\missionname.scar LINE: 86
function M10_Set_StatMods() end

---
---Located in FILE: \scar\missionname.scar LINE: 429
---@param group any
---@param pos any
function M99_CreateInfantryResponse_Med(group, pos) end

---
---Located in FILE: \scar\missionname.scar LINE: 194
function M99_DelayOBJ_ObjectiveName() end

---
---Located in FILE: \scar\missionname.scar LINE: 142
function M99_MOOD() end

---
---Located in FILE: \scar\missionname.scar LINE: 158
function M99_Mission_Preset() end

---
---Located in FILE: \scar\missionname.scar LINE: 179
function M99_Mission_Start() end

---
---Located in FILE: \scar\missionname.scar LINE: 76
function M99_Objective_Register() end

---
---Located in FILE: \scar\missionname.scar LINE: 102
function M99_Restrictions() end

---
---Located in FILE: \scar\simtest.scar LINE: 16
---@param groupName any
---@param playerID any
---@param team_sign any
function MakeArmy(groupName, playerID, team_sign) end

---
---Located in FILE: \scar\deprecated.scar LINE: 677
---@param playerid any
---@param list any
---@param regularebp any
---@param finalebp any
function MarchTerritoryForwards(playerid, list, regularebp, finalebp) end

---
---Located in FILE: \scar\deprecated.scar LINE: 691
function MarchTerritoryForwards_Manager() end

--- Returns true if a generic number attribute exists for the marker type.
--- RETURNS: Boolean
---
---@param marker MarkerID
---@param attrname string
function Marker_DoesNumberAttributeExist(marker, attrname) end

--- Returns true if a generic string attribute exists for the marker type.
--- RETURNS: Boolean
---
---@param marker MarkerID
---@param attrname string
function Marker_DoesStringAttributeExist(marker, attrname) end

--- Returns true if marker exists. If you don't care about the type, pass in an empty string ( "" )
--- RETURNS: Boolean
---
---@param name string
---@param type string
function Marker_Exists(name, type) end

--- Returns a ScarMarker from the Mission Editor. If you don't care about the type, pass in an empty string ( "" )
--- RETURNS: MarkerID
---
---@param name string
---@param type string
function Marker_FromName(name, type) end

--- Returns a vector for the marker direction
--- RETURNS: Position
---
---@param marker MarkerID
function Marker_GetDirection(marker) end

--- Returns the name of a given marker.  This value gets set in the Mission Editor.
--- RETURNS: String
---
---@param marker MarkerID
function Marker_GetName(marker) end

--- Returns a fixed size table of markers from the world builder.  Markers that do not exist in the WB, will be nil in the table.  This is why we call it 'non-sequential'
--- RETURNS: LuaTable
---
---@param format string
---@param size integer
function Marker_GetNonSequentialTable(format, size) end

--- Returns a generic number attribute defined in a marker.
--- RETURNS: Real
---
---@param marker MarkerID
---@param attrname string
function Marker_GetNumberAttribute(marker, attrname) end

--- Returns the position of a given marker.
--- RETURNS: Position
---
---@param marker MarkerID
function Marker_GetPosition(marker) end

--- Returns the proximity radius of a given marker.  Only for marker with proximity type PT_Circle. This value gets set in the Mission Editor.
--- RETURNS: Real
---
---@param marker MarkerID
function Marker_GetProximityRadius(marker) end

--- Returns the proximity type of a given marker.  The possible results are PT_Circle and PT_Rectangle
--- RETURNS: Integer
---
---@param marker MarkerID
function Marker_GetProximityType(marker) end

--- Returns a random position within the marker's proximity. Range is ignored for rectangular markers
--- RETURNS: Pos
---
---@param marker[ MarkerID/Pos
---@param range] number
function Marker_GetRandomPosition(marker[, range]) end

--- Builds a table of MarkerIDs that are named in a sequence. i.e. a name of "spot" will find markers "spot1", "spot2" and so on, up until it looks for a marker that isn't there.
--- RETURNS: Table
---
---@param name string
function Marker_GetSequence(name) end

--- Returns a generic string attribute defined in a marker.
--- RETURNS: String
---
---@param marker MarkerID
---@param attrname string
function Marker_GetStringAttribute(marker, attrname) end

--- Returns a table of markers from the world builder. Creates as many as it finds
--- RETURNS: LuaTable
---
---@param format string
function Marker_GetTable(format) end

--- Returns the typename of a given marker.  This is the typename from the Mission Editor (name displayed when placing markers)
--- RETURNS: String
---
---@param marker MarkerID
function Marker_GetType(marker) end

--- Returns true if the given position is in the markers proximity radius or proximity rectangle (depending on the type).
--- RETURNS: Boolean
---
---@param marker MarkerID
---@param pos Position
function Marker_InProximity(marker, pos) end

--- Let AI take over local player
--- RETURNS: Boolean
---
---@param Void any
function Misc_AIControlLocalPlayer(Void) end

--- Abort straight out of the game to the frontend, without asking the user
--- RETURNS: Void
---
---@param Void any
function Misc_AbortToFE(Void) end

--- Returns the enabled/disabled state of the right-click command input.
--- RETURNS: Boolean
---
---@param Void any
function Misc_AreDefaultCommandsEnabled(Void) end

--- Returns true if the app has had any keyboard input in the last second
--- RETURNS: Boolean
---
---@param Void any
function Misc_DetectKeyboardInput(Void) end

--- Returns true if the app has had any mouse input in the last second
--- RETURNS: Boolean
---
---@param Void any
function Misc_DetectMouseInput(Void) end

--- Do weapon hit effect on the entity from the view camera origin
--- RETURNS: Void
---
---@param entity EntityID
---@param pos Position
---@param weaponID integer
---@param penetrated boolean
function Misc_DoWeaponHitEffectOnEntity(entity, pos, weaponID, penetrated) end

--- Do weapon hit effect on the ground
--- RETURNS: Void
---
---@param pos Position
---@param weaponID integer
---@param penetrated boolean
function Misc_DoWeaponHitEffectOnPosition(pos, weaponID, penetrated) end

--- Turn on or off the performance test monitoring
--- RETURNS: Void
---
---@param toEnable boolean
function Misc_EnablePerformanceTest(toEnable) end

--- Returns the list of all entities that's not part of a squad from the player on the screen
--- RETURNS: Void
---
---@param player PlayerID
---@param egroup EGroupID
function Misc_GetAllPlayerEntitiesOnScreen(player, egroup) end

--- Returns the list of all squads from the player on screen
--- RETURNS: Void
---
---@param player PlayerID
---@param sgroup SGroupID
function Misc_GetAllPlayerSquadsOnScreen(player, sgroup) end

--- Returns a hidden position on path from origin to destination. If there's none, it returns the origin position
--- RETURNS: Position
---
---@param checkType integer
---@param origin Position
---@param dest Position
---@param ebpID integer
---@param stepDistance number
---@param cameraPadding number
---@param FOWPlayer PlayerID
---@param debugDisplay boolean
function Misc_GetHiddenPositionOnPath(checkType, origin, dest, ebpID, stepDistance, cameraPadding, FOWPlayer, debugDisplay) end

--- Returns the world position of the mouse on the terrain
--- RETURNS: Math::Vector3f
---
---@param Void any
function Misc_GetMouseOnTerrain(Void) end

--- Returns the entity under the mouse (if any)
--- RETURNS: EntityID
---
---@param Void any
function Misc_GetMouseOverEntity(Void) end

--- Returns the full path name to the main SCAR script
--- RETURNS: String
---
---@param Void any
function Misc_GetScarFullFilename(Void) end

--- Clears a given egroup and adds the current selection to the group
--- RETURNS: Void
---
---@param group EGroupID
---@param subSelection boolean
function Misc_GetSelectedEntities(group, subSelection) end

--- Clears a given sgroup and adds the current selection to the group
--- RETURNS: Void
---
---@param group SGroupID
---@param subSelection boolean
function Misc_GetSelectedSquads(group, subSelection) end

--- Returns the control group index that this squad belongs to, from 0 to 9, or -1 if none
--- RETURNS: Integer
---
---@param squad SquadID
function Misc_GetSquadControlGroup(squad) end

--- Returns true if -option is specified on the command line (don't pass the dash in the string)
--- RETURNS: Boolean
---
---@param option string
function Misc_IsCommandLineOptionSet(option) end

--- Returns true if ANY or ALL of the EGroup is selected
--- RETURNS: Boolean
---
---@param egroup EGroupID
---@param all boolean
function Misc_IsEGroupSelected(egroup, all) end

--- Check if the entity is on screen currently
--- RETURNS: Boolean
---
---@param entity EntityID
---@param pct number
function Misc_IsEntityOnScreen(entity, pct) end

--- Returns true if an entity is currently selected
--- RETURNS: Boolean
---
---@param entity EntityID
function Misc_IsEntitySelected(entity) end

--- Returns true if the mouse is over an entity
--- RETURNS: Boolean
---
---@param Void any
function Misc_IsMouseOverEntity(Void) end

--- Check if position is on screen
--- RETURNS: Boolean
---
---@param pos Position
function Misc_IsPosOnScreen(pos) end

--- Returns true if ANY or ALL of the SGroup is selected
--- RETURNS: Boolean
---
---@param sgroup SGroupID
---@param all boolean
function Misc_IsSGroupSelected(sgroup, all) end

--- Returns the enabled/disabled state of the selection input.
--- RETURNS: Boolean
---
---@param Void any
function Misc_IsSelectionInputEnabled(Void) end

--- Check if the squad is on screen currently
--- RETURNS: Boolean
---
---@param squad SquadID
---@param pct number
function Misc_IsSquadOnScreen(squad, pct) end

--- Returns true if a squad is currently selected
--- RETURNS: Boolean
---
---@param squad SquadID
function Misc_IsSquadSelected(squad) end

--- Add or remove squad members from the current selection
--- RETURNS: Void
---
---@param squad SquadID
---@param bSelected boolean
function Misc_SelectSquad(squad, bSelected) end

--- Enables / Disables right-click command input.
--- RETURNS: Void
---
---@param enabled boolean
function Misc_SetDefaultCommandsEnabled(enabled) end

--- Shows or hides designer splats, which are splats in the UI folder
--- RETURNS: Void
---
---@param bVisible boolean
function Misc_SetDesignerSplatsVisibility(bVisible) end

--- Enables / Disables selection input.
--- RETURNS: Void
---
---@param enabled boolean
function Misc_SetSelectionInputEnabled(enabled) end

--- Makes a squad belong to a specific control group. If it already belongs to another control group, it's removed from that one before being added to the new one.
--- RETURNS: Boolean
---
---@param squad SquadID
---@param groupIndex integer
function Misc_SetSquadControlGroup(squad, groupIndex) end

---
---Located in FILE: \scar\designerlib.scar LINE: 1187
---@param playerID any
---@param sgroupTable any
---@param mobName any
---@param sourceTable any
---@param destinationTable any
---@param dedication any
function MobRule_AddMob(playerID, sgroupTable, mobName, sourceTable, destinationTable, dedication) end

---
---Located in FILE: \scar\designerlib.scar LINE: 1293
function MobRule_AssaultMob_Manager() end

--- gathering the strike team
---[[ BUILD PHASE ]]
---
---Located in FILE: \scar\designerlib.scar LINE: 1206
function MobRule_BuildMob_Manager() end

---
---Located in FILE: \scar\designerlib.scar LINE: 1267
---@param player any
---@param group any
---@param blue any
---@param source any
function MobRule_CreateMobMember(player, group, blue, source) end

---
---Located in FILE: \scar\mob_useage.scar LINE: 6
function Mob_Test() end

---
---Located in FILE: \scar\mob_useage.scar LINE: 14
function Mob_Trigger() end

--- Make the passed entity a casualty by triggering the MakeCasualtyAction
--- RETURNS: Void
---
---@param pTargetEntity EntityID
function ModMisc_MakeCasualtyAction(pTargetEntity) end

--- Make the passed entity a wreck by triggering the MakeWreckAction
--- RETURNS: Void
---
---@param pTargetEntity EntityID
function ModMisc_MakeWreckAction(pTargetEntity) end

--- Make the passed entity go out of control
--- RETURNS: Void
---
---@param pTargetEntity EntityID
function ModMisc_OOCAction(pTargetEntity) end

---
---Located in FILE: \scar\deprecated.scar LINE: 395
---@param groupid any
---@param Modifier any
---@param apply any
function ModifierUtil_ApplyToEGroup(groupid, Modifier, apply) end

---
---Located in FILE: \scar\deprecated.scar LINE: 378
---@param groupid any
---@param Modifier any
---@param apply any
function ModifierUtil_ApplyToSGroup(groupid, Modifier, apply) end

---
---Located in FILE: \scar\deprecated.scar LINE: 349
---@param Modifier any
---@param entityid any
function ModifierUtil_GetEntityApplierIndex(Modifier, entityid) end

---
---Located in FILE: \scar\deprecated.scar LINE: 355
---@param Modifier any
---@param playerid any
function ModifierUtil_GetPlayerApplierIndex(Modifier, playerid) end

---
---Located in FILE: \scar\deprecated.scar LINE: 343
---@param Modifier any
---@param squadid any
function ModifierUtil_GetSquadApplierIndex(Modifier, squadid) end

---
---Located in FILE: \scar\deprecated.scar LINE: 365
---@param Modifier any
---@param entityid any
function ModifierUtil_IsEntityModifierApplied(Modifier, entityid) end

---
---Located in FILE: \scar\deprecated.scar LINE: 369
---@param Modifier any
---@param playerid any
function ModifierUtil_IsPlayerModifierApplied(Modifier, playerid) end

---
---Located in FILE: \scar\deprecated.scar LINE: 361
---@param Modifier any
---@param squadid any
function ModifierUtil_IsSquadModifierApplied(Modifier, squadid) end

--- this function adds a modifier id to the global modifier storage table
---
---Located in FILE: \scar\modifiers.scar LINE: 42
---@param entity any
---@param modid any
function Modifier_AddToEntityTable(entity, modid) end

--- this function adds a modifier id to the global modifier storage table
---
---Located in FILE: \scar\modifiers.scar LINE: 58
---@param modid any
function Modifier_AddToMiscTable(modid) end

--- this function adds a modifier id to the global modifier storage table
---
---Located in FILE: \scar\modifiers.scar LINE: 27
---@param squad any
---@param modid any
function Modifier_AddToSquadTable(squad, modid) end

--- Applies an entity modifier to an entity.
--- RETURNS: Integer
---
---@param modifier ScarModifier
---@param entity EntityID
function Modifier_ApplyToEntity(modifier, entity) end

--- Applies a player modifier to a player.
--- RETURNS: Integer
---
---@param modifier ScarModifier
---@param player PlayerID
function Modifier_ApplyToPlayer(modifier, player) end

--- Applies a squad modifier to a squad.
--- RETURNS: Integer
---
---@param modifier ScarModifier
---@param squad SquadID
function Modifier_ApplyToSquad(modifier, squad) end

--- Returns a modifier that you can apply to stuff.
--- RETURNS: ScarModifier
---
---@param applicationType integer
---@param modtype string
---@param usageType integer
---@param exclusive boolean
---@param value number
---@param v LuaBinding::StackVar
function Modifier_Create(applicationType, modtype, usageType, exclusive, value, v) end

--- Todo.
--- RETURNS: Void
---
---@param modifier ScarModifier
function Modifier_Destroy(modifier) end

--- create the blank modifier storage table at the start of a mission
---
---Located in FILE: \scar\modifiers.scar LINE: 14
function Modifier_Init() end

--- Remove an applied modifier. 
--- RETURNS: Void
---
---@param modifier ModID
function Modifier_Remove(modifier) end

--- Removes all SCAR-applied modifiers for a specific EGroup. 
--- RETURNS: Void
---
---@param egroup EGroupID
function Modifier_RemoveAllFromEGroup(egroup) end

--- Removes all SCAR-applied modifiers for a specific SGroup. 
--- RETURNS: Void
---
---@param sgroup SGroupID
function Modifier_RemoveAllFromSGroup(sgroup) end

--- Modifies the recharge time of an ability
--- RETURNS: ModID
---
---@param player PlayerID
---@param ability AbilityID
---@param scalefactor number
function Modify_AbilityRechargeTime(player, ability, scalefactor) end

--- Modifies the capture time of all strategic points in an EGroup
--- RETURNS: ModID
---
---@param sgroup EGroupID
---@param scalefactor number
function Modify_CaptureTime(sgroup, scalefactor) end

--- Enable or disable hold (garrisoning) for an egroup or sgroup
--- RETURNS: ModID
---
---@param group EGroupID
---@param disable boolean
function Modify_DisableHold(group, disable) end

---
---Located in FILE: \scar\deprecated.scar LINE: 435
---@param egroupid any
---@param factor any
function Modify_EGroupSightRadius(egroupid, factor) end

--- Allows paratroopers to reinforce from the sky.  Set to true to enable, false to disable.
--- RETURNS: 
---
---@param playerId PlayerID
---@param enable boolean
function Modify_Enable_ParadropReinforcements(playerId, enable) end

--- Modifies the time taken to build a particular EBP. This only affects the given player. 
--- RETURNS: ModID
---
---@param playerId PlayerID
---@param ebp string
---@param scalefactor number
function Modify_EntityBuildTime(playerId, ebp, scalefactor) end

--- Modifies the cost of an entity for a particular player. Possible resource types are RT_Manpower, RT_Munition, RT_Fuel, RT_Action 
--- RETURNS: ModID
---
---@param player PlayerID
---@param blueprint string
---@param resourcetype integer
---@param addition integer
function Modify_EntityCost(player, blueprint, resourcetype, addition) end

---
---Located in FILE: \scar\deprecated.scar LINE: 775
---@param group any
---@param addition any
function Modify_EntityTargetPriority(group, addition) end

--- Modifies the veterancy experience received by a player
--- RETURNS: ModID
---
---@param player PlayerID
---@param factor number
function Modify_PlayerExperienceReceived(player, factor) end

--- Modifies the production rate of a player.
--- RETURNS: ModID
---
---@param sgroup PlayerID
---@param scalefactor number
function Modify_PlayerProductionRate(sgroup, scalefactor) end

--- Modifies a player's resource bonus received (ie. one-time resource gifts) Possible resource types are RT_Manpower, RT_Munition, RT_Fuel, RT_Action 
--- RETURNS: ModID
---
---@param playerId PlayerID
---@param resourceType integer
---@param scalefactor number
function Modify_PlayerResourceGift(playerId, resourceType, scalefactor) end

--- Modifies a player's incoming resource rate. Possible resource types are RT_Manpower, RT_Munition, RT_Fuel, RT_Action 
--- RETURNS: ModID
---
---@param playerId PlayerID
---@param resourceType integer
---@param scalefactor number
function Modify_PlayerResourceRate(playerId, resourceType, scalefactor) end

--- Modifies the sight radius for a player.
--- RETURNS: ModID
---
---@param player PlayerID
---@param scalefactor number
function Modify_PlayerSightRadius(player, scalefactor) end

--- Modifies the production rate of all factories in an EGroup
--- RETURNS: ModID
---
---@param sgroup EGroupID
---@param scalefactor number
function Modify_ProductionRate(sgroup, scalefactor) end

--- Modifies the chance of a squad/entity being hit
--- RETURNS: ModID
---
---@param group SGroupID/EGroupID
---@param scalefactor number
function Modify_ReceivedAccuracy(group, scalefactor) end

--- Modifies the damage a squad/entity receives.
--- RETURNS: ModID
---
---@param group SGroupID/EGroupID
---@param scalefactor number
function Modify_ReceivedDamage(group, scalefactor) end

--- Modifies the rate at which a squad gets suppressed
--- RETURNS: ModID
---
---@param sgroup SGroupID
---@param scalefactor number
function Modify_ReceivedSuppression(sgroup, scalefactor) end

---
---Located in FILE: \scar\deprecated.scar LINE: 419
---@param sgroupid any
---@param factor any
function Modify_SGroupSightRadius(sgroupid, factor) end

--- Sets the cost of an upgrade. This only affects the given player
--- RETURNS: ModID
---
---@param playerId PlayerID
---@param upgrade UpgradeID
---@param resourceType integer
---@param newCost number
function Modify_SetUpgradeCost(playerId, upgrade, resourceType, newCost) end

--- Modifies the sight radius for an egroup or an sgroup.
--- RETURNS: ModID
---
---@param group SGroupID/EGroupID
---@param scalefactor number
function Modify_SightRadius(group, scalefactor) end

--- Modifies the availability limit of a squad type for any given player
--- RETURNS: ModID
---
---@param player PlayerID
---@param blueprint string
---@param addition integer
function Modify_SquadAvailability(player, blueprint, addition) end

--- Modifies the target priority of a squad or entity group from the attacker. The value is an addition
--- RETURNS: ModID
---
---@param group SGroupID/EGroupID
---@param addition integer
function Modify_TargetPriority(group, addition) end

--- Modifies the territory radius for an egroup or an sgroup.
--- RETURNS: ModID
---
---@param group EGroupID
---@param scalefactor number
function Modify_TerritoryRadius(group, scalefactor) end

--- Modifies the maximum speed for a vehicle. This has no effect on infantry.
--- RETURNS: ModID
---
---@param sgroup SGroupID
---@param scalefactor number
function Modify_UnitSpeed(sgroup, scalefactor) end

--- Modifies the build time for a particular upgrade. This only affects the given player. 
--- RETURNS: ModID
---
---@param playerId PlayerID
---@param upgrade UpgradeID
---@param scalefactor number
function Modify_UpgradeBuildTime(playerId, upgrade, scalefactor) end

--- Modifies the upkeep for a player 
--- RETURNS: ModID
---
---@param playerId PlayerID
---@param scalefactor number
function Modify_Upkeep(playerId, scalefactor) end

--- Modifies the vehicle repair rate of all a player's engineers
--- RETURNS: ModID
---
---@param player PlayerID
---@param factor[ number
---@param engineer_blueprint] string
function Modify_VehicleRepairRate(player, factor[, engineer_blueprint]) end

--- Increases the received accuracy, penetration, and damage on a squad by the scalefactor.  For example, a scalefactor of 2 means that the squad gets 2x the received accuracy, 2x the received penetration, and 2x the received damage.
--- RETURNS: ModID
---
---@param sgroup SGroupID
---@param scalefactor number
function Modify_Vulnerability(sgroup, scalefactor) end

--- Modifies a squad's weapon accuracy.
--- RETURNS: ModID
---
---@param group SGroupID/EGroupID
---@param hardpoint string
---@param scalefactor number
function Modify_WeaponAccuracy(group, hardpoint, scalefactor) end

--- Modifies a squad's weapon burst time.
--- RETURNS: ModID
---
---@param group SGroupID/EGroupID
---@param hardpoint string
---@param scalefactor number
function Modify_WeaponBurst(group, hardpoint, scalefactor) end

--- Modifies a squad's weapon cooldown time.
--- RETURNS: ModID
---
---@param group SGroupID/EGroupID
---@param hardpoint string
---@param scalefactor number
function Modify_WeaponCooldown(group, hardpoint, scalefactor) end

--- Modifies a squad's weapon damage.
--- RETURNS: ModID
---
---@param group SGroupID/EGroupID
---@param hardpoint string
---@param scalefactor number
function Modify_WeaponDamage(group, hardpoint, scalefactor) end

--- Modifies a squad's weapon range.
--- RETURNS: ModID
---
---@param group SGroupID/EGroupID
---@param hardpoint string
---@param scalefactor number
function Modify_WeaponRange(group, hardpoint, scalefactor) end

--- Plays subtitle during NISes.
--- RETURNS: Void
---
---@param locID integer
---@param speechTime number
function NIS_PlaySpeech(locID, speechTime) end

--- start the related action
---
---Located in FILE: \scar\missionname.scar LINE: 292
function OBJ_NAME_ActionKickoff() end

---
---Located in FILE: \scar\missionname.scar LINE: 272
function OBJ_NAME_WinConditionCheck() end

--- grant the objective when the player gets near the wounded
---
---Located in FILE: \scar\outfit_style_objectives.scar LINE: 96
function OBJ_RW_Grant_PlayerProximity() end

--- detect the health/status of the wounded squad memebers
---
---Located in FILE: \scar\outfit_style_objectives.scar LINE: 107
function OBJ_RW_Wounded_DetectState() end

--- once the presets are dead, the guards will respawn to more retreated positions
--- guards created in the preset, focus is on the area 'around' the town center
---
---Located in FILE: \scar\missionname.scar LINE: 331
function OBJ_TT_Defense1() end

--- first step to making the axis defenders 'around' the town retreat
---
---Located in FILE: \scar\missionname.scar LINE: 356
function OBJ_TT_Defense_Manager1() end

--- Create an on screen event cue to alert new sit rep
--- RETURNS: Void
---
---@param player PlayerID
---@param id integer
function Obj_AlertSitRep(player, id) end

--- Create an objective and returns the unique ID for it
--- RETURNS: Integer
---
---@param title LocString
---@param desc LocString
---@param icon string
---@param type integer
---@param medalPBG integer
function Obj_Create(title, desc, icon, type, medalPBG) end

--- Get objective state ( OS_Off, OS_Incomplete, OS_Complete, OS_Failed )
--- RETURNS: Integer
---
---@param objectiveID integer
function Obj_GetState(objectiveID) end

--- Get objective visibility
--- RETURNS: Boolean
---
---@param objectiveID integer
function Obj_GetVisible(objectiveID) end

--- Set description text localization ID for the objective
--- RETURNS: Void
---
---@param objectiveID integer
---@param desc LocString
function Obj_SetDescription(objectiveID, desc) end

--- Set icon path for the objective
--- RETURNS: Void
---
---@param objectiveID integer
---@param icon string
function Obj_SetIcon(objectiveID, icon) end

--- Set callback functions for the objective. 
--- RETURNS: Void
---
---@param id integer
---@param fnType integer
---@param f function
function Obj_SetObjectiveFunction(id, fnType, f) end

--- Set objective state ( OS_Off, OS_Incomplete, OS_Complete, OS_Failed )
--- RETURNS: Void
---
---@param objectiveID integer
---@param state integer
function Obj_SetState(objectiveID, state) end

--- Set title text localization ID for the objective
--- RETURNS: Void
---
---@param objectiveID integer
---@param title LocString
function Obj_SetTitle(objectiveID, title) end

--- Set objective visibility
--- RETURNS: Void
---
---@param objectiveID integer
---@param visible boolean
function Obj_SetVisible(objectiveID, visible) end

--- Adds a tactical map arrow to an objective
--- RETURNS: ArrowID
---
---@param objectiveTable LuaTable
---@param from Position
---@param to Position
function Objective_AddArrow(objectiveTable, from, to) end

--- Adds a tactical map label to an objective
--- RETURNS: LabelID
---
---@param objectiveTable LuaTable
---@param pos Position
---@param text LocString
function Objective_AddLabel(objectiveTable, pos, text) end

--- Adds a tactical map ping to an objective
--- RETURNS: PingID
---
---@param objectiveTable LuaTable
---@param pos Position
---@param radius number
function Objective_AddPing(objectiveTable, pos, radius) end

--- Adds a tactical map pointer to an objective
--- RETURNS: PointerID
---
---@param objectiveTable LuaTable
---@param pos Position
function Objective_AddPointer(objectiveTable, pos) end

--- Adds multiple UI elements on one position. pass in 'false' for elements that you don't want. 'pos' can be group/entity/squad/marker. Setting 'worldHighlight' to true makes this entity/squad/group be highlighted in the world. objectiveArrowOffset is an optional offset applied to the arrow's position (you can specify a height offset or a 3D position offset).
--- RETURNS: ElementID
---
---@param objTable LuaTable
---@param pos Position
---@param ping boolean
---@param hintpointText LocString
---@param arrowFrom Position
---@param pointer[ boolean
---@param worldHighlight boolean
---@param objectiveArrowOffset] Float/Position
function Objective_AddUIElements(objTable, pos, ping, hintpointText, arrowFrom, pointer[, worldHighlight, objectiveArrowOffset]) end

--- Drops an event cue for player 1, allowing them to access the sitrep for the objective
--- RETURNS: Void
---
---@param objTable LuaTable
function Objective_AlertSitRep(objTable) end

--- Returns whether all primary objectives have been completed.
--- RETURNS: Boolean
---
function Objective_AreAllPrimaryObjectivesComplete() end

--- 'Completes' an objective. Wrapper function for Objective_SetState with a few other features. If you do not want the objective title to be shown on screen, pass in 'false' for bShowTitle
--- RETURNS: Void
---
---@param objTable[ LuaTable
---@param bShowTitle] boolean
function Objective_Complete(objTable[, bShowTitle]) end

--- 'Fails' an objective. Wrapper function for Objective_SetState with a few other features.
--- RETURNS: Void
---
---@param objTable[ LuaTable
---@param bShowTitle] boolean
function Objective_Fail(objTable[, bShowTitle]) end

--- Brings you to the tactical map screen and plays the sitrep for this objective
--- RETURNS: Void
---
---@param objTable LuaTable
function Objective_ForceSitRep(objTable) end

--- Returns the current count associated with this objective.
--- RETURNS: Integer
---
---@param objTable LuaTable
function Objective_GetCounter(objTable) end

--- Returns the amount of seconds on the timer (time remaining or time elapsed, based on the type of timer used)
--- RETURNS: Integer
---
---@param objTable LuaTable
function Objective_GetTimerSeconds(objTable) end

--- Returns whether an objective is complete
--- RETURNS: Boolean
---
---@param objTable LuaTable
function Objective_IsComplete(objTable) end

--- Returns true if a counter has been set for this objective
--- RETURNS: Boolean
---
---@param objTable LuaTable
function Objective_IsCounterSet(objTable) end

--- Returns whether an objective is failed
--- RETURNS: Boolean
---
---@param objTable LuaTable
function Objective_IsFailed(objTable) end

--- Returns whether an objective has been started. Completed objectives will return true.
--- RETURNS: Void
---
---@param objTable LuaTable
function Objective_IsStarted(objTable) end

--- Returns true if a timer has been set for this objective
--- RETURNS: Boolean
---
---@param objTable LuaTable
function Objective_IsTimerSet(objTable) end

--- Pauses the objective's timer. If a timer has not been set, it does nothing.
--- RETURNS: Void
---
---@param objTable LuaTable
function Objective_PauseTimer(objTable) end

--- 'Registers' an objective. Wrapper function for Objective_Create with a few other features.
--- RETURNS: ObjectiveID
---
---@param objTable LuaTable
function Objective_Register(objTable) end

--- Removes a tactical map arrow from an objective
--- RETURNS: Void
---
---@param objectiveTable LuaTable
---@param ArrowID integer
function Objective_RemoveArrow(objectiveTable, ArrowID) end

--- Removes a tactical map label from an objective
--- RETURNS: Void
---
---@param objectiveTable LuaTable
---@param LabelID integer
function Objective_RemoveLabel(objectiveTable, LabelID) end

--- Removes a tactical map ping from an objective
--- RETURNS: Void
---
---@param objectiveTable LuaTable
---@param PingID integer
function Objective_RemovePing(objectiveTable, PingID) end

--- Removes a tactical map pointer from an objective
--- RETURNS: Void
---
---@param objectiveTable LuaTable
---@param PointerID integer
function Objective_RemovePointer(objectiveTable, PointerID) end

--- Removes a group of UI elements that were added by Objective_AddUIElements
--- RETURNS: Void
---
---@param objTable LuaTable
---@param elementID integer
function Objective_RemoveUIElements(objTable, elementID) end

--- Resume the objective's timer. If a timer has not been set, it does nothing.
--- RETURNS: Void
---
---@param objTable LuaTable
function Objective_ResumeTimer(objTable) end

--- Sets whether this objective always shows detailed text, the HUD arrow, or the hintpoints. There can only be one objective at a time that forces the HUD arrow to show up. If you pass in 'nil' for hud_arrow then its behavior is not affected.
--- RETURNS: Void
---
---@param objTable LuaTable
---@param title boolean
---@param hud_arrow boolean
---@param hintpoints boolean
function Objective_SetAlwaysShowDetails(objTable, title, hud_arrow, hintpoints) end

--- Sets a counter that is associated with this objective in the UI. You can provide a 'maximum' so that it shows up as "1 of 5"
--- RETURNS: Void
---
---@param objTable LuaTable
---@param current number
---@param maximum number
function Objective_SetCounter(objTable, current, maximum) end

--- Shows or hides an objective from the UI and tactical map
--- RETURNS: Void
---
---@param objective_table LuaTable
---@param on/off boolean
function Objective_Show(objective_table, on/off) end

--- Shows an objective to the player and activates it
--- RETURNS: Void
---
---@param objTable[ LuaTable
---@param bShowTitle] boolean
function Objective_Start(objTable[, bShowTitle]) end

--- Starts a timer that is associated with this objective in the UI. Use COUNT_DOWN or COUNT_UP for the 'direction' parameter
--- RETURNS: Void
---
---@param objTable LuaTable
---@param direction[ integer
---@param initialTime number
---@param flashThreshold] number
function Objective_StartTimer(objTable, direction[, initialTime, flashThreshold]) end

--- Stops the objective's counter. If a counter has not been set, it does nothing.
--- RETURNS: Void
---
---@param objTable LuaTable
function Objective_StopCounter(objTable) end

--- Stops the objective's timer. If a timer has not been set, it does nothing.
--- RETURNS: Void
---
---@param objTable LuaTable
function Objective_StopTimer(objTable) end

--- Toggles minimap blips on or off.
--- RETURNS: Void
---
---@param objective_table LuaTable
---@param on/off boolean
function Objective_TogglePings(objective_table, on/off) end

--- Updates the title and description for the objective. If you only want to set one of them, pass in nil for the other
--- RETURNS: Void
---
---@param objTable LuaTable
---@param title LocString
---@param description[ LocString
---@param bShowTitle] boolean
function Objective_UpdateText(objTable, title, description[, bShowTitle]) end

---
---Located in FILE: \scar\missionname.scar LINE: 32
function OnGameRestore() end

---
---Located in FILE: \scar\missionname.scar LINE: 22
function OnGameSetup() end

---
---Located in FILE: \scar\missionname.scar LINE: 49
function OnInit() end

---
---Located in FILE: \scar\missionname_idtables.scar LINE: 1
function OnInitID() end

---
---Located in FILE: \scar\deprecated.scar LINE: 513
---@param sgroupid any
---@param pathname any
---@param startpoint any
function Path_AddSquadsToPath(sgroupid, pathname, startpoint) end

---
---Located in FILE: \scar\deprecated.scar LINE: 493
---@param pathname any
---@param pathmarkers any
---@param pathloop any
function Path_CreatePath(pathname, pathmarkers, pathloop) end

---
---Located in FILE: \scar\deprecated.scar LINE: 561
---@param pathname any
function Path_GetNumberOfSquadsOnPath(pathname) end

--- Path Manager initialisation function
---
---Located in FILE: \scar\deprecated.scar LINE: 668
function Path_Init() end

--- Retrieve a custom value in current player profile. Returns an empty string if value does not exist
--- RETURNS: String
---
---@param varname string
function PlayerProfile_GetVar(varname) end

--- Save a custom value in current player profile.
--- RETURNS: Void
---
---@param varname string
---@param value string
function PlayerProfile_SetVar(varname, value) end

--- Specifies a marker where an ability cannot be used. This only applies to abilities where you use the cursor to pick a location in the world (like a location to paradrop at).
--- RETURNS: Void
---
---@param player PlayerID
---@param pbgID integer
---@param marker MarkerID
function Player_AddAbilityLockoutZone(player, pbgID, marker) end

--- Add resource to player, as opposed to just setting it. Possible resource types are RT_Manpower, RT_Munition, RT_Fuel, RT_Action 
--- RETURNS: Void
---
---@param playerId PlayerID
---@param resourceType integer
---@param value number
function Player_AddResource(playerId, resourceType, value) end

--- For the given player, get all of the squads gathered into a squadgroup of your naming.  
--- RETURNS: SGroupID
---
---@param playerId PlayerID
---@param squadgroupName string
function Player_AddSquadsToSGroup(playerId, squadgroupName) end

--- Gives the player new command points to spent on
--- RETURNS: Void
---
---@param player PlayerID
---@param points integer
function Player_AddUnspentCommandPoints(player, points) end

--- Returns true if ANY of a players squads are in proximity of a marker
--- RETURNS: Boolean
---
---@param playerid PlayerID
---@param marker MarkerID
function Player_AreSquadsNearMarker(playerid, marker) end

--- Returns true if a player can see ALL or ANY items in an egroup
--- RETURNS: Boolean
---
---@param playerid PlayerID
---@param egroup EGroupID
---@param all boolean
function Player_CanSeeEGroup(playerid, egroup, all) end

--- Returns true if a player can see a given entity
--- RETURNS: Boolean
---
---@param ModPlayer* player const
---@param entity EntityID
function Player_CanSeeEntity(ModPlayer* player, entity) end

--- Returns true if a player can see a given position.
--- RETURNS: Boolean
---
---@param ModPlayer* player const
---@param pos Position
function Player_CanSeePosition(ModPlayer* player, pos) end

--- Returns true if a player can see ALL or ANY items in an sgroup
--- RETURNS: Boolean
---
---@param playerid PlayerID
---@param sgroup SGroupID
---@param all boolean
function Player_CanSeeSGroup(playerid, sgroup, all) end

--- Returns true if a player can see ALL or ANY units in a given squad
--- RETURNS: Boolean
---
---@param ModPlayer* player const
---@param squad SquadID
---@param all boolean
function Player_CanSeeSquad(ModPlayer* player, squad, all) end

--- Any of the player's units in the marker area move out of the area, and can be made invulnerable for a bit whilst they do it
--- RETURNS: Void
---
---@param player PlayerID
---@param marker MarkerID
---@param invulnerable boolean
function Player_ClearArea(player, marker, invulnerable) end

--- Clears item, command and construction menu availabilities for the player.
--- RETURNS: Void
---
---@param player PlayerID
function Player_ClearAvailabilities(player) end

--- Clears the pop cap override so that modifiers can take effect again
--- RETURNS: Void
---
---@param player PlayerID
function Player_ClearPopCapOverride(player) end

--- Do a paradrop with custom parameters for this player. This is similar to regular paradrop ability without any prereq check
--- RETURNS: Void
---
---@param player PlayerID
---@param sgroup SGroupID
---@param pos Position
---@param dropHeight number
---@param dropDrift number
---@param blueprintID integer
---@param spawnSquad boolean
---@param maxSquadEntityCount integer
---@param maxDeathOnBuilding integer
function Player_DoParadrop(player, sgroup, pos, dropHeight, dropDrift, blueprintID, spawnSquad, maxSquadEntityCount, maxDeathOnBuilding) end

--- Searches the player list in the world and returns the id of the first enemy player
--- RETURNS: PlayerID
---
---@param ModPlayer* player const
function Player_FindFirstEnemyPlayer(ModPlayer* player) end

--- Returns a player given a player id from the ME.  NOTE: this is a temp function, and will be replaced with Player_FromName
--- RETURNS: PlayerID
---
---@param id integer
function Player_FromId(id) end

--- Get a player using their name from the Mission Editor.  NOT DONE YET
--- RETURNS: PlayerID
---
---@param name string
function Player_FromName(name) end

--- Creates/Clears groups that contain all of a player's units and buildings. Defaults - sg_allsquads and eg_allentities
--- RETURNS: Void
---
---@param player[ PlayerID
---@param sgroup SGroupID
---@param egroup] EGroupID
function Player_GetAll(player[, sgroup, egroup]) end

--- Gather together all of a player's entities that are in proximity to a marker, a position, or within a territory sector into an EGroup. The EGroup is cleared beforehand. 
--- RETURNS: Void
---
---@param playerid PlayerID
---@param egroup EGroupID
---@param position[ MarkerID/Pos/SectorID
---@param range] number
function Player_GetAllEntitiesNearMarker(playerid, egroup, position[, range]) end

---
---Located in FILE: \scar\deprecated.scar LINE: 760
---@param playerid any
---@param egroupid any
---@param pos any
---@param distance any
function Player_GetAllEntitiesNearPos(playerid, egroupid, pos, distance) end

---
---Located in FILE: \scar\deprecated.scar LINE: 748
---@param playerid any
---@param egroupid any
---@param sectorid any
function Player_GetAllEntitiesWithinTerritorySector(playerid, egroupid, sectorid) end

--- Gather together all of a player's squads that are in proximity to a marker, a position, or within a territory sector into an SGroup. The SGroup is cleared beforehand. 
--- RETURNS: Void
---
---@param player PlayerID
---@param sgroup SGroupID
---@param position[ MarkerID/Pos/SectorID
---@param range] number
function Player_GetAllSquadsNearMarker(player, sgroup, position[, range]) end

---
---Located in FILE: \scar\deprecated.scar LINE: 735
---@param playerid any
---@param sgroupid any
---@param pos any
---@param distance any
function Player_GetAllSquadsNearPos(playerid, sgroupid, pos, distance) end

---
---Located in FILE: \scar\deprecated.scar LINE: 723
---@param playerid any
---@param sgroupid any
---@param sectorid any
function Player_GetAllSquadsWithinTerritorySector(playerid, sgroupid, sectorid) end

--- Returns the total number of buildings owned by this player.
--- RETURNS: Integer
---
---@param playerId PlayerID
function Player_GetBuildingsCount(playerId) end

--- Returns the total number of buildings owned by this player (with exclusions).
--- RETURNS: Integer
---
---@param playerId PlayerID
---@param exceptions table
function Player_GetBuildingsCountExcept(playerId, exceptions) end

--- Returns the number of buildings owned by this player (inclusive).
--- RETURNS: Integer
---
---@param playerId PlayerID
---@param building_names table
function Player_GetBuildingsCountOnly(playerId, building_names) end

--- Use capType CT_Personnel to get current squad cap, CT_Vehicle to get current vehicle cap, CT_Medic to get current medic cap
--- RETURNS: Integer
---
---@param ModPlayer* player const
---@param capType integer
function Player_GetCurrentPopulation(ModPlayer* player, capType) end

--- Returns the players UI name.
--- RETURNS: LocString
---
---@param ModPlayer* player const
function Player_GetDisplayName(ModPlayer* player) end

--- Returns an EntityGroupObs containing all the players entities.
--- RETURNS: EGroupID
---
---@param player PlayerID
function Player_GetEntities(player) end

--- Returns the number of entities a player currently owns
--- RETURNS: Integer
---
---@param ModPlayer* player const
function Player_GetEntityCount(ModPlayer* player) end

--- Returns the name of an entity a player currently owns
--- RETURNS: String
---
---@param ModPlayer* player const
---@param index integer
function Player_GetEntityName(ModPlayer* player, index) end

--- Returns the id of the player
--- RETURNS: Integer
---
---@param ModPlayer* player const
function Player_GetID(ModPlayer* player) end

--- Use capType CT_Personnel to get max squad cap or CT_VehicleCap to get max vehicle cap.
--- RETURNS: Integer
---
---@param ModPlayer* player const
---@param capType integer
function Player_GetMaxPopulation(ModPlayer* player, capType) end

--- Returns the number of strategic points (not objectives) this player owns
--- RETURNS: Integer
---
---@param ModPlayer* p const
function Player_GetNumStrategicPoints(ModPlayer* p) end

--- Returns the number of strategic objectives this player owns
--- RETURNS: Integer
---
---@param ModPlayer* p const
function Player_GetNumVictoryPoints(ModPlayer* p) end

--- Returns the phase the given player is currently at
--- RETURNS: Integer
---
---@param player PlayerID
function Player_GetPhase(player) end

--- Returns the race index for the given player.
--- RETURNS: Integer
---
---@param ModPlayer* player const
function Player_GetRace(ModPlayer* player) end

--- Returns the ME name of the race for a given player.
--- RETURNS: String
---
---@param ModPlayer* player const
function Player_GetRaceName(ModPlayer* player) end

--- Returns the race name of this player in non-localized english (eg. allies, axis)
--- RETURNS: String
---
---@param ModPlayer* player const
function Player_GetRaceString(ModPlayer* player) end

--- Returns the relationship between 2 players.  Possible relationships are R_ENEMY, R_ALLY, R_NEUTRAL, R_UNDEFINED.
--- RETURNS: Integer
---
---@param ModPlayer* player1 const
---@param player2 PlayerID
function Player_GetRelationship(ModPlayer* player1, player2) end

--- Returns the amount of resources a given player has.  Possible resource types are RT_Manpower, RT_Fuel, RT_Munition, RT_Action, RT_Command
--- RETURNS: Real
---
---@param ModPlayer* player const
---@param resourceType integer
function Player_GetResource(ModPlayer* player, resourceType) end

--- Returns the amount of resources a given player is getting per second.  Possible resource types are RT_Manpower, RT_Fuel, RT_Munition, RT_Action, RT_Command
--- RETURNS: Real
---
---@param ModPlayer* player const
---@param resourceType integer
function Player_GetResourceRate(ModPlayer* player, resourceType) end

--- Returns the number of squads a player currently owns
--- RETURNS: Integer
---
---@param ModPlayer* player const
function Player_GetSquadCount(ModPlayer* player) end

--- Returns a SquadGroupObs containing all the players units.
--- RETURNS: SGroupID
---
---@param ModPlayer* player const
function Player_GetSquads(ModPlayer* player) end

--- Returns the starting position for this player
--- RETURNS: Position
---
---@param ModPlayer* player const
function Player_GetStartingPosition(ModPlayer* player) end

--- Get the team a player is on
--- RETURNS: Integer
---
---@param ModPlayer* p const
function Player_GetTeam(ModPlayer* p) end

--- Returns the current number of units the player has.
--- RETURNS: Integer
---
---@param ModPlayer* player const
function Player_GetUnitCount(ModPlayer* player) end

--- Returns the cost of an upgrade. Possible resource types are RT_Manpower, RT_Fuel, RT_Munition, RT_Action, RT_Command.
--- RETURNS: Real
---
---@param player PlayerID
---@param upgradeID integer
---@param resourceType integer
function Player_GetUpgradeCost(player, upgradeID, resourceType) end

--- Returns true if this player owns any buildings listed in the table.
--- RETURNS: boolean
---
---@param player PlayerID
---@param entitytypes Blueprint/ID/Table
function Player_HasBuilding(player, entitytypes) end

--- Returns true if this player owns any buildings listed in the table currently under construction.
--- RETURNS: boolean
---
---@param player PlayerID
---@param entitytypes Blueprint/ID/Table
function Player_HasBuildingUnderConstruction(player, entitytypes) end

--- Returns true if this player owns any buildings. (with exclusions).
--- RETURNS: boolean
---
---@param playerId PlayerID
---@param exceptions-entitytypes Blueprint/ID/Table
function Player_HasBuildingsExcept(playerId, exceptions-entitytypes) end

--- Checks to see if a player has lost (if player owns any squads and any buildings contained in blueprints)
--- RETURNS: Boolean
---
---@param player PlayerID
---@param Blueprints LuaTable
function Player_HasLost(player, Blueprints) end

--- Return true if the squad has purchased the specified upgrade.
--- RETURNS: Boolean
---
---@param ModPlayer* pPlayer const
---@param pbgID integer
function Player_HasUpgrade(ModPlayer* pPlayer, pbgID) end

--- Returns true if player is still alive and false if player is dead.  Will error if playerIdx is an invalid index.
--- RETURNS: Boolean
---
---@param ModPlayer* player const
function Player_IsAlive(ModPlayer* player) end

--- Returns true if the players are allied and false if they are not.
--- RETURNS: Boolean
---
---@param playerId1 PlayerID
---@param playerId2 PlayerID
function Player_IsAllied(playerId1, playerId2) end

--- Kill a player.  Will error if playerIdx is an invalid index.
--- RETURNS: Void
---
---@param player PlayerID
function Player_Kill(player) end

---
---Located in FILE: \scar\scarutil.scar LINE: 1505
---@param player any
function Player_KillEnemyOwnedUnits(player) end

--- Tries to promote each squad in an sgroup, given that the player has veteran squads available for the appropriate squad types. See Player_MakeSquadVeteran for more details.
--- RETURNS: Integer (amount of squads promoted)
---
---@param player PlayerID
---@param sgroup SGroupID
function Player_MakeSGroupVeteran(player, sgroup) end

--- If the player has a veteran squad of the same type in reserve, it promotes an existing squad to match the veteran squad (rank and slot items). The veteran squad is "used up" and is no longer available. Does not promote squad if there would be no benefit in doing so.
--- RETURNS: Boolean
---
---@param player PlayerID
---@param squad SquadID
function Player_MakeSquadVeteran(player, squad) end

--- Returns the number of upgrades that this player has. The upgradeName should be a relative filename (eg. upgrade/allies/research/phase_2.lua)
--- RETURNS: Integer
---
---@param ModPlayer* player const
---@param char *upgradeName const
function Player_NumUpgradeComplete(ModPlayer* player, char *upgradeName) end

--- Returns true if a given player exculsivley owns all items in a group
--- RETURNS: Boolean
---
---@param playerid PlayerID
---@param egroup EGroupID
function Player_OwnsEGroup(playerid, egroup) end

--- Returns true if a given player exculsivley owns all items in a group
--- RETURNS: Boolean
---
---@param playerid PlayerID
---@param sgroup SGroupID
function Player_OwnsSGroup(playerid, sgroup) end

--- Removes a marker that was previously a lockout zone.
--- RETURNS: Void
---
---@param player PlayerID
---@param pbgID integer
---@param marker MarkerID
function Player_RemoveAbilityLockoutZone(player, pbgID, marker) end

--- Restrict a list of addons.
--- RETURNS: Void
---
---@param playerid PlayerID
---@param addonlist table
function Player_RestrictAddOnList(playerid, addonlist) end

--- Restrict a list of buildings.
--- RETURNS: Void
---
---@param playerid PlayerID
---@param blueprintlist table
function Player_RestrictBuildingList(playerid, blueprintlist) end

--- Restrict a list of research items.
--- RETURNS: Void
---
---@param playerid PlayerID
---@param list StringTable
function Player_RestrictResearchList(playerid, list) end

--- Removes chosen CC tree and all upgrades associated with it.
--- RETURNS: Void
---
---@param player PlayerID
function Player_RevertChosenCommanderTree(player) end

--- Sets the availability of an ability. Availability can be either ITEM_LOCKED, ITEM_UNLOCKED, ITEM_REMOVED or ITEM_DEFAULT
--- RETURNS: Void
---
---@param pPlayer PlayerID
---@param pbgID integer
---@param availability integer
function Player_SetAbilityAvailability(pPlayer, pbgID, availability) end

--- Sets availability of ALL entity, squad and player commands. Availability can be either ITEM_LOCKED, ITEM_UNLOCKED, ITEM_REMOVED or ITEM_DEFAULT
--- RETURNS: Void
---
---@param player PlayerID
---@param availability integer
function Player_SetAllCommandAvailability(player, availability) end

---
---Located in FILE: \scar\deprecated.scar LINE: 803
---@param player any
---@param item any
---@param availability any
function Player_SetAvailability(player, item, availability) end

--- Sets the availability of entity, squad and player commands. See Command_Entity, Command_Squad, and Command_Player for valid command types. Availability can be either ITEM_LOCKED, ITEM_UNLOCKED, ITEM_REMOVED or ITEM_DEFAULT
--- RETURNS: Void
---
---@param player PlayerID
---@param cmdType integer
---@param availability integer
function Player_SetCommandAvailability(player, cmdType, availability) end

--- Sets the availability of construction menus by name. In AE, they're under type_construction. Availability can be either ITEM_LOCKED, ITEM_UNLOCKED, ITEM_REMOVED or ITEM_DEFAULT
--- RETURNS: Void
---
---@param player PlayerID
---@param menuName string
---@param availability integer
function Player_SetConstructionMenuAvailability(player, menuName, availability) end

--- Set default squad mood mode which can be overrided by squad level mood mode settings
--- RETURNS: Void
---
---@param pPlayer PlayerID
---@param mode integer
function Player_SetDefaultSquadMoodMode(pPlayer, mode) end

--- Sets the availability of a entity/building production item. Availability can be either ITEM_LOCKED, ITEM_UNLOCKED, ITEM_REMOVED or ITEM_DEFAULT
--- RETURNS: Void
---
---@param pPlayer PlayerID
---@param pbgID integer
---@param availability integer
function Player_SetEntityProductionAvailability(pPlayer, pbgID, availability) end

--- Sets the current personnel or vehicle max-cap for a player. The captype is either CT_Personnel or CT_Vehicle (you can't adjust Medic caps just yet).
--- RETURNS: Void
---
---@param playerid PlayerID
---@param captype integer
---@param newcap integer
function Player_SetMaxCapPopulation(playerid, captype, newcap) end

--- Sets the current personnel or vehicle cap for a player. The captype is either CT_Personnel or CT_Vehicle (you can't adjust Medic caps just yet).
--- RETURNS: Void
---
---@param playerid PlayerID
---@param captype integer
---@param newcap integer
function Player_SetMaxPopulation(playerid, captype, newcap) end

--- Sets a pop cap override that ignores any modifiers. You can pass in -1 for a cap value to use the default [modified] value
--- RETURNS: Void
---
---@param player PlayerID
---@param personnel integer
---@param vehicles integer
---@param medics integer
function Player_SetPopCapOverride(player, personnel, vehicles, medics) end

--- Set the resource amount for a given player.  Ignores income cap and resource sharing.
--- RETURNS: Void
---
---@param player PlayerID
---@param resourceType integer
---@param amt number
function Player_SetResource(player, resourceType, amt) end

--- Sets the availability of a squad production item. Availability can be either ITEM_LOCKED, ITEM_UNLOCKED, ITEM_REMOVED or ITEM_DEFAULT
--- RETURNS: Void
---
---@param pPlayer PlayerID
---@param pbgID integer
---@param availability integer
function Player_SetSquadProductionAvailability(pPlayer, pbgID, availability) end

--- Sets the availability of an upgrade. Availability can be either ITEM_LOCKED, ITEM_UNLOCKED, ITEM_REMOVED or ITEM_DEFAULT
--- RETURNS: Void
---
---@param pPlayer PlayerID
---@param pbgID integer
---@param availability integer
function Player_SetUpgradeAvailability(pPlayer, pbgID, availability) end

--- Sets the cost of an upgrade.
--- RETURNS: Void
---
---@param player PlayerID
---@param upgrade UpgradeID
---@param manpower number
---@param fuel number
---@param munition number
---@param action number
---@param command number
function Player_SetUpgradeCost(player, upgrade, manpower, fuel, munition, action, command) end

--- Prevents a player from earning any action points (and by extention, command points)
--- RETURNS: Void
---
---@param player PlayerID
function Player_StopEarningActionPoints(player) end

--- returns vector between two positions
---
---Located in FILE: \scar\screensaver.scar LINE: 187
---@param startPos any
---@param endPos any
function Pos2PosVector(startPos, endPos) end

--- Prints a message on the screen
--- RETURNS: Void
---
---@param text string
function PrintOnScreen(text) end

--- Remove any messages from the screen
--- RETURNS: Void
---
function PrintOnScreen_RemoveFromScreen() end

--- Adds a add-on item to a production list table.
--- RETURNS: Void
---
---@param addonname string
---@param list LuaTable
function Prod_AddAddOnToList(addonname, list) end

--- Adds a research item to a production list table.
--- RETURNS: Void
---
---@param researchname string
---@param list LuaTable
function Prod_AddResearchToList(researchname, list) end

--- Adds a squad item to a production list table.
--- RETURNS: Void
---
---@param blueprintname string
---@param list LuaTable
function Prod_AddSquadToList(blueprintname, list) end

--- Returns true if a production list contains the given add-on item
--- RETURNS: Boolean
---
---@param addonname string
---@param list LuaTable
function Prod_DoesListContainAddOn(addonname, list) end

--- Returns true if a production list contains the given research item
--- RETURNS: Boolean
---
---@param researchname string
---@param list LuaTable
function Prod_DoesListContainResearch(researchname, list) end

--- Returns true if a production list contains the given squad item
--- RETURNS: Boolean
---
---@param blueprintname string
---@param list LuaTable
function Prod_DoesListContainSquad(blueprintname, list) end

--- Gets everything a given player currently has in production.
--- RETURNS: Void
---
---@param playerId PlayerID
---@param list LuaTable
function Prod_GetPlayerProductionList(playerId, list) end

--- Checks all entities to see if they're production-capable and if so, adds their queued items to a Production Table.
--- RETURNS: Void
---
---@param egroupname string
---@param list LuaTable
function Prod_GetProductionList(egroupname, list) end

--- Returns true the players henchmen are currently constructing a building with given blueprintname.
--- RETURNS: Boolean
---
---@param playerId PlaierID
---@param buildingname string
function Prod_IsPlayerConstructing(playerId, buildingname) end

--- Returns true if the player is producing all or any items in a production table.  See Prod_IsProducing for more info.
--- RETURNS: Boolean
---
---@param playerId PlayerID
---@param list LuaTable
---@param all boolean
function Prod_IsPlayerProducing(playerId, list, all) end

--- Returns true if buildings in a entity group are producing ALL or ANY items in a given production table.
--- RETURNS: Boolean
---
---@param egroupname string
---@param list LuaTable
---@param all boolean
function Prod_IsProducing(egroupname, list, all) end

---
---Located in FILE: \scar\deprecated.scar LINE: 38
---@param groupid any
---@param markerid any
function Prox_AllEntitiesNearMarker(groupid, markerid) end

---
---Located in FILE: \scar\deprecated.scar LINE: 16
---@param groupid any
---@param markerid any
function Prox_AllSquadsNearMarker(groupid, markerid) end

---
---Located in FILE: \scar\deprecated.scar LINE: 49
---@param groupid any
---@param markerid any
function Prox_AnyEntityNearMarker(groupid, markerid) end

---
---Located in FILE: \scar\deprecated.scar LINE: 27
---@param groupid any
---@param markerid any
function Prox_AnySquadNearMarker(groupid, markerid) end

--- Returns true if ANY or ALL entities from a group are in range of a given position, marker, or territory sector.
--- RETURNS: Boolean
---
---@param egroup EGroupID
---@param position MarkerID/Position/SectorID
---@param all[ boolean
---@param range] number
function Prox_AreEntitiesNearMarker(egroup, position, all[, range]) end

--- Returns true if ANY or ALL of a player's members (i.e. individual guys, not squads as a whole) are in range of a given position, marker, or territory sector. DO NOT USE THIS FUNCTION UNLESS YOU ABSOLUTELY HAVE TO!!
--- RETURNS: Boolean
---
---@param player PlayerID
---@param position MarkerID/Position/SectorID
---@param all[ boolean
---@param range] number
function Prox_ArePlayerMembersNearMarker(player, position, all[, range]) end

--- Returns true if ANY or ALL of a player's squads are in range of a given position, marker, or territory sector. THIS FUNCTION IS VERY SLOW. DO NOT USE THIS UNLESS ABSOLUTELY NECESSARY.
--- RETURNS: Boolean
---
---@param player PlayerID
---@param position MarkerID/Position/SectorID
---@param all[ boolean
---@param range] number
function Prox_ArePlayersNearMarker(player, position, all[, range]) end

--- Returns true if ANY or ALL squad members (i.e. individual guys, not squads as a whole) from a group are in range of a given position, marker, or territory sector. DO NOT USE THIS FUNCTION UNLESS YOU ABSOLUTELY HAVE TO!!
--- RETURNS: Boolean
---
---@param sgroup SGroupID
---@param position MarkerID/Position/SectorID
---@param all[ boolean
---@param range] number
function Prox_AreSquadMembersNearMarker(sgroup, position, all[, range]) end

--- Returns true if ANY or ALL squads from a group are in range of a given position, marker, or territory sector
--- RETURNS: Boolean
---
---@param sgroup SGroupID
---@param position MarkerID/Position/SectorID
---@param all[ boolean
---@param range] number
function Prox_AreSquadsNearMarker(sgroup, position, all[, range]) end

--- Returns the distance between two entity groups. use checktype PROX_SHORTEST, PROX_LONGEST, or PROX_CENTER.
--- RETURNS: Real
---
---@param egroup1 EGroupID
---@param egroup2 EGroupID
---@param checktype ProxType
function Prox_EGroupEGroup(egroup1, egroup2, checktype) end

--- Returns the distance between an entity group and a squad group.  use checktype PROX_SHORTEST, PROX_LONGEST, or PROX_CENTER.
--- RETURNS: Real
---
---@param egroup1 EGroupID
---@param sgroup2 SGroupID
---@param checktype ProxType
function Prox_EGroupSGroup(egroup1, sgroup2, checktype) end

--- Checks if ALL or ANY entities are in proximity of a given entity group.
--- RETURNS: Boolean
---
---@param egroup1 EGroupID
---@param egroup2 EGroupID
---@param proximity number
---@param all boolean
function Prox_EntitiesInProximityOfEntities(egroup1, egroup2, proximity, all) end

---
---Located in FILE: \scar\deprecated.scar LINE: 457
---@param playerid any
---@param pos any
---@param all any
---@param range any
function Prox_IsPlayerNearMarker(playerid, pos, all, range) end

--- Returns the distance between a marker and an entity group.  use checktype PROX_SHORTEST, PROX_LONGEST, or PROX_CENTER.
--- RETURNS: Real
---
---@param marker MarkerID
---@param egroup EGroupID
---@param checktype ProxType
function Prox_MarkerEGroup(marker, egroup, checktype) end

--- Returns the distance between a marker and a squad group.  use checktype PROX_SHORTEST, PROX_LONGEST, or PROX_CENTER.
--- RETURNS: Real
---
---@param marker MarkerID
---@param sgroup SGroupID
---@param checktype ProxType
function Prox_MarkerSGroup(marker, sgroup, checktype) end

--- Checks if ALL or ANY players squads are in proximity of a given entity group.
--- RETURNS: Boolean
---
---@param playerid PlayerID
---@param egroup EGroupID
---@param proximity number
---@param all boolean
---@param exclude EntityID
function Prox_PlayerEntitiesInProximityOfEntities(playerid, egroup, proximity, all, exclude) end

--- Checks if ALL or ANY players entities are in proximity of a given squad group.
--- RETURNS: Boolean
---
---@param playerentities PlayerID
---@param playersquads PlayerID
---@param proximity number
---@param all boolean
function Prox_PlayerEntitiesInProximityOfPlayerSquads(playerentities, playersquads, proximity, all) end

--- Checks if ALL or ANY players entities are in proximity of a given squad group.
--- RETURNS: Boolean
---
---@param playerid PlayerID
---@param sgroup SGroupID
---@param proximity number
---@param all boolean
function Prox_PlayerEntitiesInProximityOfSquads(playerid, sgroup, proximity, all) end

--- Checks if ALL or ANY players squads are in proximity of a given entity group.
--- RETURNS: Boolean
---
---@param playerid PlayerID
---@param egroup EGroupID
---@param proximity number
---@param all boolean
function Prox_PlayerSquadsInProximityOfEntities(playerid, egroup, proximity, all) end

--- Checks if ALL or ANY players squads are in proximity of a given players entities.
--- RETURNS: Boolean
---
---@param playersquads PlayerID
---@param playerentities PlayerID
---@param proximity number
---@param all boolean
function Prox_PlayerSquadsInProximityOfPlayerEntities(playersquads, playerentities, proximity, all) end

--- Checks if ALL or ANY players squads are in proximity of a given players squads.
--- RETURNS: Boolean
---
---@param playerid1 PlayerID
---@param playerid2 PlayerID
---@param proximity number
---@param all boolean
function Prox_PlayerSquadsInProximityOfPlayerSquads(playerid1, playerid2, proximity, all) end

--- Checks if ALL or ANY players squads are in proximity of a given squad group.
--- RETURNS: Boolean
---
---@param playerid PlayerID
---@param sgroup SGroupID
---@param proximity number
---@param all boolean
---@param exclude SquadID
function Prox_PlayerSquadsInProximityOfSquads(playerid, sgroup, proximity, all, exclude) end

--- Returns the distance between two squad groups. use checktype PROX_SHORTEST, PROX_LONGEST, or PROX_CENTER.
--- RETURNS: Real
---
---@param sgroup1 SGroupID
---@param sgroup2 SGroupID
---@param checktype ProxType
function Prox_SGroupSGroup(sgroup1, sgroup2, checktype) end

--- Checks if ALL or ANY squads are in proximity of a given entity group.
--- RETURNS: Boolean
---
---@param sgroup SGroupID
---@param egroup EGroupID
---@param proximity number
---@param all boolean
function Prox_SquadsInProximityOfEntities(sgroup, egroup, proximity, all) end

--- Checks if ALL or ANY squads are in proximity of a given squad group.
--- RETURNS: Boolean
---
---@param sgroup1 SGroupID
---@param sgroup2 SGroupID
---@param proximity number
---@param all boolean
function Prox_SquadsInProximityOfSquads(sgroup1, sgroup2, proximity, all) end

---
---Located in FILE: \scar\objectives.scar LINE: 48
function RegisterObjectiveUpdate() end

--- Add each element of the two passed in ResourceAmounts together
--- RETURNS: ResourceAmount
---
---@param amt1 ResourceAmount
---@param amt2 ResourceAmount
function ResourceAmount_Add(amt1, amt2) end

--- Clamps the passed in resource to zero if it has any negative numbers
--- RETURNS: ResourceAmount
---
---@param amt ResourceAmount
function ResourceAmount_ClampToZero(amt) end

--- Returns true if the first amount has enough resources for the second amount (amt2)
--- RETURNS: Boolean
---
---@param amt1 ResourceAmount
---@param amt2 ResourceAmount
function ResourceAmount_Has(amt1, amt2) end

--- Take a resource amount and multiply each value inside of it by mult
--- RETURNS: ResourceAmount
---
---@param amt1 ResourceAmount
---@param mult number
function ResourceAmount_Mult(amt1, mult) end

--- Subtract the second amount from the first and return the new ResourceAmount
--- RETURNS: ResourceAmount
---
---@param amt1 ResourceAmount
---@param amt2 ResourceAmount
function ResourceAmount_Subtract(amt1, amt2) end

--- Add up all the numbers in the resource and return a single value
--- RETURNS: Real
---
---@param amt1 ResourceAmount
function ResourceAmount_Sum(amt1) end

--- Creates a new ResourceAmount all set to zero
--- RETURNS: ResourceAmount
---
---@param Void any
function ResourceAmount_Zero(Void) end

--- Disables any resource income - useful to stop resources accruing during the opening movie
--- RETURNS: Void
---
function Resources_Disable() end

--- Re-enables resource income. 
--- RETURNS: Void
---
function Resources_Enable() end

--- Applies restrictions for allies.  You should call this at the begining of your mission for Player 1.
--- RETURNS: Void
---
---@param playerId PlayerID
---@param level integer
function Restrict_Allies(playerId, level) end

--- Add a rule to be executed every frame. Priority can be from 0 to 1000, with 0 being the lowest. Priority is used in conjunction with Rule_RemoveAll so that rules with high priority do not get removed.
--- RETURNS: Void
---
---@param rule[ function
---@param priority] integer
function Rule_Add(rule[, priority]) end

--- Add a rule to be executed when the event of 'eventType' has happened on entities in the 'egroup'
--- RETURNS: Void
---
---@param rule function
---@param egroup EGroupID
---@param eventtype integer
function Rule_AddEGroupEvent(rule, egroup, eventtype) end

--- Add a rule to be executed when the event of 'eventType' has happened on the 'entity'
--- RETURNS: Void
---
---@param rule function
---@param entity EntityID
---@param eventtype integer
function Rule_AddEntityEvent(rule, entity, eventtype) end

--- Add a rule to be executed at every 'interval' seconds. Priority can be from 0 to 1000, with 0 being the lowest. Priority is used in conjunction with Rule_RemoveAll so that rules with high priority do not get removed.
--- RETURNS: Void
---
---@param rule function
---@param interval[ number
---@param priority] integer
function Rule_AddInterval(rule, interval[, priority]) end

--- Add a rule to be executed 'calls' times, at every 'interval' seconds. Priority can be from 0 to 1000, with 0 being the lowest. Priority is used in conjunction with Rule_RemoveAll so that rules with high priority do not get removed.
--- RETURNS: Void
---
---@param rule function
---@param interval number
---@param calls[ integer
---@param priority] integer
function Rule_AddIntervalEx(rule, interval, calls[, priority]) end

--- Add a rule to be executed once, after 'interval' seconds. Priority can be from 0 to 1000, with 0 being the lowest. Priority is used in conjunction with Rule_RemoveAll so that rules with high priority do not get removed.
--- RETURNS: Void
---
---@param rule function
---@param interval[ number
---@param priority] integer
function Rule_AddOneShot(rule, interval[, priority]) end

--- Add a rule to be executed when the event of 'eventType' has happened on the 'player'
--- RETURNS: Void
---
---@param rule function
---@param player PlayerID
---@param eventtype integer
function Rule_AddPlayerEvent(rule, player, eventtype) end

--- Add a rule to be executed when the event of 'eventType' has happened on squads in the 'sgroup'
--- RETURNS: Void
---
---@param rule function
---@param sgroup SGroupID
---@param eventtype integer
function Rule_AddSGroupEvent(rule, sgroup, eventtype) end

--- Add a rule to be executed when the event of 'eventType' has happened on the 'squad'
--- RETURNS: Void
---
---@param rule function
---@param squad SquadID
---@param eventtype integer
function Rule_AddSquadEvent(rule, squad, eventtype) end

--- Caps the player's action points at a certain amount so they can't earn more than x command points
---
---Located in FILE: \scar\techtreesetup.scar LINE: 350
function Rule_CapCommandPoints() end

--- Change 'interval' seconds of an existing rule
--- RETURNS: Void
---
---@param rule function
---@param interval number
function Rule_ChangeInterval(rule, interval) end

---
---Located in FILE: \scar\underattack.scar LINE: 25
function Rule_DetectUnderAttack() end

--- Disables the Company Commander menu.  Note that this must be done 1 second after the OnInit.
---
---Located in FILE: \scar\techtreesetup.scar LINE: 344
function Rule_DisableCCMenu() end

--- Test if a rule is currently active
--- RETURNS: Boolean
---
---@param rule function
function Rule_Exists(rule) end

---
---Located in FILE: \scar\fallback.scar LINE: 115
function Rule_FallBackManager() end

--- The manager function
---
---Located in FILE: \scar\deprecated.scar LINE: 592
function Rule_PathManager() end

--- Remove a currently active rule (this does not remove any event rules)
--- RETURNS: Void
---
---@param rule function
function Rule_Remove(rule) end

--- Kills all rules below a certain priority. The default is to remove ALL rules.
--- RETURNS: Void
---
---@param max_priority? integer
function Rule_RemoveAll(max_priority) end

--- Remove an active event rule for entities in the 'egroup'
--- RETURNS: Void
---
---@param rule function
---@param egroup EGroupID
function Rule_RemoveEGroupEvent(rule, egroup) end

--- Remove an active event rule for the 'entity'
--- RETURNS: Void
---
---@param rule function
---@param entity EntityID
function Rule_RemoveEntityEvent(rule, entity) end

--- Remove a currently active rule if it exists(this does not remove any event rules)
--- RETURNS: Void
---
---@param rule function
function Rule_RemoveIfExist(rule) end

--- Remove a currently executing rule (only works inside a rule function)
--- RETURNS: Void
---
function Rule_RemoveMe() end

--- Remove an active event rule for the 'player'
--- RETURNS: Void
---
---@param rule function
---@param player PlayerID
function Rule_RemovePlayerEvent(rule, player) end

--- Remove an active event rule for squads in the 'sgroup'
--- RETURNS: Void
---
---@param rule function
---@param sgroup SGroupID
function Rule_RemoveSGroupEvent(rule, sgroup) end

--- Remove an active event rule for the 'squad'
--- RETURNS: Void
---
---@param rule function
---@param squad SquadID
function Rule_RemoveSquadEvent(rule, squad) end

---
---Located in FILE: \scar\command.scar LINE: 175
function Rule_SquadMarkerProxDeleter() end

--- Returns true if a squad blueprint exists with the given name.
--- RETURNS: Boolean
---
---@param name string
function SBP_Exists(name) end

--- Adds an squadron to the end of a group if the group doesnt already have it.
--- RETURNS: Void
---
---@param group SGroupID
---@param squadron SquadID
function SGroup_Add(group, squadron) end

--- Same as EGroup_AddGroup.  Note: You cannot mix squad groups and entity groups.
--- RETURNS: Void
---
---@param group SGroupID
---@param grouptoadd SGroupID
function SGroup_AddGroup(group, grouptoadd) end

--- Adds a leader to all squads in a group that can take a leader.
--- RETURNS: Void
---
---@param sgroup SGroupID
function SGroup_AddLeaders(sgroup) end

--- Add to the list of slot items to drop when any one of the squads is wiped out
--- RETURNS: Void
---
---@param groupid SGroupID
---@param itemid integer
---@param drop_chance number
---@param exclusive boolean
function SGroup_AddSlotItemToDropOnDeath(groupid, itemid, drop_chance, exclusive) end

--- Return true if ANY or ALL of a group can reinforce now
--- RETURNS: Boolean
---
---@param group SGroupID
---@param all boolean
function SGroup_CanInstantReinforceNow(group, all) end

--- Returns true if ALL or ANY squads in a group can see ALL or ANY squads in a target sgroup.
--- RETURNS: Boolean
---
---@param sgroup SGroupID
---@param targetsgroup SGroupID
---@param all boolean
function SGroup_CanSeeSGroup(sgroup, targetsgroup, all) end

--- Removes all entities from a group.
--- RETURNS: Void
---
---@param sgroup SGroupID
function SGroup_Clear(sgroup) end

--- Clears any previous posture suggestions made to a squad
--- RETURNS: Void
---
---@param sgroup SGroupID
function SGroup_ClearPostureSuggestion(sgroup) end

--- Returns true if the contents of the two groups are equal. Order of the entities does not matter.
--- RETURNS: Boolean
---
---@param group1 SGroupID
---@param group2 SGroupID
function SGroup_Compare(group1, group2) end

--- Check if a group contains ALL or ANY of the items in a blueprint table.
--- RETURNS: Boolean
---
---@param sgroup SGroupID
---@param blueprints BlueprintTable
---@param all boolean
function SGroup_ContainsBlueprints(sgroup, blueprints, all) end

--- Returns true if SGroup1 contains ANY or ALL of SGroup2
--- RETURNS: Boolean
---
---@param group1 SGroupID
---@param group2 SGroupID
---@param all boolean
function SGroup_ContainsSGroup(group1, group2, all) end

--- Returns true if SGroup contains a particular SquadID
--- RETURNS: Boolean
---
---@param group SGroupID
---@param SquadID integer
function SGroup_ContainsSquad(group, SquadID) end

--- Returns the total number of spawned and despawned squads in a group.
--- RETURNS: Integer
---
---@param sgroup SGroupID
function SGroup_Count(sgroup) end

--- Returns the number of despawned squads in a group.
--- RETURNS: Integer
---
---@param sgroup SGroupID
function SGroup_CountDeSpawned(sgroup) end

--- Returns the number of spawned squads in a group.
--- RETURNS: Integer
---
---@param sgroup SGroupID
function SGroup_CountSpawned(sgroup) end

--- Returns a new squadron group with the given name.  
--- RETURNS: SGroupID
---
---@param name string
function SGroup_Create(name) end

--- Find a squad group from name.  Creates a new one with given name if it doesnt exist.
--- RETURNS: SGroupID
---
---@param name string
function SGroup_CreateIfNotFound(name) end

--- Create and display kicker message on the each squad in the sgroup to the player
--- RETURNS: Void
---
---@param group SGroupID
---@param player PlayerID
---@param textid LocString
function SGroup_CreateKickerMessage(group, player, textid) end

--- Returns a table of sgroups NOT in the world builder
--- RETURNS: LuaTable
---
---@param format string
---@param size integer
function SGroup_CreateTable(format, size) end

--- Despawn all spawned squads in a group.
--- RETURNS: Void
---
---@param groupid SGroupID
function SGroup_DeSpawn(groupid) end

--- Manually destroy a group that you dont need anymore.
--- RETURNS: Void
---
---@param egroup SGroupID
function SGroup_Destroy(egroup) end

--- Destroys all items in a group that are in proximity to a given marker.
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param marker MarkerID
function SGroup_DestroyAllInMarker(sgroup, marker) end

--- Destroys all spawned and despawned squads in a group.
--- RETURNS: Void
---
---@param sgroup SGroupID
function SGroup_DestroyAllSquads(sgroup) end

--- Disables all current combat plans for the squads in the sgroup
--- RETURNS: Void
---
---@param groupID SGroupID
function SGroup_DisableCombatPlans(groupID) end

--- Duplicates an SGroup
--- RETURNS: Void
---
---@param sgroup1 SGroupID
---@param sgroup2 SGroupID
function SGroup_Duplicate(sgroup1, sgroup2) end

--- Tells an SGroup to eject themselves from whatever they're inside (building or vehicle)
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param destination Position
function SGroup_Eject(sgroup, destination) end

--- Enable or disable minimap indicator on all squads in the sgroup
--- RETURNS: Void
---
---@param group SGroupID
---@param enable boolean
function SGroup_EnableMinimapIndicator(group, enable) end

--- Enables or disables the surprise feature for an sgroup
--- RETURNS: Void
---
---@param groupid SGroupID
---@param enable boolean
function SGroup_EnableSurprise(groupid, enable) end

--- Enable or disable decorators on all squads in the sgroup 
--- RETURNS: Void
---
---@param group SGroupID
---@param enable boolean
function SGroup_EnableUIDecorator(group, enable) end

--- Returns true if the squad group with the given name exists
--- RETURNS: Boolean
---
---@param name string
function SGroup_Exists(name) end

--- Makes two SGroups face each other
--- RETURNS: Void
---
---@param sgroup1 SGroupID
---@param sgroup2 SGroupID
function SGroup_FaceEachOther(sgroup1, sgroup2) end

--- Makes a SGroup face a marker.
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param marker MarkerID
function SGroup_FaceMarker(sgroup, marker) end

--- Works like Squad_FacePosition.  All Squads will face the same direction, with the squad the closest to the center determining the direction.
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param pos Position
function SGroup_FacePosition(sgroup, pos) end

--- Filters an SGroup by blueprint.
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param blueprint String/ID/Table
---@param filtertype integer
function SGroup_Filter(sgroup, blueprint, filtertype) end

--- Pass in a group and it will filter it down to the indicated number
--- RETURNS: Void
---
---@param sgroup1 SGroupID
---@param groupSize integer
function SGroup_FilterCount(sgroup1, groupSize) end

--- Call a lua function for each item in a group. Function will recieve (groupid, itemindex, itemid) and should return true to break or false to continue.
--- RETURNS: Boolean
---
---@param sgroup SGroupID
---@param f LuaBinding::StackVarFunction
function SGroup_ForEach(sgroup, f) end

--- Call a lua function for each item in a group. Function will recieve (groupid, itemindex, itemid) and should return a bool.
--- RETURNS: Boolean
---
---@param sgroup SGroupID
---@param all boolean
---@param f LuaBinding::StackVarFunction
function SGroup_ForEachAllOrAny(sgroup, all, f) end

--- Same as SGroup_ForEachAllOrAny except you have a choice to iterate over spawned squads, despawned squads, or both.
--- RETURNS: Boolean
---
---@param sgroup SGroupID
---@param all boolean
---@param f LuaBinding::StackVarFunction
---@param spawned boolean
---@param despawned boolean
function SGroup_ForEachAllOrAnyEx(sgroup, all, f, spawned, despawned) end

--- Same as SGroup_ForEach except you have a choice to iterate over spawned squads, despawned squads, or both.
--- RETURNS: Boolean
---
---@param sgroup SGroupID
---@param f LuaBinding::StackVarFunction
---@param spawned boolean
---@param despawned boolean
function SGroup_ForEachEx(sgroup, f, spawned, despawned) end

--- Find an squadron group with a given name.
--- RETURNS: SGroupID
---
---@param name string
function SGroup_FromName(name) end

--- Returns the average health of all units in a squad group as a weighted percent [0.0, 1.0].
--- RETURNS: Real
---
---@param sgroup SGroupID
function SGroup_GetAvgHealth(sgroup) end

--- Returns the average loadout of all squads in a group as a percent [0.0, 1.0].
--- RETURNS: Real
---
---@param sgroup SGroup
function SGroup_GetAvgLoadout(sgroup) end

--- Returns the despawned squad at a certain position in the group.
--- RETURNS: SquadID
---
---@param group SGroupID
---@param index integer
function SGroup_GetDeSpawnedSquadAt(group, index) end

--- Get the entity id of the building that any squad of the sgroup is garrisoned in
--- RETURNS: EntityID
---
---@param groupid SGroupID
function SGroup_GetGarrisonedBuildingEntity(groupid) end

--- Check invulnerablity state for ALL or ANY squads in a squad group.
--- RETURNS: Boolean
---
---@param sgroup SGroupID
---@param all boolean
function SGroup_GetInvulnerable(sgroup, all) end

--- Gets the last attacker(s) for all the squads in an SGroup Gets the last attacker for all the squads in an SGroup and stores that in 
--- RETURNS: Void
---
---@param SGroupVictim SGroup
---@param SGroupAttacker SGroup
function SGroup_GetLastAttacker(SGroupVictim, SGroupAttacker) end

--- Get the squad id of the vehicle squad that any squad of the sgroup is loaded in
--- RETURNS: SquadID
---
---@param groupid SGroupID
function SGroup_GetLoadedVehicleSquad(groupid) end

--- Returns the name of a given squad group.
--- RETURNS: String
---
---@param sgroup SGroupID
function SGroup_GetName(sgroup) end

--- Get the number of slot items with the same ID that the squads in the sgroup own
--- RETURNS: Int
---
---@param group SGroupID
---@param itemID integer
function SGroup_GetNumSlotItem(group, itemID) end

--- Returns the center position of a squad group.
--- RETURNS: Position
---
---@param group SGroupID
function SGroup_GetPosition(group) end

--- Get a random spawned squad from sgroup
--- RETURNS: SquadID
---
---@param sgroupid SGroup
function SGroup_GetRandomSpawnedSquad(sgroupid) end

--- Builds a table of SGroupIDs that are named in a sequence. i.e. a name of "sg_killer" will find groups "sg_killer1", "sg_killer2" and so on, up until it looks for a group that isn't there.
--- RETURNS: Table
---
---@param name string
function SGroup_GetSequence(name) end

--- Returns the spawned squad at a certain position in the group.
--- RETURNS: SquadID
---
---@param group SGroupID
---@param index integer
function SGroup_GetSpawnedSquadAt(group, index) end

--- Returns the distance from the centre of the group of the unit that furthest out. 
--- RETURNS: Real
---
---@param sgroup SGroupID
function SGroup_GetSpread(sgroup) end

--- Returns an sgroup containing all squads held by any squad in an sgroup
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param sgroupRecipient SGroupID
function SGroup_GetSquadsHeld(sgroup, sgroupRecipient) end

--- Get the suppression level for the first squad in the sgroup
--- RETURNS: Float
---
---@param groupid SGroupID
function SGroup_GetSuppression(groupid) end

--- Get the veterancy experience value for the first squad in the sgroup
--- RETURNS: Float
---
---@param groupid SGroupID
function SGroup_GetVeterancyExperience(groupid) end

--- Get the veterancy rank for the first squad in the sgroup
--- RETURNS: Int
---
---@param groupid SGroupID
function SGroup_GetVeterancyRank(groupid) end

--- Returns a table of sgroups from the world builder
--- RETURNS: LuaTable
---
---@param format string
function SGroup_GetWBTable(format) end

--- Returns true if ANY or ALL of the squads in the SGroup have the specified critical
--- RETURNS: Boolean
---
---@param group SGroupID
---@param critical CriticalID
---@param all boolean
function SGroup_HasCritical(group, critical, all) end

--- Check if ALL or ANY squads in a sgroup have a leader
--- RETURNS: Boolean
---
---@param sgroup SGroupID
---@param all boolean
function SGroup_HasLeader(sgroup, all) end

--- Returns true if ANY or ALL of the squads in an SGroup are carrying some kind of team weapon
--- RETURNS: Boolean
---
---@param sgroup SGroupID
---@param all boolean
function SGroup_HasTeamWeapon(sgroup, all) end

--- Returns whether ANY or ALL squads in an SGroup have the specified upgrade
--- RETURNS: Boolean
---
---@param sgroup SGroupID
---@param upgrade UpgradeID
---@param all boolean
function SGroup_HasUpgrade(sgroup, upgrade, all) end

--- Hide or show all entities in all squads in an SGroup
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param hide boolean
function SGroup_Hide(sgroup, hide) end

--- Increase squad veterancy experience for all squads in the sgroup. Can do silent promotion (no sound/UI). Can skip modifiers, giving you direct control of experience gained.
--- RETURNS: Void
---
---@param groupid SGroupID
---@param experience[ number
---@param silent boolean
---@param applyModifiers] boolean
function SGroup_IncreaseVeterancyExperience(groupid, experience[, silent, applyModifiers]) end

--- Increase squad veterancy rank for all squads in the sgroup. By default, increases rank by 1. Can do silent promotion (no sound/UI. ex: mass rank insrease at mission start)
--- RETURNS: Void
---
---@param groupid[ SGroupID
---@param numranks integer
---@param silent] boolean
function SGroup_IncreaseVeterancyRank(groupid[, numranks, silent]) end

---
---Located in FILE: \scar\deprecated.scar LINE: 468
---@param sgroupid any
function SGroup_InstantSetup(sgroupid) end

--- Same as EGroup_Intersection. Note: You cannot mix squad groups and entity groups.
--- RETURNS: Void
---
---@param group SGroupID
---@param grouptointersect SGroupID
function SGroup_Intersection(group, grouptointersect) end

--- Returns true if ANY or ALL squads in an SGroup are attack moving.
--- RETURNS: Boolean
---
---@param sgroup SGroupID
---@param all boolean
function SGroup_IsAttackMoving(sgroup, all) end

---TEMP
---
---Located in FILE: \scar\designerlib.scar LINE: 769
---@param group any
---@param all any
function SGroup_IsAttacking(group, all) end

--- Returns true if ALL or ANY squads are attacking within the time.
--- RETURNS: Boolean
---
---@param sgroup SGroupID
---@param all boolean
---@param time number
function SGroup_IsDoingAttack(sgroup, all, time) end

--- Returns true if ANY or ALL of the squads in the SGroup is dug in (or in the process of digging in)
--- RETURNS: Boolean
---
---@param group SGroupID
---@param all boolean
function SGroup_IsDugIn(group, all) end

--- Returns true if a named squad group is empty
--- RETURNS: Boolean
---
---@param sgroup SGroupID
function SGroup_IsEmpty(sgroup) end

--- Returns whether any entity in an SGroup has a hold on anything
--- RETURNS: Void
---
---@param sgroup SGroupID
function SGroup_IsHoldingAny(sgroup) end

--- Returns the percentage of the SGroup members that are in cover. Alternatively, specify ANY or ALL as a second parameter to return true/false.
--- RETURNS: Real/Boolean
---
---@param sgroup[ SGroupID
---@param all] boolean
function SGroup_IsInCover(sgroup[, all]) end

--- Checks if ANY or ALL squads in an SGroup are garrisoned in an entity (building) 
--- RETURNS: Boolean
---
---@param sgroup SGroupID
---@param ALL boolean
function SGroup_IsInHoldEntity(sgroup, ALL) end

--- Returns true if ALL or ANY squads in a group are infiltrated
--- RETURNS: Boolean
---
---@param sgroup SGroupID
---@param all boolean
function SGroup_IsInfiltrated(sgroup, all) end

--- Returns true if ANY or ALL squads in an SGroup are moving.
--- RETURNS: Boolean
---
---@param sgroup SGroupID
---@param all boolean
function SGroup_IsMoving(sgroup, all) end

--- Returns true if ANY or ALL (use those keywords) of the squads in the group are present onscreen. You can pass in a percentage of the screen to check, so 0.8 would be a centered rectangle occupying 80% of the screen.
--- RETURNS: Bool
---
---@param player PlayerID
---@param group SGroupID
---@param all[ boolean
---@param percent] number
function SGroup_IsOnScreen(player, group, all[, percent]) end

--- Returns true if ANY or ALL of a group is pinned
--- RETURNS: Boolean
---
---@param group SGroupID
---@param all boolean
function SGroup_IsPinned(group, all) end

--- Returns true if ANY or ALL of the squads in an SGroup are currently retreating
--- RETURNS: Boolean
---
---@param sgroup SGroupID
---@param all boolean
function SGroup_IsRetreating(sgroup, all) end

--- Returns true is ANY or ALL of a group is suppressed
--- RETURNS: Boolean
---
---@param group SGroupID
---@param all boolean
function SGroup_IsSuppressed(group, all) end

--- Returns true if ALL or ANY squads are under attack within the time.
--- RETURNS: Boolean
---
---@param sgroup SGroupID
---@param all boolean
---@param time number
function SGroup_IsUnderAttack(sgroup, all, time) end

--- Check if the squads are attacked by the player
--- RETURNS: Bool
---
---@param group SGroupID
---@param attackerplayer PlayerID
---@param duration number
function SGroup_IsUnderAttackByPlayer(group, attackerplayer, duration) end

--- Kills all squads in an SGroup. This kills them 'naturally', as opposed to SGroup_DestroyAllSquads() which makes them blink out of existance.
--- RETURNS: Void
---
---@param sgroup SGroupID
function SGroup_Kill(sgroup) end

--- Merge first squads in the SGroups. The resulting squad will be in both SGroups
--- RETURNS: 
---
---@param sgroupid1 SGroup
---@param sgroupid2 SGroup
function SGroup_MergeFirstSquad(sgroupid1, sgroupid2) end

--- Respawn all despawned squads in a group.
--- RETURNS: Void
---
---@param groupid SGroupID
function SGroup_ReSpawn(groupid) end

--- Removes an squadron from a group.
--- RETURNS: Void
---
---@param group SGroupID
---@param squadron SquadID
function SGroup_Remove(group, squadron) end

--- Remove from the first SGroup all squads contained in the second SGroup. SGroup2 remains untouched.
--- RETURNS: Void
---
---@param group SGroupID
---@param grouptoremove SGroupID
function SGroup_RemoveGroup(group, grouptoremove) end

--- Restore all current combat plans for the squads in the sgroup
--- RETURNS: Void
---
---@param sgroupID SGroupID
function SGroup_RestoreCombatPlans(sgroupID) end

--- Sets whether a weapon to auto-target things or not
--- RETURNS: Void
---
---@param group SGroupID
---@param hardpoint string
---@param enable boolean
function SGroup_SetAutoTargetting(group, hardpoint, enable) end

--- Sets the health of each squad in a squad group to a given percent [0.0, 1.0].
--- RETURNS: Real
---
---@param sgroup SGroupID
---@param healthpercent number
function SGroup_SetAvgHealth(sgroup, healthpercent) end

--- Sets the moral of each squad in a squad group to a given percent [0.0, 1.0].
--- RETURNS: Real
---
---@param sgroup SGroup
---@param moralepercent number
function SGroup_SetAvgMorale(sgroup, moralepercent) end

--- Enable/Disable invulnerablity for an entire SGroup. Use true and false for simple on/off, or use a number between 0.0 and 1.0 for more precise control on how much damage a squad can take before it takes no more.
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param enabled[ Boolean/Real
---@param reset_time] number
function SGroup_SetInvulnerable(sgroup, enabled[, reset_time]) end

--- Set soldier mood mode. Mode could be MM_Auto, MM_ForceCalm or MM_ForceTense
--- RETURNS: Void
---
---@param group SGroupID
---@param mode integer
function SGroup_SetMoodMode(group, mode) end

--- Set the move type for an SGroup - move type is either MOVETYPE_SAFE, MOVETYPE_NORMAL, MOVETYPE_FAST
--- RETURNS: Void
---
---@param group SGroupID
---@param movetype string
function SGroup_SetMoveType(group, movetype) end

--- Changes the player owner of spawned and despawned squads in an SGroup.
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param owner PlayerID
function SGroup_SetPlayerOwner(sgroup, owner) end

--- Set player selectable state of squads in the sgroup
--- RETURNS: 
---
---@param sgroupid SGroup
---@param selectable bool
function SGroup_SetSelectable(sgroupid, selectable) end

--- Set the suppression level for all squads in the sgroup
--- RETURNS: Void
---
---@param groupid SGroupID
---@param suppression number
function SGroup_SetSuppression(groupid, suppression) end

--- Set team weapon in the squads to be capturable or not
--- RETURNS: Void
---
---@param group SGroupID
---@param enable boolean
function SGroup_SetTeamWeaponCapturable(group, enable) end

--- Creates a squad group containing a single squad
--- RETURNS: SGroupID
---
---@param groupID SGroupID
---@param squad squadID
function SGroup_Single(groupID, squad) end

--- Makes two SGroups face each other at no time
--- RETURNS: Void
---
---@param sgroup1 SGroupID
---@param sgroup2 SGroupID
function SGroup_SnapFaceEachOther(sgroup1, sgroup2) end

--- Works like SGroup_FacePosition except with no interpolation.  All Squads will face the same direction, with the squad the closest to the center determining the direction.
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param pos Position
function SGroup_SnapFacePosition(sgroup, pos) end

--- Split first squad into a new squad with size "num"
--- RETURNS: SquadID
---
---@param sgroupid1 SGroup
---@param num INT
function SGroup_SplitFirstSquad(sgroupid1, num) end

--- Suggests a posture to an SGroup, lasting the passed duration
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param posture unsigned
---@param duration number
function SGroup_SuggestPosture(sgroup, posture, duration) end

--- Returns the total count of all members of all the squads in a given SGroup.
--- RETURNS: Integer
---
---@param sgroup SGroupID
function SGroup_TotalMembersCount(sgroup) end

--- Warps all members of an SGroup immediately to a marker
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param marker MarkerID
function SGroup_WarpToMarker(sgroup, marker) end

--- Warps all members of an SGroup immediately to a new position
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param pos Position
function SGroup_WarpToPos(sgroup, pos) end

--- Register an init function with the scar system.
--- RETURNS: Void
---
---@param f function
function Scar_AddInit(f) end

--- Saves the game. the savegame name is built using the abbreviated mission name + checkpoint.
--- RETURNS: Void
---
---@param checkpoint LocString
function Scar_Autosave(checkpoint) end

--- Awards a medal to the current player. The campaign must be set up to allow that medal to be awarded during this mission, or you will get a fatal SCAR error.
--- RETURNS: Void
---
---@param pbgID integer
function Scar_AwardMedal(pbgID) end

--- execute console command string. Will only work if dev mode is enabled! (it's OFF by default in RTM builds)
--- RETURNS: Void
---
---@param command string
function Scar_DebugConsoleExecute(command) end

--- Returns true if an init function exists
--- RETURNS: Boolean
---
---@param f function
function Scar_InitExists(f) end

--- Play NIS through SCAR. Should only be used inside a event function
--- RETURNS: Void
---
---@param filename string
function Scar_PlayNIS(filename) end

--- Play 2 NISs through SCAR. Should only be used inside a event function
--- RETURNS: Void
---
---@param filename1 string
---@param filename2 string
function Scar_PlayNIS2(filename1, filename2) end

--- Returns whether the player has a medal that can be awarded during this mission.
--- RETURNS: Boolean
---
---@param pbgID integer
function Scar_PlayerHasMedal(pbgID) end

--- Unregister an init function that was registered from Scar_AddInit
--- RETURNS: Void
---
---@param f function
function Scar_RemoveInit(f) end

--- Starts COH "screensaver".  Picks random squads and looks at them from various camera angles.
---
---Located in FILE: \scar\scarutil.scar LINE: 1968
---@param flag any
---@param turnonAI any
function Screensaver(flag, turnonAI) end

--- mark the objective as complete once all road flags are captured
---
---Located in FILE: \scar\capturing_toggleui.scar LINE: 11
function SecureRoad_Check() end

--- Sets up difficulty (currently: modifies entity health, and sets AI difficulty). You should pass in Game_GetSPDifficulty() as the difficulty.
--- RETURNS: 
---
---@param player[ PlayerID
---@param difficulty] integer
function Setup_Difficulty(player[, difficulty]) end

--- Get the victory point ticker option selected
--- RETURNS: Real
---
---@param Void any
function Setup_GetVictoryPointTickerOption(Void) end

--- Initializes the all of the players settings in one function and returns the playerID.
--- RETURNS: PlayerID
---
---@param playerIndex integer
---@param playerName LocString
---@param playerRace string
---@param team integer
function Setup_Player(playerIndex, playerName, playerRace, team) end

--- Set the UI name of a given player.
--- RETURNS: Void
---
---@param player PlayerID
---@param name LocString
function Setup_SetPlayerName(player, name) end

--- Set the race for a given player.  Use World_GetRaceIndex() to get the race index from the ME name.
--- RETURNS: Void
---
---@param player PlayerID
---@param raceIndex integer
function Setup_SetPlayerRace(player, raceIndex) end

--- Put a player in a team. Use TEAM_NEUTRAL as the team_id to set the player as neutral
--- RETURNS: Void
---
---@param p PlayerID
---@param team_id integer
function Setup_SetPlayerTeam(p, team_id) end

--- Forces a sync weapon to shoot at the sky, so long as it's manned by a given player.
--- RETURNS: Void
---
---@param syncweapon SyncWeaponID
---@param player PlayerID
function ShootTheSky_AddSyncWeapon(syncweapon, player) end

---
---Located in FILE: \scar\designerlib.scar LINE: 1097
function ShootTheSky_Manager() end

--- Stops all sync weapons from going through their "shooting at the sky" routine.
--- RETURNS: Void
---
function ShootTheSky_RemoveAll() end

--- Removes a sync weapon from the "shoot at the sky" system. It can then target people again.
--- RETURNS: Void
---
---@param syncweapon SyncWeaponID
function ShootTheSky_RemoveSyncWeapon(syncweapon) end

--- Plays a global speech with subtitle and actor icon in the overlay
--- RETURNS: Void
---
---@param icon string
---@param locID integer
---@param disableIconSubtitle boolean
---@param continueButton boolean
---@param blockInput boolean
---@param stickySubtitle boolean
---@param additionalPath string
function SitRep_PlaySpeech(icon, locID, disableIconSubtitle, continueButton, blockInput, stickySubtitle, additionalPath) end

--- Turns container debugging on/off
--- RETURNS: Void
---
---@param Void any
function Sound_ContainerDebug(Void) end

--- Disables certain speech events.
--- RETURNS: Void
---
---@param event integer
function Sound_DisableSpeechEvent(event) end

--- Plays a 2D sound from the Data:Sound folder.  Use the path relative to Sound folder without the file extension.
--- RETURNS: Void
---
---@param name string
function Sound_Play2D(name) end

--- Plays a 3D sound from the Data:Sound folder on the entity.  Use the path relative to Sound folder without the file extension.
--- RETURNS: Void
---
---@param name string
---@param actor EntityID
function Sound_Play3D(name, actor) end

--- Plays music from the Data:Sound folder.
--- RETURNS: Void
---
---@param name string
---@param fade number
---@param delay number
function Sound_PlayMusic(name, fade, delay) end

--- Play sound on the first entity of the squad in sgroup
--- RETURNS: Void
---
---@param soundpathname string
---@param sgroupid SGroupID
function Sound_PlayOnSquad(soundpathname, sgroupid) end

--- Plays a streaming sound from the Data:Sound folder.  Use the path relative to Sound folder without the file extension.
--- RETURNS: Void
---
---@param name string
function Sound_PlayStreamed(name) end

--- pre-caches single player speech. For instance pre-caching mission 1 speech is done like: Sound_PreCacheSinglePlayerSpeech( "mission01" )
--- RETURNS: Void
---
---@param mission string
function Sound_PreCacheSinglePlayerSpeech(mission) end

--- pre-caches a sound
--- RETURNS: Void
---
---@param name string
function Sound_PreCacheSound(name) end

--- Sets new master reverb parameters.
--- RETURNS: Void
---
---@param time number
---@param predelay number
---@param damping number
function Sound_SetReverb(time, predelay, damping) end

--- Sets a new volume for the group.
--- RETURNS: Void
---
---@param name string
---@param volume number
---@param time number
function Sound_SetVolume(name, volume, time) end

--- Sets the default volume for the group.
--- RETURNS: Void
---
---@param name string
---@param time number
function Sound_SetVolumeDefault(name, time) end

--- Starts recording the sound driver output
--- RETURNS: Void
---
---@param name string
function Sound_StartRecording(name) end

--- Stop current playing music
--- RETURNS: Void
---
---@param fade number
---@param delay number
function Sound_StopMusic(fade, delay) end

--- Stops recording the sound driver output
--- RETURNS: Void
---
---@param Void any
function Sound_StopRecording(Void) end

--- Call this in night missions to have the soldiers use their stealth speech read codes
--- RETURNS: Void
---
---@param useStealthRead boolean
function Speech_SetGlobalStealthRead(useStealthRead) end

--- Add to the list of slot items to drop when this squad is wiped out
--- RETURNS: Void
---
---@param squad SquadID
---@param itemID integer
---@param dropChance number
---@param exclusive boolean
function Squad_AddSlotItemToDropOnDeath(squad, itemID, dropChance, exclusive) end

--- Returns true if squad can capture stategic point
--- RETURNS: Boolean
---
---@param squad SquadID
---@param entity EntityID
function Squad_CanCaptureStrategicPoint(squad, entity) end

--- Returns true if the squad is available to be reinforced
--- RETURNS: Boolean
---
---@param squad SquadID
function Squad_CanInstantReinforceNow(squad) end

--- Returns true if a given entity is in the squads sight radius
--- RETURNS: Boolean
---
---@param squad SquadID
---@param entity EntityID
function Squad_CanSeeEntity(squad, entity) end

--- Returns true if a target squad is in the squads sight radius
--- RETURNS: Boolean
---
---@param squad SquadID
---@param target SquadID
function Squad_CanSeeSquad(squad, target) end

--- Clears any previous posture suggestions made to a squad
--- RETURNS: Void
---
---@param squad SquadID
function Squad_ClearPostureSuggestion(squad) end

--- Returns the number of units currently in a squad (spawned AND despawned!!)
--- RETURNS: Integer
---
---@param squad SquadID
function Squad_Count(squad) end

--- Create a squad, spawn it and assign it to a player. 
--- RETURNS: SquadID
---
---@param squadBlueprintID integer
---@param player PlayerID
---@param loadoutCount integer
---@param pos Position
function Squad_CreateAndSpawn(squadBlueprintID, player, loadoutCount, pos) end

--- Create a squad, spawn it and assign it to a player. 
--- RETURNS: SquadID
---
---@param squadBlueprintID integer
---@param player PlayerID
---@param loadoutCount integer
---@param pos Position
---@param toward Position
function Squad_CreateAndSpawnToward(squadBlueprintID, player, loadoutCount, pos, toward) end

--- Despawn the entire squad at its current position.
--- RETURNS: Void
---
---@param squad SquadID
function Squad_DeSpawn(squad) end

--- Remove an squad from the world and destroy it.
--- RETURNS: Void
---
---@param squad SquadID
function Squad_Destroy(squad) end

--- Enables or disables the surprise feature on thie given squad
--- RETURNS: Void
---
---@param squad SquadID
---@param enable boolean
function Squad_EnableSurprise(squad, enable) end

--- Set the rotation of all troopers in a squad to face the position.
--- RETURNS: Void
---
---@param squad SquadID
---@param pos Position
function Squad_FacePosition(squad, pos) end

--- Get 2 squads to face each other. This function works on spawned squads only.
--- RETURNS: Void
---
---@param squad1 SquadID
---@param squad2 SquadID
function Squad_FaceSquad(squad1, squad2) end

--- Tries to find cover within a certain radius of a position. If no cover is found, it returns the position used for the search.
--- RETURNS: Position
---
---@param squad SquadID
---@param pos Position
---@param coverSearchRadius number
function Squad_FindCover(squad, pos, coverSearchRadius) end

--- Tries to find cover within a certain radius of a position, traveling a max distance to get there, and possibly comparing against current position's cover. If no cover is found, it returns the position used for the search.
--- RETURNS: Position
---
---@param squad SquadID
---@param pos Position
---@param coverSearchRadius number
---@param maxPathDistanceFromGoal number
---@param compareToCurrentCover boolean
function Squad_FindCoverCompareCurrent(squad, pos, coverSearchRadius, maxPathDistanceFromGoal, compareToCurrentCover) end

--- Get a squad from a mission editor ID.
--- RETURNS: SquadID
---
---@param id integer
function Squad_FromWorldID(id) end

--- Returns the active squad command.  Valid squad commands are SQUADSTATEID_Idle, SQUADSTATEID_Move, SQUADSTATEID_Stop, SQUADSTATEID_Combat, SQUADSTATEID_Capture, SQUADSTATEID_Ability, SQUADSTATEID_Construction, SQUADSTATEID_CombatStance, SQUADSTATEID_Load, SQUADSTATEID_Unload, SQUADSTATEID_HoldUnload, SQUADSTATEID_Retreat, SQUADSTATEID_WeaponTransition, SQUADSTATEID_AttackMove, SQUADSTATEID_CaptureTeamWeapon, SQUADSTATEID_Defuse, SQUADSTATEID_PlaceCharges, SQUADSTATEID_DefuseMine, SQUADSTATEID_PickUpSlotItem, SQUADSTATEID_Plan, SQUADSTATEID_Patrol, SQUADSTATEID_RevertFieldSupport.
--- RETURNS: Integer
---
---@param squad SquadID
function Squad_GetActiveCommand(squad) end

--- Get name of current Squad AI attack plan
--- RETURNS: String
---
---@param squad SquadID
function Squad_GetAttackPlan(squad) end

--- Find the squad member current or forced targets. If found, the targets squads are added to the sgroup. Entity targets like buildings are ignored
--- RETURNS: Void
---
---@param squad SquadID
---@param group SGroupID
function Squad_GetAttackTargets(squad, group) end

--- Returns a string containing the base unit name for the current squadron
--- RETURNS: String
---
---@param squad SquadID
function Squad_GetBaseUnitName(squad) end

--- Returns the id of the squad blueprint
--- RETURNS: Integer
---
---@param squad SquadID
function Squad_GetBlueprintID(squad) end

--- Returns the name of the squad blueprint (from the attribute editor)
--- RETURNS: String
---
---@param squad SquadID
function Squad_GetBlueprintName(squad) end

--- Returns the squad's destination, if it's moving. IMPORTANT: you must only call this function if Squad_HasDestination has returned true.
--- RETURNS: Position
---
---@param squad SquadID
function Squad_GetDestination(squad) end

--- Returns an integer containing the unqiue squad ID for this squad.
--- RETURNS: Integer
---
---@param squad SquadID
function Squad_GetGameID(squad) end

--- Returns the average heading of the spawned units in the squad. The heading is currently a lua table with three entries (x, y, z)
--- RETURNS: Position
---
---@param squad SquadID
function Squad_GetHeading(squad) end

--- Returns the current health of a squad.
--- RETURNS: Real
---
---@param squad SquadID
function Squad_GetHealth(squad) end

--- Returns the max health of the squad.
--- RETURNS: Real
---
---@param squad SquadID
function Squad_GetHealthMax(squad) end

--- Get which building (entity) is the squad garrisoned
--- RETURNS: EntityID
---
---@param squad SquadID
function Squad_GetHoldEntity(squad) end

--- Get which vehicle (squad) is the squad garrisoned
--- RETURNS: SquadID
---
---@param squad SquadID
function Squad_GetHoldSquad(squad) end

--- Check if the squad is invulnerable or not
--- RETURNS: Bool
---
---@param squad SquadID
function Squad_GetInvulnerable(squad) end

--- Returns the highest invulnerable min cap percentage from members of the squad.
--- RETURNS: Real
---
---@param squad SquadID
function Squad_GetInvulnerableMinCap(squad) end

--- Find the last squad attacker on this squad. If found, the squad is added to the sgroup
--- RETURNS: Void
---
---@param squad SquadID
---@param group SGroupID
function Squad_GetLastAttacker(squad, group) end

--- Find the squad attackers on this squad from the last seconds specified. If found, the squads are added to the sgroup. Building attackers are ignored
--- RETURNS: Void
---
---@param squad SquadID
---@param group SGroupID
---@param timeSeconds number
function Squad_GetLastAttackers(squad, group, timeSeconds) end

--- Returns the max number of units allowed in the squadron (from squad_loadout_ext)
--- RETURNS: Integer
---
---@param squad SquadID
function Squad_GetMax(squad) end

--- Get the number of slot items with the same ID that the squad has
--- RETURNS: Integer
---
---@param squad SquadID
---@param itemID integer
function Squad_GetNumSlotItem(squad, itemID) end

--- Get name of current Squad AI reaction plan
--- RETURNS: String
---
---@param squad SquadID
function Squad_GetPinnedPlan(squad) end

--- Returns the Player owner of the given squad. Squad MUST NOT be owned by the world.
--- RETURNS: PlayerID
---
---@param squad SquadID
function Squad_GetPlayerOwner(squad) end

--- Returns the average position of the spawned units in the squad. The position is currently a lua table with three entries (x, y, z)
--- RETURNS: Position
---
---@param squad SquadID
function Squad_GetPosition(squad) end

--- Returns the average position of the despawned AND spawned units in the squad.
--- RETURNS: Position
---
---@param squad SquadID
function Squad_GetPositionDeSpawned(squad) end

--- Get name of current Squad AI reaction plan
--- RETURNS: String
---
---@param squad SquadID
function Squad_GetReactionPlan(squad) end

--- Get name of current Squad AI retaliation plan
--- RETURNS: String
---
---@param squad SquadID
function Squad_GetRetaliationPlan(squad) end

--- Returns the ID of the slot item. Use Squad_GetSlotItemCount to determine how many slot items the squad has. The first index is 1
--- RETURNS: Integer
---
---@param squad SquadID
---@param index integer
function Squad_GetSlotItemAt(squad, index) end

--- Returns how many slot items this squad has
--- RETURNS: Integer
---
---@param squad SquadID
function Squad_GetSlotItemCount(squad) end

--- Returns a table of SlotItem ID's that this squad currently owns
--- RETURNS: LuaTable
---
---@param squadid SquadID
function Squad_GetSlotItemsTable(squadid) end

--- Adds all squads held by 'squad' to an sgroup
--- RETURNS: Boolean
---
---@param squad SquadID
---@param sgroup SGroupID
function Squad_GetSquadsHeld(squad, sgroup) end

--- Check current squad suppresion level. Return value from 0 to 1
--- RETURNS: Real
---
---@param squad SquadID
function Squad_GetSuppression(squad) end

--- Get current squad veterancy experience
--- RETURNS: Real
---
---@param squad SquadID
function Squad_GetVeterancyExperience(squad) end

--- Get current squad veterancy rank.
--- RETURNS: Integer
---
---@param squad SquadID
function Squad_GetVeterancyRank(squad) end

--- Gives a slot item to the squad. Can fail due to not enough slots left
--- RETURNS: Void
---
---@param squad SquadID
---@param itemID integer
function Squad_GiveSlotItem(squad, itemID) end

--- Gives all slot items in a table to the squad. The table should come from Squad_GetSlotItemsTable
--- RETURNS: Void
---
---@param squadid SquadID
---@param itemTable LuaTable
function Squad_GiveSlotItemsFromTable(squadid, itemTable) end

--- Returns true if there's an active command currently for the squad
--- RETURNS: Boolean
---
---@param squad SquadID
function Squad_HasActiveCommand(squad) end

--- Check if a squad has a critical or not
--- RETURNS: Bool
---
---@param squad SquadID
---@param critical CriticalID
function Squad_HasCritical(squad, critical) end

--- Returns whether this squad is moving and has a destination
--- RETURNS: Boolean
---
---@param squad SquadID
function Squad_HasDestination(squad) end

--- Returns true if the squad is carrying some kind of team weapon
--- RETURNS: Boolean
---
---@param squad SquadID
function Squad_HasTeamWeapon(squad) end

--- Return true if the squad has purchased the specified upgrade.
--- RETURNS: Boolean
---
---@param squad SquadID
---@param pbgID integer
function Squad_HasUpgrade(squad, pbgID) end

--- Increase current squad veterancy experience
--- RETURNS: Void
---
---@param squad SquadID
---@param experience number
---@param silent boolean
---@param applyModifiers boolean
function Squad_IncreaseVeterancyExperience(squad, experience, silent, applyModifiers) end

--- Increase current squad veterancy rank
--- RETURNS: Void
---
---@param squad SquadID
---@param numranks integer
---@param silent boolean
function Squad_IncreaseVeterancyRank(squad, numranks, silent) end

--- Stops current squads activity and instant setup the team weapon if they have one
--- RETURNS: Void
---
---@param squad SquadID
function Squad_InstantSetupTeamWeapon(squad) end

--- Returns true if any unit in the squad is attacking within the time
--- RETURNS: Boolean
---
---@param squad SquadID
---@param time number
function Squad_IsAttacking(squad, time) end

--- Check if the squad has a hold on anything (use this on vehicles)
--- RETURNS: Boolean
---
---@param squad SquadID
function Squad_IsHoldingAny(squad) end

--- Returns true if ALL or ANY troopers in a squad are in cover.
--- RETURNS: Boolean
---
---@param squadId SquadID
---@param all boolean
function Squad_IsInCover(squadId, all) end

--- Check if the squad is garrisoned in entity (building)
--- RETURNS: Boolean
---
---@param squad SquadID
function Squad_IsInHoldEntity(squad) end

--- Check if the squad is loaded in squad (vehicle
--- RETURNS: Boolean
---
---@param squad SquadID
function Squad_IsInHoldSquad(squad) end

--- Returns whether any entity in the squad is moving
--- RETURNS: Boolean
---
---@param squad SquadID
function Squad_IsMoving(squad) end

--- True if squad is currently pinned
--- RETURNS: Boolean
---
---@param squad SquadID
function Squad_IsPinned(squad) end

--- Returns true if the squad is currently reinforcing.  This function will return false if the squad does not have a reinforce ext.
--- RETURNS: Boolean
---
---@param squad SquadID
function Squad_IsReinforcing(squad) end

--- Returns true if the squad is currently retreating
--- RETURNS: Boolean
---
---@param squadid SquadID
function Squad_IsRetreating(squadid) end

--- True if squad is currently suppressed
--- RETURNS: Boolean
---
---@param squad SquadID
function Squad_IsSuppressed(squad) end

--- Returns true if any unit in the squad is under attack within the time
--- RETURNS: Boolean
---
---@param squad SquadID
---@param time number
function Squad_IsUnderAttack(squad, time) end

--- Returns true if squad is under attack by enemy from a particular player
--- RETURNS: Boolean
---
---@param squad SquadID
---@param pAttackerOwner PlayerID
---@param time number
function Squad_IsUnderAttackByPlayer(squad, pAttackerOwner, time) end

--- Returns true if the squad is currently upgrading. Pass in -1 for pbgID if it doesn't matter what is being upgraded.
--- RETURNS: Boolean
---
---@param squad SquadID
---@param pbgID integer
function Squad_IsUpgrading(squad, pbgID) end

--- Check if a squad with the given ID can be found in the world
--- RETURNS: Boolean
---
---@param id integer
function Squad_IsValid(id) end

--- Kill whole squad.  Sets health to 0, and triggers death effects.
--- RETURNS: Void
---
---@param squad SquadID
function Squad_Kill(squad) end

--- Merge source squad into destination squad.
--- RETURNS: Void
---
---@param dest SquadID
---@param source SquadID
function Squad_Merge(dest, source) end

--- Set overriding Squad AI attack plan 
--- RETURNS: Void
---
---@param squad SquadID
---@param planName string
function Squad_SetAttackPlan(squad, planName) end

--- Set the health of all units in a squad.  Health must be in range [0.0, 1.0]
--- RETURNS: Void
---
---@param squad SquadID
---@param healthPercent number
function Squad_SetHealth(squad, healthPercent) end

--- Set invulnerability on the squad. Reset time is in seconds. If it it set, the invulnerability will expire after this time.
--- RETURNS: Void
---
---@param squad SquadID
---@param enable boolean
---@param reset_time number
function Squad_SetInvulnerable(squad, enable, reset_time) end

--- Make a squad invulnerable to physical damage.
--- RETURNS: Void
---
---@param squad SquadID
---@param minHealthPercentage number
---@param resetTime number
function Squad_SetInvulnerableMinCap(squad, minHealthPercentage, resetTime) end

--- Set soldier mood mode. Can be MM_Auto, MM_ForceCalm and MM_ForceTense
--- RETURNS: Void
---
---@param squad SquadID
---@param mode integer
function Squad_SetMoodMode(squad, mode) end

--- Sets the squad's move type
--- RETURNS: Void
---
---@param squad SquadID
---@param moveTypeName string
function Squad_SetMoveType(squad, moveTypeName) end

--- Set overriding Squad AI pinned plan
--- RETURNS: Void
---
---@param squad SquadID
---@param planName string
function Squad_SetPinnedPlan(squad, planName) end

--- Changes the owner of the given squad.
--- RETURNS: Void
---
---@param squad SquadID
---@param owner PlayerID
function Squad_SetPlayerOwner(squad, owner) end

--- Set overriding Squad AI reaction plan
--- RETURNS: Void
---
---@param squad SquadID
---@param planName string
function Squad_SetReactionPlan(squad, planName) end

--- Set overriding Squad AI idle retaliation plan
--- RETURNS: Void
---
---@param squad SquadID
---@param planName string
function Squad_SetRetaliationPlan(squad, planName) end

--- Set current squad suppresion level. Suppression value value from 0 to 1
--- RETURNS: Void
---
---@param squad SquadID
---@param percentage number
function Squad_SetSuppression(squad, percentage) end

--- Set team weapon entity inside the squad to be capturable or not when it becomes neutral
--- RETURNS: Void
---
---@param squad SquadID
---@param capturable boolean
function Squad_SetTeamWeaponCapturable(squad, capturable) end

--- Spawn the entire squad at a given position
--- RETURNS: Boolean
---
---@param squad SquadID
---@param pos Position
function Squad_Spawn(squad, pos) end

--- Spawn the entire squad at a given position
--- RETURNS: Boolean
---
---@param squad SquadID
---@param pos Position
---@param toward Position
function Squad_SpawnToward(squad, pos, toward) end

--- Split the squad into 2. The new squad size is specified by the number passed in
--- RETURNS: SquadID
---
---@param squad SquadID
---@param num integer
function Squad_Split(squad, num) end

--- Suggests a posture to a squad, lasting the passed duration
--- RETURNS: Void
---
---@param squad SquadID
---@param posture unsigned
---@param duration number
function Squad_SuggestPosture(squad, posture, duration) end

--- Warps a squad immediately to a marker
--- RETURNS: Void
---
---@param squad SquadID
---@param marker MarkerID
function Squad_WarpToMarker(squad, marker) end

--- Warps a squad immediately to a new position
--- RETURNS: Void
---
---@param squad SquadID
---@param pos Position
function Squad_WarpToPos(squad, pos) end

--- Takes a statistic function and totals up the results for all the players on a given team
--- RETURNS: Integer
---
---@param teamindex integer
---@param statfunction function
function Stats_TeamTally(teamindex, statfunction) end

--- Add a stinger to trigger at a specific location
--- RETURNS: 
---
---@param player PlayerID
---@param marker MarkerID
---@param eventname EVENT
function Stinger_AddEvent(player, marker, eventname) end

--- Add a stinger to trigger at a specific location
--- RETURNS: 
---
---@param player PlayerID
---@param marker MarkerID
---@param functionname function
function Stinger_AddFunction(player, marker, functionname) end

--- this is the rule that checks the stingers during the mission
---
---Located in FILE: \scar\stingers.scar LINE: 60
function Stinger_Manager() end

--- Remove all stingers triggered from a specific location
--- RETURNS: 
---
---@param player PlayerID
---@param marker MarkerID
function Stinger_Remove(player, marker) end

--- Prematurely finish currently playing speech and advance the next one in queue
--- RETURNS: Void
---
---@param Void any
function Subtitle_EndCurrentSpeech(Void) end

--- Plays a global speech with subtitle and actor icon in the overlay
--- RETURNS: Void
---
---@param icon string
---@param locID integer
---@param disableIconSubtitle boolean
---@param continueButton boolean
---@param blockInput boolean
---@param stickySubtitle boolean
---@param additionalPath string
function Subtitle_PlaySpeech(icon, locID, disableIconSubtitle, continueButton, blockInput, stickySubtitle, additionalPath) end

--- Plays a subtitle at a position on screen.
--- RETURNS: Void
---
---@param locID integer
---@param x number
---@param y number
---@param stickySubtitle boolean
---@param additionalPath string
function Subtitle_PlaySpeechAt(locID, x, y, stickySubtitle, additionalPath) end

--- Plays 3D speech on one of guys in the squad with subtitle and actor icon in the overlay
--- RETURNS: Void
---
---@param squad SquadID
---@param icon string
---@param locID integer
---@param disableIconSubtitle boolean
---@param continueButton boolean
---@param blockInput boolean
---@param stickySubtitle boolean
---@param additionalPath string
function Subtitle_PlaySquadSpeech(squad, icon, locID, disableIconSubtitle, continueButton, blockInput, stickySubtitle, additionalPath) end

--- Removes "sticky" state from currently playing speech (if any)
--- RETURNS: Void
---
---@param Void any
function Subtitle_UnstickCurrentSpeech(Void) end

--- Returns true if a sync weapon still exists in the game world
--- RETURNS: Boolean
---
---@param weapon SyncWeaponID
function SyncWeapon_Exists(weapon) end

--- Returns the EntityID of a sync weapon, or nil if it's been destroyed
--- RETURNS: EntityID
---
---@param weapon SyncWeaponID
function SyncWeapon_GetEntity(weapon) end

--- Registers the sync weapon in the egroup and returns a SyncWeaponID you can use to keep track of it
--- RETURNS: SyncWeaponID
---
---@param egroup EGroupID
function SyncWeapon_GetFromEGroup(egroup) end

--- Registers the sync weapon in the sgroup and returns a SyncWeaponID you can use to keep track of it
--- RETURNS: SyncWeaponID
---
---@param sgroup SGroupID
function SyncWeapon_GetFromSGroup(sgroup) end

--- Returns the position of a sync weapon, or nil if it's been destroyed
--- RETURNS: Position
---
---@param weapon SyncWeaponID
function SyncWeapon_GetPosition(weapon) end

--- Returns true if the specified player owns the sync weapon. Use a playerid of nil to see if it's unonwed.
--- RETURNS: Boolean
---
---@param weapon SyncWeaponID
---@param player PlayerID
function SyncWeapon_IsOwnedByPlayer(weapon, player) end

--- Sets whether a weapon to auto-target things or not
--- RETURNS: Void
---
---@param weapon SyncWeaponID
---@param hardpoint string
---@param enable boolean
function SyncWeapon_SetAutoTargetting(weapon, hardpoint, enable) end

--- Returns a random item from a table. You can return multiple items (without duplicates) by passing in an optional number parameter.
--- RETURNS: Item/Table
---
---@param table[ table
---@param numberofitems] integer
function Table_GetRandomItem(table[, numberofitems]) end

--- Add arrpw display to tactical map at specific vector. fnType are FN_OnShow, FN_OnSelect and FN_OnActivate
--- RETURNS: Integer
---
---@param objectiveID integer
---@param fnType integer
---@param from Position
---@param to Position
function TacticalMap_AddArrow(objectiveID, fnType, from, to) end

--- Add message display to tactical map at specific location. fnType are FN_OnShow, FN_OnSelect and FN_OnActivate
--- RETURNS: Integer
---
---@param objectiveID integer
---@param fnType integer
---@param pos Position
---@param message LocString
function TacticalMap_AddMessage(objectiveID, fnType, pos, message) end

--- Add pings to tactical map.
--- RETURNS: blipID
---
---@param where StackVar
---@param lifetime number
---@param blipType integer
function TacticalMap_AddPing(where, lifetime, blipType) end

--- Add pointer to tactical map
--- RETURNS: Integer
---
---@param objectiveID integer
---@param fnType integer
---@param to Position
function TacticalMap_AddPointer(objectiveID, fnType, to) end

--- Makes a territory flash (overrides any other flashing behavior)
--- RETURNS: Void
---
---@param sectorID integer
function TacticalMap_FlashTerritory(sectorID) end

--- Removes a tactical map element (created via the TacticalMap_Addxxx functions)
--- RETURNS: Void
---
---@param elementID integer
function TacticalMap_RemoveElement(elementID) end

--- Returns a territory to default flashing behavior
--- RETURNS: Void
---
---@param sectorID integer
function TacticalMap_StopFlashingTerritory(sectorID) end

--- Returns whether the taskbar is visible
--- RETURNS: Boolean
---
---@param Void any
function Taskbar_IsVisible(Void) end

--- sets taskbar visibility!
--- RETURNS: Void
---
---@param visible boolean
function Taskbar_SetVisibility(visible) end

---
---Located in FILE: \scar\techtreesetup.scar LINE: 323
function TechTreeRestore() end

--- Sets up tech tree locking / unlocking for each mission in the SP campaign
---
---Located in FILE: \scar\techtreesetup.scar LINE: 3
function TechTreeSetup() end

---
---Located in FILE: \scar\screensaver.scar LINE: 166
function TestSpawn() end

--- Add a rule to be executed every frame
--- RETURNS: Void
---
---@param f function
---@param priority integer
function TimeRule_Add(f, priority) end

--- Add a rule to be executed at every 'interval' seconds
--- RETURNS: Void
---
---@param f function
---@param interval number
---@param priority integer
function TimeRule_AddInterval(f, interval, priority) end

--- Add a rule to be executed 'calls' times, at every 'interval' seconds
--- RETURNS: Void
---
---@param f function
---@param interval number
---@param calls integer
---@param priority integer
function TimeRule_AddIntervalEx(f, interval, calls, priority) end

--- Add a rule to be executed once, after 'interval' seconds
--- RETURNS: Void
---
---@param f function
---@param interval number
---@param priority integer
function TimeRule_AddOneShot(f, interval, priority) end

--- Change 'interval' seconds of an existing rule
--- RETURNS: Void
---
---@param f function
---@param interval number
function TimeRule_ChangeInterval(f, interval) end

--- Test if a rule is currently active
--- RETURNS: Boolean
---
---@param f function
function TimeRule_Exists(f) end

--- Remove a currently active rule
--- RETURNS: Void
---
---@param f function
function TimeRule_Remove(f) end

--- Kills all rules.
--- RETURNS: Void
---
---@param maxpriority integer
function TimeRule_RemoveAll(maxpriority) end

--- Remove a currently executing rule (only works inside a rule function)
--- RETURNS: Void
---
---@param Void any
function TimeRule_RemoveMe(Void) end

--- 
--- RETURNS: Void
---
---@param oldf function
---@param newf function
function TimeRule_Replace(oldf, newf) end

--- Add the amount of time to the specified timer
--- RETURNS: Void
---
---@param timerID integer
---@param period number
function Timer_Add(timerID, period) end

--- Advances the timer by the specified amount of time
--- RETURNS: Void
---
---@param timerID integer
---@param period number
function Timer_Advance(timerID, period) end

--- Display (in the console) the amount of time remaining in the specified timer.
--- RETURNS: Void
---
---@param timerID integer
function Timer_Display(timerID) end

--- Displays a timer on the screen - You need to call this regularly (i.e. every second) to update the onscreen display. THIS IS A TEMPORARY FUNCTION - WELL GET PROPER UI SUPPORT LATER ON
--- RETURNS: Void
---
---@param timerID integer
function Timer_DisplayOnScreen(timerID) end

--- Stop the specified timer.
--- RETURNS: Void
---
---@param timerID integer
function Timer_End(timerID) end

--- Returns whether the timer with this ID exists
--- RETURNS: Boolean
---
---@param timerID integer
function Timer_Exists(timerID) end

--- Returns how much time has elapsed since this timer has been started args Integer timerID
--- RETURNS: Real
---
---@param Void any
function Timer_GetElapsed(Void) end

--- Returns TWO values: minutes and seconds. Provide it a function like Timer_GetRemaining or Timer_GetElapsed
--- RETURNS: 
---
---@param getTimeFunction Luafunction
---@param TimerID integer
function Timer_GetMinutesAndSeconds(getTimeFunction, TimerID) end

--- Get the remaining time for the specified timer.
--- RETURNS: Real
---
---@param timerID integer
function Timer_GetRemaining(timerID) end

---
---Located in FILE: \scar\timer.scar LINE: 10
function Timer_Init() end

--- Pause the specified timer.
--- RETURNS: Void
---
---@param timerID integer
function Timer_Pause(timerID) end

--- Resume the specified timer.
--- RETURNS: Void
---
---@param timerID integer
function Timer_Resume(timerID) end

--- Start a timer with the specified id, taking the required period ( in seconds )
--- RETURNS: Void
---
---@param timerID integer
---@param period number
function Timer_Start(timerID, period) end

---
---Located in FILE: \scar\timer.scar LINE: 22
---@param id any
---@param number any
---@param bCheckExistance any
---@param caller any
function Timer_Validate(id, number, bCheckExistance, caller) end

---
---Located in FILE: \scar\designerlib.scar LINE: 1167
function Trigger_Mob() end

---
---Located in FILE: \scar\battle_simulator.scar LINE: 45
---@param path any
function TrimPath(path) end

--- Enables or disables the Resume button in the tutorial menu. Can be called at any time.
--- RETURNS: Void
---
---@param enable boolean
function TutorialMenu_EnableResumeButton(enable) end

--- Sets up a button on the tutorial menu.
--- RETURNS: Void
---
---@param index integer
---@param enabled boolean
---@param text LocString
---@param tooltip LocString
---@param icon string
---@param callback function
function TutorialMenu_SetButtonInfo(index, enabled, text, tooltip, icon, callback) end

--- Displays a breif UI warning in the critical alert message area.
--- RETURNS: Void
---
---@param locText LocString
function UIWarning_Show(locText) end

--- Creates and flashes an ability button on the taskbar if the unit is selected
--- RETURNS: Void
---
---@param playerid PlayerID
---@param abilityID AbilityID
---@param text LocString
---@param length integer
---@param blueprint_filter? Table/Blueprint
function UI_AddHintAndFlashAbility(playerid, abilityID, text, length, blueprint_filter) end

--- If set to true, the button always flashes. If set to false, the default flashing behavior resumes.
--- RETURNS: Void
---
---@param alwaysFlash boolean
---@param stopWhenClicked boolean
function UI_AlwaysFlashCompanyCommanderButton(alwaysFlash, stopWhenClicked) end

--- Enables or disables a named segment of the taskbar.
--- RETURNS: Void
---
---@param bindingName string
---@param enabled boolean
function UI_BindingSetEnabled(bindingName, enabled) end

--- Clears the building placement callback.
--- RETURNS: Void
---
---@param Void any
function UI_ClearBuildingPlacementCallback(Void) end

--- Clears the construction menu navigation callback.
--- RETURNS: Void
---
---@param Void any
function UI_ClearConstructionMenuCallback(Void) end

--- Clears all active event cues
--- RETURNS: Void
---
---@param Void any
function UI_ClearEventCues(Void) end

--- Clears the override on an option. See LuaConsts.scar for list of available options.
--- RETURNS: Void
---
---@param name string
function UI_ClearOptionOverride(name) end

--- Clears the submenu navigation callback.
--- RETURNS: Void
---
---@param Void any
function UI_ClearSubmenuNavigationCallback(Void) end

--- Toggle off cover preview. Each call to UI_CoverPreviewHide must be matched by a call to UI_CoverPreviewShow
--- RETURNS: Void
---
---@param Void any
function UI_CoverPreviewHide(Void) end

--- Toggle on cover preview. Each call to UI_CoverPreviewShow must be matched by a call to UI_CoverPreviewHide
--- RETURNS: Void
---
---@param Void any
function UI_CoverPreviewShow(Void) end

--- Create a custom kicker message on the entity and display to the player
--- RETURNS: Void
---
---@param player PlayerID
---@param entity EntityID
---@param message LocString
function UI_CreateEntityKickerMessage(player, entity, message) end

--- Creates an event cue without a callback (you won't know when it's clicked)
--- RETURNS: ID
---
---@param iconPath string
---@param soundPath string
---@param title LocString
---@param description[ LocString
---@param lifetime number
---@param dismissOnClick] boolean
function UI_CreateEventCue(iconPath, soundPath, title, description[, lifetime, dismissOnClick]) end

--- Creates a event cue with custom lua callback function when icon is clicked. Lifetime of -1 means use the default. The return value is an event cue ID. The callback receives one parameter: the ID of the event cue that was clicked.
--- RETURNS: Integer
---
---@param iconPath string
---@param soundPath string
---@param title LocString
---@param description LocString
---@param f function
---@param lifetime number
---@param dismissOnClick boolean
function UI_CreateEventCueClickable(iconPath, soundPath, title, description, f, lifetime, dismissOnClick) end

--- Creates a blip on the minimap; return the ID of the blip.
--- RETURNS: blipID
---
---@param where StackVar
---@param lifetime number
---@param blipType integer
function UI_CreateMinimapBlip(where, lifetime, blipType) end

--- Create a custom kicker message on the squad and display to the player.
--- RETURNS: Void
---
---@param player PlayerID
---@param sgroup SGroup
---@param message LocString
function UI_CreateSGroupKickerMessage(player, sgroup, message) end

--- Create a custom kicker message on the squad and display to the player
--- RETURNS: Void
---
---@param player PlayerID
---@param squad SquadID
---@param message LocString
function UI_CreateSquadKickerMessage(player, squad, message) end

--- Removes a blip already created on the minimap
--- RETURNS: Void
---
---@param blipID integer
function UI_DeleteMinimapBlip(blipID) end

--- Enable or disable event cues of a game event type to be displayed in the UI
--- RETURNS: Void
---
---@param gameEventType integer
---@param enable boolean
function UI_EnableEventCueType(gameEventType, enable) end

--- Enables or disables resource kickers. Possible resource types are RT_Manpower, RT_Fuel, RT_Munition, RT_Action, RT_Command.
--- RETURNS: Void
---
---@param resourceType integer
---@param enable boolean
function UI_EnableResourceTypeKicker(resourceType, enable) end

--- Turn on or off squad decorator. The default is decorator enabled
--- RETURNS: Void
---
---@param squad SquadID
---@param enabled boolean
function UI_EnableSquadDecorator(squad, enabled) end

--- Turn on or off squad minimap indicator. The default is decorator enabled
--- RETURNS: Void
---
---@param squad SquadID
---@param enabled boolean
function UI_EnableSquadMinimapIndicator(squad, enabled) end

--- Begins flashing an ability widget.
--- RETURNS: Integer
---
---@param abilityBagID integer
---@param bStopOnClick boolean
---@param blipType integer
function UI_FlashAbilityButton(abilityBagID, bStopOnClick, blipType) end

--- Begins flashing of a binding widget.
--- RETURNS: Integer
---
---@param name string
---@param bStopOnClick boolean
---@param blipType integer
function UI_FlashBinding(name, bStopOnClick, blipType) end

--- Flashes a command button associated with a particular squad command
--- RETURNS: Integer
---
---@param command integer
---@param bStopOnClick boolean
---@param blipType integer
function UI_FlashCommandButton(command, bStopOnClick, blipType) end

--- Flashes one of the 6 upgrades on the commander branch
--- RETURNS: Integer
---
---@param upgradeID integer
---@param blipType integer
function UI_FlashCommanderBranchUpgrade(upgradeID, blipType) end

--- Flashes one of the 3 upgrades on the main commander tree
--- RETURNS: Integer
---
---@param upgradeID integer
---@param blipType integer
function UI_FlashCommanderTreeUpgrade(upgradeID, blipType) end

--- Flashes a construction button associated to a building EBP.
--- RETURNS: Integer
---
---@param entityBlueprintID integer
---@param bStopOnClick boolean
---@param blipType integer
function UI_FlashConstructionButton(entityBlueprintID, bStopOnClick, blipType) end

--- Flashes a construction menu.
--- RETURNS: Integer
---
---@param menuName string
---@param bStopOnClick boolean
---@param blipType integer
function UI_FlashConstructionMenu(menuName, bStopOnClick, blipType) end

--- Flashes the entity using attributes from [tuning][ui]
--- RETURNS: Void
---
---@param entity EntityID
---@param actionOnName string
function UI_FlashEntity(entity, actionOnName) end

--- Flashes an event cue icon
--- RETURNS: Integer
---
---@param eventCueID integer
---@param bStopOnClick boolean
---@param blipType integer
function UI_FlashEventCue(eventCueID, bStopOnClick, blipType) end

--- Flashes an objective counter
--- RETURNS: Integer
---
---@param objectiveID integer
---@param blipType integer
function UI_FlashObjectiveCounter(objectiveID, blipType) end

--- Flashes an objective icon
--- RETURNS: Integer
---
---@param objectiveID integer
---@param bStopOnClick boolean
---@param blipType integer
function UI_FlashObjectiveIcon(objectiveID, bStopOnClick, blipType) end

--- Begins flashing a production widget.
--- RETURNS: Integer
---
---@param pType integer
---@param pbgID integer
---@param bStopOnClick boolean
---@param blipType integer
function UI_FlashProductionButton(pType, pbgID, bStopOnClick, blipType) end

--- Flashes a production queue item, from 0 to n. Be careful - the icon will continue to flash regardless of what's in that slot...
--- RETURNS: Integer
---
---@param index integer
---@param bStopOnClick boolean
---@param blipType integer
function UI_FlashProductionQueueIcon(index, bStopOnClick, blipType) end

--- Flashes a squad's badge in the selection area, using the squad as the lookup
--- RETURNS: Integer
---
---@param squad SquadID
---@param bStopOnClick boolean
---@param blipType integer
function UI_FlashSquadBadge(squad, bStopOnClick, blipType) end

--- Flashes a squad's badge in the selection area, using the display index (from 0 to 9) as the lookup
--- RETURNS: Integer
---
---@param displayIndex integer
---@param bStopOnClick boolean
---@param blipType integer
function UI_FlashSquadBadgeByIndex(displayIndex, bStopOnClick, blipType) end

--- Flashes a squad control group icon. You can use Misc_GetSquadControlGroup to find out which group to flash.
--- RETURNS: Integer
---
---@param controlGroupIndex integer
---@param bStopOnClick boolean
---@param blipType integer
function UI_FlashSquadControlGroupIcon(controlGroupIndex, bStopOnClick, blipType) end

--- Flashes a widget on the tactical map
--- RETURNS: Integer
---
---@param widgetName string
---@param blipType integer
function UI_FlashTacticalMapWidget(widgetName, blipType) end

--- Picks a commander tree for the player. If he already picked one, does nothing. upgradeID is the upgrade that unlocks the tree.
--- RETURNS: Void
---
---@param player PlayerID
---@param upgradeID integer
function UI_ForceCommanderTreeChoice(player, upgradeID) end

--- Hides/unhides the squad control group UI on the right side of the screen
--- RETURNS: Void
---
---@param hide boolean
function UI_HideSquadControlGroupBindings(hide) end

--- Hides the tactical map
--- RETURNS: Void
---
---@param Void any
function UI_HideTacticalMap(Void) end

--- Removes a team weapon "cone of fire" UI. You got the ID from UI_ShowTeamWeaponConeOfFire
--- RETURNS: Void
---
---@param id integer
function UI_HideTeamWeaponConeOfFire(id) end

--- Highlights an SGroup in the UI for the given duration
--- RETURNS: Void
---
---@param sgroup SGroupID
---@param duration number
function UI_HighlightSGroup(sgroup, duration) end

--- Turn on squad highlight UI feature for the duration
--- RETURNS: Void
---
---@param squad SquadID
---@param duration number
function UI_HighlightSquad(squad, duration) end

--- Returns whether a branch of the commander tree is up (once you've already selected an upgrade path)
--- RETURNS: Boolean
---
---@param Void any
function UI_IsCommanderTreeBranchShown(Void) end

--- Returns whether the main commander tree is up (where you select your upgrade path)
--- RETURNS: Boolean
---
---@param Void any
function UI_IsCommanderTreeShown(Void) end

--- Returns whether the tactical map is being shown
--- RETURNS: Boolean
---
---@param Void any
function UI_IsTacticalMapShown(Void) end

--- If the message box is activated, close it.  The callback will receive the button parameter given.
--- RETURNS: Void
---
---@param btn integer
function UI_MessageBoxHide(btn) end

--- Set the text/tooltip/enabled state of a button on the dialog.
--- RETURNS: Void
---
---@param btn integer
---@param text LocString
---@param tooltip LocString
---@param bEnabled boolean
function UI_MessageBoxSetButton(btn, text, tooltip, bEnabled) end

--- Set the title and message body of the dialog.
--- RETURNS: Void
---
---@param title LocString
---@param msg LocString
function UI_MessageBoxSetText(title, msg) end

--- Show the message box.  The callback will be called when deactivated, with the button that was pressed.
--- RETURNS: Void
---
---@param f function
function UI_MessageBoxShow(f) end

--- Only allow buildings to be placed inside a marker
--- RETURNS: Void
---
---@param mkr MarkerID
function UI_RestrictBuildingPlacement(mkr) end

--- Fades the screen to a given RGBA colour over a number of seconds. mouse input is blocked during the fade, and you can control whether the input keeps being blocked after the fade has ended (be careful!)
--- RETURNS: Void
---
---@param r number
---@param g number
---@param b number
---@param a number
---@param duration number
---@param bKeepBlockingInput boolean
function UI_ScreenFade(r, g, b, a, duration, bKeepBlockingInput) end

--- Sets the game to allow allied squads to be selected at the same time as your own squads
--- RETURNS: Void
---
---@param allow boolean
function UI_SetAlliedBandBoxSelection(allow) end

--- Sets a function to be called when the player clicks a building on the taskbar and gets a selection UI. Your function will have 2 arguments: [Integer] building EBP, [integer] phase: CP_Selected, CP_StartedPlacing (only applicable to fields, fences and buildings that let you set facing), or CP_FinishedPlacing 
--- RETURNS: Void
---
---@param f function
function UI_SetBuildingPlacementCallback(f) end

--- Sets a function to be called when the player clicks a construction menu on the taskbar. Your function will have 2 arguments: [String] menu name, [Boolean] whether this menu is being activated (true) or deactivated (false)
--- RETURNS: Void
---
---@param f function
function UI_SetConstructionMenuCallback(f) end

--- Overrides an option. See LuaConsts.scar for list of available options.
--- RETURNS: Void
---
---@param name string
---@param value integer
function UI_SetOptionOverride(name, value) end

--- Sets a function to be called when the player clicks a submenu on the taskbar. Your function will have 2 arguments: [String] menu name, [Boolean] whether this menu is being activated (true) or deactivated (false)
--- RETURNS: Void
---
---@param f function
function UI_SetSubmenuNavigationCallback(f) end

--- Sets the alpha value of the subtitle background. Value from 0 to 1
--- RETURNS: Void
---
---@param alpha number
function UI_SetSubtitleBackgroundAlpha(alpha) end

--- 
--- RETURNS: Void
---
---@param show boolean
function UI_ShowMinimapBlip(show) end

--- Shows the tactical map
--- RETURNS: Void
---
---@param Void any
function UI_ShowTacticalMap(Void) end

--- Shows a team weapon's "cone of fire" UI. The position and orientation is not updated, so don't let this stay on screen for too long. Returns an ID for use with UI_HideTeamWeaponConeOfFire.
--- RETURNS: Integer
---
---@param squad SquadID
function UI_ShowTeamWeaponConeOfFire(squad) end

--- Stops a flash handler of a given id.
--- RETURNS: Void
---
---@param updaterID integer
function UI_StopFlashing(updaterID) end

--- Stops flashing one of the 6 commander branch upgrades
--- RETURNS: Void
---
---@param flashID integer
function UI_StopFlashingCommanderBranchUpgrade(flashID) end

--- Stops flashing one of the 3 commander tree upgrades
--- RETURNS: Void
---
---@param flashID integer
function UI_StopFlashingCommanderTreeUpgrade(flashID) end

--- Stops flashing a widget on the tactical map
--- RETURNS: Void
---
---@param id integer
function UI_StopFlashingTacticalMapWidget(id) end

--- Toggle off territory lines. Each call to UI_TerritoryHide must be matched by a call to UI_TerritoryShow
--- RETURNS: Void
---
---@param Void any
function UI_TerritoryHide(Void) end

--- Toggle on territory lines. Each call to UI_TerritoryShow must be matched by a call to UI_TerritoryHide
--- RETURNS: Void
---
---@param Void any
function UI_TerritoryShow(Void) end

--- Toggles all decorators on or off.
--- RETURNS: Void
---
---@param Void any
function UI_ToggleDecorators(Void) end

--- Removes the restriction on building placement
--- RETURNS: Void
---
---@param Void any
function UI_UnrestrictBuildingPlacement(Void) end

---get unit number of a group
---
---Located in FILE: \scar\battle_simulator.scar LINE: 60
---@param group any
function UnitsOfGroup(group) end

--- Utility wrapper to setup the AI. Visit Scardoc to read the extended description.
--- RETURNS: Void
---
---@param Player playerID
---@param CapLimits table
---@param Target playerID
---@param AI_difficulty int
---@param ProdTemplate int
---@param Aggression int
---@param Preference int
---@param Counter int
function Util_AI_Setup(Player, CapLimits, Target, AI_difficulty, ProdTemplate, Aggression, Preference, Counter) end

--- If there's a squad under the mouse cursor, this adds it to a unique SGroup. Return value is the SGroup it was added to, or nil if there was no squad under the mouse cursor. Pass in true for 'clearGroup' to clear the sgroup before adding the squad.
--- RETURNS: SGroupID
---
---@param clearGroup? boolean
function Util_AddMouseoverSquadToSGroup(clearGroup) end

--- If only one team is alive, players on that team win.  Returns true if one team left.
--- RETURNS: Boolean
---
---@param win_condition string
function Util_CheckOneTeamLeft(win_condition) end

--- Clears any vehicle wrecks from a given area
--- RETURNS: Void
---
---@param position[ MarkerID/Pos/SectorID
---@param range] number
function Util_ClearWrecksFromMarker(position[, range]) end

--- Creates a given number of entities at a location and adds them to an egroup. A PlayerID of nil will create the entities as world objects.
--- RETURNS: Void
---
---@param player PlayerID
---@param egroup EGroupID
---@param blueprintID integer
---@param location MarkerID/Pos
---@param numentities integer
function Util_CreateEntitiesAtMarker(player, egroup, blueprintID, location, numentities) end

---
---Located in FILE: \scar\deprecated.scar LINE: 68
---@param playerid any
---@param egroupid any
---@param blueprint any
---@param pos any
---@param numentities any
function Util_CreateEntitiesAtPosition(playerid, egroupid, blueprint, pos, numentities) end

--- Creates a given number of squads and instant load them in to a hold and adds them to an sgroup. You can optionally also specify whether to overload the buiding or not & the squad loadout (default setting sets to NOT overload & uses normal loadout )
--- RETURNS: Void
---
---@param playerid PlayerID
---@param sgroup SGroupID
---@param blueprintID integer
---@param hold EGroupID/SGroupID
---@param numsquads[ integer
---@param overload[ boolean
---@param loadout]] integer
function Util_CreateSquadsAndGarrison(playerid, sgroup, blueprintID, hold, numsquads[, overload[, loadout]]) end

--- Creates a given number of squads and instant load them in to a hold and adds them to an sgroup. It then exit them from the hold and move to a position. You can optionally also specify whether to overload the buiding or not & the squad loadout (default setting sets to NOT overload & uses normal loadout )
--- RETURNS: SGroupID
---
---@param playerid PlayerID
---@param sgroup SGroupID
---@param blueprintID integer
---@param hold_egroupid EGroupID
---@param numsquads integer
---@param exitpos[ MarkerID/Pos
---@param overload[ boolean
---@param loadout]] integer
function Util_CreateSquadsAndGarrisonExit(playerid, sgroup, blueprintID, hold_egroupid, numsquads, exitpos[, overload[, loadout]]) end

--- Creates a given number of squads at a location and adds them to an sgroup.  You can optionally also specify the loadout (it will default to the min loadout if left off)
--- RETURNS: Void
---
---@param playerid PlayerID
---@param sgroup SGroupID
---@param blueprintID integer
---@param location MarkerID/Pos
---@param numsquads[ integer
---@param loadout] integer
function Util_CreateSquadsAtMarker(playerid, sgroup, blueprintID, location, numsquads[, loadout]) end

---
---Located in FILE: \scar\deprecated.scar LINE: 118
---@param playerId any
---@param squadgroupName any
---@param squadBlueprint any
---@param markerName any
---@param numsquads any
---@param loadout any
function Util_CreateSquadsAtMarkerEx(playerId, squadgroupName, squadBlueprint, markerName, numsquads, loadout) end

--- Creates a given number of squads at a marker position (facing another marker) and adds them to an sgroup.  You can optionally also specify the loadout (it will default to the min loadout if left off)
--- RETURNS: Void
---
---@param playerid PlayerID
---@param sgroup SGroupID
---@param blueprintID integer
---@param marker MarkerID/Pos
---@param facingmarker MarkerID/Pos
---@param numsquads[ integer
---@param loadout] integer
function Util_CreateSquadsAtMarkerFacing(playerid, sgroup, blueprintID, marker, facingmarker, numsquads[, loadout]) end

--- ReSpawns or DeSpawns sgroups (and egroups) for all players or the indicated player.
--- RETURNS: Void
---
---@param despawn boolean
---@param playerNum] Boolean!allPlayers![or!Int
---@param egroups boolean
function Util_DespawnAll(despawn, playerNum], egroups) end

--- Takes in a table and chooses the right variable for the difficulty setting. 1-4 elements.
--- RETURNS: Variable
---
---@param difficultyVariables table
function Util_DifVar(difficultyVariables) end

--- Checks the entity count for the world and returns true or false depending on the result. A specific value can be passed in to override the default amount.
--- RETURNS: Boolean
---
---@param entityLimit? integer
function Util_EntityLimit(entityLimit) end

--- Try to garrison a loadable building within radius that is closer to the first squad of the sgroup than enemy
--- RETURNS: EntityID
---
---@param sgroupid SGroupID
---@param radius integer
function Util_FallBackToGarrisonBuilding(sgroupid, radius) end

--- Returns the ability ID, given its name (full path name to the ability bag).
--- RETURNS: Int
---
---@param blueprint_name string
function Util_GetAbilityID(blueprint_name) end

--- Returns the camouflage stance ID, given its name (full path name to the stance bag).
--- RETURNS: Int
---
---@param blueprint_name string
function Util_GetCamouflageStanceID(blueprint_name) end

--- Returns the closest MarkerID to the entity/marker/pos/egroup/sgroup/squad from the table of markers provided
--- RETURNS: MarkerID
---
---@param var Variable
---@param markers table
function Util_GetClosestMarker(var, markers) end

--- Returns the critical bag ID, given its name (full path name to the critical bag).
--- RETURNS: Int
---
---@param blueprint_name string
function Util_GetCriticalID(blueprint_name) end

--- Returns the damage bag ID, given its name (full path name to the critical bag).
--- RETURNS: Int
---
---@param blueprint_name string
function Util_GetDamageID(blueprint_name) end

---
---Located in FILE: \scar\deprecated.scar LINE: 795
function Util_GetDifficulty() end

--- Find all the entities with a given blueprint in sourcegroup and add them to destgroup.
--- RETURNS: Void
---
---@param sourcegroup EGroupID
---@param destgroup EGroupID
---@param bpID integer
function Util_GetEntitiesByBP(sourcegroup, destgroup, bpID) end

--- Returns the ebp ID, given its name (full path name to the entity blueprint).
--- RETURNS: Int
---
---@param blueprint_name string
function Util_GetEntityBlueprintID(blueprint_name) end

--- Returns a unique SGroup used to hold mouseover squads obtained from Util_AddMouseoverSquadToSGroup
--- RETURNS: Void
---
---@param Void any
function Util_GetMouseoverSGroup(Void) end

--- Returns a position from entity/marker/pos/egroup/sgroup/squad
--- RETURNS: Position
---
---@param var Variable
function Util_GetPosition(var) end

--- Returns a position that is distance metres from point A, headed in the direction of point B.
--- RETURNS: Position
---
---@param a MarkerID/Pos
---@param b MarkerID/Pos
---@param distance number
function Util_GetPositionFromAtoB(a, b, distance) end

--- but for a position value
--- returns a random offset, same as Marker_GetRandomPosition
---
---Located in FILE: \scar\missionname.scar LINE: 403
---@param pos any
---@param dist any
function Util_GetRandOffset(pos, dist) end

--- returns a random SBP
--- when I don't feel like making up my mind
---
---Located in FILE: \scar\missionname.scar LINE: 424
function Util_GetRandSBP_AxisArt() end

--- returns a random SBP
--- when I don't feel like making up my mind
---
---Located in FILE: \scar\missionname.scar LINE: 416
function Util_GetRandSBP_AxisInf() end

--- Returns the slot item ID, given its name (full path name to the slot item bag).
--- RETURNS: Int
---
---@param blueprint_name string
function Util_GetSlotItemID(blueprint_name) end

--- Returns the sbp ID, given its name (full path name to the squad blueprint).
--- RETURNS: Int
---
---@param blueprint_name string
function Util_GetSquadBlueprintID(blueprint_name) end

--- Find all the squads with a given armour type in sourcegroup and add them to destgroup.
--- RETURNS: Void
---
---@param sourcegroup SGroupID
---@param destgroup SGroupID
---@param armourtype string
function Util_GetSquadsByArmourType(sourcegroup, destgroup, armourtype) end

--- Find all the squads with a given blueprint in sourcegroup and add them to destgroup.
--- RETURNS: Void
---
---@param sourcegroup SGroupID
---@param destgroup SGroupID
---@param bpID integer
function Util_GetSquadsByBP(sourcegroup, destgroup, bpID) end

--- Returns trailing numbers from a string, if it exists, nil otherwise. E.G. "marker23" would return 23.
--- RETURNS: Number
---
---@param val string
function Util_GetTrailingNumber(val) end

--- Returns the upgrade ID, given its name (full path name to the upgrade bag).
--- RETURNS: Int
---
---@param blueprint_name string
function Util_GetUpgradeID(blueprint_name) end

--- Hides all of a player's squads and/or buildings
--- RETURNS: Void
---
---@param player PlayerID
---@param hide boolean
function Util_HidePlayerForNIS(player, hide) end

--- Function takes multiple strings and returns a BlueprintTable ( used for SGroup_ContainsBlueprints )
--- RETURNS: BlueprintTable
---
---@param args Args
function Util_MakeBlueprintTable(args) end

--- Play an events file at a given markers location
--- RETURNS: void
---
---@param markername string
---@param eventfile string
function Util_MarkerFX(markername, eventfile) end

--- Play the mission title fade.
--- RETURNS: Void
---
---@param title LocString
function Util_MissionTitle(title) end

---
---Located in FILE: \scar\deprecated.scar LINE: 238
---@param group any
---@param factor any
function Util_ModifyReceivedDamage(group, factor) end

---
---Located in FILE: \scar\deprecated.scar LINE: 158
---@param egroupid any
---@param factor any
function Util_ModifySightRadiusForEGroup(egroupid, factor) end

---
---Located in FILE: \scar\deprecated.scar LINE: 130
---@param playerid any
---@param factor any
function Util_ModifySightRadiusForPlayer(playerid, factor) end

---
---Located in FILE: \scar\deprecated.scar LINE: 146
---@param sgroupid any
---@param factor any
function Util_ModifySightRadiusForSGroup(sgroupid, factor) end

---
---Located in FILE: \scar\deprecated.scar LINE: 182
---@param group any
---@param hardpoint any
---@param factor any
function Util_ModifyWeaponAccuracy(group, hardpoint, factor) end

---
---Located in FILE: \scar\deprecated.scar LINE: 210
---@param group any
---@param hardpoint any
---@param factor any
function Util_ModifyWeaponBurst(group, hardpoint, factor) end

---
---Located in FILE: \scar\deprecated.scar LINE: 196
---@param group any
---@param hardpoint any
---@param factor any
function Util_ModifyWeaponCooldown(group, hardpoint, factor) end

---
---Located in FILE: \scar\deprecated.scar LINE: 224
---@param group any
---@param hardpoint any
---@param factor any
function Util_ModifyWeaponDamage(group, hardpoint, factor) end

---
---Located in FILE: \scar\deprecated.scar LINE: 170
---@param sgroupid any
---@param hardpoint any
---@param factor any
function Util_ModifyWeaponRanges(sgroupid, hardpoint, factor) end

--- Enabling this function will mute the ambient sound (NOT all sound).
--- RETURNS: Void
---
---@param enable boolean
---@param fade? number
function Util_MuteAmbientSound(enable, fade) end

--- TODO: make sure to document how to create the table etc
---
---Located in FILE: \scar\scarutil.scar LINE: 352
---@param playerId any
---@param tableName any
---@param mkr1 any
---@param mkr2 any
function Util_NearMkrGoMkr(playerId, tableName, mkr1, mkr2) end

--- Parachutes in a given number of squads at a location and adds them to an sgroup. You can optionally also specify the loadout (it will default to the min loadout if left off) 
--- RETURNS: Void
---
---@param player PlayerID
---@param sgroup SGroupID
---@param blueprintID integer
---@param location MarkerID/Pos
---@param numsquads[ integer
---@param loadout] integer
function Util_ParachuteSquadsAtMarker(player, sgroup, blueprintID, location, numsquads[, loadout]) end

--- Plays a mission's closing NIS with appropriate fades, letterboxes, etc. Provides 1 callback: OnComplete is called after the event is over (this would be where to set the winner)
--- RETURNS: Void
---
---@param NISname string
---@param OnComplete function
function Util_PlayClosingNIS(NISname, OnComplete) end

--- Plays a mid mission NIS with appropriate fades, letterboxes, etc. Provides 2 callbacks: OnScreenFaded is called when the screen is faded to black right after the NIS (but still during the event), and OnComplete is called after the event is over.
--- RETURNS: Void
---
---@param NISname string
---@param OnScreenFaded function
---@param OnComplete function
function Util_PlayMidMissionNIS(NISname, OnScreenFaded, OnComplete) end

--- Plays music from the Data:Sound folder, and stores the music track so it can be resumed after a save/load
--- RETURNS: Void
---
---@param name string
---@param fade number
---@param delay number
function Util_PlayMusic(name, fade, delay) end

--- Plays a mission's opening NIS with appropriate fades, letterboxes, etc. Provides 2 callbacks: OnScreenFaded is called when the screen is faded to black right after the NIS (but still during the event), and OnComplete is called after the event is over.
--- RETURNS: Void
---
---@param NISname string
---@param OnScreenFaded function
---@param OnComplete function
function Util_PlayOpeningNIS(NISname, OnScreenFaded, OnComplete) end

--- Reloads the running scar script. Current running rules would also be updated to the redefined functioin.
--- RETURNS: 
---
---@param Void any
function Util_ReloadScript(Void) end

--- Resumes playing the music track that was last triggered i.e. after a save/load
--- RETURNS: Void
---
function Util_RestoreMusic() end

--- this function orders the squad to move to a location, but to take its time
---
---Located in FILE: \scar\scarutil.scar LINE: 363
---@param sgroup any
---@param finalMarker any
function Util_SGroupAmble(sgroup, finalMarker) end

--- Converts a 2D top down position to a 3D ScarPosition.
--- RETURNS: Position
---
---@param xpos number
---@param ypos number
function Util_ScarPos(xpos, ypos) end

---
---Located in FILE: \scar\deprecated.scar LINE: 799
---@param player any
---@param dif any
function Util_SetDifficulty_Health(player, dif) end

--- Trigger function immediately after the opening NIS to set starting resources for the player
--- RETURNS: Void
---
---@param missionNumber integer
function Util_SetStartingResources(missionNumber) end

--- Play an Intel Event. These are medium priority, and will interrupt a Stinger, but not an NIS.
--- RETURNS: Void
---
---@param func function
function Util_StartIntel(func) end

--- Play an NIS. These have the highest priority, and will interrupt Intel Events and Stingers.
--- RETURNS: Void
---
---@param func function
function Util_StartNIS(func) end

--- Play a Speech Stinger. These are the lowest priority, and will be bumped by Intel Events or NIS's.
--- RETURNS: Void
---
---@param func function
function Util_StartStinger(func) end

--- Library function to trigger NIS event under a certain sets of conditions.  NOTE: if checking against a marker DO NOT specify a range.  The range of the marker set in the WorldBuilder will be used.
--- RETURNS: Void
---
---@param playerid PlayerID
---@param position Marker/EGroup/SGroup/ScarPos
---@param range integer
---@param func function
---@param non_combat boolean
---@param onscreen_only boolean
---@param onscreen_duration integer
function Util_TriggerEvent(playerid, position, range, func, non_combat, onscreen_only, onscreen_duration) end

--- Returns a table containing either the total or a specific player's squad count, entity count, and vehicle count.
--- RETURNS: Lua Table
---
---@param player Boolean!world!OR!playerID
function Util_UnitCounts(player) end

--- toggle the visibility of occlusion culling OBB
--- RETURNS: Void
---
---@param Void any
function VIS_OccCullToggleOBB(Void) end

---
---Located in FILE: \scar\view.scar LINE: 74
function View_DownArrow() end

---
---Located in FILE: \scar\view.scar LINE: 133
---@param item any
---@param xpos any
---@param ypos any
---@param column any
function View_DrawItem(item, xpos, ypos, column) end

---
---Located in FILE: \scar\view.scar LINE: 108
function View_Focus() end

---
---Located in FILE: \scar\view.scar LINE: 13
function View_Init() end

---
---Located in FILE: \scar\view.scar LINE: 98
function View_LeftArrow() end

---
---Located in FILE: \scar\view.scar LINE: 125
function View_Manager() end

---
---Located in FILE: \scar\view.scar LINE: 87
function View_RightArrow() end

---
---Located in FILE: \scar\view.scar LINE: 114
function View_ToggleDisplay() end

---
---Located in FILE: \scar\view.scar LINE: 61
function View_UpArrow() end

---
---Located in FILE: \scar\wcutil.scar LINE: 99
function WC_CheckEndGame() end

--- If only one team is alive, players on that team win.  Returns true if one team left.
--- RETURNS: Boolean
---
---@param win_condition string
function WC_CheckOneTeamLeft(win_condition) end

---
---Located in FILE: \scar\wcutil.scar LINE: 183
function WC_InitGameCheck() end

---
---Located in FILE: \scar\wcutil.scar LINE: 203
function WC_OnPlayerKilled() end

---
---Located in FILE: \scar\wcutil.scar LINE: 38
function WC_RemoveUnusedObjects() end

--- Create win/lose warning.  You can use the name to reference the warning later.
--- RETURNS: Void
---
---@param name string
---@param player PlayerID
---@param icon string
---@param wWinConditionName WString
---@param wHelpTip LocString
function WinWarning_Add(name, player, icon, wWinConditionName, wHelpTip) end

--- Returns true if the named win warning exists.
--- RETURNS: Boolean
---
---@param name string
function WinWarning_Exists(name) end

--- Triggers a UI event cue and an audio cue that the player is about to lose the game.
--- RETURNS: Void
---
---@param player PlayerID
---@param warningLevel integer
function WinWarning_PublishLoseReminder(player, warningLevel) end

--- Removes an added win/lose warning
--- RETURNS: Void
---
---@param name string
function WinWarning_Remove(name) end

--- Set the text for a win warning.
--- RETURNS: Void
---
---@param name string
---@param locText LocString
function WinWarning_SetText(name, locText) end

--- Kill off a specific player's dead bodies (enter ALL to clean them all up)
--- RETURNS: Void
---
---@param player PlayerID
function World_CleanUpTheDead(player) end

--- Destroys walls (entities with a wall_ext) near a marker
--- RETURNS: Void
---
---@param marker MarkerID
function World_DestroyWallsNearMarker(marker) end

--- Get the distance between a squad group and a point.
--- RETURNS: Real
---
---@param egroup EGroupID
---@param p1 Position
---@param closest boolean
function World_DistanceEGroupToPoint(egroup, p1, closest) end

--- Get the distance between two points.
--- RETURNS: Real
---
---@param p1 Position
---@param p2 Position
function World_DistancePointToPoint(p1, p2) end

--- Get the distance between a squad group and a point.
--- RETURNS: Real
---
---@param sgroup SGroupID
---@param p1 Position
---@param closest boolean
function World_DistanceSGroupToPoint(sgroup, p1, closest) end

--- Get the distance squared between two points.
--- RETURNS: Real
---
---@param p1 Position
---@param p2 Position
function World_DistanceSquaredPointToPoint(p1, p2) end

--- Enables or disables the fog of war between these two players
--- RETURNS: Void
---
---@param p0 PlayerID
---@param p1 PlayerID
---@param enable boolean
function World_EnablePlayerToPlayerFOW(p0, p1, enable) end

--- Wins/loses a single team mission for the local teams
--- RETURNS: Void
---
---@param win boolean
function World_EndSP(win) end

--- Find and add entities near the marker to the egroup
--- RETURNS: Integer
---
---@param player PlayerID
---@param egroup EGroupID
---@param marker MarkerID
---@param ownerType integer
function World_GetEntitiesNearMarker(player, egroup, marker, ownerType) end

--- Find and add entities near the point to the egroup
--- RETURNS: Integer
---
---@param player PlayerID
---@param egroup EGroupID
---@param pos Position
---@param radius number
---@param ownerType integer
function World_GetEntitiesNearPoint(player, egroup, pos, radius, ownerType) end

--- Find and add entities within the territory sector to the egroup
--- RETURNS: Integer
---
---@param player PlayerID
---@param egroup EGroupID
---@param sectorID integer
---@param ownerType integer
function World_GetEntitiesWithinTerritorySector(player, egroup, sectorID, ownerType) end

--- Returns the EntityID at given index, use with World_GetNumEntities() to iterate through all the entities in the world
--- RETURNS: EntityID
---
---@param index integer
function World_GetEntity(index) end

--- Return the total game time in seconds.
--- RETURNS: Real
---
---@param Void any
function World_GetGameTime(Void) end

--- Find a position on a path hidden from view, as close to the destination as possible whilst still satisfying your hidden checktype. Checktype can be either CHECK_IN_FOW, CHECK_OFFCAMERA or CHECK_BOTH.
--- RETURNS: Position
---
---@param player PlayerID
---@param origin MarkerID/Pos
---@param destination MarkerID/Pos
---@param checktype integer
function World_GetHiddenPositionOnPath(player, origin, destination, checktype) end

--- Returns the total playable length of the game world (z coordinate)
--- RETURNS: Real
---
---@param Void any
function World_GetLength(Void) end

--- Find and add neutral entities near the marker to the egroup
--- RETURNS: Integer
---
---@param egroup EGroupID
---@param marker MarkerID
function World_GetNeutralEntitiesNearMarker(egroup, marker) end

--- Find and add neutral entities near the point to the egroup
--- RETURNS: Integer
---
---@param egroup EGroupID
---@param pos Position
---@param radius number
function World_GetNeutralEntitiesNearPoint(egroup, pos, radius) end

--- Find and add neutral entities within the territory sector to the egroup
--- RETURNS: Integer
---
---@param egroup EGroupID
---@param sectorID integer
function World_GetNeutralEntitiesWithinTerritorySector(egroup, sectorID) end

--- Returns the number of spawned entities in the entire world (use sparingly and never at runtime)
--- RETURNS: Integer
---
---@param Void any
function World_GetNumEntities(Void) end

--- Return the number of entities of the same ebpid in the sphere volume
--- RETURNS: Integer
---
---@param ebpid integer
---@param pos Position
---@param radius integer
function World_GetNumEntitiesNearPoint(ebpid, pos, radius) end

--- Returns the number of strategic points on this map (does not count strat. objectives)
--- RETURNS: Integer
---
---@param Void any
function World_GetNumStrategicPoints(Void) end

--- Returns the number of strategic objectives on this map
--- RETURNS: Integer
---
---@param Void any
function World_GetNumVictoryPoints(Void) end

--- Return a PBG for use by many functions (should only be used at load time)
--- RETURNS: PropertyBagGroup
---
---@param pbgname string
function World_GetPBG(pbgname) end

--- - Returns the player at a given index, numbers start at one
--- RETURNS: PlayerID
---
---@param index integer
function World_GetPlayerAt(index) end

--- Return the total number of players in the world
--- RETURNS: Integer
---
---@param Void any
function World_GetPlayerCount(Void) end

--- - Returns the player index given the Player*
--- RETURNS: Integer
---
---@param player PlayerID
function World_GetPlayerIndex(player) end

--- Returns the number of types of buildings a race can build
--- RETURNS: Integer
---
---@param race_index integer
function World_GetPossibleBuildingsCount(race_index) end

--- Returns the name of a chosen building for a race
--- RETURNS: String
---
---@param race_index integer
---@param building_index integer
function World_GetPossibleBuildingsName(race_index, building_index) end

--- Returns the PBG ID of a chosen building for a race
--- RETURNS: Integer
---
---@param race_index integer
---@param building_index integer
function World_GetPossibleBuildingsPBGID(race_index, building_index) end

--- Returns the number of types of squads a race can build
--- RETURNS: Integer
---
---@param race_index integer
function World_GetPossibleSquadsCount(race_index) end

--- Returns the blueprint name of a chosen squad for a race
--- RETURNS: String
---
---@param race_index integer
---@param squad_index integer
function World_GetPossibleSquadsName(race_index, squad_index) end

--- Return the number of property bag groups of the same type
--- RETURNS: Integer
---
---@param type integer
function World_GetPropertyBagGroupCount(type) end

--- Returns the property bag group ID, given its name (full path name to the property bag).
--- RETURNS: Integer
---
---@param groupname string
function World_GetPropertyBagGroupID(groupname) end

--- Return the path name of the group
--- RETURNS: String
---
---@param pbg PropertyBagGroup
function World_GetPropertyBagGroupName(pbg) end

--- Return the path name of the group
--- RETURNS: String
---
---@param type integer
---@param id integer
function World_GetPropertyBagGroupPathName(type, id) end

--- Returns the race index of a race, given its name (from the ME).
--- RETURNS: Integer
---
---@param racename string
function World_GetRaceIndex(racename) end

--- Returns a random integer with range [min, max]
--- RETURNS: Integer
---
---@param min integer
---@param max integer
function World_GetRand(min, max) end

--- Given any position in the world, this function will return position safe for spawning a given entity
--- RETURNS: Position
---
---@param around Position
---@param entity EntityID
function World_GetSpawnablePosition(around, entity) end

--- Find and add squads near the marker to the sgroup
--- RETURNS: Integer
---
---@param player PlayerID
---@param sgroup SGroupID
---@param marker MarkerID
---@param ownerType integer
function World_GetSquadsNearMarker(player, sgroup, marker, ownerType) end

--- Find and add squads near the point to the sgroup
--- RETURNS: Integer
---
---@param player PlayerID
---@param sgroup SGroupID
---@param pos Position
---@param radius number
---@param ownerType integer
function World_GetSquadsNearPoint(player, sgroup, pos, radius, ownerType) end

--- Find and add squads within territory sector identified by sector ID
--- RETURNS: Integer
---
---@param player PlayerID
---@param sgroup SGroupID
---@param sectorID integer
---@param ownerType integer
function World_GetSquadsWithinTerritorySector(player, sgroup, sectorID, ownerType) end

--- Returns the percentage (0-1) of supplied territory this team has
--- RETURNS: Real
---
---@param team_index integer
function World_GetTeamSuppliedTerritoryPercentage(team_index) end

--- Returns the area in square meters of supplied territory this team has
--- RETURNS: Real
---
---@param team_index integer
function World_GetTeamsSuppliedTerritoryArea(team_index) end

--- Return the sector ID from the position
--- RETURNS: Integer
---
---@param pos Position
function World_GetTerritorySectorID(pos) end

--- Returns the total playable width of the game world (x coordinate)
--- RETURNS: Real
---
---@param Void any
function World_GetWidth(Void) end

--- - to document
--- RETURNS: Boolean
---
---@param Void any
function World_IsGameOver(Void) end

--- Returns true if position is in-supply for the given player
--- RETURNS: Boolean
---
---@param ModPlayer* player const
---@param pos Position
function World_IsInSupply(ModPlayer* player, pos) end

--- Returns true if position if part of the entity territory
--- RETURNS: Boolean
---
---@param player PlayerID
---@param position Position
function World_IsPointInPlayerTerritory(player, position) end

--- Returns true if this propertybaggroup is valid, given its name (full path name to the property bag).
--- RETURNS: Boolean
---
---@param groupname string
function World_IsPropertyBagGroupValid(groupname) end

--- Find if player has ownership to this territory sector.
--- RETURNS: Boolean
---
---@param player PlayerID
---@param sectorID integer
function World_IsTerritorySectorOwnedByPlayer(player, sectorID) end

--- Kills ALL world entities near a marker
--- RETURNS: Void
---
---@param marker MarkerID
function World_KillAllNeutralEntitesNearMarker(marker) end

--- Returns true if the squad is owned by the world
--- RETURNS: Boolean
---
---@param entity EntityID
function World_OwnsEntity(entity) end

--- Returns true if the squad is owned by the world
--- RETURNS: Boolean
---
---@param squad SquadID
function World_OwnsSquad(squad) end

--- Returns true if two world positions are in proximity to each other
--- RETURNS: Boolean
---
---@param p1 Math::Vector3f
---@param p2 Math::Vector3f
---@param prox number
function World_PointPointProx(p1, p2, prox) end

--- Creates a new Position object.
--- RETURNS: Position
---
---@param x number
---@param y number
---@param z number
function World_Pos(x, y, z) end

--- Removes all resource and strategic points but not entities with a supply ext NOTE: this could eventually be written as - RemoveAllOfType( type ) which would be more flexible.
--- RETURNS: Void
---
---@param Void any
function World_RemoveAllResourcePoints(Void) end

--- Sets a particular sector in the world as a supply provider (or not)
--- RETURNS: Void
---
---@param point Math::Vector3f
---@param bSupply boolean
function World_SetDesignerSupply(point, bSupply) end

--- - to document
--- RETURNS: Void
---
---@param Void any
function World_SetGameOver(Void) end

--- Sets the player and all the members allied to player to a win state, with a reason for winning.   Sets the player to a lose state, with a reason for losing, and kills the player
--- RETURNS: Void
---
---@param player PlayerID
---@param identifier string
function World_SetPlayerLose(player, identifier) end

--- Also sets all other players to a lose state, and kills them (if they're not already dead)
--- RETURNS: Void
---
---@param player PlayerID
---@param identifier string
function World_SetTeamWin(player, identifier) end

--- Returns true if the two points are in the same territory region, and owned by the specified territory team, Returns false otherwise.
--- RETURNS: Boolean
---
---@param team_index integer
---@param p1 Math::Vector3f
---@param p2 Math::Vector3f
function World_TeamTerritoryPointsConnected(team_index, p1, p2) end

--- Rotates around point slghtly offset from squad
---
---Located in FILE: \scar\screensaver.scar LINE: 1205
---@param sgroup any
function cam_AutoRotate_DefaultCam(sgroup) end

--- Follows an sgroup from a close distance at a low declination while facing the front of the squad
---
---Located in FILE: \scar\screensaver.scar LINE: 954
---@param sgroup any
function cam_FollowMovingSGroup_LookAtFront(sgroup) end

--- Follows an sgroup by its position
---
---Located in FILE: \scar\screensaver.scar LINE: 1433
function cam_FollowPos() end

--- Follows an sgroup from on offset position
---
---Located in FILE: \scar\screensaver.scar LINE: 1011
function cam_FollowPos_offset() end

--- Default camera! Follows sgroup
---
---Located in FILE: \scar\screensaver.scar LINE: 1105
---@param sgroup any
function cam_FollowSGroup_DefaultCam_RandomAngle(sgroup) end

--- Follows sgroup from angle facing the squad as it moves, from a medium zoom distance
---
---Located in FILE: \scar\screensaver.scar LINE: 1179
---@param sgroup any
function cam_FollowSGroup_MedZoom_LookAtSquad(sgroup) end

--- Follows an sgroup from a close distance at a low declination with a random angle
---
---Located in FILE: \scar\screensaver.scar LINE: 980
---@param sgroup any
function cam_FollowSGroup_RandomAngle(sgroup) end

--- Follows an sgroup from a long distance with a telephoto FOV
---
---Located in FILE: \scar\screensaver.scar LINE: 997
---@param sgroup any
function cam_FollowSGroup_RandomAngle_Telephoto(sgroup) end

--- Follows sgroup from almost top-down and looks in the direction they're moving
---
---Located in FILE: \scar\screensaver.scar LINE: 1119
---@param sgroup any
function cam_FollowSGroup_TopDown_LookForward(sgroup) end

--- Move camera in close to the attacker while he's firing
---
---Located in FILE: \scar\screensaver.scar LINE: 918
---@param sgroup any
function cam_LookAtAttacker(sgroup) end

--- This function CAN continuously track the target if running on an interval, but the SCAR update rate of 8 ticks / second makes this stutter noticeably
--- Move the camera down near the attacker SGroup and looks at the target sgroup
---
---Located in FILE: \scar\screensaver.scar LINE: 870
---@param sgroup any
function cam_LookAtTarget(sgroup) end

--- To add later: watch for squad to no longer be on screen
--- Sets camera to static position in front of moving squad so it moves past the camera
---
---Located in FILE: \scar\screensaver.scar LINE: 1049
---@param sgroup any
function cam_MovePastCam(sgroup) end

--- Static camera from an almost top-down view; oriented 90 degrees to the squad's movement so it walks across the frame 
---
---Located in FILE: \scar\screensaver.scar LINE: 1144
---@param sgroup any
function cam_MovePastCam_TopDown(sgroup) end

--- Pans camera from a top-down angle from the sgroup to an enemy HQ
---
---Located in FILE: \scar\screensaver.scar LINE: 1241
---@param sgroup any
function cam_PanAcrossMap(sgroup) end

--- delay camera move slightly to give the camera time to go to default cam
---
---Located in FILE: \scar\screensaver.scar LINE: 1299
function cam_PanAcrossMap_Delay() end

--- sets all tuning values back to defaults
---
---Located in FILE: \scar\screensaver.scar LINE: 851
function cam_SetDefaults() end

--- sets tuning values to extreme values for fast transitions
---
---Located in FILE: \scar\screensaver.scar LINE: 860
function cam_SetFastTransitions() end

--- static shot of stationary squad, from an offset near the squad, looking towards it
---
---Located in FILE: \scar\screensaver.scar LINE: 1081
---@param sgroup any
function cam_StaticShot_Offset(sgroup) end

--- abandoned because it doesn't seem feasible with any of our current SCAR functions
--- set angle to look from center marker to other markers (first step towards testing whether it's possible to smoothly adjust the camera angle
---
---Located in FILE: \scar\screensaver.scar LINE: 1443
function cam_TestRotate() end

---
---Located in FILE: \scar\screensaver.scar LINE: 690
function cameraDefines() end

--- decides which squad to focus on and which shot to use
---
---Located in FILE: \scar\screensaver.scar LINE: 466
function chooseShot() end

--- returns value between [0-1]
---
---Located in FILE: \scar\screensaver.scar LINE: 273
function custom_random() end

--- returns an integer value between 0 and maxval
---
---Located in FILE: \scar\screensaver.scar LINE: 291
---@param maxval any
function custom_random_max(maxval) end

--- returns an integer value between the low and high values
---
---Located in FILE: \scar\screensaver.scar LINE: 286
---@param low any
---@param high any
function custom_random_range(low, high) end

--- repeatedly calls camera function for continuous shot
---
---Located in FILE: \scar\screensaver.scar LINE: 664
function doContinuousShot() end

--- calls camera function for static shot
---
---Located in FILE: \scar\screensaver.scar LINE: 656
function doStaticShot() end

--- Throws an error to lua and print out the error message
--- RETURNS: Integer
---
---@param state lua_State*
function fatal(state) end

--- Find cool stuff that's going on in the world and pick something to look at
---
---Located in FILE: \scar\screensaver.scar LINE: 324
function findAction() end

--- Find squads that are doing interesting things and add them to actionList table
---
---Located in FILE: \scar\screensaver.scar LINE: 353
---@param groupid any
---@param itemid any
---@param squadid any
function findActionSquads (groupid, itemid, squadid) end

--- returns angle in radians (set with Camera_SetOrbit)
--- finds camera angle between two points, looking from startPos towards endPos
---
---Located in FILE: \scar\screensaver.scar LINE: 194
---@param startPos any
---@param endPos any
function getAngle(startPos, endPos) end

--- returns declination in radians (set with Camera_SetDeclination)
--- finds camera declination between two points, looking from startPos towards endPos
---
---Located in FILE: \scar\screensaver.scar LINE: 203
---@param startPos any
---@param endPos any
function getDeclination(startPos, endPos) end

--- find point x meters in front of a moving squad, in the direction that it's moving
---
---Located in FILE: \scar\screensaver.scar LINE: 252
---@param sgroup any
---@param offset any
function getMoveOffsetPosition(sgroup, offset) end

--- Find point offset by x meters from start point, in direction of endpoint
---
---Located in FILE: \scar\screensaver.scar LINE: 222
---@param startPos any
---@param endPos any
---@param offset any
function getOffsetPosition(startPos, endPos, offset) end

--- If set to true extra debugging information will be displayed
--- RETURNS: Void
---
---@param bDisplay boolean
function nis_debugdisplay(bDisplay) end

--- Sets the number of seconds it takes to transition from game to nis, 0 is instantaneous
--- RETURNS: Void
---
---@param numSeconds number
function nis_setintransitiontime(numSeconds) end

--- Lets the nis system know which nis will be transitioned to when the first one ends.
--- RETURNS: Void
---
---@param filename string
function nis_setouttransitionnis(filename) end

--- Sets the number of seconds it takes to transition from nis back to game, 0 is instantaneous
--- RETURNS: Void
---
---@param numSeconds number
function nis_setouttransitiontime(numSeconds) end

--- If set to true the nis camera isn't used, when an nis is playing
--- RETURNS: Void
---
---@param bUseGameCamera boolean
function nis_usegamecamera(bUseGameCamera) end

---
---Located in FILE: \scar\ui.scar LINE: 20
function nothing() end

---[[
---
---Located in FILE: \scar\scarutil.scar LINE: 1630
function now() end

--- Returns ST_NIL,ST_BOOLEAN,ST_NUMBER,ST_STRING,ST_TABLE,ST_FUNCTION,ST_SCARPOS,ST_EGROUP,ST_ENTITY,ST_SGROUP,ST_SQUAD,ST_TEAM,ST_MARKER, ST_PBG, or ST_UNKNOWN
--- RETURNS: Integer
---
---@param v LuaBinding::StackVar
function scartype(v) end

--- Returns a string representing the scartype
--- RETURNS: String
---
---@param v LuaBinding::StackVar
function scartype_tostring(v) end

--- turn off all special modes and go back to default
---
---Located in FILE: \scar\screensaver.scar LINE: 120
function screensaverShutdown() end

---
---Located in FILE: \scar\screensaver.scar LINE: 108
function screensaverStartup() end

---
---Located in FILE: \scar\screensaver.scar LINE: 15
---@param flag any
---@param turnonAI any
function startScreensaver(flag, turnonAI) end

---
---Located in FILE: \scar\screensaver.scar LINE: 1496
function testTableRemove() end

--- track target of HMG group
---
---Located in FILE: \scar\screensaver.scar LINE: 1464
---@param sgroup any
function testTargetTracking(sgroup) end

---
---Located in FILE: \scar\view.scar LINE: 37
---@param item any
function view(item) end

