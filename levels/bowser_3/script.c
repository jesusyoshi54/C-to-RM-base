#include <ultra64.h>
#include "sm64.h"
#include "behavior_data.h"
#include "model_ids.h"
#include "seq_ids.h"
#include "dialog_ids.h"
#include "segment_symbols.h"
#include "level_commands.h"

#include "game/level_update.h"

#include "levels/scripts.h"

#include "actors/common1.h"

/* Fast64 begin persistent block [includes] */
/* Fast64 end persistent block [includes] */

#include "make_const_nonconst.h"
#include "levels/bowser_3/header.h"

/* Fast64 begin persistent block [scripts] */
/* Fast64 end persistent block [scripts] */

const LevelScript level_bowser_3_entry[] = {
	INIT_LEVEL(),
	LOAD_MIO0(0x07, _bowser_3_segment_7SegmentRomStart, _bowser_3_segment_7SegmentRomEnd), 
	LOAD_MIO0(0x06, _group12_mio0SegmentRomStart, _group12_mio0SegmentRomEnd), 
	LOAD_RAW(0x0D, _group12_geoSegmentRomStart, _group12_geoSegmentRomEnd), 
	LOAD_MIO0(0x0A, _water_skybox_mio0SegmentRomStart, _water_skybox_mio0SegmentRomEnd), 
	ALLOC_LEVEL_POOL(),
	MARIO(MODEL_MARIO, 0x00000001, bhvMario), 
	JUMP_LINK(script_func_global_13), 
	LOAD_MODEL_FROM_GEO(MODEL_BOWSER_3_FALLING_PLATFORM_1, bowser_3_geo_000290), 
	LOAD_MODEL_FROM_GEO(MODEL_BOWSER_3_FALLING_PLATFORM_2, bowser_3_geo_0002A8), 
	LOAD_MODEL_FROM_GEO(MODEL_BOWSER_3_FALLING_PLATFORM_3, bowser_3_geo_0002C0), 
	LOAD_MODEL_FROM_GEO(MODEL_BOWSER_3_FALLING_PLATFORM_4, bowser_3_geo_0002D8), 
	LOAD_MODEL_FROM_GEO(MODEL_BOWSER_3_FALLING_PLATFORM_5, bowser_3_geo_0002F0), 
	LOAD_MODEL_FROM_GEO(MODEL_BOWSER_3_FALLING_PLATFORM_6, bowser_3_geo_000308), 
	LOAD_MODEL_FROM_GEO(MODEL_BOWSER_3_FALLING_PLATFORM_7, bowser_3_geo_000320), 
	LOAD_MODEL_FROM_GEO(MODEL_BOWSER_3_FALLING_PLATFORM_8, bowser_3_geo_000338), 
	LOAD_MODEL_FROM_GEO(MODEL_BOWSER_3_FALLING_PLATFORM_9, bowser_3_geo_000350), 
	LOAD_MODEL_FROM_GEO(MODEL_BOWSER_3_FALLING_PLATFORM_10, bowser_3_geo_000368), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_03, bowser_3_geo_000380), 

	/* Fast64 begin persistent block [level commands] */
	/* Fast64 end persistent block [level commands] */

	AREA(1, bowser_3_area_1),
		WARP_NODE(WARP_NODE_ENTRANCE, LEVEL_CASTLE_GROUNDS, 1, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0x0B, LEVEL_CASTLE_GROUNDS, 1, 0x0B, WARP_NO_CHECKPOINT),
		WARP_NODE(0x0C, LEVEL_CASTLE_GROUNDS, 1, 0x0C, WARP_NO_CHECKPOINT),
		WARP_NODE(WARP_NODE_F0, LEVEL_CASTLE_GROUNDS, 1, 0x0B, WARP_NO_CHECKPOINT),
		WARP_NODE(WARP_NODE_DEATH, LEVEL_CASTLE_GROUNDS, 1, 0x0C, WARP_NO_CHECKPOINT),
		OBJECT(MODEL_NONE, 0, 150, 0, 0, 0, 0, (0xC << 16), bhvAirborneDeathWarp),
		OBJECT(MODEL_NONE, 0, 150, 0, 0, 0, 0, (0xB << 16), bhvAirborneStarCollectWarp),
		OBJECT(MODEL_NONE, 0, 150, 0, 0, 0, 0, (0xA << 16), bhvSpinAirborneWarp),
		TERRAIN(bowser_3_area_1_collision),
		MACRO_OBJECTS(bowser_3_area_1_macro_objs),
		SET_BACKGROUND_MUSIC(0x00, SEQ_LEVEL_GRASS),
		TERRAIN_TYPE(TERRAIN_GRASS),
		/* Fast64 begin persistent block [area commands] */
		/* Fast64 end persistent block [area commands] */
	END_AREA(),

	FREE_LEVEL_POOL(),
	MARIO_POS(1, 0, 0, 0, 0),
	CALL(0, lvl_init_or_update),
	CALL_LOOP(1, lvl_init_or_update),
	CLEAR_LEVEL(),
	SLEEP_BEFORE_EXIT(1),
	EXIT(),
};
