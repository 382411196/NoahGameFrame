// -------------------------------------------------------------------------
//    @FileName         :    NFCSkillModule.cpp
//    @Author               :    LvSheng.Huang
//    @Date                 :    2013-06-11
//    @Module               :    NFCSkillModule
//    @Desc                 :
// -------------------------------------------------------------------------

#include "NFCPackModule.h"
#include "NFComm/NFCore/NFTimer.h"

const std::string mstrPackTableName[] =
{
    NFrame::Player::R_BagItemList(),
    NFrame::Player::R_BagEquipList(),
    NFrame::Player::R_PlayerHero(),
};

bool NFCPackModule::Init()
{
    return true;
}

bool NFCPackModule::Shut()
{
    return true;
}

bool NFCPackModule::Execute()
{
    //λ����
    return true;
}

bool NFCPackModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>( "NFCKernelModule" );
    m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>( "NFCElementInfoModule" );
    m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>( "NFCSceneProcessModule" );
    m_pPropertyModule = pPluginManager->FindModule<NFIPropertyModule>( "NFCPropertyModule" );
    m_pLogModule = pPluginManager->FindModule<NFILogModule>("NFCLogModule");
    m_pUUIDModule = pPluginManager->FindModule<NFIUUIDModule>("NFCUUIDModule");

    assert( NULL != m_pKernelModule );
    assert( NULL != m_pElementInfoModule );
    assert( NULL != m_pSceneProcessModule );
    assert( NULL != m_pPropertyModule );
    assert( NULL != m_pLogModule );
    assert( NULL != m_pUUIDModule );

    m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFCPackModule::OnClassObjectEvent );
    m_pKernelModule->AddClassCallBack(NFrame::NPC::ThisName(), this, &NFCPackModule::OnClassObjectEvent );

    return true;
}

const std::string& NFCPackModule::GetPackName( const PackTableType name )
{
    if ( name > PackTableType::MaxPack || name < PackTableType::NONE_PACK )
    {
        return NULL_STR;
    }

    return mstrPackTableName[name];
}

// const NFGUID& NFCPackModule::CreateEquip( const NFGUID& self, const std::string& strConfigName )
// {
//     NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
//     if ( NULL == pObject )
//     {
//         return NULL_OBJECT;
//     }
// 
//     //����ȷ�������װ��
//     NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = m_pElementInfoModule->GetPropertyManager( strConfigName );
//     if ( NULL == pPropertyManager )
//     {
//         return NULL_OBJECT;
//     }
// 
//     NF_SHARE_PTR<NFIProperty> pPropertyType = pPropertyManager->GetElement( "ItemType" );
//     if (NULL == pPropertyType)
//     {
//         return NULL_OBJECT;
//     }
// 
//     // �ж���Ʒ�Ƿ�Ϊװ��
//     int nItemType = pPropertyType->GetInt();
//     if ( NFMsg::EIT_EQUIP != nItemType )
//     {
//         return NULL_OBJECT;
//     }
// 
//     NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::NormalPack ) );
//     const int nPackCount = pObject->GetPropertyInt("PackCount"); // ������������
// 
//     int nHasUsedCount = 0;
//     for ( int i = 0; i < pRecord->GetRows(); i++ )
//     {
//         nHasUsedCount++;
//         if ( pRecord->IsUsed( i ) )
//         {
//             continue;
//         }
// 
//         if (nHasUsedCount > nPackCount)
//         {
//             m_pLogModule->LogElement(NFILogModule::NLL_WARING_NORMAL, self, strConfigName, "NormalPack is full, cannot add equip", __FUNCTION__, __LINE__);
//             continue;
//         }
// 
//         NFGUID ident = m_pUUIDModule->CreateGUID();
//         NFCDataList var;
//         var << ident;
//         var << strConfigName.c_str() ;
//         var << 1;
// 
//         // ������
//         NF_SHARE_PTR<NFIProperty> pBoundProperty = pPropertyManager->GetElement( "Bound" );
//         var << (NULL != pBoundProperty ? pBoundProperty->GetInt() : 0);
//         var << 0;
//         var << 0;
// 
//         for ( int j = 0; j < 6; j++ )
//         {
//             var <<  NULL_STR;
//         }
// 
//         var << 0;
//         var << 0;
//         var << NULL_STR;
// 
//         std::string strRndProperty;
//         var << strRndProperty.c_str();
// 
//         var << NFTime::GetNowTime();
// 
//         std::string strBaseProperty;
//         var << strBaseProperty.c_str(); // �̶�
// 
//         pRecord->AddRow( i, var );
//         return ident;
//     }
// 
//     return NULL_OBJECT;
// }

