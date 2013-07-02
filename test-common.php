<?php

if(!extension_loaded('dogma')) {
	fprintf(STDERR, "Extension dogma not loaded, test cannot run.\n");
	die(50);
}

assert_options(ASSERT_ACTIVE, 1);
assert_options(ASSERT_BAIL, 1);
error_reporting(-1);

const TYPE_125mmGatlingAutoCannonII = 2873;
const TYPE_CapBooster25 = 263;
const TYPE_Drones = 3436;
const TYPE_Rifter = 587;
const TYPE_Scimitar = 11978;
const TYPE_SmallAncillaryShieldBooster = 32774;
const TYPE_SnakeOmega = 19556;
const TYPE_StasisWebifierI = 526;
const TYPE_StrongBluePillBooster = 10156;
const TYPE_WarriorI = 2486;

const ATT_CapacitorBonus = 67;
const ATT_CapacitorNeed = 6;
const ATT_DroneBandwidthUsed = 1272;
const ATT_Implantness = 331;
const ATT_MaxActiveDroneBonus = 353;
const ATT_MaxActiveDrones = 352;
const ATT_MaxLockedTargets = 192;
const ATT_SkillLevel = 280;

const EFFECT_BoosterShieldCapacityPenalty = 2737;
