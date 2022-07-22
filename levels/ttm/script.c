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
#include "levels/ttm/header.h"

/* Fast64 begin persistent block [scripts] */
/* Fast64 end persistent block [scripts] */

const LevelScript level_ttm_entry[] = {
	INIT_LEVEL(),
	LOAD_MIO0(0x07, _ttm_segment_7SegmentRomStart, _ttm_segment_7SegmentRomEnd), 
	LOAD_MIO0_TEXTURE(0x09, _mountain_mio0SegmentRomStart, _mountain_mio0SegmentRomEnd), 
	LOAD_MIO0(0x0A, _water_skybox_mio0SegmentRomStart, _water_skybox_mio0SegmentRomEnd), 
	LOAD_MIO0(0x05, _group6_mio0SegmentRomStart, _group6_mio0SegmentRomEnd), 
	LOAD_RAW(0x0C, _group6_geoSegmentRomStart, _group6_geoSegmentRomEnd), 
	LOAD_MIO0(0x08, _common0_mio0SegmentRomStart, _common0_mio0SegmentRomEnd), 
	LOAD_RAW(0x0F, _common0_geoSegmentRomStart, _common0_geoSegmentRomEnd), 
	ALLOC_LEVEL_POOL(),
	MARIO(MODEL_MARIO, 0x00000001, bhvMario), 
	JUMP_LINK(script_func_global_1), 
	JUMP_LINK(script_func_global_7), 
	LOAD_MODEL_FROM_GEO(MODEL_TTM_SLIDE_EXIT_PODIUM, ttm_geo_000DF4), 
	LOAD_MODEL_FROM_GEO(MODEL_TTM_ROLLING_LOG, ttm_geo_000730), 
	LOAD_MODEL_FROM_GEO(MODEL_TTM_STAR_CAGE, ttm_geo_000710), 
	LOAD_MODEL_FROM_GEO(MODEL_TTM_BLUE_SMILEY, ttm_geo_000D14), 
	LOAD_MODEL_FROM_GEO(MODEL_TTM_YELLOW_SMILEY, ttm_geo_000D4C), 
	LOAD_MODEL_FROM_GEO(MODEL_TTM_STAR_SMILEY, ttm_geo_000D84), 
	LOAD_MODEL_FROM_GEO(MODEL_TTM_MOON_SMILEY, ttm_geo_000DBC), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_03, ttm_geo_000748), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_04, ttm_geo_000778), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_05, ttm_geo_0007A8), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_06, ttm_geo_0007D8), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_07, ttm_geo_000808), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_08, ttm_geo_000830), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_09, ttm_geo_000858), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0A, ttm_geo_000880), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0B, ttm_geo_0008A8), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0C, ttm_geo_0008D0), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0D, ttm_geo_0008F8), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0F, ttm_geo_000920), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_10, ttm_geo_000948), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_11, ttm_geo_000970), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_12, ttm_geo_000990), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_13, ttm_geo_0009C0), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_14, ttm_geo_0009F0), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_15, ttm_geo_000A18), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_16, ttm_geo_000A40), 

	/* Fast64 begin persistent block [level commands] */
	/* Fast64 end persistent block [level commands] */

	AREA(1, ttm_area_1),
		WARP_NODE(WARP_NODE_ENTRANCE, LEVEL_CASTLE_GROUNDS, 1, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0x0B, LEVEL_CASTLE_GROUNDS, 1, 0x0B, WARP_NO_CHECKPOINT),
		WARP_NODE(0x0C, LEVEL_CASTLE_GROUNDS, 1, 0x0C, WARP_NO_CHECKPOINT),
		WARP_NODE(WARP_NODE_F0, LEVEL_CASTLE_GROUNDS, 1, 0x0B, WARP_NO_CHECKPOINT),
		WARP_NODE(WARP_NODE_DEATH, LEVEL_CASTLE_GROUNDS, 1, 0x0C, WARP_NO_CHECKPOINT),
		OBJECT(MODEL_NONE, 0, 150, 0, 0, 0, 0, (0xC << 16), bhvAirborneDeathWarp),
		OBJECT(MODEL_NONE, 0, 150, 0, 0, 0, 0, (0xB << 16), bhvAirborneStarCollectWarp),
		OBJECT(MODEL_NONE, 0, 150, 0, 0, 0, 0, (0xA << 16), bhvSpinAirborneWarp),
		TERRAIN(ttm_area_1_collision),
		MACRO_OBJECTS(ttm_area_1_macro_objs),
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
