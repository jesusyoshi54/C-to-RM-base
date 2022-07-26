const Collision rr_area_1_collision[] = {
	COL_INIT(),
	COL_VERTEX_INIT(29),
	COL_VERTEX(-8192, -2800, -8192),
	COL_VERTEX(-8192, -2800, 0),
	COL_VERTEX(0, -2800, 0),
	COL_VERTEX(0, -2800, -8192),
	COL_VERTEX(-8192, -2800, 8192),
	COL_VERTEX(0, -2800, 8192),
	COL_VERTEX(8192, -2800, 0),
	COL_VERTEX(8192, -2800, -8192),
	COL_VERTEX(8192, -2800, 8192),
	COL_VERTEX(-16384, -2800, -16384),
	COL_VERTEX(-16384, -2800, -8192),
	COL_VERTEX(-8192, -2800, -16384),
	COL_VERTEX(-16384, -2800, 0),
	COL_VERTEX(-16384, -2800, 8192),
	COL_VERTEX(-16384, -2800, 16384),
	COL_VERTEX(-8192, -2800, 16384),
	COL_VERTEX(0, -2800, 16384),
	COL_VERTEX(8192, -2800, 16384),
	COL_VERTEX(16384, -2800, 16384),
	COL_VERTEX(16384, -2800, 8192),
	COL_VERTEX(16384, -2800, 0),
	COL_VERTEX(16384, -2800, -8192),
	COL_VERTEX(8192, -2800, -16384),
	COL_VERTEX(16384, -2800, -16384),
	COL_VERTEX(0, -2800, -16384),
	COL_VERTEX(-2048, 0, -2048),
	COL_VERTEX(-2048, 0, 2048),
	COL_VERTEX(2048, 0, 2048),
	COL_VERTEX(2048, 0, -2048),
	COL_TRI_INIT(SURFACE_DEATH_PLANE, 32),
	COL_TRI(0, 1, 2),
	COL_TRI(0, 2, 3),
	COL_TRI(1, 4, 5),
	COL_TRI(1, 5, 2),
	COL_TRI(3, 2, 6),
	COL_TRI(3, 6, 7),
	COL_TRI(2, 5, 8),
	COL_TRI(2, 8, 6),
	COL_TRI(9, 10, 0),
	COL_TRI(9, 0, 11),
	COL_TRI(10, 12, 1),
	COL_TRI(10, 1, 0),
	COL_TRI(12, 13, 4),
	COL_TRI(12, 4, 1),
	COL_TRI(13, 14, 15),
	COL_TRI(13, 15, 4),
	COL_TRI(4, 15, 16),
	COL_TRI(4, 16, 5),
	COL_TRI(5, 16, 17),
	COL_TRI(5, 17, 8),
	COL_TRI(8, 17, 18),
	COL_TRI(8, 18, 19),
	COL_TRI(6, 8, 19),
	COL_TRI(6, 19, 20),
	COL_TRI(7, 6, 20),
	COL_TRI(7, 20, 21),
	COL_TRI(22, 7, 21),
	COL_TRI(22, 21, 23),
	COL_TRI(24, 3, 7),
	COL_TRI(24, 7, 22),
	COL_TRI(11, 0, 3),
	COL_TRI(11, 3, 24),
	COL_TRI_INIT(SURFACE_DEFAULT, 2),
	COL_TRI(25, 26, 27),
	COL_TRI(25, 27, 28),
	COL_TRI_STOP(),
	COL_END()
};
