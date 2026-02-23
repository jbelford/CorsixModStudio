---@meta scar-types

-- Shared SCAR (Scripting At Relic) type definitions.
-- These opaque handle types are used by both Dawn of War and Company of Heroes.
-- Auto-generated type stubs — do not edit manually.

---Squad group handle (collection of military squads).
---Created by SGroup_Create() or SGroup_CreateIfNotFound().
---Pass the group name as a string to most SGroup_* functions.
---@class SGroupID
local SGroupID = {}

---Entity group handle (collection of buildings/objects).
---Created by EGroup_Create() or EGroup_CreateIfNotFound().
---Pass the group name as a string to most EGroup_* functions.
---@class EGroupID
local EGroupID = {}

---Individual squad handle.
---Obtained from SGroup_GetSpawnedSquadAt(), Squad_Create(), etc.
---@class SquadID
local SquadID = {}

---Individual entity handle.
---Obtained from EGroup_GetSpawnedEntityAt(), Entity_Create(), etc.
---@class EntityID
local EntityID = {}

---Player handle.
---Obtained from Setup_Player(), Player_FromId(), or Player_FromName().
---@class PlayerID
local PlayerID = {}

---World-editor marker handle.
---Obtained from Marker_FromName(name, type).
---@class MarkerID
local MarkerID = {}

---3D world coordinate.
---Created by World_Pos(x, y, z) or returned by position functions.
---The x axis is left-to-right, z is in-to-out, y is height. (0,0) is map center.
---@class Position
---@field x number
---@field y number
---@field z number
local Position = {}

---NIS actor reference table used for speech events.
---Constructed as `{ squadID = nil, blueprint = "name", icon = "path.tga" }`.
---Pre-defined actors are in the global `ACT` table (e.g. `ACT.Gabriel`).
---Use Actor_SetFromSGroup() to bind an actor to a live squad.
---@class ActorTable
---@field blueprint string Blueprint name to match in an sgroup
---@field icon string TGA icon path for speech UI display
---@field squadID SquadID|nil Bound squad (set by Actor_SetFromSGroup)
---@field squadGameID integer|nil Game ID of bound squad
local ActorTable = {}

---Ability handle.
---@class AbilityID
local AbilityID = {}

---Upgrade handle.
---@class UpgradeID
local UpgradeID = {}

---Critical hit type handle.
---@class CriticalID
local CriticalID = {}

---Property bag group handle.
---@class PropertyBagGroup
local PropertyBagGroup = {}

---Localized string handle.
---Returned by Loc_FormatText*() and Loc_ConvertNumber().
---@class LocString
local LocString = {}

---Blueprint handle.
---@class Blueprint
local Blueprint = {}

---UI button handle.
---Returned by Button_Add(), Util_AbilityButton_Create(), Util_ScarButton_Create(), etc.
---@class ButtonID
local ButtonID = {}

---Sound playlist channel (e.g. PC_Music, PC_Ambient).
---@class SoundChannel
local SoundChannel = {}

---Modifier handle created by Modifier_Create().
---Applied via Modifier_ApplyToEntity(), Modifier_ApplyToSquad(), Modifier_ApplyToPlayer().
---@class ScarModifier
local ScarModifier = {}

---Proximity type handle.
---@class ProxType
local ProxType = {}

---Minimap ping handle.
---Returned by Ping_Marker(), Ping_Position(), Objective_PingMarker(), Objective_PingPosition().
---@class PingID
local PingID = {}

---RGB colour value.
---Created by World_Color(r, g, b). Used by Fade_StartColor().
---@class Colour
---@field r integer Red component (0-255)
---@field g integer Green component (0-255)
---@field b integer Blue component (0-255)
local Colour = {}

---Blueprint table for SGroup_ContainsBlueprints() / EGroup_ContainsBlueprints().
---Created by Util_MakeBlueprintTable("bp1", "bp2", ...).
---Each entry is `{ name = "blueprint_name", count = n }`.
---@class BlueprintTable : table
local BlueprintTable = {}
