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
#include "levels/bits/header.h"

/* Fast64 begin persistent block [scripts] */
/* Fast64 end persistent block [scripts] */

const LevelScript level_bits_entry[] = {
	INIT_LEVEL(),
	LOAD_MIO0(0x07, _bits_segment_7SegmentRomStart, _bits_segment_7SegmentRomEnd), 
	LOAD_MIO0(0x0A, _water_skybox_mio0SegmentRomStart, _water_skybox_mio0SegmentRomEnd), 
	LOAD_MIO0_TEXTURE(0x09, _sky_mio0SegmentRomStart, _sky_mio0SegmentRomEnd), 
	LOAD_MIO0(0x06, _group14_mio0SegmentRomStart, _group14_mio0SegmentRomEnd), 
	LOAD_RAW(0x0D, _group14_geoSegmentRomStart, _group14_geoSegmentRomEnd), 
	LOAD_MIO0(0x08, _common0_mio0SegmentRomStart, _common0_mio0SegmentRomEnd), 
	LOAD_RAW(0x0F, _common0_geoSegmentRomStart, _common0_geoSegmentRomEnd), 
	ALLOC_LEVEL_POOL(),
	MARIO(MODEL_MARIO, 0x00000001, bhvMario), 
	JUMP_LINK(script_func_global_15), 
	JUMP_LINK(script_func_global_1), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_03, bits_geo_000430), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_04, bits_geo_000448), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_05, bits_geo_000460), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_06, bits_geo_000478), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_07, bits_geo_000490), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_08, bits_geo_0004A8), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_09, bits_geo_0004C0), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0A, bits_geo_0004D8), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0B, bits_geo_0004F0), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0C, bits_geo_000508), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0D, bits_geo_000520), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0E, bits_geo_000538), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0F, bits_geo_000550), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_10, bits_geo_000568), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_11, bits_geo_000580), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_12, bits_geo_000598), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_13, bits_geo_0005B0), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_14, bits_geo_0005C8), 
	LOAD_MODEL_FROM_GEO(MODEL_BITS_SLIDING_PLATFORM, bits_geo_0005E0), 
	LOAD_MODEL_FROM_GEO(MODEL_BITS_TWIN_SLIDING_PLATFORMS, bits_geo_0005F8), 
	LOAD_MODEL_FROM_GEO(MODEL_BITS_OCTAGONAL_PLATFORM, bits_geo_000610), 
	LOAD_MODEL_FROM_GEO(MODEL_BITS_BLUE_PLATFORM, bits_geo_000628), 
	LOAD_MODEL_FROM_GEO(MODEL_BITS_FERRIS_WHEEL_AXLE, bits_geo_000640), 
	LOAD_MODEL_FROM_GEO(MODEL_BITS_ARROW_PLATFORM, bits_geo_000658), 
	LOAD_MODEL_FROM_GEO(MODEL_BITS_SEESAW_PLATFORM, bits_geo_000670), 
	LOAD_MODEL_FROM_GEO(MODEL_BITS_TILTING_W_PLATFORM, bits_geo_000688), 
	LOAD_MODEL_FROM_GEO(MODEL_BITS_STAIRCASE, bits_geo_0006A0), 
	LOAD_MODEL_FROM_GEO(MODEL_BITS_STAIRCASE_FRAME1, bits_geo_0006B8), 
	LOAD_MODEL_FROM_GEO(MODEL_BITS_STAIRCASE_FRAME2, bits_geo_0006D0), 
	LOAD_MODEL_FROM_GEO(MODEL_BITS_STAIRCASE_FRAME3, bits_geo_0006E8), 
	LOAD_MODEL_FROM_GEO(MODEL_BITS_STAIRCASE_FRAME4, bits_geo_000700), 
	LOAD_MODEL_FROM_GEO(MODEL_BITS_WARP_PIPE, warp_pipe_geo), 

	/* Fast64 begin persistent block [level commands] */
	/* Fast64 end persistent block [level commands] */

	AREA(1, bits_area_1),
		WARP_NODE(WARP_NODE_ENTRANCE, LEVEL_CASTLE_GROUNDS, 1, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0x0B, LEVEL_CASTLE_GROUNDS, 1, 0x0B, WARP_NO_CHECKPOINT),
		WARP_NODE(0x0C, LEVEL_CASTLE_GROUNDS, 1, 0x0C, WARP_NO_CHECKPOINT),
		WARP_NODE(WARP_NODE_F0, LEVEL_CASTLE_GROUNDS, 1, 0x0B, WARP_NO_CHECKPOINT),
		WARP_NODE(WARP_NODE_DEATH, LEVEL_CASTLE_GROUNDS, 1, 0x0C, WARP_NO_CHECKPOINT),
		OBJECT(MODEL_NONE, 0, 150, 0, 0, 0, 0, (0xC << 16), bhvAirborneDeathWarp),
		OBJECT(MODEL_NONE, 0, 150, 0, 0, 0, 0, (0xB << 16), bhvAirborneStarCollectWarp),
		OBJECT(MODEL_NONE, 0, 150, 0, 0, 0, 0, (0xA << 16), bhvSpinAirborneWarp),
		TERRAIN(bits_area_1_collision),
		MACRO_OBJECTS(bits_area_1_macro_objs),
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
