//////////////////////////////////////////////////////////////////////////
//name:		DND headfile
//author:	Lveyou
//date:		17-07-26

//other:
//17-07-26: 此文件被用户包含，用来使用DND引擎（用户指使用DND的程序员） - Lveyou
//17-07-27: Sorry, my English is taught by my C language teacher.
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_DND_H_
#define _DND_DND_H_

#define DND_IMPORT
///////////////////////////CORE///////////////////////////////////////////
#include "DNDMain.h"					//DNDMain					主函数
#include "DNDGame.h"					//Game base class			游戏基类
#include "DNDGameServer.h"				//GameServer				服务器
#include "DNDSystem.h"					//System					系统
#include "DNDDebug.h"					//Debug						调试
#include "DNDTime.h"					//Time						时间
#include "DNDInput.h"					//Input						输入
#include "DNDNet.h"						//Net						网络
#include "DNDCoor.h"					//Coor						坐标系
#include "DNDSprite.h"					//Sprite					精灵
#include "DNDAnimation.h"				//Animation					帧动画
#include "DNDCanvas.h"					//Canvas					画布
#include "DNDText.h"					//Text						文本
#include "DNDRigidBody.h"				//2D Physics				物理
#include "DNDThread.h"					//Thread					线程
#include "DNDMutex.h"					//Mutex						互斥锁
#include "DNDCycleLock.h"				//CycleLock					循环锁
#include "DNDSound.h"					//Sound						音效
#include "DNDStreamOutput.h"			//StreamOutput				内存输出流
#include "DNDStreamInput.h"				//StreamInput				内存输入流
//////////////////////////BASE//////////////////////////////////////////
#include "DNDSingleton.h"				//singleton template class	单例模板
#include "DNDString.h"					//String					字符串
#include "DNDColor.h"					//Color						颜色
#include "DNDImage.h"					//Image						图像
#include "DNDFile.h"					//File						文件
#include "DNDMath.h"					//Math						数学
///////////////////////////GUI///////////////////////////////////////////
#include "DNDGUI.h"						//GUI						控件基类
#include "DNDGUIButtonSpriteScale.h"	//ButtonSpriteScale			精灵缩放按钮
#include "DNDGUIButtonTextColor.h"		//ButtonTextColor			文本颜色按钮
#include "DNDGUIButtonSprite3Text1.h"	//ButtonSprite3Text1		3精灵1文本
#include "DNDGUIEditBoxSprite.h"		//EditBoxSprite				精灵背景输入框
#include "DNDGUISwitchSprite2.h"		//SwitchSprite				2精灵开关
#include "DNDGUISwitchSprite3Color2.h"	//SwitchSprite				3精灵2颜色开关
#include "DNDGUIButtonSprite3.h"		//ButtonSprite				3精灵按钮
#include "DNDGUISprite9.h"				//Sprite9					9妹精灵
#include "DNDGUISliderSprite2.h"		//SliderSprite2				2精灵滚动条
#include "DNDGUISliderSprite5.h"		//SliderSprite5				5精灵滚动条
#include "DNDGUIComboBox.h"				//ComboBox					下拉框
#include "DNDGUIShadeBox.h"				//ShadeBox					旋转技能读条
//////////////////////////////////////////////////////////////////////////

#endif