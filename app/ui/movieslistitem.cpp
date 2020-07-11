#include "movieslistitem.h"
#include "mainwindow.h"
#include "panel.h"
#include "global.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QFileInfo>
#include <QFile>
#include <QDesktopServices>

MoviesListItem::MoviesListItem(QTreeWidget *view, Movie *movie)
    : QTreeWidgetItem(view)
    , m_movie(movie)
{
}

void MoviesListItem::setSortData(int column, QVariant data)
{
    m_sortData[column] = data;
}

bool MoviesListItem::operator<(const QTreeWidgetItem &other) const
{
    int column = treeWidget()->sortColumn();
    const MoviesListItem *otherCasted = dynamic_cast<const MoviesListItem *>(&other);
    if (otherCasted) {
        if (m_sortData.contains(column) && otherCasted->m_sortData.contains(column)) {
            return m_sortData[column] < otherCasted->m_sortData[column];
        }
    }
    return text(column) < other.text(column);
}


