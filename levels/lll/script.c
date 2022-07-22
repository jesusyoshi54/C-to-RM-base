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
#include "levels/lll/header.h"

/* Fast64 begin persistent block [scripts] */
/* Fast64 end persistent block [scripts] */

const LevelScript level_lll_entry[] = {
	INIT_LEVEL(),
	LOAD_MIO0(0x07, _lll_segment_7SegmentRomStart, _lll_segment_7SegmentRomEnd), 
	LOAD_MIO0_TEXTURE(0x09, _fire_mio0SegmentRomStart, _fire_mio0SegmentRomEnd), 
	LOAD_MIO0(0x0A, _water_skybox_mio0SegmentRomStart, _water_skybox_mio0SegmentRomEnd), 
	LOAD_MIO0(0x0B, _effect_mio0SegmentRomStart, _effect_mio0SegmentRomEnd), 
	LOAD_MIO0(0x05, _group2_mio0SegmentRomStart, _group2_mio0SegmentRomEnd), 
	LOAD_RAW(0x0C, _group2_geoSegmentRomStart, _group2_geoSegmentRomEnd), 
	LOAD_MIO0(0x06, _group17_mio0SegmentRomStart, _group17_mio0SegmentRomEnd), 
	LOAD_RAW(0x0D, _group17_geoSegmentRomStart, _group17_geoSegmentRomEnd), 
	LOAD_MIO0(0x08, _common0_mio0SegmentRomStart, _common0_mio0SegmentRomEnd), 
	LOAD_RAW(0x0F, _common0_geoSegmentRomStart, _common0_geoSegmentRomEnd), 
	ALLOC_LEVEL_POOL(),
	MARIO(MODEL_MARIO, 0x00000001, bhvMario), 
	JUMP_LINK(script_func_global_1), 
	JUMP_LINK(script_func_global_3), 
	JUMP_LINK(script_func_global_18), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_03, lll_geo_0009E0), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_04, lll_geo_0009F8), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_05, lll_geo_000A10), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_06, lll_geo_000A28), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_07, lll_geo_000A40), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_08, lll_geo_000A60), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0A, lll_geo_000A90), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0B, lll_geo_000AA8), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0C, lll_geo_000AC0), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0D, lll_geo_000AD8), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0E, lll_geo_000AF0), 
	LOAD_MODEL_FROM_GEO(MODEL_LLL_DRAWBRIDGE_PART, lll_geo_000B20), 
	LOAD_MODEL_FROM_GEO(MODEL_LLL_ROTATING_BLOCK_FIRE_BARS, lll_geo_000B38), 
	LOAD_MODEL_FROM_GEO(MODEL_LLL_ROTATING_HEXAGONAL_RING, lll_geo_000BB0), 
	LOAD_MODEL_FROM_GEO(MODEL_LLL_SINKING_RECTANGULAR_PLATFORM, lll_geo_000BC8), 
	LOAD_MODEL_FROM_GEO(MODEL_LLL_SINKING_SQUARE_PLATFORMS, lll_geo_000BE0), 
	LOAD_MODEL_FROM_GEO(MODEL_LLL_TILTING_SQUARE_PLATFORM, lll_geo_000BF8), 
	LOAD_MODEL_FROM_GEO(MODEL_LLL_BOWSER_PIECE_1, lll_geo_000C10), 
	LOAD_MODEL_FROM_GEO(MODEL_LLL_BOWSER_PIECE_2, lll_geo_000C30), 
	LOAD_MODEL_FROM_GEO(MODEL_LLL_BOWSER_PIECE_3, lll_geo_000C50), 
	LOAD_MODEL_FROM_GEO(MODEL_LLL_BOWSER_PIECE_4, lll_geo_000C70), 
	LOAD_MODEL_FROM_GEO(MODEL_LLL_BOWSER_PIECE_5, lll_geo_000C90), 
	LOAD_MODEL_FROM_GEO(MODEL_LLL_BOWSER_PIECE_6, lll_geo_000CB0), 
	LOAD_MODEL_FROM_GEO(MODEL_LLL_BOWSER_PIECE_7, lll_geo_000CD0), 
	LOAD_MODEL_FROM_GEO(MODEL_LLL_BOWSER_PIECE_8, lll_geo_000CF0), 
	LOAD_MODEL_FROM_GEO(MODEL_LLL_BOWSER_PIECE_9, lll_geo_000D10), 
	LOAD_MODEL_FROM_GEO(MODEL_LLL_BOWSER_PIECE_10, lll_geo_000D30), 
	LOAD_MODEL_FROM_GEO(MODEL_LLL_BOWSER_PIECE_11, lll_geo_000D50), 
	LOAD_MODEL_FROM_GEO(MODEL_LLL_BOWSER_PIECE_12, lll_geo_000D70), 
	LOAD_MODEL_FROM_GEO(MODEL_LLL_BOWSER_PIECE_13, lll_geo_000D90), 
	LOAD_MODEL_FROM_GEO(MODEL_LLL_BOWSER_PIECE_14, lll_geo_000DB0), 
	LOAD_MODEL_FROM_GEO(MODEL_LLL_MOVING_OCTAGONAL_MESH_PLATFORM, lll_geo_000B08), 
	LOAD_MODEL_FROM_GEO(MODEL_LLL_SINKING_ROCK_BLOCK, lll_geo_000DD0), 
	LOAD_MODEL_FROM_GEO(MODEL_LLL_ROLLING_LOG, lll_geo_000DE8), 
	LOAD_MODEL_FROM_GEO(MODEL_LLL_ROTATING_HEXAGONAL_PLATFORM, lll_geo_000A78), 
	LOAD_MODEL_FROM_GEO(MODEL_LLL_WOOD_BRIDGE, lll_geo_000B50), 
	LOAD_MODEL_FROM_GEO(MODEL_LLL_LARGE_WOOD_BRIDGE, lll_geo_000B68), 
	LOAD_MODEL_FROM_GEO(MODEL_LLL_FALLING_PLATFORM, lll_geo_000B80), 
	LOAD_MODEL_FROM_GEO(MODEL_LLL_LARGE_FALLING_PLATFORM, lll_geo_000B98), 
	LOAD_MODEL_FROM_GEO(MODEL_LLL_VOLCANO_FALLING_TRAP, lll_geo_000EA8), 

	/* Fast64 begin persistent block [level commands] */
	/* Fast64 end persistent block [level commands] */

	AREA(1, lll_area_1),
		WARP_NODE(WARP_NODE_ENTRANCE, LEVEL_CASTLE_GROUNDS, 1, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0x0B, LEVEL_CASTLE_GROUNDS, 1, 0x0B, WARP_NO_CHECKPOINT),
		WARP_NODE(0x0C, LEVEL_CASTLE_GROUNDS, 1, 0x0C, WARP_NO_CHECKPOINT),
		WARP_NODE(WARP_NODE_F0, LEVEL_CASTLE_GROUNDS, 1, 0x0B, WARP_NO_CHECKPOINT),
		WARP_NODE(WARP_NODE_DEATH, LEVEL_CASTLE_GROUNDS, 1, 0x0C, WARP_NO_CHECKPOINT),
		OBJECT(MODEL_NONE, 0, 150, 0, 0, 0, 0, (0xC << 16), bhvAirborneDeathWarp),
		OBJECT(MODEL_NONE, 0, 150, 0, 0, 0, 0, (0xB << 16), bhvAirborneStarCollectWarp),
		OBJECT(MODEL_NONE, 0, 150, 0, 0, 0, 0, (0xA << 16), bhvSpinAirborneWarp),
		TERRAIN(lll_area_1_collision),
		MACRO_OBJECTS(lll_area_1_macro_objs),
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
