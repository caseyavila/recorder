#pragma once

#include <iostream>
#include <QtWidgets>

class EpgView : public QAbstractItemView {
  private:
    constexpr static int ROW_HEIGHT = 40;
    constexpr static int MINUTE_WIDTH = 5;
    constexpr static int HEADER_WIDTH = 40;
    constexpr static int HEADER_HEIGHT = 20;
    constexpr static int VIEWABLE_TIME = 4320 * MINUTE_WIDTH; /* 3 days */

    mutable std::vector<std::vector<int>> ends;
    mutable int max_end;

  public:
    EpgView(QWidget *parent = nullptr);

    void setModel(QAbstractItemModel *model) override;
    void scrollTo(const QModelIndex &index, QAbstractItemView::ScrollHint) override;
    QRect visualRect(const QModelIndex &index) const override;
    QModelIndex indexAt(const QPoint &point) const override;
    QModelIndex moveCursor(CursorAction, Qt::KeyboardModifiers) override;
    int horizontalOffset() const override;
    int verticalOffset() const override;
    bool isIndexHidden(const QModelIndex &index) const override;
    void setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command) override;
    QRegion visualRegionForSelection(const QItemSelection &selection) const override;

    void paintEvent(QPaintEvent *) override;
    void wheelEvent(QWheelEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;

    void paintOutline(QPainter *painter, const QRectF &rect);
    void setBars();
};
