#pragma once

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

constexpr int TABLE_X = SCREEN_WIDTH / 6;
constexpr int TABLE_Y = SCREEN_HEIGHT / 6;
constexpr int TABLE_WIDTH = SCREEN_WIDTH * 2 / 3;
constexpr int TABLE_HEIGHT = SCREEN_HEIGHT * 2 / 3;

constexpr int TABLE_W = TABLE_X + TABLE_WIDTH;
constexpr int TABLE_H = TABLE_Y + TABLE_HEIGHT;

constexpr int TABLE_LINE_FROM_X = 200;

constexpr int BLACK_POINT_X = TABLE_W / 2 + TABLE_X / 2 + TABLE_WIDTH / 4;
constexpr int BLACK_POINT_Y = TABLE_H / 2 + TABLE_Y / 2;