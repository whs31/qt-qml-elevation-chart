//
// Created by whs31 on 14.08.23.
//

#include "scenegraphobject.h"
#include <QtQuick/QSGNode>

namespace SG
{
  ScenegraphObject::ScenegraphObject(QQuickItem* parent)
    : QQuickItem(parent)
    , m_require_recolor(false)
  {
    this->setFlag(ItemHasContents);
  }

  QSGNode* ScenegraphObject::updatePaintNode(QSGNode* old_node, QQuickItem::UpdatePaintNodeData* x)
  {
    (void)x;
    if(old_node == nullptr)
    {
      old_node = createNode();
      setupChildNodes(old_node);
    }

    if(recolor())
      setupNodeColors(old_node);

    drawCall(old_node);

    for(int i = 0; i < old_node->childCount(); i++)
      old_node->childAtIndex(i)->markDirty(recolor() ? (QSGNode::DirtyGeometry |  QSGNode::DirtyMaterial) : QSGNode::DirtyGeometry);

    old_node->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);
    return old_node;
  }

  QSGNode* ScenegraphObject::createNode() { return new QSGNode; }

  bool ScenegraphObject::recolor() const { return m_require_recolor; }
  void ScenegraphObject::requireRecolor() { m_require_recolor = true; this->update(); }
  void ScenegraphObject::fulfillRecolor() { m_require_recolor = false; }
} // SG