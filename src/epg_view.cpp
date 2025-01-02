#include "epg_view.hpp"

EpgView::EpgView(QWidget *parent) : QAbstractItemView(parent) { }

void EpgView::setModel(QAbstractItemModel *model) {
    QAbstractItemView::setModel(model);

    setBars();
    verticalScrollBar()->setSingleStep(ROW_HEIGHT);
    horizontalScrollBar()->setSingleStep(15 * MINUTE_WIDTH);
    setSelectionMode(QAbstractItemView::SingleSelection);
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

    if (itemRect.left() < viewRect.left()) {
        horizontalScrollBar()->setValue(horizontalOffset()
            + itemRect.left() - viewRect.left());
    } else if (itemRect.right() > viewRect.right()) {
        horizontalScrollBar()->setValue(horizontalOffset()
            + std::min(itemRect.right() - viewRect.right(),
                itemRect.left() - viewRect.left()));
    }

    if (itemRect.top() < viewRect.top()) {
        verticalScrollBar()->setValue(verticalOffset() +
           itemRect.top() - viewRect.top());
    } else if (itemRect.bottom() > viewRect.bottom()) {
        verticalScrollBar()->setValue(verticalOffset() +
            std::min(itemRect.bottom() - viewRect.bottom(),
                itemRect.top() - viewRect.top()));
    }

    viewport()->update();
}


QRegion EpgView::visualRegionForSelection(const QItemSelection &selection) const {
    QRegion region;

    for (const QItemSelectionRange &range : selection) {
        for (int row = range.top(); row <= range.bottom(); ++row) {
            for (int column = range.left(); column < range.right(); ++column) {
                region += visualRect(model()->index(row, column));
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

        rect = QRect(int_start * MINUTE_WIDTH - horizontalOffset(),
                     index.row() * ROW_HEIGHT - verticalOffset(),
                     duration * MINUTE_WIDTH,
                     ROW_HEIGHT);

        rect.moveRight(rect.right() + HEADER_WIDTH);
        rect.moveTop(rect.top() + HEADER_HEIGHT);
    }

    return rect;
}

void EpgView::wheelEvent(QWheelEvent *e) {
    verticalScrollBar()->setValue(verticalOffset() - e->angleDelta().y());
    horizontalScrollBar()->setValue(horizontalOffset() - e->angleDelta().x());
}

void EpgView::resizeEvent(QResizeEvent *e) {
    setBars();
}

void EpgView::paintEvent(QPaintEvent *e) {
    QAbstractItemView::paintEvent(e);

    QPainter painter(viewport());

    /* Can optimize */
    for (int row = 0; row < model()->rowCount(); ++row) {
        for (int column = 0; column < model()->columnCount(); ++column) {
            QModelIndex index = model()->index(row, column);

            QRect rect = visualRect(index);

            if (column == 0) {
                ends.push_back(std::vector<int>());
            }

            ends[row].push_back(rect.right());

            if (e->rect().intersects(rect)) {

                QStyleOptionViewItem option = viewOptions();
                option.rect = rect;

                if (selectionModel()->isSelected(index)) {
                    option.state |= QStyle::State_Selected;
                }

                itemDelegate()->paint(&painter, option, index);
                paintOutline(&painter, rect);
            }
        }
    }
}

void EpgView::paintOutline(QPainter *painter, const QRectF &rect) {
    painter->save();
    painter->setPen(QPen(Qt::gray));
    painter->drawRect(rect);
    painter->restore();
}


void EpgView::setBars() {
    verticalScrollBar()->setRange(0, model()->rowCount() * ROW_HEIGHT - viewport()->height());
    horizontalScrollBar()->setRange(0, VIEWABLE_TIME - viewport()->width());
}

QModelIndex EpgView::moveCursor(CursorAction action, Qt::KeyboardModifiers) {
    QModelIndex index = currentIndex();

    if (!index.isValid()) {
        return index;
    }

    if (action == MoveLeft || action == MoveRight) {
        const int offset = (action == MoveLeft ? -1 : 1);
        index = model()->index(index.row(), index.column() + offset);

        /* So we don't arrow key off-screen */
        if (action == MoveRight && ends[index.row()][index.column()] >
                horizontalScrollBar()->maximum() + viewport()->width()) {
            return currentIndex();
        }
    }

    return index;
}

void EpgView::setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command) {
    QModelIndex index = indexAt(rect.bottomRight());
    QItemSelection selection(index, index);
    selectionModel()->select(selection, command);
}

QModelIndex EpgView::indexAt(const QPoint &point) const {
    int row = (point.y() + verticalOffset() - HEADER_HEIGHT) / ROW_HEIGHT;
    int column = 0;

    while (ends[row][column] < point.x() + horizontalOffset()) {
        column++;
    }

    return model()->index(row, column);
}

bool EpgView::isIndexHidden(const QModelIndex &index) const {
    return false;
}
