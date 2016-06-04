// -------------------------------------------------------------------------
//    @FileName         ��    NFIEquipModule.h
//    @Author           ��    chuanbo.guo
//    @Date             ��    2016-01-12
//    @Module           ��    NFIEquipModule
//
// -------------------------------------------------------------------------

#ifndef NFI_EQUIP_MODULE_H
#define NFI_EQUIP_MODULE_H

#include <iostream>
#include "NFILogicModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFMessageDefine/NFDefine.pb.h"

class NFIEquipModule : public NFILogicModule
{
public:
    virtual bool IntensifylevelToEquip(const NFGUID& self, const NFGUID& xEquipID) = 0;
    virtual bool HoleToEquip(const NFGUID& self, const NFGUID& xEquipID) = 0;
    virtual bool InlaystoneToEquip(const NFGUID& self, const NFGUID& xEquipID, const std::string& strStoneID, const int nHoleIndex) = 0;
    virtual bool ElementlevelToEquip(const NFGUID& self, const NFGUID& xEquipID, const NFMsg::EGameElementType& eElemetType) = 0;
    virtual bool WearEquip(const NFGUID& self, const NFGUID& xEquipID, const NFGUID& xTareget) = 0;
    virtual bool TakeOffEquip(const NFGUID& self, const NFGUID& xEquipID, const NFGUID& xTareget) = 0;

    /////////////Ӣ��װ���ͱ�������/////////////////////////////////////////////////////////////

    virtual bool DressEquipForHero(const NFGUID& self, const NFGUID& hero, const NFGUID& id) = 0;
    virtual bool TakeOffEquipForm(const NFGUID& self, const NFGUID& hero, const NFGUID& id) = 0;

    /////////////װ�����������/////////////////////////////////////////////////////////////

    virtual int SetEquipRandPropertyID(const NFGUID& self, const NFGUID& id, const std::string& strPropertyID) = 0;
    virtual const std::string& GetEquipRandPropertyID(const NFGUID& self, const NFGUID& id) = 0;

    /////////////װ���Ĵ�/////////////////////////////////////////////////////////////

    virtual bool SetEquipHoleCount(const NFGUID& self, const NFGUID& id, const int nCount) = 0;
    virtual int GetEquipHoleCount(const NFGUID& self, const NFGUID& id) = 0;

    /////////////װ����Ƕ��ʯͷ/////////////////////////////////////////////////////////////

    virtual bool SetEquipInlayStoneID(const NFGUID& self, const NFGUID& id, NFrame::Player::BagEquipList eIndex, const std::string& strPropertyID) = 0;
    virtual const std::string& GetEquipInlayStoneID(const NFGUID& self, const NFGUID& id, NFrame::Player::BagEquipList eIndex) = 0;

    /////////////װ��ǿ���ĵȼ�/////////////////////////////////////////////////////////////

    virtual bool SetEquipIntensifyLevel(const NFGUID& self, const NFGUID& id, const int nLevel) = 0;
    virtual int GetEquipIntensifyLevel(const NFGUID& self, const NFGUID& id) = 0;

    /////////////װ��Ԫ�صĵȼ�/////////////////////////////////////////////////////////////

    virtual bool SetEquipElementLevel(const NFGUID& self, const NFGUID& id, NFrame::Player::BagEquipList eIndex, const int nLevel) = 0;
    virtual int GetEquipElementLevel(const NFGUID& self, const NFGUID& id, NFrame::Player::BagEquipList eIndex) = 0;
};

#endif