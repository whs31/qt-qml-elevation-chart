//
// Created by whs31 on 14.08.23.
//

#pragma once

#include <map>
#include <QtQuick/QQuickItem>

using std::map;

namespace SG
{
  class ScenegraphObject : public QQuickItem
  {
    Q_OBJECT

    public:
      explicit ScenegraphObject(QQuickItem* parent = nullptr);

    protected:
      enum Node
      {
        Idx
      };

      virtual QSGNode* createNode();
      virtual void setupChildNodes(QSGNode* node) = 0;
      virtual void setupNodeColors(QSGNode* node) = 0;
      virtual void drawCall(QSGNode* node) = 0;

      [[nodiscard]] bool recolor() const;
      void requireRecolor();
      void fulfillRecolor();

    protected:
      map<Node, QSGNode*> tree;

    private:
      QSGNode* updatePaintNode(QSGNode* old_node, UpdatePaintNodeData*) override;

    private:
      bool m_require_recolor;
  };
} // SG
