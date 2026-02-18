#include "rainman/lua/CInheritTable.h"
#include <zlib.h>
#include "rainman/util/crc32_case_idt.h"
#include <algorithm>

extern "C"
{
    using ub4 = unsigned long int; /* unsigned 4-byte quantities */
    using ub1 = unsigned char;
    ub4 hash(ub1 *k, ub4 length, ub4 initval);
    ub4 hash3(ub1 *k, ub4 length, ub4 initval);
}

CInheritTable::CNode::CNode() : m_pParent(nullptr), m_pHashMapNext(nullptr), m_bIsNil(false) {}

void CInheritTable::CNode::setParent(CInheritTable::CNode *pParent)
{
    // Remove from old parent's child list
    if (m_pParent)
    {
        auto itr = std::find(m_pParent->m_vChildren.begin(), m_pParent->m_vChildren.end(), this);
        if (itr != m_pParent->m_vChildren.end())
        {
            m_pParent->m_vChildren.erase(itr);
        }
    }

    // Update parent
    m_pParent = pParent;

    // Add to new parent's child list
    if (m_pParent)
    {
        m_pParent->m_vChildren.push_back(this);
    }
}

void CInheritTable::CNode::print(FILE *f, int iL)
{
    for (int i = 0; i < iL; ++i)
        fputc(' ', f);
    fprintf(f, "%s (%s)\n", m_sMiniName.c_str(), m_sFullName.c_str());
    for (auto *pChild : m_vChildren)
    {
        pChild->print(f, iL + 1);
    }
}

void CInheritTable::CNode::setName(const char *sPath)
{
    m_sFullName = sPath;

    // Extract mini name (part after last backslash)
    auto pos = m_sFullName.rfind('\\');
    m_sMiniName = (pos != std::string::npos) ? m_sFullName.substr(pos + 1) : "";

    // Check for .nil extension and strip any extension from mini name
    auto dotPos = m_sMiniName.rfind('.');
    m_bIsNil = false;
    if (dotPos != std::string::npos)
    {
        m_bIsNil = (stricmp(m_sMiniName.c_str() + dotPos, ".nil") == 0);
        m_sMiniName.erase(dotPos);
    }
}

CInheritTable::CNode *CInheritTable::CNode::getParent() const { return m_pParent; }

const char *CInheritTable::CNode::getFullName() const { return m_sFullName.c_str(); }

const char *CInheritTable::CNode::getMiniName() const { return m_sMiniName.c_str(); }

size_t CInheritTable::CNode::getChildCount() const { return m_vChildren.size(); }

CInheritTable::CNode *CInheritTable::CNode::getChild(size_t i) const { return m_vChildren[i]; }

bool CInheritTable::CNode::getIsNil() const { return m_bIsNil; }

CInheritTable::CInheritTable()
{
    RAINMAN_LOG_DEBUG("CInheritTable — constructing inheritance table");
    m_pRootNode.reset(new CNode);
    m_pRootNode->setName("");
    m_mapNodes[crc32_case_idt(0, (const Bytef *)"", 0)] = m_pRootNode.get();
}

CInheritTable::~CInheritTable() = default;

CInheritTable::CNode *CInheritTable::getRoot() { return m_pRootNode.get(); }

CInheritTable::CNode *CInheritTable::findOrMake(const char *sPath)
{
    RAINMAN_LOG_TRACE("CInheritTable::findOrMake(\"{}\")", sPath ? sPath : "(null)");
    unsigned long iHash = crc32_case_idt(0, (const Bytef *)sPath, (uInt)strlen(sPath));
    CNode *pNode = m_mapNodes[iHash];
    CNode *pNode2 = pNode;

    // Attempt find
    while (pNode2)
    {
        if (stricmp(sPath, pNode2->m_sFullName.c_str()) == 0)
            return pNode2;
        pNode2 = pNode2->m_pHashMapNext;
    }

    // Make
    auto pNewNode = std::unique_ptr<CNode>(new CNode);
    pNewNode->setName(sPath);
    pNewNode->m_pHashMapNext = pNode;
    pNode2 = pNewNode.get();
    m_nodeStorage.push_back(std::move(pNewNode));
    m_mapNodes[iHash] = pNode2;
    return pNode2;
}

void CInheritTable::assignOrphansTo(CNode *pNode)
{
    RAINMAN_LOG_TRACE("CInheritTable::assignOrphansTo()");
    for (const auto &[iHash, pHead] : m_mapNodes)
    {
        if (iHash != 0)
        {
            CNode *pNode2 = pHead;
            while (pNode2)
            {
                if (pNode2->getParent() == nullptr)
                    pNode2->setParent(pNode);
                pNode2 = pNode2->m_pHashMapNext;
            }
        }
    }
}
