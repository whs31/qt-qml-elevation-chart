//
// Created by whs31 on 14.08.23.
//

#pragma once

#include <QtQuick/QQuickItem>

namespace SG
{
  class ScenegraphObject : public QQuickItem
  {
    Q_OBJECT

    public:
      explicit ScenegraphObject(QQuickItem* parent = nullptr);

    protected:
      virtual QSGNode* createNode();
      virtual void setupChildNodes(QSGNode* node) = 0;
      virtual void setupNodeColors(QSGNode* node) = 0;
      virtual void drawCall(QSGNode* node) = 0;

      [[nodiscard]] bool recolor() const;
      void requireRecolor();
      void fulfillRecolor();

    private:
      QSGNode* updatePaintNode(QSGNode* old_node, UpdatePaintNodeData*) override;

    private:
      bool m_require_recolor;
  };
} // SG
