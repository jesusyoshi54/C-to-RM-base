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
#include "levels/rr/header.h"

/* Fast64 begin persistent block [scripts] */
/* Fast64 end persistent block [scripts] */

const LevelScript level_rr_entry[] = {
	INIT_LEVEL(),
	LOAD_MIO0(0x07, _rr_segment_7SegmentRomStart, _rr_segment_7SegmentRomEnd), 
	LOAD_MIO0_TEXTURE(0x09, _sky_mio0SegmentRomStart, _sky_mio0SegmentRomEnd), 
	LOAD_MIO0(0x0A, _water_skybox_mio0SegmentRomStart, _water_skybox_mio0SegmentRomEnd), 
	LOAD_MIO0(0x05, _group11_mio0SegmentRomStart, _group11_mio0SegmentRomEnd), 
	LOAD_RAW(0x0C, _group11_geoSegmentRomStart, _group11_geoSegmentRomEnd), 
	LOAD_MIO0(0x08, _common0_mio0SegmentRomStart, _common0_mio0SegmentRomEnd), 
	LOAD_RAW(0x0F, _common0_geoSegmentRomStart, _common0_geoSegmentRomEnd), 
	ALLOC_LEVEL_POOL(),
	MARIO(MODEL_MARIO, 0x00000001, bhvMario), 
	JUMP_LINK(script_func_global_12), 
	JUMP_LINK(script_func_global_1), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_03, rr_geo_000660), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_04, rr_geo_000678), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_05, rr_geo_000690), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_06, rr_geo_0006A8), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_07, rr_geo_0006C0), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_08, rr_geo_0006D8), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_09, rr_geo_0006F0), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0A, rr_geo_000708), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0B, rr_geo_000720), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0C, rr_geo_000738), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0D, rr_geo_000758), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0E, rr_geo_000770), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0F, rr_geo_000788), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_10, rr_geo_0007A0), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_11, rr_geo_0007B8), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_12, rr_geo_0007D0), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_13, rr_geo_0007E8), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_14, rr_geo_000800), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_15, rr_geo_000818), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_16, rr_geo_000830), 
	LOAD_MODEL_FROM_GEO(MODEL_RR_SLIDING_PLATFORM, rr_geo_0008C0), 
	LOAD_MODEL_FROM_GEO(MODEL_RR_FLYING_CARPET, rr_geo_000848), 
	LOAD_MODEL_FROM_GEO(MODEL_RR_OCTAGONAL_PLATFORM, rr_geo_0008A8), 
	LOAD_MODEL_FROM_GEO(MODEL_RR_ROTATING_BRIDGE_PLATFORM, rr_geo_000878), 
	LOAD_MODEL_FROM_GEO(MODEL_RR_TRIANGLE_PLATFORM, rr_geo_0008D8), 
	LOAD_MODEL_FROM_GEO(MODEL_RR_CRUISER_WING, rr_geo_000890), 
	LOAD_MODEL_FROM_GEO(MODEL_RR_SEESAW_PLATFORM, rr_geo_000908), 
	LOAD_MODEL_FROM_GEO(MODEL_RR_L_SHAPED_PLATFORM, rr_geo_000940), 
	LOAD_MODEL_FROM_GEO(MODEL_RR_SWINGING_PLATFORM, rr_geo_000860), 
	LOAD_MODEL_FROM_GEO(MODEL_RR_DONUT_PLATFORM, rr_geo_000920), 
	LOAD_MODEL_FROM_GEO(MODEL_RR_ELEVATOR_PLATFORM, rr_geo_0008F0), 
	LOAD_MODEL_FROM_GEO(MODEL_RR_TRICKY_TRIANGLES, rr_geo_000958), 
	LOAD_MODEL_FROM_GEO(MODEL_RR_TRICKY_TRIANGLES_FRAME1, rr_geo_000970), 
	LOAD_MODEL_FROM_GEO(MODEL_RR_TRICKY_TRIANGLES_FRAME2, rr_geo_000988), 
	LOAD_MODEL_FROM_GEO(MODEL_RR_TRICKY_TRIANGLES_FRAME3, rr_geo_0009A0), 
	LOAD_MODEL_FROM_GEO(MODEL_RR_TRICKY_TRIANGLES_FRAME4, rr_geo_0009B8), 

	/* Fast64 begin persistent block [level commands] */
	/* Fast64 end persistent block [level commands] */

	AREA(1, rr_area_1),
		WARP_NODE(WARP_NODE_ENTRANCE, LEVEL_CASTLE_GROUNDS, 1, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0x0B, LEVEL_CASTLE_GROUNDS, 1, 0x0B, WARP_NO_CHECKPOINT),
		WARP_NODE(0x0C, LEVEL_CASTLE_GROUNDS, 1, 0x0C, WARP_NO_CHECKPOINT),
		WARP_NODE(WARP_NODE_F0, LEVEL_CASTLE_GROUNDS, 1, 0x0B, WARP_NO_CHECKPOINT),
		WARP_NODE(WARP_NODE_DEATH, LEVEL_CASTLE_GROUNDS, 1, 0x0C, WARP_NO_CHECKPOINT),
		OBJECT(MODEL_NONE, 0, 150, 0, 0, 0, 0, (0xC << 16), bhvAirborneDeathWarp),
		OBJECT(MODEL_NONE, 0, 150, 0, 0, 0, 0, (0xB << 16), bhvAirborneStarCollectWarp),
		OBJECT(MODEL_NONE, 0, 150, 0, 0, 0, 0, (0xA << 16), bhvSpinAirborneWarp),
		TERRAIN(rr_area_1_collision),
		MACRO_OBJECTS(rr_area_1_macro_objs),
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
