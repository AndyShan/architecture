#ifndef _elo_h_
#define _elo_h_

#include "math.h"

static const unsigned int deno = 400;

/*
国际象棋的评分标准
*/
static const double novice  = 1000.0;  //0-999 初学者
static const double someExp = 1500.0;  //1000-1499 有经验者
static const double skill   = 2000.0;  //1500-1999 熟练者
static const double expert  = 2200.0;  //2000-2199 专家
static const double master  = 2400.0;  //2200-2399 大师 >2400 宗师

/*
*计算自适应值k
*@pram score: 用户的分数
*@pram is_win: 用户是否赢了这一局
*@returns: 自适应值k
*/
static inline unsigned int self_adaption_k(double score, bool is_win) {
	return score < novice ? (32.0 + (is_win ? 32.0:0.0)): 
		   score < someExp ? (32.0 + (is_win ? 16.0:0.0)):
		   score < skill ? 32.0 :
		   score < expert ? 20.0 :
		   score < master ? 15.0 : 10.0;
}

/*
*计算a用户对b用户的胜率
*@pram a: 用户a的分数
*@pram b: 用户b的分数
*@pram returns: 用户a对b的胜率
*/
static inline double get_win_rate(double a, double b)
{
	return 1 / (1 + pow(10, (b-a)/deno));//Sigmoid函数
}

/*
*计算用户a的积分变化
*@pram a: 用户a的分数
*@pram b: 用户b的分数
*@pram is_win: 用户a是否获胜
*@pram is_draw: 用户a是否与b打成平手
*@pram returns: 用户的积分变化
*/
static inline double score_chg(double a, double b, bool is_win, bool is_draw)
{
	double w = is_draw ? 0.5 : (is_win ? 1.0 : 0.0);
	double win_rate = get_win_rate(a, b);
	return (w - win_rate) * self_adaption_k(a, is_win);
}

/*
*计算用户的积分
*@pram a: 用户a的分数
*@pram b: 用户b的分数
*@pram is_win: 用户a是否获胜
*@pram is_draw: 用户a是否与b打成平手
*@pram returns: 用户的积分
*/
static inline double elo(double a, double b, bool is_win, bool is_draw)
{
	return (a + score_chg(a, b, is_win, is_draw));
}

#endif
