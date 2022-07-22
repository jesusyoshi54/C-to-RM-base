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
#include "levels/sa/header.h"

/* Fast64 begin persistent block [scripts] */
/* Fast64 end persistent block [scripts] */

const LevelScript level_sa_entry[] = {
	INIT_LEVEL(),
	LOAD_MIO0(0x07, _sa_segment_7SegmentRomStart, _sa_segment_7SegmentRomEnd), 
	LOAD_MIO0_TEXTURE(0x09, _inside_mio0SegmentRomStart, _inside_mio0SegmentRomEnd), 
	LOAD_MIO0(0x0A, _water_skybox_mio0SegmentRomStart, _water_skybox_mio0SegmentRomEnd), 
	LOAD_MIO0(0x0B, _effect_mio0SegmentRomStart, _effect_mio0SegmentRomEnd), 
	LOAD_MIO0(0x05, _group4_mio0SegmentRomStart, _group4_mio0SegmentRomEnd), 
	LOAD_RAW(0x0C, _group4_geoSegmentRomStart, _group4_geoSegmentRomEnd), 
	LOAD_MIO0(0x06, _group13_mio0SegmentRomStart, _group13_mio0SegmentRomEnd), 
	LOAD_RAW(0x0D, _group13_geoSegmentRomStart, _group13_geoSegmentRomEnd), 
	ALLOC_LEVEL_POOL(),
	MARIO(MODEL_MARIO, 0x00000001, bhvMario), 
	JUMP_LINK(script_func_global_5), 
	JUMP_LINK(script_func_global_14), 

	/* Fast64 begin persistent block [level commands] */
	/* Fast64 end persistent block [level commands] */

	AREA(1, sa_area_1),
		WARP_NODE(WARP_NODE_ENTRANCE, LEVEL_CASTLE_GROUNDS, 1, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0x0B, LEVEL_CASTLE_GROUNDS, 1, 0x0B, WARP_NO_CHECKPOINT),
		WARP_NODE(0x0C, LEVEL_CASTLE_GROUNDS, 1, 0x0C, WARP_NO_CHECKPOINT),
		WARP_NODE(WARP_NODE_F0, LEVEL_CASTLE_GROUNDS, 1, 0x0B, WARP_NO_CHECKPOINT),
		WARP_NODE(WARP_NODE_DEATH, LEVEL_CASTLE_GROUNDS, 1, 0x0C, WARP_NO_CHECKPOINT),
		OBJECT(MODEL_NONE, 0, 150, 0, 0, 0, 0, (0xC << 16), bhvAirborneDeathWarp),
		OBJECT(MODEL_NONE, 0, 150, 0, 0, 0, 0, (0xB << 16), bhvAirborneStarCollectWarp),
		OBJECT(MODEL_NONE, 0, 150, 0, 0, 0, 0, (0xA << 16), bhvSpinAirborneWarp),
		TERRAIN(sa_area_1_collision),
		MACRO_OBJECTS(sa_area_1_macro_objs),
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
