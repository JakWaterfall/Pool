#pragma once

/// <summary>
/// Constants for the screen size and table dimensions so all objects can interact with the table
/// </summary>
constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

//constexpr int SCREEN_WIDTH = 1920;
//constexpr int SCREEN_HEIGHT = 1080;

constexpr int TABLE_X = SCREEN_WIDTH / 6;
constexpr int TABLE_Y = SCREEN_HEIGHT / 6;
constexpr int TABLE_WIDTH = SCREEN_WIDTH * 2 / 3;
constexpr int TABLE_HEIGHT = SCREEN_HEIGHT * 2 / 3;

constexpr int TABLE_X2 = TABLE_X + TABLE_WIDTH;
constexpr int TABLE_Y2 = TABLE_Y + TABLE_HEIGHT;

constexpr int TABLE_LINE_FROM_X = TABLE_WIDTH / 4;

constexpr int BLACK_POINT_X = TABLE_X + (int)(TABLE_WIDTH * 0.75);
constexpr int BLACK_POINT_Y = TABLE_Y + TABLE_HEIGHT / 2;
