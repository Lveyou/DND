//////////////////////////////////////////////////////////////////////////
//name:		DND headfile
//author:	Lveyou
//data:		17-07-26

//other:
//17-07-26: ���ļ����û�����������ʹ��DND���棨�û�ָʹ��DND�ĳ���Ա�� - Lveyou
//17-07-27: Sorry, my English is taught by my C language teacher.
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_DND_H_
#define _DND_DND_H_

#define DND_IMPORT
///////////////////////////CORE///////////////////////////////////////////
#include "DNDMain.h"					//DNDMain					������
#include "DNDGame.h"					//Game base class			��Ϸ����
#include "DNDSystem.h"					//System					ϵͳ
#include "DNDDebug.h"					//Debug						����
#include "DNDTime.h"					//Time						ʱ��
#include "DNDInput.h"					//Input						����
#include "DNDNet.h"						//Net						����
#include "DNDCoor.h"					//Coor						����ϵ
#include "DNDSprite.h"					//Sprite					����
#include "DNDCanvas.h"					//Canvas					����
#include "DNDText.h"					//Text						�ı�
#include "DNDRigidBody.h"				//2D Physics				����
#include "DNDThread.h"					//Thread					�߳�
//////////////////////////BASE//////////////////////////////////////////
#include "DNDSingleton.h"				//singleton template class	����ģ��
#include "DNDString.h"					//String					�ַ���
#include "DNDColor.h"					//Color						��ɫ
#include "DNDImage.h"					//Image						ͼ��
///////////////////////////GUI///////////////////////////////////////////
#include "DNDGUI.h"						//GUI						�ؼ�����
#include "DNDGUIButtonSpriteScale.h"	//ButtonSpriteScale			�������Ű�ť
//////////////////////////////////////////////////////////////////////////

#endif