int NFCPackModule::OnClassObjectEvent( const NFGUID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var )
{
    if (strClassNames == NFrame::Player::ThisName())
    {
        if (CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent )
        {
            //m_pKernelModule->AddRecordCallBack( self, GetPackName( PackTableType::BagItemPack ), this, &NFCPackModule::OnObjectPackViewRecordEvent );
        }
        else if ( CLASS_OBJECT_EVENT::COE_CREATE_EFFECTDATA == eClassEvent )
        {
            //RefreshInitViewItem(self);
            //��һ�β�ˢ�£���Ϊ��һ��װ���󣬻��Զ�ˢ��
            //RefreshEquipProperty(self);
        }
        else if ( CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent )
        {
            //m_pEventProcessModule->AddEventCallBack( self, NFED_ON_CLIENT_SWAP_TABLE, this, &NFCPackModule::OnSwapTableRowEvent );

            // TOADD ����������Ҫ���ټӻص�
        }
    }

    if ( strClassNames == NFrame::NPC::ThisName())
    {
        if (CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent)
        {
            m_pKernelModule->AddEventCallBack(self, NFED_ON_OBJECT_BE_KILLED, this, &NFCPackModule::OnObjectBeKilled);
        }
    }

    return 0;
}

// int NFCPackModule::QueryCount( const NFGUID& self, const std::string& strItemConfigID ) const
// {
//     NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
//     if ( NULL == pObject )
//     {
//         m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);
//         return 0;
//     }
// 
//     NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::NormalPack ) );
//     if (NULL == pRecord)
//     {
//         return 0;
//     }
// 
//     NFINT64 nCount = 0;
//     for (int i = 0; i < pRecord->GetRows(); ++i)
//     {
//         if (pRecord->IsUsed(i) && pRecord->GetString(i, NFMsg::BagItemList_ItemCount) == strItemConfigID)
//         {
//             nCount += pRecord->GetInt(i, NFMsg::BagItemList_ItemCount);
//         }
//     }
// 
//     return nCount;
// }

// bool NFCPackModule::DeleteItem( const NFGUID& self, const std::string& strItemConfigID, const int nCount )
// {
//      if (QueryCount(self, strItemConfigID) < nCount)
//      {
//          m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "There is not enough item count", strItemConfigID, __FUNCTION__, __LINE__);
//  
//          return false;
//      }
//  
//      NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
//      if ( NULL == pObject )
//      {
//          m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);
//          return false;
//      }
//  
//      NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::NormalPack ) );
//      if (NULL == pRecord)
//      {
//          return false;
//      }
//  
//      int nRemainDelCount = nCount;
//      for (int i = 0; i < pRecord->GetRows(); ++i)
//      {
//          if (nRemainDelCount <= 0)
//          {
//              break;
//          }
//  
//          if (pRecord->IsUsed(i) && pRecord->GetString(i, NFMsg::BagItemList_ConfigID) == strItemConfigID)
//          {
//              const int nGridCount = pRecord->GetInt(i, NFMsg::BagItemList_ItemCount);
//              if (nGridCount < nRemainDelCount)
//              {
//                  //�˸�����������
//                  nRemainDelCount -= nGridCount;
//                  pRecord->Remove(i);
//              }
//              else if (nGridCount == nRemainDelCount)
//              {
//                  //�˸������պú���
//                  pRecord->Remove(i);
//                  return true;
//              }
//              else if (nGridCount > nRemainDelCount)
//              {
//                  //������ɾ������ʣ��
//                  pRecord->SetInt(i, NFMsg::BagItemList_ItemCount, nGridCount - nRemainDelCount);
//                  return true;
//              }
//          }
//      }
// 
//     return false;
// }

int NFCPackModule::OnObjectBeKilled(const NFGUID& self, const int nEventID, const NFIDataList& var)
{
    if ( var.GetCount() != 1 || !var.TypeEx(TDATA_OBJECT, TDATA_UNKNOWN))
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "args error", "", __FUNCTION__, __LINE__);
        return 1;
    }

    NFGUID identKiller = var.Object( 0 );
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(identKiller);
    if (nullptr == pObject)
    {
        return 1;
    }


    return 0;
}

