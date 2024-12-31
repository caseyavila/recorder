#include "epg_view.hpp"

constexpr int ROW_HEIGHT = 30;

EpgView::EpgView(QWidget *parent) : QAbstractItemView(parent) { }

void EpgView::setModel(QAbstractItemModel *model) {
    QAbstractItemView::setModel(model);

    verticalScrollBar()->setRange(0, model->rowCount() * ROW_HEIGHT - viewport()->height());
    verticalScrollBar()->setSingleStep(ROW_HEIGHT);

    horizontalScrollBar()->setRange(0, 1000);
}

int EpgView::horizontalOffset() const {
    return horizontalScrollBar()->value();
}

int EpgView::verticalOffset() const {
    return verticalScrollBar()->value();
}


void EpgView::scrollTo(const QModelIndex &index,
                       QAbstractItemView::ScrollHint) {
    QRect viewRect = viewport()->rect();
    QRect itemRect = visualRect(index);

    if (itemRect.left() < viewRect.left())
        horizontalScrollBar()->setValue(horizontalOffset()
                + itemRect.left() - viewRect.left());
    else if (itemRect.right() > viewRect.right())
        horizontalScrollBar()->setValue(horizontalOffset()
                + qMin(itemRect.right() - viewRect.right(),
                       itemRect.left() - viewRect.left()));
    if (itemRect.top() < viewRect.top())
        verticalScrollBar()->setValue(verticalOffset() +
                itemRect.top() - viewRect.top());
    else if (itemRect.bottom() > viewRect.bottom())
        verticalScrollBar()->setValue(verticalOffset() +
                qMin(itemRect.bottom() - viewRect.bottom(),
                     itemRect.top() - viewRect.top()));
    viewport()->update();
}


QRegion EpgView::visualRegionForSelection(const QItemSelection &selection) const {
    QRegion region;

    for (const QItemSelectionRange &range : selection) {
        for (int row = range.top(); row <= range.bottom(); ++row) {
            for (int column = range.left(); column < range.right(); ++column) {
                QModelIndex index = model()->index(row, column);
                region += visualRect(index);
            }
        }
    }

    return region;
}


QRect EpgView::visualRect(const QModelIndex &index) const {
    QRect rect;

    if (index.isValid()) {
        int duration = model()->data(index, Qt::UserRole).toInt();
        int int_start = model()->data(index, Qt::UserRole + 1).toInt();

        rect = QRect(int_start * 5 - horizontalOffset(),
                     index.row() * ROW_HEIGHT - verticalOffset(),
                     duration * 5,
                     ROW_HEIGHT);
    }

    return rect;
}

void EpgView::wheelEvent(QWheelEvent *e) {
    verticalScrollBar()->setValue(verticalOffset() - e->angleDelta().y());
    horizontalScrollBar()->setValue(horizontalOffset() - e->angleDelta().x());
}

void EpgView::resizeEvent(QResizeEvent *e) {
    verticalScrollBar()->setRange(0, model()->rowCount() * ROW_HEIGHT - viewport()->height());
    horizontalScrollBar()->setRange(0, 1000);
}

void EpgView::paintOutline(QPainter *painter, const QRectF &rect) {
    painter->save();
    painter->setPen(QPen(Qt::gray));
    painter->drawRect(rect);
    painter->restore();
}

void EpgView::paintEvent(QPaintEvent *e) {
    QAbstractItemView::paintEvent(e);

    QPainter painter(viewport());

    for (int row = 0; row < model()->rowCount(); ++row) {
        for (int column = 0; column < model()->columnCount(); ++column) {
            QModelIndex index = model()->index(row, column);

            QRect rect = visualRect(index);

            if (e->rect().intersects(rect)) {
                QStyleOptionViewItem option = viewOptions();
                option.rect = rect;
                itemDelegate()->paint(&painter, option, index);

                paintOutline(&painter, rect);
            }
        }
    }
}

QModelIndex EpgView::moveCursor(CursorAction, Qt::KeyboardModifiers) {
    /* Probably want to change this ... */
    std::cout << "movecursor\n";
    return currentIndex();
}

void EpgView::setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command) {
    /* Probably want to change this ... */
    std::cout << "setSelection\n";
}

QModelIndex EpgView::indexAt(const QPoint &point) const {
    /* Probably want to change this ... */
    std::cout << "indexAt\n";
    return rootIndex();
}

bool EpgView::isIndexHidden(const QModelIndex &index) const {
    /* Probably want to change this ... */
    std::cout << "isindexhidden\n";
    return false;
}
