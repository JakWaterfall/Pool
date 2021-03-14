#pragma once

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

//constexpr int SCREEN_WIDTH = 1920;
//constexpr int SCREEN_HEIGHT = 1080;

constexpr int TABLE_X = SCREEN_WIDTH / 6;
constexpr int TABLE_Y = SCREEN_HEIGHT / 6;
constexpr int TABLE_WIDTH = SCREEN_WIDTH * 2 / 3;
constexpr int TABLE_HEIGHT = SCREEN_HEIGHT * 2 / 3;

constexpr int TABLE_W = TABLE_X + TABLE_WIDTH;
constexpr int TABLE_H = TABLE_Y + TABLE_HEIGHT;

constexpr int TABLE_LINE_FROM_X = TABLE_WIDTH / 4;

constexpr int BLACK_POINT_X = TABLE_X + (int)(TABLE_WIDTH * 0.75);
constexpr int BLACK_POINT_Y = TABLE_Y + TABLE_HEIGHT / 2;

// MAKE A BETTER SOLITION THEN ALL CONSTANTS IN A HEADER FILE