const NFGUID& NFCPackModule::CreateEquip( const NFGUID& self, const std::string& strConfigName )
{
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
	if ( NULL == pObject )
	{
		return NULL_OBJECT;
	}

	//����ȷ�������װ��
	bool bExist = m_pElementInfoModule->ExistElement( strConfigName );
	if ( !bExist )
	{
		return NULL_OBJECT;
	}

	int nItemType = m_pElementInfoModule->GetPropertyInt(strConfigName, NFrame::Item::ItemType());
	if ( NFMsg::EItemType::EIT_EQUIP != nItemType )
	{
		return NULL_OBJECT;
	}

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::BagEquipPack ) );
	if (!pRecord)
	{
		return NULL_OBJECT;
	}

	NFGUID ident = m_pUUIDModule->CreateGUID();

	NFCDataList var = pRecord->GetInitData();

	var.SetObject(NFrame::Player::BagEquipList_GUID, ident);
	var.SetString(NFrame::Player::BagEquipList_ConfigID, strConfigName.c_str());
	var.SetInt(NFrame::Player::BagEquipList_Date, NFTime::GetNowTime());


	int nAddRow = pRecord->AddRow(-1, var);
	if (nAddRow > 0)
	{
		return pRecord->GetObject(nAddRow, NFrame::Player::BagEquipList_GUID);
	}

	return NULL_OBJECT;
}

bool NFCPackModule::CreateItem( const NFGUID& self, const std::string& strConfigName, const int nCount )
{
	if (nCount <= 0)
	{
		return 0;
	}

	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
	if ( NULL == pObject )
	{
		return 0;
	}

	//����ȷ�������װ��
	bool bExist = m_pElementInfoModule->ExistElement( strConfigName );
	if ( !bExist )
	{
		return 0;
	}

	int nItemType = m_pElementInfoModule->GetPropertyInt(strConfigName, NFrame::Item::ItemType());
    PackTableType eBagType = GetPackBagType(nItemType);
    if (eBagType != PackTableType::BagItemPack)
    {
        return -1;
    }

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::BagItemPack ) );
	if (!pRecord)
	{
		return 0;
	}

	int nRow = FindItemRowByConfig(self, strConfigName);
	if (nRow < 0)
	{
        NFCDataList varRowData;
        varRowData << strConfigName;
        varRowData << nCount;
        varRowData << 0;
        varRowData << 0;
        varRowData << NFGUID(0, NFTime::GetNowTime());

		int nRow= pRecord->AddRow(-1, varRowData);
		if (nRow >= 0)
		{
			return nCount;
		}
	}
	else
	{
		int nOldCount = pRecord->GetInt(nRow, NFrame::Player::BagItemList_ItemCount);
		int nNewCount = nOldCount + nCount;
		pRecord->SetInt(nRow, NFrame::Player::BagItemList_ItemCount, nNewCount);
	}

	return 0;
}

PackTableType NFCPackModule::GetPackBagType(int nItemType)
{
    switch (nItemType)
    {
    case NFMsg::EItemType::EIT_NORMAL:
        break;
    case NFMsg::EItemType::EIT_EQUIP:
    {
        return PackTableType::BagEquipPack;
    }
    break;
    case NFMsg::EItemType::EIT_BOUNTY:
    case NFMsg::EItemType::EIT_HERO_CARD:
    case NFMsg::EItemType::EIT_HERO_STONE:
    case NFMsg::EItemType::EIT_STRENGTHEN_STONE:
    case NFMsg::EItemType::EIT_DEMONIZATION_STONE:
    case NFMsg::EItemType::EIT_GEM_STONE:
    case NFMsg::EItemType::EIT_CURRENCY:
    case NFMsg::EItemType::EIT_ITEM_REBORN:
    case NFMsg::EItemType::EIT_ITEM_POSITION:
    case NFMsg::EItemType::EIT_WOOD:
    case NFMsg::EItemType::EIT_STONE:
        break;
    default:
        break;
    }

    return PackTableType::BagItemPack;
}

int NFCPackModule::FindItemRowByConfig( const NFGUID& self, const std::string& strItemConfigID )
{
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
	if ( NULL == pObject )
	{
		return -1;
	}

	//����ȷ�������װ��
	bool bExist = m_pElementInfoModule->ExistElement( strItemConfigID );
	if ( !bExist )
	{
		return -1;
	}

	int nItemType = m_pElementInfoModule->GetPropertyInt(strItemConfigID, NFrame::Item::ItemType());
    PackTableType eBagType = GetPackBagType(nItemType);
    if (eBagType != PackTableType::BagItemPack)
    {
        return -1;
    }

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::BagItemPack ) );
	if (!pRecord)
	{
		return -1;
	}

	for (int i = 0; i < pRecord->GetRows(); ++i)
	{
		if (pRecord->IsUsed(i))
		{
			const std::string& strCfgID = pRecord->GetString(i, NFrame::Player::BagItemList_ConfigID);
			if (strCfgID == strItemConfigID)
			{
				return i;
			}
		}
	}

	return -1;
}

bool NFCPackModule::DeleteEquip( const NFGUID& self, const NFGUID& id )
{
	if (id.IsNull())
	{
		return false;
	}

	//ɾ��3���ط���������Ӣ�۴�������Ҵ���
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
	if (nullptr == pObject)
	{
		return false;
	}


	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::BagEquipPack ) );
	if (nullptr == pRecord)
	{
		return false;
	}

	for (int i = 0; i < pRecord->GetRows(); ++i)
	{
		if (pRecord->IsUsed(i))
		{
			const NFGUID& xID = pRecord->GetObject(i, NFrame::Player::BagEquipList_GUID);
			if (xID == id)
			{
				const NFGUID& xWearID = pRecord->GetObject(i, NFrame::Player::BagEquipList_WearGUID);
				if (!xWearID.IsNull())
				{
					//�������װ��������Ӣ��װ��
					const std::string& strCfgID = pRecord->GetString(i, NFrame::Player::BagEquipList_ConfigID);

				}

				pRecord->Remove(i);
			}
		}
	}

	return true;
}

bool NFCPackModule::DeleteItem( const NFGUID& self, const std::string& strItemConfigID, const int nCount )
{
	if(nCount <= 0)
	{
		return false;
	}

	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
	if ( NULL == pObject )
	{
		return false;
	}

	//����ȷ�������װ��
	if (!m_pElementInfoModule->ExistElement(strItemConfigID))
	{
		return false;
	}

	int nItemType = m_pElementInfoModule->GetPropertyInt(strItemConfigID, NFrame::Item::ItemType());
    PackTableType eBagType = GetPackBagType(nItemType);
    if (eBagType != PackTableType::BagItemPack)
    {
        return false;
    }

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::BagItemPack ) );
	if (!pRecord)
	{
		return false;
	}

	int nRow = FindItemRowByConfig(self, strItemConfigID);
	if (nRow >= 0)
	{
		int nOldCount = pRecord->GetInt(nRow, NFrame::Player::BagItemList_ItemCount);
		int nNewCount = nOldCount - nCount;
		if (nNewCount >= 0)
		{
			if (nNewCount > 0)
			{
				pRecord->SetInt(nRow, NFrame::Player::BagItemList_ItemCount, nNewCount);
			}
			else
			{
				pRecord->Remove(nRow);
			}

			return true;
		}
	}

	return false;
}



bool NFCPackModule::EnoughItem( const NFGUID& self, const std::string& strItemConfigID, const int nCount )
{
    if(nCount <= 0)
    {
        return false;
    }

    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( NULL == pObject )
    {
        return false;
    }

    //����ȷ�������װ��
    bool bExist = m_pElementInfoModule->ExistElement( strItemConfigID );
    if ( !bExist )
    {
        return false;
    }

    int nItemType = m_pElementInfoModule->GetPropertyInt(strItemConfigID, NFrame::Item::ItemType());
    PackTableType eBagType = GetPackBagType(nItemType);
    if (eBagType != PackTableType::BagItemPack)
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::BagItemPack ) );
    if (!pRecord)
    {
        return false;
    }

    int nRow = FindItemRowByConfig(self, strItemConfigID);
    if (nRow >= 0)
    {
        int nOldCount = pRecord->GetInt(nRow, NFrame::Player::BagItemList_ItemCount);
        if (nCount >= nOldCount)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    return false;
}
